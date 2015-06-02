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

         const Station::Map
         station_map;

         const Model
         model;

         void
         render_product (const Product& product,
                         const RefPtr<Context>& cr,
                         const Transform_2D& transform,
                         const Size_2D& size_2d,
                         const Dtime& dtime,
                         const Level& level,
                         const twiin::Stage& stage) const;

         void
         render_wind_barbs (const RefPtr<Context>& cr,
                            const Transform_2D& transform,
                            const Size_2D& size_2d,
                            const Dtime& dtime,
                            const Level& level,
                            const Stage& stage) const;

      public:

         Display (const string& station_file_path,
                  const string& model_config_file_path);

         ~Display ();

         const Model&
         get_model () const;

         void
         cairo (const RefPtr<Context>& cr,
                const Transform_2D& transform,
                const Size_2D& size_2d,
                const Dtime& dtime,
                const Level& level,
                const twiin::Stage& stage,
                const Product product) const;

   };

};

#endif /* DISPLAY_H */
