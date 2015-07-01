#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <denise/geodesy.h>
#include "display.h"

using namespace std;
using namespace denise;
using namespace Cairo;

namespace twiin
{

   class Twiin
   {

      private:

         class Transform_Ptr_Map : public map<string, Geodetic_Transform*>
         {

            public:

               Transform_Ptr_Map (const Size_2D& size_2d,
                                  const Tokens& config_file_content);

               ~Transform_Ptr_Map ();

         };

         const Size_2D
         size_2d;

         const Tokens
         config_file_content;

         const string
         output_dir;

         string
         get_png_file_path (const Stage& stage,
                            const Product& product,
                            const Dtime& dtime) const;

         string
         get_png_file_path (const Stage& stage,
                            const Product& product,
                            const Dtime& dtime,
                            const Multi_Journey& multi_journey) const;

         string
         get_png_file_path (const Stage& stage,
                            const Dtime& dtime,
                            const Lat_Long& lat_long) const;

         string
         get_png_file_path (const Stage& stage,
                            const Lat_Long& lat_long) const;

      public:

         Twiin (const Size_2D& size_2d,
                const Tokens& config_file_content,
                const string& output_dir);

#ifndef ENABLE_GTKMM
#else /* ENABLE_GTKMM */
         void
         interactive (const string& stage_str,
                      const string& product_str,
                      const string& level_str,
                      const string& time_str) const;
#endif /* ENABLE_GTKMM */

         void
         command_line (const string& stage_str,
                       const string& product_str,
                       const string& level_str,
                       const string& time_str,
                       const bool is_bludge) const;

         void
         cross_section (const string& stage_str,
                        const string& product_str,
                        const Multi_Journey& multi_journey,
                        const string& time_str,
                        const bool is_bludge) const;

         void
         meteogram (const string& stage_str,
                    const Lat_Long& lat_long,
                    const string& time_str,
                    const bool is_bludge) const;

         void
         vertical_profile (const string& stage_str,
                           const Lat_Long& lat_long,
                           const string& time_str,
                           const bool is_bludge) const;

   };

};

#endif /* MAIN_H */
