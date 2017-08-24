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
Hrit::init (const Dstring& data_path)
{

   this->data_path = data_path;

   const Reg_Exp re ("DK01_[0-9]...........");
   const Tokens& dir_listing = get_dir_listing (data_path, re);

   for (auto l = dir_listing.begin (); l != dir_listing.end (); l++)
   {

      const Dstring& leaf = *(l);
      const Dstring& time_str = leaf.substr (5, 11) + "2";
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

Hrit::File::File (const Dstring& file_path)
   : ifstream (file_path.get_string ())
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
                    const Dstring& leaf)
   : hrit (hrit)
{

   const Reg_Exp re ("DK01_[0-9]+");

   const Dstring& time_str = leaf.substr (5, 11) + "2";

   for (auto c = hrit.channel_tokens.begin ();
        c != hrit.channel_tokens.end (); c++)
   {

      const Dstring& channel = *(c);
      Tokens file_path_tokens;

      for (Integer s = 1; s <= 10; s++)
      {
         const Dstring& prefix = hrit.get_data_path () +
            "/" + leaf + "/IMG_DK01";
         const Dstring& segment_str = Dstring::render ("_%03d", s);
         const Dstring& file_path = prefix +
            channel + "_" + time_str + segment_str;
         file_path_tokens.push_back (file_path);
      }

      insert (make_pair (channel, file_path_tokens));

   }

}

const Dstring&
Hrit::get_data_path () const
{
   return data_path;
}

Dstring
Hrit::Frame::get_file_path (const Dstring& channel,
                            const Integer segment) const
{
   return at (channel).at (segment);
}

Geos_Transform 
Hrit::Frame::get_navigation (const Dstring& channel) const
{
   Hrit::File file (at (channel).at (0));
   return file.get_navigation ();
}

Hrit::Disk::Disk (const Frame& frame,
                  const Dstring& channel)
{

   for (Integer segment = 0; segment < 10; segment++)
   {
      const Dstring& file_path = frame.get_file_path (channel, segment);
      push_back (new Hrit::File (file_path));
   }

   this->segment_size = front ()->get_segment_size ();

}

/*
Hrit::Disk::Disk (const Disk& disk)
{

   for (auto iterator = disk.begin (); iterator != disk.end (); iterator++)
   {
      Hrit::File* file_ptr = *(iterator);
      push_back (file_ptr);
   }

   this->segment_size = disk.segment_size;

}
*/

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

Hrit::Disk_Ptr_Map::~Disk_Ptr_Map ()
{
   for (auto iterator = begin (); iterator != end (); iterator++)
   {
      Hrit::Disk* disk_ptr = iterator->second;
      delete disk_ptr;
   }
}

Hrit::Hrit (const Dstring& data_path)
   : channel_tokens ("IR1 IR2 IR3 IR4 VIS")
{
   init (data_path);
}

Hrit::Hrit (const Rc_File& rc_file)
   : channel_tokens ("IR1 IR2 IR3 IR4 VIS")
{

   for (auto iterator = rc_file.begin ();
        iterator != rc_file.end (); iterator++)
   {

      const Dstring input_line = *(iterator);
      const Tokens tokens (input_line, " \t\n");
      if (tokens.size () != 2) { continue; }
      if (tokens[0] != "hrit") { continue; }

      const Dstring& data_path = tokens[1];
      init (data_path);
      break;

   }

}

Dstring
Hrit::get_file_path (const Dtime& dtime,
                     const Dstring& channel,
                     const Integer segment) const
{
   return frame_map.get_frame (dtime).at (channel).at (segment);
}

set<Dtime>
Hrit::get_valid_time_set () const
{

   set<Dtime> valid_time_set;

   for (auto l = frame_map.begin (); l != frame_map.end (); l++)
   {
      const Dtime& dtime = l->first;
      valid_time_set.insert (dtime);
   }

   return valid_time_set;

}

vector<Dtime>
Hrit::get_valid_time_vector (const Dtime::Set& time_set) const
{

   vector<Dtime> valid_time_vector;
   auto valid_time_set = get_valid_time_set ();

   for (auto l = valid_time_set.begin (); l != valid_time_set.end (); l++)
   {
      const Dtime& dtime = *(l);
      if (time_set.match (dtime)) { valid_time_vector.push_back (dtime); }
   }

   return valid_time_vector;

}

Geos_Transform
Hrit::get_navigation (const Dtime& dtime,
                      const Dstring& channel) const
{
   Hrit::File file (frame_map.get_frame (dtime).at (channel).at (0));
   return file.get_navigation ();
}

map<Dstring, Geos_Transform>
Hrit::get_navigation_map (const Dtime& dtime) const
{

   map<Dstring, Geos_Transform> navigation_map;

   for (auto c = channel_tokens.begin ();
        c != channel_tokens.end (); c++)
   {
      const Dstring& channel = *(c);
      const Geos_Transform& navigation = get_navigation (dtime, channel);
      navigation_map.insert (make_pair (channel, navigation));
   }

   return navigation_map;

}

Hrit::Disk*
Hrit::get_disk_ptr (const Dtime& dtime,
                const Dstring& channel) const
{
   const Hrit::Frame& frame = frame_map.get_frame (dtime);
   return new Hrit::Disk (frame, channel);
}

Hrit::Disk_Ptr_Map
Hrit::get_disk_ptr_map (const Dtime& dtime) const
{

   Hrit::Disk_Ptr_Map disk_ptr_map;

   for (auto c = channel_tokens.begin ();
        c != channel_tokens.end (); c++)
   {
      const Dstring& channel = *(c);
      Hrit::Disk* disk_ptr = get_disk_ptr (dtime, channel);
      disk_ptr_map.insert (make_pair (channel, disk_ptr));
   }

   return disk_ptr_map;

}

uint16_t
Hrit::get_datum (Hrit::Disk& disk,
                 const Geos_Transform& navigation,
                 const Lat_Long& lat_long)
{

   Real x, y;

   navigation.t (x, y, lat_long.latitude, lat_long.longitude);
   const Integer line = Integer (round (y));
   const Integer element = Integer (round (x));

   return disk.get_datum (line, element);

}

Color
Hrit::get_color (const Dstring& hrit_product,
                 Disk_Ptr_Map& disk_ptr_map,
                 const map<Dstring, Geos_Transform>& navigation_map,
                 const Lat_Long& lat_long)
{

   const Integer max_index = 1024;
   //const Integer max_index = (channel == "VIS" ? 1024 : 1024);

   if (hrit_product == "IR1" ||
       hrit_product == "IR2" ||
       hrit_product == "IR4" ||
       hrit_product == "VIS")
   {

      const Dstring channel (hrit_product);

      const auto& navigation = navigation_map.at (channel);
      auto& disk = *(disk_ptr_map.at (channel));

      const auto datum = Hrit::get_datum (disk, navigation, lat_long);
      const Real brightness = Real (datum) / max_index;
      return Color::hsb (0, 0, brightness);

   }
   else
   if (hrit_product == "IR3")
   {

      Enhancement_Wvjl3 enhancement;

      const auto& navigation = navigation_map.at ("IR3");
      auto& disk = *(disk_ptr_map.at ("IR3"));

      const auto datum = Hrit::get_datum (disk, navigation, lat_long);
      return enhancement.get_color (datum);

   }
   else
   if (hrit_product == "Pseudo")
   {

      const auto& navigation_ir1 = navigation_map.at ("IR1");
      const auto& navigation_vis = navigation_map.at ("VIS");
      auto& disk_ir1 = *(disk_ptr_map.at ("IR1"));
      auto& disk_vis = *(disk_ptr_map.at ("VIS"));

      const auto ir1 = Hrit::get_datum (disk_ir1, navigation_ir1, lat_long);
      const auto vis = Hrit::get_datum (disk_vis, navigation_vis, lat_long);
      const Real brightness_ir1 = Real (ir1) / max_index;
      const Real brightness_vis = Real (vis) / max_index;
      return Color (brightness_vis, brightness_vis, brightness_ir1);

   }
   else
   {
      return Color::transparent ();
   }

}

Raster*
Hrit::get_raster_ptr (const Size_2D& size_2d,
                      const Transform_2D& transform,
                      const Dstring& channel_str,
                      const Dtime& dtime) const
{
   const Integer max_index = (channel_str == "VIS" ? 1024 : 1024);

   auto* raster_ptr = new Raster (size_2d);
   auto& raster = *raster_ptr;

   const auto& navigation_map = get_navigation_map (dtime);
   auto disk_ptr_map = get_disk_ptr_map (dtime);

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i);

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j);
         transform.r (latitude, longitude, x, y);

         const Color& color = Hrit::get_color (channel_str,
            disk_ptr_map, navigation_map, lat_long);
         //const Color& color = enhancement.get_color (raw_datum);
         raster.set_pixel (i, j, color);

      }

   }

   return raster_ptr;

}

