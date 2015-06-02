#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <map>
#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <denise/geodesy.h>
#include <denise/nwp.h>
#include "nc_file.h"

using namespace std;
using namespace denise;
using namespace Cairo;

namespace twiin
{

   class Stage : public string
   {

      public:

         Stage (const string& str);

         Stage (const Stage& stage);

         string
         get_string () const;

   };

   class Product : public string
   {

      public:

         Product (const string& str);

         Product (const Product& product);

         string
         get_string () const;

   };

   class Ffdi_Color_Chooser : public Color_Chooser
   {

      private:

         Real
         alpha;

      public:

         Ffdi_Color_Chooser (const Real alpha = 1.0);

         Color
         get_color (const Real ffdi) const;

   };

   class Station : public Lat_Long
   {

      public:

         const Integer
         id;

         const string
         name;

         const Real
         height;

         Station (const Integer id,
                  const Real latitude,
                  const Real longitude,
                  const Real height,
                  const string& name);

         class Map : public map<Integer, Station>
         {

            public:

               const Domain_2D
               stage_3;

               const Domain_2D
               stage_4;

               const Domain_2D
               stage_5;

               Map (const string& file_path);

               void
               cairo (const RefPtr<Context>& cr,
                      const Transform_2D& transform) const;

               void
               render_stages (const RefPtr<Context>& cr,
                              const Transform_2D& transform) const;

         };

   };

   class Model
   {

      public:

         class Varname : public string
         {

            public:

               Varname (const string& str);

               Varname (const Varname& varname);

               string
               get_string () const;

         };

         class File_Path_Map : public map<Varname, string>
         {

            public:

               void
               insert (const Varname& varname,
                       const string& file_path);

         };

         class Terrain
         {

            public:

               class Stage : public twiin::Stage
               {

                  protected:

                     const Model&
                     model;

                     Tuple
                     tuple_latitude;

                     Tuple
                     tuple_longitude;

                     map<Varname, Nc_File*>
                     nc_file_ptr_map;

                     map<Varname, Integer>
                     varid_map;

                     void
                     acquire_ij (size_t& i,
                                 size_t& j,
                                 const Real latitude,
                                 const Real longitude) const;

                  public:

                     Stage (const Stage& stage);

                     Stage (const Model& model,
                            const twiin::Stage& stage);

                     Stage (const Model& model,
                            const twiin::Stage& stage,
                            const File_Path_Map& file_path_map);

                     ~Stage ();

                     virtual void
                     init (const File_Path_Map& file_path_map);

                     bool
                     out_of_bounds (const Real latitude,
                                    const Real longitude) const;

                     Real
                     evaluate (const Varname& varname,
                               const size_t& i,
                               const size_t& j) const;

                     Real
                     evaluate (const Varname& varname, 
                               const Real latitude,
                               const Real longitude) const;

                     Raster*
                     get_raster_ptr (const Size_2D& size_2d,
                                     const Transform_2D& transform) const;

               };

               const Model&
               model;

               map<twiin::Stage, Terrain::Stage>
               stage_map;

               virtual const Stage&
               get_stage (const twiin::Stage& stage) const;

               virtual Stage&
               get_stage (const twiin::Stage& stage);

            public:

               Terrain (const Model& model);

               virtual void
               init (const Tokens& stage_tokens);

               virtual void
               init2 (const twiin::Stage& twiin_stage,
                      const Model::File_Path_Map& file_path_map);

         };

         class Surface : public Terrain
         {

            public:

               class Stage : public Terrain::Stage
               {

                  protected:

                     set<Dtime>
                     valid_time_set;

                     void
                     fill_valid_time_set ();

                  public:

                     Stage (const Model& model,
                            const twiin::Stage& stage);

                     virtual void
                     init (const File_Path_Map& file_path_map);

                     const set<Dtime>&
                     get_valid_time_set () const;

                     size_t
                     get_l (const Dtime& dtime) const;

                     Real
                     evaluate (const Nwp_Element& nwp_element,
                               const Real latitude,
                               const Real longitude,
                               const size_t l) const;
                               
                     Real
                     evaluate (const Nwp_Element& nwp_element,
                               const size_t i,
                               const size_t j,
                               const size_t l) const;

                     Real
                     evaluate_raw (const string& varname,
                                   const size_t i,
                                   const size_t j,
                                   const size_t l) const;

                     Raster*
                     get_raster_ptr (const Size_2D& size_2d,
                                     const Transform_2D& transform,
                                     const Product& product,
                                     const Dtime& dtime) const;

                     Color
                     get_color (const Product& product,
                                const Lat_Long& lat_long,
                                const size_t l) const;


               };

               map<twiin::Stage, Surface::Stage>
               stage_map;

               virtual const Stage&
               get_stage (const twiin::Stage& stage) const;

               virtual Stage&
               get_stage (const twiin::Stage& stage);

            public:

               Surface (const Model& model);

               virtual void
               init (const Tokens& stage_tokens);

               virtual void
               init2 (const twiin::Stage& twiin_stage,
                      const Model::File_Path_Map& file_path_map);

         };

         class Uppers : public Surface
         {

            public:

               class Stage : public Surface::Stage
               {

                  public:

                     Stage (const Model& model,
                            const twiin::Stage& stage);

                     Real
                     evaluate (const Nwp_Element& nwp_element,
                               const Real latitude,
                               const Real longitude,
                               const Real z,
                               const size_t l) const;
                               
                     Real
                     evaluate (const Nwp_Element& nwp_element,
                               const Real latitude,
                               const Real longitude,
                               const size_t k,
                               const size_t l) const;

                     Real
                     evaluate (const Nwp_Element& nwp_element,
                               const size_t i,
                               const size_t j,
                               const size_t k,
                               const size_t l) const;

                     Real
                     evaluate_raw (const string& varname,
                                   const size_t i,
                                   const size_t j,
                                   const size_t k,
                                   const size_t l) const;

                     Raster*
                     get_raster_ptr (const Size_2D& size_2d,
                                     const Transform_2D& transform,
                                     const Product& product,
                                     const Dtime& dtime,
                                     const Level& level) const;

                     Color
                     get_color (const Product& product,
                                const Lat_Long& lat_long,
                                const Level& level,
                                const size_t l) const;


               };

               map<twiin::Stage, Uppers::Stage>
               stage_map;

               virtual const Stage&
               get_stage (const twiin::Stage& stage) const;

               virtual Stage&
               get_stage (const twiin::Stage& stage);

            public:

               Uppers (const Model& model);

               virtual void
               init (const Tokens& stage_tokens);

               virtual void
               init2 (const twiin::Stage& twiin_stage,
                      const Model::File_Path_Map& file_path_map);

         };

         class Vertical_Coefficients
         {

            private:

               Tuple
               A_theta;

               Tuple
               B_theta;

               Tuple
               A_rho;

               Tuple
               B_rho;

            public:

               Vertical_Coefficients ();

               Vertical_Coefficients (const string& file_path);

               void
               init (const string& file_path);

               const Tuple&
               get_A_theta () const;

               const Tuple&
               get_B_theta () const;

               const Tuple&
               get_A_rho () const;

               const Tuple&
               get_B_rho () const;

         };

      public:

         Terrain
         terrain;

         Uppers
         uppers;

         Surface
         surface;

         Vertical_Coefficients
         vertical_coefficients;

         static string
         get_nc_varname (const Varname& varname);

         const Real
         get_z (const Integer k,
                const Real topography,
                const Tuple& A,
                const Tuple& B) const;

         const Integer
         get_k (const Real z,
                const Real topography,
                const Tuple& A,
                const Tuple& B,
                Integer start_k = -1,
                Integer end_k = -1) const;

         Model (const string& model_config_file_path);

         ~Model ();

         bool
         out_of_bounds (const Real latitude,
                        const Real longitude,
                        const twiin::Stage& stage) const;

         Real
         evaluate (const Nwp_Element& nwp_element,
                   const Real latitude,
                   const Real longitude,
                   const size_t k,
                   const Dtime& dtime,
                   const twiin::Stage& stage) const;

         Real
         evaluate (const Nwp_Element& nwp_element,
                   const Real latitude,
                   const Real longitude,
                   const Real z,
                   const Dtime& dtime,
                   const twiin::Stage& stage) const;

         const set<Dtime>&
         get_valid_time_set (const Product& product,
                             const twiin::Stage& stage,
                             const Level& level) const;

   };

};

#endif /* DATA_H */
