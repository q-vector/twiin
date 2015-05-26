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
   return Console_2D::get_tokens (marker);
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
                              const Display& display,
                              const string& stage_str,
                              const string& product_str)
   : Console_2D (gtk_window, size_2d),
     Time_Canvas (*this, 12),
     product_panel (*this, 12),
     display (display),
     stage (stage_str),
     product (product_str)
{

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
   product_panel.add_product ("Thermo", Product ("TD"));
   product_panel.add_product ("Thermo", Product ("RH"));
   product_panel.add_product ("Thermo", Product ("THETA_E"));
   product_panel.add_product ("Dynamic", Product ("WIND"));
   product_panel.add_product ("Dynamic", Product ("VORTICITY"));
   product_panel.add_product ("Fire", Product ("FFDI"));
   product_panel.add_product ("Misc", Product ("MSLP"));
   product_panel.add_product ("Misc", Product ("TERRAIN"));

   const Model& model = display.get_model ();
   const Model::Stage& model_stage = model.get_model_stage (stage);
   const set<Dtime>& time_set = model_stage.get_valid_time_set ();
   time_chooser.set_shape (Time_Chooser::Shape (time_set));
   time_chooser.set_leap (1);

   pack ();

}

Cross_Section::~Cross_Section ()
{
}

void
Cross_Section::pack ()
{

   const Real title_height = title.get_height ();
   const Real margin = title_height * 0.15;
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

   const Size_2D& size_2d = get_size_2d ();
   const Transform_2D& transform = get_transform ();
   const Dtime& dtime = get_time_chooser ().get_time ();

   cr->save ();
   Color (0.47, 0.85, 0.47).cairo (cr);

cout << "render image buffer" << endl;
   cr->restore ();

}

int
main (int argc,
      char** argv)
{

   const string vertical_coefficients_file_path (argv[1]);
   const string orog_3_file_path (argv[2]);
   const string lsm_3_file_path (argv[3]);
   const string orog_4_file_path (argv[4]);
   const string lsm_4_file_path (argv[5]);
   const string orog_5_file_path (argv[6]);
   const string lsm_5_file_path (argv[7]);
   const string station_file_path (argv[8]);

   Model::Stage::File_Path_Map model_file_path_3_map;
   Model::Stage::File_Path_Map model_file_path_4_map;
   Model::Stage::File_Path_Map model_file_path_5_map;
   model_file_path_3_map.insert (string ("temp"), argv[9]);
   model_file_path_3_map.insert (string ("dewpt"), argv[10]);
   model_file_path_3_map.insert (string ("xwind"), argv[11]);
   model_file_path_3_map.insert (string ("ywind"), argv[12]);
   model_file_path_3_map.insert (string ("mslp"), argv[13]);
   model_file_path_3_map.insert (string ("ml_prho"), argv[14]);
   model_file_path_3_map.insert (string ("ml_ptheta"), argv[15]);
   model_file_path_3_map.insert (string ("ml_theta"), argv[16]);
   model_file_path_3_map.insert (string ("ml_spechum"), argv[17]);
   model_file_path_3_map.insert (string ("ml_xwind"), argv[18]);
   model_file_path_3_map.insert (string ("ml_ywind"), argv[19]);
   model_file_path_4_map.insert (string ("temp"), argv[20]);
   model_file_path_4_map.insert (string ("dewpt"), argv[21]);
   model_file_path_4_map.insert (string ("xwind"), argv[22]);
   model_file_path_4_map.insert (string ("ywind"), argv[23]);
   model_file_path_4_map.insert (string ("mslp"), argv[24]);
   model_file_path_5_map.insert (string ("temp"), argv[25]);
   model_file_path_5_map.insert (string ("dewpt"), argv[26]);
   model_file_path_5_map.insert (string ("xwind"), argv[27]);
   model_file_path_5_map.insert (string ("ywind"), argv[28]);
   model_file_path_5_map.insert (string ("mslp"), argv[29]);

   const string product_str (argv[30]);
   const string stage_str (argv[31]);
   const string level_str (argv[32]);

   try
   {

      Gtk::Main gtk_main (argc, argv);
      Gtk::Window gtk_window;

      const Size_2D size_2d (1280, 720);
      const Display display (vertical_coefficients_file_path,
         orog_3_file_path, lsm_3_file_path, orog_4_file_path, lsm_4_file_path,
         orog_5_file_path, lsm_5_file_path, station_file_path,
         model_file_path_3_map, model_file_path_4_map, model_file_path_5_map);

      Cross_Section cross_section (gtk_window, size_2d,
         display, stage_str, product_str);
      gtk_window.add (cross_section);
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

