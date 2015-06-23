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

      string
      data_path;

      void
      init (const string& data_path);

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

            File (const string& file_path);

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

      class Frame : public map<string, Tokens>
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
                   const string& leaf);

            string
            get_file_path (const string& channel,
                           const Integer segment) const;

            Geos_Transform
            get_navigation (const string& channel) const;

      };

      class Disk : public vector<Hrit::File*>
      {

         private:

            uint16_t
            segment_size;

         public:

            Disk (const Frame& frame,
                  const string& channel);

            //Disk (const Disk& disk);

            ~Disk ();

            uint16_t
            get_datum (const Integer line,
                       const Integer element);

      };

      class Disk_Ptr_Map : public map<string, Disk*>
      {

         public:

            ~Disk_Ptr_Map ();

      };

      Frame::Map
      frame_map;

      Hrit (const string& data_path);

      Hrit (const Tokens& config_file_content);

      const string&
      get_data_path () const;

      string
      get_file_path (const Dtime& dtime,
                     const string& channel,
                     const Integer segment) const;

      Geos_Transform
      get_navigation (const Dtime& dtime,
                      const string& channel) const;

      map<string, Geos_Transform>
      get_navigation_map (const Dtime& dtime) const;

      Disk*
      get_disk_ptr (const Dtime& dtime,
                    const string& channel) const;

      Disk_Ptr_Map
      get_disk_ptr_map (const Dtime& dtime) const;

      static uint16_t
      get_datum (Disk& disk,
                 const Geos_Transform& navigation,
                 const Lat_Long& lat_long);

      static Color
      get_color (const string& hrit_product,
                 Disk_Ptr_Map& disk_ptr_map,
                 const map<string, Geos_Transform>& navigation_map,
                 const Lat_Long& lat_long);

};

#endif /* HRIT_H */
