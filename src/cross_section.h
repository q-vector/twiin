#ifndef CROSS_SECTION_H
#define CROSS_SECTION_H

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

   class Cross_Section : public Console_2D,
                         public Time_Canvas
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

               Cross_Section&
               cross_section;

               Signal
               signal;

               void
               emit (const Product& product);

            public:

               Product_Panel (Cross_Section& console,
                              const Real font_size = 12);

               void
               add_product (const string& drawer_str,
                            const Product& product);

               Signal&
               get_signal ();

         };

         Product_Panel
         product_panel;

         const Display&
         display;

         Stage
         stage;

         Product
         product;

         Tokens
         get_tokens (const Marker& marker) const;

         virtual bool
         on_key_pressed (const Dkey_Event& event);

         bool
         is_all_collapsed () const;

         void
         unify_drawers (const bool expand);

      public:

         Cross_Section (Gtk::Window& gtk_window,
                        const Size_2D& size_2d,
                        const Display& display,
                        const string& stage_str,
                        const string& product_str);

         ~Cross_Section ();

         void
         pack ();

         void
         set_product (const Product& product);

         virtual void
         render_queue_draw ();

         void
         render_image_buffer (const RefPtr<Context>& cr);

   };

};

#endif /* CROSS_SECTION_H */
