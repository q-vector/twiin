#include <getopt.h>
#include <denise/dstring.h>
#include "console.h"
#include "cross_section.h"

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;

void
Console::Product_Panel::emit (const Product& product)
{
   signal.emit (product);
}

Console::Product_Panel::Product_Panel (Console& console,
                                       const Real font_size)
   : Drawer_Panel (console, true, font_size),
     console (console)
{
}

void
Console::Product_Panel::add_product (const string& drawer_str,
                                     const Product& product)
{
   Button* pb_ptr = new Button (console, product, font_size);
   pb_ptr->set_hidable (true);
   Button::T_Signal& signal = pb_ptr->get_t_signal ();
   signal.connect (sigc::mem_fun (*this, &Product_Panel::emit));
   add_widget_ptr (drawer_str, pb_ptr);
}

Console::Product_Panel::Signal&
Console::Product_Panel::get_signal ()
{
   return signal;
}

Tokens
Console::get_tokens (const Marker& marker) const
{

   const Lat_Long lat_long (marker);
   const Real latitude = marker.x;
   const Real longitude = marker.y;

   if (model.out_of_bounds (lat_long, stage))
   {
      return Map_Console::get_tokens (marker);
   }

   const Dtime& dtime = get_time_chooser ().get_time ();

   Tokens tokens = model.get_marker_tokens (lat_long,
      dtime, product, stage, level);

   const string& lat_long_str = lat_long.get_string ("false", "%.3f\u00b0");
   tokens.insert (tokens.begin (), lat_long_str);

   return tokens;

}

bool
Console::is_all_collapsed () const
{
   const bool p = product_panel.all_collapsed ();
   const bool all_collapsed = (p);
   return all_collapsed;
}

bool
Console::on_key_pressed (const Dkey_Event& event)
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
Console::unify_drawers (const bool expand)
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
Console::process_cross_section (const Integer route_id)
{

   const Console_2D::Route& route = get_route_store ().get_route (route_id);
   const Multi_Journey multi_journey (route);

   const Size_2D size_2d (960, 480);
   const Dtime& dtime = get_time_chooser ().get_time ();

   Cross_Section* cross_section_ptr = new Cross_Section (
      gtk_window, size_2d, model, route, stage, product, dtime);

   Gtk::Window* gtk_window_ptr = new Gtk::Window ();
   gtk_window_ptr->add (*cross_section_ptr);
   gtk_window_ptr->show_all_children ();
   gtk_window_ptr->show ();
   gtk_window_ptr->set_resizable (true);
   gtk_window_ptr->resize (size_2d.i, size_2d.j);

}

Console::Console (Gtk::Window& gtk_window,
                  const Size_2D& size_2d,
                  const Tokens& zoom_tokens,
                  const Tokens& config_file_content,
                  const Stage& stage,
                  const Product& product,
                  const Level& level)
   : Map_Console (gtk_window, size_2d, zoom_tokens),
     Time_Canvas (*this, 12),
     Level_Canvas (*this, 12),
     product_panel (*this, 12),
     model (config_file_content),
     stage (stage),
     product (product),
     level (level)
{

   time_chooser.get_signal ().connect (
      sigc::mem_fun (*this, &Console::render_queue_draw));
   level_panel.get_level_signal ().connect (
      sigc::mem_fun (*this, &Console::set_level));
   product_panel.get_signal ().connect (
      sigc::mem_fun (*this, &Console::set_product));

   level_panel.add_extra_level (Level ("Surface"));
   level_panel.set_level (level);

   product_panel.set_hidable (true);
   time_chooser.set_hidable (true);
   level_panel.set_hidable (true);

   register_widget (time_chooser);
   register_widget (level_panel);
   register_widget (product_panel);

   product_panel.add_product ("Thermo", Product ("P_THETA"));
   product_panel.add_product ("Thermo", Product ("T"));
   product_panel.add_product ("Thermo", Product ("THETA"));
   product_panel.add_product ("Thermo", Product ("Q"));
   product_panel.add_product ("Thermo", Product ("TD"));
   product_panel.add_product ("Thermo", Product ("RH"));
   product_panel.add_product ("Thermo", Product ("THETA_E"));
   product_panel.add_product ("Thermo", Product ("RHO"));
   product_panel.add_product ("Dynamic", Product ("P_RHO"));
   product_panel.add_product ("Dynamic", Product ("WIND"));
   product_panel.add_product ("Dynamic", Product ("W"));
   product_panel.add_product ("Dynamic", Product ("VORTICITY"));
   product_panel.add_product ("Fire", Product ("FFDI"));
   product_panel.add_product ("Misc", Product ("MSLP"));
   product_panel.add_product ("Misc", Product ("TERRAIN"));

   const set<Dtime>& time_set = model.get_valid_time_set (product, stage, level);
   time_chooser.set_shape (Time_Chooser::Shape (time_set));
   time_chooser.set_leap (1);

   route_popup_menu.append ("Cross Section");
   route_popup_menu.get_id_signal ("Cross Section").connect (
      sigc::mem_fun (*this, &Console::process_cross_section));

   pack ();

}

Console::~Console ()
{
}

void
Console::pack ()
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

   time_chooser.being_packed (tc_anchor, tc_width, tc_height);
   time_chooser.pack ();

   level_panel.being_packed (lp_anchor, lp_width, lp_height);
   level_panel.pack ();

   product_panel.being_packed (pp_anchor, pp_width, pp_height);
   product_panel.pack ();


}

void
Console::set_level (const Level& level)
{
   this->level = level;
   render_queue_draw ();
}

void
Console::set_product (const Product& product)
{
   this->product = product;
   render_queue_draw ();
}

void
Console::render_queue_draw ()
{

   const Dtime& dtime = get_time_chooser ().get_time ();
   const string& time_string = dtime.get_string ("%Y.%m.%d %H:%M UTC");
   title.set ("", time_string, product, stage, level.get_string ());
   set_foreground_ready (false);

   Map_Console::render_queue_draw ();

}

void
Console::render_image_buffer (const RefPtr<Context>& cr)
{

   const Size_2D& size_2d = get_size_2d ();
   const Transform_2D& transform = get_transform ();
   const Dtime& dtime = get_time_chooser ().get_time ();
   const Level& level = get_level_panel ().get_level ();

   Display::render (cr, transform, size_2d, model, //station_map,
      dtime, level, stage, product);

}

namespace twiin
{

   void
   interactive (const Size_2D& size_2d,
                const Tokens& zoom_tokens,
                const Tokens& config_file_content,
                const string& stage_str,
                const string& product_str,
                const string& level_str,
                const string& time_str)
   {

      const Tokens stage_tokens (stage_str, ":");
      const Tokens product_tokens (product_str, ":");

      const Stage stage (stage_tokens[0]);
      const Product product (product_tokens[0]);
      const Level level (level_str);

      Gtk::Window gtk_window;
      Console console (gtk_window, size_2d, zoom_tokens,
         config_file_content, stage, product, level);
      gtk_window.add (console);
      gtk_window.show_all_children ();
      gtk_window.show ();
      gtk_window.set_resizable (true);
      gtk_window.resize (size_2d.i, size_2d.j);
      Gtk::Main::run (gtk_window);

   }

   void
   command_line (const Size_2D& size_2d,
                 const Tokens& zoom_tokens,
                 const Tokens& config_file_content,
                 const string& stage_str,
                 const string& product_str,
                 const string& level_str,
                 const string& time_str,
                 const bool is_bludge)
   {

      const Dtime::Set time_set (time_str);
      const Level level (level_str);
      const Tokens stage_tokens (stage_str, ":");
      const Tokens product_tokens (product_str, ":");
      const Point_2D centre (size_2d.i/2, size_2d.j/2);

      typedef Geodetic_Transform Gt;
      map<string, Transform_2D*> transform_ptr_map;

      for (auto iterator = zoom_tokens.begin ();
           iterator != zoom_tokens.end (); iterator++)
      {
         const Tokens tokens (*(iterator), "/");
         const string& identifier = tokens[0];
         const string& zoom_str = tokens[1];
         Geodetic_Transform* gt_ptr = Gt::get_transform_ptr (zoom_str, centre);
         transform_ptr_map.insert (make_pair (identifier, gt_ptr));
      }

      Title title (size_2d);
      const Model model (config_file_content);

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

               const string png_file_path = stage + "_" + product +
                  "_" + dtime.get_string ("%Y%m%d%H%M") + ".png";
               cout << "Rendering " << png_file_path << endl;
               if (is_bludge) { continue; }

               RefPtr<ImageSurface> surface = ImageSurface::create (
                  FORMAT_ARGB32, size_2d.i, size_2d.j);
               RefPtr<Context> cr = Context::create (surface);
               cr->select_font_face ("Verdana", FONT_SLANT_NORMAL,
                  FONT_WEIGHT_NORMAL);

               Display::render (cr, transform, size_2d,
                  model, dtime, level, stage, product);
               const string& time_str = dtime.get_string ("%Y.%m.%d %H:%M UTC");
               title.set ("", time_str, product, stage, level.get_string ());
               title.cairo (cr);

               surface->write_to_png (png_file_path);

            }

         }

      }

      for (auto iterator = transform_ptr_map.begin ();
           iterator != transform_ptr_map.end (); iterator++)
      {
         Transform_2D* transform_ptr = iterator->second;
         delete transform_ptr;
      }

   }

   void
   cross_section (const Size_2D& size_2d,
                  const Tokens& zoom_tokens,
                  const Tokens& config_file_content,
                  const string& stage_str,
                  const string& product_str,
                  const Multi_Journey& multi_journey,
                  const string& time_str,
                  const bool is_bludge)
   {

      const Dtime::Set time_set (time_str);
      const Tokens stage_tokens (stage_str, ":");
      const Tokens product_tokens (product_str, ":");

      const Geodesy geodesy;
      const Real distance = multi_journey.get_distance (geodesy);

      const Domain_1D domain_x (0, distance);
      const Domain_1D domain_z (0, 8000);

      Title title (size_2d);
      const Model model (config_file_content);

      const Real margin_l = 50;
      const Real margin_r = 50;
      const Real margin_t = title.get_height () + 50;
      const Real margin_b = 50;
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

            for (auto k = valid_time_set.begin ();
                 k != valid_time_set.end (); k++)
            {

               const Dtime& dtime = *(k);
               if (!time_set.match (dtime)) { continue; }

               const string png_file_path = "crosssection_" + stage + "_" +
                  product + "_" + dtime.get_string ("%Y%m%d%H%M") + ".png";
               cout << "Rendering " << png_file_path << endl;
               if (is_bludge) { continue; }

               RefPtr<ImageSurface> surface = ImageSurface::create (
                  FORMAT_ARGB32, size_2d.i, size_2d.j);
               RefPtr<Context> cr = Context::create (surface);
               cr->select_font_face ("Verdana", FONT_SLANT_NORMAL,
                  FONT_WEIGHT_NORMAL);

               const Index_2D i2d (margin_l, margin_t);
               const Size_2D s2d (size_2d.i - margin_l - margin_r,
                                  size_2d.j - margin_t - margin_b);
               const Box_2D box_2d (i2d, s2d);

               if (s2d.i < 0 || s2d.j < 0) { continue; }

               Display::render_cross_section (cr, transform, box_2d,
                  domain_z, model, stage, product, dtime, multi_journey);
               const string& time_str = dtime.get_string ("%Y.%m.%d %H:%M UTC");
               title.set ("", time_str, product, stage, "");
               title.cairo (cr);

               surface->write_to_png (png_file_path);

            }
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
      { "product", 1, 0, 'p' },
      { "stage", 1, 0, 's' },
      { "time", 1, 0, 't' },
      { "cross-section", 1, 0, 'x' },
      { NULL, 0, NULL, 0 }
   };

   Size_2D size_2d (960, 960);
   string product_str ("WIND");
   string stage_str ("STAGE3");
   string level_str ("Surface");
   string time_str;

   int c;
   bool is_bludge = false;
   bool is_interactive = false;
   int option_index = 0;

   Multi_Journey multi_journey;

   while ((c = getopt_long (argc, argv, "bg:il:p:s:t:x:",
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

   const string config_file_path (argv[optind]);

   const string zoom_str_3 ("LAMBERT_CONIC_SOUTH:3000:-33.5:150.5");
   const string zoom_str_4 ("LAMBERT_CONIC_SOUTH:1200:-33.75:150.5");
   const string zoom_str_5 ("LAMBERT_CONIC_SOUTH:380:-33.7:150.55");

   Tokens zoom_tokens;
   zoom_tokens.push_back ("STAGE3/" + zoom_str_3);
   zoom_tokens.push_back ("STAGE4/" + zoom_str_4);
   zoom_tokens.push_back ("STAGE5/" + zoom_str_5);

   try
   {

      const Tokens& config_file_content = read_config_file (config_file_path);

      if (is_interactive)
      {
         Gtk::Main gtk_main (argc, argv);
         interactive (size_2d, zoom_tokens, config_file_content,
            stage_str, product_str, level_str, time_str);
      }
      else
      {
         const bool is_cross_section = (multi_journey.size () >= 2);
         if (is_cross_section)
         {
            cross_section (size_2d, zoom_tokens, config_file_content,
               stage_str, product_str, multi_journey, time_str, is_bludge);
         }
         else
         {
            command_line (size_2d, zoom_tokens, config_file_content,
               stage_str, product_str, level_str, time_str, is_bludge);
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

