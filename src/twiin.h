#ifndef TWIIN_H
#define TWIIN_H

#include <iostream>
#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <denise/geodesy.h>
#include <andrea/andrea.h>
#include "data.h"
<<<<<<< HEAD
#include "model.h"
=======

#ifndef ENABLE_GTKMM
#else  /* ENABLE_GTKMM */
#include <denise/gtkmm.h>
#endif /* ENABLE_GTKMM */
>>>>>>> 5fa20472beb086837a94d7ce59134be3755dac62

using namespace std;
using namespace denise;
using namespace andrea;
using namespace Cairo;

namespace twiin
{

   class Twiin
   {

      public:

         class Transform_Ptr_Map : public map<Dstring, Geodetic_Transform*>
         {

            public:

               Transform_Ptr_Map (const Size_2D& size_2d,
                                  const Config_File& config_file);

               ~Transform_Ptr_Map ();

         };

         const Size_2D
         size_2d;

         const Config_File
         config_file;

         const Dstring
         output_dir;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Model::Product& product,
                        const Level& level,
                        const Dtime& dtime) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Model::Product& product,
                        const Dtime& dtime,
                        const Journey& journey) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Model::Product& product,
                        const Dtime& dtime,
                        const Lat_Long& lat_long,
                        const Real distance,
                        const bool lagrangian) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Model::Product& product,
                        const Location& location) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Model::Product& product,
                        const Dstring& track_id,
                        const bool lagrangian) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Model::Product& product,
                        const Level& level,
                        const Dtime& dtime,
                        const Dstring& track_id) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Dtime& dtime,
                        const Location& location) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Dtime& dtime,
                        const Dstring& location_name) const;

         Dstring
         get_file_path (const Dstring& format,
                        const Dstring& stage,
                        const Location& location) const;

      public:

         Twiin (const Size_2D& size_2d,
                const Config_File& config_file,
                const Dstring& output_dir);

#ifndef ENABLE_GTKMM
#else /* ENABLE_GTKMM */
         void
         gui (const Dstring& stage_str,
              const Dstring& product_str,
              const Dstring& level_str,
              const Dstring& time_str,
              const Tokens& journey_tokens,
              const Dstring& zoom_str) const;
#endif /* ENABLE_GTKMM */

         void
         plan (const Dstring& stage_str,
               const Dstring& product_str,
               const Dstring& level_str,
               const Dstring& time_str,
               const Dstring& zoom_str,
               const Dstring& track_id_str,
               const bool track_id_initial,
               const Track::Map& track_map,
               const Tokens& annotation_tokens,
               const Dstring& format,
               const Tokens& title_tokens,
               const Dstring& filename,
               const Dstring& color_bar_str,
               const Dstring& scale_bar_str,
               const bool no_stage,
               const bool no_wind_barb,
               const bool is_bludge) const;

         void
         plan (const Dstring& stage_str,
               const Dstring& product_str,
               const Dstring& time_str,
               const Dstring& zoom_str,
               const Dstring& track_id_str,
               const bool track_id_initial,
               const Track::Map& track_map,
               const Tokens& annotation_tokens,
               const Dstring& format,
               const Tokens& title_tokens,
               const Dstring& filename,
               const Dstring& color_bar_str,
               const Dstring& scale_bar_str,
               const bool no_stage,
               const bool no_wind_barb,
               const bool is_bludge) const;

         void
         cross_section (const Dstring& stage_str,
                        const Dstring& product_str,
                        const Tokens& journey_tokens,
                        const Dstring& time_str,
                        const Real height,
                        const Dstring& format,
                        const Tokens& title_tokens,
                        const Dstring& filename,
                        const Real u_bg,
                        const bool is_bludge) const;

         void
         cross_section (const Dstring& stage_str,
                        const Dstring& product_str,
                        const Dstring& track_id_str,
                        const Track::Map& track_map,
                        const Real distance,
                        const Dstring& time_str,
                        const Real height,
                        const Dstring& format,
                        const Tokens& title_tokens,
                        const Dstring& filename,
                        const bool lagrangian,
                        const bool is_bludge) const;

         void
         time_cross (const Dstring& stage_str,
                     const Dstring& product_str,
                     const Real azimuth,
                     const Dstring& location_str,
                     const Dstring& time_str,
                     const Real height,
                     const Dstring& format,
                     const Tokens& title_tokens,
                     const Dstring& filename,
                     const bool is_bludge) const;

         void
         time_cross (const Dstring& stage_str,
                     const Dstring& product_str,
                     const Dstring& track_id_str,
                     const Track::Map& track_map,
                     const Real height,
                     const Dstring& format,
                     const Tokens& title_tokens,
                     const Dstring& filename,
                     const bool lagrangian,
                     const bool is_bludge) const;

         void
         meteogram (const Dstring& stage_str,
                    const Dstring& location_str,
                    const Dstring& time_str,
                    const Dstring& format,
                    const Tokens& title_tokens,
                    const Dstring& filename,
                    const bool ignore_pressure,
                    const bool no_nwp,
                    const bool is_bludge) const;

         void
         vertical_profile (const Dstring& stage_str,
                           const Dstring& location_str,
                           const Dstring& time_str,
                           const Dstring& format,
                           const Tokens& title_tokens,
                           const Dstring& filename,
                           const bool is_bludge) const;

         void
         vertical_profile (const Dstring& stage_str,
                           const Dstring& track_id_str,
                           const Track::Map& track_map,
                           const Dstring& time_str,
                           const Dstring& format,
                           const Tokens& title_tokens,
                           const Dstring& filename,
                           const bool is_bludge) const;

         class Andrea : public andrea::Andrea
         {

            private:

               const Config_File
               config_file;

               Track::Map
               trajectory_map;

               void
               twiin_trajectory_generate (const Dstring& identifier,
                                          const Dstring& stage_str,
                                          const Lat_Long& lat_long,
                                          const Level& level,
                                          const Dtime& dtime,
                                          const Real finish_tau,
                                          const Tokens& arguments);

               void
               twiin_trajectory_survey (const Dstring& identifier,
                                        const Dstring& stage_str,
                                        const Tokens& arguments);

               void
               twiin_trajectory_ingest (const Dstring& file_path);

               void
               twiin_trajectory_write (const Dstring& file_path) const;

               void
               twiin_trajectory_print (const Dstring& identifier) const;

               void
               twiin_trajectory_surface (const Dstring& surface_identifier,
                                         const Dstring& geodetic_transform_identifier,
                                         const Dstring& identifier,
                                         const Dtime& dtime) const;

               void
               twiin_trajectory (const Tokens& tokens);

               void
               twiin_surface_plan (const Dstring& surface_identifier,
                                   const Dstring& geodetic_transform_identifier,
                                   const Dstring& stage,
                                   const Model::Product& product,
                                   const Level& level,
                                   const Dtime& dtime,
                                   const Tokens& arguments);

               void
               twiin_surface (const Tokens& tokens);

               void
               twiin_time_series (const Tokens& tokens);

               void
               twiin_time_series_aws (const Tokens& tokens);

               void
               twiin_parse (const Tokens& tokens);

               void
               parse (const Tokens& tokens);

            public:

               Andrea (const Config_File& config_file);

         };

      public:

         static void
         usage ();

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
                    const Dtime& dtime,
                    const Level& level);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
                    const Dtime& dtime,
                    const Journey& journey);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
                    const Dtime& dtime,
                    const Lat_Long& lat_long,
                    const Real distance,
                    const bool lagrangian);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
                    const Location& location);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Model::Product& product,
                    const Dstring& track_id,
                    const bool lagrangian);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Dtime& dtime,
                    const Location& location);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Dtime& dtime,
                    const Dstring& location_str);

         static void
         set_title (Title& title,
                    const Dtime& basetime,
                    const Dstring& stage_str,
                    const Location& location);

         static void
         render_stages (const RefPtr<Context>& cr,
                        const Geodetic_Transform& transform,
                        const Data& data);

         static void
         render_product (const RefPtr<Context>& cr,
                         const Geodetic_Transform& transform,
                         const Size_2D& size_2d,
                         const Data& data,
                         const Model::Product& product,
                         const Dtime& dtime,
                         const Level& level,
                         const Dstring& stage_str);

         static void
         render_wind_barbs (const RefPtr<Context>& cr,
                            const Geodetic_Transform& transform,
                            const Size_2D& size_2d,
                            const Data& data,
                            const Dtime& dtime,
                            const Level& level,
                            const Dstring& stage_str);

         static void
         render_scale_bar (const RefPtr<Context>& cr,
                           const Geodetic_Transform& transform,
                           const Size_2D& size_2d,
                           const Dstring& scale_bar_str = "lb:10");

         static void
         render_color_bar (const RefPtr<Context>& cr,
                           const Model::Product& product,
                           const Tuple& tick_tuple,
                           const Box_2D& box_2d,
                           const bool negative = false);

         static void
         render_color_bar (const RefPtr<Context>& cr,
                           const Size_2D& size_2d,
                           const Model::Product& product,
                           const Dstring& color_bar_str = "r:10");

         static void
         render_annotation_point (const RefPtr<Context>& cr,
                                  const Geodetic_Transform& transform,
                                  const Lat_Long& lat_long,
                                  const Dstring& str,
                                  const char h_align = 'h',
                                  const char v_align = 'c');

         static void
         render_annotation (const RefPtr<Context>& cr,
                            const Geodetic_Transform& transform,
                            const Dstring& annotation_str,
                            const Data& data);

         static void
         render_annotations (const RefPtr<Context>& cr,
                             const Geodetic_Transform& transform,
                             const Tokens& annotation_tokens,
                             const Data& data);

         static void
         render (const RefPtr<Context>& cr,
                 const Geodetic_Transform& geodetic_transform,
                 const Size_2D& size_2d,
                 const Data& data,
                 const Dtime& dtime,
                 const Level& level,
                 const Dstring& stage_str,
                 const Model::Product product,
                 const bool no_stage,
                 const bool no_wind_barb);

         static void
         render_cross_section_w (const RefPtr<Context>& cr,
                                 const Transform_2D& transform,
                                 const Box_2D& box_2d,
                                 const Model::Stage& stage,
                                 const Dtime& dtime,
                                 const Journey& journey);

         static void
         render_cross_section_mesh (const RefPtr<Context>& cr,
                                    const Transform_2D& transform,
                                    const Domain_1D& domain_z,
                                    const Journey& journey);

         static void
         render_cross_section_arrows (const RefPtr<Context>& cr,
                                      const Transform_2D& transform,
                                      const Box_2D& box_2d,
                                      const Model::Stage& stage,
                                      const Model::Product& product,
                                      const Dtime& dtime,
                                      const Journey& journey,
                                      const Real u_bg = 0);

         static void
         render_cross_section (const RefPtr<Context>& cr,
                               const Transform_2D& transform,
                               const Box_2D& box_2d,
                               const Domain_1D& domain_z,
                               const Model::Stage& stage,
                               const Model::Product& product,
                               const Dtime& dtime,
                               const Journey& journey,
                               const Real u_bg = 0);

         static void
         render_time_cross_w (const RefPtr<Context>& cr,
                              const Transform_2D& transform,
                              const Box_2D& box_2d,
                              const Model::Stage& stage,
                              const Track& track);

         static void
         render_time_cross_mesh (const RefPtr<Context>& cr,
                                 const Transform_2D& transform,
                                 const Domain_1D& domain_t,
                                 const Domain_1D& domain_z);

         static void
         render_time_cross_track (const RefPtr<Context>& cr,
                                  const Transform_2D& transform,
                                  const Track& track);

         static void
         render_time_cross (const RefPtr<Context>& cr,
                            const Transform_2D& transform,
                            const Box_2D& box_2d,
                            const Domain_1D& domain_z,
                            const Model::Stage& stage,
                            const Model::Product& product,
                            const Location& location,
                            const Dtime::Span& time_span,
                            const Real azimuth);

         static void
         render_time_cross (const RefPtr<Context>& cr,
                            const Transform_2D& transform,
                            const Box_2D& box_2d,
                            const Domain_1D& domain_z,
                            const Model::Stage& stage,
                            const Model::Product& product,
                            const Track& track,
                            const bool lagrangian = false);

         static void
         render_meteogram_mesh (const RefPtr<Context>& cr,
                                const Domain_1D& domaint_t,
                                const Domain_1D& domain_temperature,
                                const Domain_1D& domain_direction,
                                const Domain_1D& domain_speed,
                                const Domain_1D& domain_pressure,
                                const Transform_2D& transform_temperature,
                                const Transform_2D& transform_direction,
                                const Transform_2D& transform_speed,
                                const Transform_2D& transform_pressure,
                                const bool ignore_pressure);

         static void
         render_meteogram (const RefPtr<Context>& cr,
                           const Transform_2D& transform_temperature,
                           const Transform_2D& transform_direction,
                           const Transform_2D& transform_speed,
                           const Transform_2D& transform_pressure,
                           const Aws::Repository& aws_repository,
                           const Real alpha,
                           const Real ring_size,
                           const bool fill,
                           const bool ignore_pressure);

         static void
         render_meteogram (const RefPtr<Context>& cr,
                           const Size_2D& size_2d,
                           const Model::Stage& stage,
                           const Aws::Repository& aws_repository,
                           const Location& location,
                           const Dstring& time_str,
                           const bool ignore_pressure,
                           const bool no_nwp);

         static void
         render_vertical_profile (const RefPtr<Context>& cr,
                                  const Thermo_Diagram& thermo_diagram,
                                  const Model::Stage& stage,
                                  const Dtime& dtime,
                                  const Lat_Long& lat_long,
                                  const Level& level = Level (Level::NAL));

         static void
         render_vertical_profile (const RefPtr<Context>& cr,
                                  const Thermo_Diagram& thermo_diagram,
                                  const Model::Stage& stage,
                                  const Dtime& dtime,
                                  const Lat_Long::List& lat_long_list);

#ifndef ENABLE_GTKMM
#else  /* ENABLE_GTKMM */
         class Gui : public Map_Console,
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

                     Gui&
                     gui;

                     Signal
                     signal;

                     void
                     emit (const Model::Product& product);

                  public:

                     Product_Panel (Gui& gui,
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

               Gui (Gtk::Window& gtk_window,
                        const Size_2D& size_2d,
                        const Config_File& config_file,
                        const Data& data,
                        const Dstring& stage_str,
                        const Model::Product& product,
                        const Level& level,
                        const Dtime& dtime,
                        const Tokens& journey_tokens,
                        const Dstring& zoom_str = "");

               ~Gui ();

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
#endif /* ENABLE_GTKMM */

   };

};

#endif /* TWIIN_H */
