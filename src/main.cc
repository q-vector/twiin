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
                                             const Tokens& config_file_content)
{

   typedef Geodetic_Transform Gt;
   const Point_2D centre (size_2d.i/2, size_2d.j/2);

   for (auto iterator = config_file_content.begin ();
        iterator != config_file_content.end (); iterator++)
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
Twiin::get_png_file_path (const Stage& stage,
                          const Product& product,
                          const Dtime& dtime) const
{
   const string& time_str = dtime.get_string ("%Y%m%d%H%M");
   const string& file_name = stage + "_" + product + "_" + time_str + ".png";
   return output_dir + "/" + file_name;
}

string
Twiin::get_png_file_path (const Stage& stage,
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
      const string& ll_str = lat_long.get_string (true, string ("%.3f"));
      mj_str += ll_str;
   }

   const string& time_str = dtime.get_string ("%Y%m%d%H%M");
   const string& file_name =  mj_str + "_" + stage +
      "_" + product + "_" + time_str + ".png";
   return output_dir + "/" + file_name;

}

Twiin::Twiin (const Size_2D& size_2d,
              const Tokens& config_file_content,
              const string& output_dir)
   : size_2d (size_2d),
     config_file_content (config_file_content),
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

   const Stage stage (stage_tokens[0]);
   const Product product (product_tokens[0]);
   const Level level (level_str);

   Gtk::Window gtk_window;
   Console console (gtk_window, size_2d,
      config_file_content, stage, product, level);
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
                     const bool is_bludge) const
{

   const Dtime::Set time_set (time_str);
   const Level level (level_str);
   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");

   Transform_Ptr_Map transform_ptr_map (size_2d, config_file_content);

   Title title (size_2d);
   const Model model (config_file_content);
   const Hrit hrit (config_file_content);

   for (Tokens::const_iterator i = stage_tokens.begin ();
        i != stage_tokens.end (); i++)
   {

      const twiin::Stage stage (*i);
      const Transform_2D& transform = *(transform_ptr_map[stage]);

      for (Tokens::const_iterator j = product_tokens.begin ();
           j != product_tokens.end (); j++)
      {

         const Product product (*j);
         const auto& valid_time_set = model.get_valid_time_set (
            product, stage, level);

         for (auto k = valid_time_set.begin ();
              k != valid_time_set.end (); k++)
         {
           
            const Dtime& dtime = *(k);
            if (!time_set.match (dtime)) { continue; }

            const string& png_file_path =
               get_png_file_path (stage, product, dtime);
            cout << "Rendering " << png_file_path << endl;
            if (is_bludge) { continue; }

            RefPtr<ImageSurface> surface = denise::get_surface (size_2d);
            RefPtr<Context> cr = denise::get_cr (surface);

            Display::render (cr, transform, size_2d,
               model, hrit, dtime, level, stage, product);
            Display::set_title (title, stage, product, dtime, level);
            title.cairo (cr);

            surface->write_to_png (png_file_path);

         }

      }

   }

}

void
Twiin::cross_section (const string& stage_str,
                      const string& product_str,
                      const Multi_Journey& multi_journey,
                      const string& time_str,
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
   const Model model (config_file_content);

   const Real margin_l = 60;
   const Real margin_r = 40;
   const Real margin_t = title.get_height () + 40;
   const Real margin_b = 40;
   const Real w = size_2d.i - margin_l - margin_r;
   const Real h = size_2d.j - margin_t - margin_b;

   Affine_Transform_2D transform;
   const Real span_x = domain_x.get_span ();
   const Real span_z = domain_z.get_span ();
   transform.scale (1, -1);
   transform.scale (w / span_x, h / span_z);
   transform.translate (0, size_2d.j);
   transform.translate (margin_l, -margin_b);

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

         for (auto iterator = valid_time_set.begin ();
              iterator != valid_time_set.end (); iterator++)
         {

            const Dtime& dtime = *(iterator);
            if (!time_set.match (dtime)) { continue; }

            const string& png_file_path =
               get_png_file_path (stage, product, dtime, multi_journey);
            cout << "Rendering " << png_file_path << endl;
            if (is_bludge) { continue; }

            RefPtr<ImageSurface> surface = denise::get_surface (size_2d);
            RefPtr<Context> cr = denise::get_cr (surface);

            const Index_2D i2d (margin_l, margin_t);
            const Size_2D s2d (size_2d.i - margin_l - margin_r,
                               size_2d.j - margin_t - margin_b);
            const Box_2D box_2d (i2d, s2d);

            if (s2d.i < 0 || s2d.j < 0) { continue; }

            Display::render_cross_section (cr, transform, box_2d,
               domain_z, model, stage, product, dtime, multi_journey);

            Display::set_title (title, stage, product, dtime, multi_journey);
            title.cairo (cr);

            surface->write_to_png (png_file_path);

         }

      }
   }

};

int
main (int argc,
      char** argv)
{

   static struct option long_options[] =
   {
      { "bludge", 0, 0, 'b' },
      { "geometry", 1, 0, 'g' },
      { "interactive", 0, 0, 'i' },
      { "level", 1, 0, 'l' },
      { "output-dir", 1, 0, 'o' },
      { "product", 1, 0, 'p' },
      { "stage", 1, 0, 's' },
      { "time", 1, 0, 't' },
      { "cross-section", 1, 0, 'x' },
      { NULL, 0, NULL, 0 }
   };

   Size_2D size_2d (960, 960);
   string output_dir (".");
   string product_str ("WIND");
   string stage_str ("STAGE3");
   string level_str ("Surface");
   string time_str;

   int c;
   bool is_bludge = false;
   bool is_interactive = false;
   int option_index = 0;

   Multi_Journey multi_journey;

   while ((c = getopt_long (argc, argv, "bg:il:o:p:s:t:x:",
          long_options, &option_index)) != -1)
   {

      switch (c)
      {

         case 'b':
         {
            is_bludge = true;
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

         case 'o':
         {
            output_dir = (string (optarg));
            break;
         }

         case 'p':
         {
            product_str = (string (optarg));
            break;
         }

         case 's':
         {
            stage_str = (string (optarg));
            break;
         }

         case 't':
         {
            time_str = (string (optarg));
            break;
         }

         case 'x':
         {
            is_interactive = false;
            Tokens tokens (string (optarg), ":");
            while (tokens.size () >= 2)
            {
               const Lat_Long lat_long (stof (tokens[0]), stof (tokens[1]));
               tokens.erase (tokens.begin (), tokens.begin () + 2);
               multi_journey.push_back (lat_long);
            }
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

      const string config_file_path (string (getenv ("HOME")) + "/.twiin.rc");
      const Tokens& config_file_content = read_config_file (config_file_path);

      const Twiin twiin (size_2d, config_file_content, output_dir);

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
         const bool is_cross_section = (multi_journey.size () >= 2);
         if (is_cross_section)
         {
            twiin.cross_section (stage_str, product_str,
               multi_journey, time_str, is_bludge);
         }
         else
         {
            twiin.command_line (stage_str, product_str,
               level_str, time_str, is_bludge);
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

