#ifndef OBS_H
#define OBS_H

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

               Real
               get_rh () const;

               Real
               get_gfdi (const Real gust_weight = 0.333,
                         const Real curing = 100) const;

               Real
               get_ffdi (const Real gust_weight = 0.333,
                         const Real df = 10) const;

               Real
               get_product (const Dstring& product_str) const;

               Tuple
               get_products (const Tokens& product_tokens) const;

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

};

#endif /* OBS_H */
