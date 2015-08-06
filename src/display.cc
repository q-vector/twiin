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
         const Product product (lsm > 0.5 ?
            Product::TERRAIN : Product::TERRAIN_WATER);
         const Color& color = Display::get_color (product, orog);
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

   const string channel (product.get_string ());
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

         const Color& color = Hrit::get_color (channel,
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
                                       const Journey& journey,
                                       const Real u_bg)
{

   Raster* raster_ptr = new Raster (box_2d);

   Color color;
   const auto& terrain_stage = model.terrain.get_stage (stage);
   const auto& uppers_stage = model.uppers.get_stage (stage);
   const size_t l = uppers_stage.get_l (dtime);

   const bool is_speed = (product.enumeration == Product::SPEED);
   const bool is_along_speed = (product.enumeration == Product::ALONG_SPEED);
   const bool is_normal_speed = (product.enumeration == Product::NORMAL_SPEED);
   const bool is_wind = (product.enumeration == Product::WIND);
   const bool is_scorer = (product.enumeration == Product::SCORER);
   const bool is_brunt_vaisala = (product.enumeration == Product::BRUNT_VAISALA);

   const Product& p = (is_speed ? Product (Product::SPEED_HIGHER) : product);

   Real x;
   Level level (HEIGHT_LEVEL, GSL_NAN);
   Real& z = level.value;

   const Geodesy geodesy;
   const Index_2D& index_2d = box_2d.index_2d;
   const Size_2D& size_2d = box_2d.size_2d;
   const Real distance = journey.get_distance (geodesy);

   for (Integer i = index_2d.i; i < index_2d.i + size_2d.i; i++)
   {

      transform.reverse (x, z, Real (i), 0);
      if (x < 0 || x > distance) { continue; }
      const Lat_Long& lat_long = journey.get_lat_long (x, geodesy);
      const Lat_Long& ll = lat_long;
      if (model.out_of_bounds (lat_long, stage)) { continue; }

      const Real topography = terrain_stage.get_topography (lat_long);

      for (Integer j = index_2d.j; j < index_2d.j + size_2d.j; j++)
      {
         transform.reverse (x, z, Real (i), Real (j));
         if (z < topography) { color = Color::black (); }
         else
         {
            if (is_wind)
            {
               const Real u = uppers_stage.evaluate (U, lat_long, z, l);
               const Real v = uppers_stage.evaluate (V, lat_long, z, l);
               const Wind wind (u, v);
               const Real direction = wind.get_direction ();
               const Real speed = wind.get_speed ();
               color = Display::get_wind_color (direction, speed);
            }
            else
            if (is_brunt_vaisala)
            {
               const Real datum =
                  uppers_stage.evaluate_brunt_vaisala (lat_long, z, l);
               color = Display::get_color (p, datum);
            }
            else
            if (is_scorer)
            {
               const Real azimuth =
                  journey.get_azimuth_forward (x, geodesy);
               const Real datum = uppers_stage.evaluate_scorer (
                  azimuth, lat_long, z, l, u_bg);
               color = Display::get_color (p, datum);
            }
            else
            if (is_along_speed)
            {
               const Real azimuth =
                  journey.get_azimuth_forward (x, geodesy);
               const Real datum = uppers_stage.evaluate_along_speed (
                  azimuth, lat_long, z, l, u_bg);
               color = Display::get_color (p, datum);
            }
            else
            if (is_normal_speed)
            {
               const Real azimuth =
                  journey.get_azimuth_forward (x, geodesy);
               const Real datum =
                  uppers_stage.evaluate_normal_speed (azimuth, lat_long, z, l);
               color = Display::get_color (p, datum);
            }
            else
            {
               const Nwp_Element nwp_element = product.get_nwp_element ();
               const Real datum = uppers_stage.evaluate (nwp_element, ll, z, l);
               color = Display::get_color (p, datum);
            }
         }
         raster_ptr->set_pixel (i - index_2d.i, j - index_2d.j, color);
      }

   }

   return raster_ptr;

}

void
Display::set_title (Title& title,
                    const Dtime& basetime,
                    const Stage& stage,
                    const Product& product,
                    const Dtime& dtime,
                    const Level& level)
{

   const Real forecast_hour = dtime.t - basetime.t;
   const Integer forecast_minutes = Integer (round (forecast_hour * 60));
   const Integer fh = forecast_minutes / 60;
   const Integer fm = forecast_minutes % 60;

   const string fmt ("%Y.%m.%d %H:%M UTC");
   const string& time_str = dtime.get_string (fmt);
   const string& basetime_str = basetime.get_string () +
      string_render (" +%d:%02d", fh, fm);
   const string stage_product = stage + " / " + product.get_string ();

   title.set (time_str, "", stage_product, basetime_str, level.get_string ());

}

void
Display::set_title (Title& title,
                    const Dtime& basetime,
                    const Stage& stage,
                    const Product& product,
                    const Dtime& dtime,
                    const Journey& journey)
{

   const Real forecast_hour = dtime.t - basetime.t;
   const Integer forecast_minutes = Integer (round (forecast_hour * 60));
   const Integer fh = Integer (round (forecast_hour * 60)) / 60;
   const Integer fm = Integer (round (forecast_hour * 60)) % 60;

   const Lat_Long orig (journey.front ());
   const Lat_Long dest (journey.back ());
   const bool complex_j = (journey.size () > 2);

   const string o_suffix (complex_j ? " ..." : "");
   const string d_preffix (complex_j ? "... " : "");
   const string& o_str = orig.get_string (4, true, true, true) + o_suffix;
   const string& d_str = d_preffix + dest.get_string (4, true, true, true);

   const string fmt ("%Y.%m.%d %H:%M UTC");
   const string& time_str = dtime.get_string (fmt);
   const string& basetime_str = basetime.get_string () +
      string_render (" +%d:%02d", fh, fm);
   const string stage_product = stage + " / " + product.get_string ();

   title.set (time_str, o_str, stage_product, basetime_str, d_str);

}

void
Display::set_title (Title& title,
                    const Dtime& basetime,
                    const Stage& stage,
                    const Dtime& dtime,
                    const Location& location)
{

   const Real forecast_hour = dtime.t - basetime.t;
   const Integer forecast_minutes = Integer (round (forecast_hour * 60));
   const Integer fh = forecast_minutes / 60;
   const Integer fm = forecast_minutes % 60;

   const Lat_Long& lat_long (location);
   const string& ll_str = lat_long.get_string (4, true, true, true);

   const string fmt ("%Y.%m.%d %H:%M UTC");
   const string& time_str = dtime.get_string (fmt);
   const string& basetime_str = basetime.get_string () +
      string_render (" +%d:%02d", fh, fm);

   title.set (time_str, stage, location.get_long_str (), basetime_str, ll_str);

}

void
Display::set_title (Title& title,
                    const Dtime& basetime,
                    const Stage& stage,
                    const Dtime& dtime,
                    const string& location_name)
{

   const Real forecast_hour = dtime.t - basetime.t;
   const Integer forecast_minutes = Integer (round (forecast_hour * 60));
   const Integer fh = forecast_minutes / 60;
   const Integer fm = forecast_minutes % 60;

   const string fmt ("%Y.%m.%d %H:%M UTC");
   const string& time_str = dtime.get_string (fmt);
   const string& basetime_str = basetime.get_string () +
      string_render (" +%d:%02d", fh, fm);

   title.set (time_str, stage, location_name, basetime_str, "");

}

void
Display::set_title (Title& title,
                    const Dtime& basetime,
                    const Stage& stage,
                    const Location& location)
{
   const string& basetime_str = basetime.get_string ();
   const Lat_Long& lat_long (location);
   const string& ll_str = lat_long.get_string (4, true, true, true);
   title.set ("", stage, location.get_long_str (), basetime_str, ll_str);
}

string
Display::get_unit (const Product& product)
{

   switch (product.enumeration)
   {

      case Product::T:
      {
         return string ("\u00b0C");
      }

      case Product::TD:
      {
         return string ("\u00b0C");
      }

      case Product::RH:
      {
         return string ("%%");
      }

      case Product::THETA:
      {
         return string ("\u00b0C");
      }

      case Product::THETA_V:
      {
         return string ("\u00b0C");
      }

      case Product::THETA_E:
      {
         return string ("\u00b0C");
      }

      case Product::Q:
      {
         return string ("g kg\u207b\u00b9");
      }

      case Product::RHO:
      {
         return string ("kg m\u207b\u00b9");
      }

      case Product::W:
      {
         return string ("m s\u207b\u00b9");
      }

      case Product::W_TRANSLUCENT:
      {
         return string ("m s\u207b\u00b9");
      }

      case Product::SPEED_HIGHER:
      {
         return string ("knots");
      }

      case Product::SPEED:
      case Product::ALONG_SPEED:
      case Product::NORMAL_SPEED:
      {
         return string ("knots");
      }

      case Product::VORTICITY:
      {
         return string ("10\u207b\u00b3 s\u207b\u00b9");
      }

      case Product::FFDI:
      {
         return string ("");
      }

      case Product::MSLP:
      {
         return string ("hPa");
      }

      case Product::PRECIP_RATE:
      {
         return string ("mm hr\u207b\u00b9");
      }

      case Product::WIND:
      {
         return string ("degree");
      }

      case Product::TERRAIN:
      {
         return string ("m");
      }

      case Product::BRUNT_VAISALA:
      {
         return string ("s\u207b\u00b9");
      }

      default:
      {
         return string ("");
      }

   }

}

Tuple
Display::get_tick_tuple (const Product& product)
{

   switch (product.enumeration)
   {

      case Product::T:
      {
         return Tuple ("10:15:20:25:30:35");
      }

      case Product::TD:
      {
         return Tuple ("-10:-5:0:5:10:15:20");
      }

      case Product::RH:
      {
         return Tuple ("0:20:40:60:80:100");
      }

      case Product::THETA:
      case Product::THETA_V:
      {
         return Tuple ("0:10:20:30:40:50:60");
      }

      case Product::THETA_E:
      {
         return Tuple ("5:15:25:35:45:55:65");
      }

      case Product::Q:
      {
         return Tuple ("0:1:2:3:4:5:6:7:8:9:10");
      }

      case Product::RHO:
      {
         return Tuple ("0.0:1.0:2.0;3.0:4.0:5.0:6.0:7.0:8.0:9.0:10.0");
      }

      case Product::W:
      {
         return Tuple ("-4:-3:-2:-1:0:1:2:3:4");
      }

      case Product::W_TRANSLUCENT:
      {
         return Tuple ("-4:-3:-2:-1:0:1:2:3:4");
      }

      case Product::SPEED_HIGHER:
      {
         return Tuple ("0:5:15:25:35:45:55:65:75:85:95:105:115:125");
      }

      case Product::SPEED:
      {
         return Tuple ("0:5:10:15:20:25:30:35:40:45:50:55:60:65");
      }

      case Product::ALONG_SPEED:
      case Product::NORMAL_SPEED:
      {
         return Tuple (26, -125.0, 125.0);
      }

      case Product::VORTICITY:
      {
         return Tuple ("-5:-4:-3:-2:-1:0:1:2:3:4:5");
      }

      case Product::FFDI:
      {
         return Tuple ("1:12:25:50:75:100:150");
      }

      case Product::MSLP:
      {
         return Tuple ("980:990:1000:1010:1020:1030");
      }

      case Product::PRECIP_RATE:
      {
         return Tuple ("0.05:0.1:1:2:5:10:20:30:50:75:100:150");
      }

      case Product::WIND:
      {
         return Tuple ("0:30:60:90:120:150:180:210:240:270:300:330:360");
      }

      case Product::TERRAIN:
      {
         return Tuple ("0:200:400:600:800:1000:1200:1400:1600:1800:2000");
      }

      case Product::BRUNT_VAISALA:
      {
         return Tuple ("1e-3:3.2e-3:1e-2:3.2e-2:1e-1");
      }

      default:
      {
         return Tuple ();
      }

   }

}

Color
Display::get_wind_color (const Real direction,
                         const Real speed)
{
   const Real hue = direction / 360.;
   const Real brightness = std::min (speed / 15, 1.0);
   return Color::hsb (hue, 0.8, brightness);
}

Color
Display::get_color (const Product& product,
                    const Real datum)
{

   switch (product.enumeration)
   {

      case Product::T:
      {
         const Real hue = Domain_1D (35 + K, 10 + K).normalize (datum) * 0.833;
         const Real brightness = (Integer (floor (datum)) % 2) ? 0.83:0.77;
         return Color::hsb (hue, 0.8, brightness);
      }

      case Product::TD:
      {
         const Real saturation = Domain_1D (-10+K, 20+K).normalize (datum);
         const Real brightness = (Integer (floor (datum)) % 2) ? 0.82:0.78;
         return Color::hsb (0.25, saturation, brightness);
      }

      case Product::RH:
      {
         const Real hue = (datum < 0.5 ? 0.08 : 0.35);
         //const Real saturation = std::min ((fabs (datum - 0.5) * 2), 1.0);
         //return Color::hsb (hue, saturation, 1, 0.4);
         const Real saturation = fabs (datum - 0.5) * 0.8;
         return Color::hsb (hue, saturation, 1);
      }

      case Product::THETA:
      case Product::THETA_V:
      {
         const Real jump = 28+K;
         const Real deviate = datum - jump;
         const Real x = atan (deviate / 1);
         const Real odd = (Integer (floor ((datum-K) / 1)) % 2);
         const Real fluctuation = (odd ? 0.03 : -0.03);
         const Domain_1D d1d (-M_PI_2, M_PI_2);
         const Real brightness = d1d.normalize (x) * 0.5 + 0.45;
         return Color::hsb (0.0, 0.0, brightness + fluctuation);
      }

      case Product::THETA_E:
      {
         const Real saturation = Domain_1D (5+K, 65+K).normalize (datum);
         const Real odd = (Integer (floor ((datum-K) / 4)) % 2);
         const Real brightness = (odd ? 0.82 : 0.78);
         return Color::hsb (0.35, saturation, brightness);
      }

      case Product::Q:
      {
         const Real saturation = Domain_1D (0, 10).normalize (datum * 1e3);
         const Real odd = (Integer (floor (datum * 1e3)) % 2);
         const Real brightness = (odd ? 0.82 : 0.78);
         return Color::hsb (0.45, saturation, brightness);
      }

      case Product::RHO:
      {
         const Real jump = 1.10;
         const Real deviate = datum - jump;
         const Real x = atan (deviate / 0.05);
         const Domain_1D d1d (M_PI_2, -M_PI_2);
         const Real b = d1d.normalize (x) * 0.85 + 0.15;
         const Real s = exp (-fabs (deviate) / 0.01) * 0.8 + 0.2;
         return Color::hsb (0.800, s, b);
      }

      case Product::W:
      {
         const Real hue = (datum < 0 ? 0.667 : 0.000);
         const Real absolute = fabs (datum);
         const Real quantized = floor (absolute / 0.1) * 0.1;
         const Real saturation = Domain_1D (0, 3.5).normalize (quantized) * 0.7;
         return Color::hsb (hue, saturation, 1.0);
      }

      case Product::W_TRANSLUCENT:
      {
         const Real hue = (datum < 0 ? 0.667 : 0.000);
         const Real absolute = fabs (datum);
         const Real quantized = floor (absolute / 0.1) * 0.1;
         const Real alpha = Domain_1D (0, 3.5).normalize (quantized);
         return Color::hsb (hue, 1.0, 1.0, alpha);
      }

      case Product::SPEED_HIGHER:
      {
         const Real knots = datum * 3.6/1.852;
         const Real m0 = modulo (knots, 5.0) / 5.0 * 0.15 + 0.85;
         const Real m = modulo (knots - 5, 10.0) / 10.0 * 0.15 + 0.85;
              if (knots <   5) { return Color::hsb (0.000, 0.0*m0, 1.0*m0); }
         else if (knots <  15) { return Color::hsb (0.167, 0.30*m, 1.00*m); }
         else if (knots <  25) { return Color::hsb (0.150, 0.35*m, 0.95*m); }
         else if (knots <  35) { return Color::hsb (0.133, 0.40*m, 0.90*m); }
         else if (knots <  45) { return Color::hsb (0.333, 0.40*m, 0.95*m); }
         else if (knots <  55) { return Color::hsb (0.333, 0.40*m, 0.75*m); }
         else if (knots <  65) { return Color::hsb (0.333, 0.40*m, 0.65*m); }
         else if (knots <  75) { return Color::hsb (0.600, 0.40*m, 1.00*m); }
         else if (knots <  85) { return Color::hsb (0.633, 0.40*m, 0.90*m); }
         else if (knots <  95) { return Color::hsb (0.667, 0.40*m, 0.80*m); }
         else if (knots < 105) { return Color::hsb (0.033, 0.40*m, 0.65*m); }
         else if (knots < 115) { return Color::hsb (0.016, 0.45*m, 0.80*m); }
         else if (knots < 125) { return Color::hsb (0.000, 0.50*m, 0.95*m); }
         else                  { return Color (1.000, 1.000, 1.000); }
      }

      case Product::SPEED:
      {
         const Real knots = datum * 3.6/1.852;
         const Real m = modulo (knots, 5.0) / 5.0 * 0.15 + 0.85;
              if (knots <  5) { return Color::hsb (0.000, 0.00*m, 1.00*m); }
         else if (knots < 10) { return Color::hsb (0.167, 0.30*m, 1.00*m); }
         else if (knots < 15) { return Color::hsb (0.150, 0.35*m, 0.95*m); }
         else if (knots < 20) { return Color::hsb (0.133, 0.40*m, 0.90*m); }
         else if (knots < 25) { return Color::hsb (0.333, 0.40*m, 0.95*m); }
         else if (knots < 30) { return Color::hsb (0.333, 0.40*m, 0.75*m); }
         else if (knots < 35) { return Color::hsb (0.333, 0.40*m, 0.65*m); }
         else if (knots < 40) { return Color::hsb (0.600, 0.40*m, 1.00*m); }
         else if (knots < 45) { return Color::hsb (0.633, 0.40*m, 0.90*m); }
         else if (knots < 50) { return Color::hsb (0.667, 0.40*m, 0.80*m); }
         else if (knots < 55) { return Color::hsb (0.033, 0.40*m, 0.65*m); }
         else if (knots < 60) { return Color::hsb (0.016, 0.45*m, 0.80*m); }
         else if (knots < 65) { return Color::hsb (0.000, 0.50*m, 0.95*m); }
         else                 { return Color (1.000, 1.000, 1.000); }
      }

      case Product::ALONG_SPEED:
      case Product::NORMAL_SPEED:
      {
         const bool negative = (datum < 0);
         const Real knots = fabs (datum * 3.6/1.852);
         const Real m0 = modulo (knots, 5.0) / 5.0 * 0.15 + 0.85;
         const Real m = modulo (knots - 5, 10.0) / 10.0 * 0.15 + 0.85;
         if (!negative)
         {
                 if (knots <  5) { return Color::hsb (0.000, 0.0*m0, 1.0*m0); }
            else if (knots < 15) { return Color::hsb (0.167, 0.30*m, 1.00*m); }
            else if (knots < 25) { return Color::hsb (0.150, 0.35*m, 0.95*m); }
            else if (knots < 35) { return Color::hsb (0.133, 0.40*m, 0.90*m); }
            else if (knots < 45) { return Color::hsb (0.333, 0.40*m, 0.95*m); }
            else if (knots < 55) { return Color::hsb (0.333, 0.40*m, 0.75*m); }
            else if (knots < 65) { return Color::hsb (0.333, 0.40*m, 0.65*m); }
            else if (knots < 75) { return Color::hsb (0.600, 0.40*m, 1.00*m); }
            else if (knots < 85) { return Color::hsb (0.633, 0.40*m, 0.90*m); }
            else if (knots < 95) { return Color::hsb (0.667, 0.40*m, 0.80*m); }
            else if (knots < 105){ return Color::hsb (0.033, 0.40*m, 0.65*m); }
            else if (knots < 115){ return Color::hsb (0.016, 0.45*m, 0.80*m); }
            else if (knots < 125){ return Color::hsb (0.000, 0.50*m, 0.95*m); }
            else                 { return Color (1.000, 1.000, 1.000); }
         }
         else
         {
                 if (knots <  5) { return Color::hsb (0.000, 0.0*m0, 0.8*m0); }
            else if (knots < 15) { return Color::hsb (0.167, 0.20*m, 0.80*m); }
            else if (knots < 25) { return Color::hsb (0.150, 0.25*m, 0.75*m); }
            else if (knots < 35) { return Color::hsb (0.133, 0.30*m, 0.70*m); }
            else if (knots < 45) { return Color::hsb (0.333, 0.30*m, 0.75*m); }
            else if (knots < 55) { return Color::hsb (0.333, 0.30*m, 0.55*m); }
            else if (knots < 65) { return Color::hsb (0.333, 0.30*m, 0.45*m); }
            else if (knots < 75) { return Color::hsb (0.600, 0.30*m, 0.80*m); }
            else if (knots < 85) { return Color::hsb (0.633, 0.30*m, 0.70*m); }
            else if (knots < 95) { return Color::hsb (0.667, 0.30*m, 0.60*m); }
            else if (knots < 105){ return Color::hsb (0.033, 0.30*m, 0.45*m); }
            else if (knots < 115){ return Color::hsb (0.016, 0.35*m, 0.60*m); }
            else if (knots < 125){ return Color::hsb (0.000, 0.40*m, 0.75*m); }
            else                 { return Color (1.000, 1.000, 1.000); }
         }
      }

      case Product::VORTICITY:
      {
         const Real hue = (datum < 0 ? 0.667 : 0.000);
         const Real modified_datum = (log10 (fabs (datum)) + 4) / 3;
         const Real saturation = std::max (std::min (modified_datum, 1.0), 0.0);
         //const Real saturation = Domain_1D (0, 1).normalize (modified_datum);
         return Color::hsb (hue, saturation, 1, 1);
      }

      case Product::FFDI:
      {
         const Ffdi_Color_Chooser ffdi_color_chooser (1.0);
         return ffdi_color_chooser.get_color (datum);
      }

      case Product::MSLP:
      {
         //const Real hue = Domain_1D (990e2, 1025e2).normalize (datum) * 0.833;
         const Real brightness = (Integer (floor (datum/200)) % 2) ? 0.82:0.78;
         return Color::hsb (0, 0, brightness);
      }

      case Product::PRECIP_RATE:
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

      case Product::TERRAIN:
      {
         const Real h = std::min (std::max (datum / 2000.0, 0.0), 1.0);
         const Real hue = 0.45 - h * 0.4;
         const Real brightness = h * 0.7 + 0.28;
         return Color::hsb (hue, 0.34, brightness);
      }

      case Product::TERRAIN_WATER:
      {
         const Real h = std::min (std::max (datum / 2000.0, 0.0), 1.0);
         const Real hue = 0.67;
         const Real brightness = h * 0.7 + 0.28;
         return Color::hsb (hue, 0.34, brightness);
      }

      case Product::BRUNT_VAISALA:
      {
         if (!gsl_finite (datum)) { return Color::white (); }
         const Real e = log10 (datum) - (-3.0);
         const Real x = std::max (std::min (e / 2.0, 1.0), 0.0);
         const Real hue = 0.2 + (floor (e / 0.5)) * 0.10;
         return Color::hsb (hue, x, 1.0 - x * 0.5);
      }

      default:
      {
         return Color::transparent ();
      }

   }

}

Color
Display::get_color (const Product& product,
                    const Real datum,
                    const string& unit)
{

   if (unit == "\u00b0C")
   {
      return get_color (product, datum + K);
   }

   if (unit == "knots")
   {
      return get_color (product, datum * 1.852/3.6);
   }

   if (unit == "g kg\u207b\u00b9")
   {
      return get_color (product, datum * 1e-3);
   }

   if (unit == "%%")
   {
      return get_color (product, datum * 1e-2);
   }

   if (unit == "hPa")
   {
      return get_color (product, datum * 1e2);
   }

   if (unit == "degree")
   {
      return get_wind_color (datum, 15);
   }

   if (unit == "mm hr\u207b\u00b9")
   {
      return get_color (product, datum / 3600.);
   }

   if (unit == "10\u207b\u00b3 s\u207b\u00b9")
   {
      return get_color (product, datum * 1e-3);
   }

   return get_color (product, datum);

}

void
Display::render_stages (const RefPtr<Context>& cr,
                        const Geodetic_Transform& transform,
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
                         const Geodetic_Transform& transform,
                         const Size_2D& size_2d,
                         const Model& model,
                         const Hrit& hrit,
                         const Product& product,
                         const Dtime& dtime,
                         const Level& level,
                         const twiin::Stage& stage)
{

   Raster* raster_ptr = NULL;

   switch (product.enumeration)
   {

      case Product::TERRAIN:
      {
         raster_ptr = get_terrain_raster_ptr (size_2d, transform, model, stage);
         break;
      }

      case Product::T:
      case Product::P_THETA:
      case Product::P_RHO:
      case Product::TD:
      case Product::RH:
      case Product::Q:
      case Product::THETA:
      case Product::THETA_V:
      case Product::RHO:
      case Product::WIND:
      case Product::SPEED:
      case Product::SPEED_HIGHER:
      case Product::W:
      case Product::W_TRANSLUCENT:
      case Product::VORTICITY:
      case Product::THETA_E:
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
         break;
      }

      case Product::FFDI:
      case Product::MSLP:
      case Product::PRECIP_RATE:
      {
         raster_ptr = get_surface_raster_ptr (
            size_2d, transform, model, stage, product, dtime);
         break;
      }

      case Product::IR1:
      case Product::IR2:
      case Product::IR3:
      case Product::IR4:
      case Product::VIS:
      case Product::Pseudo:
      {
         raster_ptr = get_hrit_raster_ptr (
            size_2d, transform, hrit, product, dtime);
         break;
      }

   }

   if (raster_ptr != NULL) { raster_ptr->blit (cr); }
   delete raster_ptr;

}

void
Display::render_wind_barbs (const RefPtr<Context>& cr,
                            const Geodetic_Transform& transform,
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
Display::render_scale_bar (const RefPtr<Context>& cr,
                           const Geodetic_Transform& geodetic_transform,
                           const Size_2D& size_2d)
{

   const Real bar_height = 6;
   const Real margin = 10;
   const Real font_size = 10;
   const Real scale = geodetic_transform.data.scale;

   const Real log10_scale = log10 (scale * 1.6);
   Real exponent = floor (log10_scale);
   Real residue = log10_scale - floor (log10_scale);
   if (residue > 0.849) { exponent++; residue = 0; }
   const Real mantissa = (residue > 0.548 ? 5.0 : (residue > 0.199 ? 2.5 : 1));
   const Real bar_length = mantissa * exp10 (exponent + 2);

   const Real pixels = bar_length / scale;
   const Real box_height = bar_height + font_size*2 + margin*2;
   const Point_2D point (2*margin, size_2d.j - 2*margin - box_height / 2);

   cr->save ();
   cr->set_line_width (1);
   cr->set_font_size (font_size);

   Color::white (0.7).cairo (cr);
   Rect (Point_2D (point.x - margin, point.y - bar_height/2 - font_size - margin), 2 * pixels + 2*margin, bar_height + 2 * font_size + 2 * margin).cairo (cr);
   cr->fill ();

   Color::black ().cairo (cr);
   Rect (Point_2D (point.x, point.y - bar_height/2), 2 * pixels, bar_height).cairo (cr);
   cr->stroke ();

   const Real tick = pixels/5;
   for (Integer i = 0; i < 5; i += 2)
   {
      const Real x = point.x + i * tick;
      Rect (Point_2D (x, point.y - bar_height/2), tick, bar_height).cairo (cr);
      cr->fill ();
   }

   for (Integer i = 0; i < 5; i++)
   {
      const Real x = point.x + i * tick;
      const Point_2D p (x, point.y);
      const Real tick_length = fabs ((bar_length - i * bar_length/5) * 1e-3);
      const string& str = string_render ("%.0f", tick_length);
      Label (str, p, 'c', 't', bar_height*1.5).cairo (cr);
   }

   const Real x = point.x + pixels;
   Label ("0", point + Point_2D (pixels, 0), 'c', 't', bar_height*1.5).cairo (cr);
   Label (string_render ("%.0f", bar_length*1e-3),
      point + Point_2D (2 * pixels, 0), 'c', 't', bar_height*1.5).cairo (cr);

   Label ("kilometres", point + Point_2D (pixels, 0), 'c', 'b', bar_height*1.5).cairo (cr);

   cr->restore ();

}

void
Display::render_color_bar (const RefPtr<Context>& cr,
                           const Size_2D& size_2d,
                           const Product& product)
{

   const string& unit = Display::get_unit (product);
   const Tuple& tick_tuple = Display::get_tick_tuple (product);
   if (tick_tuple.size () < 2) { return; }

   const Real title_height = 40;
   const Real margin = 10;
   const Real font_size = 12;

   const Real box_width = 80;
   const Real box_height = size_2d.j - title_height - 2*margin;
   const Real box_x = size_2d.i - box_width - margin;
   const Real box_y = title_height + margin;
   const Point_2D box_point (box_x, box_y);

   const Real bar_width = box_width - 5 * margin;
   const Real bar_height = box_height - 2 * margin - font_size;
   const Real bar_x = box_x + 4 * margin;
   const Real bar_y = box_y + margin + font_size;
   const Point_2D bar_point (bar_x, bar_y);

   const Domain_1D value_domain (tick_tuple.front (), tick_tuple.back ());
   const Domain_1D y_domain (bar_y + bar_height, bar_y);
   const bool is_log = (product.enumeration == Product::PRECIP_RATE ||
                        product.enumeration == Product::BRUNT_VAISALA);
   Cartesian_Transform_1D transform (value_domain, y_domain, is_log);

   cr->save ();
   cr->set_line_width (1);
   cr->set_font_size (font_size);

   Color::white (0.7).cairo (cr);
   Rect (box_point, box_width, box_height).cairo (cr);
   cr->fill ();

   const Real start_value = tick_tuple.front ();
   const Real span_value = value_domain.get_span ();

   Raster* raster_ptr = new Raster (Box_2D (Index_2D (bar_x, bar_y),
      Size_2D (bar_width, bar_height)));
   Raster& raster = *raster_ptr;

   for (Integer j = 0; j < bar_height; j++)
   {
      const Real y = bar_y + j;
      const Real value = transform.reverse (y);
      const Color& color = Display::get_color (product, value, unit);
      for (Integer i = 0; i < bar_width; i++)
      {
         raster.set_pixel (i, j, color);
      }
   }

   raster_ptr->blit (cr);
   delete raster_ptr;

   Color::black ().cairo (cr);
   Rect (bar_point, bar_width, bar_height).cairo (cr);
   cr->stroke ();

   for (auto iterator = tick_tuple.begin ();
        iterator != tick_tuple.end (); iterator++)
   {
      const Real tick_value = *(iterator);
      const Real y = transform.transform (tick_value);
      const Point_2D point (bar_x, y);
      const string& str = string_render ("%g", tick_value);
      Label (str, point, 'r', 'c', font_size/2).cairo (cr);
   }

   const Real label_x = bar_point.x + bar_width/2;
   const Real label_y = bar_point.y;
   const Point_2D label_point (label_x, label_y);
   Label (unit, label_point, 'c', 'b', font_size/2).cairo (cr);

   cr->restore ();

}

void
Display::render_annotation_point (const RefPtr<Context>& cr,
                                  const Geodetic_Transform& transform,
                                  const Lat_Long& lat_long,
                                  const string& str)
{

   const Real ring_size = 4;
   const Ring ring (ring_size);
   const Real font_size = 14;
   cr->set_font_size (font_size);

   const Point_2D& point = transform.transform (lat_long);
   ring.cairo (cr, point);

   Color::green (0.8).cairo (cr);
   cr->fill_preserve ();
   Color::black ().cairo (cr);
   cr->stroke ();

   const Point_2D anchor (point.x, point.y);
   Color::gray (0.8).cairo (cr);
   Label (str, point + Point_2D (2, 2), 'r', 'c', font_size/2).cairo (cr);
   Color::gray (0.2).cairo (cr);
   Label (str, point, 'r', 'c', font_size/2).cairo (cr);

}

void
Display::render_annotation (const RefPtr<Context>& cr,
                            const Geodetic_Transform& transform,
                            const string& annotation_str)
{

   const Tokens tokens (annotation_str, ":");
   const string& genre = tokens[0];

   if (genre == "location" ||
       genre == "lat_long" ||
       genre == "l")
   {
      const Location location (tokens[1]);
      const string& str = (tokens.size () > 2 ? tokens[2] : string (""));
      render_annotation_point (cr, transform, location, str);
   }
   else
   if (genre == "journey" ||
       genre == "route" ||
       genre == "j" ||
       genre == "r")
   {
      Journey journey;
      for (auto iterator = tokens.begin ();
           iterator != tokens.end (); iterator++)
      {
         if (iterator == tokens.begin ()) { continue; }
         const Location location (*(iterator));
         journey.push_back (location);
      }
      journey.cairo (cr, transform);
   }

}

void
Display::render_annotations (const RefPtr<Context>& cr,
                             const Geodetic_Transform& transform,
                             const Tokens& annotation_tokens)
{

   cr->save ();
   cr->set_line_width (1);

   for (auto iterator = annotation_tokens.begin ();
        iterator != annotation_tokens.end (); iterator++)
   {
      const string& annotation_str = *(iterator);
      render_annotation (cr, transform, annotation_str);
   }

   cr->restore ();

}

void
Display::render (const RefPtr<Context>& cr,
                 const Geodetic_Transform& transform,
                 const Size_2D& size_2d,
                 const Model& model,
                 const Hrit& hrit,
                 const Station::Map& station_map,
                 const Dtime& dtime,
                 const Level& level,
                 const Stage& stage,
                 const Product product,
                 const bool no_stage,
                 const bool no_wind_barb)
{

   cr->save ();

   //Color (0.86, 0.85, 0.47).cairo (cr);
   Checkered (Color::gray (0.55), Color::gray (0.45)).cairo (cr);
   cr->paint();

   render_product (cr, transform, size_2d, model, hrit,
      product, dtime, level, stage);

   if (!no_wind_barb)
   {
      render_wind_barbs (cr, transform, size_2d, model, dtime, level, stage);
   }

   // Stage 3/4/5 Frames
   if (!no_stage)
   {
      render_stages (cr, transform, model);
   }    

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
                                 const Journey& journey)
{
   Raster* raster_ptr = Display::get_cross_section_raster_ptr (box_2d,
      transform, model, stage, Product ("W_TRANSLUCENT"), dtime, journey);
   raster_ptr->blit (cr);
   delete raster_ptr;
}

void
Display::render_cross_section_mesh (const RefPtr<Context>& cr,
                                    const Transform_2D& transform,
                                    const Domain_1D& domain_z,
                                    const Journey& journey)
{

   const Geodesy geodesy;
   const Real distance = journey.get_distance (geodesy);

   Real d_distance = 100e3;
   if (distance < 1000e3) { d_distance = 50e3; }
   if (distance < 400e3) { d_distance = 20e3; }
   if (distance < 200e3) { d_distance = 10e3; }
   const Journey fj = Journey::fine_journey (journey, geodesy, d_distance);
   const Tuple& tuple_x = fj.get_tuple_x (geodesy);

   const Domain_1D domain_x (0, distance);
   const Domain_2D domain_2d (domain_x, domain_z);
   const Mesh_2D mesh_2d (Size_2D (2, 2),
      domain_2d, 1e8, 1000, Color::black (0.4));

   cr->set_line_width (2);
   mesh_2d.render (cr, transform);
   Color::black ().cairo (cr);

   //mesh_2d.render_label_x (cr, transform, 0, 0,
   //   "%.0f", NUMBER_REAL, 'c', 't', 5);
   mesh_2d.render_label_y (cr, transform, 0, 0,
      "%.0fm", NUMBER_REAL, 'r', 'c', 5);

   for (Tuple::const_iterator iterator = tuple_x.begin ();
        iterator != tuple_x.end (); iterator++)
   {

      const Real d = *(iterator);
      const Point_2D& point_a = transform.transform (Point_2D (d, 0));
      const Point_2D& point_b = transform.transform (Point_2D (d, 8000));

      const Integer i = std::distance (tuple_x.begin (), iterator);
      const string& str = string_render ("%d", i);

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
                                      const Journey& journey,
                                      const Real u_bg)
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
   const Real distance = journey.get_distance (geodesy);

   cr->set_line_width (1);

   for (Integer i = index_2d.i; i < index_2d.i + size_2d.i; i += Integer (h))
   {

      transform.reverse (x, z, Real (i), 0);
      if (x < 0 || x > distance) { continue; }

      const Lat_Long lat_long = journey.get_lat_long (x, geodesy);
      if (model.out_of_bounds (lat_long, stage)) { continue; }

      const Real topography = terrain_stage.get_topography (lat_long);
      const Real azimuth = journey.get_azimuth_forward (x, geodesy);
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
         Real uu = u * s + v * c - u_bg;
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
                               const Journey& journey,
                               const Real u_bg)
{

   cr->save ();

   Color::white ().cairo (cr);
   cr->paint ();

   Raster* raster_ptr = Display::get_cross_section_raster_ptr (box_2d,
      transform, model, stage, product, dtime, journey, u_bg);
   raster_ptr->blit (cr);
   delete raster_ptr;

   if (product.enumeration == Product::RHO ||
       product.enumeration == Product::THETA)
   {
      render_cross_section_w (cr, transform, box_2d,
         model, stage, dtime, journey);
   }

   render_cross_section_mesh (cr, transform, domain_z, journey);

   render_cross_section_arrows (cr, transform, box_2d, model, stage,
      product, dtime, journey, u_bg);

   if (u_bg != 0)
   {
      Color::black ().cairo (cr);
      const string fmt ("%.2f ms\u207b\u00b9 removed");
      const string& u_bg_string = string_render (fmt.c_str (), u_bg);
      const Index_2D anchor = box_2d.get_ne ();
      Label (u_bg_string, box_2d.get_ne (), 'r', 't', 10).cairo (cr);
   }

   cr->restore ();

}

void
Display::render_meteogram_mesh (const RefPtr<Context>& cr,
                                const Domain_1D& domain_t,
                                const Domain_1D& domain_temperature,
                                const Domain_1D& domain_direction,
                                const Domain_1D& domain_speed,
                                const Domain_1D& domain_pressure,
                                const Transform_2D& transform_temperature,
                                const Transform_2D& transform_direction,
                                const Transform_2D& transform_speed,
                                const Transform_2D& transform_pressure,
                                const bool ignore_pressure)
{

   cr->save ();

   const Real start_t = domain_t.start;
   const Real end_t = domain_t.end;
   const Real start_temperature = domain_temperature.start;
   const Real end_temperature = domain_temperature.end;
   const Real start_direction = domain_direction.start;
   const Real end_direction = domain_direction.end;
   const Real start_speed = domain_speed.start;
   const Real end_speed = domain_speed.end;
   const Real start_pressure = domain_pressure.start;
   const Real end_pressure = domain_pressure.end;

   const Point_2D& bl_temperature =
      transform_temperature.transform (start_t, start_temperature);
   const Point_2D& bl_direction =
      transform_direction.transform (start_t, start_direction);
   const Point_2D& bl_speed =
      transform_speed.transform (start_t, start_speed);
   const Point_2D& bl_pressure =
      transform_pressure.transform (start_t, start_pressure);

   const Point_2D& ur_temperature =
      transform_temperature.transform (end_t, end_temperature);
   const Point_2D& ur_direction =
      transform_direction.transform (end_t, end_direction);
   const Point_2D& ur_speed =
      transform_speed.transform (end_t, end_speed);
   const Point_2D& ur_pressure =
      transform_pressure.transform (end_t, end_pressure);

   Color::black (0.3).cairo (cr);

   const Color& minor_color = Color::black (0.10);
   const Color& middle_color = Color::black (0.25);
   const Color& major_color = Color::black (0.50);

   Mesh_2D mesh_temperature (Size_2D (2, 2),
      Domain_2D (domain_t, domain_temperature), 1, 1, minor_color,
      6, GSL_NAN, middle_color, 24, 10, major_color);
   Mesh_2D mesh_direction (Size_2D (2, 2),
      Domain_2D (domain_t, domain_direction), 1, 10, minor_color,
      6, GSL_NAN, middle_color, 24, 90, major_color);
   Mesh_2D mesh_speed (Size_2D (2, 2),
      Domain_2D (domain_t, domain_speed), 1, 1, minor_color,
      6, GSL_NAN, middle_color, 24, 5, major_color);
   Mesh_2D mesh_pressure (Size_2D (2, 2),
      Domain_2D (domain_t, domain_pressure), 1, 1, minor_color,
      6, GSL_NAN, middle_color, 24, 10, major_color);

   mesh_temperature.set_offset_multiplier_y (-K, 1);
   mesh_pressure.set_offset_multiplier_y (0, 1e-2);

   Rect (bl_temperature, ur_temperature).cairo (cr);
   cr->stroke ();
   mesh_temperature.render (cr, transform_temperature);

   Rect (bl_direction,   ur_direction  ).cairo (cr);
   cr->stroke ();
   mesh_direction.render (cr, transform_direction);

   Rect (bl_speed,       ur_speed      ).cairo (cr);
   cr->stroke ();
   mesh_speed.render (cr, transform_speed);

   if (!ignore_pressure)
   {
      Rect (bl_pressure,    ur_pressure   ).cairo (cr);
      cr->stroke ();
      mesh_pressure.render (cr, transform_pressure);
   }

   Color::black ().cairo (cr);

   mesh_temperature.render_label_y (cr, transform_temperature, 2,
      start_t, "%.0f\u00b0C", NUMBER_REAL, 'r', 'c', 5);
   mesh_temperature.render_label_x (cr, transform_temperature, 2,
      domain_temperature.end, "%b %d", NUMBER_TIME, 'c', 't', 15);
   mesh_temperature.render_label_x (cr, transform_temperature, 1,
      domain_temperature.end, "%HZ", NUMBER_TIME, 'c', 't', 5);

   mesh_direction.render_label_y (cr, transform_direction, 2,
      start_t, "%03.0f\u00b0", NUMBER_REAL, 'r', 'c', 5);
   mesh_direction.render_label_x (cr, transform_direction, 2,
      domain_direction.end, "%b %d", NUMBER_TIME, 'c', 't', 15);
   mesh_direction.render_label_x (cr, transform_direction, 1,
      domain_direction.end, "%HZ", NUMBER_TIME, 'c', 't', 5);

   mesh_speed.render_label_y (cr, transform_speed, 2,
      start_t, "%.0f ms\u207b\u00b9", NUMBER_REAL, 'r', 'c', 5);
   mesh_speed.render_label_x (cr, transform_speed, 2,
      domain_speed.end, "%b %d", NUMBER_TIME, 'c', 't', 15);
   mesh_speed.render_label_x (cr, transform_speed, 1,
      domain_speed.end, "%HZ", NUMBER_TIME, 'c', 't', 5);

   if (!ignore_pressure)
   {
      mesh_pressure.render_label_y (cr, transform_pressure, 2,
         start_t, "%.0fPa", NUMBER_REAL, 'r', 'c', 5);
      mesh_pressure.render_label_x (cr, transform_pressure, 2,
         domain_pressure.end, "%b %d", NUMBER_TIME, 'c', 't', 15);
      mesh_pressure.render_label_x (cr, transform_pressure, 1,
         domain_pressure.end, "%HZ", NUMBER_TIME, 'c', 't', 5);
   }

   cr->restore ();

}

void
Display::render_meteogram (const RefPtr<Context>& cr,
                           const Transform_2D& transform_temperature,
                           const Transform_2D& transform_direction,
                           const Transform_2D& transform_speed,
                           const Transform_2D& transform_pressure,
                           const Aws::Repository& aws_repository,
                           const bool faint,
                           const bool ignore_pressure)
{

   cr->save ();
   cr->set_line_width (0.2);

   const Real alpha = faint ? 0.04 : 1.0;
   const Real ring_size = faint ? 4.0 : 2.0;
   const Ring ring (ring_size);

   for (auto iterator = aws_repository.begin ();
        iterator != aws_repository.end (); iterator++)
   {

      const Aws::Key& key = iterator->first;
      const Aws::Obs& obs = iterator->second;
      const Dtime& dtime = key.dtime;
      const string& time_str = dtime.get_string ("%Y%m%d%H%M");

      const Real temperature = obs.temperature;
      const Point_2D p_temperature (dtime.t, temperature);
      const Point_2D& tp_temperature = transform_temperature.transform (p_temperature);
      ring.cairo (cr, tp_temperature);
      Color::hsb (0.000, 0.00, 0.40, alpha).cairo (cr);
      if (faint) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      const Real dew_point = obs.dew_point;
      const Point_2D p_dew_point (dtime.t, dew_point);
      const Point_2D& tp_dew_point = transform_temperature.transform (p_dew_point);
      ring.cairo (cr, tp_dew_point);
      Color::hsb (0.000, 0.80, 0.80, alpha).cairo (cr);
      if (faint) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      const Real direction = obs.wind_direction;
      const Point_2D p_direction (dtime.t, direction);
      const Point_2D& tp_direction = transform_direction.transform (p_direction);
      ring.cairo (cr, tp_direction);
      Color::hsb (0.167, 0.80, 0.60, alpha).cairo (cr);
      if (faint) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      const Real speed = obs.wind_speed;
      const Real gust = obs.wind_gust;
      const Point_2D p_speed (dtime.t, speed);
      const Point_2D& tp_speed = transform_speed.transform (p_speed);
      ring.cairo (cr, tp_speed);
      Color::hsb (0.333, 0.80, 0.80, alpha).cairo (cr);
      if (faint) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      const Point_2D p_gust (dtime.t, gust);
      const Point_2D& tp_gust = transform_speed.transform (p_gust);
      ring.cairo (cr, tp_gust);
      Color::hsb (0.500, 0.80, 0.80, alpha).cairo (cr);
      if (faint) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      if (ignore_pressure) { continue; }

      const Real mslp = obs.mslp;
      const Point_2D p_mslp (dtime.t, mslp);
      const Point_2D& tp_mslp = transform_pressure.transform (p_mslp);
      ring.cairo (cr, tp_mslp);
      Color::hsb (0.833, 0.80, 0.80, alpha).cairo (cr);
      if (faint) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

   }

   cr->restore ();

}

void
Display::render_meteogram (const RefPtr<Context>& cr,
                           const Size_2D& size_2d,
                           const Model& model, 
                           const Aws::Repository& aws_repository,
                           const Stage& stage, 
                           const Location& location,
                           const bool ignore_pressure)
{

   cr->save ();
   Color::white ().cairo (cr);
   cr->paint ();

   const Aws::Repository* model_aws_repository_ptr =
      model.get_aws_repository_ptr (location, stage);
   const Aws::Repository& model_aws_repository = *model_aws_repository_ptr;

   const Dtime& start_time = model.get_basetime ();
   const Dtime& end_time = model_aws_repository.rbegin ()->first.dtime;
   const Domain_1D domain_t (start_time.t, end_time.t);

   const Integer station_id = location.get_station_id ();

   const Dtime::Span time_span (start_time, end_time);
   const Aws::Repository* aws_repository_ptr = NULL;

   Real start_temperature = model_aws_repository.get_dew_point_domain ().start;
   Real end_temperature = model_aws_repository.get_temperature_domain ().end;
   Real end_speed = model_aws_repository.get_wind_speed_domain ().end;
   Real start_pressure = model_aws_repository.get_mslp_domain ().start;
   Real end_pressure = model_aws_repository.get_mslp_domain ().end;

   if (station_id > 0)
   {

      aws_repository_ptr =
         aws_repository.get_aws_repository_ptr (station_id, time_span);

      start_temperature = std::min (start_temperature,
         aws_repository_ptr->get_dew_point_domain ().start);
      end_temperature = std::max (end_temperature,
         aws_repository_ptr->get_temperature_domain ().end);

      end_speed = std::max (end_speed,
         aws_repository_ptr->get_wind_speed_domain ().end);

      start_pressure = std::min (start_pressure,
         aws_repository_ptr->get_mslp_domain ().start);
      end_pressure = std::max (end_pressure,
         aws_repository_ptr->get_mslp_domain ().end);

   }

   const Domain_1D domain_temperature (end_temperature, start_temperature);
   const Domain_1D domain_direction (360, 0);
   const Domain_1D domain_speed (end_speed, 0);
   const Domain_1D domain_pressure (end_pressure, start_pressure);

   const Real title_height = 40;
   const Real margin = 30;
   const Real margin_t = title_height + 10;
   const Real margin_b = 30;
   const Real margin_l = 55;
   const Real margin_r = 20;

   const Real n = (ignore_pressure ? 3 : 4);
   const Real width = size_2d.i - margin_l - margin_r;
   const Real height = (size_2d.j - margin_t - margin_b - (n - 1) * margin) / n;

   const Real span_t = domain_t.get_span ();
   const Real span_temperature = domain_temperature.get_span ();
   const Real span_direction = domain_direction.get_span ();
   const Real span_speed = domain_speed.get_span ();
   const Real span_pressure = domain_pressure.get_span ();

   const Real scale_t = width / span_t;
   const Real scale_temperature = height / span_temperature;
   const Real scale_speed = height / span_speed;
   const Real scale_direction = height / span_direction;
   const Real scale_pressure = height / span_pressure;

   const Real position_temperature = margin_t + 0 * height + 0 * margin;
   const Real position_direction   = margin_t + 1 * height + 1 * margin;
   const Real position_speed       = margin_t + 2 * height + 2 * margin;
   const Real position_pressure    = margin_t + 3 * height + 3 * margin;

   const Point_2D origin_temperature (margin_l, position_temperature);
   const Point_2D origin_direction (margin_l, position_direction);
   const Point_2D origin_speed (margin_l, position_speed);
   const Point_2D origin_pressure (margin_l, position_pressure);

   const Cartesian_Transform_2D transform_temperature (
      domain_t, domain_temperature, width, height, origin_temperature);
   const Cartesian_Transform_2D transform_direction (
      domain_t, domain_direction, width, height, origin_direction);
   const Cartesian_Transform_2D transform_speed (
      domain_t, domain_speed, width, height, origin_speed);
   const Cartesian_Transform_2D transform_pressure (
      domain_t, domain_pressure, width, height, origin_pressure);

   render_meteogram_mesh (cr, domain_t, domain_temperature,
      domain_direction, domain_speed, domain_pressure,
      transform_temperature, transform_direction,
      transform_speed, transform_pressure, ignore_pressure);

   if (aws_repository_ptr != NULL)
   {
      render_meteogram (cr, transform_temperature, transform_direction,
         transform_speed, transform_pressure, *aws_repository_ptr, true,
         ignore_pressure);
   }

   render_meteogram (cr, transform_temperature, transform_direction,
      transform_speed, transform_pressure, *model_aws_repository_ptr, false,
      ignore_pressure);

   delete aws_repository_ptr;
   delete model_aws_repository_ptr;

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

void
Display::render_vertical_profile (const RefPtr<Context>& cr,
                                  const Thermo_Diagram& thermo_diagram,
                                  const Model& model,
                                  const Stage& stage,
                                  const Dtime& dtime,
                                  const Lat_Long::List& lat_long_list)
{

   cr->save ();

   Color::white ().cairo (cr);
   cr->paint ();

   const Sounding* sounding_ptr = model.get_sounding_ptr (
      lat_long_list, dtime, stage, thermo_diagram);
   const Sounding& sounding = *sounding_ptr;

   cr->set_line_width (1);
   thermo_diagram.render (cr, 900);
   cr->set_line_width (2);
   sounding.render (cr, thermo_diagram);

   delete sounding_ptr;

   cr->restore ();

}

