#include <getopt.h>
#include <denise/dstring.h>
#include "main.h"

#ifndef ENABLE_GTKMM
#else /* ENABLE_GTKMM */
#include "cross_section.h"
#include "console.h"
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

      const Tokens tokens (*(iterator));
      if (tokens.size () != 3) { continue; }
      if (tokens[0] != "geodetic_transform") { continue; }

      const string& identifier = tokens[1];
      const string& zoom_str = tokens[2];
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

string
Twiin::get_file_path (const string& format,
                      const Stage& stage,
                      const Product& product,
                      const Level& level,
                      const Dtime& dtime) const
{
   const string& time_str = dtime.get_string ("%Y%m%d%H%M");
   const string& file_name = stage + "_" + product + "_" +
      level.get_string () + "_" + time_str + "." + format;
   return output_dir + "/" + file_name;
}

string
Twiin::get_file_path (const string& format,
                      const Stage& stage,
                      const Product& product,
                      const Dtime& dtime,
                      const Multi_Journey& multi_journey) const
{

   const Lat_Long origin (multi_journey.front ());
   const Lat_Long destination (multi_journey.back ());

   string mj_str;
   for (auto iterator = multi_journey.begin ();
        iterator != multi_journey.end (); iterator++)
   {
      const Lat_Long lat_long (*(iterator));
      const string& ll_str = lat_long.get_string (true, string ("%.4f"));
      mj_str += ll_str;
   }

   const string& time_str = dtime.get_string ("%Y%m%d%H%M");
   const string& file_name = stage + "_" + product + "_" +
      time_str + "_" + mj_str + "." + format;
   return output_dir + "/" + file_name;

}

string
Twiin::get_file_path (const string& format,
                      const Stage& stage,
                      const Dtime& dtime,
                      const Location& location) const
{
   const string& location_str = location.get_str ();
   const string& time_str = dtime.get_string ("%Y%m%d%H%M");
   const string& file_name = stage + "_" + time_str + "_"
      + location_str + "." + format;
   return output_dir + "/" + file_name;
}

string
Twiin::get_file_path (const string& format,
                      const Stage& stage,
                      const Dtime& dtime,
                      const string& location_name) const
{
   const string& time_str = dtime.get_string ("%Y%m%d%H%M");
   const string& file_name = stage + "_" + time_str + "_"
      + location_name + "." + format;
   return output_dir + "/" + file_name;
}

string
Twiin::get_file_path (const string& format,
                      const Stage& stage,
                      const Location& location) const
{
   const string& location_str = location.get_str ();
   const string& file_name = stage + "_" + location_str + "." + format;
   return output_dir + "/" + file_name;
}

Twiin::Twiin (const Size_2D& size_2d,
              const Config_File& config_file,
              const string& output_dir)
   : size_2d (size_2d),
     config_file (config_file),
     output_dir (output_dir)
{
}

#ifndef ENABLE_GTKMM
#else /* ENABLE_GTKMM */
void
Twiin::interactive (const string& stage_str,
                    const string& product_str,
                    const string& level_str,
                    const string& time_str) const
{

   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");
   const Tokens level_tokens (level_str, ":");

   const Stage stage (stage_tokens[0]);
   const Product product (product_tokens[0]);
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
Twiin::command_line (const string& stage_str,
                     const string& product_str,
                     const string& level_str,
                     const string& time_str,
                     const string& zoom_str,
                     const Tokens& annotation_tokens,
                     const string& format,
                     const Tokens& title_tokens,
                     const string& filename,
                     const bool no_stage,
                     const bool no_wind_barb,
                     const bool is_bludge) const
{

   Gshhs* gshhs_ptr = NULL;

   for (auto iterator = config_file.begin ();
        iterator != config_file.end (); iterator++)
   {

      const Tokens tokens (*(iterator));
      if (tokens.size () != 2) { continue; }
      if (tokens[0] != "overlay") { continue; }

      const Tokens overlay_tokens (tokens[1], ":");
      if (overlay_tokens[0] != "GSHHS") { continue; }

      const string gshhs_file_path (overlay_tokens[8]);
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

      const twiin::Stage stage (*i);
      const Geodetic_Transform& transform = gt_ptr == NULL ?
         *(transform_ptr_map[stage]) : *gt_ptr;

      const Geodetic_Mesh& mesh = transform.get_mesh (size_2d);

      for (Tokens::const_iterator j = product_tokens.begin ();
           j != product_tokens.end (); j++)
      {

         const Product product (*j);
         const bool is_speed = (product == "SPEED");

         for (auto k = level_tokens.begin ();
              k != level_tokens.end (); k++)
         {

            const Level level (*k);
            const bool is_higher = (level.type == HEIGHT_LEVEL) &&
               (level.value > 1500);
            const Product& p = ((is_speed && is_higher) ?
               Product ("SPEED_HIGHER") : product);
           
            const auto& valid_time_set = model.get_valid_time_set (
               product, stage, level);

            for (auto l = valid_time_set.begin ();
                 l != valid_time_set.end (); l++)
            {
           
               const Dtime& dtime = *(l);
               if (!time_set.match (dtime)) { continue; }

               const string& file_path = (filename == "") ?
                  get_file_path (format, stage, product, level, dtime) :
                  output_dir + "/" + filename;
               cout << "Rendering " << file_path << endl;
               if (is_bludge) { continue; }

               RefPtr<Surface> surface = denise::get_surface (
                  size_2d, format, file_path);
               RefPtr<Context> cr = denise::get_cr (surface);

               Display::render (cr, transform, size_2d, model, hrit,
                  station_map, dtime, level, stage, p, no_stage,
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
                     stage, product, dtime, level);
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

         }

      }

   }

   if (gt_ptr != NULL) { delete gt_ptr; }

}

void
Twiin::cross_section (const string& stage_str,
                      const string& product_str,
                      const Multi_Journey& multi_journey,
                      const string& time_str,
                      const string& format,
                      const Tokens& title_tokens,
                      const string& filename,
                      const bool is_bludge) const
{

   const Dtime::Set time_set (time_str);
   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");

   const Geodesy geodesy;
   const Real distance = multi_journey.get_distance (geodesy);
   const Lat_Long origin (multi_journey.front ());
   const Lat_Long destination (multi_journey.back ());

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

      const twiin::Stage stage (*i);

      for (Tokens::const_iterator j = product_tokens.begin ();
           j != product_tokens.end (); j++)
      {

         const Product product (*j);
         const auto& valid_time_set = model.get_valid_time_set (
            product, stage, Level ("100m"));

         const bool is_speed = (product == "SPEED");
         const Product& p = (is_speed ? Product ("SPEED_HIGHER") : product);

         for (auto iterator = valid_time_set.begin ();
              iterator != valid_time_set.end (); iterator++)
         {

            const Dtime& dtime = *(iterator);
            if (!time_set.match (dtime)) { continue; }

            const string& file_path = (filename == "") ?
               get_file_path (format, stage, product, dtime, multi_journey) :
               output_dir + "/" + filename;
            cout << "Rendering " << file_path << endl;
            if (is_bludge) { continue; }

            RefPtr<Surface> surface = denise::get_surface (
               size_2d, format, file_path);
            RefPtr<Context> cr = denise::get_cr (surface);

            const Index_2D i2d (margin_l, margin_t);
            const Size_2D s2d (size_2d.i - margin_l - margin_r,
                               size_2d.j - margin_t - margin_b);
            const Box_2D box_2d (i2d, s2d);

            if (s2d.i < 0 || s2d.j < 0) { continue; }

            Display::render_cross_section (cr, transform, box_2d,
               domain_z, model, stage, product, dtime, multi_journey);

            Display::render_color_bar (cr, size_2d, p);

            if (title_tokens.size () == 0)
            {
               Display::set_title (title, basetime, stage,
                  product, dtime, multi_journey);
            }
            else
            {
               title.set (title_tokens);
            }
            title.cairo (cr);

            if (format == "png") { surface->write_to_png (file_path); }

         }

      }
   }

};

void
Twiin::meteogram (const string& stage_str,
                  const string& location_str,
                  const string& time_str,
                  const string& format,
                  const Tokens& title_tokens,
                  const string& filename,
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

      const twiin::Stage stage (*i);

      const auto& valid_time_set = model.get_valid_time_set (
         Product ("T"), stage, Level ("Surface"));

      for (Tokens::const_iterator j = location_tokens.begin ();
           j != location_tokens.end (); j++)
      {

         const Location location (*j, station_map);

         const string& file_path = (filename == "") ?
            get_file_path (format, stage, location) :
            output_dir + "/" + filename;
         cout << "Rendering " << file_path << endl;
         if (is_bludge) { continue; }

         RefPtr<Surface> surface = denise::get_surface (
            size_2d, format, file_path);
         RefPtr<Context> cr = denise::get_cr (surface);

         Display::render_meteogram (cr, size_2d, model,
            aws_repository, stage, location, ignore_pressure);

         if (title_tokens.size () == 0)
         {
            Display::set_title (title, basetime, stage, location);
         }
         else
         {
            title.set (title_tokens);
         }
         title.cairo (cr);

         if (format == "png") { surface->write_to_png (file_path); }

      }

   }

}

void
Twiin::vertical_profile (const string& stage_str,
                         const string& location_str,
                         const string& time_str,
                         const string& format,
                         const Tokens& title_tokens,
                         const string& filename,
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

   const Tephigram tephigram (size_2d);

   for (Tokens::const_iterator i = stage_tokens.begin ();
        i != stage_tokens.end (); i++)
   {

      const twiin::Stage stage (*i);

      const auto& valid_time_set = model.get_valid_time_set (
         Product ("T"), stage, Level ("100m"));

      for (auto iterator = valid_time_set.begin ();
           iterator != valid_time_set.end (); iterator++)
      {

         const Dtime& dtime = *(iterator);
         if (!time_set.match (dtime)) { continue; }

         for (Tokens::const_iterator j = location_tokens.begin ();
              j != location_tokens.end (); j++)
         {

            string location_name ("");
            Lat_Long::List lat_long_list;

            const string& location_token = *j;
            const Location sole_location (location_token, station_map);
            const bool is_mj = Reg_Exp ("=").match (location_token);

            if (is_mj)
            {
               const Geodesy geodesy;
               const Tokens tokens (location_token, "/");
               lat_long_list.add (Multi_Journey (tokens[0]), geodesy);
               location_name = tokens.size () > 1 ? tokens[1] : "mean";
            }
            else
            {
               lat_long_list.push_back (Location (location_token, station_map));
            }

            const string& file_path =
               (filename == "") ?
                  ((location_name == "") ?
                     get_file_path (format, stage, dtime, sole_location) :
                     get_file_path (format, stage, dtime, location_name)) :
                  output_dir + "/" + filename;
            cout << "Rendering " << file_path << endl;
            if (is_bludge) { continue; }

            RefPtr<Surface> surface = denise::get_surface (
               size_2d, format, file_path);
            RefPtr<Context> cr = denise::get_cr (surface);

            if (location_name == "")
            {
               Display::render_vertical_profile (cr, tephigram,
                  model, stage, dtime, sole_location);
            }
            else
            {
               Display::render_vertical_profile (cr, tephigram,
                  model, stage, dtime, lat_long_list);
            }

            if (title_tokens.size () == 0)
            {
               if (location_name == "")
               {
                  Display::set_title (title, basetime,
                     stage, dtime, sole_location);
               }
               else
               {
                  Display::set_title (title, basetime,
                     stage, dtime, location_name);
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
      { "vertical-profile", 1, 0, 'v' },
      { "no-wind-barb",     0, 0, 'W' },
      { "zoom",             1, 0, 'z' },
      { NULL, 0, NULL, 0 }
   };

   Size_2D size_2d (960, 960);
   string output_dir (".");
   string product_str ("WIND");
   string stage_str ("STAGE3");
   string level_str ("Surface");
   string time_str;
   string zoom_str ("");
   string format ("png");
   Tokens annotation_tokens;

   string filename ("");
   Tokens title_tokens;
   bool is_bludge = false;
   bool is_cross_section = false;
   bool is_interactive = false;
   bool is_meteogram = false;
   bool ignore_pressure = false;
   bool no_stage = false;
   bool no_wind_barb = false;
   bool is_vertical_profile = false;
   string location_str ("");
   Multi_Journey multi_journey;
   string config_file_path (string (getenv ("HOME")) + "/.twiin.rc");

   int c;
   int option_index = 0;
   while ((c = getopt_long (argc, argv, "a:bc:F:f:g:il:m:o:Pp:Ss:T:t:x:v:Wz:",
          long_options, &option_index)) != -1)
   {

      switch (c)
      {

         case 'a':
         {
            annotation_tokens.push_back (string (optarg));
            break;
         }

         case 'b':
         {
            is_bludge = true;
            break;
         }

         case 'c':
         {
            config_file_path = (string (optarg));
            break;
         }

         case 'F':
         {
            filename = (string (optarg));
            break;
         }

         case 'f':
         {
            format = (string (optarg));
            break;
         }

         case 'g':
         {
            const Tokens tokens (string (optarg), "x");
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
            level_str = (string (optarg));
            break;
         }

         case 'm':
         {
            is_interactive = false;
            is_meteogram = true;
            location_str = (string (optarg));
            break;
         }

         case 'o':
         {
            output_dir = (string (optarg));
            break;
         }

         case 'P':
         {
            ignore_pressure = true;
            break;
         }

         case 'p':
         {
            product_str = (string (optarg));
            break;
         }

         case 'S':
         {
            no_stage = true;
            break;
         }

         case 's':
         {
            stage_str = (string (optarg));
            break;
         }

         case 'T':
         {
            title_tokens = Tokens (string (optarg), ":");
            break;
         }

         case 't':
         {
            time_str = (string (optarg));
            break;
         }

         case 'v':
         {
            is_interactive = false;
            is_vertical_profile = true;
            location_str = (string (optarg));
            break;
         }

         case 'W':
         {
            no_wind_barb = true;
            break;
         }

         case 'x':
         {
            is_interactive = false;
            is_cross_section = true;
            multi_journey = Multi_Journey (string (optarg));
            break;
         }

         case 'z':
         {
            zoom_str = (string (optarg));
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
      const Twiin twiin (size_2d, config_file, output_dir);

      if (is_interactive)
      {
#ifndef ENABLE_GTKMM
         cerr << "Interactive mode not available" << endl;
#else /* ENABLE_GTKMM */
         Gtk::Main gtk_main (argc, argv);
         twiin.interactive (stage_str, product_str, level_str, time_str);
#endif /* ENABLE_GTKMM */
      }
      else
      {

         if (is_cross_section)
         {
            twiin.cross_section (stage_str, product_str, multi_journey,
               time_str, format, title_tokens, filename, is_bludge);
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
            twiin.command_line (stage_str, product_str, level_str,
               time_str, zoom_str, annotation_tokens, format,
               title_tokens, filename, no_stage, no_wind_barb, is_bludge);
         }
      }

   }
   catch (const Exception& e)
   {
      cerr << "Exception " << e << endl;
   }
   catch (const std::exception& se)
   {
      cerr << "std::exception " << se.what () << endl;
   }

}

