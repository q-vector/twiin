#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <denise/geodesy.h>
#include <denise/gtkmm.h>
#include "display.h"

using namespace std;
using namespace denise;
using namespace Cairo;

namespace twiin
{

   class Console : public Map_Console,
                   public Time_Canvas,
                   public Level_Canvas
   {

      private:

         class Product_Panel : public Drawer_Panel
         {

            public:

               typedef sigc::signal<void, const Product&>
               Signal;

               typedef Template_Button<Product>
               Button;

            private:

               Console&
               console;

               Signal
               signal;

               void
               emit (const Product& product);

            public:

               Product_Panel (Console& console,
                              const Real font_size = 12);

               void
               add_product (const string& drawer_str,
                            const Product& product);

               Signal&
               get_signal ();

         };

         Product_Panel
         product_panel;

         const Model
         model;

         //const Station::Map
         //station_map;

         Stage
         stage;

         Product
         product;

         Level
         level;

         Tokens
         get_tokens (const Marker& marker) const;

         virtual bool
         on_key_pressed (const Dkey_Event& event);

         bool
         is_all_collapsed () const;

         void
         unify_drawers (const bool expand);

         void
         process_cross_section (const Integer route_id);

      public:

         Console (Gtk::Window& gtk_window,
                  const Size_2D& size_2d,
                  const Tokens& zoom_tokens,
                  const Tokens& config_file_content,
                  const Stage& stage,
                  const Product& product,
                  const Level& level);

         ~Console ();

         void
         pack ();

         void
         set_product (const Product& product);

         void
         set_level (const Level& level);

         virtual void
         render_queue_draw ();

         void
         render_image_buffer (const RefPtr<Context>& cr);

   };

   void
   start_console ();

};

#endif /* CONSOLE_H */
