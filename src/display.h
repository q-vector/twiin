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

   class Ffdi_Color_Chooser : public Color_Chooser
   {

      private:

         Real
         alpha;

      public:

         Ffdi_Color_Chooser (const Real alpha = 1.0);

         Color
         get_color (const Real ffdi) const;

   };

   class Display
   {

      private:

         static Raster*
         get_terrain_raster_ptr (const Size_2D& size_2d,
                                 const Transform_2D& transform,
                                 const Model& model,
                                 const Dstring& stage);

         static Raster*
         get_surface_raster_ptr (const Size_2D& size_2d,
                                 const Transform_2D& transform,
                                 const Model& model,
                                 const Dstring& stage,
                                 const Model::Product& product,
                                 const Dtime& dtime);

         static Raster*
         get_uppers_raster_ptr (const Size_2D& size_2d,
                                const Transform_2D& transform,
                                const Model& model,
                                const Dstring& stage,
                                const Model::Product& product,
                                const Dtime& dtime,
                                const Level& level);

         static Raster*
         get_hrit_raster_ptr (const Size_2D& size_2d,
                              const Transform_2D& transform,
                              const Hrit& hrit,
                              const Model::Product& product,
                              const Dtime& dtime);

         static Raster*
         get_cross_section_raster_ptr (const Box_2D& box_2d,
                                       const Transform_2D& transform,
                                       const Model& model,
                                       const Dstring& stage,
                                       const Model::Product& product,
                                       const Dtime& dtime,
                                       const Journey& journey,
                                       const Real u_bg = 0);

      public:

         static void
         set_title_tokens (Title& title,
                           const Tokens& title_tokens);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage,
                    const Model::Product& product,
                    const Dtime& dtime,
                    const Level& level);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage,
                    const Model::Product& product,
                    const Dtime& dtime,
                    const Journey& journey);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage,
                    const Dtime& dtime,
                    const Location& location);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage,
                    const Dtime& dtime,
                    const Dstring& location_str);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage,
                    const Location& location);

         static Tuple
         get_tick_tuple (const Model::Product& product);

         static Color
         get_wind_color (const Real u,
                         const Real v);

         static Color
         get_color (const Model::Product& product,
                    const Real datum);

         static Color
         get_color (const Model::Product& product,
                    const Real datum,
                    const Dstring& unit);

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
                         const Dstring& stage);

         static void
         render_wind_barbs (const RefPtr<Context>& cr,
                            const Geodetic_Transform& transform,
                            const Size_2D& size_2d,
                            const Model& model,
                            const Dtime& dtime,
                            const Level& level,
                            const Dstring& stage);

         static void
         render_scale_bar (const RefPtr<Context>& cr,
                           const Geodetic_Transform& transform,
                           const Size_2D& size_2d);

         static void
         render_color_bar (const RefPtr<Context>& cr,
                           const Model::Product& product,
                           const Tuple& tick_tuple,
                           const Box_2D& box_2d,
                           const bool negative = false);

         static void
         render_color_bar (const RefPtr<Context>& cr,
                           const Size_2D& size_2d,
                           const Model::Product& product);

         static void
         render_annotation_point (const RefPtr<Context>& cr,
                                  const Geodetic_Transform& transform,
                                  const Lat_Long& lat_long,
                                  const Dstring& str);

         static void
         render_annotation (const RefPtr<Context>& cr,
                            const Geodetic_Transform& transform,
                            const Dstring& annotation_str);

         static void
         render_annotations (const RefPtr<Context>& cr,
                             const Geodetic_Transform& transform,
                             const Tokens& annotation_tokens);

         static void
         render (const RefPtr<Context>& cr,
                 const Geodetic_Transform& geodetic_transform,
                 const Size_2D& size_2d,
                 const Model& model,
                 const Hrit& hrit,
                 const Station::Map& station_map,
                 const Dtime& dtime,
                 const Level& level,
                 const Dstring& stage,
                 const Model::Product product,
                 const bool no_stage,
                 const bool no_wind_barb);

         static void
         render_cross_section_w (const RefPtr<Context>& cr,
                                 const Transform_2D& transform,
                                 const Box_2D& box_2d,
                                 const Model& model,
                                 const Dstring& stage,
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
                                      const Model& model,
                                      const Dstring& stage,
                                      const Model::Product& product,
                                      const Dtime& dtime,
                                      const Journey& journey,
                                      const Real u_bg = 0);

         static void
         render_cross_section (const RefPtr<Context>& cr,
                               const Transform_2D& transform,
                               const Box_2D& box_2d,
                               const Domain_1D& domain_z,
                               const Model& model,
                               const Dstring& stage,
                               const Model::Product& product,
                               const Dtime& dtime,
                               const Journey& journey,
                               const Real u_bg = 0);

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
                           const bool faint,
                           const bool ignore_pressure);

         static void
         render_meteogram (const RefPtr<Context>& cr,
                           const Size_2D& size_2d,
                           const Model& model,
                           const Aws::Repository& aws_repository,
                           const Dstring& stage,
                           const Location& location,
                           const bool ignore_pressure);

         static void
         render_vertical_profile (const RefPtr<Context>& cr,
                                  const Thermo_Diagram& thermo_diagram,
                                  const Model& model,
                                  const Dstring& stage,
                                  const Dtime& dtime,
                                  const Lat_Long& lat_long);

         static void
         render_vertical_profile (const RefPtr<Context>& cr,
                                  const Thermo_Diagram& thermo_diagram,
                                  const Model& model,
                                  const Dstring& stage,
                                  const Dtime& dtime,
                                  const Lat_Long::List& lat_long_list);

   };

};

#endif /* DISPLAY_H */
