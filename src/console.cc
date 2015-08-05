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
   const Model& model = data.get_model ();

   if (model.out_of_bounds (lat_long, stage))
   {
      return Map_Console::get_tokens (marker);
   }

   const Dtime& dtime = get_time_chooser ().get_time ();

   Tokens tokens = model.get_marker_tokens (lat_long,
      dtime, product, stage, level);

   const string& ll_str = lat_long.get_string (false, string ("%.4f\u00b0"));
   tokens.insert (tokens.begin (), ll_str);

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
Console::process_tephigram (const Integer marker_id)
{

   const auto& marker = get_marker_store ().get_marker (marker_id);
   const Lat_Long lat_long (marker);
   const Dtime& dtime = get_time_chooser ().get_time ();

   const Model& model = data.get_model ();
   Sounding* sounding_ptr = model.get_sounding_ptr (lat_long, dtime, stage);
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
Console::process_aws (const Integer marker_id)
{

   const auto& marker = get_marker_store ().get_marker (marker_id);
   const Lat_Long lat_long (marker);

   cout << "aws for " << lat_long.get_string () << endl;

}

void
Console::process_cross_section (const Integer route_id)
{

   const auto& route = get_route_store ().get_route (route_id);
   const Multi_Journey multi_journey (route);

   const Size_2D size_2d (960, 480);
   const Dtime& dtime = get_time_chooser ().get_time ();
   const Model& model = data.get_model ();

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
                  const Config_File& config_file,
                  const Data& data,
                  const Stage& stage,
                  const Product& product,
                  const Level& level)
   : Map_Console (gtk_window, size_2d, config_file),
     Time_Canvas (*this, 12),
     Level_Canvas (*this, 12),
     product_panel (*this, 12),
     data (data),
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

   option_panel.set_hidable (true);
   product_panel.set_hidable (true);
   time_chooser.set_hidable (true);
   level_panel.set_hidable (true);

   register_widget (option_panel);
   register_widget (time_chooser);
   register_widget (level_panel);
   register_widget (product_panel);

   product_panel.add_product ("Thermo", Product ("P_THETA"));
   product_panel.add_product ("Thermo", Product ("T"));
   product_panel.add_product ("Thermo", Product ("THETA"));
   product_panel.add_product ("Thermo", Product ("THETA_V"));
   product_panel.add_product ("Thermo", Product ("Q"));
   product_panel.add_product ("Thermo", Product ("TD"));
   product_panel.add_product ("Thermo", Product ("RH"));
   product_panel.add_product ("Thermo", Product ("THETA_E"));
   product_panel.add_product ("Thermo", Product ("RHO"));
   product_panel.add_product ("Dynamic", Product ("P_RHO"));
   product_panel.add_product ("Dynamic", Product ("WIND"));
   product_panel.add_product ("Dynamic", Product ("SPEED"));
   product_panel.add_product ("Dynamic", Product ("SPEED_HIGHER"));
   product_panel.add_product ("Dynamic", Product ("W"));
   //product_panel.add_product ("Dynamic", Product ("W_TRANSLUCENT"));
   product_panel.add_product ("Dynamic", Product ("VORTICITY"));
   product_panel.add_product ("Fire", Product ("FFDI"));
   product_panel.add_product ("Misc", Product ("PRECIP_RATE"));
   product_panel.add_product ("Misc", Product ("MSLP"));
   product_panel.add_product ("Misc", Product ("TERRAIN"));
   product_panel.add_product ("Misc", Product ("IR1"));
   product_panel.add_product ("Misc", Product ("IR2"));
   product_panel.add_product ("Misc", Product ("IR3"));
   product_panel.add_product ("Misc", Product ("IR4"));
   product_panel.add_product ("Misc", Product ("VIS"));
   product_panel.add_product ("Misc", Product ("Pseudo"));

   const Model& model = data.get_model ();
   const set<Dtime>& ts = model.get_valid_time_set (product, stage, level);
   time_chooser.set_shape (Time_Chooser::Shape (ts));
   time_chooser.set_leap (1);

   marker_popup_menu.append ("Tephigram");
   marker_popup_menu.get_id_signal ("Tephigram").connect (
      sigc::mem_fun (*this, &Console::process_tephigram));

   marker_popup_menu.append ("AWS");
   marker_popup_menu.get_id_signal ("AWS").connect (
      sigc::mem_fun (*this, &Console::process_aws));

   route_popup_menu.append ("Cross Section");
   route_popup_menu.get_id_signal ("Cross Section").connect (
      sigc::mem_fun (*this, &Console::process_cross_section));

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

   const Lat_Long mountain_wave_rjf_a (-32.4774, 147.1520);
   const Lat_Long mountain_wave_rjf_b (-34.3215, 153.2280);
   const Lat_Long mountain_wave_a (-31.7849, 147.9219);
   const Lat_Long mountain_wave_b (-35.4056, 152.7273);
   const Lat_Long dry_slow_nw_a (-32.8879, 144.7283);
   const Lat_Long dry_slow_nw_b (-29.0739, 149.1825);
   get_route_store ().insert (mountain_wave_a, mountain_wave_b);
   get_route_store ().insert (mountain_wave_rjf_a, mountain_wave_rjf_b);
   get_route_store ().insert (dry_slow_nw_a, dry_slow_nw_b);

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
   const Model& model = data.get_model ();
   const Dtime& basetime = model.get_basetime ();
   const Dtime& dtime = get_time_chooser ().get_time ();
   Display::set_title (title, basetime, stage, product, dtime, level);
   set_foreground_ready (false);
   Map_Console::render_queue_draw ();
}

void
Console::render_image_buffer (const RefPtr<Context>& cr)
{

   const Model& model = data.get_model ();
   const Hrit& hrit = data.get_hrit ();
   const Station::Map& station_map = data.get_station_map ();

   const Size_2D& size_2d = get_size_2d ();
   const Geodetic_Transform& transform =
      dynamic_cast<Geodetic_Transform&>(get_transform ());
   const Dtime& dtime = get_time_chooser ().get_time ();
   const Level& level = get_level_panel ().get_level ();

   Display::render (cr, transform, size_2d, model, hrit,
      station_map, dtime, level, stage, product, false, false);
   render_mesh (cr);
   render_overlays (cr);

}

