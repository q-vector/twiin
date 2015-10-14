#include "display.h"
#include "hrit.h"
#include "model.h"
#include "obs.h"

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;

void
Display::set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
                    const Dtime& dtime,
                    const Level& level)
{

   const Real forecast_hour = dtime.t - basetime.t;
   const Integer forecast_minutes = Integer (round (forecast_hour * 60));
   const Integer fh = forecast_minutes / 60;
   const Integer fm = forecast_minutes % 60;

   const Dstring fmt ("%Y.%m.%d %H:%M UTC");
   const Dstring& time_str = dtime.get_string (fmt);
   const Dstring& basetime_str = basetime.get_string () +
      Dstring::render (" +%d:%02d", fh, fm);
   const Dstring stage_product = stage_str + " / " + product.get_description ();

   title.set (time_str, "", stage_product, basetime_str, level.get_string ());

}

void
Display::set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
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

   const Dstring o_suffix (complex_j ? " ..." : "");
   const Dstring d_preffix (complex_j ? "... " : "");
   const Dstring& o_str = orig.get_string (4, true, true, true) + o_suffix;
   const Dstring& d_str = d_preffix + dest.get_string (4, true, true, true);

   const Dstring fmt ("%Y.%m.%d %H:%M UTC");
   const Dstring& time_str = dtime.get_string (fmt);
   const Dstring& basetime_str = basetime.get_string () +
      Dstring::render (" +%d:%02d", fh, fm);
   const Dstring stage_product = stage_str + " / " + product.get_description ();

   title.set (time_str, o_str, stage_product, basetime_str, d_str);

}

void
Display::set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
                    const Dtime& dtime,
                    const Lat_Long& lat_long,
                    const Real distance,
                    const bool lagrangian)
{

   const Real forecast_hour = dtime.t - basetime.t;
   const Integer forecast_minutes = Integer (round (forecast_hour * 60));
   const Integer fh = Integer (round (forecast_hour * 60)) / 60;
   const Integer fm = Integer (round (forecast_hour * 60)) % 60;

   const Real d = distance * 1e-3;
   const Dstring& ll_str = lat_long.get_string (4, true, true, true);
   const Dstring& where_str = ll_str + Dstring::render ("\u00b1%.0f km", d);

   const Dstring pov_str (lagrangian ? "Lagrangian" : "Eulerian");

   const Dstring fmt ("%Y.%m.%d %H:%M UTC");
   const Dstring& time_str = dtime.get_string (fmt);
   const Dstring& basetime_str = basetime.get_string () +
      Dstring::render (" +%d:%02d", fh, fm);
   const Dstring stage_product = stage_str + " / " + product.get_description ();

   title.set (time_str, where_str, stage_product, basetime_str, pov_str);

}

void
Display::set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
                    const Dstring& track_id,
                    const bool lagrangian)
{
   const Dstring pov_str (lagrangian ? "Lagrangian" : "Eulerian");
   const Dstring& basetime_str = basetime.get_string ();
   const Dstring stage_product = stage_str + " / " + product.get_description ();
   title.set (track_id, "", stage_product, basetime_str, pov_str);
}

void
Display::set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Dtime& dtime,
                    const Location& location)
{

   const Real forecast_hour = dtime.t - basetime.t;
   const Integer forecast_minutes = Integer (round (forecast_hour * 60));
   const Integer fh = forecast_minutes / 60;
   const Integer fm = forecast_minutes % 60;

   const Lat_Long& lat_long (location);
   const Dstring& ll_str = lat_long.get_string (4, true, true, true);

   const Dstring fmt ("%Y.%m.%d %H:%M UTC");
   const Dstring& time_str = dtime.get_string (fmt);
   const Dstring& basetime_str = basetime.get_string () +
      Dstring::render (" +%d:%02d", fh, fm);

   title.set (time_str, stage_str, location.get_long_str (),
      basetime_str, ll_str);

}

void
Display::set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Dtime& dtime,
                    const Dstring& location_name)
{

   const Real forecast_hour = dtime.t - basetime.t;
   const Integer forecast_minutes = Integer (round (forecast_hour * 60));
   const Integer fh = forecast_minutes / 60;
   const Integer fm = forecast_minutes % 60;

   const Dstring fmt ("%Y.%m.%d %H:%M UTC");
   const Dstring& time_str = dtime.get_string (fmt);
   const Dstring& basetime_str = basetime.get_string () +
      Dstring::render (" +%d:%02d", fh, fm);

   title.set (time_str, stage_str, location_name, basetime_str, "");

}

void
Display::set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Location& location)
{
   const Dstring& basetime_str = basetime.get_string ();
   const Lat_Long& lat_long (location);
   const Dstring& ll_str = lat_long.get_string (4, true, true, true);
   title.set ("", stage_str, location.get_long_str (), basetime_str, ll_str);
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
      const Dstring& stage_str = *(iterator);
      const Model::Stage& stage = model.get_stage (stage_str);
      const Domain_2D& domain_2d = stage.get_domain_2d ();
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
                         const Model::Product& product,
                         const Dtime& dtime,
                         const Level& level,
                         const Dstring& stage_str)
{

   Raster* raster_ptr = NULL;
   const Model::Stage& stage = model.get_stage (stage_str);

   if (product.enumeration == Model::Product::TERRAIN)
   {
      raster_ptr = stage.get_terrain_raster_ptr (size_2d, transform);
   }
   else
   if (product.enumeration == Model::Product::FFDI ||
       product.enumeration == Model::Product::MSLP ||
       product.enumeration == Model::Product::PRECIP_RATE)
   {
      raster_ptr = stage.get_surface_raster_ptr (
         size_2d, transform, product, dtime);
   }
   else
   if (product.enumeration == Model::Product::IR1 ||
       product.enumeration == Model::Product::IR2 ||
       product.enumeration == Model::Product::IR3 ||
       product.enumeration == Model::Product::IR4 ||
       product.enumeration == Model::Product::VIS ||
       product.enumeration == Model::Product::Pseudo)
   {
      const Dstring& channel_str = product.get_string ();
      raster_ptr = hrit.get_raster_ptr (size_2d, transform, channel_str, dtime);
   }
   else
   {
      if (level.type == Level::SURFACE)
      {
         raster_ptr = stage.get_surface_raster_ptr (size_2d,
            transform, product, dtime);
      }
      else
      {
         raster_ptr = stage.get_uppers_raster_ptr (size_2d,
            transform, product, dtime, level);
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
                            const Dstring& stage_str)
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

   const Model::Stage& stage = model.get_stage (stage_str);

   for (point.x = start_x; point.x < width; point.x += h)
   {
      for (point.y = start_y; point.y < height; point.y += h)
      {

         transform.reverse (latitude, longitude, point.x, point.y);
         if (stage.out_of_bounds (lat_long)) { continue; }

         Real u = stage.evaluate (U, ll, level, dtime);
         Real v = stage.evaluate (V, ll, level, dtime);
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
   Rect (point - Point_2D (margin, bar_height/2 + font_size + margin),
      2 * (pixels + margin), bar_height + 2 * (font_size + margin)).cairo (cr);
   cr->fill ();

   Color::black ().cairo (cr);
   Rect (point - Point_2D (0, bar_height/2), 2 * pixels, bar_height).cairo (cr);
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
      const Real tick_length = fabs ((bar_length - i * bar_length/5) * 1e-3);
      const Dstring& str = Dstring::render ("%.0f", tick_length);
      Label (str, Point_2D (x, point.y), 'c', 't', bar_height*1.5).cairo (cr);
   }

   const Point_2D p (point.x + pixels, point.y);
   Label ("0", p, 'c', 't', bar_height * 1.5).cairo (cr);
   Label (Dstring::render ("%.0f", bar_length*1e-3),
      point + Point_2D (2 * pixels, 0), 'c', 't', bar_height*1.5).cairo (cr);
   Label ("kilometres", p, 'c', 'b', bar_height*1.5).cairo (cr);

   cr->restore ();

}

void
Display::render_color_bar (const RefPtr<Context>& cr,
                           const Model::Product& product,
                           const Tuple& tick_tuple,
                           const Box_2D& box_2d,
                           const bool negative)
{

   const Real font_size = 12;
   const Real sign = (negative ? -1.0 : 1.0);
   const Dstring& unit = product.get_unit ();

   const Integer bar_x = box_2d.index_2d.i;
   const Integer bar_y = box_2d.index_2d.j;
   const Integer bar_width = box_2d.size_2d.i;
   const Integer bar_height = box_2d.size_2d.j;

   const bool is_log = (product.enumeration == Model::Product::Q_TENDENCY ||
                        product.enumeration == Model::Product::Q_ADVECTION ||
                        product.enumeration == Model::Product::Q_H_ADVECTION ||
                        product.enumeration == Model::Product::Q_V_ADVECTION ||
                        product.enumeration == Model::Product::Q_S_ADVECTION ||
                        product.enumeration == Model::Product::Q_N_ADVECTION ||
                        product.enumeration == Model::Product::Q_SV_ADVECTION ||
                        product.enumeration == Model::Product::Q_NV_ADVECTION ||
                        product.enumeration == Model::Product::PRECIP_RATE ||
                        product.enumeration == Model::Product::BRUNT_VAISALA);

   const Real start_value = tick_tuple.front ();
   const Real end_value = tick_tuple.back ();
   const Domain_1D value_domain (start_value, end_value);

   const Real start_y = bar_y + (negative ? 0 : bar_height);
   const Real end_y = bar_y + (negative ? bar_height : 0);
   const Domain_1D y_domain (start_y, end_y);
   const Cartesian_Transform_1D transform (value_domain, y_domain, is_log);

   Raster* raster_ptr = new Raster (box_2d);
   Raster& raster = *raster_ptr;

   for (Integer j = 0; j < bar_height; j++)
   {
      const Real y = bar_y + j;
      const Real value = transform.reverse (y) * sign; 
      const Color& color = Model::Stage::get_color (product, value, unit);
      for (Integer i = 0; i < bar_width; i++)
      {
         raster.set_pixel (i, j, color);
      }
   }

   raster_ptr->blit (cr);
   delete raster_ptr;

   Color::black ().cairo (cr);

   for (auto iterator = tick_tuple.begin ();
        iterator != tick_tuple.end (); iterator++)
   {
      if (negative && iterator == tick_tuple.begin ()) { continue; }
      const Real tick_value = *(iterator);
      const Real y = transform.transform (tick_value);
      const Point_2D point (bar_x, y);
      const Dstring& str = Dstring::render ("%g", tick_value * sign);
      Label (str, point, 'r', 'c', font_size/2).cairo (cr);
   }

}

void
Display::render_color_bar (const RefPtr<Context>& cr,
                           const Size_2D& size_2d,
                           const Model::Product& product)
{

   const Dstring& unit = product.get_unit ();
   const Tuple& tick_tuple = product.get_tick_tuple ();
   if (tick_tuple.size () < 2) { return; }

   const Real title_height = 40;
   const Real margin = 10;
   const Real font_size = 12;

   const Real box_width = 80;
   const Real box_height = size_2d.j - title_height - 2*margin;
   const Real box_x = size_2d.i - box_width - margin;
   const Real box_y = title_height + margin;
   const Point_2D box_point (box_x, box_y);

   const Integer bar_width = box_width - 5 * margin;
   const Integer bar_height = box_height - 2 * margin - font_size;
   const Integer bar_x = box_x + 4 * margin;
   const Integer bar_y = box_y + margin + font_size;

   cr->save ();
   cr->set_line_width (1);
   cr->set_font_size (font_size);

   Color::white (0.7).cairo (cr);
   Rect (box_point, box_width, box_height).cairo (cr);
   cr->fill ();

   if (product.enumeration == Model::Product::Q_TENDENCY ||
       product.enumeration == Model::Product::Q_ADVECTION ||
       product.enumeration == Model::Product::Q_H_ADVECTION ||
       product.enumeration == Model::Product::Q_V_ADVECTION ||
       product.enumeration == Model::Product::Q_S_ADVECTION ||
       product.enumeration == Model::Product::Q_N_ADVECTION ||
       product.enumeration == Model::Product::Q_SV_ADVECTION ||
       product.enumeration == Model::Product::Q_NV_ADVECTION)
   {
      const Index_2D p_index_2d (bar_x, bar_y);
      const Index_2D n_index_2d (bar_x, bar_y + bar_height / 2);
      const Box_2D p_box_2d (p_index_2d, Size_2D (bar_width, bar_height / 2));
      const Box_2D n_box_2d (n_index_2d, Size_2D (bar_width, bar_height / 2));
      render_color_bar (cr, product, tick_tuple, p_box_2d, false);
      render_color_bar (cr, product, tick_tuple, n_box_2d, true);
   }
   else
   {
      const Index_2D index_2d (bar_x, bar_y);
      const Box_2D box_2d (index_2d, Size_2D (bar_width, bar_height));
      render_color_bar (cr, product, tick_tuple, box_2d);
   }

   Color::black ().cairo (cr);
   Rect (Point_2D (bar_x, bar_y), bar_width, bar_height).cairo (cr);
   cr->stroke ();

   const Real label_x = bar_x + bar_width / 2;
   const Real label_y = bar_y;
   const Point_2D label_point (label_x, label_y);
   Label (unit, label_point, 'c', 'b', font_size/2).cairo (cr);

   cr->restore ();

}

void
Display::render_annotation_point (const RefPtr<Context>& cr,
                                  const Geodetic_Transform& transform,
                                  const Lat_Long& lat_long,
                                  const Dstring& str)
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
                            const Dstring& annotation_str)
{

   const Tokens tokens (annotation_str, ":");
   const Dstring& genre = tokens[0];

   if (genre == "location" ||
       genre == "lat_long" ||
       genre == "l")
   {
      const Location location (tokens[1]);
      const Dstring& str = (tokens.size () > 2 ? tokens[2] : Dstring (""));
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
      const Dstring& annotation_str = *(iterator);
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
                 const Dstring& stage_str,
                 const Model::Product product,
                 const bool no_stage,
                 const bool no_wind_barb)
{

   cr->save ();

   //Color (0.86, 0.85, 0.47).cairo (cr);
   Checkered (Color::gray (0.55), Color::gray (0.45)).cairo (cr);
   cr->paint();

   render_product (cr, transform, size_2d, model, hrit,
      product, dtime, level, stage_str);

   if (!no_wind_barb)
   {
      render_wind_barbs (cr, transform, size_2d,
         model, dtime, level, stage_str);
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
                                 const Model::Stage& stage,
                                 const Dtime& dtime,
                                 const Journey& journey)
{
   Raster* raster_ptr = stage.get_cross_section_raster_ptr (box_2d,
      transform, Model::Product ("W"), dtime, journey);
   raster_ptr->blit (cr, 0.4);
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
   //   "%.0f", Label::REAL, 'c', 't', 5);
   mesh_2d.render_label_y (cr, transform, 0, 0,
      "%.0fm", Label::REAL, 'r', 'c', 5);

   for (Tuple::const_iterator iterator = tuple_x.begin ();
        iterator != tuple_x.end (); iterator++)
   {

      const Real d = *(iterator);
      const Point_2D& point_a = transform.transform (Point_2D (d, 0));
      const Point_2D& point_b = transform.transform (Point_2D (d, 8000));

      const Integer i = std::distance (tuple_x.begin (), iterator);
      const Dstring& str = Dstring::render ("%d", i);

      Label (str, point_a, 'c', 't', 6).cairo (cr);

   }

}

void
Display::render_cross_section_arrows (const RefPtr<Context>& cr,
                                      const Transform_2D& transform,
                                      const Box_2D& box_2d,
                                      const Model::Stage& stage,
                                      const Model::Product& product,
                                      const Dtime& dtime,
                                      const Journey& journey,
                                      const Real u_bg)
{

   Real x;
   Level level (Level::HEIGHT, GSL_NAN);
   Real& z = level.value;

   const Real arrow_size = 15;
   const Real h = arrow_size * 1.5;

   Color::black (0.2).cairo (cr);
   const size_t l = stage.get_uppers_l (dtime);

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
      if (stage.out_of_bounds (lat_long)) { continue; }

      const Real topography = stage.get_topography (lat_long);
      const Real azimuth = journey.get_azimuth_forward (x, geodesy);
      const Real theta = azimuth * DEGREE_TO_RADIAN;
      const Real c = cos (theta);
      const Real s = sin (theta);

      for (Integer j = index_2d.j; j < index_2d.j + size_2d.j; j += Integer (h))
      {

         transform.reverse (x, z, Real (i), Real (j));
         if (z < topography) { continue; }

         const Real u = stage.evaluate (U, lat_long, level, l);
         const Real v = stage.evaluate (V, lat_long, level, l);
         Real w = stage.evaluate (W, lat_long, level, l);
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
                               const Model::Stage& stage,
                               const Model::Product& product,
                               const Dtime& dtime,
                               const Journey& journey,
                               const Real u_bg)
{

   cr->save ();

   Color::white ().cairo (cr);
   cr->paint ();

   Raster* raster_ptr = stage.get_cross_section_raster_ptr (box_2d,
      transform, product, dtime, journey, u_bg);
   raster_ptr->blit (cr);
   delete raster_ptr;

   if (product.enumeration == Model::Product::RHO ||
       product.enumeration == Model::Product::THETA)
   {
      //render_cross_section_w (cr, transform, box_2d, stage, dtime, journey);
   }

   render_cross_section_mesh (cr, transform, domain_z, journey);

   render_cross_section_arrows (cr, transform, box_2d, stage,
      product, dtime, journey, u_bg);

   if (u_bg != 0)
   {
      Color::black ().cairo (cr);
      const Dstring fmt ("%.2f ms\u207b\u00b9 removed");
      const Dstring& u_bg_string = Dstring::render (fmt, u_bg);
      const Index_2D anchor = box_2d.get_ne ();
      Label (u_bg_string, box_2d.get_ne (), 'r', 't', 10).cairo (cr);
   }

   cr->restore ();

}

void
Display::render_time_cross_w (const RefPtr<Context>& cr,
                              const Transform_2D& transform,
                              const Box_2D& box_2d,
                              const Model::Stage& stage,
                              const Track& track)
{
   Raster* raster_ptr = stage.get_time_cross_raster_ptr (box_2d,
      transform, Model::Product ("W_TRANSLUCENT"), track);
   raster_ptr->blit (cr);
   delete raster_ptr;
}

void
Display::render_time_cross_mesh (const RefPtr<Context>& cr,
                                 const Transform_2D& transform,
                                 const Domain_1D& domain_t,
                                 const Domain_1D& domain_z)
{

   cr->save ();

   const Point_2D& bl = transform.transform (domain_t.start, domain_z.start);
   const Point_2D& ur = transform.transform (domain_t.end, domain_z.end);

   Color::black (0.3).cairo (cr);

   const Color& minor_color = Color::black (0.10);
   const Color& middle_color = Color::black (0.25);
   const Color& major_color = Color::black (0.50);

   Mesh_2D mesh (Size_2D (2, 2), Domain_2D (domain_t, domain_z),
      1, GSL_NAN, minor_color,
      6, GSL_NAN, middle_color,
      24, 1000, major_color);

   Rect (bl, ur).cairo (cr);
   cr->stroke ();
   mesh.render (cr, transform);

   Color::black ().cairo (cr);

   mesh.render_label_y (cr, transform, 2,
      domain_t.start, "%.0fm", Label::REAL, 'r', 'c', 5);
   mesh.render_label_x (cr, transform, 2,
      domain_z.start, "%b %d", Label::TIME, 'c', 't', 15);
   mesh.render_label_x (cr, transform, 1,
      domain_z.start, "%HZ", Label::TIME, 'c', 't', 5);

   cr->restore ();

}

void
Display::render_time_cross_track (const RefPtr<Context>& cr,
                                  const Transform_2D& transform,
                                  const Track& track)
{

   const Dtime& epoch = track.get_epoch ();
   const Real start_t = epoch.t + track.get_start_tau ();
   const Real end_t = epoch.t + track.get_end_tau ();
   const Integer n = Integer (round ((end_t - start_t) / 0.025));
   const Tuple t_tuple (n, start_t, end_t);

   Simple_Polyline trace;

   for (auto iterator = t_tuple.begin ();
        iterator != t_tuple.end (); iterator++)
   {
      const Dtime dtime (*iterator);
      if (dtime.t < start_t || dtime.t > end_t) { continue; }
      const Real z = track.get_datum ("z", dtime);
      trace.add (Point_2D (dtime.t, z));
   }

   cr->save ();
   cr->set_line_width (1);
   Color::black (0.5).cairo (cr);
   trace.cairo (cr, transform);
   cr->stroke ();
   cr->restore ();

}

void
Display::render_time_cross (const RefPtr<Context>& cr,
                            const Transform_2D& transform,
                            const Box_2D& box_2d,
                            const Domain_1D& domain_z,
                            const Model::Stage& stage,
                            const Model::Product& product,
                            const Track& track,
                            const bool lagrangian)
{

   cr->save ();

   Color::white ().cairo (cr);
   cr->paint ();

   Raster* raster_ptr = stage.get_time_cross_raster_ptr (
      box_2d, transform, product, track, lagrangian);
   raster_ptr->blit (cr);
   delete raster_ptr;

   if (product.enumeration == Model::Product::RHO ||
       product.enumeration == Model::Product::THETA)
   {
      render_time_cross_w (cr, transform, box_2d, stage, track);
   }

   const Real start_t = track.get_start_time ().t;
   const Real end_t = track.get_end_time ().t;
   const Domain_1D domain_t (start_t, end_t);
   render_time_cross_mesh (cr, transform, domain_t, domain_z);

   render_time_cross_track (cr, transform, track); 

   //render_time_cross_arrows (cr, transform, box_2d, stage,
   //   product, dtime, journey, u_bg);

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

   Rect (bl_direction, ur_direction).cairo (cr);
   cr->stroke ();
   mesh_direction.render (cr, transform_direction);

   Rect (bl_speed, ur_speed).cairo (cr);
   cr->stroke ();
   mesh_speed.render (cr, transform_speed);

   if (!ignore_pressure)
   {
      Rect (bl_pressure, ur_pressure).cairo (cr);
      cr->stroke ();
      mesh_pressure.render (cr, transform_pressure);
   }

   Color::black ().cairo (cr);

   mesh_temperature.render_label_y (cr, transform_temperature, 2,
      start_t, "%.0f\u00b0C", Label::REAL, 'r', 'c', 5);
   mesh_temperature.render_label_x (cr, transform_temperature, 2,
      domain_temperature.end, "%b %d", Label::TIME, 'c', 't', 15);
   mesh_temperature.render_label_x (cr, transform_temperature, 1,
      domain_temperature.end, "%HZ", Label::TIME, 'c', 't', 5);

   mesh_direction.render_label_y (cr, transform_direction, 2,
      start_t, "%03.0f\u00b0", Label::REAL, 'r', 'c', 5);
   mesh_direction.render_label_x (cr, transform_direction, 2,
      domain_direction.end, "%b %d", Label::TIME, 'c', 't', 15);
   mesh_direction.render_label_x (cr, transform_direction, 1,
      domain_direction.end, "%HZ", Label::TIME, 'c', 't', 5);

   mesh_speed.render_label_y (cr, transform_speed, 2,
      start_t, "%.0f ms\u207b\u00b9", Label::REAL, 'r', 'c', 5);
   mesh_speed.render_label_x (cr, transform_speed, 2,
      domain_speed.end, "%b %d", Label::TIME, 'c', 't', 15);
   mesh_speed.render_label_x (cr, transform_speed, 1,
      domain_speed.end, "%HZ", Label::TIME, 'c', 't', 5);

   if (!ignore_pressure)
   {
      mesh_pressure.render_label_y (cr, transform_pressure, 2,
         start_t, "%.0fPa", Label::REAL, 'r', 'c', 5);
      mesh_pressure.render_label_x (cr, transform_pressure, 2,
         domain_pressure.end, "%b %d", Label::TIME, 'c', 't', 15);
      mesh_pressure.render_label_x (cr, transform_pressure, 1,
         domain_pressure.end, "%HZ", Label::TIME, 'c', 't', 5);
   }

   cr->restore ();

}

void
Display::render_meteogram (const RefPtr<Context>& cr,
                           const Transform_2D& t_temperature,
                           const Transform_2D& t_direction,
                           const Transform_2D& t_speed,
                           const Transform_2D& t_pressure,
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
      const Dstring& time_str = dtime.get_string ("%Y%m%d%H%M");

      const Real temperature = obs.temperature;
      const Point_2D p_temperature (dtime.t, temperature);
      const Point_2D& tp_temperature = t_temperature.transform (p_temperature);
      ring.cairo (cr, tp_temperature);
      Color::hsb (0.000, 0.00, 0.40, alpha).cairo (cr);
      if (faint) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      const Real dew_point = obs.dew_point;
      const Point_2D p_dew_point (dtime.t, dew_point);
      const Point_2D& tp_dew_point = t_temperature.transform (p_dew_point);
      ring.cairo (cr, tp_dew_point);
      Color::hsb (0.000, 0.80, 0.80, alpha).cairo (cr);
      if (faint) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      const Real direction = obs.wind_direction;
      const Point_2D p_direction (dtime.t, direction);
      const Point_2D& tp_direction = t_direction.transform (p_direction);
      ring.cairo (cr, tp_direction);
      Color::hsb (0.167, 0.80, 0.60, alpha).cairo (cr);
      if (faint) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      const Real speed = obs.wind_speed;
      const Real gust = obs.wind_gust;
      const Point_2D p_speed (dtime.t, speed);
      const Point_2D& tp_speed = t_speed.transform (p_speed);
      ring.cairo (cr, tp_speed);
      Color::hsb (0.333, 0.80, 0.80, alpha).cairo (cr);
      if (faint) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      const Point_2D p_gust (dtime.t, gust);
      const Point_2D& tp_gust = t_speed.transform (p_gust);
      ring.cairo (cr, tp_gust);
      Color::hsb (0.500, 0.80, 0.80, alpha).cairo (cr);
      if (faint) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      if (ignore_pressure) { continue; }

      const Real mslp = obs.mslp;
      const Point_2D p_mslp (dtime.t, mslp);
      const Point_2D& tp_mslp = t_pressure.transform (p_mslp);
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
                           const Model::Stage& stage, 
                           const Aws::Repository& aws_repository,
                           const Location& location,
                           const bool ignore_pressure)
{

   cr->save ();
   Color::white ().cairo (cr);
   cr->paint ();

   const Aws::Repository* model_aws_repository_ptr =
      stage.get_aws_repository_ptr (location);
   const Aws::Repository& model_aws_repository = *model_aws_repository_ptr;

   const Dtime& start_time = stage.get_basetime ();
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
                                  const Model::Stage& stage,
                                  const Dtime& dtime,
                                  const Lat_Long& lat_long,
                                  const Level& level)
{

   cr->save ();

   Color::white ().cairo (cr);
   cr->paint ();

   const Sounding* sounding_ptr = stage.get_sounding_ptr (lat_long, dtime);
   const Sounding& sounding = *sounding_ptr;

   cr->set_line_width (1);
   thermo_diagram.render (cr, 900);
   cr->set_line_width (2);
   sounding.render (cr, thermo_diagram);

   Real p = GSL_NAN;

   switch (level.type)
   {
      case Level::PRESSURE: { p = level.value;                         break; }
      case Level::HEIGHT:   { p = sounding.get_pressure (level.value); break; }
   }

   const Real as = 20;
   const Real t = sounding.get_temperature (thermo_diagram, p);
   const Real td = sounding.get_dew_point (thermo_diagram, p);
   const Thermo_Point& tp_t = Thermo_Point::t_p (t, p);
   const Thermo_Point& tp_td = Thermo_Point::t_p (td, p);
   const Point_2D& p_t = thermo_diagram.transform (tp_t) + Point_2D (as, 0);
   const Point_2D& p_td = thermo_diagram.transform (tp_td) + Point_2D (-as, 0);

   Color::hsb (0.33, 0.5, 0.5, 0.5).cairo (cr);

   const Arrow arrow_t (M_PI, as);
   arrow_t.cairo (cr, p_t);
   cr->fill ();

   const Arrow arrow_td (0, as);
   arrow_td.cairo (cr, p_td);
   cr->fill ();

   delete sounding_ptr;

   cr->restore ();

}

void
Display::render_vertical_profile (const RefPtr<Context>& cr,
                                  const Thermo_Diagram& thermo_diagram,
                                  const Model::Stage& stage,
                                  const Dtime& dtime,
                                  const Lat_Long::List& lat_long_list)
{

   cr->save ();

   Color::white ().cairo (cr);
   cr->paint ();

   const Sounding* sounding_ptr = stage.get_sounding_ptr (
      lat_long_list, dtime, thermo_diagram);
   const Sounding& sounding = *sounding_ptr;

   cr->set_line_width (1);
   thermo_diagram.render (cr, 900);
   cr->set_line_width (2);
   sounding.render (cr, thermo_diagram);

   delete sounding_ptr;

   cr->restore ();

}

