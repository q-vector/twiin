#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <denise/gis.h>
#include "enhancement.h"
#include "hrit.h"

using namespace std;
using namespace denise;
using namespace Cairo;

void
Hrit::init (const string& data_path)
{

   this->data_path = data_path;

   const Reg_Exp re ("DK01_[0-9]+");
   const Tokens& dir_listing = get_dir_listing (data_path, re);

   for (auto l = dir_listing.begin ();
        l != dir_listing.end (); l++)
   {

      const string& leaf = *(l);
      const string& time_str = leaf.substr (5, 11) + "2";
      const Dtime dtime (time_str);

      frame_map.insert (make_pair (dtime, Frame (*this, leaf)));

   }

}

uint32_t
Hrit::File::get_uint32_t (const uint32_t address,
                          const uint8_t size)
{

   seekg (address, ios::beg);

   char* buffer = new char[size];
   read (buffer, size);
#ifndef WORDS_BIGENDIAN
   swap_endian (buffer, size);
#endif

   uint32_t datum = *((uint32_t*)buffer);
   delete[] buffer;

   return datum;

}

uint32_t
Hrit::File::get_header_size ()
{
   return get_uint32_t (4, 4);
}

uint32_t
Hrit::File::get_data_size ()
{
   return get_uint32_t (12, 4) / 8;
}

Hrit::File::File (const string& file_path)
   : ifstream (file_path)
{
   disk_size.i = get_uint32_t (16 + 6, 2);
   disk_size.j = get_uint32_t (16 + 4, 2);
}

Hrit::File::~File ()
{
   close ();
}

Geos_Transform
Hrit::File::get_navigation ()
{

   char longitude_cstr[] = "000.00";
   seekg (25 + 8, ios::beg);
   read (longitude_cstr, 6);
   const Real longitude = atof (longitude_cstr);

   const Real cfac = get_uint32_t (25 + 35, 4);
   const Real lfac = get_uint32_t (25 + 39, 4);
   const Real coff = get_uint32_t (25 + 43, 4);
   const Real loff = get_uint32_t (25 + 47, 4);

   return Geos_Transform (longitude, coff, loff, cfac, lfac);

}

Size_2D
Hrit::File::get_disk_size ()
{
   const uint16_t nc = get_uint32_t (16 + 4, 2);
   const uint16_t nl = get_uint32_t (16 + 6, 2);
   return Size_2D (nl, nc);
}

uint16_t
Hrit::File::get_segment_size ()
{
   const uint16_t nl = get_uint32_t (16 + 6, 2);
   return nl;
}

char*
Hrit::File::get_data_buffer ()
{

   const size_t address = get_header_size ();
   const uint32_t buffer_size = get_data_size ();

   seekg (address, ios::beg);
   char* buffer = new char[buffer_size];
   read (buffer, buffer_size);

   return buffer;

}

uint16_t
Hrit::File::get_datum (const Integer relative_line,
                       const Integer element)
{

   const size_t header_size = get_header_size ();
   const size_t position = 2 * (relative_line * disk_size.j + element);
   const size_t address = header_size + position;

   uint16_t datum;
   seekg (address, ios::beg);
   read ((char*)(&datum), sizeof (datum));
#ifndef WORDS_BIGENDIAN
   swap_endian (&datum, sizeof (datum));
#endif

   return datum;

}

Hrit::Frame&
Hrit::Frame::Map::get_frame (const Dtime& dtime)
{

   Dtime closest_time;
   Real min_dt = GSL_POSINF;

   for (auto iterator = begin (); iterator != end (); iterator++)
   {
      const Dtime& t = iterator->first;
      const Real dt = fabs (dtime.t - t.t);
      if (dt < min_dt)
      {
         min_dt = dt;
         closest_time = t;
      }
   }

   return at (closest_time);

}

const Hrit::Frame&
Hrit::Frame::Map::get_frame (const Dtime& dtime) const
{

   Dtime closest_time;
   Real min_dt = GSL_POSINF;

   for (auto iterator = begin (); iterator != end (); iterator++)
   {
      const Dtime& t = iterator->first;
      const Real dt = fabs (dtime.t - t.t);
      if (dt < min_dt)
      {
         min_dt = dt;
         closest_time = t;
      }
   }

   return at (closest_time);

}

Hrit::Frame::Frame (const Hrit& hrit,
                    const string& leaf)
   : hrit (hrit)
{

   const Reg_Exp re ("DK01_[0-9]+");

   const string& time_str = leaf.substr (5, 11) + "2";

   for (auto c = hrit.channel_tokens.begin ();
        c != hrit.channel_tokens.end (); c++)
   {

      const string& channel = *(c);
      Tokens file_path_tokens;

      for (Integer s = 1; s <= 10; s++)
      {
         const string& prefix = hrit.get_data_path () + "/" + leaf + "/IMG_DK01";
         const string& segment_str = string_render ("_%03d", s);
         const string& file_path = prefix + channel + "_" + time_str + segment_str;
         file_path_tokens.push_back (file_path);
      }

      insert (make_pair (channel, file_path_tokens));

   }

}

const string&
Hrit::get_data_path () const
{
   return data_path;
}

string
Hrit::Frame::get_file_path (const string& channel,
                            const Integer segment) const
{
   return at (channel).at (segment);
}

Geos_Transform 
Hrit::Frame::get_navigation (const string& channel) const
{
   Hrit::File file (at (channel).at (0));
   return file.get_navigation ();
}

Hrit::Disk::Disk (const Frame& frame,
                  const string& channel)
{

   for (Integer segment = 0; segment < 10; segment++)
   {
      const string& file_path = frame.get_file_path (channel, segment);
      push_back (new Hrit::File (file_path));
   }

   this->segment_size = front ()->get_segment_size ();

}

Hrit::Disk::~Disk ()
{
   for (auto f = begin (); f != end (); f++)
   {
      Hrit::File* file_ptr = *(f);
      delete file_ptr;
   }

}

uint16_t
Hrit::Disk::get_datum (const Integer line,
                       const Integer element)
{

   const Integer segment = line / segment_size;
   const Integer relative_line = line % segment_size;

   Hrit::File& file = *(at (segment));
   return file.get_datum (relative_line, element);

}

Hrit::Hrit (const string& data_path)
   : channel_tokens ("IR1 IR2 IR3 IR4 VIS")
{
   init (data_path);
}

Hrit::Hrit (const Tokens& config_file_content)
   : channel_tokens ("IR1 IR2 IR3 IR4 VIS")
{

   for (auto iterator = config_file_content.begin ();
        iterator != config_file_content.end (); iterator++)
   {

      const Tokens tokens (*(iterator));
      if (tokens.size () != 2) { continue; }
      if (tokens[0] != "hrit") { continue; }

      const string& data_path = tokens[1];
      init (data_path);
      break;

   }

}

string
Hrit::get_file_path (const Dtime& dtime,
                     const string& channel,
                     const Integer segment) const
{
   return frame_map.get_frame (dtime).at (channel).at (segment);
}

Geos_Transform
Hrit::get_navigation (const Dtime& dtime,
                      const string& channel) const
{
   Hrit::File file (frame_map.get_frame (dtime).at (channel).at (0));
   return file.get_navigation ();
}

map<string, Geos_Transform>
Hrit::get_navigation_map (const Dtime& dtime) const
{

   map<string, Geos_Transform> navigation_map;

   for (auto c = channel_tokens.begin ();
        c != channel_tokens.end (); c++)
   {
      const string& channel = *(c);
      const Geos_Transform& navigation = get_navigation (dtime, channel);
      navigation_map.insert (make_pair (channel, navigation));
   }

   return navigation_map;

}

Hrit::Disk
Hrit::get_disk (const Dtime& dtime,
                const string& channel) const
{
   const Hrit::Frame& frame = frame_map.get_frame (dtime);
   return Hrit::Disk (frame, channel);
}

uint16_t
Hrit::get_datum (const Dtime& dtime,
                 const string& channel,
                 const Geos_Transform& navigation,
                 const Lat_Long& lat_long) const
{

   Real x, y;

   navigation.transform (x, y, lat_long.latitude, lat_long.longitude);
   const Integer line = Integer (round (y));
   const Integer element = Integer (round (x));

   Hrit::Disk disk = get_disk (dtime, channel);
   return disk.get_datum (line, element);

}

