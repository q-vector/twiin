#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <map>
#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <denise/geodesy.h>
#include <denise/nwp.h>
#include "nc_file.h"
#include "obs.h"

using namespace std;
using namespace denise;
using namespace Cairo;

namespace twiin
{

   class Model
   {

      public:

         class Product
         {

            public:

               enum Enumeration
               {
                  NIL,
                  TERRAIN,
                  TERRAIN_MONOCHROME,
                  TERRAIN_WATER,
                  P_THETA,
                  P_RHO,
                  ALONG_SPEED,
                  NORMAL_SPEED,
                  BRUNT_VAISALA,
                  RICHARDSON,
                  SCORER,
                  SCORER_A,
                  SCORER_B,
                  T,
                  TD,
                  RH,
                  THETA,
                  THETA_MONOCHROME,
                  THETA_28C,
                  THETA_V,
                  Q,
                  Q_MONOCHROME,
                  Q_3GKG,
                  THETA_E,
                  RHO,
                  WIND,
                  SPEED,
                  SPEED_HIGHER,
                  VORTICITY,
                  W,
                  W_TRANSLUCENT,
                  Q_TENDENCY,
                  Q_L_TENDENCY,
                  Q_ADVECTION,
                  Q_H_ADVECTION,
                  Q_V_ADVECTION,
                  Q_S_ADVECTION,
                  Q_N_ADVECTION,
                  Q_SV_ADVECTION,
                  Q_NV_ADVECTION,
                  THETA_TENDENCY,
                  THETA_L_TENDENCY,
                  THETA_ADVECTION,
                  THETA_H_ADVECTION,
                  THETA_V_ADVECTION,
                  THETA_S_ADVECTION,
                  THETA_N_ADVECTION,
                  THETA_SV_ADVECTION,
                  THETA_NV_ADVECTION,
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

               Tuple
               get_tick_tuple () const;

               ostream&
               operator<< (ostream& out_file) const;

         };

         class Stage
         {

            public:

               virtual const Dtime&
               get_basetime () const = 0;

               virtual Domain_2D
               get_domain_2d () const = 0;

               virtual bool
               out_of_bounds (const Lat_Long& lat_long) const = 0;

               virtual const set<Dtime>&
               get_valid_time_set (const Level& level) const = 0;

               virtual const set<Dtime>&
               get_valid_time_set (const Product& product,
                                   const Level& level) const = 0;

               virtual vector<Dtime>
               get_valid_time_vector (const Level& level,
                                      const Dtime::Set& time_set) const = 0;

               virtual vector<Dtime>
               get_valid_time_vector (const Product& product,
                                      const Level& level,
                                      const Dtime::Set& time_set) const = 0;

               virtual Real
               get_topography (const Lat_Long& lat_long) const = 0;

               virtual Real
               get_topography (const size_t i,
                               const size_t j) const = 0;

               virtual Color
               get_color (const Product& product,
                          const Lat_Long& lat_long,
                          const size_t l) const = 0;

               virtual Real
               get_value (const Product& product,
                          const Lat_Long& lat_long,
                          const size_t l) const = 0;

               virtual Color
               get_color (const Product& product,
                          const Lat_Long& lat_long,
                          const Level& level,
                          const size_t l) const = 0;

               virtual Real
               get_value (const Product& product,
                          const Lat_Long& lat_long,
                          const Level& level,
                          const size_t l) const = 0;

               virtual size_t
               get_surface_l (const Dtime& dtime) const = 0;

               virtual size_t
               get_uppers_l (const Dtime& dtime) const = 0;

               virtual Real
               evaluate (const Met_Element& met_element,
                         const Lat_Long& lat_long,
                         const Level& level,
                         const Dtime& dtime) const = 0;

               virtual Raster*
               get_geo_raster_ptr (const Size_2D& size_2d,
                                   const Transform_2D& transform,
                                   const Product& product = Product ("TERRAIN"),
                                   const Dtime& dtime = Dtime (),
                                   const Level& level = Level ()) const = 0;

               virtual Scalar_Data_2D*
               get_geo_scalar_data_2d_ptr (const Size_2D& size_2d,
                                           const Transform_2D& transform,
                                           const Product& product,
                                           const Dtime& dtime = Dtime (),
                                           const Level& level = Level ()) const = 0;

               virtual Raster*
               get_cross_section_raster_ptr (const Box_2D& box_2d,
                                             const Transform_2D& transform,
                                             const Product& product,
                                             const Dtime& dtime,
                                             const Journey& journey,
                                             const Real u_bg = 0) const = 0;

               virtual Scalar_Data_2D*
               get_cross_section_scalar_data_2d_ptr (const Box_2D& box_2d,
                                                     const Transform_2D& transform,
                                                     const Product& product,
                                                     const Dtime& dtime,
                                                     const Journey& journey,
                                                     const Real u_bg = 0) const = 0;

               virtual Raster*
               get_time_cross_raster_ptr (const Box_2D& box_2d,
                                          const Transform_2D& transform,
                                          const Model::Product& product,
                                          const Location& location,
                                          const Dtime::Span& time_span,
                                          const Real azimuth = GSL_NAN) const = 0;

               virtual Raster*
               get_time_cross_raster_ptr (const Box_2D& box_2d,
                                          const Transform_2D& transform,
                                          const Model::Product& product,
                                          const Track& track,
                                          const bool eulerian = true) const = 0;

               virtual Track
               get_trajectory (Lat_Long lat_long,
                               Level level,
                               Dtime dtime,
                               const Real finish_tau,
                               const vector<Product>& product_vector = vector<Product> ()) const = 0;

               virtual void
               survey_trajectory (Track& trajectory,
                                  const vector<Product>& product_vector) const = 0;

               virtual const Aws::Repository*
               get_aws_repository_ptr (const Lat_Long& lat_long,
                                       const Dtime::Set& time_set = Dtime::Set ()) const = 0;

               virtual Sounding*
               get_sounding_ptr (const Lat_Long::List& lat_long_list,
                                 const Dtime& dtime,
                                 const Real ceiling = GSL_POSINF,
                                 const Thermo_Diagram& thermo_diagram = Tephigram ()) const = 0;

               virtual Sounding*
               get_sounding_ptr (const Lat_Long& lat_long,
                                 const Dtime& dtime,
                                 const Real ceiling = GSL_POSINF) const = 0;

               static Color
               get_wind_color (const Real direction,
                               const Real speed);

               static Color
               get_color (const Model::Product& product,
                          const Real datum);

               static Color
               get_color (const Model::Product& product,
                          const Real datum,
                          const Dstring& unit);

               virtual Tokens
               get_marker_tokens (const Lat_Long& lat_long,
                                  const Dtime& dtime,
                                  const Product& product,
                                  const Level& level) const = 0; 

               class Map : public map<Dstring, Model::Stage*>
               {

                  public:

                     Map (const Model& model,
                          const Rc_File& rc_file);

                     ~Map ();

               };

         };

         class Acncrjbf : public Stage
         {

            private:

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
                     get_A (const bool is_theta) const;

                     const Tuple&
                     get_B (const bool is_theta) const;

                     Real
                     get_max_z () const;

                     const Real
                     get_z (const Integer k,
                            const Real topography,
                            const bool is_theta) const;

                     const size_t
                     get_k (const Real z,
                            const Real topography,
                            const bool is_theta,
                            Integer start_k = -1,
                            Integer end_k = -1) const;

               };

               const Model&
               model;

               const Dstring
               stage_str;

               Dtime
               basetime;

               Vertical_Coefficients
               vertical_coefficients;

               Tuple
               tuple_latitude;

               Tuple
               tuple_longitude;

               map<Dstring, Nc_File*>
               nc_file_ptr_map;

               map<Dstring, Integer>
               varid_map;

               set<Dtime>
               valid_surface_time_set;

               vector<Dtime>
               valid_surface_time_vector;

               set<Dtime>
               valid_uppers_time_set;

               vector<Dtime>
               valid_uppers_time_vector;

               void
               fill_valid_time (set<Dtime>& valid_time_set,
                                vector<Dtime>& valid_time_vector,
                                const Dstring& nc_varname);

               void
               fill_valid_time ();

               void
               acquire_ij (size_t& i,
                           size_t& j,
                           const Lat_Long& lat_long) const;

               void
               acquire_k (size_t& k,
                          const Met_Element& met_element,
                          const size_t i,
                          const size_t j,
                          const Level& level) const;

               Real
               evaluate_raw (const Dstring& varname,
                             const size_t i,
                             const size_t j,
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
                              const Dtime& dtime,
                              const Real ceiling = GSL_POSINF) const;

            public:

               Acncrjbf (const Model& model,
                         const Dstring& stage_str,
                         const Rc_File& rc_file);

               ~Acncrjbf ();

               static bool
               is_theta (const Met_Element& met_element);

               static Dstring
               get_nc_varname (const Dstring& varname);

               virtual void
               ingest (const Dstring& varname,
                       const Dstring& file_path);

               const Dtime&
               get_basetime () const;

               Domain_2D
               get_domain_2d () const;

               bool
               out_of_bounds (const Lat_Long& lat_long) const;

               pair<Real, Real>
               get_grid_size (const size_t& i,
                              const size_t& j) const;

               pair<Real, Real>
               get_grid_size (const Lat_Long& lat_long) const;

               const set<Dtime>&
               get_valid_surface_time_set () const;

               const set<Dtime>&
               get_valid_uppers_time_set () const;

               const set<Dtime>&
               get_valid_time_set (const Level& level) const;

               const set<Dtime>&
               get_valid_time_set (const Product& product,
                                   const Level& level) const;

               vector<Dtime>
               get_valid_time_vector (const Level& level,
                                      const Dtime::Set& time_set) const;

               vector<Dtime>
               get_valid_time_vector (const Product& product,
                                      const Level& level,
                                      const Dtime::Set& time_set) const;

               Real
               evaluate (const Dstring& varname,
                         const size_t& i,
                         const size_t& j) const;

               Real
               evaluate (const Dstring& varname, 
                         const Lat_Long& lat_long) const;

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
               evaluate (const Met_Element& met_element,
                         const Lat_Long& lat_long,
                         const Level& level,
                         const size_t l) const;
                         
               Real
               evaluate (const Met_Element& met_element,
                         const size_t i,
                         const size_t j,
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
               evaluate (const Met_Element& met_element,
                         const Lat_Long& lat_long,
                         const size_t k,
                         const Dtime& dtime) const;

               Real
               evaluate (const Met_Element& met_element,
                         const Lat_Long& lat_long,
                         const Level& level,
                         const Dtime& dtime) const;

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
               evaluate_dt (const Met_Element& met_element,
                            const Lat_Long& lat_long,
                            const Level& level,
                            const size_t l) const;

               Real
               evaluate_dt (const Met_Element& met_element,
                            const size_t i,
                            const size_t j,
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
                            const size_t l) const;

               Real
               evaluate_dx (const Met_Element& met_element,
                            const size_t i,
                            const size_t j,
                            const size_t l) const;

               Real
               evaluate_dx (const Met_Element& met_element,
                            const Lat_Long& lat_long,
                            const Level& level,
                            const size_t l) const;

               Real
               evaluate_dx (const Met_Element& met_element,
                            const size_t i,
                            const size_t j,
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
                            const size_t l) const;

               Real
               evaluate_dy (const Met_Element& met_element,
                            const size_t i,
                            const size_t j,
                            const size_t l) const;

               Real
               evaluate_dy (const Met_Element& met_element,
                            const Lat_Long& lat_long,
                            const Level& level,
                            const size_t l) const;

               Real
               evaluate_dy (const Met_Element& met_element,
                            const size_t i,
                            const size_t j,
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
                            const size_t i,
                            const size_t j,
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
               evaluate_tendency (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const size_t l) const;

               Real
               evaluate_tendency (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const size_t l,
                                  const Wind& wind_bg) const;

               Real
               evaluate_tendency (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const Level& level,
                                  const size_t l) const;

               Real
               evaluate_tendency (const Met_Element& met_element,
                                  const Lat_Long& lat_long,
                                  const Level& level,
                                  const size_t l,
                                  const Wind& wind_bg) const;

               Real
               evaluate_wind_advection (const Met_Element& met_element,
                                        const Lat_Long& lat_long,
                                        const size_t l,
                                        const Wind& wind) const;

               Real
               evaluate_wind_advection (const Met_Element& met_element,
                                        const size_t i,
                                        const size_t j,
                                        const size_t l,
                                        const Wind& wind) const;

               Real
               evaluate_wind_advection (const Met_Element& met_element,
                                        const Lat_Long& lat_long,
                                        const Level& level,
                                        const size_t l,
                                        const Wind& wind) const;

               Real
               evaluate_wind_advection (const Met_Element& met_element,
                                        const size_t i,
                                        const size_t j,
                                        const Level& level,
                                        const size_t l,
                                        const Wind& wind) const;

               Real
               evaluate_h_advection (const Met_Element& met_element,
                                     const Lat_Long& lat_long,
                                     const size_t l,
                                     const Wind& wind_bg = Wind (0, 0)) const;

               Real
               evaluate_h_advection (const Met_Element& met_element,
                                     const size_t i,
                                     const size_t j,
                                     const size_t l,
                                     const Wind& wind_bg = Wind (0, 0)) const;

               Real
               evaluate_h_advection (const Met_Element& met_element,
                                     const Lat_Long& lat_long,
                                     const Level& level,
                                     const size_t l,
                                     const Wind& wind_bg = Wind (0, 0)) const;

               Real
               evaluate_h_advection (const Met_Element& met_element,
                                     const size_t i,
                                     const size_t j,
                                     const Level& level,
                                     const size_t l,
                                     const Wind& wind_bg = Wind (0, 0)) const;

               Real
               evaluate_v_advection (const Met_Element& met_element,
                                     const Lat_Long& lat_long,
                                     const Level& level,
                                     const size_t l) const;

               Real
               evaluate_v_advection (const Met_Element& met_element,
                                     const size_t i,
                                     const size_t j,
                                     const Level& level,
                                     const size_t l) const;

               Real
               evaluate_s_tendency (const Met_Element& met_element,
                                    const Real azimuth,
                                    const Lat_Long& lat_long,
                                    const Level& level,
                                    const size_t l,
                                    const Real u_bg) const;

               Real
               evaluate_s_tendency (const Met_Element& met_element,
                                    const Real azimuth,
                                    const size_t i,
                                    const size_t j,
                                    const Level& level,
                                    const size_t l,
                                    const Real u_bg) const;

               Real
               evaluate_s_advection (const Met_Element& met_element,
                                     const Real azimuth,
                                     const Lat_Long& lat_long,
                                     const Level& level,
                                     const size_t l,
                                     const Real u_bg = 0) const;

               Real
               evaluate_s_advection (const Met_Element& met_element,
                                     const Real azimuth,
                                     const size_t i,
                                     const size_t j,
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
               evaluate_n_advection (const Met_Element& met_element,
                                     const Real azimuth,
                                     const size_t i,
                                     const size_t j,
                                     const Level& level,
                                     const size_t l) const;

               Real
               evaluate_normal_speed (const Real azimuth,
                                      const Lat_Long& lat_long,
                                      const Level& level,
                                      const size_t l) const;
                         
               Real
               evaluate_normal_speed (const Real azimuth,
                                      const size_t i,
                                      const size_t j,
                                      const Level& level,
                                      const size_t l) const;
                         
               Real
               evaluate_along_speed (const Real azimuth,
                                     const Lat_Long& lat_long,
                                     const Level& level,
                                     const size_t l,
                                     const Real u_bg = 0) const;

               Real
               evaluate_along_speed (const Real azimuth,
                                     const size_t i,
                                     const size_t j,
                                     const Level& level,
                                     const size_t l,
                                     const Real u_bg = 0) const;

               Real
               evaluate_richardson (const Real azimuth,
                                    const size_t i,
                                    const size_t j,
                                    const Level& level,
                                    const size_t l) const;

               Real
               evaluate_richardson (const Real azimuth,
                                    const Lat_Long& lat_long,
                                    const Level& level,
                                    const size_t l) const;

               Real
               evaluate_scorer (const Real azimuth,
                                const size_t i,
                                const size_t j,
                                const Level& level,
                                const size_t l,
                                const Real u_bg = 0) const;

               Real
               evaluate_scorer (const Real azimuth,
                                const Lat_Long& lat_long,
                                const Level& level,
                                const size_t l,
                                const Real u_bg = 0) const;

               Real
               evaluate_scorer_a (const Real azimuth,
                                  const size_t i,
                                  const size_t j,
                                  const Level& level,
                                  const size_t l,
                                  const Real u_bg = 0) const;

               Real
               evaluate_scorer_a (const Real azimuth,
                                  const Lat_Long& lat_long,
                                  const Level& level,
                                  const size_t l,
                                  const Real u_bg = 0) const;

               Real
               evaluate_scorer_b (const Real azimuth,
                                  const size_t i,
                                  const size_t j,
                                  const Level& level,
                                  const size_t l,
                                  const Real u_bg = 0) const;

               Real
               evaluate_scorer_b (const Real azimuth,
                                  const Lat_Long& lat_long,
                                  const Level& level,
                                  const size_t l,
                                  const Real u_bg = 0) const;

               Real
               evaluate_brunt_vaisala (const size_t i,
                                       const size_t j,
                                       const Level& level,
                                       const size_t l) const;

               Real
               evaluate_brunt_vaisala (const Lat_Long& lat_long,
                                       const Level& level,
                                       const size_t l) const;

               Real
               get_topography (const Lat_Long& lat_long) const;

               Real
               get_topography (const size_t i,
                               const size_t j) const;

               Color
               get_color (const Product& product,
                          const Lat_Long& lat_long,
                          const size_t l) const;

               Real
               get_value (const Product& product,
                          const Lat_Long& lat_long,
                          const size_t l) const;

               Color
               get_color (const Product& product,
                          const Lat_Long& lat_long,
                          const Level& level,
                          const size_t l) const;

               Real
               get_value (const Product& product,
                          const Lat_Long& lat_long,
                          const Level& level,
                          const size_t l) const;

               Raster*
               get_terrain_raster_ptr (const Size_2D& size_2d,
                                       const Transform_2D& transform) const;

               Scalar_Data_2D*
               get_terrain_scalar_data_2d_ptr (const Size_2D& size_2d,
                                               const Transform_2D& transform) const;

	       Raster*
               get_surface_raster_ptr (const Size_2D& size_2d,
                                       const Transform_2D& transform,
                                       const Model::Product& product,
                                       const Dtime& dtime) const;

               Scalar_Data_2D*
               get_surface_scalar_data_2d_ptr (const Size_2D& size_2d,
                                               const Transform_2D& transform,
                                               const Model::Product& product,
                                               const Dtime& dtime) const;

               Raster*
               get_uppers_raster_ptr (const Size_2D& size_2d,
                                      const Transform_2D& transform,
                                      const Model::Product& product,
                                      const Dtime& dtime,
                                      const Level& level) const;

               Scalar_Data_2D*
               get_uppers_scalar_data_2d_ptr (const Size_2D& size_2d,
                                              const Transform_2D& transform,
                                              const Model::Product& product,
                                              const Dtime& dtime,
                                              const Level& level) const;

               Raster*
               get_geo_raster_ptr (const Size_2D& size_2d,
                                   const Transform_2D& transform,
                                   const Product& product = Product ("TERRAIN"),
                                   const Dtime& dtime = Dtime (),
                                   const Level& level = Level ()) const;

               Scalar_Data_2D*
               get_geo_scalar_data_2d_ptr (const Size_2D& size_2d,
                                           const Transform_2D& transform,
                                           const Product& product,
                                           const Dtime& dtime = Dtime (),
                                           const Level& level = Level ()) const;

               Raster*
               get_cross_section_raster_ptr (const Box_2D& box_2d,
                                             const Transform_2D& transform,
                                             const Model::Product& product,
                                             const Dtime& dtime,
                                             const Journey& journey,
                                             const Real u_bg = 0) const;

               Scalar_Data_2D*
               get_cross_section_scalar_data_2d_ptr (const Box_2D& box_2d,
                                                     const Transform_2D& transform,
                                                     const Product& product,
                                                     const Dtime& dtime,
                                                     const Journey& journey,
                                                     const Real u_bg = 0) const;

               Raster*
               get_time_cross_raster_ptr (const Box_2D& box_2d,
                                          const Transform_2D& transform,
                                          const Model::Product& product,
                                          const Location& location,
                                          const Dtime::Span& time_span,
                                          const Real azimuth = GSL_NAN) const;

               Raster*
               get_time_cross_raster_ptr (const Box_2D& box_2d,
                                          const Transform_2D& transform,
                                          const Model::Product& product,
                                          const Track& track,
                                          const bool eulerian = true) const;

               size_t
               get_surface_l (const Dtime& dtime) const;

               size_t
               get_uppers_l (const Dtime& dtime) const;

               Track
               get_trajectory (Lat_Long lat_long,
                               Level level,
                               Dtime dtime,
                               const Real finish_tau,
                               const vector<Product>& product_vector = vector<Product> ()) const;

               void
               survey_trajectory (Track& trajectory,
                                  const vector<Product>& product_vector) const;

               Aws::Obs
               get_aws_obs (const Lat_Long& lat_long,
                            const Dtime& dtime) const;

               const Aws::Repository*
               get_aws_repository_ptr (const Lat_Long& lat_long,
                                       const Dtime::Set& time_set = Dtime::Set ()) const;

               Sounding*
               get_sounding_ptr (const Lat_Long& lat_long,
                                 const Dtime& dtime,
                                 const Real ceiling = GSL_POSINF) const;

               Sounding*
               get_sounding_ptr (const Lat_Long::List& lat_long_list,
                                 const Dtime& dtime,
                                 const Real ceiling = GSL_POSINF,
                                 const Thermo_Diagram& thermo_diagram = Tephigram ()) const;

               Tokens
               get_marker_tokens (const Lat_Long& lat_long,
                                  const Dtime& dtime,
                                  const Product& product,
                                  const Level& level) const;

         };

      private:

         Stage::Map
         stage_map;

      public:

         Model (const Rc_File& rc_file);

         ~Model ();

         const Model::Stage&
         get_stage (const Dstring& stage) const;

   };

};

#endif /* MODEL_H */
