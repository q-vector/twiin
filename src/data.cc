#include <denise/dstring.h>
#include <denise/gzstream.h>
#include <denise/nwp.h>
#include "data.h"
#include "display.h"

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;

Data::Data (const Config_File& config_file)
   : model (config_file),
     hrit (config_file)
{

   for (auto iterator = config_file.begin ();
        iterator != config_file.end (); iterator++)
   {

      const Tokens tokens (*(iterator), " \f\n\t");

      if (tokens.size () == 2 && tokens[0] == "aws")
      {
         const Dstring& aws_file_path = tokens[1];
         aws_repository.ingest (aws_file_path);
      }

      if (tokens.size () == 2 && tokens[0] == "aws_bin")
      {
         const Dstring& aws_file_path = tokens[1];
         aws_repository.ingest_binary (aws_file_path);
      }

      if (tokens.size () == 2 && tokens[0] == "stations")
      {
         const Dstring& station_map_file_path = tokens[1];
         station_map.ingest (station_map_file_path);
      }

   }

}

Data::~Data ()
{
}

const Model&
Data::get_model () const
{
   return model;
}

const Hrit&
Data::get_hrit () const
{
   return hrit;
}

const Station::Map&
Data::get_station_map () const
{
   return station_map;
}

const Aws::Repository&
Data::get_aws_repository () const
{
   return aws_repository;
}

const Aws::Repository*
Data::get_aws_repository_ptr (const Integer station_id,
                              const Dtime::Span& time_span) const
{
   return aws_repository.get_aws_repository_ptr (station_id, time_span);
}

Lat_Long
Data::get_lat_long (const Dstring& location_str) const
{

   const Tokens tokens (location_str, ":");

   if (tokens.size () == 1)
   {
      const Integer station_id = stof (tokens[0]);
      return station_map.at (station_id);
   }
   else
   if (tokens.size () == 2)
   {
      const Real latitude = stof (tokens[0]);
      const Real longitude = stof (tokens[1]);
      return Lat_Long (latitude, longitude);
   }

   return Lat_Long (GSL_NAN, GSL_NAN);

}

