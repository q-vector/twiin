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
                                 const Stage& stage);

         static Raster*
         get_surface_raster_ptr (const Size_2D& size_2d,
                                 const Transform_2D& transform,
                                 const Model& model,
                                 const Stage& stage,
                                 const Product& product,
                                 const Dtime& dtime);

         static Raster*
         get_uppers_raster_ptr (const Size_2D& size_2d,
                                const Transform_2D& transform,
                                const Model& model,
                                const Stage& stage,
                                const Product& product,
                                const Dtime& dtime,
                                const Level& level);

         static Raster*
         get_cross_section_raster_ptr (const Box_2D& box_2d,
                                       const Transform_2D& transform,
                                       const Model& model,
                                       const Stage& stage,
                                       const Product& product,
                                       const Dtime& dtime,
                                       const Multi_Journey& multi_journey);

      public:

         static Color
         get_color (const Product& product,
                    const Real datum);

         static void
         render_stages (const RefPtr<Context>& cr,
                        const Transform_2D& transform,
                        const Station::Map& station_map);

         static void
         render_product (const RefPtr<Context>& cr,
                         const Transform_2D& transform,
                         const Size_2D& size_2d,
                         const Model& model,
                         const Product& product,
                         const Dtime& dtime,
                         const Level& level,
                         const twiin::Stage& stage);

         static void
         render_wind_barbs (const RefPtr<Context>& cr,
                            const Transform_2D& transform,
                            const Size_2D& size_2d,
                            const Model& model,
                            const Dtime& dtime,
                            const Level& level,
                            const Stage& stage);

         static void
         render (const RefPtr<Context>& cr,
                 const Transform_2D& transform,
                 const Size_2D& size_2d,
                 const Model& model,
                 //const Station::Map& station_map,
                 const Dtime& dtime,
                 const Level& level,
                 const twiin::Stage& stage,
                 const Product product);

         static Raster*
         render_cross_section (const RefPtr<Context>& cr,
                               const Transform_2D& transform,
                               const Box_2D& box_2d,
                               const Domain_1D& domain_z,
                               const Model& model,
                               const Stage& stage,
                               const Product& product,
                               const Dtime& dtime,
                               const Multi_Journey& multi_journey);

         static Raster*
         render_cross_section_arrows (const RefPtr<Context>& cr,
                                      const Transform_2D& transform,
                                      const Box_2D& box_2d,
                                      const Model& model,
                                      const Stage& stage,
                                      const Product& product,
                                      const Dtime& dtime,
                                      const Multi_Journey& multi_journey);

   };

};

#endif /* DISPLAY_H */
