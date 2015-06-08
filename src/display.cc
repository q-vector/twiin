#include "display.h"

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;

Ffdi_Color_Chooser::Ffdi_Color_Chooser (const Real alpha)
   : alpha (alpha)
{
}

Color
Ffdi_Color_Chooser::get_color (const Real ffdi) const
{

   if (ffdi < 12)
   {
      const Real delta = ffdi;
      const Real r = 0.400 + delta * 0.03137;
      const Real g = 0.400 + delta * 0.04549;
      const Real b = 0.400 + delta * 0.03012;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 25)
   {
      const Real delta = ffdi - 10;
      const Real r = 0.278 + delta * 0.02133;
      const Real g = 0.400 + delta * 0.03075;
      const Real b = 0.500 + delta * 0.03859;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 50)
   {
      const Real delta = ffdi - 25;
      const Real r = 0.600 - delta * 0.02137;
      const Real g = 0.600 - delta * 0.02065;
      const Real b = 0.600 - delta * 0.02012;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 75)
   {
      const Real delta = ffdi - 50;
      const Real r = 0.549 + delta * 0.01757 * 2;
      const Real g = 0.549 + delta * 0.01757 * 2;
      const Real b = 0.329 + delta * 0.02133 * 2;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 100)
   {
      const Real delta = ffdi - 75;
      const Real r = 0.097 + delta * 0.01553 * 2;
      const Real g = 0.357 + delta * 0.00925 * 2;
      const Real b = 0.000 + delta * 0.00000 * 2;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 150)
   {
      const Real delta = ffdi - 100;
      const Real r = 0.647 + delta * 0.01396;
      const Real g = 0.000 + delta * 0.00000;
      const Real b = 0.000 + delta * 0.00000;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 200)
   {
      const Real delta = ffdi - 150;
      const Real r = 0.698 + delta * 0.01176;
      const Real g = 0.000 + delta * 0.00000;
      const Real b = 0.463 + delta * 0.00784;
      return Color (r, g, b, alpha);
   }
   else
   {
      return Color (0, 0, 0, 0);
   }

}

Raster*
Display::get_terrain_raster_ptr (const Size_2D& size_2d,
                                 const Transform_2D& transform,
                                 const Model& model,
                                 const Stage& stage)
{

   Raster* raster_ptr = new Raster (size_2d);
   Raster& raster = *raster_ptr;

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   Real orog, lsm;
   const Color transparent (0, 0, 0, 0);
   const Model::Terrain::Stage& terrain_stage = model.terrain.get_stage (stage);

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i);

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j);
         transform.reverse (latitude, longitude, x, y);

         if (terrain_stage.out_of_bounds (lat_long))
         {
            raster.set_pixel (i, j, transparent);
            continue;
         }

         const Real orog = terrain_stage.get_topography (i, j);
         const Real lsm = terrain_stage.evaluate (string ("lsm"), i, j);
         const Real h = std::min (std::max (orog / 2000.0, 0.0), 1.0);

         const bool land = (lsm > 0.5);
         const Real hue = (land ? 0.4 - h * 0.4 : 0.67);
         const Real brightness = h * 0.7 + 0.28;

         const Color& color = Color::hsb (hue, 0.34, brightness);
         raster.set_pixel (i, j, color);

      }

   }

   return raster_ptr;

}

Raster*
Display::get_surface_raster_ptr (const Size_2D& size_2d,
                                 const Transform_2D& transform,
                                 const Model& model,
                                 const Stage& stage,
                                 const Product& product,
                                 const Dtime& dtime)
{

   Raster* raster_ptr = new Raster (size_2d);
   Raster& raster = *raster_ptr;

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   const Color transparent (0, 0, 0, 0);
   const Model::Surface::Stage& surface_stage = model.surface.get_stage (stage);
   const size_t l = surface_stage.get_l (dtime);

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i);

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j);
         transform.reverse (latitude, longitude, x, y);

         if (surface_stage.out_of_bounds (lat_long))
         {
            raster.set_pixel (i, j, transparent);
            continue;
         }

         const Color& color = surface_stage.get_color (product, lat_long, l);
         raster.set_pixel (i, j, color);

      }

   }

   return raster_ptr;

}

Raster*
Display::get_uppers_raster_ptr (const Size_2D& size_2d,
                                const Transform_2D& transform,
                                const Model& model,
                                const Stage& stage,
                                const Product& product,
                                const Dtime& dtime,
                                const Level& level)
{

   Raster* raster_ptr = new Raster (size_2d);
   Raster& raster = *raster_ptr;

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   const Color transparent (0, 0, 0, 0);
   const Model::Uppers::Stage& uppers_stage = model.uppers.get_stage (stage);
   const size_t l = uppers_stage.get_l (dtime);

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i);

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j);
         transform.reverse (latitude, longitude, x, y);

         if (uppers_stage.out_of_bounds (lat_long))
         {
            raster.set_pixel (i, j, transparent);
            continue;
         }

         const Color& color = uppers_stage.get_color (
            product, lat_long, level, l);
         raster.set_pixel (i, j, color);

      }

   }

   return raster_ptr;

}

Color
Display::get_color (const Product& product,
                    const Real datum)
{

   const Color transparent (0, 0, 0, 0);

   if (product == "T")
   {
      const Real hue = Domain_1D (35 + K, 10 + K).normalize (datum) * 0.833;
      const Real brightness = (Integer (floor (datum)) % 2) ? 0.83:0.77;
      return Color::hsb (hue, 0.8, brightness);
   }
   else
   if (product == "TD")
   {
      const Real saturation = Domain_1D (-10+K, 20+K).normalize (datum);
      const Real brightness = (Integer (floor (datum)) % 2) ? 0.82:0.78;
      return Color::hsb (0.25, saturation, brightness);
   }
   else
   if (product == "RH")
   {
      const Real hue = (datum < 0.5 ? 0.08 : 0.35);
      const Real saturation = std::min ((fabs (datum - 0.5) * 2), 1.0);
      return Color::hsb (hue, saturation, 1, 0.4);
   }
   else
   if (product == "THETA")
   {
      const Real hue = Domain_1D (60+K, 0+K).normalize (datum) * 0.833;
      const Real brightness = (Integer (floor (datum)) % 2) ? 0.83:0.77;
      return Color::hsb (hue, 0.8, brightness);
   }
   else
   if (product == "THETA_E")
   {
      const Real saturation = Domain_1D (5+K, 65+K).normalize (datum);
      const Real brightness = (Integer (floor (datum)) % 2) ? 0.82:0.78;
      return Color::hsb (0.35, saturation, brightness);
   }
   else
   if (product == "Q")
   {
      const Real saturation = Domain_1D (0, 10).normalize (datum * 1e3);
      const Real brightness = (Integer (floor (datum * 1e3)) % 2) ? 0.82:0.78;
      return Color::hsb (0.45, saturation, brightness);
   }
   else
   if (product == "VORTICITY")
   {
      const Real hue = (datum < 0 ? 0.667 : 0.000);
      const Real modified_datum = (log10 (fabs (datum)) + 4) / 3;
      const Real saturation = std::max (std::min (modified_datum, 1.0), 0.0);
      return Color::hsb (hue, saturation, 1, 1);
   }
   else
   if (product == "FFDI")
   {
      const Ffdi_Color_Chooser ffdi_color_chooser (0.7);
      return ffdi_color_chooser.get_color (datum);
   }
   else
   if (product == "MSLP")
   {
      //const Real hue = Domain_1D (990e2, 1025e2).normalize (datum) * 0.833;
      const Real brightness = (Integer (floor (datum/200)) % 2) ? 0.82:0.78;
      return Color::hsb (0, 0, brightness);
   }
   else
   {
      return transparent;
   }

}

void
Display::render_stages (const RefPtr<Context>& cr,
                        const Transform_2D& transform,
                        const Station::Map& station_map)
{
}

void
Display::render_product (const RefPtr<Context>& cr,
                         const Transform_2D& transform,
                         const Size_2D& size_2d,
                         const Model& model,
                         const Product& product,
                         const Dtime& dtime,
                         const Level& level,
                         const twiin::Stage& stage)
{

   Raster* raster_ptr = NULL;

   if (product == "TERRAIN")
   {
      raster_ptr = get_terrain_raster_ptr (size_2d, transform, model, stage);
   }
   else
   if (product == "T" ||
       product == "P_THETA" ||
       product == "P_RHO" ||
       product == "TD" ||
       product == "RH" ||
       product == "THETA" ||
       product == "WIND" ||
       product == "VORTICITY" ||
       product == "THETA_E")
   {
      if (level.type == SURFACE_LEVEL)
      {
         raster_ptr = get_surface_raster_ptr (size_2d,
            transform, model, stage, product, dtime);
      }
      else
      if (level.type == HEIGHT_LEVEL)
      {
         raster_ptr = get_uppers_raster_ptr (size_2d,
            transform, model, stage, product, dtime, level);
      }
   }
   else
   if (product == "FFDI" ||
       product == "MSLP")
   {
      raster_ptr = get_surface_raster_ptr (
         size_2d, transform, model, stage, product, dtime);
   }
   else
   if (product == "Q")
   {
      raster_ptr = get_uppers_raster_ptr (size_2d,
         transform, model, stage, product, dtime, level);
   }

   if (raster_ptr != NULL) { raster_ptr->blit (cr); }
   delete raster_ptr;

}

void
Display::render_wind_barbs (const RefPtr<Context>& cr,
                            const Transform_2D& transform,
                            const Size_2D& size_2d,
                            const Model& model,
                            const Dtime& dtime,
                            const Level& level,
                            const Stage& stage)
{

   Point_2D point;
   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

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

   for (point.x = start_x; point.x < width; point.x += h)
   {
      for (point.y = start_y; point.y < height; point.y += h)
      {

         transform.reverse (latitude, longitude, point.x, point.y);
         if (model.out_of_bounds (lat_long, stage)) { continue; }

         Real u = model.evaluate (U, lat_long, level, dtime, stage);
         Real v = model.evaluate (V, lat_long, level, dtime, stage);
         transform.transform_uv (u, v, latitude, longitude);
         if (gsl_isnan (u) || gsl_isnan (v)) { continue; }

         const bool nh = (latitude > 0);
         const Wind_Barb wind_barb (Wind (u, v), wind_barb_size, nh, nan, ct);
         wind_barb.cairo (cr, point);
         cr->fill ();

      }
   }

}

void
Display::render (const RefPtr<Context>& cr,
                 const Transform_2D& transform,
                 const Size_2D& size_2d,
                 const Model& model,
                 //const Station::Map& station_map,
                 const Dtime& dtime,
                 const Level& level,
                 const Stage& stage,
                 const Product product)
{

   cr->save ();

   Color (0.86, 0.85, 0.47).cairo (cr);
   cr->paint();

   render_product (cr, transform, size_2d, model,product, dtime, level, stage);
   render_wind_barbs (cr, transform, size_2d, model, dtime, level, stage);

   // Stage 3/4/5 Frames
   //render_stages (cr, transform, station_map);

   // All Stations
   //station_map.cairo (cr, transform);

   cr->restore ();

}

