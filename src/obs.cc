#include <denise/dstring.h>
#include <denise/gzstream.h>
#include <denise/nwp.h>
#include "obs.h"

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;

Station::Station (const Integer id,
                  const Real latitude,
                  const Real longitude,
                  const Real height,
                  const Dstring& name)
   : Lat_Long (latitude, longitude),
     id (id),
     height (height),
     name (name)
{
}

Station::Map::Map ()
{
}

Station::Map::Map (const Dstring& file_path)
{
   ingest (file_path);
}

void
Station::Map::ingest (const Dstring& file_path)
{

   string is;
   ifstream file (file_path);

   while (getline (file, is))
   {

      if (is.size () == 0) { continue; }
      if (is[0] == '#') { continue; }

      const Dstring input_string (is);
      const Tokens tokens (input_string, ":");
      const Integer id = stoi (tokens[0]);
      const Real latitude = stof (tokens[1]);
      const Real longitude = stof (tokens[2]);
      const Real height = stof (tokens[3]);
      const Dstring& name = tokens[4];
      insert (make_pair (id, Station (id, latitude, longitude, height, name)));

   }

   file.close ();

}

const Station&
Station::Map::get_station (const Lat_Long& lat_long,
                           const Real tolerance) const
{

   for (auto iterator = begin (); iterator != end (); iterator++)
   {

      const Station& station = iterator->second;
      const Real distance = Geodesy::get_distance (lat_long, station);
      if (distance >= tolerance) { continue; }

      return station;

   }

}

const Station&
Station::Map::get_nearest_station (const Lat_Long& lat_long) const
{

   Integer nearest_station_id = -1;
   Real min_distance = GSL_POSINF;

   for (auto iterator = begin (); iterator != end (); iterator++)
   {

      const Station& station = iterator->second;
      const Real distance = Geodesy::get_distance (lat_long, station);
      if (distance >= min_distance) { continue; }

      min_distance = distance;
      nearest_station_id = station.id;

   }

   return at (nearest_station_id);

}

void
Station::Map::attract (Real& latitude,
                       Real& longitude) const
{
   const Lat_Long lat_long (latitude, longitude);
   const Station& station = get_nearest_station (lat_long);
   latitude = station.latitude;
   longitude = station.longitude;
}

pair<Dstring, Lat_Long>
Station::Map::nearest (const Lat_Long& lat_long) const
{
   const Station& station = get_nearest_station (lat_long);
   const Dstring& str = Dstring::render ("%d", station.id);
   return make_pair (str, station);
}

void
Station::Map::cairo (const RefPtr<Context>& cr,
                     const Transform_2D& transform,
                     const Color& color) const
{

   cr->save ();
   color.cairo (cr);

   for (auto iterator = begin (); iterator != end (); iterator++)
   {
      const Station& station = iterator->second;
      Ring (3).cairo (cr, transform.transform (station));
      cr->fill ();
   }

   cr->restore ();

}

Location::Location (const Lat_Long& lat_long)
   : Lat_Long (lat_long),
     station_id (-1)
{
   this->str = Lat_Long::get_string (4, true);
   this->long_str = Lat_Long::get_string (4, false);
}

Location::Location (const Dstring& str,
                    const Station::Map& station_map)
   : Lat_Long (GSL_NAN, GSL_NAN),
     station_id (-1),
     str (str),
     long_str ("")
{

   const bool is_ll = Reg_Exp (",").match (str);

   if (is_ll)
   {
      const Tokens tokens (str, ",");
      this->latitude = stof (tokens[0]);
      this->longitude = stof (tokens[1]);
      this->str = Lat_Long::get_string (false, Dstring ("%.4f"));
   }
   else
   {
      const Dstring& station_id_str = str;
      const Integer station_id = stof (station_id_str);
      const Station& station = station_map.at (station_id);
      this->latitude = station.latitude;
      this->longitude = station.longitude;
      this->station_id = station_id;
      this->str = station_id_str;
      this->long_str = station.name + " (" + station_id_str + ")";
   }

}

Integer
Location::get_station_id () const
{
   return station_id;
}

const Dstring&
Location::get_str () const
{
   return str;
}

const Dstring&
Location::get_long_str () const
{
   return long_str;
}

Aws::Obs::Obs (const Real temperature,
               const Real dew_point,
               const Real wind_direction,
               const Real wind_speed,
               const Real ten_min_wind_speed,
               const Real station_p,
               const Real mslp,
               const Real wind_gust)
   : temperature (temperature),
     dew_point (dew_point),
     wind_direction (wind_direction),
     wind_speed (wind_speed),
     ten_min_wind_speed (ten_min_wind_speed),
     station_p (station_p),
     mslp (mslp),
     wind_gust (wind_gust)
{
}

Aws::Obs::Obs (const Aws::Obs& obs)
   : temperature (obs.temperature),
     dew_point (obs.dew_point),
     wind_direction (obs.wind_direction),
     wind_speed (obs.wind_speed),
     ten_min_wind_speed (obs.ten_min_wind_speed),
     station_p (obs.station_p),
     mslp (obs.mslp),
     wind_gust (obs.wind_gust)
{
}

Real
Aws::Obs::get_rh () const
{
   return Moisture::get_rh (temperature - K, dew_point - K);
}

Real
Aws::Obs::get_gfdi (const Real gust_weight,
                    const Real curing) const
{
   const Real gw = std::max (std::min (gust_weight, 1.0), 0.0);
   const Real speed = (wind_speed * (1 - gw) + wind_gust * gw);
   return Fire::get_gfdi_si (temperature, dew_point, speed, curing);
}

Real
Aws::Obs::get_ffdi (const Real gust_weight,
                    const Real df) const
{
   const Real gw = std::max (std::min (gust_weight, 1.0), 0.0);
   const Real speed = (wind_speed * (1 - gw) + wind_gust * gw);
   return Fire::get_ffdi_si (temperature, dew_point, speed, df);
}

Real
Aws::Obs::get_product (const Dstring& product_str) const
{

   const Tokens tokens (product_str, ":");
   const Dstring& p = tokens[0];
   const Integer n = tokens.size ();

   if (p == "t" || p == "temperature")
   {
      return temperature;
   }
   else
   if (p == "td" || p == "dew_point")
   {
      return dew_point;
   }
   else
   if (p == "rh" || p == "relative_humidity")
   {
      return get_rh ();
   }
   else
   if (p == "direction" || p == "wind_direction")
   {
      return wind_direction;
   }
   else
   if (p == "speed" || p == "wind_speed")
   {
      return wind_speed;
   }
   else
   if (p == "gust" || p == "wind_gust")
   {
      return wind_gust;
   }
   else
   if (p == "p" || p == "station_p")
   {
      return station_p;
   }
   else
   if (p == "mslp" || p == "mean_sea_level_pressure")
   {
      return mslp;
   }
   else
   if (p == "gfdi")
   {

      if (n < 2) { return get_gfdi (); }
      const Real gust_weight = stof (tokens[1]);

      if (n < 3) { return get_gfdi (gust_weight); }
      const Real curing = stof (tokens[2]);

      return get_gfdi (gust_weight, curing);

   }
   else
   if (p == "ffdi")
   {

      if (n < 2) { return get_ffdi (); }
      const Real gust_weight = stof (tokens[1]);

      if (n < 3) { return get_ffdi (gust_weight); }
      const Real df = stof (tokens[2]);

      return get_ffdi (gust_weight, df);

   }

}

Tuple
Aws::Obs::get_products (const Tokens& product_tokens) const
{

   Tuple tuple;

   for (auto p = product_tokens.begin (); p != product_tokens.end (); p++)
   {
      const Dstring& product_str = *(p);
      const Real datum = get_product (product_str);
      tuple.push_back (datum);
   }

   return tuple;

}

Aws::Key::Key (const Integer station_id,
               const Dtime& dtime)
   : station_id (station_id),
     dtime (dtime)
{
}

Aws::Key::Key (const Key& key)
   : station_id (key.station_id),
     dtime (key.dtime)
{
}

bool
Aws::Key::operator == (const Key& key) const
{
   return (station_id == key.station_id) && (dtime == key.dtime);
}
      
bool
Aws::Key::operator > (const Key& key) const
{
   if (station_id == key.station_id)
   {
      return dtime > key.dtime;
   }
   else
   {
      return station_id > key.station_id;
   }
}

bool
Aws::Key::operator < (const Key& key) const
{
   if (station_id == key.station_id)
   {
      return dtime < key.dtime;
   }
   else
   {
      return station_id < key.station_id;
   }
}

Real
Aws::Repository::to_real (const Dstring& token)
{
  return (token == "-9999" ? GSL_NAN : stof (token));
}

Aws::Repository::Repository ()
{
}

Aws::Repository::Repository (const Dstring& file_path)
{
   ingest (file_path);
}

void
Aws::Repository::insert (const Aws::Key& key,
                         const Aws::Obs& obs)
{
   map<Key, Obs>::insert (make_pair (key, obs));
   station_id_set.insert (key.station_id);
   valid_time_set.insert (key.dtime);
}

void
Aws::Repository::ingest (const Dstring& file_path)
{

   igzstream file (file_path.get_string ());

   for (string il; std::getline (file, il); )
   {

      const Dstring input_line (il);
      const Tokens tokens (input_line, ":");

      const Integer station_id = stoi (tokens[0]);
      const Dtime dtime (tokens[1]);
      const Real t          = Aws::Repository::to_real (tokens[2]) + K;
      const Real t_d        = Aws::Repository::to_real (tokens[3]) + K;
      const Real wind_dir   = Aws::Repository::to_real (tokens[4]);
      const Real wind_speed = Aws::Repository::to_real (tokens[5]);
      const Real wind_gust  = Aws::Repository::to_real (tokens[6]);
      const Real station_p  = Aws::Repository::to_real (tokens[7]) * 1e2;
      const Real mslp       = Aws::Repository::to_real (tokens[8]) * 1e2;
      const Real ten_min_wind_speed = Aws::Repository::to_real (tokens[9]);

      Aws::Key key (station_id, dtime);
      Aws::Obs obs (t, t_d, wind_dir, wind_speed,
         ten_min_wind_speed, station_p, mslp, wind_gust);

      insert (key, obs);

   }

   file.close ();

}

void
Aws::Repository::ingest_binary (const Dstring& file_path)
{

   igzstream file (file_path.get_string (), ios_base::in | ios_base::binary);

   const Integer buffer_size = 22;
   char* buffer = new char[buffer_size];

Integer i = 0;
   for (; file.read ((char*)buffer, buffer_size); )
   {

      uint32_t station_id = *((uint32_t*)(buffer + 0));
      int32_t minutes     = *((int32_t*)(buffer + 4));
      int16_t t_bin          = *((int16_t*)(buffer +  8));
      int16_t t_d_bin        = *((int16_t*)(buffer + 10));
      int16_t wind_dir_bin   = *((int16_t*)(buffer + 12));
      int16_t wind_speed_bin = *((int16_t*)(buffer + 14));
      int16_t wind_gust_bin  = *((int16_t*)(buffer + 16));
      int16_t station_p_bin  = *((int16_t*)(buffer + 18));
      int16_t mslp_bin       = *((int16_t*)(buffer + 20));

#ifndef WORDS_BIGENDIAN
      swap_endian (&station_id, 4);
      swap_endian (&minutes, 4);
      swap_endian (&t_bin, 2);
      swap_endian (&t_d_bin, 2);
      swap_endian (&wind_dir_bin, 2);
      swap_endian (&wind_speed_bin, 2);
      swap_endian (&wind_gust_bin, 2);
      swap_endian (&station_p_bin, 2);
      swap_endian (&mslp_bin, 2);
#endif

if (i < 20)
{
   cout << station_id << " " << minutes << " " << Dtime (minutes / 60.0).get_string ("%Y%m%d%H%M") << " " << t_bin << " " << t_d_bin << " " << wind_dir_bin << " " << wind_speed_bin << " " << wind_gust_bin<< " " << station_p_bin << " " << mslp_bin << endl;
}

      const Dtime dtime (minutes / 60.0);
      const Real t          = t_bin * 1e-1 + K;
      const Real t_d        = t_d_bin* 1e-1 + K;
      const Real wind_dir   = wind_dir_bin;
      const Real wind_speed = wind_speed_bin * 1e-1;
      const Real wind_gust  = wind_gust_bin * 1e-1;
      const Real station_p  = station_p_bin * 10.0;
      const Real mslp       = mslp_bin * 10.0;

      Aws::Key key (station_id, dtime);
      Aws::Obs obs (t, t_d, wind_dir, wind_speed,
         wind_speed, station_p, mslp, wind_gust);
      insert (key, obs);

i++;
   }

   file.close ();
   delete[] buffer;

}

void
Aws::Repository::write (const Dstring& file_path) const
{

   ogzstream file (file_path.get_string (), ios_base::out);

   for (auto iterator = begin (); iterator != end (); iterator++)
   {

      const Aws::Key& key = iterator->first;
      const Aws::Obs& obs = iterator->second;

      const Integer station_id = key.station_id;
      const Dtime& dtime       = key.dtime;
      const Real temperature = obs.temperature - K;
      const Real dew_point   = obs.dew_point - K;
      const Real wind_dir    = obs.wind_direction;
      const Real wind_speed  = obs.wind_speed;
      const Real wind_gust   = obs.wind_gust;
      const Real station_p   = obs.station_p * 1e-2;
      const Real mslp        = obs.mslp * 1e-2;

      file << station_id << ":";
      file << dtime.get_string ("%Y%m%d%H%M") << ":";
      file << temperature << ":";
      file << dew_point << ":";
      file << wind_dir << ":";
      file << wind_speed << ":";
      file << station_p << ":";
      file << mslp << endl;

   }

   file.close ();

}

void
Aws::Repository::write_binary (const Dstring& file_path) const
{

   ogzstream file (file_path.get_string (), ios_base::out | ios::binary);

   const Integer buffer_size = 22;
   char* buffer = new char[buffer_size];

   for (auto iterator = begin (); iterator != end (); iterator++)
   {

      const Aws::Key& key = iterator->first;
      const Aws::Obs& obs = iterator->second;

      uint32_t station_id = key.station_id;
      int32_t minutes     = int32_t (round (key.dtime.t * 60));
      int16_t temperature = int16_t (round ((obs.temperature - K) * 10));
      int16_t dew_point   = int16_t (round ((obs.dew_point - K) * 10));
      int16_t wind_dir    = int16_t (round (obs.wind_direction));
      int16_t wind_speed  = int16_t (round (obs.wind_speed * 10));
      int16_t wind_gust   = int16_t (round (obs.wind_gust * 10));
      int16_t station_p   = int16_t (round (obs.station_p / 10));
      int16_t mslp        = int16_t (round (obs.mslp / 10));

#ifndef WORDS_BIGENDIAN
      swap_endian (&station_id, 4);
      swap_endian (&minutes, 4);
      swap_endian (&temperature, 2);
      swap_endian (&dew_point, 2);
      swap_endian (&wind_dir, 2);
      swap_endian (&wind_speed, 2);
      swap_endian (&wind_gust, 2);
      swap_endian (&station_p, 2);
      swap_endian (&mslp, 2);
#endif

      memcpy (buffer +  0, &station_id,  4);
      memcpy (buffer +  4, &minutes,     4);
      memcpy (buffer +  8, &temperature, 2);
      memcpy (buffer + 10, &dew_point,   2);
      memcpy (buffer + 12, &wind_dir,    2);
      memcpy (buffer + 14, &wind_speed,  2);
      memcpy (buffer + 16, &wind_gust,   2);
      memcpy (buffer + 18, &station_p,   2);
      memcpy (buffer + 20, &mslp,        2);

      file.write (buffer, buffer_size);

   }

   file.close ();

}

const set<Integer>&
Aws::Repository::get_station_id_set () const
{
   return station_id_set;
}

const set<Dtime>&
Aws::Repository::get_valid_time_set () const
{
   return valid_time_set;
}

const Aws::Repository*
Aws::Repository::get_aws_repository_ptr (const Integer station_id,
                                         const Dtime::Set& time_set) const
{

   Aws::Repository* aws_repository_ptr = new Aws::Repository ();
   Aws::Repository& aws_repository = *aws_repository_ptr;

   for (auto iterator = begin (); iterator != end (); iterator++)
   {

      const Aws::Key& key = iterator->first;
      if (station_id != key.station_id) { continue; }

      const Dtime& dtime = key.dtime;
      if (!time_set.match (dtime)) { continue; }

      const Aws::Obs& obs = iterator->second;
      aws_repository.insert (key, obs);

   }

   return aws_repository_ptr;

}

Domain_1D
Aws::Repository::get_temperature_domain (const Dtime::Set& time_set) const
{

   Domain_1D temperature_domain (GSL_POSINF, GSL_NEGINF);

   for (auto iterator = begin (); iterator != end (); iterator++)
   {

      const Aws::Obs& obs = iterator->second;
      const Real temperature = obs.temperature;

      if (temperature > temperature_domain.end)
      {
         temperature_domain.end = temperature;
      }

      if (temperature < temperature_domain.start)
      {
         temperature_domain.start = temperature;
      }

   }

   return temperature_domain;

}

Domain_1D
Aws::Repository::get_dew_point_domain (const Dtime::Set& time_set) const
{

   Domain_1D dew_point_domain (GSL_POSINF, GSL_NEGINF);

   for (auto iterator = begin (); iterator != end (); iterator++)
   {

      const Aws::Obs& obs = iterator->second;
      const Real dew_point = obs.dew_point;

      if (dew_point > dew_point_domain.end)
      {
         dew_point_domain.end = dew_point;
      }

      if (dew_point < dew_point_domain.start)
      {
         dew_point_domain.start = dew_point;
      }

   }

   return dew_point_domain;

}

Domain_1D
Aws::Repository::get_wind_speed_domain (const Dtime::Set& time_set) const
{

   Domain_1D wind_speed_domain (GSL_POSINF, GSL_NEGINF);

   for (auto iterator = begin (); iterator != end (); iterator++)
   {

      const Aws::Obs& obs = iterator->second;
      const Real wind_speed = obs.wind_speed;

      if (wind_speed > wind_speed_domain.end)
      {
         wind_speed_domain.end = wind_speed;
      }

      if (wind_speed < wind_speed_domain.start)
      {
         wind_speed_domain.start = wind_speed;
      }

   }

   return wind_speed_domain;

}

Domain_1D
Aws::Repository::get_mslp_domain (const Dtime::Set& time_set) const
{

   Domain_1D mslp_domain (GSL_POSINF, GSL_NEGINF);

   for (auto iterator = begin (); iterator != end (); iterator++)
   {

      const Aws::Obs& obs = iterator->second;
      const Real mslp = obs.mslp;

      if (mslp > mslp_domain.end)
      {
         mslp_domain.end = mslp;
      }

      if (mslp < mslp_domain.start)
      {
         mslp_domain.start = mslp;
      }

   }

   return mslp_domain;

}

Domain_1D
Aws::Repository::get_station_p_domain (const Dtime::Set& time_set) const
{

   Domain_1D station_p_domain (GSL_POSINF, GSL_NEGINF);

   for (auto iterator = begin (); iterator != end (); iterator++)
   {

      const Aws::Obs& obs = iterator->second;
      const Real station_p = obs.station_p;

      if (station_p > station_p_domain.end)
      {
         station_p_domain.end = station_p;
      }

      if (station_p < station_p_domain.start)
      {
         station_p_domain.start = station_p;
      }

   }

   return station_p_domain;

}

