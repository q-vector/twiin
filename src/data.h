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
#include "model.h"
#include "obs.h"

using namespace std;
using namespace denise;
using namespace Cairo;

namespace twiin
{

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
