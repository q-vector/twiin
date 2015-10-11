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

               typedef sigc::signal<void, const Model::Product&>
               Signal;

               typedef Template_Button<Model::Product>
               Button;

            private:

               Console&
               console;

               Signal
               signal;

               void
               emit (const Model::Product& product);

            public:

               Product_Panel (Console& console,
                              const Real font_size = 12);

               void
               add_product (const Dstring& drawer_str,
                            const Model::Product& product);

               Signal&
               get_signal ();

         };

         Product_Panel
         product_panel;

         const Data&
         data;

         //const Station::Map
         //station_map;

         Dstring
         stage_str;

         Model::Product
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
         process_tephigram (const Integer marker_id);

         void
         process_aws (const Integer marker_id);

         void
         process_cross_section (const Integer route_id);

      public:

         Console (Gtk::Window& gtk_window,
                  const Size_2D& size_2d,
                  const Config_File& config_file,
                  const Data& data,
                  const Dstring& stage_str,
                  const Model::Product& product,
                  const Level& level,
                  const Dtime& dtime,
                  const Tokens& journey_tokens);

         ~Console ();

         void
         pack ();

         void
         set_product (const Model::Product& product);

         void
         set_level (const Level& level);

         virtual void
         render_queue_draw ();

         void
         render_image_buffer (const RefPtr<Context>& cr);

   };

};

#endif /* CONSOLE_H */
