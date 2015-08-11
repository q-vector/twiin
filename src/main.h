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

         class Transform_Ptr_Map : public map<Dstring, Geodetic_Transform*>
         {

            public:

               Transform_Ptr_Map (const Size_2D& size_2d,
                                  const Config_File& config_file);

               ~Transform_Ptr_Map ();

         };

         const Size_2D
         size_2d;

         const Config_File
         config_file;

         const Dstring
         output_dir;

         Dstring
         get_file_path (const Dstring& format,
                        const Stage& stage,
                        const Product& product,
                        const Level& level,
                        const Dtime& dtime) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Stage& stage,
                        const Product& product,
                        const Dtime& dtime,
                        const Journey& journey) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Stage& stage,
                        const Dtime& dtime,
                        const Location& location) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Stage& stage,
                        const Dtime& dtime,
                        const Dstring& location_name) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Stage& stage,
                        const Location& location) const;

      public:

         Twiin (const Size_2D& size_2d,
                const Config_File& config_file,
                const Dstring& output_dir);

#ifndef ENABLE_GTKMM
#else /* ENABLE_GTKMM */
         void
         interactive (const Dstring& stage_str,
                      const Dstring& product_str,
                      const Dstring& level_str,
                      const Dstring& time_str) const;
#endif /* ENABLE_GTKMM */

         void
         command_line (const Dstring& stage_str,
                       const Dstring& product_str,
                       const Dstring& level_str,
                       const Dstring& time_str,
                       const Dstring& zoom_str,
                       const Tokens& annotation_tokens,
                       const Dstring& format,
                       const Tokens& title_tokens,
                       const Dstring& filename,
                       const bool no_stage,
                       const bool no_wind_barb,
                       const bool is_bludge) const;

         void
         cross_section (const Dstring& stage_str,
                        const Dstring& product_str,
                        const Journey& journey,
                        const Dstring& time_str,
                        const Dstring& format,
                        const Tokens& title_tokens,
                        const Dstring& filename,
                        const Real u_bg,
                        const bool is_bludge) const;

         void
         meteogram (const Dstring& stage_str,
                    const Dstring& location_str,
                    const Dstring& time_str,
                    const Dstring& format,
                    const Tokens& title_tokens,
                    const Dstring& filename,
                    const bool ignore_pressure,
                    const bool is_bludge) const;

         void
         vertical_profile (const Dstring& stage_str,
                           const Dstring& location_str,
                           const Dstring& time_str,
                           const Dstring& format,
                           const Tokens& title_tokens,
                           const Dstring& filename,
                           const bool is_bludge) const;

   };

};

#endif /* MAIN_H */
