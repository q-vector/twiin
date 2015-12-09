#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <map>
#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <denise/geodesy.h>
#include <denise/nwp.h>
#include "display.h"
#include "data.h"
#include "hrit.h"

using namespace std;
using namespace denise;
using namespace Cairo;

namespace twiin
{

   class Display
   {

      public:

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
                    const Dtime& dtime,
                    const Level& level);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
                    const Dtime& dtime,
                    const Journey& journey);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
                    const Dtime& dtime,
                    const Lat_Long& lat_long,
                    const Real distance,
                    const bool lagrangian);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
                    const Dstring& track_id,
                    const bool lagrangian);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Dtime& dtime,
                    const Location& location);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Dtime& dtime,
                    const Dstring& location_str);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Location& location);

         static void
         render_stages (const RefPtr<Context>& cr,
                        const Geodetic_Transform& transform,
                        const Model& model);

         static void
         render_product (const RefPtr<Context>& cr,
                         const Geodetic_Transform& transform,
                         const Size_2D& size_2d,
                         const Model& model,
                         const Hrit& hrit,
                         const Model::Product& product,
                         const Dtime& dtime,
                         const Level& level,
                         const Dstring& stage_str);

         static void
         render_wind_barbs (const RefPtr<Context>& cr,
                            const Geodetic_Transform& transform,
                            const Size_2D& size_2d,
                            const Model& model,
                            const Dtime& dtime,
                            const Level& level,
                            const Dstring& stage_str);

         static void
         render_scale_bar (const RefPtr<Context>& cr,
                           const Geodetic_Transform& transform,
                           const Size_2D& size_2d,
                           const Dstring& scale_bar_str = "lb:10");

         static void
         render_color_bar (const RefPtr<Context>& cr,
                           const Model::Product& product,
                           const Tuple& tick_tuple,
                           const Box_2D& box_2d,
                           const bool negative = false);

         static void
         render_color_bar (const RefPtr<Context>& cr,
                           const Size_2D& size_2d,
                           const Model::Product& product,
                           const Dstring& color_bar_str = "r:10");

         static void
         render_annotation_point (const RefPtr<Context>& cr,
                                  const Geodetic_Transform& transform,
                                  const Lat_Long& lat_long,
                                  const Dstring& str,
                                  const char h_align = 'h',
                                  const char v_align = 'c');

         static void
         render_annotation (const RefPtr<Context>& cr,
                            const Geodetic_Transform& transform,
                            const Dstring& annotation_str,
                            const Station::Map& station_map);

         static void
         render_annotations (const RefPtr<Context>& cr,
                             const Geodetic_Transform& transform,
                             const Tokens& annotation_tokens,
                             const Station::Map& station_map);

         static void
         render (const RefPtr<Context>& cr,
                 const Geodetic_Transform& geodetic_transform,
                 const Size_2D& size_2d,
                 const Model& model,
                 const Hrit& hrit,
                 const Station::Map& station_map,
                 const Dtime& dtime,
                 const Level& level,
                 const Dstring& stage_str,
                 const Model::Product product,
                 const bool no_stage,
                 const bool no_wind_barb);

         static void
         render_cross_section_w (const RefPtr<Context>& cr,
                                 const Transform_2D& transform,
                                 const Box_2D& box_2d,
                                 const Model::Stage& stage,
                                 const Dtime& dtime,
                                 const Journey& journey);

         static void
         render_cross_section_mesh (const RefPtr<Context>& cr,
                                    const Transform_2D& transform,
                                    const Domain_1D& domain_z,
                                    const Journey& journey);

         static void
         render_cross_section_arrows (const RefPtr<Context>& cr,
                                      const Transform_2D& transform,
                                      const Box_2D& box_2d,
                                      const Model::Stage& stage,
                                      const Model::Product& product,
                                      const Dtime& dtime,
                                      const Journey& journey,
                                      const Real u_bg = 0);

         static void
         render_cross_section (const RefPtr<Context>& cr,
                               const Transform_2D& transform,
                               const Box_2D& box_2d,
                               const Domain_1D& domain_z,
                               const Model::Stage& stage,
                               const Model::Product& product,
                               const Dtime& dtime,
                               const Journey& journey,
                               const Real u_bg = 0);

         static void
         render_time_cross_w (const RefPtr<Context>& cr,
                              const Transform_2D& transform,
                              const Box_2D& box_2d,
                              const Model::Stage& stage,
                              const Track& track);

         static void
         render_time_cross_mesh (const RefPtr<Context>& cr,
                                 const Transform_2D& transform,
                                 const Domain_1D& domain_t,
                                 const Domain_1D& domain_z);

         static void
         render_time_cross_track (const RefPtr<Context>& cr,
                                  const Transform_2D& transform,
                                  const Track& track);

         static void
         render_time_cross (const RefPtr<Context>& cr,
                            const Transform_2D& transform,
                            const Box_2D& box_2d,
                            const Domain_1D& domain_z,
                            const Model::Stage& stage,
                            const Model::Product& product,
                            const Track& track,
                            const bool lagrangian = false);

         static void
         render_meteogram_mesh (const RefPtr<Context>& cr,
                                const Domain_1D& domaint_t,
                                const Domain_1D& domain_temperature,
                                const Domain_1D& domain_direction,
                                const Domain_1D& domain_speed,
                                const Domain_1D& domain_pressure,
                                const Transform_2D& transform_temperature,
                                const Transform_2D& transform_direction,
                                const Transform_2D& transform_speed,
                                const Transform_2D& transform_pressure,
                                const bool ignore_pressure);

         static void
         render_meteogram (const RefPtr<Context>& cr,
                           const Transform_2D& transform_temperature,
                           const Transform_2D& transform_direction,
                           const Transform_2D& transform_speed,
                           const Transform_2D& transform_pressure,
                           const Aws::Repository& aws_repository,
                           const Real alpha,
                           const Real ring_size,
                           const bool fill,
                           const bool ignore_pressure);

         static void
         render_meteogram (const RefPtr<Context>& cr,
                           const Size_2D& size_2d,
                           const Model::Stage& stage,
                           const Aws::Repository& aws_repository,
                           const Location& location,
                           const Dstring& time_str,
                           const bool ignore_pressure,
                           const bool no_nwp);

         static void
         render_vertical_profile (const RefPtr<Context>& cr,
                                  const Thermo_Diagram& thermo_diagram,
                                  const Model::Stage& stage,
                                  const Dtime& dtime,
                                  const Lat_Long& lat_long,
                                  const Level& level = Level (Level::NAL));

         static void
         render_vertical_profile (const RefPtr<Context>& cr,
                                  const Thermo_Diagram& thermo_diagram,
                                  const Model::Stage& stage,
                                  const Dtime& dtime,
                                  const Lat_Long::List& lat_long_list);

   };

};

#endif /* DISPLAY_H */
