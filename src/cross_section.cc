#include <denise/dstring.h>
#include "cross_section.h"
#include "model.h"
#include "twiin.h"

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;

void
Cross_Section::Product_Panel::emit (const Model::Product& product)
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
Cross_Section::Product_Panel::add_product (const Dstring& drawer_str,
                                           const Model::Product& product)
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
   const Tuple& tuple_x = journey.get_tuple_x (geodesy);

   Tokens tokens;

   if (marker.x < tuple_x.front () ||
       marker.x > tuple_x.back ())
   {
      return tokens;
   }

   const Lat_Long lat_long = journey.get_lat_long (marker.x, geodesy);
   const Level level (Level::HEIGHT, marker.y);
   const Dtime& dtime = get_time_chooser ().get_time ();

   const Model::Stage& stage = model.get_stage (stage_str);
   tokens = stage.get_marker_tokens (lat_long, dtime, product, level);

   const Dstring& ll_str = lat_long.get_string (false, Dstring ("%.4f\u00b0"));
   const Dstring& height_str = level.get_string ();
   tokens.insert (tokens.begin (), ll_str);
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
                              const Dstring& stage_str,
                              const Model::Product& product,
                              const Dtime& dtime)
   : Console_2D (gtk_window, size_2d),
     Time_Canvas (*this, 12),
     product_panel (*this, 12),
     model (model),
     journey (route),
     stage_str (stage_str),
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

   product_panel.add_product ("Thermo", Model::Product ("T"));
   product_panel.add_product ("Thermo", Model::Product ("THETA"));
   product_panel.add_product ("Thermo", Model::Product ("THETA_V"));
   product_panel.add_product ("Thermo", Model::Product ("Q"));
   product_panel.add_product ("Thermo", Model::Product ("Q_TENDENCY"));
   product_panel.add_product ("Thermo", Model::Product ("TD"));
   product_panel.add_product ("Thermo", Model::Product ("RH"));
   product_panel.add_product ("Thermo", Model::Product ("THETA_E"));
   product_panel.add_product ("Thermo", Model::Product ("RHO"));
   product_panel.add_product ("Derivatives", Model::Product ("Q_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("Q_H_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("Q_V_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("Q_S_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("Q_N_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("Q_SV_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("Q_NV_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("THETA_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("THETA_H_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("THETA_V_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("THETA_S_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("THETA_N_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("THETA_SV_ADVECTION"));
   product_panel.add_product ("Derivatives", Model::Product ("THETA_NV_ADVECTION"));
   product_panel.add_product ("Dynamic", Model::Product ("WIND"));
   product_panel.add_product ("Dynamic", Model::Product ("SPEED"));
   product_panel.add_product ("Dynamic", Model::Product ("W"));
   product_panel.add_product ("Dynamic", Model::Product ("VORTICITY"));
   product_panel.add_product ("Fire", Model::Product ("FFDI"));
   product_panel.add_product ("Misc", Model::Product ("MSLP"));
   product_panel.add_product ("Misc", Model::Product ("TERRAIN"));

   const Model::Stage& stage = model.get_stage (stage_str);
   const set<Dtime>& time_set = stage.get_valid_time_set (product, level);
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
   const Real distance = journey.get_distance (geodesy);
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
Cross_Section::set_product (const Model::Product& product)
{
   this->product = product;
   render_queue_draw ();
}

void
Cross_Section::render_queue_draw ()
{

   const Model::Stage& stage = model.get_stage (stage_str);
   const Dtime& basetime = stage.get_basetime ();
   const Dtime& dtime = get_time_chooser ().get_time ();
   Twiin::set_title (title, basetime, stage_str, product, dtime, journey);

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

   const Model::Stage& stage = model.get_stage (stage_str);
   Twiin::render_cross_section (cr, transform, box_2d,
      domain_z, stage, product, dtime, journey);

}

