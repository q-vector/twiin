#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <denise/geodesy.h>
#include <andrea/andrea.h>
#include "display.h"

using namespace std;
using namespace denise;
using namespace andrea;
using namespace Cairo;

namespace twiin
{

   class Twiin : public Andrea
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

         Track_Map
         trajectory_map;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Model::Product& product,
                        const Level& level,
                        const Dtime& dtime) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Model::Product& product,
                        const Dtime& dtime,
                        const Journey& journey) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Dtime& dtime,
                        const Location& location) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Dtime& dtime,
                        const Dstring& location_name) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Location& location) const;

      public:

         Twiin (const Size_2D& size_2d,
                const Config_File& config_file,
                const Dstring& output_dir);

#ifndef ENABLE_GTKMM
#else /* ENABLE_GTKMM */
         void
         gui (const Dstring& stage_str,
              const Dstring& product_str,
              const Dstring& level_str,
              const Dstring& time_str) const;
#endif /* ENABLE_GTKMM */

         void
         plan (const Dstring& stage_str,
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

         void
         twiin_trajectory_generate (const Dstring& identifier,
                                    const Dstring& stage_str,
                                    const Lat_Long& lat_long,
                                    const Level& level,
                                    const Dtime& dtime,
                                    const Real finish_tau,
                                    const Tokens& arguments);

         void
         twiin_trajectory_ingest (const Dstring& file_path);

         void
         twiin_trajectory_write (const Dstring& file_path) const;

         void
         twiin_trajectory_print (const Dstring& identifier) const;

         void
         twiin_trajectory (const Tokens& tokens);

         void
         twiin_surface_plan (const Dstring& surface_identifier,
                             const Dstring& geodetic_transform_identifier,
                             const Dstring& stage,
                             const Model::Product& product,
                             const Level& level,
                             const Dtime& dtime,
                             const Tokens& arguments);

         void
         twiin_surface (const Tokens& tokens);

         void
         twiin_parse (const Tokens& tokens);

         void
         parse (const Tokens& tokens);

   };

};

#endif /* MAIN_H */
