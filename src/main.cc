#include <getopt.h>
#include <denise/dstring.h>
#include "main.h"

#ifndef ENABLE_GTKMM
#else /* ENABLE_GTKMM */
#include "cross_section.h"
#include "console.h"
#endif /* ENABLE_GTKMM */

using namespace std;
using namespace Cairo;
using namespace denise;
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
   const Dstring& file_name = stage + "_" + location_str + "." + format;
   return output_dir + "/" + file_name;
}

Twiin::Twiin (const Size_2D& size_2d,
              const Config_File& config_file,
              const Dstring& output_dir)
   : andrea::Andrea (""),
     size_2d (size_2d),
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
            const Dstring& time_str) const
{

   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");
   const Tokens level_tokens (level_str, ":");

   const Dstring stage (stage_tokens[0]);
   const Model::Product product (product_tokens[0]);
   const Level level (level_tokens[0]);

   const Data data (config_file);

   Gtk::Window gtk_window;
   Console console (gtk_window, size_2d,
      config_file, data, stage, product, level);
   gtk_window.add (console);
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
             const Tokens& annotation_tokens,
             const Dstring& format,
             const Tokens& title_tokens,
             const Dstring& filename,
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

   Transform_Ptr_Map transform_ptr_map (size_2d, config_file);

   Title title (size_2d);
   const Data data (config_file);
   const Model& model = data.get_model ();
   const Dtime& basetime = model.get_basetime ();
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

            const vector<Dtime>& valid_time_vector =
               stage.get_valid_time_vector (product, level, time_set);

            #pragma omp parallel for
            for (Integer l = 0; l < valid_time_vector.size (); l++)
            {
           
               const Dtime& dtime = valid_time_vector.at (l);

               const Dstring& file_path = (filename == "") ?
                  get_file_path (format, s, product, level, dtime) :
                  output_dir + "/" + filename;
               cout << file_path << endl;
               if (is_bludge) { continue; }

               try
               {

                  RefPtr<Surface> surface = denise::get_surface (
                     size_2d, format, file_path);
                  RefPtr<Context> cr = denise::get_cr (surface);

                  Display::render (cr, transform, size_2d, model, hrit,
                     station_map, dtime, level, s, p, no_stage,
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

                  if (title_tokens.size () == 0)
                  {
                     Display::set_title (title, basetime,
                        s, product, dtime, level);
                  }
                  else
                  {
                     title.set (title_tokens);
                  }
                  title.cairo (cr);

                  Display::render_annotations (cr, transform, annotation_tokens);
                  Display::render_scale_bar (cr, transform, size_2d);
                  Display::render_color_bar (cr, size_2d, p);

                  if (format == "png") { surface->write_to_png (file_path); }

               }
               catch (...) { cout << "failed" << endl; }

            }

         }

      }

   }

   if (gt_ptr != NULL) { delete gt_ptr; }

}

void
Twiin::cross_section (const Dstring& stage_str,
                      const Dstring& product_str,
                      const Journey& journey,
                      const Dstring& time_str,
                      const Dstring& format,
                      const Tokens& title_tokens,
                      const Dstring& filename,
                      const Real u_bg,
                      const bool is_bludge) const
{

   const Level level ("100m");
   const Dtime::Set time_set (time_str);
   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");

   const Geodesy geodesy;
   const Real distance = journey.get_distance (geodesy);
   const Lat_Long origin (journey.front ());
   const Lat_Long destination (journey.back ());

   const Domain_1D domain_x (0, distance);
   const Domain_1D domain_z (0, 8000);

   Title title (size_2d);
   const Data data (config_file);
   const Model& model = data.get_model ();
   const Dtime& basetime = model.get_basetime ();

   const Real margin_l = 50;
   const Real margin_r = 20 + 80;
   const Real margin_t = title.get_height () + 10;
   const Real margin_b = 20;
   const Real w = size_2d.i - margin_l - margin_r;
   const Real h = size_2d.j - margin_t - margin_b;

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

      for (Tokens::const_iterator j = product_tokens.begin ();
           j != product_tokens.end (); j++)
      {

         const Model::Product product (*j);

         const bool is_speed = (product.enumeration == Model::Product::SPEED);
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
            cout << file_path << endl;
            if (is_bludge) { continue; }

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

               Display::render_cross_section (cr, transform, box_2d,
                  domain_z, stage, product, dtime, journey, u_bg);

               Display::render_color_bar (cr, size_2d, p);

               if (title_tokens.size () == 0)
               {
                  Display::set_title (title, basetime, s, product, dtime, journey);
               }
               else
               {
                  title.set (title_tokens);
               }
               title.cairo (cr);

               if (format == "png") { surface->write_to_png (file_path); }

            }
            catch (...) { cout << "failed" << endl; }

         }

      }
   }

};

void
Twiin::meteogram (const Dstring& stage_str,
                  const Dstring& location_str,
                  const Dstring& time_str,
                  const Dstring& format,
                  const Tokens& title_tokens,
                  const Dstring& filename,
                  const bool ignore_pressure,
                  const bool is_bludge) const
{

   const Dtime::Set time_set (time_str);
   const Tokens stage_tokens (stage_str, ":");
   const Tokens location_tokens (location_str, ":");

   Title title (size_2d);
   const Data data (config_file);
   const Model& model = data.get_model ();
   const Dtime& basetime = model.get_basetime ();
   const Station::Map& station_map = data.get_station_map ();
   const Aws::Repository& aws_repository = data.get_aws_repository ();

   for (Tokens::const_iterator i = stage_tokens.begin ();
        i != stage_tokens.end (); i++)
   {

      const Dstring s (*i);
      const Model::Stage& stage = model.get_stage (s);

      for (Tokens::const_iterator j = location_tokens.begin ();
           j != location_tokens.end (); j++)
      {

         const Location location (*j, station_map);

         const Dstring& file_path = (filename == "") ?
            get_file_path (format, s, location) :
            output_dir + "/" + filename;
         cout << file_path << endl;
         if (is_bludge) { continue; }

         try
         {

            RefPtr<Surface> surface = denise::get_surface (
               size_2d, format, file_path);
            RefPtr<Context> cr = denise::get_cr (surface);

            Display::render_meteogram (cr, size_2d, stage,
               aws_repository, location, ignore_pressure);

            if (title_tokens.size () == 0)
            {
               Display::set_title (title, basetime, s, location);
            }
            else
            {
               title.set (title_tokens);
            }
            title.cairo (cr);

            if (format == "png") { surface->write_to_png (file_path); }

         }
         catch (...) { cout << "failed" << endl; }

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
   const Dtime& basetime = model.get_basetime ();
   const Station::Map& station_map = data.get_station_map ();

   const Tephigram tephigram (size_2d);

   for (Tokens::const_iterator i = stage_tokens.begin ();
        i != stage_tokens.end (); i++)
   {

      const Dstring s (*i);
      const Model::Stage& stage = model.get_stage (s);

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
            cout << file_path << endl;
            if (is_bludge) { continue; }

            try
            {

               if (format == "snd")
               {

                  Sounding* sounding_ptr = stage.get_sounding_ptr (
                     lat_long_list, dtime, tephigram);
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
                     Display::render_vertical_profile (cr, tephigram,
                        stage, dtime, sole_location);
                  }
                  else
                  {
                     Display::render_vertical_profile (cr, tephigram,
                        stage, dtime, lat_long_list);
                  }

                  if (title_tokens.size () == 0)
                  {
                     if (location_name == "")
                     {
                        Display::set_title (title, basetime,
                           s, dtime, sole_location);
                     }
                     else
                     {
                        Display::set_title (title, basetime,
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

            }
            catch (...) { cout << "failed" << endl; }

         }

      }

   }

}

int
main (int argc,
      char** argv)
{

   static struct option long_options[] =
   {
      { "annotation",       1, 0, 'a' },
      { "bludge",           0, 0, 'b' },
      { "config",           1, 0, 'c' },
      { "filename",         1, 0, 'F' },
      { "format",           1, 0, 'f' },
      { "geometry",         1, 0, 'g' },
      { "gui",              0, 0, 'G' },
      { "interactive",      0, 0, 'i' },
      { "level",            1, 0, 'l' },
      { "meteogram",        1, 0, 'm' },
      { "output-dir",       1, 0, 'o' },
      { "ignore-pressure",  0, 0, 'P' },
      { "product",          1, 0, 'p' },
      { "no-stage",         0, 0, 'S' },
      { "stage",            1, 0, 's' },
      { "title",            1, 0, 'T' },
      { "time",             1, 0, 't' },
      { "cross-section",    1, 0, 'x' },
      { "u_bg",             1, 0, 'u' },
      { "vertical-profile", 1, 0, 'v' },
      { "no-wind-barb",     0, 0, 'W' },
      { "zoom",             1, 0, 'z' },
      { NULL, 0, NULL, 0 }
   };

   Size_2D size_2d (960, 960);
   Dstring output_dir (".");
   Dstring product_str ("WIND");
   Dstring stage_str ("STAGE3");
   Dstring level_str ("Surface");
   Dstring time_str;
   Dstring zoom_str ("");
   Dstring format ("png");
   Tokens annotation_tokens;

   Dstring filename ("");
   Tokens title_tokens;
   bool is_bludge = false;
   bool is_cross_section = false;
   bool is_gui = false;
   bool is_interactive = false;
   bool is_meteogram = false;
   bool ignore_pressure = false;
   bool no_stage = false;
   bool no_wind_barb = false;
   bool is_vertical_profile = false;
   Real u_bg = 0;
   Dstring location_str ("");
   Journey journey;
   Dstring config_file_path (Dstring (getenv ("HOME")) + "/.twiin.rc");

   int c;
   int option_index = 0;
   while ((c = getopt_long (argc, argv, "a:bc:F:f:Gg:il:m:o:Pp:Ss:T:t:x:u:v:Wz:",
          long_options, &option_index)) != -1)
   {

      switch (c)
      {

         case 'a':
         {
            annotation_tokens.push_back (Dstring (optarg));
            break;
         }

         case 'b':
         {
            is_bludge = true;
            break;
         }

         case 'c':
         {
            config_file_path = (Dstring (optarg));
            break;
         }

         case 'F':
         {
            filename = (Dstring (optarg));
            break;
         }

         case 'f':
         {
            format = (Dstring (optarg));
            break;
         }

         case 'G':
         {
            is_gui = true;
            break;
         }

         case 'g':
         {
            const Tokens tokens (Dstring (optarg), "x");
            size_2d.i = stof (tokens[0]);
            size_2d.j = stof (tokens[1]);
            break;
         }

         case 'i':
         {
            is_interactive = true;
            break;
         }

         case 'l':
         {
            level_str = (Dstring (optarg));
            break;
         }

         case 'm':
         {
            is_gui = false;
            is_interactive = false;
            is_meteogram = true;
            location_str = (Dstring (optarg));
            break;
         }

         case 'o':
         {
            output_dir = (Dstring (optarg));
            break;
         }

         case 'P':
         {
            ignore_pressure = true;
            break;
         }

         case 'p':
         {
            product_str = (Dstring (optarg));
            break;
         }

         case 'S':
         {
            no_stage = true;
            break;
         }

         case 's':
         {
            stage_str = (Dstring (optarg));
            break;
         }

         case 'T':
         {
            title_tokens = Tokens (Dstring (optarg), ":");
            break;
         }

         case 't':
         {
            time_str = (Dstring (optarg));
            break;
         }

         case 'u':
         {
            u_bg = stof (Dstring (optarg));
            break;
         }

         case 'v':
         {
            is_gui = false;
            is_interactive = false;
            is_vertical_profile = true;
            location_str = (Dstring (optarg));
            break;
         }

         case 'W':
         {
            no_wind_barb = true;
            break;
         }

         case 'x':
         {
            is_gui = false;
            is_interactive = false;
            is_cross_section = true;
            journey = Journey (Dstring (optarg));
            break;
         }

         case 'z':
         {
            zoom_str = (Dstring (optarg));
            break;
         }

         default:
         {
            cerr << "Error options " << c << endl;
            break;
         }

      }

   }

   try
   {

      const Config_File config_file (config_file_path);
      Twiin twiin (size_2d, config_file, output_dir);

      if (is_interactive)
      {
         twiin.loop ();
      }
      else
      if (is_gui)
      {
#ifndef ENABLE_GTKMM
         cerr << "Interactive mode not available" << endl;
#else /* ENABLE_GTKMM */
         Gtk::Main gtk_main (argc, argv);
         twiin.gui (stage_str, product_str, level_str, time_str);
#endif /* ENABLE_GTKMM */
      }
      else
      {

         if (is_cross_section)
         {
            twiin.cross_section (stage_str, product_str, journey,
               time_str, format, title_tokens, filename, u_bg, is_bludge);
         }
         else
         if (is_meteogram)
         {
            twiin.meteogram (stage_str, location_str, time_str,
               format, title_tokens, filename, ignore_pressure, is_bludge);
         }
         else
         if (is_vertical_profile)
         {
            twiin.vertical_profile (stage_str, location_str,
               time_str, format, title_tokens, filename, is_bludge);
         }
         else
         {
            twiin.plan (stage_str, product_str, level_str,
               time_str, zoom_str, annotation_tokens, format,
               title_tokens, filename, no_stage, no_wind_barb, is_bludge);
         }
      }

   }
   catch (const Exception& e)
   {
      cerr << "Exception " << e.what () << endl;
   }
   catch (const std::exception& se)
   {
      cerr << "std::exception " << se.what () << endl;
   }

}

void
Twiin::twiin_trajectory_generate (const Dstring& identifier,
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
Twiin::twiin_trajectory_ingest (const Dstring& file_path)
{
   igzstream file (file_path);
   trajectory_map.ingest (file);
   file.close ();
}

void
Twiin::twiin_trajectory_write (const Dstring& file_path) const
{
   ofstream file (file_path, ios::app);
   trajectory_map.write (file);
   file.close ();
}

void
Twiin::twiin_trajectory_print (const Dstring& identifier) const
{

   const Track& trajectory = trajectory_map.at (identifier);
   const set<Dtime>& dtime_set = trajectory.get_dtime_set ();

   for (auto iterator = dtime_set.begin ();
        iterator != dtime_set.end (); iterator++)
   {
      const Dtime& dtime = *(iterator);
      const Dstring& time_str = dtime.get_string ("%Y%m%d%H%M%S");
      const Lat_Long& ll = trajectory.get_lat_long (dtime);
      const Real z = trajectory.get_datum ("z", dtime);

      cout << time_str << " " << ll << " " << z << " ";

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
Twiin::twiin_trajectory (const Tokens& tokens)
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
   {
      throw Exception ("twiin_trajectory");
   }

}

void
Twiin::twiin_surface_plan (const Dstring& surface_identifier,
                           const Dstring& geodetic_transform_identifier,
                           const Dstring& stage,
                           const Model::Product& product,
                           const Level& level,
                           const Dtime& dtime,
                           const Tokens& arguments)
{

   typedef Display D;
   typedef Surface_Package Sp;
   typedef Geodetic_Transform_Package Gtp;

   const RefPtr<Surface>& surface = Sp::get_surface (surface_identifier);
   const RefPtr<Context> cr = Sp::get_cr (surface_identifier);
   const Size_2D& size_2d = Sp::get_size_2d (surface_identifier);
   const Point_2D centre (Real (size_2d.i) / 2, Real (size_2d.j) / 2);

   const Geodetic_Transform* geodetic_transform_ptr =
      Gtp::get_geodetic_transform_ptr (geodetic_transform_identifier, centre);
   const Geodetic_Transform& geodetic_transform = *geodetic_transform_ptr;

   bool no_color_bar = true;
   bool no_scale_bar = true;
   bool no_stage = true;
   bool no_wind_barb = true;

   for (auto iterator = arguments.begin ();
        iterator != arguments.end (); iterator++)
   {
      const Tokens tokens (iterator->get_lower_case (), "=");
      const Dstring& option = tokens[0];
      const Dstring& value = tokens[1];
      const bool is_yes = (value == "yes" || value == "y" || value == "true");
      if (option == "color_bar") { no_color_bar = !is_yes; }
      if (option == "scale_bar") { no_scale_bar = !is_yes; }
      if (option == "stage")     { no_stage = !is_yes; }
      if (option == "wind_barb") { no_wind_barb = !is_yes; }
   }

   Color::white ().cairo (cr);
   cr->paint ();

   const Data data (config_file);
   const Model& model = data.get_model ();
   const Dtime& basetime = model.get_basetime ();
   const Hrit& hrit = data.get_hrit ();
   const Station::Map& station_map = data.get_station_map ();
   Display::render (cr, geodetic_transform, size_2d, model, hrit,
      station_map, dtime, level, stage, product, no_stage, no_wind_barb);

   if (!no_scale_bar) { D::render_scale_bar (cr, geodetic_transform, size_2d); }
   if (!no_color_bar) { D::render_color_bar (cr, size_2d, product); }

   delete geodetic_transform_ptr;

}

void
Twiin::twiin_surface (const Tokens& tokens)
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
Twiin::twiin_parse (const Tokens& tokens)
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
   {
      throw Exception ("twiin_parse");
   }

}

void
Twiin::parse (const Tokens& tokens)
{

   const Dstring& action = tokens[0].get_lower_case ();

   if (action == "twiin")
   {
      twiin_parse (tokens.subtokens (1));
      return;
   }

   andrea::Andrea::parse (tokens);

}

