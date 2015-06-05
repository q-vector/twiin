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

      public:

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

   };

};

#endif /* DISPLAY_H */
