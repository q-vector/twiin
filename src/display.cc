#include "display.h"

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;

void
Display::render_model (const Product& product,
                       const RefPtr<Context>& cr,
                       const Transform_2D& transform,
                       const Size_2D& size_2d,
                       const Dtime& dtime,
                       const Level& level,
                       const twiin::Stage& stage) const
{
   Raster* raster_ptr = model.get_raster_ptr (product,
      dtime, size_2d, transform, level, stage);
   raster_ptr->blit (cr);
   delete raster_ptr;
}

void
Display::render_wind_barbs (const RefPtr<Context>& cr,
                            const Transform_2D& transform,
                            const Size_2D& size_2d,
                            const Dtime& dtime,
                            const Level& level,
                            const Stage& stage) const
{

   Point_2D point;
   Real latitude, longitude;

   const Real wind_barb_size = 15;
   const Real h = wind_barb_size * 1.01;

   const Real start_x = Real (0);
   const Real start_y = Real (0);
   const Real width = Real (size_2d.i);
   const Real height = Real (size_2d.j);

   const Real calm_threshold = 2;
   const Real ct = calm_threshold;
   const Real nan = GSL_NAN;

   const Real hue = 0.0;
   const Color wind_barb_color (0.0, 0.0, 0.0, 0.5);
   wind_barb_color.cairo (cr);

   const Real z = level.value;
   const Model::Stage& model_stage = model.get_model_stage (stage);

   for (point.x = start_x; point.x < width; point.x += h)
   {
      for (point.y = start_y; point.y < height; point.y += h)
      {

         transform.reverse (latitude, longitude, point.x, point.y);
         if (model_stage.out_of_bounds (latitude, longitude)) { continue; }

         Real u = model.evaluate (ZONAL_WIND, latitude,
            longitude, z, dtime, stage);
         Real v = model.evaluate (MERIDIONAL_WIND, latitude,
            longitude, z, dtime, stage);
         transform.transform_uv (u, v, latitude, longitude);
         if (gsl_isnan (u) || gsl_isnan (v)) { continue; }

         const bool nh = (latitude > 0);
         const Wind_Barb wind_barb (Wind (u, v), wind_barb_size, nh, nan, ct);
         wind_barb.cairo (cr, point);
         cr->fill ();

      }
   }

}

Display::Display (const string& station_file_path,
                  const string& model_config_file_path)
   : station_map (station_file_path),
     model (model_config_file_path)
{
}

Display::~Display ()
{
}

const Model&
Display::get_model () const
{
   return model;
}

void
Display::cairo (const RefPtr<Context>& cr,
                const Transform_2D& transform,
                const Size_2D& size_2d,
                const Dtime& dtime,
                const Level& level,
                const Stage& stage,
                const Product product) const
{

   cr->save ();

   Color (0.86, 0.85, 0.47).cairo (cr);
   cr->paint();

   if (product == "TERRAIN")
   {
      model.terrain.cairo (cr, transform, size_2d, stage);
   }
   else
   {
//      model.terrain.cairo (cr, transform, size_2d, stage);
      render_model (product, cr, transform, size_2d, dtime, level, stage);
   }

   render_wind_barbs (cr, transform, size_2d, dtime, level, stage);

   // Stage 3/4/5 Frames
   station_map.render_stages (cr, transform);

   // All Stations
   //station_map.cairo (cr, transform);

   cr->restore ();

}

