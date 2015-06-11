#include <denise/dstring.h>
#include "cross_section.h"

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;

void
Cross_Section::Product_Panel::emit (const Product& product)
{
   signal.emit (product);
}

Cross_Section::Product_Panel::Product_Panel (Cross_Section& cross_section,
                                             const Real font_size)
   : Drawer_Panel (cross_section, true, font_size),
     cross_section (cross_section)
{
}

void
Cross_Section::Product_Panel::add_product (const string& drawer_str,
                                           const Product& product)
{
   Button* pb_ptr = new Button (cross_section, product, font_size);
   pb_ptr->set_hidable (true);
   Button::T_Signal& signal = pb_ptr->get_t_signal ();
   signal.connect (sigc::mem_fun (*this, &Product_Panel::emit));
   add_widget_ptr (drawer_str, pb_ptr);

}

Cross_Section::Product_Panel::Signal&
Cross_Section::Product_Panel::get_signal ()
{
   return signal;
}

Tokens
Cross_Section::get_tokens (const Marker& marker) const
{


   const Geodesy geodesy;
   const Tuple& tuple_x = multi_journey.get_tuple_x (geodesy);

   Tokens tokens;

   if (marker.x < tuple_x.front () ||
       marker.x > tuple_x.back ())
   {
      return tokens;
   }

   const Lat_Long lat_long = multi_journey.get_lat_long (marker.x, geodesy);
   const Level level (HEIGHT_LEVEL, marker.y);
   const Dtime& dtime = get_time_chooser ().get_time ();

   tokens = model.get_marker_tokens (
      lat_long, dtime, product, stage, level);

   const string& lat_long_str = lat_long.get_string ();
   const string& height_str = level.get_string ();
   tokens.insert (tokens.begin (), lat_long_str);
   tokens.insert (tokens.begin (), height_str);

   //return Console_2D::get_tokens (marker);
   return tokens;

}

bool
Cross_Section::is_all_collapsed () const
{
   const bool p = product_panel.all_collapsed ();
   const bool all_collapsed = (p);
   return all_collapsed;
}

bool
Cross_Section::on_key_pressed (const Dkey_Event& event)
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

   return Console_2D::on_key_pressed (event);

}

void
Cross_Section::unify_drawers (const bool expand)
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

Cross_Section::Cross_Section (Gtk::Window& gtk_window,
                              const Size_2D& size_2d,
                              const Model& model,
                              const Route& route,
                              const Stage& stage,
                              const Product& product,
                              const Dtime& dtime)
   : Console_2D (gtk_window, size_2d),
     Time_Canvas (*this, 12),
     product_panel (*this, 12),
     model (model),
     multi_journey (route),
     stage (stage),
     product (product),
     domain_z (0, 8000)
{

   // So that it knows we are only checking the uppers
   const Level level ("200m");

   time_chooser.get_signal ().connect (
      sigc::mem_fun (*this, &Cross_Section::render_queue_draw));
   product_panel.get_signal ().connect (
      sigc::mem_fun (*this, &Cross_Section::set_product));

   product_panel.set_hidable (true);
   time_chooser.set_hidable (true);

   register_widget (time_chooser);
   register_widget (product_panel);

   product_panel.add_product ("Thermo", Product ("T"));
   product_panel.add_product ("Thermo", Product ("THETA"));
   product_panel.add_product ("Thermo", Product ("Q"));
   product_panel.add_product ("Thermo", Product ("TD"));
   product_panel.add_product ("Thermo", Product ("RH"));
   product_panel.add_product ("Thermo", Product ("THETA_E"));
   product_panel.add_product ("Thermo", Product ("RHO"));
   product_panel.add_product ("Dynamic", Product ("WIND"));
   product_panel.add_product ("Dynamic", Product ("W"));
   product_panel.add_product ("Dynamic", Product ("VORTICITY"));
   product_panel.add_product ("Fire", Product ("FFDI"));
   product_panel.add_product ("Misc", Product ("MSLP"));
   product_panel.add_product ("Misc", Product ("TERRAIN"));

   const set<Dtime>& time_set =
      model.get_valid_time_set (product, stage, level);
   time_chooser.set_shape (Time_Chooser::Shape (time_set));
   time_chooser.set_leap (1);
   time_chooser.set_data (Time_Chooser::Data (dtime));

   pack ();
   reset_transform ();

}

Cross_Section::~Cross_Section ()
{
}

void
Cross_Section::pack ()
{

   const Real title_height = title.get_height ();
   const Real margin = title_height * 0.2;
   const Real font_size = title_height * 0.3;
   const Real button_height = font_size * 2;

   const Real tc_width = time_chooser.get_preferred_width ();
   const Real tc_height = (height - title_height - 2 * margin);
   const Point_2D tc_anchor (margin, margin + title_height);

   const Real lp_width = width * 0.07;

   const Real pp_width = width - tc_width - lp_width - 4 * margin;
   const Real pp_height = button_height;
   const Real pp_anchor_x = tc_width + 2 * margin;
   const Real pp_anchor_y = height - pp_height - margin;
   const Point_2D pp_anchor (pp_anchor_x, pp_anchor_y);

   time_chooser.being_packed (tc_anchor, tc_width, tc_height);
   time_chooser.pack ();

   product_panel.being_packed (pp_anchor, pp_width, pp_height);
   product_panel.pack ();

   this->margin_l = 10 * margin + tc_width;
   this->margin_r = 4 * margin;
   this->margin_t = title_height + 4 * margin;
   this->margin_b = 5 * margin + pp_height;

}

void
Cross_Section::reset_transform ()
{

   const Real top_z = 8000;
   const Real bottom_z = 0;

   const Geodesy geodesy;
   const Real distance = multi_journey.get_distance (geodesy);
   const Domain_1D domain_z (top_z, bottom_z);
   const Domain_1D domain_x (0, distance);
   const Domain_2D domain_2d (domain_x, domain_z);

   const Real w = width - margin_l - margin_r;
   const Real h = height - margin_t - margin_b;
   const Point_2D p (margin_l, margin_t);

   Console_2D::reset_transform ();
   const Real span_x = domain_x.get_span ();
   const Real span_z = domain_z.get_span ();
   Affine_Transform_2D& transform = get_affine_transform ();
   transform.scale (w / span_x, h / span_z);
   transform.translate (0, height);
   transform.translate (margin_l, -margin_b);

}

void
Cross_Section::set_product (const Product& product)
{
   this->product = product;
   render_queue_draw ();
}

void
Cross_Section::render_queue_draw ()
{

   const Dtime& dtime = get_time_chooser ().get_time ();
   const string& time_string = dtime.get_string ("%Y.%m.%d %H:%M UTC");

   const Lat_Long origin (multi_journey.front ());
   const Lat_Long destination (multi_journey.back ());
   const bool complex_mj = (multi_journey.size () > 2);
   const string o_suffix (complex_mj ? " ..." : "");
   const string d_preffix (complex_mj ? "... " : "");
   const string& o_str = origin.get_string (3) + o_suffix;
   const string& d_str = d_preffix + destination.get_string (3);

   title.set (time_string, o_str, product, stage, d_str);
   set_foreground_ready (false);

   Console_2D::render_queue_draw ();

}

void
Cross_Section::render_image_buffer (const RefPtr<Context>& cr)
{

   const Size_2D& size_2d = get_size_2d ();
   const Dtime& dtime = get_time_chooser ().get_time ();
   const Affine_Transform_2D& transform = get_affine_transform ();

   const Index_2D i2d (margin_l, margin_t);
   const Size_2D s2d (size_2d.i - margin_l - margin_r,
                      size_2d.j - margin_t - margin_b);
   const Box_2D box_2d (i2d, s2d);

   if (s2d.i < 0 || s2d.j < 0) { return; }

   Display::render_cross_section (cr, transform, box_2d,
      domain_z, model, stage, product, dtime, multi_journey);

}

