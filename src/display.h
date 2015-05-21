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

         const Terrain
         terrain;

         const Station::Map
         station_map;

         const Model
         model;

         void
         render_model (const Product& product,
                       const RefPtr<Context>& cr,
                       const Transform_2D& transform,
                       const Size_2D& size_2d,
                       const Dtime& dtime,
                       const twiin::Stage& stage) const;

         void
         render_wind_barbs (const RefPtr<Context>& cr,
                            const Transform_2D& transform,
                            const Size_2D& size_2d,
                            const Dtime& dtime,
                            const Stage& stage) const;

      public:

         Display (const string& orog_3_file_path,
                  const string& lsm_3_file_path,
                  const string& orog_4_file_path,
                  const string& lsm_4_file_path,
                  const string& orog_5_file_path,
                  const string& lsm_5_file_path,
                  const string& station_file_path,
                  const std::map<Model::Varname, string>& mdoel_file_path_3_map,
                  const std::map<Model::Varname, string>& mdoel_file_path_4_map,
                  const std::map<Model::Varname, string>& mdoel_file_path_5_map);

         ~Display ();

         const Model&
         get_model () const;

         void
         cairo (const RefPtr<Context>& cr,
                const Transform_2D& transform,
                const Size_2D& size_2d,
                const Dtime& dtime,
                const twiin::Stage& stage,
                const Product product) const;

   };

};

#endif /* DISPLAY_H */
