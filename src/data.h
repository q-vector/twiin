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
#include "hrit.h"

using namespace std;
using namespace denise;
using namespace Cairo;

namespace twiin
{

   class Stage : public Dstring
   {

      public:

         Stage (const Dstring& str);

         Stage (const Stage& stage);

         Dstring
         get_string () const;

   };

   class Product
   {

      public:

         enum Enumeration
         {
            NIL,
            TERRAIN,
            TERRAIN_WATER,
            P_THETA,
            P_RHO,
            ALONG_SPEED,
            NORMAL_SPEED,
            BRUNT_VAISALA,
            SCORER,
            T,
            TD,
            RH,
            THETA,
            THETA_V,
            Q,
            THETA_E,
            RHO,
            WIND,
            SPEED,
            SPEED_HIGHER,
            VORTICITY,
            W,
            W_TRANSLUCENT,
	    Q_LOCAL_CHANGE,
	    Q_ADVECTION,
	    Q_H_ADVECTION,
            Q_V_ADVECTION,
            Q_S_ADVECTION,
            Q_N_ADVECTION,
            Q_SV_ADVECTION,
            Q_NV_ADVECTION,
            FFDI,
            MSLP,
            PRECIP_RATE,
            IR1,
            IR2,
            IR3,
            IR4,
            VIS,
            Pseudo
         };

         Product::Enumeration
         enumeration;

         Product (const Enumeration& enumeration);

         Product (const Product& product);

         Product (const Dstring& str);

         Dstring
         get_string () const;

         Dstring
         get_description () const;

         Met_Element
         get_met_element () const;

         Dstring
         get_unit () const;

         ostream&
         operator<< (ostream& out_file) const;

   };

   class Station : public Lat_Long
   {

      public:

         const Integer
         id;

         const Dstring
         name;

         const Real
         height;

         Station (const Integer id,
                  const Real latitude,
                  const Real longitude,
                  const Real height,
                  const Dstring& name);

         class Map : public map<Integer, Station>,
                     public Attractor,
                     public Geodetic_Attractor
         {

            public:

               Map ();

               Map (const Dstring& file_path);

               void
               ingest (const Dstring& file_path);

               const Station&
               get_station (const Lat_Long& lat_long,
                            const Real tolerance = 1) const;

               const Station&
               get_nearest_station (const Lat_Long& lat_long) const;

               void
               attract (Real& latitude,
                        Real& longitude) const;

               pair<Dstring, Lat_Long>
               nearest (const Lat_Long& lat_long) const;

               void
               cairo (const RefPtr<Context>& cr,
                      const Transform_2D& transform,
                      const Color& color = Color::green ()) const;

         };

   };

   class Location : public Lat_Long
   {

      private:

         Integer
         station_id;

         Dstring
         str;

         Dstring
         long_str;

      public:

         Location (const Lat_Long& lat_long);

         Location (const Dstring& location_str,
                   const Station::Map& station_map);

         Integer
         get_station_id () const;

         const Dstring&
         get_str () const;

         const Dstring&
         get_long_str () const;

   };

   class Aws
   {

      public:

         class Obs
         {

            public:

               const Real
               temperature;

               const Real
               dew_point;

               const Real
               wind_direction;

               const Real
               wind_speed;

               const Real
               wind_gust;

               const Real
               station_p;

               const Real
               mslp;

               Obs (const Real temperature,
                    const Real dew_point,
                    const Real wind_direction,
                    const Real wind_speed,
                    const Real station_p,
                    const Real mslp,
                    const Real wind_gust = GSL_NAN);

               Obs (const Obs& obs);

         };

         class Key
         {

            public:

               const Integer
               station_id;

               const Dtime
               dtime;

               Key (const Integer station_id,
                    const Dtime& dtime);

               Key (const Key& key);

               bool
               operator == (const Key& key) const;

               bool
               operator > (const Key& key) const;

               bool
               operator < (const Key& key) const;

         };

         class Repository : public map<Key, Obs>
         {

            private:

               set<Integer>
               station_id_set;

               set<Dtime>
               valid_time_set;

               static Real
               to_real (const Dstring& token);

            public:

               Repository ();

               Repository (const Dstring& file_path);

               void
               insert (const Key& key,
                       const Obs& obs);

               void
               ingest (const Dstring& file_path);

               void
               ingest_binary (const Dstring& file_path);

               void
               write (const Dstring& file_path) const;

               void
               write_binary (const Dstring& file_path) const;

               const set<Integer>&
               get_station_id_set () const;

               const set<Dtime>&
               get_valid_time_set () const;

               const Aws::Repository*
               get_aws_repository_ptr (const Integer station_id,
                                       const Dtime::Span& time_span) const;

               Domain_1D
               get_temperature_domain () const;

               Domain_1D
               get_dew_point_domain () const;

               Domain_1D
               get_wind_speed_domain () const;

               Domain_1D
               get_station_p_domain () const;

               Domain_1D
               get_mslp_domain () const;

         };

   };

   class Model
   {

      private:

         class Varname : public Dstring
         {

            public:

               Varname (const Dstring& str);

               Varname (const Varname& varname);

               Dstring
               get_string () const;

         };

         class File_Path_Map : public map<Varname, Dstring>
         {

            public:

               void
               insert (const Varname& varname,
                       const Dstring& file_path);

         };

      public:

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
                                 const Lat_Long& lat_long) const;

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

                     Domain_2D
                     get_domain_2d () const;

                     bool
                     out_of_bounds (const Lat_Long& lat_long) const;

                     pair<Real, Real>
                     get_grid_size (const size_t& i,
                                    const size_t& j) const;

                     pair<Real, Real>
                     get_grid_size (const Lat_Long& lat_long) const;

                     Real
                     evaluate (const Varname& varname,
                               const size_t& i,
                               const size_t& j) const;

                     Real
                     evaluate (const Varname& varname, 
                               const Lat_Long& lat_long) const;

                     Real
                     get_topography (const Lat_Long& lat_long) const;

                     Real
                     get_topography (const size_t i,
                                     const size_t j) const;


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
               init_stage (const twiin::Stage& twiin_stage,
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

                     vector<Dtime>
                     valid_time_vector;

                     void
                     fill_valid_time_set ();

                  public:

                     Stage (const Model& model,
                            const twiin::Stage& stage);

                     virtual void
                     init (const File_Path_Map& file_path_map);

                     const set<Dtime>&
                     get_valid_time_set () const;

                     const vector<Dtime>&
                     get_valid_time_vector () const;

                     size_t
                     get_l (const Dtime& dtime) const;

                     Real
                     evaluate_h_advection (const Met_Element& met_element,
                                           const Lat_Long& lat_long,
                                           const size_t l) const;

                     Real
                     evaluate (const Met_Element& met_element,
                               const Lat_Long& lat_long,
                               const size_t l) const;
                               
                     Real
                     evaluate (const Met_Element& met_element,
                               const size_t i,
                               const size_t j,
                               const size_t l) const;

                     Real
                     evaluate_dt (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const size_t l) const;

                     Real
                     evaluate_dt (const Met_Element& met_element,
                                  const size_t i,
                                  const size_t j,
                                  const size_t l) const;

                     Real
                     evaluate_dx (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const size_t l) const;

                     Real
                     evaluate_dx (const Met_Element& met_element,
                                  const size_t i,
                                  const size_t j,
                                  const size_t l) const;

                     Real
                     evaluate_dy (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const size_t l) const;

                     Real
                     evaluate_dy (const Met_Element& met_element,
                                  const size_t i,
                                  const size_t j,
                                  const size_t l) const;

                     Real
                     evaluate_raw (const Dstring& varname,
                                   const size_t i,
                                   const size_t j,
                                   const size_t l) const;

                     Aws::Obs
                     get_aws_obs (const Lat_Long& lat_long,
                                  const Dtime& dtime) const;

                     void
                     fill_sounding (Sounding& sounding,
                                    const Lat_Long& lat_long,
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
               init_stage (const twiin::Stage& twiin_stage,
                           const Model::File_Path_Map& file_path_map);

         };

         class Uppers : public Surface
         {

            public:

               class Stage : public Surface::Stage
               {

                  protected:

                     void
                     acquire_k (size_t& k,
                                const Met_Element& met_element,
                                const size_t i,
                                const size_t j,
                                const Level& level) const;


                  public:

                     Stage (const Model& model,
                            const twiin::Stage& stage);

                     Real
                     evaluate_normal_speed (const Real azimuth,
                                            const Lat_Long& lat_long,
                                            const Level& level,
                                            const size_t l) const;
                               
                     Real
                     evaluate_along_speed (const Real azimuth,
                                           const Lat_Long& lat_long,
                                           const Level& level,
                                           const size_t l,
                                           const Real u_bg) const;
                               
                     Real
                     evaluate_scorer (const Real azimuth,
                                      const Lat_Long& lat_long,
                                      const Level& level,
                                      const size_t l,
                                      const Real u_bg = 0) const;

                     Real
                     evaluate_brunt_vaisala (const Lat_Long& lat_long,
                                             const Level& level,
                                             const size_t l) const;

                     Real
                     evaluate_h_advection (const Met_Element& met_element,
                                           const Lat_Long& lat_long,
                                           const Level& level,
                                           const size_t l) const;

                     Real
                     evaluate_v_advection (const Met_Element& met_element,
                                           const Lat_Long& lat_long,
                                           const Level& level,
                                           const size_t l) const;

                     Real
                     evaluate_s_advection (const Met_Element& met_element,
                                           const Real azimuth,
                                           const Lat_Long& lat_long,
                                           const Level& level,
                                           const size_t l,
                                           const Real u_bg = 0) const;

                     Real
                     evaluate_n_advection (const Met_Element& met_element,
                                           const Real azimuth,
                                           const Lat_Long& lat_long,
                                           const Level& level,
                                           const size_t l) const;

                     Real
                     evaluate (const Met_Element& met_element,
                               const Lat_Long& lat_long,
                               const Level& level,
                               const size_t l) const;
                               
                     Real
                     evaluate (const Met_Element& met_element,
                               const Lat_Long& lat_long,
                               const size_t k,
                               const size_t l) const;

                     Real
                     evaluate (const Met_Element& met_element,
                               const size_t i,
                               const size_t j,
                               const size_t k,
                               const size_t l) const;

                     Real
                     evaluate_dt (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const Level& level,
                                  const size_t l) const;

                     Real
                     evaluate_dt (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const size_t k,
                                  const size_t l) const;

                     Real
                     evaluate_dt (const Met_Element& met_element,
                                  const size_t i,
                                  const size_t j,
                                  const size_t k,
                                  const size_t l) const;

                     Real
                     evaluate_dx (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const Level& level,
                                  const size_t l) const;

                     Real
                     evaluate_dx (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const size_t k,
                                  const size_t l) const;

                     Real
                     evaluate_dx (const Met_Element& met_element,
                                  const size_t i,
                                  const size_t j,
                                  const size_t k,
                                  const size_t l) const;

                     Real
                     evaluate_dy (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const Level& level,
                                  const size_t l) const;

                     Real
                     evaluate_dy (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const size_t k,
                                  const size_t l) const;

                     Real
                     evaluate_dy (const Met_Element& met_element,
                                  const size_t i,
                                  const size_t j,
                                  const size_t k,
                                  const size_t l) const;

                     Real
                     evaluate_dz (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const Level& level,
                                  const size_t l) const;

                     Real
                     evaluate_dz (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const size_t k,
                                  const size_t l) const;

                     Real
                     evaluate_dz (const Met_Element& met_element,
                                  const size_t i,
                                  const size_t j,
                                  const size_t k,
                                  const size_t l) const;

                     Real
                     evaluate_raw (const Dstring& varname,
                                   const size_t i,
                                   const size_t j,
                                   const size_t k,
                                   const size_t l) const;

                     void
                     fill_sounding (Sounding& sounding,
                                    const Lat_Long& lat_long,
                                    const Dtime& dtime) const;

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
               init_stage (const twiin::Stage& twiin_stage,
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

               Vertical_Coefficients (const Dstring& file_path);

               void
               init (const Dstring& file_path);

               const Tuple&
               get_A_theta () const;

               const Tuple&
               get_B_theta () const;

               const Tuple&
               get_A_rho () const;

               const Tuple&
               get_B_rho () const;

               const Tuple&
               get_A (const bool is_theta) const;

               const Tuple&
               get_B (const bool is_theta) const;

               const Tuple&
               get_A (const Met_Element& met_element) const;

               const Tuple&
               get_B (const Met_Element& met_element) const;

         };

      private:

         Dtime
         basetime;

         Vertical_Coefficients
         vertical_coefficients;

         static Dstring
         get_nc_varname (const Varname& varname);

         const Real
         get_z (const Integer k,
                const Real topography,
                const Tuple& A,
                const Tuple& B) const;

         const size_t
         get_k (const Real z,
                const Real topography,
                const Tuple& A,
                const Tuple& B,
                Integer start_k = -1,
                Integer end_k = -1) const;

      public:

         Terrain
         terrain;

         Surface
         surface;

         Uppers
         uppers;

         Model (const Config_File& config_file);

         ~Model ();

         static bool
         is_theta (const Met_Element& met_element);

         const Dtime&
         get_basetime () const;

         Domain_2D
         get_domain_2d (const twiin::Stage& stage) const;

         bool
         out_of_bounds (const Lat_Long& lat_long,
                        const twiin::Stage& stage) const;

         Real
         get_topography (const Lat_Long& lat_long,
                         const twiin::Stage& stage) const;

         Real
         evaluate (const Met_Element& met_element,
                   const Lat_Long& lat_long,
                   const size_t k,
                   const Dtime& dtime,
                   const twiin::Stage& stage) const;

         Real
         evaluate (const Met_Element& met_element,
                   const Lat_Long& lat_long,
                   const Level& level,
                   const Dtime& dtime,
                   const twiin::Stage& stage) const;

         Track
         get_trajectory (Lat_Long lat_long,
                         Level level,
                         Dtime dtime,
                         const twiin::Stage& stage,
                         const Real finish_tau,
                         const vector<Product>& product_vector = vector<Product> ()) const;

         Aws::Obs
         get_aws_obs (const Lat_Long& lat_long,
                      const Dtime& dtime,
                      const twiin::Stage& stage) const;

         const Aws::Repository*
         get_aws_repository_ptr (const Lat_Long& lat_long,
                                 const twiin::Stage& stage) const;

         Sounding*
         get_sounding_ptr (const Lat_Long& lat_long,
                           const Dtime& dtime,
                           const twiin::Stage& stage) const;

         Sounding*
         get_sounding_ptr (const Lat_Long::List& lat_long_list,
                           const Dtime& dtime,
                           const twiin::Stage& stage,
                           const Thermo_Diagram& thermo_diagram = Tephigram (Size_2D (1000, 1000))) const;

         const set<Dtime>&
         get_valid_time_set (const Product& product,
                             const twiin::Stage& stage,
                             const Level& level) const;

         vector<Dtime>
         get_valid_time_vector (const Product& product,
                                const twiin::Stage& stage,
                                const Level& level,
                                const Dtime::Set& time_set) const;

         Tokens
         get_marker_tokens (const Lat_Long& lat_long,
                            const Dtime& dtime,
                            const Product& product,
                            const twiin::Stage& stage,
                            const Level& level) const;

   };

   class Data
   {

      private:

         Model
         model;

         Hrit
         hrit;

         Station::Map
         station_map;

         Aws::Repository
         aws_repository;

      public:

         Data (const Config_File& config_file);

         const Model&
         get_model () const;

         const Hrit&
         get_hrit () const;

         const Station::Map&
         get_station_map () const;

         const Aws::Repository&
         get_aws_repository () const;

         const Aws::Repository*
         get_aws_repository_ptr (const Integer station_id,
                                 const Dtime::Span& time_Span) const;

         Lat_Long
         get_lat_long (const Dstring& location_str) const;

   };

};

#endif /* DATA_H */
