#include <getopt.h>
#include <denise/dstring.h>
#include "twiin.h"
#include "hrit.h"
#include "model.h"
#include "obs.h"

#ifndef ENABLE_GTKMM
#else /* ENABLE_GTKMM */
#include "cross_section.h"
#endif /* ENABLE_GTKMM */

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;


Twiin::Transform_Ptr_Map::Transform_Ptr_Map (const Size_2D& size_2d,
                                             const Config_File& config_file)
{

   typedef Geodetic_Transform Gt;
   const Point_2D centre (size_2d.i/2, size_2d.j/2);

   for (auto iterator = config_file.begin ();
        iterator != config_file.end (); iterator++)
   {

      const Tokens tokens (*(iterator), " \f\t\n");
      if (tokens.size () != 3) { continue; }
      if (tokens[0] != "geodetic_transform") { continue; }

      const Dstring& identifier = tokens[1];
      const Dstring& zoom_str = tokens[2];
      Gt* gt_ptr = Gt::get_transform_ptr (zoom_str, centre);
      insert (make_pair (identifier, gt_ptr));

   }

}

Twiin::Transform_Ptr_Map::~Transform_Ptr_Map ()
{

   typedef Geodetic_Transform Gt;
   for (auto iterator = begin (); iterator != end (); iterator++)
   {
      Gt* gt_ptr = iterator->second;
      delete gt_ptr;
   }

}
Dstring
Twiin::get_file_path (const Dstring& format,
                      const Dstring& stage,
                      const Model::Product& product,
                      const Level& level,
                      const Dtime& dtime) const
{
   const Dstring& time_str = dtime.get_string ("%Y%m%d%H%M");
   const Dstring& file_name = stage + "-" + product.get_string () + "-" +
      level.get_string () + "-" + time_str + "." + format;
   return output_dir + "/" + file_name;
}

Dstring
Twiin::get_file_path (const Dstring& format,
                      const Dstring& stage,
                      const Model::Product& product,
                      const Dtime& dtime,
                      const Journey& journey) const
{

   const Lat_Long origin (journey.front ());
   const Lat_Long destination (journey.back ());

   Dstring j_str;
   for (auto iterator = journey.begin ();
        iterator != journey.end (); iterator++)
   {
      const Lat_Long lat_long (*(iterator));
      const Dstring& ll_str = lat_long.get_string (false, Dstring ("%.4f"));
      j_str += ll_str;
   }

   const Dstring& time_str = dtime.get_string ("%Y%m%d%H%M");
   const Dstring& file_name = stage + "-" + product.get_string () + "-" +
      time_str + "-" + j_str + "." + format;
   return output_dir + "/" + file_name;

}

Dstring
Twiin::get_file_path (const Dstring& format,
                      const Dstring& stage,
                      const Model::Product& product,
                      const Dtime& dtime,
                      const Lat_Long& lat_long,
                      const Real distance,
                      const bool lagrangian) const
{
   const Dstring pov_str (lagrangian ? "LAGRANGIAN" : "EULERIAN");
   const Dstring& time_str = dtime.get_string ("%Y%m%d%H%M");
   const Dstring& ll_str = lat_long.get_string (false, Dstring ("%.4f"));

   const Real d = distance * 1e-3;
   const Dstring& where_str = ll_str + Dstring::render ("-%.0fkm", d);
   const Dstring& file_name = stage + "-" + product.get_string () + "-" +
      time_str + "-" + where_str + "-" + pov_str + + "." + format;
   return output_dir + "/" + file_name;
}

Dstring
Twiin::get_file_path (const Dstring& format,
                      const Dstring& stage,
                      const Model::Product& product,
                      const Location& location) const
{
   const Dstring& location_str = location.get_str ();
   const Dstring& file_name = stage + "-" + product.get_string () + "-" +
      location_str + "." + format;
   return output_dir + "/" + file_name;
}

Dstring
Twiin::get_file_path (const Dstring& format,
                      const Dstring& stage,
                      const Model::Product& product,
                      const Dstring& track_id,
                      const bool lagrangian) const
{
   const Dstring pov_str (lagrangian ? "LAGRANGIAN" : "EULERIAN");
   const Dstring& file_name = stage + "-" + product.get_string () + "-" +
      track_id + "-" + pov_str + "." + format;
   return output_dir + "/" + file_name;
}

Dstring
Twiin::get_file_path (const Dstring& format,
                      const Dstring& stage,
                      const Model::Product& product,
                      const Level& level,
                      const Dtime& dtime,
                      const Dstring& track_id) const
{
   const Dstring& time_str = dtime.get_string ("%Y%m%d%H%M");
   const Dstring& file_name = stage + "-" + product.get_string () + "-" +
      time_str + "-" + track_id + "-" + level.get_string () + "." + format;
   return output_dir + "/" + file_name;
}

Dstring
Twiin::get_file_path (const Dstring& format,
                      const Dstring& stage,
                      const Dtime& dtime,
                      const Location& location) const
{
   const Dstring& location_str = location.get_str ();
   const Dstring& time_str = dtime.get_string ("%Y%m%d%H%M");
   const Dstring& file_name = stage + "-" + time_str + "-"
      + location_str + "." + format;
   return output_dir + "/" + file_name;
}

Dstring
Twiin::get_file_path (const Dstring& format,
                      const Dstring& stage,
                      const Dtime& dtime,
                      const Dstring& location_name) const
{
   const Dstring& time_str = dtime.get_string ("%Y%m%d%H%M");
   const Dstring& file_name = stage + "-" + time_str + "-"
      + location_name + "." + format;
   return output_dir + "/" + file_name;
}

Dstring
Twiin::get_file_path (const Dstring& format,
                      const Dstring& stage,
                      const Location& location) const
{
   const Dstring& location_str = location.get_str ();
   const Dstring& file_name = stage + "-" + location_str + "." + format;
   return output_dir + "/" + file_name;
}

Twiin::Twiin (const Size_2D& size_2d,
              const Config_File& config_file,
              const Dstring& output_dir)
   : size_2d (size_2d),
     config_file (config_file),
     output_dir (output_dir)
{
}

#ifndef ENABLE_GTKMM
#else /* ENABLE_GTKMM */
void
Twiin::gui (const Dstring& stage_str,
            const Dstring& product_str,
            const Dstring& level_str,
            const Dstring& time_str,
            const Tokens& journey_tokens,
            const Dstring& zoom_str) const
{

   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");
   const Tokens level_tokens (level_str, ":");

   const Dstring stage (stage_tokens[0]);
   const Model::Product product (product_tokens[0]);
   const Level level (level_tokens[0]);

   const Dtime::Set time_set (time_str);
   const Dtime& dtime = time_set.get_start ();

   const Data data (config_file);

   Gtk::Window gtk_window;
   Twiin::Gui gui (gtk_window, size_2d, config_file, data,
      stage, product, level, dtime, journey_tokens, zoom_str);
   gtk_window.add (gui);
   gtk_window.show_all_children ();
   gtk_window.show ();
   gtk_window.set_resizable (true);
   gtk_window.resize (size_2d.i, size_2d.j);
   Gtk::Main::run (gtk_window);

}
#endif /* ENABLE_GTKMM */

void
Twiin::plan (const Dstring& stage_str,
             const Dstring& product_str,
             const Dstring& level_str,
             const Dstring& time_str,
             const Dstring& zoom_str,
             const Dstring& track_id_str,
             const bool track_id_initial,
             const Track::Map& track_map,
             const Tokens& annotation_tokens,
             const Dstring& format,
             const Tokens& title_tokens,
             const Dstring& filename,
             const Dstring& color_bar_str,
             const Dstring& scale_bar_str,
             const list<Polygon>& polygon_list,
             const bool no_stage,
             const bool no_wind_barb,
             const bool is_bludge) const
{

   Gshhs* gshhs_ptr = NULL;

   for (auto iterator = config_file.begin ();
        iterator != config_file.end (); iterator++)
   {

      const Tokens tokens (*(iterator), " \f\t\n");
      if (tokens.size () != 2) { continue; }
      if (tokens[0] != "overlay") { continue; }

      const Tokens overlay_tokens (tokens[1], ":");
      if (overlay_tokens[0] != "GSHHS") { continue; }

      const Dstring gshhs_file_path (overlay_tokens[8]);
      gshhs_ptr = new Gshhs (gshhs_file_path);
      break;

   }

   const Dtime::Set time_set (time_str);
   const Tokens level_tokens (level_str, ":");
   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");
   const Tokens track_id_tokens (track_id_str, ":");

   Transform_Ptr_Map transform_ptr_map (size_2d, config_file);

   Title title (size_2d);
   const Data data (config_file);
   const Model& model = data.get_model ();
   const Hrit& hrit = data.get_hrit ();
   const Station::Map& station_map = data.get_station_map ();

   const Point_2D centre (size_2d.i/2, size_2d.j/2);
   Geodetic_Transform* gt_ptr = (zoom_str == "" ? NULL :
      Geodetic_Transform::get_transform_ptr (zoom_str, centre)); 

   for (Tokens::const_iterator i = stage_tokens.begin ();
        i != stage_tokens.end (); i++)
   {

      const Dstring s (*i);
      const Model::Stage& stage = model.get_stage (s);
      const Dtime& basetime = stage.get_basetime ();
      const Geodetic_Transform& transform = gt_ptr == NULL ?
         *(transform_ptr_map[s]) : *gt_ptr;

      const Geodetic_Mesh& mesh = transform.get_mesh (size_2d);

      for (Tokens::const_iterator j = product_tokens.begin ();
           j != product_tokens.end (); j++)
      {

         const Model::Product product (*j);
         const bool is_speed = (product.enumeration == Model::Product::SPEED);

         for (auto k = level_tokens.begin ();
              k != level_tokens.end (); k++)
         {

            const Level level (*k);
            const bool is_higher = (level.type == Level::HEIGHT) &&
               (level.value > 1500);
            const Model::Product& p = ((is_speed && is_higher) ?
               Model::Product (Model::Product::SPEED_HIGHER) : product);

            const bool is_hrit = (p.enumeration == Model::Product::IR1) ||
                                 (p.enumeration == Model::Product::IR2) ||
                                 (p.enumeration == Model::Product::IR3) ||
                                 (p.enumeration == Model::Product::IR4) ||
                                 (p.enumeration == Model::Product::VIS);

cout << "is_hrit = " << (is_hrit ? "yes" : "no") << endl;

            const vector<Dtime>& valid_time_vector = is_hrit ?
               hrit.get_valid_time_vector (time_set) :
               stage.get_valid_time_vector (product, level, time_set);
cout << valid_time_vector.size () << endl;

            #pragma omp parallel for
            for (Integer l = 0; l < valid_time_vector.size (); l++)
            {
           
               const Dtime& dtime = valid_time_vector.at (l);

               const Dstring& file_path = (filename == "") ?
                  get_file_path (format, s, product, level, dtime) :
                  output_dir + "/" + filename;
               cout << file_path;
               if (is_bludge) { cout << " will be made" << endl; continue; }

               try
               {

                  RefPtr<Surface> surface = denise::get_surface (
                     size_2d, format, file_path);
                  RefPtr<Context> cr = denise::get_cr (surface);

                  Twiin::render (cr, transform, size_2d, data,
                     dtime, level, s, p, polygon_list, no_stage,
                     no_wind_barb);

                  if (gshhs_ptr != NULL)
                  {
                     cr->save ();

                     cr->set_line_width (2);
                     Color::black (0.6).cairo (cr);
                     gshhs_ptr->cairo (cr, transform);
                     cr->stroke ();

                     cr->save ();
                     mesh.cairo (cr, transform);

                     cr->restore ();

                  }

                  for (auto t = track_id_tokens.begin ();
                       t != track_id_tokens.end (); t++)
                  {

                     const Dstring& token = *(t);
                     const char c = token[0];
                     const bool bw_track = (c == '@' || c == '^');
                     const Dstring& track_id = token.substr (bw_track ? 1 : 0);
                     const Track& track = track_map.at (track_id);

                     const Real start_tau = track.get_start_tau ();
                     const Real end_tau = track.get_end_tau ();
                     const Real n = Integer (round ((end_tau - start_tau) / 0.025));
                     const Tuple tau_tuple (n, start_tau, end_tau);

                     cr->save ();
                     cr->set_font_size (14);

                     for (auto iterator = tau_tuple.begin ();
                          iterator != tau_tuple.end (); iterator++)
                     {
                        const Real tau = *(iterator);
                        const Lat_Long& lat_long = track.get_lat_long (tau);
                        const Real z = track.get_datum ("z", tau);
                        const Real zz = std::min (z, 5000.0);
                        const Real hue = (zz / 5000) * 0.833;
                        const Point_2D& point = transform.transform (lat_long);
                        const Color& color = !bw_track ? Color::hsb (hue, 0.8, 0.6) :
                           (c == '^' ? Color::black () : Color::white ());
                        Color::black (0.5).cairo (cr);
                        Ring (0.4).cairo (cr, point);
                        color.cairo (cr);
                        Ring (0.4).cairo (cr, point);
                        cr->fill ();
                     }

                     const Dstring& track_id_icon = track_id_initial ?
                        Dstring (track_id.substr (0, 1)) : track_id;

                     const Lat_Long& lat_long = track.get_lat_long (dtime);
                     const Real z = track.get_datum ("z", dtime);
                     const Real zz = std::min (z, 5000.0);
                     const Real hue = (zz / 5000) * 0.833;
                     const Point_2D& point = transform.transform (lat_long);
                     const Color& color = !bw_track ? Color::hsb (hue, 0.8, 0.6) :
                        (c == '^' ? Color::black () : Color::white ());
                     Color::black (0.5).cairo (cr);
                     Label (track_id_icon, point + Point_2D (2, 2), 'c', 'c').cairo (cr);
                     color.cairo (cr);
                     Label (track_id_icon, point, 'c', 'c').cairo (cr);

                  }

                  cr->restore ();

                  if (title_tokens.size () == 0)
                  {
                     Twiin::set_title (title, basetime,
                        s, product, dtime, level);
                  }
                  else
                  {
                     title.set (title_tokens);
                  }
                  title.cairo (cr);

                  Twiin::render_annotations (cr, transform,
                     annotation_tokens, data);
                  Twiin::render_scale_bar (cr, transform, size_2d, scale_bar_str);

                  Twiin::render_color_bar (cr, size_2d, p, color_bar_str);

                  if (format == "png") { surface->write_to_png (file_path); }

                  cout << " okay" << endl;

               }
               catch (...) { cout << " failed" << endl; }

            }

         }

      }

   }

   if (gt_ptr != NULL) { delete gt_ptr; }

}

void
Twiin::plan (const Dstring& stage_str,
             const Dstring& product_str,
             const Dstring& time_str,
             const Dstring& zoom_str,
             const Dstring& track_id_str,
             const bool track_id_initial,
             const Track::Map& track_map,
             const Tokens& annotation_tokens,
             const Dstring& format,
             const Tokens& title_tokens,
             const Dstring& filename,
             const Dstring& color_bar_str,
             const Dstring& scale_bar_str,
             const list<Polygon>& polygon_list,
             const bool no_stage,
             const bool no_wind_barb,
             const bool is_bludge) const
{

   Gshhs* gshhs_ptr = NULL;

   for (auto iterator = config_file.begin ();
        iterator != config_file.end (); iterator++)
   {

      const Tokens tokens (*(iterator), " \f\t\n");
      if (tokens.size () != 2) { continue; }
      if (tokens[0] != "overlay") { continue; }

      const Tokens overlay_tokens (tokens[1], ":");
      if (overlay_tokens[0] != "GSHHS") { continue; }

      const Dstring gshhs_file_path (overlay_tokens[8]);
      gshhs_ptr = new Gshhs (gshhs_file_path);
      break;

   }

   const Dtime::Set time_set (time_str);
   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");
   const Tokens track_id_tokens (track_id_str, ":");

   Transform_Ptr_Map transform_ptr_map (size_2d, config_file);

   Title title (size_2d);
   const Data data (config_file);
   const Model& model = data.get_model ();
   const Hrit& hrit = data.get_hrit ();
   const Station::Map& station_map = data.get_station_map ();

   const Point_2D centre (size_2d.i/2, size_2d.j/2);
   Geodetic_Transform* gt_ptr = (zoom_str == "" ? NULL :
      Geodetic_Transform::get_transform_ptr (zoom_str, centre)); 

   for (Tokens::const_iterator i = stage_tokens.begin ();
        i != stage_tokens.end (); i++)
   {

      const Dstring s (*i);
      const Model::Stage& stage = model.get_stage (s);
      const Dtime& basetime = stage.get_basetime ();
      const Geodetic_Transform& transform = gt_ptr == NULL ?
         *(transform_ptr_map[s]) : *gt_ptr;

      const Geodetic_Mesh& mesh = transform.get_mesh (size_2d);

      for (Tokens::const_iterator j = product_tokens.begin ();
           j != product_tokens.end (); j++)
      {

         const Model::Product product (*j);
         const bool is_speed = (product.enumeration == Model::Product::SPEED);

         for (auto t = track_id_tokens.begin ();
              t != track_id_tokens.end (); t++)
         {

            const Dstring& track_id = *(t);
            const Track& track = track_map.at (track_id);
            const Dtime& start_time = track.get_start_time ();
            const Dtime& end_time = track.get_end_time ();
            const Real start_tau = track.get_start_tau ();
            const Real end_tau = track.get_end_tau ();

            const Level surface ("Surface");

            const bool is_hrit = (product.enumeration == Model::Product::IR1) ||
                                 (product.enumeration == Model::Product::IR2) ||
                                 (product.enumeration == Model::Product::IR3) ||
                                 (product.enumeration == Model::Product::IR4) ||
                                 (product.enumeration == Model::Product::VIS);

            const vector<Dtime>& valid_time_vector = is_hrit ?
               hrit.get_valid_time_vector (time_set) :
               stage.get_valid_time_vector (product, surface, time_set);

            for (auto l = valid_time_vector.begin ();
                 l != valid_time_vector.end (); l++)
            {
        
               const Dtime& dtime = *(l);
               if (dtime < start_time || dtime > end_time) { continue; }

               const Lat_Long& lat_long = track.get_lat_long (dtime);
               if (stage.out_of_bounds (lat_long)) { continue; }

               const Real z = track.get_datum ("z", dtime);
               const Real topography = stage.get_topography (lat_long);

               const bool is_surface = (z <= topography);
               if (!is_surface)
               {
                  const set<Dtime>& uvts =
                     stage.get_valid_time_set (product, Level ("100m"));
                  if (uvts.find (dtime) == uvts.end ()) { continue; }
               }

               const Level& level = (is_surface ?
                  surface : Level (Level::HEIGHT, z));
               const bool is_higher =
                  (level.type == Level::HEIGHT) && (level.value > 1500);
               const Model::Product& p = ((is_speed && is_higher) ?
                  Model::Product (Model::Product::SPEED_HIGHER) : product);

               const Dstring& file_path = (filename == "") ?
                  get_file_path (format, s, product, level, dtime, track_id) :
                  output_dir + "/" + filename;
               cout << file_path;
               if (is_bludge) { cout << " will be made" << endl; continue; }

               try
               {

                  RefPtr<Surface> surface = denise::get_surface (
                     size_2d, format, file_path);
                  RefPtr<Context> cr = denise::get_cr (surface);

                  Twiin::render (cr, transform, size_2d, data, dtime,
                     level, s, p, polygon_list, no_stage, no_wind_barb);

                  if (gshhs_ptr != NULL)
                  {
                     cr->save ();

                     cr->set_line_width (2);
                     Color::black (0.6).cairo (cr);
                     gshhs_ptr->cairo (cr, transform);
                     cr->stroke ();

                     cr->save ();
                     mesh.cairo (cr, transform);

                     cr->restore ();

                  }

                  if (title_tokens.size () == 0)
                  {
                     Twiin::set_title (title, basetime,
                        s, product, dtime, level);
                  }
                  else
                  {
                     title.set (title_tokens);
                  }
                  title.cairo (cr);

                  const Real n = Integer (round ((end_tau - start_tau) / 0.025));
                  const Tuple tau_tuple (n, start_tau, end_tau);

                  cr->save ();
                  Color::black ().cairo (cr);
                  cr->set_font_size (14);

                  Simple_Polyline simple_polyline;

                  for (auto iterator = tau_tuple.begin ();
                       iterator != tau_tuple.end (); iterator++)
                  {
                     const Real tau = *(iterator);
                     const Lat_Long& lat_long = track.get_lat_long (tau);
                     simple_polyline.add (lat_long);
                     const Point_2D& point = transform.transform (lat_long);
                     Ring (0.2).cairo (cr, point);
                     cr->fill ();
                  }

                  cr->set_line_width (0.5);
                  simple_polyline.cairo (cr, transform);
                  cr->stroke ();

                  const Dstring& track_id_icon = track_id_initial ?
                     Dstring (track_id.substr (0, 1)) : track_id;

                  const Point_2D& point = transform.transform (lat_long);
                  Label (track_id_icon, point, 'c', 'c').cairo (cr);

                  cr->restore ();

                  Twiin::render_annotations (cr, transform,
                     annotation_tokens, data);
                  Twiin::render_scale_bar (cr, transform, size_2d, scale_bar_str);
                  Twiin::render_color_bar (cr, size_2d, p, color_bar_str);

                  if (format == "png") { surface->write_to_png (file_path); }

                  cout << " okay" << endl;

               }
               catch (...) { cout << " failed" << endl; }

            }

         }

      }

   }

   if (gt_ptr != NULL) { delete gt_ptr; }

}

void
Twiin::cross_section (const Dstring& stage_str,
                      const Dstring& product_str,
                      const Tokens& journey_tokens,
                      const Dstring& time_str,
                      const Real height,
                      const Dstring& format,
                      const Tokens& title_tokens,
                      const Dstring& filename,
                      const Real u_bg,
                      const bool is_bludge) const
{

   const Geodesy geodesy;
   const Level level ("100m");
   const Dtime::Set time_set (time_str);
   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");

   Title title (size_2d);
   const Data data (config_file);
   const Model& model = data.get_model ();

   const Real margin_l = 50;
   const Real margin_r = 20 + 90;
   const Real margin_t = title.get_height () + 10;
   const Real margin_b = 20;
   const Real w = size_2d.i - margin_l - margin_r;
   const Real h = size_2d.j - margin_t - margin_b;

   for (auto jny = journey_tokens.begin ();
        jny != journey_tokens.end (); jny++)
   {

      const Dstring& journey_str = *(jny);
      const Journey journey (journey_str);

      const Real distance = journey.get_distance (geodesy);
      const Domain_1D domain_x (0, distance);
      const Domain_1D domain_z (0, height);

      Affine_Transform_2D transform;
      const Real span_x = domain_x.get_span ();
      const Real span_z = domain_z.get_span ();
      transform.scale (1, -1);
      transform.scale (w / span_x, h / span_z);
      transform.translate (margin_l, size_2d.j - margin_b);

      for (Tokens::const_iterator i = stage_tokens.begin ();
           i != stage_tokens.end (); i++)
      {

         const Dstring s (*i);
         const Model::Stage& stage = model.get_stage (s);
         const Dtime& basetime = stage.get_basetime ();

         for (Tokens::const_iterator j = product_tokens.begin ();
              j != product_tokens.end (); j++)
         {

            const Model::Product product (*j);

            const Model::Product::Enumeration enumeration = product.enumeration;
            const bool is_speed = (enumeration == Model::Product::SPEED);
            const Model::Product& p = (is_speed ?
               Model::Product (Model::Product::SPEED_HIGHER) : product);

            const vector<Dtime>& valid_time_vector =
               stage.get_valid_time_vector (product, level, time_set);

            #pragma omp parallel for
            for (Integer l = 0; l < valid_time_vector.size (); l++)
            {

               const Dtime& dtime = valid_time_vector.at (l);

               const Dstring& file_path = (filename == "") ?
                  get_file_path (format, s, product, dtime, journey) :
                  output_dir + "/" + filename;
               cout << file_path;
               if (is_bludge) { cout << " will be made" << endl; continue; }

               try
               {

                  RefPtr<Surface> surface = denise::get_surface (
                     size_2d, format, file_path);
                  RefPtr<Context> cr = denise::get_cr (surface);

                  const Index_2D i2d (margin_l, margin_t);
                  const Size_2D s2d (size_2d.i - margin_l - margin_r,
                                     size_2d.j - margin_t - margin_b);
                  const Box_2D box_2d (i2d, s2d);

                  if (s2d.i < 0 || s2d.j < 0) { continue; }

                  Twiin::render_cross_section (cr, transform, box_2d,
                     domain_z, stage, product, dtime, journey, u_bg);

                  Twiin::render_color_bar (cr, size_2d, p);

                  if (title_tokens.size () == 0)
                  {
                     Twiin::set_title (title, basetime,
                        s, product, dtime, journey);
                  }
                  else
                  {
                     title.set (title_tokens);
                  }
                  title.cairo (cr);

                  if (format == "png") { surface->write_to_png (file_path); }

                  cout << " okay" << endl;

               }
               catch (...) { cout << " failed" << endl; }

            }
         }
      }
   }

}

void
Twiin::cross_section (const Dstring& stage_str,
                      const Dstring& product_str,
                      const Dstring& track_id_str,
                      const Track::Map& track_map,
                      const Real distance,
                      const Dstring& time_str,
                      const Real height,
                      const Dstring& format,
                      const Tokens& title_tokens,
                      const Dstring& filename,
                      const bool lagrangian,
                      const bool is_bludge) const
{

   const Level level ("100m");
   const Dtime::Set time_set (time_str);
   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");
   const Tokens track_id_tokens (track_id_str, ":");

   Title title (size_2d);
   const Data data (config_file);
   const Model& model = data.get_model ();

   const Real margin_l = 50;
   const Real margin_r = 20 + 90;
   const Real margin_t = title.get_height () + 10;
   const Real margin_b = 20;
   const Real w = size_2d.i - margin_l - margin_r;
   const Real h = size_2d.j - margin_t - margin_b;

   for (Tokens::const_iterator i = stage_tokens.begin ();
        i != stage_tokens.end (); i++)
   {

      const Dstring s (*i);
      const Model::Stage& stage = model.get_stage (s);
      const Dtime& basetime = stage.get_basetime ();

      for (Tokens::const_iterator j = product_tokens.begin ();
           j != product_tokens.end (); j++)
      {

         const Model::Product product (*j);

         const bool is_speed = (product.enumeration == Model::Product::SPEED);
         const Model::Product& p = (is_speed ?
            Model::Product (Model::Product::SPEED_HIGHER) : product);

         for (Integer index = 0; index < track_id_tokens.size (); index++)
         {

            const Dstring& track_id = track_id_tokens[index];
            const Track& track = track_map.at (track_id);

            const Real start_t = track.get_start_time ().t;
            const Real end_t = track.get_end_time ().t;

            const vector<Dtime>& valid_time_vector =
               stage.get_valid_time_vector (product, level, time_set);

            #pragma omp parallel for
            for (Integer l = 0; l < valid_time_vector.size (); l++)
            {

               const Dtime& dtime = valid_time_vector.at (l);
               if (dtime.t < start_t || dtime.t > end_t) { continue; }

               typedef Geodesy G;
               const Lat_Long& ll = track.get_lat_long (dtime);
               const Real z = track.get_datum ("z", dtime);
               const Motion& motion = track.get_motion (dtime);
               const Real direction = motion.get_direction ();
               const Real speed = motion.get_speed ();

               Journey journey;
               journey.push_back (G::get_destination (ll, -distance, direction));
               journey.push_back (G::get_destination (ll, distance, direction));
               const Domain_1D domain_x (-distance, distance);
               const Domain_1D domain_z (0, height);

               Affine_Transform_2D transform;
               const Real span_x = domain_x.get_span ();
               const Real span_z = domain_z.get_span ();
               transform.scale (1, -1);
               transform.scale (w / span_x, h / span_z);
               transform.translate (margin_l, size_2d.j - margin_b);

               const Real u_bg = (lagrangian ? speed : 0);

               const Dstring& file_path = (filename == "") ? get_file_path (
                  format, s, product, dtime, ll, distance, lagrangian) :
                  output_dir + "/" + filename;
               cout << file_path;
               if (is_bludge) { cout << " will be made" << endl; continue; }

               try
               {

                  RefPtr<Surface> surface = denise::get_surface (
                     size_2d, format, file_path);
                  RefPtr<Context> cr = denise::get_cr (surface);

                  const Index_2D i2d (margin_l, margin_t);
                  const Size_2D s2d (size_2d.i - margin_l - margin_r,
                                     size_2d.j - margin_t - margin_b);
                  const Box_2D box_2d (i2d, s2d);

                  if (s2d.i < 0 || s2d.j < 0) { continue; }

                  Twiin::render_cross_section (cr, transform, box_2d,
                     domain_z, stage, product, dtime, journey, u_bg);

                  cr->save ();
                  cr->set_line_width (4);
                  Color::black (0.2).cairo (cr);
                  Ring (10).cairo (cr, transform.transform (distance, z));
                  cr->stroke ();
                  cr->restore ();

                  Twiin::render_color_bar (cr, size_2d, p);

                  if (title_tokens.size () == 0)
                  {
                     Twiin::set_title (title, basetime, s, product,
                        dtime, ll, distance, lagrangian);
                  }
                  else
                  {
                     title.set (title_tokens);
                  }
                  title.cairo (cr);

                  if (format == "png") { surface->write_to_png (file_path); }

                  cout << " okay" << endl;

               }
               catch (...) { cout << " failed" << endl; }

            }
         }
      }
   }

}

void
Twiin::time_cross (const Dstring& stage_str,
                   const Dstring& product_str,
                   const Real azimuth,
                   const Dstring& location_str,
                   const Dstring& time_str,
                   const Real height,
                   const Dstring& format,
                   const Tokens& title_tokens,
                   const Dstring& filename,
                   const bool is_bludge) const
{

   const Level level ("100m");
   const Dtime::Set time_set (time_str);
   const Dtime::Span time_span = time_set.get_span ();
   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");
   const Tokens location_tokens (location_str, ":");

   const Domain_1D domain_z (0, height);

   Title title (size_2d);
   const Data data (config_file);
   const Model& model = data.get_model ();
   const Station::Map& station_map = data.get_station_map ();

   const Real margin_l = 50;
   const Real margin_r = 20 + 90;
   const Real margin_t = title.get_height () + 10;
   const Real margin_b = 40;
   const Real w = size_2d.i - margin_l - margin_r;
   const Real h = size_2d.j - margin_t - margin_b;

   for (Tokens::const_iterator i = stage_tokens.begin ();
        i != stage_tokens.end (); i++)
   {

      const Dstring s (*i);
      const Model::Stage& stage = model.get_stage (s);
      const Dtime& basetime = stage.get_basetime ();

      for (Tokens::const_iterator j = product_tokens.begin ();
           j != product_tokens.end (); j++)
      {

         const Model::Product product (*j);

         const bool is_speed = (product.enumeration == Model::Product::SPEED);
         const Model::Product& p = (is_speed ?
            Model::Product (Model::Product::SPEED_HIGHER) : product);

         for (Tokens::const_iterator k = location_tokens.begin ();
              k != location_tokens.end (); k++)
         {

            const Location location (*k, station_map);
            if (stage.out_of_bounds (location)) { continue; }

            const Real start_t = time_span.get_start ().t;
            const Real end_t = time_span.get_end ().t;
            const Domain_1D domain_t (start_t, end_t);

            Affine_Transform_2D transform;
            const Real span_t = domain_t.get_span ();
            const Real span_z = domain_z.get_span ();
            transform.scale (1, -1);
            transform.translate (-start_t, 0);
            transform.scale (w / span_t, h / span_z);
            transform.translate (margin_l, size_2d.j - margin_b);

            const Dstring& file_path = (filename == "") ?
               get_file_path (format, s, product, location) :
               output_dir + "/" + filename;
            cout << file_path;
            if (is_bludge) { cout << " will be made" << endl; continue; }

            try
            {

               RefPtr<Surface> surface = denise::get_surface (
                  size_2d, format, file_path);
               RefPtr<Context> cr = denise::get_cr (surface);

               const Index_2D i2d (margin_l, margin_t);
               const Size_2D s2d (size_2d.i - margin_l - margin_r,
                                  size_2d.j - margin_t - margin_b);
               const Box_2D box_2d (i2d, s2d);

               if (s2d.i < 0 || s2d.j < 0) { continue; }

               Twiin::render_time_cross (cr, transform, box_2d,
                  domain_z, stage, product, location, time_span, azimuth);

               Twiin::render_color_bar (cr, size_2d, p);

               if (title_tokens.size () == 0)
               {
                  Twiin::set_title (title, basetime, s, product, location);
               }
               else
               {
                  title.set (title_tokens);
               }
               title.cairo (cr);

               if (format == "png") { surface->write_to_png (file_path); }

               cout << " okay" << endl;

            }
            catch (...) { cout << " failed" << endl; }

         }

      }
   }

}

void
Twiin::time_cross (const Dstring& stage_str,
                   const Dstring& product_str,
                   const Dstring& track_id_str,
                   const Track::Map& track_map,
                   const Real height,
                   const Dstring& format,
                   const Tokens& title_tokens,
                   const Dstring& filename,
                   const bool lagrangian,
                   const bool is_bludge) const
{

   const Level level ("100m");
   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");
   const Tokens track_id_tokens (track_id_str, ":");

   const Domain_1D domain_z (0, height);

   Title title (size_2d);
   const Data data (config_file);
   const Model& model = data.get_model ();

   const Real margin_l = 50;
   const Real margin_r = 20 + 90;
   const Real margin_t = title.get_height () + 10;
   const Real margin_b = 40;
   const Real w = size_2d.i - margin_l - margin_r;
   const Real h = size_2d.j - margin_t - margin_b;

   for (Tokens::const_iterator i = stage_tokens.begin ();
        i != stage_tokens.end (); i++)
   {

      const Dstring s (*i);
      const Model::Stage& stage = model.get_stage (s);
      const Dtime& basetime = stage.get_basetime ();

      for (Tokens::const_iterator j = product_tokens.begin ();
           j != product_tokens.end (); j++)
      {

         const Model::Product product (*j);

         const bool is_speed = (product.enumeration == Model::Product::SPEED);
         const Model::Product& p = (is_speed ?
            Model::Product (Model::Product::SPEED_HIGHER) : product);

         #pragma omp parallel for
         for (Integer index = 0; index < track_id_tokens.size (); index++)
         {

            const Dstring& track_id = track_id_tokens[index];
            const Track& track = track_map.at (track_id);

            const Real start_t = track.get_start_time ().t;
            const Real end_t = track.get_end_time ().t;
            const Domain_1D domain_t (start_t, end_t);

            Affine_Transform_2D transform;
            const Real span_t = domain_t.get_span ();
            const Real span_z = domain_z.get_span ();
            transform.scale (1, -1);
            transform.translate (-start_t, 0);
            transform.scale (w / span_t, h / span_z);
            transform.translate (margin_l, size_2d.j - margin_b);

            const Dstring& file_path = (filename == "") ?
               get_file_path (format, s, product, track_id, lagrangian) :
               output_dir + "/" + filename;
            cout << file_path;
            if (is_bludge) { cout << " will be made" << endl; continue; }

            try
            {

               RefPtr<Surface> surface = denise::get_surface (
                  size_2d, format, file_path);
               RefPtr<Context> cr = denise::get_cr (surface);

               const Index_2D i2d (margin_l, margin_t);
               const Size_2D s2d (size_2d.i - margin_l - margin_r,
                                  size_2d.j - margin_t - margin_b);
               const Box_2D box_2d (i2d, s2d);

               if (s2d.i < 0 || s2d.j < 0) { continue; }

               Twiin::render_time_cross (cr, transform, box_2d,
                  domain_z, stage, product, track, lagrangian);

               Twiin::render_color_bar (cr, size_2d, p);

               if (title_tokens.size () == 0)
               {
                  Twiin::set_title (title, basetime, s,
                     product, track_id, lagrangian);
               }
               else
               {
                  title.set (title_tokens);
               }
               title.cairo (cr);

               if (format == "png") { surface->write_to_png (file_path); }

               cout << " okay" << endl;

            }
            catch (...) { cout << " failed" << endl; }

         }

      }
   }

}

void
Twiin::meteogram (const Dstring& stage_str,
                  const Dstring& location_str,
                  const Dstring& time_str,
                  const Dstring& format,
                  const Tokens& title_tokens,
                  const Dstring& filename,
                  const bool ignore_pressure,
                  const Dstring& meteogram_mode,
                  const bool is_bludge) const
{

   const Tokens stage_tokens (stage_str, ":");
   const Tokens location_tokens (location_str, ":");

   Title title (size_2d);
   const Data data (config_file);
   const Model& model = data.get_model ();
   const Station::Map& station_map = data.get_station_map ();
   const Aws::Repository& aws_repository = data.get_aws_repository ();

   for (Tokens::const_iterator i = stage_tokens.begin ();
        i != stage_tokens.end (); i++)
   {

      const Dstring s (*i);
      const Model::Stage& stage = model.get_stage (s);
      const Dtime& basetime = stage.get_basetime ();

      for (Tokens::const_iterator j = location_tokens.begin ();
           j != location_tokens.end (); j++)
      {

         const Location location (*j, station_map);
         if (stage.out_of_bounds (location)) { continue; }

         const Dstring& file_path = (filename == "") ?
            get_file_path (format, s, location) :
            output_dir + "/" + filename;
         cout << file_path;
         if (is_bludge) { cout << " will be made" << endl; continue; }

         try
         {

            RefPtr<Surface> surface = denise::get_surface (
               size_2d, format, file_path);
            RefPtr<Context> cr = denise::get_cr (surface);

            Twiin::render_meteogram (cr, size_2d, stage, aws_repository,
               location, time_str, ignore_pressure, meteogram_mode);

            if (title_tokens.size () == 0)
            {
               Twiin::set_title (title, basetime, s, location, meteogram_mode);
            }
            else
            {
               title.set (title_tokens);
            }
            title.cairo (cr);

            if (format == "png") { surface->write_to_png (file_path); }

            cout << " okay" << endl;

         }
         catch (...) { cout << " failed" << endl; }

      }

   }

}

void
Twiin::vertical_profile (const Dstring& stage_str,
                         const Dstring& location_str,
                         const Dstring& time_str,
                         const Dstring& format,
                         const Tokens& title_tokens,
                         const Dstring& filename,
                         const bool is_bludge) const
{

   const Model::Product product ("T");
   const Level level ("100m");
   const Dtime::Set time_set (time_str);
   const Tokens stage_tokens (stage_str, ":");
   const Tokens location_tokens (location_str, ":");

   Title title (size_2d);
   const Data data (config_file);
   const Model& model = data.get_model ();
   const Station::Map& station_map = data.get_station_map ();

   const Tephigram tephigram (size_2d);

   for (Tokens::const_iterator i = stage_tokens.begin ();
        i != stage_tokens.end (); i++)
   {

      const Dstring s (*i);
      const Model::Stage& stage = model.get_stage (s);
      const Dtime& basetime = stage.get_basetime ();

      const vector<Dtime>& valid_time_vector =
         stage.get_valid_time_vector (product, level, time_set);

      #pragma omp parallel for
      for (Integer l = 0; l < valid_time_vector.size (); l++)
      {

         const Dtime& dtime = valid_time_vector.at (l);

         for (Tokens::const_iterator j = location_tokens.begin ();
              j != location_tokens.end (); j++)
         {

            Dstring location_name ("");
            Lat_Long::List lat_long_list;

            const Dstring& location_token = *j;
            const Location sole_location (location_token, station_map);
            const bool is_j = Reg_Exp ("@").match (location_token);

            if (is_j)
            {
               const Geodesy geodesy;
               const Tokens tokens (location_token, "/");
               lat_long_list.add (Journey (tokens[0]), geodesy);
               location_name = tokens.size () > 1 ? tokens[1] : "mean";
            }
            else
            {
               lat_long_list.push_back (Location (location_token, station_map));
            }

            const Dstring& file_path =
               (filename == "") ?
                  ((location_name == "") ?
                     get_file_path (format, s, dtime, sole_location) :
                     get_file_path (format, s, dtime, location_name)) :
                  output_dir + "/" + filename;
            cout << file_path;
            if (is_bludge) { cout << " will be made" << endl; continue; }

            try
            {

               if (format == "snd")
               {

                  Sounding* sounding_ptr = stage.get_sounding_ptr (
                     lat_long_list, dtime, GSL_POSINF, tephigram);
                  sounding_ptr->save (file_path);
                  delete sounding_ptr;
               }
               else
               {

                  RefPtr<Surface> surface = denise::get_surface (
                     size_2d, format, file_path);
                  RefPtr<Context> cr = denise::get_cr (surface);

                  if (location_name == "")
                  {
                     Twiin::render_vertical_profile (cr, tephigram,
                        stage, dtime, sole_location);
                  }
                  else
                  {
                     Twiin::render_vertical_profile (cr, tephigram,
                        stage, dtime, lat_long_list);
                  }

                  if (title_tokens.size () == 0)
                  {
                     if (location_name == "")
                     {
                        Twiin::set_title (title, basetime,
                           s, dtime, sole_location);
                     }
                     else
                     {
                        Twiin::set_title (title, basetime,
                           s, dtime, location_name);
                     }
                  }
                  else
                  {
                     title.set (title_tokens);
                  }
                  title.cairo (cr);

                  if (format == "png") { surface->write_to_png (file_path); }

               }

               cout << " okay" << endl;

            }
            catch (...) { cout << " failed" << endl; }

         }

      }

   }

}

void
Twiin::vertical_profile (const Dstring& stage_str,
                         const Dstring& track_id_str,
                         const Track::Map& track_map,
                         const Dstring& time_str,
                         const Dstring& format,
                         const Tokens& title_tokens,
                         const Dstring& filename,
                         const bool is_bludge) const
{

   const Model::Product product ("T");
   const Level level ("100m");
   const Dtime::Set time_set (time_str);
   const Tokens stage_tokens (stage_str, ":");
   const Tokens track_id_tokens (track_id_str, ":");

   Title title (size_2d);
   const Data data (config_file);
   const Model& model = data.get_model ();
   const Station::Map& station_map = data.get_station_map ();

   const Tephigram tephigram (size_2d);

   for (Tokens::const_iterator i = stage_tokens.begin ();
        i != stage_tokens.end (); i++)
   {

      const Dstring s (*i);
      const Model::Stage& stage = model.get_stage (s);
      const Dtime& basetime = stage.get_basetime ();

      for (auto t = track_id_tokens.begin ();
           t != track_id_tokens.end (); t++)
      {

         const Dstring& track_id = *(t);
         const Track& track = track_map.at (track_id);
         const Dtime& start_time = track.get_start_time ();
         const Dtime& end_time = track.get_end_time ();
         const Real start_tau = track.get_start_tau ();
         const Real end_tau = track.get_end_tau ();

         const vector<Dtime>& valid_time_vector =
            stage.get_valid_time_vector (product, level, time_set);

         for (Integer l = 0; l < valid_time_vector.size (); l++)
         {

            const Dtime& dtime = valid_time_vector.at (l);
            if (dtime < start_time || dtime > end_time) { continue; }

            const Lat_Long& lat_long = track.get_lat_long (dtime);
            if (stage.out_of_bounds (lat_long)) { continue; }

            const Real z = track.get_datum ("z", dtime);
            const Level level (Level::HEIGHT, z);

            const Dstring& file_path =
               (filename == "") ?
                  get_file_path (format, s, dtime, track_id) :
                  output_dir + "/" + filename;
            cout << file_path;
            if (is_bludge) { cout << " will be made" << endl; continue; }

            try
            {

               if (format == "snd")
               {

                  Sounding* sounding_ptr =
                     stage.get_sounding_ptr (lat_long, dtime);
                  sounding_ptr->save (file_path);
                  delete sounding_ptr;
               }
               else
               {

                  RefPtr<Surface> surface = denise::get_surface (
                     size_2d, format, file_path);
                  RefPtr<Context> cr = denise::get_cr (surface);

                  Twiin::render_vertical_profile (cr, tephigram,
                     stage, dtime, lat_long, level);

                  if (title_tokens.size () == 0)
                  {
                     Twiin::set_title (title, basetime, s, dtime, track_id +
                       " " + lat_long.get_string (4, true, true, true));
                  }
                  else
                  {
                     title.set (title_tokens);
                  }
                  title.cairo (cr);

                  if (format == "png") { surface->write_to_png (file_path); }

               }

               cout << " okay" << endl;

            }
            catch (...) { cout << " failed" << endl; }

         }

      }

   }

}

void
Twiin::Andrea::twiin_trajectory_generate (const Dstring& identifier,
                                          const Dstring& stage_str,
                                          const Lat_Long& lat_long,
                                          const Level& level,
                                          const Dtime& dtime,
                                          const Real finish_tau,
                                          const Tokens& arguments)
{

   vector<Model::Product> product_vector;
   for (Integer i = 0; i < arguments.size (); i++)
   {
      const Tokens t (arguments[i], "=");
      if (t.size () == 2 && t[0].get_lower_case () == "product")
      {
         const Model::Product product (t[1]);
         product_vector.push_back (product);
      }
   }

   const Data data (config_file);
   const Model& model = data.get_model ();
   const Track& trajectory = model.get_stage (stage_str).get_trajectory (
      lat_long, level, dtime, finish_tau, product_vector);

   trajectory_map.insert (make_pair (identifier, trajectory));

}

void
Twiin::Andrea::twiin_trajectory_survey (const Dstring& identifier,
                                        const Dstring& stage_str,
                                        const Tokens& arguments)
{

   vector<Model::Product> product_vector;
   for (Integer i = 0; i < arguments.size (); i++)
   {
      const Tokens t (arguments[i], "=");
      if (t.size () == 2 && t[0].get_lower_case () == "product")
      {
         const Model::Product product (t[1]);
         product_vector.push_back (product);
      }
   }

   const Data data (config_file);
   const Model& model = data.get_model ();
   Track& trajectory = trajectory_map.at (identifier);
   model.get_stage (stage_str).survey_trajectory (trajectory, product_vector);

}

void
Twiin::Andrea::twiin_trajectory_ingest (const Dstring& file_path)
{
   igzstream file (file_path);
   trajectory_map.ingest (file);
   file.close ();
}

void
Twiin::Andrea::twiin_trajectory_write (const Dstring& file_path) const
{
   ofstream file (file_path, ios::app);
   trajectory_map.write (file);
   file.close ();
}

void
Twiin::Andrea::twiin_trajectory_print (const Dstring& identifier) const
{

   const Track& trajectory = trajectory_map.at (identifier);
   const set<Dtime>& dtime_set = trajectory.get_dtime_set ();

   for (auto iterator = dtime_set.begin ();
        iterator != dtime_set.end (); iterator++)
   {
      const Dtime& dtime = *(iterator);
      const Dstring& time_str = dtime.get_string ("%Y%m%d%H%M%S");
      const Lat_Long& ll = trajectory.get_lat_long (dtime);
      const Motion& motion = trajectory.get_motion (dtime);
      const Real direction = motion.get_direction ();
      const Real speed = motion.get_speed ();
      const Real z = trajectory.get_datum ("z", dtime);

      cout << time_str << " " << ll << " " << direction << " "
           << speed << " " << z << " ";

      for (auto iterator_e = trajectory.begin ();
           iterator_e != trajectory.end (); iterator_e++)
      {
         const Dstring& e = iterator_e->first;
         if (e == "latitude" || e == "longitude" || e == "z") { continue; }
         const Real datum = trajectory.get_datum (e, dtime);
         cout << e << ":" << datum << " ";
      }

      cout << endl;

   }

}

void
Twiin::Andrea::twiin_trajectory_surface (const Dstring& surface_identifier,
                                         const Dstring& geodetic_transform_identifier,
                                         const Dstring& identifier,
                                         const Dtime& dtime) const
{

   const RefPtr<Surface>& surface = get_surface (surface_identifier);
   const RefPtr<Context> cr = get_cr (surface_identifier);
   const Size_2D& size_2d = get_size_2d (surface_identifier);
   const Point_2D centre (Real (size_2d.i) / 2, Real (size_2d.j) / 2);

   const Geodetic_Transform* geodetic_transform_ptr =
      get_geodetic_transform_ptr (geodetic_transform_identifier, centre);
   const Geodetic_Transform& geodetic_transform = *geodetic_transform_ptr;

   const Track& trajectory = trajectory_map.at (identifier);
   const Real start_tau = trajectory.get_start_tau ();
   const Real end_tau = trajectory.get_end_tau ();
   const Real n = Integer (round ((end_tau - start_tau) / 0.025));
   const Tuple tau_tuple (n, start_tau, end_tau);

   Color::black ().cairo (cr);
   cr->set_font_size (14);

   for (auto iterator = tau_tuple.begin ();
        iterator != tau_tuple.end (); iterator++)
   {
      const Real tau = *(iterator);
      const Lat_Long& lat_long = trajectory.get_lat_long (tau);
      const Point_2D& point = geodetic_transform.transform (lat_long);
      Ring (0.2).cairo (cr, point);
      cr->fill ();
   }

   const Lat_Long& lat_long = trajectory.get_lat_long (dtime);
   const Point_2D& point = geodetic_transform.transform (lat_long);
   Label (identifier, point, 'c', 'c').cairo (cr);

}

void
Twiin::Andrea::twiin_trajectory (const Tokens& tokens)
{

   const Dstring& action = tokens[0].get_lower_case ();
   const Integer n = tokens.size ();

   if (action == "clear")
   {
      trajectory_map.clear ();
   }
   else
   if (action == "generate")
   {
      const Dstring& identifier = tokens[1];
      const Dstring& stage_str = tokens[2];
      const Lat_Long lat_long (tokens[3]);
      const Level level (tokens[4]);
      const Dtime dtime (tokens[5]);
      const Real finish_tau = stof (tokens[6]);
      twiin_trajectory_generate (identifier, stage_str, lat_long,
         level, dtime, finish_tau, tokens.subtokens (7));
   }
   else
   if (action == "survey")
   {
      const Dstring& identifier = tokens[1];
      const Dstring& stage_str = tokens[2];
      twiin_trajectory_survey (identifier, stage_str, tokens.subtokens (3));
   }
   else
   if (action == "ingest")
   {
      const Dstring& file_path = tokens[1];
      twiin_trajectory_ingest (file_path);
   }
   else
   if (action == "write")
   {
      const Dstring& file_path = tokens[1];
      twiin_trajectory_write (file_path);
   }
   else
   if (action == "print")
   {
      const Dstring& identifier = tokens[1];
      twiin_trajectory_print (identifier);
   }
   else
   if (action == "surface")
   {
      const Dstring& surface_identifier = tokens[1];
      const Dstring& geodetic_transform_identifier = tokens[2];
      const Dstring& identifier = tokens[3];
      const Dtime& dtime (tokens[4]);
      twiin_trajectory_surface (surface_identifier,
         geodetic_transform_identifier, identifier, dtime);
   }
   else
   {
      throw Exception ("twiin_trajectory");
   }

}

void
Twiin::Andrea::twiin_surface_plan (const Dstring& surface_identifier,
                                   const Dstring& geodetic_transform_identifier,
                                   const Dstring& stage,
                                   const Model::Product& product,
                                   const Level& level,
                                   const Dtime& dtime,
                                   const Tokens& arguments)
{

   typedef Surface_Package Sp;
   typedef Geodetic_Transform_Package Gtp;

   const RefPtr<Surface>& surface = Sp::get_surface (surface_identifier);
   const RefPtr<Context> cr = Sp::get_cr (surface_identifier);
   const Size_2D& size_2d = Sp::get_size_2d (surface_identifier);
   const Point_2D centre (Real (size_2d.i) / 2, Real (size_2d.j) / 2);

   const Geodetic_Transform* geodetic_transform_ptr =
      Gtp::get_geodetic_transform_ptr (geodetic_transform_identifier, centre);
   const Geodetic_Transform& geodetic_transform = *geodetic_transform_ptr;

   Dstring color_bar_str = "r:10";
   Dstring scale_bar_str = "lb:10";
   bool no_stage = true;
   bool no_wind_barb = true;

   for (auto iterator = arguments.begin ();
        iterator != arguments.end (); iterator++)
   {
      const Tokens tokens (iterator->get_lower_case (), "=");
      const Dstring& option = tokens[0];
      const Dstring& value = tokens[1];
      const bool is_yes = (value == "yes" || value == "y" || value == "true");
      if (option == "color_bar") { color_bar_str = value; }
      if (option == "scale_bar") { scale_bar_str = value; }
      if (option == "stage")     { no_stage = !is_yes; }
      if (option == "wind_barb") { no_wind_barb = !is_yes; }
   }

   Color::white ().cairo (cr);
   cr->paint ();

   const Data data (config_file);
   const Model& model = data.get_model ();
   const Station::Map& station_map = data.get_station_map ();
   Twiin::render (cr, geodetic_transform, size_2d, data, dtime,
      level, stage, product, polygon_list, no_stage, no_wind_barb);

   Twiin::render_scale_bar (cr, geodetic_transform, size_2d, scale_bar_str);
   Twiin::render_color_bar (cr, size_2d, product, color_bar_str);

   delete geodetic_transform_ptr;

}

void
Twiin::Andrea::twiin_surface (const Tokens& tokens)
{

   const Dstring& surface_identifier = tokens[0];
   const Dstring& geodetic_transform_identifier = tokens[1];
   const Dstring stage (tokens[2]);
   const Model::Product product (tokens[3]);
   const Level level (tokens[4]);
   const Dtime dtime (tokens[5]);

   twiin_surface_plan (surface_identifier, geodetic_transform_identifier,
      stage, product, level, dtime, tokens.subtokens (6));

}

void
Twiin::Andrea::twiin_time_series (const Tokens& tokens)
{

   const Dstring& data = tokens[0];
   if (data == "aws")
   {
      twiin_time_series_aws (tokens.subtokens (1));
   }
   else
   {
      throw Exception ("twiin_time_series");
   }

}

void
Twiin::Andrea::twiin_time_series_aws (const Tokens& arguments)
{

   Ituple station_ituple;
   Tokens product_tokens;
   Dtime::Span time_span;

   const Data data (config_file);
   const Aws::Repository& aws_repository = data.get_aws_repository ();

   for (auto iterator = arguments.begin ();
        iterator != arguments.end (); iterator++)
   {

      const Tokens tkns (iterator->get_lower_case (), "=");
      const Dstring& option = tkns[0];
      const Dstring& value = tkns[1];
      const bool is_yes = (value == "yes" || value == "y" || value == "true");

      if (option == "start") { time_span.start_t = Dtime (value).t; }
      if (option == "end") { time_span.end_t = Dtime (value).t; }

      if (option == "station") { station_ituple.push_back (stoi (value)); }
      if (option == "product") { product_tokens.push_back (value); }

   }

   for (auto s = station_ituple.begin (); s != station_ituple.end (); s++)
   {

      const Integer station_id = *(s);
      const Aws::Repository* ar_ptr =
         aws_repository.get_aws_repository_ptr (station_id, time_span);
      const Aws::Repository& ar = *ar_ptr;

      for (auto o = ar.begin (); o != ar.end (); o++)
      {

         const Aws::Key& key = o->first;
         const Aws::Obs& obs = o->second;
         const Tuple& products = obs.get_products (product_tokens);

         cout << key.station_id << " ";
         cout << key.dtime.get_string ("%Y%m%d%H%M") << " ";
         cout << products << endl;

      }

      delete ar_ptr;

   }

}

void
Twiin::Andrea::twiin_parse (const Tokens& tokens)
{

   const Dstring& action = tokens[0].get_lower_case ();
   const Integer n = tokens.size ();

   if (action == "trajectory")
   {
      twiin_trajectory (tokens.subtokens (1));
   }
   else
   if (action == "surface")
   {
      twiin_surface (tokens.subtokens (1));
   }
   else
   if (action == "time_series")
   {
      twiin_time_series (tokens.subtokens (1));
   }
   else
   {
      throw Exception ("twiin_parse");
   }

}

void
Twiin::Andrea::parse (const Tokens& tokens)
{

   const Dstring& action = tokens[0].get_lower_case ();

   if (action == "twiin")
   {
      twiin_parse (tokens.subtokens (1));
      return;
   }

   andrea::Andrea::parse (tokens);

}

Twiin::Andrea::Andrea (const Config_File& config_file)
   : andrea::Andrea (""),
     config_file (config_file)
{
}

void
Twiin::usage ()
{

   cout << endl;
   cout << "twiin, twiin is ink initially" << endl;
   cout << "Usage: twiin [ long options ] [ option ]" << endl;
   cout << endl;
   cout << "Options:" << endl;
   cout << "  -?  --help                         display this help and exit" << endl;
   cout << "  -A  --azimuth=NUMBER               specify a direction in Lagrangian cross sections" << endl;
   cout << "  -a  --annotation=STR               annotation as dots or journeys" << endl;
   cout << "  -b  --bludge                       do noting" << endl;
   cout << "  -C  --color-bar=r:10               location / size of color bar" << endl;
   cout << "  -c  --config=CONFIG_FILE           specify config file location" << endl;
   cout << "  -F  --filename=FILENAME            filename of output file" << endl;
   cout << "  -f  --format=eps|pdf|png           eps or pdf or png" << endl;
   cout << "  -g  --geometry=HEIGHTxWIDTH        geometry of image produced" << endl;
   cout << "  -G  --gui                          GUI Mode" << endl;
   cout << "  -h  --height=STR                   cross section height" << endl;
   cout << "  -I  --plot-first-char-of-track-id  display this help and exit" << endl;
   cout << "  -i  --interactive                  interactive mode" << endl;
   cout << "  -J  --journey=JOURNEY_STR          <location>@<location>@<location>" << endl;
   cout << "  -j  --track=IDENTIFIER             track/trajectory identifier" << endl;
   cout << "  -j  --trajectory=IDENTIFIER        track/trajectory identifier" << endl;
   cout << "  -K  --scale-bar=lb:10              location / size of scale bar" << endl;
   cout << "  -l  --level=LEVEL_STR              horizontal level" << endl;
   cout << "  -M  --track-map=FILENAME           track/trajectory database" << endl;
   cout << "  -M  --trajectory-map=FILENAME      track/trajectory database" << endl;
   cout << "  -m  --meteogram=MODE_STR           renders meteogram" << endl;
   cout << "  -O  --location=STR                 lat,long or AWS identifier" << endl;
   cout << "  -o  --output-dir=STR               output directory" << endl;
   cout << "  -P  --ignore-pressure              don't render pressure in meteogram" << endl;
   cout << "  -p  --product=PRODUCT_STR          model product" << endl;
   cout << "  -S  --no-stage                     don't draw stage boxes" << endl;
   cout << "  -s  --stage=STAGE_STR              specify stage identifier" << endl;
   cout << "  -T  --title=STR                    colon separated title tokens" << endl;
   cout << "  -t  --time=YYYYMMDDHHMM            colon sepearated times of time spans" << endl;
   cout << "  -u  --u_bg=NUMBER                  specify horizontal speed in Lagrangian cross sections" << endl;
   cout << "  -v  --vertical-profile             render tephigram" << endl;
   cout << "  -W  --no-wind-barb                 don't draw wind barbs" << endl;
   cout << "  -X  --time-cross                   render vertical time cross section" << endl;
   cout << "  -x  --cross-section                render vertical cross section" << endl;
   cout << "  -z  --zoom=ZOOM_STR                specify geographic transform" << endl;
   cout << endl;
   cout << "Examples:" << endl;
   cout << endl;
   cout << "  Plan Visualization:" << endl;
   cout << "    twiin -s STAGE4 -p Q -l Surface:1800m -t 201310170000-201310170600 -z LAMBERT_CONIC_SOUTH:850:-34.5918,149.1288" << endl;
   cout << "    twiin -s STAGE4 -p Q -l Surface -a " << endl;
   cout << endl;
   cout << "  Vertical Cross Section:" << endl;
   cout << endl;
   cout << "    twiin -g 1024x300 -L -h 3000m -s STAGE4 -p Q:THETA -t 201310170000 -x -J -35.2378,144.8571@-37.9481,148.9278" << endl;
   cout << "    twiin -g 1024x300 -L -h 4000m -d 180e3 -s STAGE4 -p THETA:W -t 201310170000 -x -j X:Y:Z -M whatever_file.trajector.gz" << endl;
   cout << endl;
   cout << endl;
   cout << "  Vertical Profile:" << endl;
   cout << endl;
   cout << "    twiin -g 800x900 -s STAGE4 -t 201310170200 -v -O 63292 " << endl;
   cout << "    twiin -g 800x900 -s STAGE4 -t 201310170200 -v -O -35.2378,144.8571 " << endl;
   cout << endl;

}

void
Twiin::set_title (Title& title,
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
Twiin::set_title (Title& title,
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
Twiin::set_title (Title& title,
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
Twiin::set_title (Title& title,
                  const Dtime& basetime,
                  const Dstring& stage_str,
                  const Model::Product& product,
                  const Location& location)
{
   const Dstring& location_str = location.get_str ();
   const Dstring& basetime_str = basetime.get_string ();
   const Dstring stage_product = stage_str + " / " + product.get_description ();
   title.set (location_str, "", stage_product, basetime_str, "");
}

void
Twiin::set_title (Title& title,
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
Twiin::set_title (Title& title,
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
Twiin::set_title (Title& title,
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
Twiin::set_title (Title& title,
                  const Dtime& basetime,
                  const Dstring& stage_str,
                  const Location& location,
                  const Dstring& meteogram_mode)
{
   const Dstring& basetime_str = meteogram_mode == "no_nwp" ?
      "" : "Model Basetime: " + basetime.get_string ();
   const Lat_Long& lat_long (location);
   const Dstring& ll_str = lat_long.get_string (4, true, true, true);
   //title.set ("", stage_str, location.get_long_str (), basetime_str, ll_str);
   title.set (ll_str, stage_str, location.get_long_str (), basetime_str, "");
}

void
Twiin::render_polygon_list (const RefPtr<Context>& cr,
                            const Geodetic_Transform& transform,
                            const list<Polygon>& polygon_list)
{

   cr->save ();
   cr->set_line_width (4);
   Color::hsb (0.00, 0.00, 0.00, 0.5).cairo (cr);

   for (auto iterator = polygon_list.begin ();
        iterator != polygon_list.end (); iterator++)
   {
      const Polygon& polygon = *(iterator);
      polygon.cairo (cr, transform);
      cr->stroke ();
   }

   cr->restore ();

}

void
Twiin::render_stages (const RefPtr<Context>& cr,
                      const Geodetic_Transform& transform,
                      const Data& data)
{

   cr->save ();

   const Model& model = data.get_model ();

   cr->set_line_width (4);
   Color::hsb (0.00, 0.00, 0.00, 0.5).cairo (cr);
   const Tokens stage_tokens ("N4000 N1200 N0440");

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
Twiin::render_product (const RefPtr<Context>& cr,
                       const Geodetic_Transform& transform,
                       const Size_2D& size_2d,
                       const Data& data,
                       const Model::Product& product,
                       const Dtime& dtime,
                       const Level& level,
                       const Dstring& stage_str)
{

   Raster* raster_ptr = NULL;
   const Model& model = data.get_model ();
   const Model::Stage& stage = model.get_stage (stage_str);

   if (product.enumeration == Model::Product::IR1 ||
       product.enumeration == Model::Product::IR2 ||
       product.enumeration == Model::Product::IR3 ||
       product.enumeration == Model::Product::IR4 ||
       product.enumeration == Model::Product::VIS ||
       product.enumeration == Model::Product::Pseudo)
   {
      const Hrit& hrit = data.get_hrit ();
      const Dstring& channel_str = product.get_string ();
      raster_ptr = hrit.get_raster_ptr (size_2d, transform, channel_str, dtime);
   }
   else
   {
      raster_ptr = stage.get_geo_raster_ptr (size_2d, transform, product, dtime, level);
   }

   if (raster_ptr != NULL) { raster_ptr->blit (cr); }
   delete raster_ptr;

}

void
Twiin::render_wind_barbs (const RefPtr<Context>& cr,
                          const Geodetic_Transform& transform,
                          const Size_2D& size_2d,
                          const Data& data,
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

   const Model& model = data.get_model ();
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
Twiin::render_scale_bar (const RefPtr<Context>& cr,
                         const Geodetic_Transform& geodetic_transform,
                         const Size_2D& size_2d,
                         const Dstring& scale_bar_str)
{

   if (scale_bar_str == "no") { return; }

   const Tokens tokens (scale_bar_str, ":");
   const char h_align = tokens[0][0];
   const char v_align = tokens[0][1];
   const Real margin = (tokens.size () > 1 ? stof (tokens[1]) : 10);

   const Real title_height = 40;
   const Real bar_height = 6;
   const Real font_size = 10;
   const Real scale = geodetic_transform.data.scale;

   const Real log10_scale = log10 (scale * 1.6);
   Real exponent = floor (log10_scale);
   Real residue = log10_scale - floor (log10_scale);
   if (residue > 0.849) { exponent++; residue = 0; }
   const Real mantissa = (residue > 0.548 ? 5.0 : (residue > 0.199 ? 2.5 : 1));
   const Real bar_length = mantissa * exp10 (exponent + 2);

   const Real pixels = bar_length / scale;
   const Real bar_width = 2 * pixels;
   const Real box_height = bar_height + font_size*2 + margin*2;
   const Real box_width = bar_width + 2 * margin;

   const Real lx = margin;
   const Real rx = size_2d.i - margin - box_width;
   const Real cx = (size_2d.i - box_width) / 2;

   const Real ty = title_height + margin;
   const Real by = size_2d.j - margin - box_height;
   const Real cy = (size_2d.j - box_height) / 2;

   const Real x = h_align == 'l' ? lx : (h_align == 'r' ? rx : cx);
   const Real y = v_align == 't' ? ty : (v_align == 'b' ? by : cy);

   const Point_2D point (x, y);

   cr->save ();
   cr->set_line_width (1);
   cr->set_font_size (font_size);

   // Bounding Box
   Color::white (0.7).cairo (cr);
   Rect (point, box_width, box_height).cairo (cr);
   cr->fill ();

   cr->translate (point.x + box_width / 2, point.y + box_height / 2);

   // Scale Bar itself
   Color::black ().cairo (cr);
   cr->translate (-bar_width / 2, -bar_height / 2);
   Rect (Point_2D (0, 0), bar_width, bar_height).cairo (cr);
   cr->translate (bar_width / 2, bar_height / 2);
   cr->stroke ();

   // Chackered Segments
   const Real tick = pixels/5;
   for (Integer i = 0; i < 5; i += 2)
   {
      const Real x = -(i + 1) * tick;
      Rect (Point_2D (x, -bar_height/2), tick, bar_height).cairo (cr);
      cr->fill ();
   }

   // Tick Labels
   for (Integer i = 0; i < 6; i++)
   {
      const Real x = -i * tick;
      const Real tick_length = i * bar_length/5 * 1e-3;
      const Dstring& str = Dstring::render ("%.0f", tick_length);
      Label (str, Point_2D (x, 0), 'c', 't', bar_height*1.5).cairo (cr);
   }

   // Other Labels
   const Dstring& str = Dstring::render ("%.0f", bar_length * 1e-3);
   Label ("kilometres", Point_2D (0, 0), 'c', 'b', font_size).cairo (cr);
   Label (str, Point_2D (bar_length, 0), 'c', 't', font_size).cairo (cr);

   cr->restore ();

}

void
Twiin::render_color_bar (const RefPtr<Context>& cr,
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
                        product.enumeration == Model::Product::SCORER ||
                        product.enumeration == Model::Product::SCORER_A ||
                        product.enumeration == Model::Product::SCORER_B ||
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
Twiin::render_color_bar (const RefPtr<Context>& cr,
                         const Size_2D& size_2d,
                         const Model::Product& product,
                         const Dstring& color_bar_str)
{

   if (color_bar_str == "no") { return; }

   const Tokens tokens (color_bar_str, ":");
   const char h_align = tokens[0][0];
   const Real margin = (tokens.size () > 1 ? stof (tokens[1]) : 10);

   const Dstring& unit = product.get_unit ();
   const Tuple& tick_tuple = product.get_tick_tuple ();
   if (tick_tuple.size () < 2) { return; }

   const Real title_height = 40;
   const Real font_size = 12;

   const Real box_width = 80;
   const Real box_height = size_2d.j - title_height - 2*margin;

   const Real lx = margin;
   const Real rx = size_2d.i - margin - box_width;
   const Real box_x = h_align == 'l' ? lx : rx;
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
       product.enumeration == Model::Product::Q_NV_ADVECTION ||
       product.enumeration == Model::Product::THETA_TENDENCY ||
       product.enumeration == Model::Product::THETA_ADVECTION ||
       product.enumeration == Model::Product::THETA_H_ADVECTION ||
       product.enumeration == Model::Product::THETA_V_ADVECTION ||
       product.enumeration == Model::Product::THETA_S_ADVECTION ||
       product.enumeration == Model::Product::THETA_N_ADVECTION ||
       product.enumeration == Model::Product::THETA_SV_ADVECTION ||
       product.enumeration == Model::Product::THETA_NV_ADVECTION ||
       product.enumeration == Model::Product::SCORER ||
       product.enumeration == Model::Product::SCORER_A ||
       product.enumeration == Model::Product::SCORER_B)
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
Twiin::render_annotation_point (const RefPtr<Context>& cr,
                                const Geodetic_Transform& transform,
                                const Lat_Long& lat_long,
                                const Dstring& str,
                                const char h_align,
                                const char v_align)
{

   const Real ring_size = 4;
   const Ring ring (ring_size);
   const Real font_size = 14;
   cr->set_font_size (font_size);

   const Point_2D& point = transform.transform (lat_long);
   const Point_2D shadow_point (point.x + 2, point.y + 2);
   ring.cairo (cr, point);

   Color::green (0.8).cairo (cr);
   cr->fill_preserve ();
   Color::black ().cairo (cr);
   cr->stroke ();

   const Point_2D anchor (point.x, point.y);
   Color::gray (0.8).cairo (cr);
   Label (str, shadow_point, h_align, v_align, font_size/2).cairo (cr);
   Color::gray (0.2).cairo (cr);
   Label (str, point, h_align, v_align, font_size/2).cairo (cr);

}

void
Twiin::render_annotation (const RefPtr<Context>& cr,
                          const Geodetic_Transform& transform,
                          const Dstring& annotation_str,
                          const Data& data)
{

   const Station::Map& station_map = data.get_station_map ();
   const Tokens tokens (annotation_str, ":");
   const Dstring& genre = tokens[0];

   if (genre == "location" ||
       genre == "lat_long" ||
       genre == "l")
   {
      const Location location (tokens[1], station_map);
      const Dstring& str = (tokens.size () > 2 ? tokens[2] : Dstring (""));
      const char& h_align = (tokens.size () > 3 ? tokens[3][0] : 'r');
      const char& v_align = (tokens.size () > 4 ? tokens[4][0] : 'c');
      render_annotation_point (cr, transform, location, str, h_align, v_align);
   }
   else
   if (genre == "journey" ||
       genre == "route" ||
       genre == "j" ||
       genre == "r")
   {
      Journey journey;
      bool annotated = true;
      for (auto iterator = tokens.begin ();
           iterator != tokens.end (); iterator++)
      {
         if (iterator == tokens.begin ()) { continue; }
         const Dstring& token = *(iterator);
         if (token == "plain") { annotated = false; continue; }
         const Location location (token);
         journey.push_back (location);
      }
      journey.cairo (cr, transform, annotated);
   }

}

void
Twiin::render_annotations (const RefPtr<Context>& cr,
                           const Geodetic_Transform& transform,
                           const Tokens& annotation_tokens,
                           const Data& data)
{

   cr->save ();
   cr->set_line_width (1);

   for (auto iterator = annotation_tokens.begin ();
        iterator != annotation_tokens.end (); iterator++)
   {
      const Dstring& annotation_str = *(iterator);
      render_annotation (cr, transform, annotation_str, data);
   }

   cr->restore ();

}

void
Twiin::render (const RefPtr<Context>& cr,
               const Geodetic_Transform& transform,
               const Size_2D& size_2d,
               const Data& data,
               const Dtime& dtime,
               const Level& level,
               const Dstring& stage_str,
               const Model::Product product,
               const list<Polygon>& polygon_list,
               const bool no_stage,
               const bool no_wind_barb)
{

   cr->save ();

   //Color (0.86, 0.85, 0.47).cairo (cr);
   Checkered (Color::gray (0.55), Color::gray (0.45)).cairo (cr);
   cr->paint();

   render_product (cr, transform, size_2d, data,
      product, dtime, level, stage_str);

   render_polygon_list (cr, transform, polygon_list);

   if (!no_wind_barb)
   {
      render_wind_barbs (cr, transform, size_2d,
         data, dtime, level, stage_str);
   }

   // Stage 3/4/5 Frames
   if (!no_stage)
   {
      render_stages (cr, transform, data);
   }    

   // All Stations
   //const Station::Map& station_map = data.get_station_map ();
   //station_map.cairo (cr, transform);

   cr->restore ();

}

void
Twiin::render_cross_section_w (const RefPtr<Context>& cr,
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
Twiin::render_cross_section_mesh (const RefPtr<Context>& cr,
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
Twiin::render_cross_section_arrows (const RefPtr<Context>& cr,
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
Twiin::render_cross_section (const RefPtr<Context>& cr,
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

   //render_cross_section_streamlines (cr, transform, box_2d, stage,
   //   product, dtime, journey, u_bg);

   if (u_bg != 0)
   {
      Color::black ().cairo (cr);
      const Dstring fmt ("%.2f ms\u207b\u00b9");
      const Dstring& u_bg_string = Dstring::render (fmt, u_bg);
      const Index_2D anchor = box_2d.get_ne ();
      Label (u_bg_string, box_2d.get_ne (), 'r', 't', 10).cairo (cr);
   }

   cr->restore ();

}

void
Twiin::render_time_cross_w (const RefPtr<Context>& cr,
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
Twiin::render_time_cross_mesh (const RefPtr<Context>& cr,
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
Twiin::render_time_cross_track (const RefPtr<Context>& cr,
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
Twiin::render_time_cross (const RefPtr<Context>& cr,
                          const Transform_2D& transform,
                          const Box_2D& box_2d,
                          const Domain_1D& domain_z,
                          const Model::Stage& stage,
                          const Model::Product& product,
                          const Location& location,
                          const Dtime::Span& time_span,
                          const Real azimuth)
{

   cr->save ();

   Color::white ().cairo (cr);
   cr->paint ();

   Raster* raster_ptr = stage.get_time_cross_raster_ptr (
      box_2d, transform, product, location, time_span, azimuth);
   raster_ptr->blit (cr);
   delete raster_ptr;

   const Real start_t = time_span.get_start ().t;
   const Real end_t = time_span.get_end ().t;
   const Domain_1D domain_t (start_t, end_t);
   render_time_cross_mesh (cr, transform, domain_t, domain_z);

   cr->restore ();

}

void
Twiin::render_time_cross (const RefPtr<Context>& cr,
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
      //render_time_cross_w (cr, transform, box_2d, stage, track);
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
Twiin::render_meteogram_mesh (const RefPtr<Context>& cr,
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
   const Real delta_t = end_t - start_t;

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

   const Real major_t_interval = (delta_t > 23 ? 24 : 6);
   const Real middle_t_interval = (delta_t > 23 ? 6 : 1);
   const Real minor_t_interval = (delta_t > 23 ? 1 : 1.0/6.0);

   Mesh_2D mesh_temperature (Size_2D (2, 2),
      Domain_2D (domain_t, domain_temperature),
                 minor_t_interval, 1, minor_color,
                 middle_t_interval, GSL_NAN, middle_color,
                 major_t_interval, 10, major_color);
   Mesh_2D mesh_direction (Size_2D (2, 2),
      Domain_2D (domain_t, domain_direction),
                 minor_t_interval, 10, minor_color,
                 middle_t_interval, GSL_NAN, middle_color,
                 major_t_interval, 90, major_color);
   Mesh_2D mesh_speed (Size_2D (2, 2),
      Domain_2D (domain_t, domain_speed),
                 minor_t_interval, 1, minor_color,
                 middle_t_interval, GSL_NAN, middle_color,
                 major_t_interval, 5, major_color);
   Mesh_2D mesh_pressure (Size_2D (2, 2),
      Domain_2D (domain_t, domain_pressure),
                 minor_t_interval, 1, minor_color,
                 middle_t_interval, GSL_NAN, middle_color,
                 major_t_interval, 10, major_color);

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

   cr->save ();
   const Real temperature_centre = (domain_temperature.start + domain_temperature.end) / 2;
   const Point_2D temperature_point (domain_t.start, temperature_centre);
   Label temperature_label ("Temperature", temperature_point, 'c', 't', -70);
   temperature_label.set_text_angle (3 * M_PI / 2);
   cr->set_font_size (14);
   Color::black ().cairo (cr);
   temperature_label.cairo (cr, transform_temperature);
   cr->restore ();

   cr->save ();
   const Real direction_centre = (domain_direction.start + domain_direction.end) / 2;
   const Point_2D direction_point (domain_t.start, direction_centre);
   Label direction_label ("Wind Direction", direction_point, 'c', 't', -70);
   direction_label.set_text_angle (3 * M_PI / 2);
   cr->set_font_size (14);
   Color::black ().cairo (cr);
   direction_label.cairo (cr, transform_direction);
   cr->restore ();

   cr->save ();
   const Real speed_centre = (domain_speed.start + domain_speed.end) / 2;
   const Point_2D speed_point (domain_t.start, speed_centre);
   Label speed_label ("Wind Speed", speed_point, 'c', 't', -70);
   speed_label.set_text_angle (3 * M_PI / 2);
   cr->set_font_size (14);
   Color::black ().cairo (cr);
   speed_label.cairo (cr, transform_speed);
   cr->restore ();

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
         start_t, "%.0f hPa", Label::REAL, 'r', 'c', 5);
      mesh_pressure.render_label_x (cr, transform_pressure, 2,
         domain_pressure.end, "%b %d", Label::TIME, 'c', 't', 15);
      mesh_pressure.render_label_x (cr, transform_pressure, 1,
         domain_pressure.end, "%HZ", Label::TIME, 'c', 't', 5);
   }

   cr->restore ();

}

void
Twiin::render_meteogram (const RefPtr<Context>& cr,
                         const Transform_2D& t_temperature,
                         const Transform_2D& t_direction,
                         const Transform_2D& t_speed,
                         const Transform_2D& t_pressure,
                         const Aws::Repository& aws_repository,
                         const Real alpha,
                         const Real ring_size,
                         const bool fill,
                         const bool ignore_pressure)
{

   cr->save ();
   cr->set_line_width (0.2);

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
      if (fill) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      const Real dew_point = obs.dew_point;
      const Point_2D p_dew_point (dtime.t, dew_point);
      const Point_2D& tp_dew_point = t_temperature.transform (p_dew_point);
      ring.cairo (cr, tp_dew_point);
      Color::hsb (0.000, 0.80, 0.80, alpha).cairo (cr);
      if (fill) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      const Real direction = obs.wind_direction;
      const Point_2D p_direction (dtime.t, direction);
      const Point_2D& tp_direction = t_direction.transform (p_direction);
      ring.cairo (cr, tp_direction);
      Color::hsb (0.167, 0.80, 0.60, alpha).cairo (cr);
      if (fill) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      const Real speed = obs.wind_speed;
      const Real gust = obs.wind_gust;
      const Point_2D p_speed (dtime.t, speed);
      const Point_2D& tp_speed = t_speed.transform (p_speed);
      ring.cairo (cr, tp_speed);
      Color::hsb (0.333, 0.80, 0.80, alpha).cairo (cr);
      if (fill) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      const Point_2D p_gust (dtime.t, gust);
      const Point_2D& tp_gust = t_speed.transform (p_gust);
      ring.cairo (cr, tp_gust);
      Color::hsb (0.500, 0.80, 0.80, alpha).cairo (cr);
      if (fill) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

      if (ignore_pressure) { continue; }

      const Real mslp = obs.mslp;
      const Point_2D p_mslp (dtime.t, mslp);
      const Point_2D& tp_mslp = t_pressure.transform (p_mslp);
      ring.cairo (cr, tp_mslp);
      Color::hsb (0.833, 0.80, 0.80, alpha).cairo (cr);
      if (fill) { cr->fill (); }
      else { cr->fill_preserve (); Color::black ().cairo (cr); cr->stroke (); }

   }

   cr->restore ();

}

void
Twiin::render_meteogram (const RefPtr<Context>& cr,
                         const Size_2D& size_2d,
                         const Model::Stage& stage, 
                         const Aws::Repository& aws_repository,
                         const Location& location,
                         const Dstring& time_str,
                         const bool ignore_pressure,
                         const Dstring& meteogram_mode)
{

   cr->save ();
   Color::white ().cairo (cr);
   cr->paint ();

   const Dtime::Span ts (time_str);
   const Dtime& st = ts.get_start ();
   const Dtime& et = ts.get_end ();

   const Aws::Repository* model_aws_repository_ptr =
      stage.get_aws_repository_ptr (location, ts);
   const Aws::Repository& model_aws_repository = *model_aws_repository_ptr;

   const Dtime& model_start_time = stage.get_basetime ();
   const Dtime& model_end_time = model_aws_repository.rbegin ()->first.dtime;
   const Dtime start_time (std::max (model_start_time.t, st.t));
   const Dtime end_time (std::min (model_end_time.t, et.t));
   const Domain_1D domain_t (start_time.t, end_time.t);

   const Integer station_id = location.get_station_id ();

   const Dtime::Span time_span (start_time, end_time);
   const Aws::Repository* aws_repository_ptr = NULL;

   Real start_temperature = model_aws_repository.get_dew_point_domain ().start;
   Real end_temperature = model_aws_repository.get_temperature_domain ().end;
   Real end_speed = model_aws_repository.get_wind_speed_domain ().end;
   Real start_pressure = model_aws_repository.get_mslp_domain ().start;
   Real end_pressure = model_aws_repository.get_mslp_domain ().end;

   Real temperature_gap = 5.1;
   Real speed_gap = 5.1;
   Real pressure_gap = 260;

   if (station_id > 0)
   {

      aws_repository_ptr =
         aws_repository.get_aws_repository_ptr (station_id, time_span);

      start_temperature = std::min (start_temperature,
         aws_repository_ptr->get_dew_point_domain ().start) - temperature_gap;
      end_temperature = std::max (end_temperature,
         aws_repository_ptr->get_temperature_domain ().end) + temperature_gap;

      end_speed = std::max (end_speed,
         aws_repository_ptr->get_wind_speed_domain ().end) + speed_gap;

      start_pressure = std::min (start_pressure,
         aws_repository_ptr->get_mslp_domain ().start) - pressure_gap;
      end_pressure = std::max (end_pressure,
         aws_repository_ptr->get_mslp_domain ().end) + pressure_gap;

   }

   const Domain_1D domain_temperature (end_temperature, start_temperature);
   const Domain_1D domain_direction (360, 0);
   const Domain_1D domain_speed (end_speed, 0);
   const Domain_1D domain_pressure (end_pressure, start_pressure);

   const Real title_height = 40;
   const Real margin = 30;
   const Real margin_t = title_height + 10;
   const Real margin_b = 30;
   //const Real margin_l = 55;
   const Real margin_l = 85;
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
 
   // nwp data
   if (meteogram_mode != "no_nwp")
   {
      // draw nwp
      const Real alpha = 0.3;
      const Real ring_size = 4.0;
      const bool fill = false;
      render_meteogram (cr, transform_temperature, transform_direction,
         transform_speed, transform_pressure, *model_aws_repository_ptr,
         alpha, ring_size, fill, ignore_pressure);
   }

   // aws data
   if (meteogram_mode != "no_aws" || aws_repository_ptr != NULL)
   {
      // draw aws
      const Real alpha = 0.999;
      const Real ring_size = 0.8;
      const bool fill = true;
      render_meteogram (cr, transform_temperature, transform_direction,
         transform_speed, transform_pressure, *aws_repository_ptr, alpha,
         ring_size, fill, ignore_pressure);
   }

   delete aws_repository_ptr;
   delete model_aws_repository_ptr;

   cr->restore ();

}

void
Twiin::render_vertical_profile (const RefPtr<Context>& cr,
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
Twiin::render_vertical_profile (const RefPtr<Context>& cr,
                                const Thermo_Diagram& thermo_diagram,
                                const Model::Stage& stage,
                                const Dtime& dtime,
                                const Lat_Long::List& lat_long_list)
{

   cr->save ();

   Color::white ().cairo (cr);
   cr->paint ();

   const Sounding* sounding_ptr = stage.get_sounding_ptr (
      lat_long_list, dtime, GSL_POSINF, thermo_diagram);
   const Sounding& sounding = *sounding_ptr;

   cr->set_line_width (1);
   thermo_diagram.render (cr, 900);
   cr->set_line_width (2);
   sounding.render (cr, thermo_diagram);

   delete sounding_ptr;

   cr->restore ();

}

#ifndef ENABLE_GTKMM
#else /* ENABLE_GTKMM */
void
Twiin::Gui::Product_Panel::emit (const Model::Product& product)
{
   signal.emit (product);
}

Twiin::Gui::Product_Panel::Product_Panel (Gui& gui,
                                          const Real font_size)
   : Drawer_Panel (gui, true, font_size),
     gui (gui)
{
}

void
Twiin::Gui::Product_Panel::add_product (const Dstring& drawer_str,
                                        const Model::Product& product)
{
   Button* pb_ptr = new Button (gui, product, font_size);
   Button::T_Signal& signal = pb_ptr->get_t_signal ();
   signal.connect (sigc::mem_fun (*this, &Product_Panel::emit));
   add_widget_ptr (drawer_str, pb_ptr);
}

Twiin::Gui::Product_Panel::Signal&
Twiin::Gui::Product_Panel::get_signal ()
{
   return signal;
}

Tokens
Twiin::Gui::get_tokens (const Marker& marker) const
{

   const Lat_Long lat_long (marker);
   const Real latitude = marker.x;
   const Real longitude = marker.y;
   const Model& model = data.get_model ();
   const Model::Stage& stage = model.get_stage (stage_str);

   if (stage.out_of_bounds (lat_long))
   {
      return Map_Console::get_tokens (marker);
   }

   const Dtime& dtime = get_time_chooser ().get_time ();
   const Dstring& ll_str = lat_long.get_string (false, Dstring ("%.4f\u00b0"));
   Tokens tokens = stage.get_marker_tokens (lat_long, dtime, product, level);
   tokens.insert (tokens.begin (), ll_str);

   return tokens;

}

bool
Twiin::Gui::is_all_collapsed () const
{
   const bool p = product_panel.all_collapsed ();
   const bool all_collapsed = (p);
   return all_collapsed;
}

bool
Twiin::Gui::on_key_pressed (const Dkey_Event& event)
{

   if (Time_Canvas::on_key_pressed (event)) { return true; }
   if (Level_Canvas::on_key_pressed (event)) { return true; }

   switch (event.value)
   {
      case GDK_KEY_Escape:
      {
         const bool all_collapsed = is_all_collapsed ();
         unify_drawers (all_collapsed);
         return true;
         break;
      }
   }

   return Map_Console::on_key_pressed (event);

}

void
Twiin::Gui::unify_drawers (const bool expand)
{

   if (expand)
   {
      product_panel.expand_all ();
   }
   else
   {
      product_panel.collapse_all ();
   }

   queue_draw ();

}

void
Twiin::Gui::process_tephigram (const Integer marker_id)
{

   const auto& marker = get_marker_store ().get_marker (marker_id);
   const Lat_Long lat_long (marker);
   const Dtime& dtime = get_time_chooser ().get_time ();

   const Model& model = data.get_model ();
   const Model::Stage& stage = model.get_stage (stage_str);
   Sounding* sounding_ptr = stage.get_sounding_ptr (lat_long, dtime);
   const Sounding& sounding = *sounding_ptr;

   const Tuple tuple_p ("1000e2:925e2:850e2:700e2:500e2:300e2:200e2", ":");
   const Tephigram tephigram (Size_2D (100, 100));
 
   for (auto iterator = tuple_p.begin ();
        iterator != tuple_p.end (); iterator++)
   {

      const Real p = *(iterator);

      try
      {

      const Real t = sounding.get_temperature (tephigram, p);
      const Real t_d = sounding.get_dew_point (tephigram, p);
      const Real z = sounding.get_height (p);

      const Wind& wind = sounding.get_wind ( p);
      const Real wind_direction = wind.get_direction ();
      const Real wind_speed = wind.get_speed ();

      cout << p << " " << t << " " << t_d << " " << z <<
         " " << wind_direction << " " << wind_speed << endl;

      }
      catch (const Exception& e)
      {
         cerr << e << endl;
      }

   }

   delete sounding_ptr;

}

void
Twiin::Gui::process_aws (const Integer marker_id)
{

   const auto& marker = get_marker_store ().get_marker (marker_id);
   const Lat_Long lat_long (marker);

   cout << "aws for " << lat_long.get_string () << endl;

}

void
Twiin::Gui::process_cross_section (const Integer route_id)
{

   const auto& route = get_route_store ().get_route (route_id);

   const Size_2D size_2d (960, 480);
   const Dtime& dtime = get_time_chooser ().get_time ();
   const Model& model = data.get_model ();

   Cross_Section* cross_section_ptr = new Cross_Section (
      gtk_window, size_2d, model, route, stage_str, product, dtime);

   Gtk::Window* gtk_window_ptr = new Gtk::Window ();
   gtk_window_ptr->add (*cross_section_ptr);
   gtk_window_ptr->show_all_children ();
   gtk_window_ptr->show ();
   gtk_window_ptr->set_resizable (true);
   gtk_window_ptr->resize (size_2d.i, size_2d.j);

}

Twiin::Gui::Gui (Gtk::Window& gtk_window,
                 const Size_2D& size_2d,
                 const Config_File& config_file,
                 const Data& data,
                 const Dstring& stage_str,
                 const Model::Product& product,
                 const Level& level,
                 const Dtime& dtime,
                 const Tokens& journey_tokens,
                 const Dstring& zoom_str)
   : Map_Console (gtk_window, size_2d, config_file),
     Time_Canvas (*this, 12),
     Level_Canvas (*this, 12),
     product_panel (*this, 12),
     data (data),
     stage_str (stage_str),
     product (product),
     level (level)
{

   if (zoom_str != "")
   {
      const Geodetic_Transform::Data gtd (zoom_str);
      set_geodetic_transform_data (gtd);
   }

   time_chooser.get_signal ().connect (
      sigc::mem_fun (*this, &Gui::render_queue_draw));
   level_panel.get_level_signal ().connect (
      sigc::mem_fun (*this, &Gui::set_level));
   product_panel.get_signal ().connect (
      sigc::mem_fun (*this, &Gui::set_product));

   level_panel.add_extra_level (Level ("Surface"));
   level_panel.set_level (level);

   option_panel.set_hidable (true);
   product_panel.set_hidable (true);
   time_chooser.set_hidable (true);
   level_panel.set_hidable (true);

   register_widget (option_panel);
   register_widget (time_chooser);
   register_widget (level_panel);
   register_widget (product_panel);

   product_panel.add_product ("Thermo", Model::Product ("P_THETA"));
   product_panel.add_product ("Thermo", Model::Product ("T"));
   product_panel.add_product ("Thermo", Model::Product ("THETA"));
   product_panel.add_product ("Thermo", Model::Product ("THETA_V"));
   product_panel.add_product ("Thermo", Model::Product ("Q"));
   product_panel.add_product ("Thermo", Model::Product ("TD"));
   product_panel.add_product ("Thermo", Model::Product ("RH"));
   product_panel.add_product ("Thermo", Model::Product ("THETA_E"));
   product_panel.add_product ("Thermo", Model::Product ("RHO"));
   product_panel.add_product ("Dynamic", Model::Product ("P_RHO"));
   product_panel.add_product ("Dynamic", Model::Product ("WIND"));
   product_panel.add_product ("Dynamic", Model::Product ("SPEED"));
   product_panel.add_product ("Dynamic", Model::Product ("SPEED_HIGHER"));
   product_panel.add_product ("Dynamic", Model::Product ("W"));
   //product_panel.add_product ("Dynamic", Model::Product ("W_TRANSLUCENT"));
   product_panel.add_product ("Dynamic", Model::Product ("VORTICITY"));
   product_panel.add_product ("Derivatives", Model::Product ("Q_TENDENCY"));
   product_panel.add_product ("Derivatives", Model::Product ("Q_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("Q_H_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("Q_V_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("THETA_TENDENCY"));
   product_panel.add_product ("Derivatives", Model::Product ("THETA_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("THETA_H_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("THETA_V_ADVECTION"));
   product_panel.add_product ("Fire", Model::Product ("FFDI"));
   product_panel.add_product ("Misc", Model::Product ("PRECIP_RATE"));
   product_panel.add_product ("Misc", Model::Product ("MSLP"));
   product_panel.add_product ("Misc", Model::Product ("TERRAIN"));
   product_panel.add_product ("Misc", Model::Product ("IR1"));
   product_panel.add_product ("Misc", Model::Product ("IR2"));
   product_panel.add_product ("Misc", Model::Product ("IR3"));
   product_panel.add_product ("Misc", Model::Product ("IR4"));
   product_panel.add_product ("Misc", Model::Product ("VIS"));
   product_panel.add_product ("Misc", Model::Product ("Pseudo"));

   const Model& model = data.get_model ();
   const Model::Stage& stage = model.get_stage (stage_str);
   const set<Dtime>& ts = stage.get_valid_time_set (product, level);
   time_chooser.set_shape (Time_Chooser::Shape (ts));
   time_chooser.set_leap (1);
   time_chooser.get_data ().set_time (dtime);

   marker_popup_menu.append ("Tephigram");
   marker_popup_menu.get_id_signal ("Tephigram").connect (
      sigc::mem_fun (*this, &Gui::process_tephigram));

   marker_popup_menu.append ("AWS");
   marker_popup_menu.get_id_signal ("AWS").connect (
      sigc::mem_fun (*this, &Gui::process_aws));

   route_popup_menu.append ("Cross Section");
   route_popup_menu.get_id_signal ("Cross Section").connect (
      sigc::mem_fun (*this, &Gui::process_cross_section));

   Map_Console::Overlay_Store& overlay_store = get_overlay_store ();

   for (auto iterator = config_file.begin ();
        iterator != config_file.end (); iterator++)
   {

      const Tokens tokens (*(iterator));
      if (tokens.size () != 2 || tokens[0] != "overlay") { continue; }

      overlay_store.add (tokens[1], false);

   }

   option_panel.setup_overlay ();

   pack ();

   const Station::Map& station_map = data.get_station_map ();
   get_marker_store ().set_attractor (station_map);

   for (auto iterator = journey_tokens.begin ();
        iterator != journey_tokens.end (); iterator++)
   {
      const Dstring& journey_str = *(iterator);
      const Journey journey (journey_str);
      get_route_store ().insert (journey);
   }

}

Twiin::Gui::~Gui ()
{
}

void
Twiin::Gui::pack ()
{

   const Real title_height = title.get_height ();
   const Real margin = title_height * 0.15;
   const Real font_size = title_height * 0.3;
   const Real button_height = font_size * 2;

   const Real lp_width = width * 0.07;
   const Real lp_height = (height - title_height - 2 * margin);
   const Real lp_anchor_x = width - lp_width - margin;
   const Real lp_anchor_y = margin + title_height;
   const Point_2D lp_anchor (lp_anchor_x, lp_anchor_y);

   const Real tc_width = time_chooser.get_preferred_width ();
   const Real tc_height = (height - title_height - 2 * margin);
   const Point_2D tc_anchor (margin, margin + title_height);

   const Real cb_width = lp_width;
   const Real cb_height = button_height;
   const Real cb_anchor_x = lp_anchor_x;
   const Real cb_anchor_y = height - cb_height - margin;
   const Point_2D cb_anchor (cb_anchor_x, cb_anchor_y);

   const Real pp_width = width - tc_width - lp_width - 4 * margin;
   const Real pp_height = button_height;
   const Real pp_anchor_x = tc_width + 2 * margin;
   const Real pp_anchor_y = height - pp_height - margin;
   const Point_2D pp_anchor (pp_anchor_x, pp_anchor_y);

   const Real op_width = width - lp_width - tc_width - 4 * margin;
   const Real op_height = button_height;
   const Real op_anchor_x = tc_width + 2 * margin;
   const Real op_anchor_y = button_height + 2 * margin + title_height;
   const Point_2D op_anchor (op_anchor_x, op_anchor_y);

   option_panel.being_packed (op_anchor, op_width, op_height);
   option_panel.pack ();

   time_chooser.being_packed (tc_anchor, tc_width, tc_height);
   time_chooser.pack ();

   level_panel.being_packed (lp_anchor, lp_width, lp_height);
   level_panel.pack ();

   product_panel.being_packed (pp_anchor, pp_width, pp_height);
   product_panel.pack ();


}

void
Twiin::Gui::set_level (const Level& level)
{
   this->level = level;
   render_queue_draw ();
}

void
Twiin::Gui::set_product (const Model::Product& product)
{
   this->product = product;
   render_queue_draw ();
}

void
Twiin::Gui::render_queue_draw ()
{
   const Model& model = data.get_model ();
   const Model::Stage& stage = model.get_stage (stage_str);
   const Dtime& basetime = stage.get_basetime ();
   const Dtime& dtime = get_time_chooser ().get_time ();
   Twiin::set_title (title, basetime, stage_str, product, dtime, level);
   set_foreground_ready (false);
   Map_Console::render_queue_draw ();
}

void
Twiin::Gui::render_image_buffer (const RefPtr<Context>& cr)
{

   const Size_2D& size_2d = get_size_2d ();
   const Geodetic_Transform& transform =
      dynamic_cast<Geodetic_Transform&>(get_transform ());
   const Dtime& dtime = get_time_chooser ().get_time ();
   const Level& level = get_level_panel ().get_level ();

   Twiin::render (cr, transform, size_2d, data, dtime,
      level, stage_str, product, polygon_list, false, false);
   render_mesh (cr);
   render_overlays (cr);

}
#endif /* ENABLE_GTKMM */

