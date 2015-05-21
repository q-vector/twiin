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

   class Terrain
   {

      private:

         class Stage : public twiin::Stage
         {

            private:

               Nc_File
               orog_file;

               Nc_File
               lsm_file;

               const Tuple
               tuple_latitude;

               const Tuple
               tuple_longitude;

               int
               orog_varid;

               int
               lsm_varid;

               void
               acquire_ij (size_t& i,
                           size_t& j,
                           const Real latitude,
                           const Real longitude) const;

            public:

               Stage (const twiin::Stage& stage,
                      const string& orog_file_path,
                      const string& lsm_file_path);

               ~Stage ();

               bool
               out_of_bounds (const Real latitude,
                              const Real longitude) const;

               Real
               get_orog (const size_t& i,
                         const size_t& j) const;
               Real
               get_lsm (const size_t& i,
                        const size_t& j) const;

               Real
               get_orog (const Real latitude,
                         const Real longitude) const;

               Real
               get_lsm (const Real latitude,
                        const Real longitude) const;

               void
               acquire_orog_lsm (Real& orog,
                                 Real& lsm,
                                 const Real latitude,
                                 const Real longitude) const;

         };

         const Stage
         stage_3;

         const Stage
         stage_4;

         const Stage
         stage_5;

         const Stage&
         get_terrain_stage (const twiin::Stage& stage) const;

      public:

         Terrain (const string& orog_3_file_path,
                  const string& lsm_3_file_path,
                  const string& orog_4_file_path,
                  const string& lsm_4_file_path,
                  const string& orog_5_file_path,
                  const string& lsm_5_file_path);

         Raster*
         get_raster_ptr (const Size_2D& size_2d,
                         const Transform_2D& transform,
                         const twiin::Stage& stage) const;

         void
         cairo (const RefPtr<Context>& cr,
                const Transform_2D& transform,
                const Size_2D& size_2d,
                const twiin::Stage& stage) const;

   };

   class Model
   {

      public:

         class Stage : public twiin::Stage
         {

            private:

               Tuple
               tuple_latitude;

               Tuple
               tuple_longitude;

               set<Dtime>
               valid_time_set;

               map<Nwp_Element, Nc_File*>
               nc_file_ptr_map;

               map<Nwp_Element, Integer>
               varid_map;

               map<Nwp_Element, string>
               varname_map;

               void
               fill_valid_time_set ();

            public:

               Stage (const twiin::Stage& stage,
                      const map<Nwp_Element, string>& file_path_map);

               ~Stage ();

               const set<Dtime>&
               get_valid_time_set () const;

               Integer
               get_l (const Dtime& dtime) const;

               bool
               out_of_bounds (const Real latitude,
                              const Real longitude) const;

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

               Color
               get_color (const Product& product,
                          const Lat_Long& lat_long,
                          const Real z,
                          const Integer l) const;

         };

      public:

         const Stage
         stage_3;

         const Stage
         stage_4;

         const Stage
         stage_5;

         const Stage&
         get_model_stage (const twiin::Stage& stage) const;

         Model (const map<Nwp_Element, string>& file_path_3_map,
                const map<Nwp_Element, string>& file_path_4_map,
                const map<Nwp_Element, string>& file_path_5_map);

         ~Model ();

         Raster*
         get_raster_ptr (const Product& product,
                         const Dtime& dtime,
                         const Size_2D& size_2d,
                         const Transform_2D& transform,
                         const twiin::Stage& stage) const;

         Real
         evaluate (const Nwp_Element& nwp_element,
                   const Real latitude,
                   const Real longitude,
                   const size_t k,
                   const Dtime& dtim,
                   const twiin::Stage& stage) const;

   };

};

#endif /* DATA_H */
