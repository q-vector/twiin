#ifndef HRIT_H
#define HRIT_H

#include <fstream>
#include <denise/util.h>
#include <denise/geodesy.h>

using namespace std;
using namespace denise;

class Hrit
{

   private:

      Dstring
      data_path;

      void
      init (const Dstring& data_path);

   public:

      const Tokens
      channel_tokens;

      class File : public ifstream
      {

         private:

            Size_2D
            disk_size;

            uint32_t
            get_uint32_t (const uint32_t address,
                          const uint8_t size);

            uint32_t
            get_header_size ();

            uint32_t
            get_data_size ();

         public:

            File (const Dstring& file_path);

            ~File ();

            Geos_Transform
            get_navigation ();

            Size_2D
            get_disk_size ();

            uint16_t
            get_segment_size ();

            char*
            get_data_buffer ();

            uint16_t
            get_datum (const Integer relative_line,
                       const Integer element);

      };

      class Frame : public map<Dstring, Tokens>
      {

         private:

            const Hrit&
            hrit;

         public:

            class Map : public map<Dtime, Frame>
            {

               public:

                  Frame&
                  get_frame (const Dtime& dtime);

                  const Frame&
                  get_frame (const Dtime& dtime) const;

            };

            Frame (const Hrit& hrit,
                   const Dstring& leaf);

            Dstring
            get_file_path (const Dstring& channel,
                           const Integer segment) const;

            Geos_Transform
            get_navigation (const Dstring& channel) const;

      };

      class Disk : public vector<Hrit::File*>
      {

         private:

            uint16_t
            segment_size;

         public:

            Disk (const Frame& frame,
                  const Dstring& channel);

            //Disk (const Disk& disk);

            ~Disk ();

            uint16_t
            get_datum (const Integer line,
                       const Integer element);

      };

      class Disk_Ptr_Map : public map<Dstring, Disk*>
      {

         public:

            ~Disk_Ptr_Map ();

      };

      Frame::Map
      frame_map;

      Hrit (const Dstring& data_path);

      Hrit (const Config_File& config_file);

      const Dstring&
      get_data_path () const;

      Dstring
      get_file_path (const Dtime& dtime,
                     const Dstring& channel,
                     const Integer segment) const;

      Geos_Transform
      get_navigation (const Dtime& dtime,
                      const Dstring& channel) const;

      map<Dstring, Geos_Transform>
      get_navigation_map (const Dtime& dtime) const;

      Disk*
      get_disk_ptr (const Dtime& dtime,
                    const Dstring& channel) const;

      Disk_Ptr_Map
      get_disk_ptr_map (const Dtime& dtime) const;

      static uint16_t
      get_datum (Disk& disk,
                 const Geos_Transform& navigation,
                 const Lat_Long& lat_long);

      static Color
      get_color (const Dstring& hrit_product,
                 Disk_Ptr_Map& disk_ptr_map,
                 const map<Dstring, Geos_Transform>& navigation_map,
                 const Lat_Long& lat_long);

};

#endif /* HRIT_H */
