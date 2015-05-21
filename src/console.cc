#include <denise/dstring.h>
#include "console.h"

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
   const string& lat_long_str = lat_long.get_string ("false", "%.3f\u00b0");

   const Model& model = display.get_model ();
   const Model::Stage& model_stage = model.get_model_stage (stage);
   const Real latitude = marker.x;
   const Real longitude = marker.y;
   if (model_stage.out_of_bounds (latitude, longitude))
   {
      return Map_Console::get_tokens (marker);
   }

   const Dtime& dtime = get_time_chooser ().get_time ();

   Tokens tokens;
   tokens.push_back (lat_long_str);

   if (product == "T")
   {
      const Real datum = model.evaluate (TEMPERATURE,
         latitude, longitude, 0, dtime, stage);
      tokens.push_back (string_render ("%.1f\u00b0C", datum - K));
   }
   else
   if (product == "TD")
   {
      const Real datum = model.evaluate (DEW_POINT,
         latitude, longitude, 0, dtime, stage);
      tokens.push_back (string_render ("%.1f\u00b0C", datum - K));
   }
   else
   if (product == "RH")
   {
      const Real datum = model.evaluate (RELATIVE_HUMIDITY,
         latitude, longitude, 0, dtime, stage);
      tokens.push_back (string_render ("%.0f%%", datum * 100));
   }
   else
   if (product == "THETA_E")
   {
      const Real datum = model.evaluate (THETA_E,
         latitude, longitude, 0, dtime, stage);
      tokens.push_back (string_render ("%.1f\u00b0C", datum - K));
   }
   else
   if (product == "WIND")
   {
      const Real u = model.evaluate (ZONAL_WIND,
         latitude, longitude, 0, dtime, stage);
      const Real v = model.evaluate (MERIDIONAL_WIND,
         latitude, longitude, 0, dtime, stage);
      const Wind wind (u, v);
      const Real msknot = 3.6/1.852;
      const string fmt ("%03.0f\u00b0 / %02.1fkt");
      tokens.push_back (wind.get_string (msknot, fmt));
   }
   else
   if (product == "FFDI")
   {
      const Real datum = model.evaluate (FFDI,
         latitude, longitude, 0, dtime, stage);
      tokens.push_back (string_render ("%02.2f", datum));
   }
   else
   if (product == "TERRAIN")
   {
   }
   else
   if (product == "VORTICITY")
   {
      const Real datum = model.evaluate (RELATIVE_VORTICITY,
         latitude, longitude, 0, dtime, stage);
      tokens.push_back (string_render ("%.4e", datum));
   }
   else
   if (product == "MSLP")
   {
      const Real datum = model.evaluate (MEAN_SEA_LEVEL_PRESSURE,
         latitude, longitude, 0, dtime, stage);
      tokens.push_back (string_render ("%0.1fhPa", datum * 1e-2));
   }

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

Console::Console (Gtk::Window& gtk_window,
                  const Size_2D& size_2d,
                  const Tokens& zoom_tokens,
                  const Display& display,
                  const string& stage_str,
                  const string& product_str)
   : Map_Console (gtk_window, size_2d, zoom_tokens),
     Time_Canvas (*this, 12),
     display (display),
     product_panel (*this, 12),
     stage (stage_str),
     product (product_str)
{

   time_chooser.get_signal ().connect (
      sigc::mem_fun (*this, &Console::render_queue_draw));

   product_panel.get_signal ().connect (
      sigc::mem_fun (*this, &Console::set_product));

   product_panel.set_hidable (true);

   time_chooser.set_hidable (true);
   register_widget (time_chooser);
   register_widget (product_panel);

   product_panel.add_product ("Raw", Product ("T"));
   product_panel.add_product ("Raw", Product ("TD"));
   product_panel.add_product ("Raw", Product ("WIND"));
   product_panel.add_product ("Raw", Product ("MSLP"));
   product_panel.add_product ("Derived", Product ("RH"));
   product_panel.add_product ("Derived", Product ("THETA_E"));
   product_panel.add_product ("Derived", Product ("FFDI"));
   product_panel.add_product ("Derived", Product ("VORTICITY"));
   product_panel.add_product ("Misc", Product ("TERRAIN"));

   const Model& model = display.get_model ();
   const Model::Stage& model_stage = model.get_model_stage (stage);
   const set<Dtime>& time_set = model_stage.get_valid_time_set ();
   time_chooser.set_shape (Time_Chooser::Shape (time_set));
   time_chooser.set_leap (1);

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

   const Real lp_width = width * 0.06;
   const Real lp_height = height - title_height - 4*margin - 2*button_height; 
   const Real lp_anchor_x = width - lp_width - margin;
   const Real lp_anchor_y = 2*margin + button_height + title_height;
   const Point_2D lp_anchor (lp_anchor_x, lp_anchor_y);

   const Real tc_width = time_chooser.get_preferred_width ();
   const Real tc_height = (height - title_height - 2 * margin);
   const Point_2D tc_anchor (margin, margin + title_height);

   const Real cb_width = lp_width;
   const Real cb_height = button_height;
   const Real cb_anchor_x = lp_anchor_x;
   const Real cb_anchor_y = height - cb_height - margin;
   const Point_2D cb_anchor (cb_anchor_x, cb_anchor_y);

   const Real pp_width = width - tc_width - cb_width - 4 * margin;
   const Real pp_height = button_height;
   const Real pp_anchor_x = tc_width + 2 * margin;
   const Real pp_anchor_y = height - pp_height - margin;
   const Point_2D pp_anchor (pp_anchor_x, pp_anchor_y);

   time_chooser.being_packed (tc_anchor, tc_width, tc_height);
   time_chooser.pack ();

   product_panel.being_packed (pp_anchor, pp_width, pp_height);
   product_panel.pack ();


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
   title.set (dtime.get_string ("%Y.%m.%d %H:%M UTC"), product, stage);
   set_foreground_ready (false);

   Map_Console::render_queue_draw ();

}

void
Console::render_image_buffer (const RefPtr<Context>& cr)
{
   const Size_2D& size_2d = get_size_2d ();
   const Transform_2D& transform = get_transform ();
   const Dtime& dtime = get_time_chooser ().get_time ();

   display.cairo (cr, transform, size_2d, dtime, stage, product);
}

int
main (int argc,
      char** argv)
{

   const string orog_3_file_path (argv[1]);
   const string lsm_3_file_path (argv[2]);
   const string orog_4_file_path (argv[3]);
   const string lsm_4_file_path (argv[4]);
   const string orog_5_file_path (argv[5]);
   const string lsm_5_file_path (argv[6]);
   const string station_file_path (argv[7]);

   map<Model::Varname, string> model_file_path_3_map;
   map<Model::Varname, string> model_file_path_4_map;
   map<Model::Varname, string> model_file_path_5_map;
   model_file_path_3_map.insert (make_pair (Model::Varname ("temp"), string (argv[8])));
   model_file_path_3_map.insert (make_pair (Model::Varname ("dewpt"), string (argv[9])));
   model_file_path_3_map.insert (make_pair (Model::Varname ("xwind"), string (argv[10])));
   model_file_path_3_map.insert (make_pair (Model::Varname ("ywind"), string (argv[11])));
   model_file_path_3_map.insert (make_pair (Model::Varname ("mslp"), string (argv[12])));
   model_file_path_4_map.insert (make_pair (Model::Varname ("temp"), string (argv[13])));
   model_file_path_4_map.insert (make_pair (Model::Varname ("dewpt"), string (argv[14])));
   model_file_path_4_map.insert (make_pair (Model::Varname ("xwind"), string (argv[15])));
   model_file_path_4_map.insert (make_pair (Model::Varname ("ywind"), string (argv[16])));
   model_file_path_4_map.insert (make_pair (Model::Varname ("mslp"), string (argv[17])));
   model_file_path_5_map.insert (make_pair (Model::Varname ("temp"), string (argv[18])));
   model_file_path_5_map.insert (make_pair (Model::Varname ("dewpt"), string (argv[19])));
   model_file_path_5_map.insert (make_pair (Model::Varname ("xwind"), string (argv[20])));
   model_file_path_5_map.insert (make_pair (Model::Varname ("ywind"), string (argv[21])));
   model_file_path_5_map.insert (make_pair (Model::Varname ("mslp"), string (argv[22])));

   const string product_str (argv[23]);
   const string stage_str (argv[24]);

   try
   {

      Gtk::Main gtk_main (argc, argv);
      Gtk::Window gtk_window;

      const Size_2D size_2d (960, 960);

      Tokens zoom_tokens;
      zoom_tokens.push_back ("Stage3/LAMBERT_CONIC_SOUTH:3000:-33.5:150.5");
      zoom_tokens.push_back ("Stage4/LAMBERT_CONIC_SOUTH:1200:-33.75:150.5");
      zoom_tokens.push_back ("Stage5/LAMBERT_CONIC_SOUTH:380:-33.7:150.55");
      const Display display (orog_3_file_path, lsm_3_file_path,
         orog_4_file_path, lsm_4_file_path, orog_5_file_path,
         lsm_5_file_path, station_file_path, model_file_path_3_map,
         model_file_path_4_map, model_file_path_5_map);
      Console console (gtk_window, size_2d, zoom_tokens,
         display, stage_str, product_str);
      gtk_window.add (console);
      gtk_window.show_all_children ();
      gtk_window.show ();
      gtk_window.set_resizable (true);
      gtk_window.resize (size_2d.i, size_2d.j);
      Gtk::Main::run (gtk_window);

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

