#include "display.h"
#include "hrit.h"

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
      const Real delta = ffdi - 12;
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
      const Real r = 0.549 + delta * 0.01757;
      const Real g = 0.549 + delta * 0.01757;
      const Real b = 0.329 + delta * 0.02133;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 100)
   {
      const Real delta = ffdi - 75;
      const Real r = 0.097 + delta * 0.01553;
      const Real g = 0.357 + delta * 0.00925;
      const Real b = 0.000 + delta * 0.00000;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 150)
   {
      const Real delta = ffdi - 100;
      const Real r = 0.647 + delta * 0.00706;
      const Real g = 0.000 + delta * 0.00000;
      const Real b = 0.000 + delta * 0.00000;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 200)
   {
      const Real delta = ffdi - 150;
      const Real r = 0.698 + delta * 0.00604;
      const Real g = 0.000 + delta * 0.00000;
      const Real b = 0.463 + delta * 0.00040;
      return Color (r, g, b, alpha);
   }
   else
   {
      return Color::transparent ();
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
   const auto& terrain_stage = model.terrain.get_stage (stage);

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i);

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j);
         transform.reverse (latitude, longitude, x, y);

         if (terrain_stage.out_of_bounds (lat_long))
         {
            raster.set_pixel (i, j, Color::transparent ());
            continue;
         }

         const Real orog = terrain_stage.get_topography (lat_long);
         const Real lsm = terrain_stage.evaluate (string ("lsm"), lat_long);
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

   const auto& surface_stage = model.surface.get_stage (stage);
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
            raster.set_pixel (i, j, Color::transparent ());
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

   const auto& uppers_stage = model.uppers.get_stage (stage);
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
            raster.set_pixel (i, j, Color::transparent ());
            continue;
         }

         const Color& color = uppers_stage.get_color (
            product, lat_long, level, l);
         raster.set_pixel (i, j, color);

      }

   }

   return raster_ptr;

}

Raster*
Display::get_hrit_raster_ptr (const Size_2D& size_2d,
                              const Transform_2D& transform,
                              const Hrit& hrit,
                              const Product& product,
                              const Dtime& dtime)
{

   const string channel (product);
   const Integer max_index = (channel == "VIS" ? 1024 : 1024);

   auto* raster_ptr = new Raster (size_2d);
   auto& raster = *raster_ptr;

   const auto& navigation_map = hrit.get_navigation_map (dtime);
   auto disk_ptr_map = hrit.get_disk_ptr_map (dtime);

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i);

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j);
         transform.reverse (latitude, longitude, x, y);

         const Color& color = Hrit::get_color (string (product),
            disk_ptr_map, navigation_map, lat_long);
         //const Color& color = enhancement.get_color (raw_datum);
         raster.set_pixel (i, j, color);

      }

   }

   return raster_ptr;

}

Raster*
Display::get_cross_section_raster_ptr (const Box_2D& box_2d,
                                       const Transform_2D& transform,
                                       const Model& model,
                                       const Stage& stage,
                                       const Product& product,
                                       const Dtime& dtime,
                                       const Multi_Journey& multi_journey)
{

   Raster* raster_ptr = new Raster (box_2d);

   Color color;
   const auto& terrain_stage = model.terrain.get_stage (stage);
   const auto& uppers_stage = model.uppers.get_stage (stage);
   const size_t l = uppers_stage.get_l (dtime);

   Real x;
   Level level (HEIGHT_LEVEL, GSL_NAN);
   Real& z = level.value;

   const Geodesy geodesy;
   const Index_2D& index_2d = box_2d.index_2d;
   const Size_2D& size_2d = box_2d.size_2d;
   const Real distance = multi_journey.get_distance (geodesy);

   for (Integer i = index_2d.i; i < index_2d.i + size_2d.i; i++)
   {

      transform.reverse (x, z, Real (i), 0);
      if (x < 0 || x > distance) { continue; }
      const Lat_Long& lat_long = multi_journey.get_lat_long (x, geodesy);
      const Lat_Long& ll = lat_long;
      if (model.out_of_bounds (lat_long, stage)) { continue; }

      const Real topography = terrain_stage.get_topography (lat_long);

      for (Integer j = index_2d.j; j < index_2d.j + size_2d.j; j++)
      {
         transform.reverse (x, z, Real (i), Real (j));
         if (z < topography) { color = Color::black (); }
         else
         {
            if (product == "WIND")
            {
               const Real u = uppers_stage.evaluate (U, lat_long, z, l);
               const Real v = uppers_stage.evaluate (V, lat_long, z, l);
               const Real speed = sqrt (u*u + v*v);
               const Real theta = atan2 (-u, -v);
               const Real hue = (theta < 0 ? theta + 2*M_PI : theta)/ (2*M_PI);
               const Real brightness = std::min (speed / 15, 1.0);
               color = Color::hsb (hue, 0.8, brightness);
            }
            else
            {
               const Nwp_Element nwp_element = product.get_nwp_element ();
               const Real datum = uppers_stage.evaluate (nwp_element, ll, z, l);
               color = Display::get_color (product, datum);
            }
         }
         raster_ptr->set_pixel (i - index_2d.i, j - index_2d.j, color);
      }

   }

   return raster_ptr;

}

void
Display::set_title (Title& title,
                    const Stage& stage,
                    const Product& product,
                    const Dtime& dtime,
                    const Level& level)
{
   const string& time_str = dtime.get_string ("%Y.%m.%d %H:%M UTC");
   title.set (time_str, "", product, stage, level.get_string ());
}

void
Display::set_title (Title& title,
                    const Stage& stage,
                    const Product& product,
                    const Dtime& dtime,
                    const Multi_Journey& multi_journey)
{

   const Lat_Long origin (multi_journey.front ());
   const Lat_Long destination (multi_journey.back ());
   const bool complex_mj = (multi_journey.size () > 2);

   const string o_suffix (complex_mj ? " ..." : "");
   const string d_preffix (complex_mj ? "... " : "");
   const string& o_str = origin.get_string (3) + o_suffix;
   const string& d_str = d_preffix + destination.get_string (3);

   const string& time_str = dtime.get_string ("%Y.%m.%d %H:%M UTC");

   title.set (time_str, o_str, product, stage, d_str);

}

void
Display::set_title (Title& title,
                    const Stage& stage,
                    const Dtime& dtime,
                    const Lat_Long& lat_long)
{

   const string& ll_str = lat_long.get_string (3);
   const string& time_str = dtime.get_string ("%Y.%m.%d %H:%M UTC");

   title.set (time_str, "", ll_str, stage, "");

}

Color
Display::get_color (const Product& product,
                    const Real datum)
{

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
      //const Real saturation = std::min ((fabs (datum - 0.5) * 2), 1.0);
      //return Color::hsb (hue, saturation, 1, 0.4);
      const Real saturation = fabs (datum - 0.5) * 0.8;
      return Color::hsb (hue, saturation, 1);
   }
   else
   if (product == "THETA")
   {
      const Real hue = Domain_1D (60+K, 0+K).normalize (datum) * 0.833;
      const Real brightness = (Integer (floor ((datum-K) / 2)) % 2) ? 0.83:0.77;
      return Color::hsb (hue, 0.8, brightness);
   }
   else
   if (product == "THETA_E")
   {
      const Real saturation = Domain_1D (5+K, 65+K).normalize (datum);
      const Real brightness = (Integer (floor ((datum-K) / 4)) % 2) ? 0.82:0.78;
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
   if (product == "RHO")
   {
      const Real jump = 1.10;
      const Real deviate = datum - jump;
      const Real x = atan (deviate / 0.05);
      const Real b = Domain_1D (M_PI_2, -M_PI_2).normalize (x) * 0.85 + 0.15;
      const Real s = exp (-fabs (deviate) / 0.01) * 0.8 + 0.2;
      return Color::hsb (0.800, s, b);
   }
   else
   if (product == "W")
   {
      const Real hue = (datum < 0 ? 0.667 : 0.000);
      const Real absolute = fabs (datum);
      const Real quantized = floor (absolute * 10) / 10;
      const Real saturation = Domain_1D (0, 1.5).normalize (quantized) * 0.7;
      return Color::hsb (hue, saturation, 1.0);
   }
   else
   if (product == "W_TRANSLUCENT")
   {
      const Real hue = (datum < 0 ? 0.667 : 0.000);
      const Real absolute = fabs (datum);
      const Real quantized = floor (absolute * 10) / 10;
      const Real alpha = Domain_1D (0, 1.5).normalize (quantized) * 0.7;
      return Color::hsb (hue, 1.0, 1.0, alpha);
   }
   else
   if (product == "SPEED")
   {
      const Real knots = datum * 3.6/1.852;
           if (knots <  5) { return Color::hsb (0.000, 0.00, 1.00); }
      else if (knots < 10) { return Color::hsb (0.167, 0.30, 1.00); }
      else if (knots < 15) { return Color::hsb (0.150, 0.35, 0.95); }
      else if (knots < 20) { return Color::hsb (0.133, 0.40, 0.90); }
      else if (knots < 25) { return Color::hsb (0.333, 0.40, 0.95); }
      else if (knots < 30) { return Color::hsb (0.333, 0.40, 0.75); }
      else if (knots < 35) { return Color::hsb (0.333, 0.40, 0.65); }
      else if (knots < 40) { return Color::hsb (0.600, 0.40, 1.00); }
      else if (knots < 45) { return Color::hsb (0.633, 0.40, 0.90); }
      else if (knots < 50) { return Color::hsb (0.667, 0.40, 0.80); }
      else if (knots < 55) { return Color::hsb (0.033, 0.40, 0.65); }
      else if (knots < 60) { return Color::hsb (0.016, 0.45, 0.80); }
      else if (knots < 65) { return Color::hsb (0.000, 0.50, 0.95); }
      else                 { return Color::transparent (); }
   }
   else
   if (product == "VORTICITY")
   {
      const Real hue = (datum < 0 ? 0.667 : 0.000);
      const Real modified_datum = (log10 (fabs (datum)) + 4) / 3;
      const Real saturation = std::max (std::min (modified_datum, 1.0), 0.0);
      //const Real saturation = Domain_1D (0, 1).normalize (modified_datum);
      return Color::hsb (hue, saturation, 1, 1);
   }
   else
   if (product == "FFDI")
   {
      const Ffdi_Color_Chooser ffdi_color_chooser (1.0);
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
   if (product == "PRECIP_RATE")
   {
      const Real mmhr = datum * 3600;
           if (mmhr < 0.1) { return Color::hsb (0.000, 0.00, 1.0); }
      else if (mmhr <   1) { return Color::hsb (0.167, 0.60, 1.0); }
      else if (mmhr <   2) { return Color::hsb (0.333, 0.60, 1.0); }
      else if (mmhr <   5) { return Color::hsb (0.333, 0.60, 0.8); }
      else if (mmhr <  10) { return Color::hsb (0.333, 0.60, 0.6); }
      else if (mmhr <  20) { return Color::hsb (0.333, 0.60, 0.4); }
      else if (mmhr <  30) { return Color::hsb (0.600, 0.60, 1.0); }
      else if (mmhr <  50) { return Color::hsb (0.633, 0.60, 0.8); }
      else if (mmhr <  75) { return Color::hsb (0.666, 0.60, 0.6); }
      else if (mmhr < 100) { return Color::hsb (0.083, 0.60, 1.0); }
      else if (mmhr < 150) { return Color::hsb (0.042, 0.60, 1.0); }
      else                 { return Color::hsb (0.000, 0.60, 1.0); }
   }
   else
   {
      return Color::transparent ();
   }

}

void
Display::render_stages (const RefPtr<Context>& cr,
                        const Transform_2D& transform,
                        const Model& model)
{

   cr->save ();

   cr->set_line_width (4);
   Color::hsb (0.00, 0.00, 0.00, 0.5).cairo (cr);
   const Tokens stage_tokens ("STAGE3 STAGE4 STAGE5");

   for (auto iterator = stage_tokens.begin ();
        iterator != stage_tokens.end (); iterator++)
   {
      const Stage& stage = *(iterator);
      const Domain_2D& domain_2d = model.get_domain_2d (stage);
      Polygon (domain_2d).cairo (cr, transform);
      cr->stroke ();
   }

   cr->restore ();

}

void
Display::render_product (const RefPtr<Context>& cr,
                         const Transform_2D& transform,
                         const Size_2D& size_2d,
                         const Model& model,
                         const Hrit& hrit,
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
       product == "Q" ||
       product == "THETA" ||
       product == "RHO" ||
       product == "WIND" ||
       product == "SPEED" ||
       product == "W" ||
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
       product == "MSLP" ||
       product == "PRECIP_RATE")
   {
      raster_ptr = get_surface_raster_ptr (
         size_2d, transform, model, stage, product, dtime);
   }
   else
   if (product == "IR1" ||
       product == "IR2" ||
       product == "IR3" ||
       product == "IR4" ||
       product == "VIS" ||
       product == "Pseudo")
   {
      raster_ptr = get_hrit_raster_ptr (
         size_2d, transform, hrit, product, dtime);
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
   const Lat_Long& ll = lat_long;
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
   const Color wind_barb_color = Color::black (0.5);
   wind_barb_color.cairo (cr);

   for (point.x = start_x; point.x < width; point.x += h)
   {
      for (point.y = start_y; point.y < height; point.y += h)
      {

         transform.reverse (latitude, longitude, point.x, point.y);
         if (model.out_of_bounds (lat_long, stage)) { continue; }

         Real u = model.evaluate (U, ll, level, dtime, stage);
         Real v = model.evaluate (V, ll, level, dtime, stage);
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
                 const Hrit& hrit,
                 const Station::Map& station_map,
                 const Dtime& dtime,
                 const Level& level,
                 const Stage& stage,
                 const Product product)
{

   cr->save ();

   //Color (0.86, 0.85, 0.47).cairo (cr);
   Checkered (Color::gray (0.55), Color::gray (0.45)).cairo (cr);
   cr->paint();

   render_product (cr, transform, size_2d, model, hrit,
      product, dtime, level, stage);
   render_wind_barbs (cr, transform, size_2d, model, dtime, level, stage);

   // Stage 3/4/5 Frames
   render_stages (cr, transform, model);

   // All Stations
   //station_map.cairo (cr, transform);

   cr->restore ();

}

void
Display::render_cross_section_w (const RefPtr<Context>& cr,
                                 const Transform_2D& transform,
                                 const Box_2D& box_2d,
                                 const Model& model,
                                 const Stage& stage,
                                 const Dtime& dtime,
                                 const Multi_Journey& multi_journey)
{
   Raster* raster_ptr = Display::get_cross_section_raster_ptr (box_2d,
      transform, model, stage, Product ("W_TRANSLUCENT"), dtime,
      multi_journey);
   raster_ptr->blit (cr);
   delete raster_ptr;
}

void
Display::render_cross_section_mesh (const RefPtr<Context>& cr,
                                    const Transform_2D& transform,
                                    const Domain_1D& domain_z,
                                    const Multi_Journey& multi_journey)
{

   const Geodesy geodesy;
   const Real distance = multi_journey.get_distance (geodesy);

   Real d_distance = 100e3;
   if (distance < 1000e3) { d_distance = 50e3; }
   if (distance < 400e3) { d_distance = 20e3; }
   if (distance < 200e3) { d_distance = 10e3; }
   const Multi_Journey mj (multi_journey, geodesy, d_distance);
   const Tuple& tuple_x = mj.get_tuple_x (geodesy);

   const Simple_Mesh_2D ma0 (Color::black (0.05), 1e8, 10);
   const Simple_Mesh_2D ma1 (Color::black (0.1), 1e8, 100);
   const Simple_Mesh_2D ma2 (Color::black (0.4), 1e8, 1000);
   const Domain_1D domain_x (0, distance);
   const Domain_2D domain_2d (domain_x, domain_z);
   const Mesh_2D mesh_2d (Size_2D (2, 2), domain_2d, ma2);

   cr->set_line_width (2);
   mesh_2d.render (cr, transform);
   Color::black (0.3).cairo (cr);

   mesh_2d.render_label_x (cr, transform, 0, 0,
      "%.0f", NUMBER_REAL, 'c', 't', 5);
   mesh_2d.render_label_y (cr, transform, 0, 0,
      "%.0f", NUMBER_REAL, 'r', 'c', 5);

   for (Tuple::const_iterator iterator = tuple_x.begin ();
        iterator != tuple_x.end (); iterator++)
   {

      const Real d = *(iterator);
      const Point_2D& point_a = transform.transform (Point_2D (d, 0));
      const Point_2D& point_b = transform.transform (Point_2D (d, 8000));

      const Integer i = std::distance (tuple_x.begin (), iterator);
      const string& str = string_render ("%d", i);

      Color::black (0.3).cairo (cr);
      Label (str, point_a, 'c', 't', 6).cairo (cr);

   }

}

void
Display::render_cross_section_arrows (const RefPtr<Context>& cr,
                                      const Transform_2D& transform,
                                      const Box_2D& box_2d,
                                      const Model& model,
                                      const Stage& stage,
                                      const Product& product,
                                      const Dtime& dtime,
                                      const Multi_Journey& multi_journey)
{

   Real x;
   Level level (HEIGHT_LEVEL, GSL_NAN);
   Real& z = level.value;

   const Real arrow_size = 15;
   const Real h = arrow_size * 1.5;

   Color::black (0.2).cairo (cr);
   const auto& terrain_stage = model.terrain.get_stage (stage);
   const auto& uppers_stage = model.uppers.get_stage (stage);
   const size_t l = uppers_stage.get_l (dtime);

   const Geodesy geodesy;
   const Index_2D& index_2d = box_2d.index_2d;
   const Size_2D& size_2d = box_2d.size_2d;
   const Real distance = multi_journey.get_distance (geodesy);

   cr->set_line_width (1);

   for (Integer i = index_2d.i; i < index_2d.i + size_2d.i; i += Integer (h))
   {

      transform.reverse (x, z, Real (i), 0);
      if (x < 0 || x > distance) { continue; }

      const Lat_Long lat_long = multi_journey.get_lat_long (x, geodesy);
      if (model.out_of_bounds (lat_long, stage)) { continue; }

      const Real topography = terrain_stage.get_topography (lat_long);
      const Real azimuth = multi_journey.get_azimuth_forward (x, geodesy);
      const Real theta = azimuth * DEGREE_TO_RADIAN;
      const Real c = cos (theta);
      const Real s = sin (theta);

      for (Integer j = index_2d.j; j < index_2d.j + size_2d.j; j += Integer (h))
      {

         transform.reverse (x, z, Real (i), Real (j));
         if (z < topography) { continue; }

         Real u = uppers_stage.evaluate (U, lat_long, z, l);
         Real v = uppers_stage.evaluate (V, lat_long, z, l);
         Real w = uppers_stage.evaluate (W, lat_long, z, l);
         Real uu = u * s + v * c;
         transform.transform_uv (uu, w, x, z);
         const Real theta = atan2 (w, uu);
         const Real mag = sqrt (uu*uu + w*w);
         const Real as = std::min (3600 * mag, arrow_size);
         const Arrow arrow (theta, as, 0.12);
         arrow.cairo (cr, Point_2D (i, j));
         cr->stroke ();

      }

   }


}

void
Display::render_cross_section (const RefPtr<Context>& cr,
                               const Transform_2D& transform,
                               const Box_2D& box_2d,
                               const Domain_1D& domain_z,
                               const Model& model,
                               const Stage& stage,
                               const Product& product,
                               const Dtime& dtime,
                               const Multi_Journey& multi_journey)
{

   cr->save ();

   Color::white ().cairo (cr);
   cr->paint ();

   Raster* raster_ptr = Display::get_cross_section_raster_ptr (box_2d,
      transform, model, stage, product, dtime, multi_journey);
   raster_ptr->blit (cr);
   delete raster_ptr;

   if (product == "RHO" ||
       product == "THETA")
   {
      render_cross_section_w (cr, transform, box_2d,
         model, stage, dtime, multi_journey);
   }

   render_cross_section_mesh (cr, transform, domain_z, multi_journey);

   render_cross_section_arrows (cr, transform, box_2d, model, stage,
      product, dtime, multi_journey);

   cr->restore ();

}

void
Display::render_meteogram (const RefPtr<Context>& cr,
                           const Size_2D& size_2d,
                           const Model& model, 
                           const Stage& stage, 
                           const Lat_Long& lat_long)
{

   cr->save ();

   Color::white ().cairo (cr);
   cr->paint ();

   const Aws::Repository* aws_repository_ptr =
      model.get_aws_repository_ptr (lat_long, stage);
   const Aws::Repository& aws_repository = *aws_repository_ptr;

   const Dtime& start_time = aws_repository.begin ()->first.dtime;
   const Dtime& end_time = aws_repository.rbegin ()->first.dtime;
   const Domain_1D domain_t (start_time.t, end_time.t);

   const Domain_1D domain_temperature (-15, 35);
   const Domain_1D domain_speed (0, 30);
   const Domain_1D domain_direction (0, 360);
   const Domain_1D domain_pressure (990e2, 1030e2);

   Affine_Transform_2D transform_temperature;
   Affine_Transform_2D transform_speed;
   Affine_Transform_2D transform_direction;
   Affine_Transform_2D transform_pressure;

   const Real margin = 50;
   const Real title_height = 100;

   const Real width = size_2d.i - 2 * margin;
   const Real height = (size_2d.j - title_height - 5 * margin) / 4;

   const Real span_t = domain_t.get_span ();
   const Real span_temperature = domain_temperature.get_span ();
   const Real span_speed = domain_speed.get_span ();
   const Real span_direction = domain_direction.get_span ();
   const Real span_pressure = domain_pressure.get_span ();

   const Real scale_t = width / span_t;
   const Real scale_temperature = height / span_temperature;
   const Real scale_speed = height / span_speed;
   const Real scale_direction = height / span_direction;
   const Real scale_pressure = height / span_pressure;

   const Real position_temperature = 3 * height + 4 * margin;
   const Real position_speed = 2 * height + 3 * margin;
   const Real position_direction = height + 2 * margin;
   const Real position_pressure = margin;

   transform_temperature.scale (1, -1);
   transform_speed.scale (1, -1);
   transform_direction.scale (1, -1);
   transform_pressure.scale (1, -1);

   transform_temperature.scale (scale_t, scale_temperature);
   transform_speed.scale (scale_t, scale_speed);
   transform_direction.scale (scale_t, scale_direction);
   transform_pressure.scale (scale_t, scale_pressure);

   transform_temperature.translate (0, size_2d.j);
   transform_speed.translate (0, size_2d.j);
   transform_direction.translate (0, size_2d.j);
   transform_pressure.translate (0, size_2d.j);

   transform_temperature.translate (margin, position_temperature);
   transform_speed.translate (margin, position_speed);
   transform_direction.translate (margin, position_direction);
   transform_pressure.translate (margin, position_pressure);

   for (auto iterator = aws_repository.begin ();
        iterator != aws_repository.end (); iterator++)
   {

      const Aws::Key& key = iterator->first;
      const Aws::Obs& obs = iterator->second;
      const Dtime& dtime = key.dtime;

      const Real temperature = obs.temperature - K;
      const Real dew_point = obs.dew_point - K;
      const Real direction = obs.wind_direction;
      const Real speed = obs.wind_speed;
      const Real mslp = obs.mslp;

      const string& time_str = dtime.get_string ("%Y%m%d%H%M");

      Color::black ().cairo (cr);
      const Point_2D p_temperature (dtime.t, temperature);
      const Point_2D& tp_temperature = transform_temperature.transform (p_temperature);
      Ring (2).cairo (cr, tp_temperature);
      cr->stroke ();

      Color::red ().cairo (cr);
      const Point_2D p_dew_point (dtime.t, dew_point);
      const Point_2D& tp_dew_point = transform_temperature.transform (p_dew_point);
      Ring (2).cairo (cr, tp_dew_point);
      cr->stroke ();

      Color::green ().cairo (cr);
      const Point_2D p_speed (dtime.t, speed);
      const Point_2D& tp_speed = transform_speed.transform (p_speed);
      Ring (2).cairo (cr, tp_speed);
      cr->stroke ();

      Color::yellow ().cairo (cr);
      const Point_2D p_direction (dtime.t, direction);
      const Point_2D& tp_direction = transform_direction.transform (p_direction);
      Ring (2).cairo (cr, tp_direction);
      cr->stroke ();

      Color::magenta ().cairo (cr);
      const Point_2D p_mslp (dtime.t, mslp);
      const Point_2D& tp_mslp = transform_pressure.transform (p_mslp);
      Ring (2).cairo (cr, tp_mslp);
      cr->stroke ();

      cout << time_str << " " << temperature << " " << dew_point <<
         " " << direction << " " << speed << endl;
      cout << "   " << p_temperature << " " << tp_temperature << endl;
      cout << "   " << p_dew_point   << " " << tp_dew_point   << endl;
      cout << "   " << p_speed       << " " << tp_speed       << endl;
      cout << "   " << p_direction   << " " << tp_direction   << endl;
      cout << "   " << p_mslp        << " " << tp_mslp        << endl;

   }

   delete aws_repository_ptr;

   cr->restore ();

}

void
Display::render_vertical_profile (const RefPtr<Context>& cr,
                                  const Thermo_Diagram& thermo_diagram,
                                  const Model& model,
                                  const Stage& stage,
                                  const Dtime& dtime,
                                  const Lat_Long& lat_long)
{

   cr->save ();

   Color::white ().cairo (cr);
   cr->paint ();

   const Sounding* sounding_ptr =
      model.get_sounding_ptr (lat_long, dtime, stage);
   const Sounding& sounding = *sounding_ptr;

   cr->set_line_width (1);
   thermo_diagram.render (cr, 900);
   cr->set_line_width (2);
   sounding.render (cr, thermo_diagram);

   delete sounding_ptr;

   cr->restore ();

}

