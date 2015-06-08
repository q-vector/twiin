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
   product_panel.add_product ("Dynamic", Product ("WIND"));
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

   this->margin_l = 16 * margin + tc_width;
   this->margin_r = 4 * margin;
   this->margin_t = title_height + 4 * margin;
   this->margin_b = 8 * margin + pp_height;

}

void
Cross_Section::reset_transform ()
{

   const Real top_z = 8000;
   const Real bottom_z = 0;

   const Geodesy geodesy;
   const Tuple& tuple_x = multi_journey.get_tuple_x (geodesy);
   const Domain_1D domain_z (top_z, bottom_z);
   const Domain_1D domain_x (tuple_x.front (), tuple_x.back ());
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
   title.set (time_string, product, stage);
   set_foreground_ready (false);

   Console_2D::render_queue_draw ();

}

void
Cross_Section::render_image_buffer (const RefPtr<Context>& cr)
{

   const Geodesy geodesy;
   const Size_2D& size_2d = get_size_2d ();
   const Dtime& dtime = get_time_chooser ().get_time ();

   cr->save ();
   //Color::hsb (0.67, 0.67, 0.85).cairo (cr);
   Color::hsb (0.0, 0.0, 1.0).cairo (cr);
   cr->paint ();

   const Simple_Mesh_2D ma0 (Color (0, 0, 0, 0.05), 1e8, 10);
   const Simple_Mesh_2D ma1 (Color (0, 0, 0, 0.1), 1e8, 100);
   const Simple_Mesh_2D ma2 (Color (0, 0, 0, 0.4), 1e8, 1000);

   const Affine_Transform_2D& transform = get_affine_transform ();
   const Real distance = multi_journey.get_distance (geodesy);

   Real d_distance = 100e3;
   if (distance < 1000e3) { d_distance = 50e3; }
   if (distance < 400e3) { d_distance = 20e3; }
   if (distance < 200e3) { d_distance = 10e3; }

   const Multi_Journey mj (multi_journey, geodesy, 100e3);

   const Tuple& tuple_x = mj.get_tuple_x (geodesy);
   const Domain_1D domain_x (tuple_x.front (), tuple_x.back ());
   const Domain_2D domain_2d (domain_x, domain_z);
   const Mesh_2D mesh_2d (domain_2d, ma2);

   const Index_2D i2d (margin_l, margin_t);
   const Size_2D s2d (size_2d.i-margin_l-margin_r, size_2d.j-margin_t-margin_b);

   Real x;

   Level level ("200m");
   Real& z = level.value;

   Nwp_Element nwp_element;
   if (product == "T") { nwp_element = T; }
   if (product == "Q") { nwp_element = Q; }
   if (product == "TD") { nwp_element = TD; }
   if (product == "RH") { nwp_element = RH; }
   //if (product == "WIND") { nwp_element = T; }
   if (product == "THETA") { nwp_element = T; }
   if (product == "THETA_E") { nwp_element = T; }

   if (s2d.i > 0 && s2d.j > 0)
   {

      const Box_2D box_2d (i2d, s2d);
      Raster* raster_ptr = new Raster (box_2d);
      Raster& raster = *raster_ptr;

      Color color;
      const Model::Terrain::Stage& terrain_stage =
         model.terrain.get_stage (stage);
      const Model::Uppers::Stage& uppers_stage =
         model.uppers.get_stage (stage);
      const size_t l = uppers_stage.get_l (dtime);

      for (Integer i = i2d.i; i < i2d.i + s2d.i; i++)
      {

         transform.reverse (x, z, Real (i), 0);
         if (x < tuple_x.front () || x > tuple_x.back ()) { continue; }
         const Lat_Long lat_long = multi_journey.get_lat_long (x, geodesy);
         if (model.out_of_bounds (lat_long, stage)) { continue; }

         const Real topography = terrain_stage.get_topography (lat_long);

         for (Integer j = i2d.j; j < i2d.j + s2d.j; j++)
         {
            transform.reverse (x, z, Real (i), Real (j));
            if (z < topography) { color = Color::hsb (0.0, 0.0, 0.0); }
            else
            {
               const Nwp_Element nwp_element = product.get_nwp_element ();
               const Real datum = model.evaluate (
                  nwp_element, lat_long, level, dtime, stage);
               color = Display::get_color (product, datum);
            }
            raster.set_pixel (i - i2d.i, j - i2d.j, color);
         }

      }

      raster.blit (cr);
      delete raster_ptr;

   }

   cr->set_line_width (2);
   mesh_2d.render (cr, transform, Size_2D (2, 2));
   Color (0, 0, 0, 0.3).cairo (cr);

   mesh_2d.render_label_x (cr, transform, 0, 0,
      "%.0f", NUMBER_REAL, 'c', 't', 5);
   mesh_2d.render_label_y (cr, transform, 0, 0,
      "%.0f", NUMBER_REAL, 'r', 'c', 5);

   for (Tuple::const_iterator iterator = tuple_x.begin ();
        iterator != tuple_x.end (); iterator++)
   {

      const Real d = *(iterator);
      const Point_2D& point_a = transform.transform (Point_2D (d, 0));
      const Point_2D& point_b = transform.transform (Point_2D (d, 8000));

      const Integer i = std::distance (tuple_x.begin (), iterator);
      const string& str = string_render ("%d", i);

      Color (0, 0, 0, 1).cairo (cr);
      Label (str, point_a, 'c', 't', 6).cairo (cr);

      //Color (0, 0, 0, 0.0).cairo (cr);
      //cr->move_to (point_a.x, point_a.y);
      //cr->line_to (point_b.x, point_b.y);
      //cr->stroke ();

   }


   {

      Real x, p;
      Point_2D point;

      const Real arrow_size = 15;
      const Real h = arrow_size * 1.5;

      Color (0, 0, 0, 0.2).cairo (cr);
      const Model::Terrain::Stage& terrain_stage =
         model.terrain.get_stage (stage);
      const Model::Uppers::Stage& uppers_stage =
         model.uppers.get_stage (stage);
      const size_t l = uppers_stage.get_l (dtime);

      for (Integer i = i2d.i; i < i2d.i + s2d.i; i += Integer (h))
      {

         transform.reverse (x, z, Real (i), 0);
         if (x < tuple_x.front () || x > tuple_x.back ()) { continue; }
         const Lat_Long lat_long = multi_journey.get_lat_long (x, geodesy);
         if (model.out_of_bounds (lat_long, stage)) { continue; }

         const Real topography = terrain_stage.get_topography (lat_long);

         for (Integer j = i2d.j; j < i2d.j + s2d.j; j += Integer (h))
         {
            transform.reverse (x, z, Real (i), Real (j));
            if (z < topography) { continue; }
            else
            {
               Real u = model.evaluate (
                  U, lat_long, level, dtime, stage);
               Real v = model.evaluate (
                  V, lat_long, level, dtime, stage);
               Real w = model.evaluate (
                  W, lat_long, level, dtime, stage);
               transform.transform_uv (u, w, x, z);
               const Real theta = atan2 (w, u);
               const Real mag = sqrt (u*u + w*w);
               const Real as = std::min (3600 * mag, arrow_size);
               const Arrow arrow (theta, arrow_size, 0.12);
               arrow.cairo (cr, Point_2D (i, j));
               Color (0, 0, 0, 0.05).cairo (cr);
               cr->fill_preserve ();
               Color (0, 0, 0, 0.10).cairo (cr);
               cr->stroke ();

            }

         }

      }
   }


   cr->restore ();

}

