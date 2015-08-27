#include <denise/dstring.h>
#include <denise/gzstream.h>
#include <denise/nwp.h>
#include "data.h"
#include "display.h"

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;

Stage::Stage (const Dstring& str)
   : Dstring (str)
{
}

Stage::Stage (const Stage& stage)
   : Dstring (stage)
{
}

Dstring
Stage::get_string () const
{
   return *this;
}

Product::Product (const Product::Enumeration& enumeration)
   : enumeration (enumeration)
{
}

Product::Product (const Product& product)
   : enumeration (product.enumeration)
{
}

Product::Product (const Dstring& str)
{
        if (str == "TERRAIN")        { enumeration = Product::TERRAIN; }
   else if (str == "TERRAIN_WATER")  { enumeration = Product::TERRAIN_WATER; }
   else if (str == "P_THETA")        { enumeration = Product::P_THETA; }
   else if (str == "P_RHO")          { enumeration = Product::P_RHO; }
   else if (str == "ALONG_SPEED")    { enumeration = Product::ALONG_SPEED; }
   else if (str == "NORMAL_SPEED")   { enumeration = Product::NORMAL_SPEED; }
   else if (str == "BRUNT_VAISALA")  { enumeration = Product::BRUNT_VAISALA; }
   else if (str == "SCORER")         { enumeration = Product::SCORER; }
   else if (str == "T")              { enumeration = Product::T; }
   else if (str == "TD")             { enumeration = Product::TD; }
   else if (str == "RH")             { enumeration = Product::RH; }
   else if (str == "THETA")          { enumeration = Product::THETA; }
   else if (str == "Q")              { enumeration = Product::Q; }
   else if (str == "THETA_E")        { enumeration = Product::THETA_E; }
   else if (str == "THETA_V")        { enumeration = Product::THETA_V; }
   else if (str == "RHO")            { enumeration = Product::RHO; }
   else if (str == "WIND")           { enumeration = Product::WIND; }
   else if (str == "SPEED")          { enumeration = Product::SPEED; }
   else if (str == "SPEED_HIGHER")   { enumeration = Product::SPEED_HIGHER; }
   else if (str == "VORTICITY")      { enumeration = Product::VORTICITY; }
   else if (str == "W")              { enumeration = Product::W; }
   else if (str == "W_TRANSLUCENT")  { enumeration = Product::W_TRANSLUCENT; }
   else if (str == "Q_ADVECTION")    { enumeration = Product::Q_ADVECTION; }
   else if (str == "Q_H_ADVECTION")  { enumeration = Product::Q_H_ADVECTION; }
   else if (str == "Q_V_ADVECTION")  { enumeration = Product::Q_V_ADVECTION; }
   else if (str == "Q_S_ADVECTION")  { enumeration = Product::Q_S_ADVECTION; }
   else if (str == "Q_N_ADVECTION")  { enumeration = Product::Q_N_ADVECTION; }
   else if (str == "Q_SV_ADVECTION") { enumeration = Product::Q_SV_ADVECTION; }
   else if (str == "Q_NV_ADVECTION") { enumeration = Product::Q_NV_ADVECTION; }
   else if (str == "FFDI")           { enumeration = Product::FFDI; }
   else if (str == "MSLP")           { enumeration = Product::MSLP; }
   else if (str == "PRECIP_RATE")    { enumeration = Product::PRECIP_RATE; }
   else if (str == "IR1")            { enumeration = Product::IR1; }
   else if (str == "IR2")            { enumeration = Product::IR2; }
   else if (str == "IR3")            { enumeration = Product::IR3; }
   else if (str == "IR4")            { enumeration = Product::IR4; }
   else if (str == "VIS")            { enumeration = Product::VIS; }
   else if (str == "Pseudo")         { enumeration = Product::Pseudo; }
   else                              { enumeration = Product::NIL; }
}

Dstring
Product::get_string () const
{
   switch (enumeration)
   {
      case Product::TERRAIN:        return "TERRAIN";
      case Product::TERRAIN_WATER:  return "TERRAIN_WATER";
      case Product::P_THETA:        return "P_THETA";
      case Product::P_RHO:          return "P_RHO";
      case Product::ALONG_SPEED:    return "ALONG_SPEED";
      case Product::NORMAL_SPEED:   return "NORMAL_SPEED";
      case Product::BRUNT_VAISALA:  return "BRUNT_VAISALA";
      case Product::SCORER:         return "SCORER";
      case Product::T:              return "T";
      case Product::TD:             return "TD";
      case Product::RH:             return "RH";
      case Product::THETA:          return "THETA";
      case Product::Q:              return "Q";
      case Product::THETA_E:        return "THETA_E";
      case Product::THETA_V:        return "THETA_V";
      case Product::RHO:            return "RHO";
      case Product::WIND:           return "WIND";
      case Product::SPEED:          return "SPEED";
      case Product::SPEED_HIGHER:   return "SPEED_HIGHER";
      case Product::VORTICITY:      return "VORTICITY";
      case Product::W:              return "W";
      case Product::W_TRANSLUCENT:  return "W_TRANSLUCENT";
      case Product::Q_ADVECTION:    return "Q_ADVECTION";
      case Product::Q_H_ADVECTION:  return "Q_H_ADVECTION";
      case Product::Q_V_ADVECTION:  return "Q_V_ADVECTION";
      case Product::Q_S_ADVECTION:  return "Q_S_ADVECTION";
      case Product::Q_N_ADVECTION:  return "Q_N_ADVECTION";
      case Product::Q_SV_ADVECTION: return "Q_SV_ADVECTION";
      case Product::Q_NV_ADVECTION: return "Q_NV_ADVECTION";
      case Product::FFDI:           return "FFDI";
      case Product::MSLP:           return "MSLP";
      case Product::PRECIP_RATE:    return "PRECIP_RATE";
      case Product::IR1:            return "IR1";
      case Product::IR2:            return "IR2";
      case Product::IR3:            return "IR3";
      case Product::IR4:            return "IR4";
      case Product::VIS:            return "VIS";
      case Product::Pseudo:         return "Pseudo";
      default:                      return "";
   }
}

Dstring
Product::get_description () const
{
   switch (enumeration)
   {
      case Product::TERRAIN:        return "Terrain";
      case Product::TERRAIN_WATER:  return "Terrain Water";
      case Product::P_THETA:        return "P_\u03b8";
      case Product::P_RHO:          return "P_\u03c1";
      case Product::ALONG_SPEED:    return "Along Speed";
      case Product::NORMAL_SPEED:   return "Normal Speed";
      case Product::BRUNT_VAISALA:  return "Brunt-V\u00e4IS\u00e4L\u00e4";
      case Product::SCORER:         return "Scorer's Parameter";
      case Product::T:              return "Temperature";
      case Product::TD:             return "Dew Point";
      case Product::RH:             return "Relative Humidity";
      case Product::THETA:          return "Potential Temperature";
      case Product::Q:              return "Specific Humidity";
      case Product::THETA_E:        return "Equivalent Potential Temperature";
      case Product::THETA_V:        return "Virtual Temperature";
      case Product::RHO:            return "Density";
      case Product::WIND:           return "Wind";
      case Product::SPEED:          return "Speed";
      case Product::SPEED_HIGHER:   return "Higher Speed";
      case Product::VORTICITY:      return "Vertical Vorticity";
      case Product::W:              return "Vertical Velocity";
      case Product::W_TRANSLUCENT:  return "Vertical Velocity";
      case Product::Q_ADVECTION:    return "Q Advection";
      case Product::Q_H_ADVECTION:  return "Horizontal Q Advection";
      case Product::Q_V_ADVECTION:  return "Vertical Q Advection";
      case Product::Q_S_ADVECTION:  return "Stream Q Advection";
      case Product::Q_N_ADVECTION:  return "Normal Q Advection";
      case Product::Q_SV_ADVECTION: return "Stream + Vertical Advection";
      case Product::Q_NV_ADVECTION: return "Normal + Vertical Advection";
      case Product::FFDI:           return "FFDI";
      case Product::MSLP:           return "MSLP";
      case Product::PRECIP_RATE:    return "Precipitation Rate";
      case Product::IR1:            return "IR1";
      case Product::IR2:            return "IR2";
      case Product::IR3:            return "IR3";
      case Product::IR4:            return "IR4";
      case Product::VIS:            return "VIS";
      case Product::Pseudo:         return "Pseudo";
   }
}

Met_Element
Product::get_met_element () const
{
   switch (enumeration)
   {
      case Product::T:              return denise::T;
      case Product::TD:             return denise::TD;
      case Product::RH:             return denise::RH;
      case Product::THETA:          return denise::THETA;
      case Product::Q:              return denise::Q;
      case Product::THETA_E:        return denise::THETA_E;
      case Product::THETA_V:        return denise::THETA_V;
      case Product::RHO:            return denise::RHO;
      case Product::WIND:           return denise::WIND_DIRECTION;
      case Product::SPEED:          return denise::WIND_SPEED;
      case Product::SPEED_HIGHER:   return denise::WIND_SPEED;
      case Product::VORTICITY:      return denise::RELATIVE_VORTICITY;
      case Product::W:              return denise::W;
      case Product::W_TRANSLUCENT:  return denise::W;
      case Product::Q_ADVECTION:    return denise::Q_ADVECTION;
      case Product::Q_H_ADVECTION:  return denise::Q_H_ADVECTION;
      case Product::Q_V_ADVECTION:  return denise::Q_V_ADVECTION;
      case Product::Q_S_ADVECTION:  return denise::Q_S_ADVECTION;
      case Product::Q_N_ADVECTION:  return denise::Q_N_ADVECTION;
      case Product::Q_SV_ADVECTION: return denise::Q_SV_ADVECTION;
      case Product::Q_NV_ADVECTION: return denise::Q_NV_ADVECTION;
      case Product::FFDI:           return denise::FFDI;
      case Product::MSLP:           return denise::MSLP;
      case Product::PRECIP_RATE:    return denise::PRECIP_RATE;
   }
}

Dstring
Product::get_unit () const
{
   switch (enumeration)
   {
      case Product::T:              return "\u00b0C";
      case Product::TD:             return "\u00b0C";
      case Product::RH:             return "%%";
      case Product::THETA:          return "\u00b0C";
      case Product::THETA_V:        return "\u00b0C";
      case Product::THETA_E:        return "\u00b0C";
      case Product::Q:              return "g kg\u207b\u00b9";
      case Product::RHO:            return "kgm\u207b\u00b9";
      case Product::W:              return "ms\u207b\u00b9";
      case Product::W_TRANSLUCENT:  return "ms\u207b\u00b9";
      case Product::Q_ADVECTION:    return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_H_ADVECTION:  return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_V_ADVECTION:  return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_S_ADVECTION:  return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_N_ADVECTION:  return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_SV_ADVECTION: return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_NV_ADVECTION: return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::SPEED_HIGHER:   return "knots";
      case Product::SPEED:          return "knots";
      case Product::ALONG_SPEED:    return "knots";
      case Product::NORMAL_SPEED:   return "knots";
      case Product::VORTICITY:      return "10\u207b\u00b3 s\u207b\u00b9";
      case Product::FFDI:           return "";
      case Product::MSLP:           return "hPa";
      case Product::PRECIP_RATE:    return "mm hr\u207b\u00b9";
      case Product::WIND:           return "degree";
      case Product::TERRAIN:        return "m";
      case Product::BRUNT_VAISALA:  return "s\u207b\u00b9";
      default:                      return "";
   }
}

ostream&
Product::operator<< (ostream& out_file) const
{
   out_file << get_string ();
   return out_file;
}

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
               const Real station_p,
               const Real mslp,
               const Real wind_gust)
   : temperature (temperature),
     dew_point (dew_point),
     wind_direction (wind_direction),
     wind_speed (wind_speed),
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
     station_p (obs.station_p),
     mslp (obs.mslp),
     wind_gust (obs.wind_gust)
{
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

      Aws::Key key (station_id, dtime);
      Aws::Obs obs (t, t_d, wind_dir, wind_speed, station_p, mslp, wind_gust);

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
      Aws::Obs obs (t, t_d, wind_dir, wind_speed, station_p, mslp, wind_gust);
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
                                         const Dtime::Span& time_span) const
{

   Aws::Repository* aws_repository_ptr = new Aws::Repository ();
   Aws::Repository& aws_repository = *aws_repository_ptr;

   for (auto iterator = begin (); iterator != end (); iterator++)
   {

      const Aws::Key& key = iterator->first;
      if (station_id != key.station_id) { continue; }

      const Dtime& dtime = key.dtime;
      if (!time_span.match (dtime)) { continue; }

      const Aws::Obs& obs = iterator->second;
      aws_repository.insert (key, obs);

   }

   return aws_repository_ptr;

}

Domain_1D
Aws::Repository::get_temperature_domain () const
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
Aws::Repository::get_dew_point_domain () const
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
Aws::Repository::get_wind_speed_domain () const
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
Aws::Repository::get_mslp_domain () const
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
Aws::Repository::get_station_p_domain () const
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

Model::Varname::Varname (const Dstring& str)
   : Dstring (str)
{
}

Model::Varname::Varname (const Model::Varname& varname)
   : Dstring (varname)
{
}

Dstring
Model::Varname::get_string () const
{
   return *this;
}

void
Model::File_Path_Map::insert (const Varname& varname,
                              const Dstring& file_path)
{
   map<Varname, Dstring>::insert (make_pair (varname, file_path));
}

void
Model::Terrain::Stage::acquire_ij (size_t& i,
                                   size_t& j,
                                   const Lat_Long& lat_long) const
{
   const Real& latitude = lat_long.latitude;
   const Real& longitude = lat_long.longitude;
   i = Grid_nD::get_nearest_node (tuple_latitude, GSL_NAN, latitude);
   j = Grid_nD::get_nearest_node (tuple_longitude, GSL_NAN, longitude);
}

Model::Terrain::Stage::Stage (const Model::Terrain::Stage& stage)
   : model (stage.model),
     twiin::Stage (stage)
{
}

Model::Terrain::Stage::Stage (const Model& model,
                              const twiin::Stage& stage)
   : model (model),
     twiin::Stage (stage)
{
}

Model::Terrain::Stage::Stage (const Model& model,
                              const twiin::Stage& stage,
                              const File_Path_Map& file_path_map)
   : model (model),
     twiin::Stage (stage)
{
   init (file_path_map);
}

void
Model::Terrain::Stage::init (const File_Path_Map& file_path_map)
{

   int ret;
   int varid;

   for (auto iterator = file_path_map.begin ();
        iterator != file_path_map.end (); iterator++)
   {

      const Model::Varname& varname = iterator->first;
      const Dstring& file_path = iterator->second;

      nc_file_ptr_map.insert (make_pair (varname, new Nc_File (file_path)));
      const Nc_File& nc_file = *(nc_file_ptr_map.at (varname));
      const Integer nc_id = nc_file.get_nc_id ();

      const bool first = (iterator == file_path_map.begin ());
      if (first)
      {
         this->tuple_latitude = nc_file.get_coordinate_tuple ("latitude");
         this->tuple_longitude = nc_file.get_coordinate_tuple ("longitude");
      }

      const Dstring& nc_varname = Model::get_nc_varname (varname);

      ret = nc_inq_varid (nc_id, nc_varname.get_string ().c_str (), &varid);
      if (ret != NC_NOERR) { throw Exception ("nc_inq_varid " + nc_varname); }
      varid_map[varname] = varid;

   }

}

Model::Terrain::Stage::~Stage ()
{
}

Domain_2D
Model::Terrain::Stage::get_domain_2d () const
{
   return Domain_2D (tuple_latitude.front (), tuple_latitude.back (),
                     tuple_longitude.front (), tuple_longitude.back ());
}

bool
Model::Terrain::Stage::out_of_bounds (const Lat_Long& lat_long) const
{
   return lat_long.latitude < tuple_latitude.front () ||
          lat_long.latitude > tuple_latitude.back () ||
          lat_long.longitude < tuple_longitude.front () ||
          lat_long.longitude > tuple_longitude.back ();
}

pair<Real, Real>
Model::Terrain::Stage::get_grid_size (const size_t& i,
                                      const size_t& j) const
{

   const Integer n = tuple_latitude.size ();
   const Integer m = tuple_longitude.size ();
   const Real inf = GSL_POSINF;

   const Tuple& tuple_lat = tuple_latitude;
   const Tuple& tuple_long = tuple_longitude;

   const Real p_dlat  = i <= 0   ? inf : tuple_lat [i]    - tuple_lat[i-1];
   const Real n_dlat  = i >= n-1 ? inf : tuple_lat [i+1]  - tuple_lat[i];
   const Real p_dlong = j <= 0   ? inf : tuple_long [j]   - tuple_long[j-1];
   const Real n_dlong = j >= m-1 ? inf : tuple_long [j+1] - tuple_long[j];

   const Real d_lat = std::min (p_dlat, n_dlat);
   const Real d_long = std::min (p_dlong, n_dlong);

   return make_pair (d_lat, d_long);

}

pair<Real, Real>
Model::Terrain::Stage::get_grid_size (const Lat_Long& lat_long) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return get_grid_size (i, j);
}

Real
Model::Terrain::Stage::evaluate (const Varname& varname,
                                 const size_t& i,
                                 const size_t& j) const
{

   float datum;
   size_t index[] = { 0, 0, i, j };

   const Nc_File& nc_file = *(nc_file_ptr_map.at (varname));
   const Integer ncid = nc_file.get_nc_id ();
   const Integer varid = varid_map.at (varname);
   int ret = nc_get_var1 (ncid, varid, index, &datum);
   if (ret != NC_NOERR) { throw Exception ("nc_get_var1 orog"); }
   return Real (datum);

}

Real
Model::Terrain::Stage::evaluate (const Varname& varname,
                                 const Lat_Long& lat_long) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate (varname, i, j);
}

Real
Model::Terrain::Stage::get_topography (const Lat_Long& lat_long) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return get_topography (i, j);
}

Real
Model::Terrain::Stage::get_topography (const size_t i,
                                       const size_t j) const
{
   const Model::Terrain::Stage& terrain_stage = model.terrain.get_stage (*this);
   return terrain_stage.evaluate (Varname ("orog"), i, j);
}

const Model::Terrain::Stage&
Model::Terrain::get_stage (const twiin::Stage& stage) const
{
   return stage_map.at (stage);
}

Model::Terrain::Stage&
Model::Terrain::get_stage (const twiin::Stage& stage)
{
   return stage_map.at (stage);
}

Model::Terrain::Terrain (const Model& model)
   : model (model)
{
}

void
Model::Terrain::init (const Tokens& stage_tokens)
{

   vector<twiin::Stage> twiin_stage_vector;

   for (auto iterator = stage_tokens.begin ();
        iterator != stage_tokens.end (); iterator++)
   {
      twiin::Stage twiin_stage (*(iterator));
      Model::Terrain::Stage stage (model, twiin_stage);
      stage_map.insert (make_pair (twiin_stage, stage));
   }

}

void
Model::Terrain::init_stage (const twiin::Stage& twiin_stage,
                            const Model::File_Path_Map& file_path_map)
{
   Model::Terrain::Stage& stage = stage_map.at (twiin_stage);
   stage.init (file_path_map);
}

void
Model::Surface::Stage::fill_valid_time_set ()
{

   int ret, dim_id, varid;
   nc_type var_type;
   size_t n;

   if (nc_file_ptr_map.size () == 0) { return; }
   const Nc_File& nc_file = *(nc_file_ptr_map.begin ()->second);
   const Integer nc_id = nc_file.get_nc_id ();
   char attribute_char[128];

   ret = nc_inq_varid (nc_id, "t", &varid);
   if (ret != NC_NOERR) { throw Exception ("nc_inq_varid t"); }

   ret = nc_get_att_text (nc_id, varid, "time_origin", attribute_char);
   if (ret != NC_NOERR) { throw Exception ("nc_get_att_text"); }

   Dstring str (attribute_char);
   const Integer yyyy = stoi (str.substr (7, 4));
   const Integer mm = 10;
   const Integer dd = stoi (str.substr (0, 2));
   const Integer HH = stoi (str.substr (12, 2));
   const Integer MM = stoi (str.substr (15, 2));
   const Integer SS = stoi (str.substr (18, 2));
   const Dtime base_time (yyyy, mm, dd, HH, MM, SS);

   ret = nc_inq_dimid (nc_id, "t", &dim_id);
   ret = nc_inq_dimlen (nc_id, varid, &n);
   ret = nc_inq_vartype (nc_id, varid, &var_type);

   Integer datum_size = Nc_File::get_datum_size (var_type);
   uint8_t* array = new uint8_t[n * datum_size];

   ret = nc_get_var (nc_id, varid, array);
   if (ret != NC_NOERR) { throw Exception ("nc_get_var t"); }

   for (Integer i = 0; i < n; i++)
   {
      const void* pointer = array + (i * datum_size);
      const Real forecast_hour = Nc_File::get_datum (pointer, var_type) * 24;
      const Dtime dtime (base_time.t + forecast_hour);
      valid_time_set.insert (dtime);
   }

   delete[] array;


}

Model::Surface::Stage::Stage (const Model& model,
                              const twiin::Stage& stage)
   : Model::Terrain::Stage (model, stage)
{
}

void
Model::Surface::Stage::init (const File_Path_Map& file_path_map)
{
   Model::Terrain::Stage::init (file_path_map);
   fill_valid_time_set ();
}

const set<Dtime>&
Model::Surface::Stage::get_valid_time_set () const
{
   return valid_time_set;
}
 
size_t
Model::Surface::Stage::get_l (const Dtime& dtime) const
{
   const auto begin = valid_time_set.begin ();
   return std::distance (begin, valid_time_set.find (dtime));
}

Real
Model::Surface::Stage::evaluate_h_advection (const Met_Element& met_element,
                                             const Lat_Long& lat_long,
                                             const size_t l) const
{

   size_t i, j;
   acquire_ij (i, j, lat_long);

   const Real u = evaluate (U, i, j, l);
   const Real v = evaluate (V, i, j, l);
   const Real ddx = evaluate_dx (met_element, i, j, l);
   const Real ddy = evaluate_dy (met_element, i, j, l);

   return -u * ddx - v * ddy;

}

Real
Model::Surface::Stage::evaluate (const Met_Element& met_element,
                                 const Lat_Long& lat_long,
                                 const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate (met_element, i, j, l);
}

Real
Model::Surface::Stage::evaluate (const Met_Element& met_element,
                                 const size_t i,
                                 const size_t j,
                                 const size_t l) const
{

   int ret;
   float datum;

   switch (met_element)
   {

      case W:
      {
         datum = 0;
         break;
      };

      case WIND_SPEED:
      {
         const Real u = evaluate_raw ("xwind", i, j, l);
         const Real v = evaluate_raw ("ywind", i, j, l);
         datum = sqrt (u*u + v*v);
         break;
      };

      case WIND_DIRECTION:
      {
         const Real u = evaluate_raw ("xwind", i, j, l);
         const Real v = evaluate_raw ("ywind", i, j, l);
         datum = Wind (u, v).get_direction ();
         break;
      };

      case TD:
      {
         const Real q = evaluate_raw ("qsair", i, j, l);
         const Real mslp = evaluate_raw ("mslp", i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         const Real r = q / (1 - q);
         datum = Thermo_Point::p_r_s (surface_p, r).get_t () + K;
         break;
      };

      case RH:
      {
         const Real t = evaluate_raw ("temp", i, j, l);
         const Real q = evaluate_raw ("qsair", i, j, l);
         const Real mslp = evaluate_raw ("mslp", i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         const Real r = q / (1 - q);
         const Real t_d = Thermo_Point::p_r_s (surface_p, r).get_t () + K;
         datum = Moisture::get_rh (t - K, t_d - K, WATER);
         break;
      };

      case ZETA:
      {
         if (i <= 0 || i >= tuple_latitude.size () - 1 ||
             j <= 0 || j >= tuple_longitude.size () - 1) { return GSL_NAN; }
         const Real upper_u = evaluate (U, i+1, j, l);
         const Real lower_u = evaluate (U, i-1, j, l);
         const Real upper_v = evaluate (V, i, j+1, l);
         const Real lower_v = evaluate (V, i, j-1, l);
         const Real du = upper_u - lower_u;
         const Real dv = upper_v - lower_v;
         const Real longitude = tuple_latitude[j];
         const Real c = cos (tuple_latitude[i] * DEGREE_TO_RADIAN);
         const Real dlatitude = tuple_latitude[i+1] - tuple_latitude[i-1];
         const Real dlongitude = tuple_longitude[j+1] - tuple_longitude[j-1];
         const Real dv_dx = (dv / dlongitude) / (LATITUDE_LENGTH * c);
         const Real du_dy = (du / dlatitude) / (LATITUDE_LENGTH);
         datum = dv_dx - du_dy;
         break;
      };

      case THETA:
      {
         const Real t = evaluate_raw ("temp", i, j, l);
         const Real mslp = evaluate_raw ("mslp", i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         datum = Thermo_Point::t_p (t - K, surface_p).get_theta () + K;
         break;
      };

      case THETA_E:
      {
         typedef Thermo_Point Tp;
         const Real t = evaluate_raw ("temp", i, j, l);
         const Real q = evaluate_raw ("qsair", i, j, l);
         const Real mslp = evaluate_raw ("mslp", i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         const Real r = q / (1 - q);
         const Real t_d = Thermo_Point::p_r_s (surface_p, r).get_t () + K;
         datum = Tp::normand (t - K, t_d - K, surface_p).get_theta_e () + K;
         break;
      };

      case THETA_V:
      {
         typedef Thermo_Point Tp;
         const Real t = evaluate_raw ("temp", i, j, l);
         const Real q = evaluate_raw ("qsair", i, j, l);
         const Real mslp = evaluate_raw ("mslp", i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         const Real r = q / (1 - q);
         const Real t_v = Moisture::get_t_v (t + K, r) + K;
         datum = Thermo_Point::t_p (t_v - K, surface_p).get_theta () + K;
         break;
      };

      case RHO:
      {
         const Real t = evaluate_raw ("temp", i, j, l);
         const Real mslp = evaluate_raw ("mslp", i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         datum = surface_p / (R_d * t);
         break;
      };

      case FFDI:
      {
         const Real t = evaluate_raw ("temp", i, j, l);
         const Real q = evaluate_raw ("qsair", i, j, l);
         const Real mslp = evaluate_raw ("mslp", i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         const Real r = q / (1 - q);
         const Real t_d = Thermo_Point::p_r_s (surface_p, r).get_t () + K;
         const Real rh = Moisture::get_rh (t - K, t_d - K, WATER);
         const Real u = evaluate_raw ("xwind", i, j, l);
         const Real v = evaluate_raw ("ywind", i, j, l);
         const Real speed = sqrt (u*u + v*v);
         datum = Fire::get_ffdi (t - K, rh * 100, speed * 3.6);
         break;
      };

      default:
      {

         Varname varname ("");

         switch (met_element)
         {
            case T:           { varname = Dstring ("temp"); break;    }
            case Q:           { varname = Dstring ("qsair"); break;   }
            case MSLP:        { varname = Dstring ("mslp"); break;    }
            case PRECIP_RATE: { varname = Dstring ("prcp8p5"); break; }
            case U:           { varname = Dstring ("xwind"); break;   }
            case V:           { varname = Dstring ("ywind"); break;   }
         }

         datum = evaluate_raw (varname, i, j, l);
         break;

      }

   }

   return Real (datum);

}

Real
Model::Surface::Stage::evaluate_dx (const Met_Element& met_element,
                                    const Lat_Long& lat_long,
                                    const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dx (met_element, i, j, l);
}

Real
Model::Surface::Stage::evaluate_dx (const Met_Element& met_element,
                                    const size_t i,
                                    const size_t j,
                                    const size_t l) const
{

   const Integer m = tuple_longitude.size ();
   if (j < 0 || j >= m) { return GSL_NAN; }

   const Integer upper_j = (j == m-1 ? j : j+1);
   const Integer lower_j = (j == 0   ? j : j-1);
   const Real upper = evaluate (met_element, i, upper_j, l);
   const Real lower = evaluate (met_element, i, lower_j, l);
   const Real delta = upper - lower;
   const Real c = cos (tuple_latitude[i] * DEGREE_TO_RADIAN);
   const Real dlongitude = tuple_longitude[upper_j] - tuple_longitude[lower_j];
   return (delta / dlongitude) / (LATITUDE_LENGTH * c);

}

Real
Model::Surface::Stage::evaluate_dy (const Met_Element& met_element,
                                    const Lat_Long& lat_long,
                                    const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dy (met_element, i, j, l);
}

Real
Model::Surface::Stage::evaluate_dy (const Met_Element& met_element,
                                    const size_t i,
                                    const size_t j,
                                    const size_t l) const
{

   const Integer n = tuple_latitude.size ();
   if (i < 0 || i >= n) { return GSL_NAN; }

   const Integer upper_i = (i == n-1 ? i : i+1);
   const Integer lower_i = (i == 0   ? i : i-1);
   const Real upper = evaluate (met_element, upper_i, j, l);
   const Real lower = evaluate (met_element, lower_i, j, l);
   const Real delta = upper - lower;
   const Real dlongitude = tuple_latitude[upper_i] - tuple_latitude[lower_i];
   return (delta / dlongitude) / (LATITUDE_LENGTH);

}

Real
Model::Surface::Stage::evaluate_raw (const Dstring& varname,
                                     const size_t i,
                                     const size_t j,
                                     const size_t l) const
{

   int ret;
   float datum;
   const Nc_File& nc_file = *(nc_file_ptr_map.at (varname));
   const Integer nc_id = nc_file.get_nc_id ();
   const Integer varid = varid_map.at (varname);
   const size_t index[] = { l, 0, i, j };

   ret = nc_get_var1 (nc_id, varid, index, &datum);
   if (ret != NC_NOERR) { throw Exception ("nc_get_var1"); }

   return Real (datum);

}

Aws::Obs
Model::Surface::Stage::get_aws_obs (const Lat_Long& lat_long,
                                    const Dtime& dtime) const
{

   size_t i, j;
   acquire_ij (i, j, lat_long);
   const Integer l = get_l (dtime);

   const Real t = evaluate (T, i, j, l);
   const Real t_d = evaluate (TD, i, j, l);
   const Real u = evaluate (U, i, j, l);
   const Real v = evaluate (V, i, j, l);

   const Real mslp = evaluate (MSLP, i, j, l);
   const Real topography = get_topography (i, j);
   const Real surface_p = mslp - 11.76 * topography;

   const Wind wind (u, v);
   const Real wind_direction = wind.get_direction ();
   const Real wind_speed = wind.get_speed ();

   return Aws::Obs (t, t_d, wind_direction, wind_speed, surface_p, mslp);   

}

void
Model::Surface::Stage::fill_sounding (Sounding& sounding,
                                      const Lat_Long& lat_long,
                                      const Dtime& dtime) const
{

   size_t i, j;
   acquire_ij (i, j, lat_long);
   const Integer l = get_l (dtime);

   const Real t = evaluate (T, i, j, l);
   const Real t_d = evaluate (TD, i, j, l);
   const Real u = evaluate (U, i, j, l);
   const Real v = evaluate (V, i, j, l);

   const Real topography = get_topography (i, j);
   const Real mslp = evaluate (MSLP, i, j, l);
   const Real surface_p = mslp - 11.76 * topography;

   sounding.get_t_line ().add (surface_p, t - K);
   sounding.get_t_d_line ().add (surface_p, t_d - K);
   sounding.get_wind_profile ().add (surface_p, Wind (u, v));
   sounding.get_height_profile ().add (surface_p, 0);

}

Color
Model::Surface::Stage::get_color (const Product& product,
                                  const Lat_Long& lat_long,
                                  const size_t l) const
{

   switch (product.enumeration)
   {

      case Product::WIND:
      {
         const Real u = evaluate (U, lat_long, l);
         const Real v = evaluate (V, lat_long, l);
         const Real speed = sqrt (u*u + v*v);
         const Real theta = atan2 (-u, -v);
         const Real hue = (theta < 0 ? theta + 2*M_PI : theta)/ (2*M_PI);
         const Real brightness = std::min (speed / 15, 1.0);
         return Color::hsb (hue, 0.8, brightness);
      }

      case Product::Q_ADVECTION:
      case Product::Q_H_ADVECTION:
      {
         const Real datum = evaluate_h_advection (Q, lat_long, l);
         return Display::get_color (product, datum);
      }

      default:
      {
         const Met_Element met_element = product.get_met_element ();
         const Real datum = evaluate (met_element, lat_long, l);
         return Display::get_color (product, datum);
      }

   }

}

const Model::Surface::Stage&
Model::Surface::get_stage (const twiin::Stage& stage) const
{
   return (const Model::Surface::Stage&)(stage_map.at (stage));
}

Model::Surface::Stage&
Model::Surface::get_stage (const twiin::Stage& stage)
{
   return (Model::Surface::Stage&)(stage_map.at (stage));
}

Model::Surface::Surface (const Model& model)
   : Model::Terrain (model)
{
}

void
Model::Surface::init (const Tokens& stage_tokens)
{

   vector<twiin::Stage> twiin_stage_vector;

   for (auto iterator = stage_tokens.begin ();
        iterator != stage_tokens.end (); iterator++)
   {
      twiin::Stage twiin_stage (*(iterator));
      Model::Surface::Stage stage (model, twiin_stage);
      stage_map.insert (make_pair (twiin_stage, stage));
   }

}

void
Model::Surface::init_stage (const twiin::Stage& twiin_stage,
                            const Model::File_Path_Map& file_path_map)
{
   Model::Surface::Stage& stage = stage_map.at (twiin_stage);
   stage.init (file_path_map);
}

void
Model::Uppers::Stage::acquire_k (size_t& k,
                                 const Met_Element& met_element,
                                 const size_t i,
                                 const size_t j,
                                 const Level& level) const
{

   switch (level.type)
   {

      case Level::MODEL:
      {
         k = size_t (round (level.value));
         return;
      }

      case Level::HEIGHT:
      {

         const Real z = level.value;
         const Real topography = get_topography (i, j);
         if (z < topography) { k = -1; return; }

         const bool is_w = (met_element == W);
         const bool is_theta = Model::is_theta (met_element);
         const Tuple& A = model.vertical_coefficients.get_A (is_theta);
         const Tuple& B = model.vertical_coefficients.get_B (is_theta);
         if (z > A.back ()) { k = -1; return; }

         const bool surface = (z < 0);
         k = surface ? -1 : model.get_k (z, topography, A, B);
         break;

      }

      default:
      {
         k = -1;
         break;
      };

   }

}

Model::Uppers::Stage::Stage (const Model& model,
                             const twiin::Stage& stage)
   : Model::Surface::Stage (model, stage)
{
}

Real
Model::Uppers::Stage::evaluate_normal_speed (const Real azimuth,
                                             const Lat_Long& lat_long,
                                             const Level& level,
                                             const size_t l) const
{

   size_t i, j, k;
   acquire_ij (i, j, lat_long);
   acquire_k (k, U, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   const Real u = evaluate (U, i, j, k, l);
   const Real v = evaluate (V, i, j, k, l);

   const Real theta = azimuth * DEGREE_TO_RADIAN;
   return v * sin (theta) - u * cos (theta);

}

Real
Model::Uppers::Stage::evaluate_along_speed (const Real azimuth,
                                            const Lat_Long& lat_long,
                                            const Level& level,
                                            const size_t l,
                                            const Real u_bg) const
{

   size_t i, j, k;
   acquire_ij (i, j, lat_long);
   acquire_k (k, U, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   const Real u = evaluate (U, i, j, k, l);
   const Real v = evaluate (V, i, j, k, l);

   const Real theta = azimuth * DEGREE_TO_RADIAN;
   return u * sin (theta) + v * cos (theta) - u_bg;

}

Real
Model::Uppers::Stage::evaluate_scorer (const Real azimuth,
                                       const Lat_Long& lat_long,
                                       const Level& level,
                                       const size_t l,
                                       const Real u_bg) const
{

   size_t i, j, k_rho, k_theta;
   acquire_ij (i, j, lat_long);

   acquire_k (k_rho, U, i, j, level);
   if (k_rho <= 0 || k_rho >= 69) { return GSL_NAN; }

   acquire_k (k_theta, THETA, i, j, level);
   if (k_theta <= 0 || k_theta >= 69) { return GSL_NAN; }

   const Real topography = get_topography (i, j);
   const Tuple& A_rho = model.vertical_coefficients.get_A_rho ();
   const Tuple& B_rho = model.vertical_coefficients.get_B_rho ();
   const Tuple& A_theta = model.vertical_coefficients.get_A_theta ();
   const Tuple& B_theta = model.vertical_coefficients.get_B_theta ();

   const Integer k_rho_0 = k_rho - 1;
   const Integer k_rho_1 = k_rho;
   const Integer k_rho_2 = k_rho + 1;

   const Integer k_theta_0 = k_theta - 1;
   const Integer k_theta_1 = k_theta;
   const Integer k_theta_2 = k_theta + 1;

   const Real theta_0 = evaluate (THETA, i, j, k_theta_0, l);
   const Real theta_1 = evaluate (THETA, i, j, k_theta_1, l);
   const Real theta_2 = evaluate (THETA, i, j, k_theta_2, l);

   const Real u_0 = evaluate (U, i, j, k_rho_0, l);
   const Real u_1 = evaluate (U, i, j, k_rho_1, l);
   const Real u_2 = evaluate (U, i, j, k_rho_2, l);

   const Real v_0 = evaluate (V, i, j, k_rho_0, l);
   const Real v_1 = evaluate (V, i, j, k_rho_1, l);
   const Real v_2 = evaluate (V, i, j, k_rho_2, l);

   const Real theta = azimuth * DEGREE_TO_RADIAN;
   const Real along_0 = u_0 * sin (theta) + v_0 * cos (theta) - u_bg;
   const Real along_1 = u_1 * sin (theta) + v_1 * cos (theta) - u_bg;
   const Real along_2 = u_2 * sin (theta) + v_2 * cos (theta) - u_bg;

   const Real z_theta_0 = model.get_z (k_theta_0, topography, A_theta, B_theta);
   const Real z_theta_1 = model.get_z (k_theta_1, topography, A_theta, B_theta);
   const Real z_theta_2 = model.get_z (k_theta_2, topography, A_theta, B_theta);

   const Real z_rho_0 = model.get_z (k_rho_0, topography, A_rho, B_rho);
   const Real z_rho_1 = model.get_z (k_rho_1, topography, A_rho, B_rho);
   const Real z_rho_2 = model.get_z (k_rho_2, topography, A_rho, B_rho);

   typedef Differentiation D;
   const Real dtheta_dz = D::d_1 (theta_0, theta_1,
      theta_2, z_theta_0, z_theta_1, z_theta_2);

   const Real A = (g / theta_1 * dtheta_dz ) / (along_1 * along_1);
   const Real B = -D::d2 (along_0, along_1, along_2,
      z_rho_0, z_rho_1, z_rho_2) / along_1;

   return A + B;

}

Real
Model::Uppers::Stage::evaluate_brunt_vaisala (const Lat_Long& lat_long,
                                              const Level& level,
                                              const size_t l) const
{

   size_t i, j, k;
   acquire_ij (i, j, lat_long);
   acquire_k (k, THETA, i, j, level);
   if (k <= 0 || k >= 69) { return GSL_NAN; }

   const Real topography = get_topography (i, j);
   const Tuple& A_theta = model.vertical_coefficients.get_A_theta ();
   const Tuple& B_theta = model.vertical_coefficients.get_B_theta ();

   const Integer k_0 = k - 1;
   const Integer k_1 = k;
   const Integer k_2 = k + 1;

   const Real theta_0 = evaluate (THETA, i, j, k_0, l);
   const Real theta_1 = evaluate (THETA, i, j, k_1, l);
   const Real theta_2 = evaluate (THETA, i, j, k_2, l);

   const Real z_0 = model.get_z (k_0, topography, A_theta, B_theta);
   const Real z_1 = model.get_z (k_1, topography, A_theta, B_theta);
   const Real z_2 = model.get_z (k_2, topography, A_theta, B_theta);

   typedef Differentiation D;
   const Real dtheta_dz = D::d_1 (theta_0, theta_1, theta_2, z_0, z_1, z_2);

   if (dtheta_dz <= 0) { return GSL_NAN; }
   else { return sqrt (g / theta_1 * dtheta_dz); }

}

Real
Model::Uppers::Stage::evaluate_h_advection (const Met_Element& met_element,
                                            const Lat_Long& lat_long,
                                            const Level& level,
                                            const size_t l) const
{

   size_t i, j;
   acquire_ij (i, j, lat_long);

   size_t k, k_rho;
   acquire_k (k_rho, U, i, j, level);
   acquire_k (k, met_element, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   const Real u = evaluate (U, i, j, k_rho, l);
   const Real v = evaluate (V, i, j, k_rho, l);
   const Real ddx = evaluate_dx (met_element, lat_long, level, l);
   const Real ddy = evaluate_dy (met_element, lat_long, level, l);

   const Real datum = -u * ddx - v * ddy;
   return -u * ddx - v * ddy;

}

Real
Model::Uppers::Stage::evaluate_v_advection (const Met_Element& met_element,
                                            const Lat_Long& lat_long,
                                            const Level& level,
                                            const size_t l) const
{

   size_t i, j, k, k_w;
   acquire_ij (i, j, lat_long);

   acquire_k (k, met_element, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   acquire_k (k_w, W, i, j, level);
   if (k_w < 0 || k_w > 69) { return GSL_NAN; }

   const Real w = evaluate (W, i, j, k_w, l);
   const Real ddz = evaluate_dz (met_element, i, j, k, l);

   return -w * ddz;

}

Real
Model::Uppers::Stage::evaluate_s_advection (const Met_Element& met_element,
                                            const Real azimuth,
                                            const Lat_Long& lat_long,
                                            const Level& level,
                                            const size_t l,
                                            const Real u_bg) const
{

   size_t i, j, k, k_rho;
   acquire_ij (i, j, lat_long);

   acquire_k (k, met_element, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   acquire_k (k_rho, U, i, j, level);
   if (k_rho < 0 || k_rho > 69) { return GSL_NAN; }

   const Real theta = azimuth * DEGREE_TO_RADIAN;

   const Real u = evaluate (U, i, j, k_rho, l);
   const Real v = evaluate (V, i, j, k_rho, l);
   const Real s = u * sin (theta) + v * cos (theta) - u_bg;

   const Real ddx = evaluate_dx (met_element, lat_long, level, l);
   const Real ddy = evaluate_dy (met_element, lat_long, level, l);
   const Real dds = ddx * sin (theta) + ddy * cos (theta);

   return -s * dds;

}

Real
Model::Uppers::Stage::evaluate_n_advection (const Met_Element& met_element,
                                            const Real azimuth,
                                            const Lat_Long& lat_long,
                                            const Level& level,
                                            const size_t l) const
{

   size_t i, j, k, k_rho;
   acquire_ij (i, j, lat_long);

   acquire_k (k, met_element, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   acquire_k (k_rho, U, i, j, level);
   if (k_rho < 0 || k_rho > 69) { return GSL_NAN; }

   const Real theta = azimuth * DEGREE_TO_RADIAN;

   const Real u = evaluate (U, i, j, k_rho, l);
   const Real v = evaluate (V, i, j, k_rho, l);
   const Real n = v * sin (theta) - u * cos (theta);

   const Real ddx = evaluate_dx (met_element, lat_long, level, l);
   const Real ddy = evaluate_dy (met_element, lat_long, level, l);
   const Real ddn = ddy * sin (theta) - ddx * cos (theta);

   return -n * ddn;

}

Real
Model::Uppers::Stage::evaluate (const Met_Element& met_element,
                                const Lat_Long& lat_long,
                                const Level& level,
                                const size_t l) const
{

   size_t i, j, k;
   acquire_ij (i, j, lat_long);
   acquire_k (k, met_element, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   const bool is_w = (met_element == W);
   return evaluate (met_element, i, j, (is_w ? k + 1 : k), l);

}

Real
Model::Uppers::Stage::evaluate (const Met_Element& met_element,
                                const Lat_Long& lat_long,
                                const size_t k,
                                const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate (met_element, i, j, k, l);
}

Real
Model::Uppers::Stage::evaluate (const Met_Element& met_element,
                                const size_t i,
                                const size_t j,
                                const size_t k,
                                const size_t l) const
{

   int ret;
   float datum;

   switch (met_element)
   {

      case WIND_SPEED:
      {
         const Real u = evaluate (U, i, j, k, l);
         const Real v = evaluate (V, i, j, k, l);
         datum = sqrt (u*u + v*v);
         break;
      };

      case WIND_DIRECTION:
      {
         const Real u = evaluate (U, i, j, k, l);
         const Real v = evaluate (V, i, j, k, l);
         datum = Wind (u, v).get_direction ();
         break;
      };

      case T:
      {
         const Real theta = evaluate_raw ("ml_theta", i, j, k, l);
         const Real p = evaluate_raw ("ml_ptheta", i, j, k, l);
         datum = Thermo_Point::theta_p (theta - K, p).get_t () + K;
         break;
      };

      case TD:
      {
         const Real q = evaluate (Q, i, j, k, l);
         const Real p = evaluate_raw ("ml_ptheta", i, j, k, l);
         const Real r = q / (1 - q);
         datum = Thermo_Point::p_r_s (p, r).get_t () + K;
         break;
      };

      case RH:
      {
         const Real t = evaluate (T, i, j, k, l);
         const Real q = evaluate (Q, i, j, k, l);
         const Real p = evaluate_raw ("ml_ptheta", i, j, k, l);
         const Real r = q / (1 - q);
         const Real t_d = Thermo_Point::p_r_s (p, r).get_t () + K;
         datum = Moisture::get_rh (t - K, t_d - K, WATER);
         break;
      };

      case RHO:
      {
         const Real theta = evaluate_raw ("ml_theta", i, j, k, l);
         const Real p = evaluate_raw ("ml_ptheta", i, j, k, l);
         const Real t = Thermo_Point::theta_p (theta - K, p).get_t () + K;
         datum = p / (R_d * t);
         break;
      };

      case RELATIVE_VORTICITY:
      {
         if (i <= 0 || i >= tuple_latitude.size () - 1 ||
             j <= 0 || j >= tuple_longitude.size () - 1) { return GSL_NAN; }
         const Real upper_u = evaluate (U, i+1, j, k, l);
         const Real lower_u = evaluate (U, i-1, j, k, l);
         const Real upper_v = evaluate (V, i, j+1, k, l);
         const Real lower_v = evaluate (V, i, j-1, k, l);
         const Real du = upper_u - lower_u;
         const Real dv = upper_v - lower_v;
         const Real c = cos (tuple_latitude[i] * DEGREE_TO_RADIAN);
         const Real dlatitude = tuple_latitude[i+1] - tuple_latitude[i-1];
         const Real dlongitude = tuple_longitude[j+1] - tuple_longitude[j-1];
         const Real dv_dx = (dv / dlongitude) / (LATITUDE_LENGTH * c);
         const Real du_dy = (du / dlatitude) / (LATITUDE_LENGTH);
         datum = dv_dx - du_dy;
         break;
      };

      case THETA_E:
      {
         typedef Thermo_Point Tp;
         const Real p = evaluate_raw ("ml_ptheta", i, j, k, l);
         const Real theta = evaluate_raw ("ml_theta", i, j, k, l);
         const Real q = evaluate_raw ("ml_spechum", i, j, k, l);
         const Real t = Thermo_Point::theta_p (theta - K, p).get_t () + K;
         const Real r = q / (1 - q);
         const Real t_d = Thermo_Point::p_r_s (p, r).get_t () + K;
         datum = Tp::normand (t - K, t_d - K, p).get_theta_e () + K;
         break;
      };

      case THETA_V:
      {
         typedef Thermo_Point Tp;
         const Real p = evaluate_raw ("ml_ptheta", i, j, k, l);
         const Real theta = evaluate_raw ("ml_theta", i, j, k, l);
         const Real q = evaluate_raw ("ml_spechum", i, j, k, l);
         const Real t = Thermo_Point::theta_p (theta - K, p).get_t () + K;
         const Real r = q / (1 - q);
         const Real t_v = Moisture::get_t_v (t + K, r) + K;
         datum = Thermo_Point::t_p (t_v - K, p).get_theta () + K;
         break;
      };

      default:
      {

         Varname varname ("");

         switch (met_element)
         {
            case U:       { varname = Dstring ("ml_xwind"); break; } 
            case V:       { varname = Dstring ("ml_ywind"); break; } 
            case W:       { varname = Dstring ("ml_zwind"); break; } 
            case THETA:   { varname = Dstring ("ml_theta"); break; } 
            case Q:       { varname = Dstring ("ml_spechum"); break; } 
            case P_THETA: { varname = Dstring ("ml_ptheta"); break; } 
            case P_RHO:   { varname = Dstring ("ml_prho"); break; } 
         }

         datum = evaluate_raw (varname, i, j, k, l);
         break;

      }

   }

   return Real (datum);

}

Real
Model::Uppers::Stage::evaluate_dx (const Met_Element& met_element,
                                   const Lat_Long& lat_long,
                                   const Level& level,
                                   const size_t l) const
{

   size_t i, j;
   size_t upper_k, lower_k;
   acquire_ij (i, j, lat_long);

   const Integer m = tuple_longitude.size ();
   if (j < 0 || j >= m) { return GSL_NAN; }

   const Integer upper_j = (j == m-1 ? j : j+1);
   const Integer lower_j = (j == 0   ? j : j-1);

   acquire_k (upper_k, met_element, i, upper_j, level);
   if (upper_k < 0 || upper_k > 69) { return GSL_NAN; }

   acquire_k (lower_k, met_element, i, lower_j, level);
   if (lower_k < 0 || lower_k > 69) { return GSL_NAN; }

   const Real upper = evaluate (met_element, i, upper_j, upper_k, l);
   const Real lower = evaluate (met_element, i, lower_j, upper_k, l);
   const Real delta = upper - lower;
   const Real c = cos (tuple_latitude[i] * DEGREE_TO_RADIAN);
   const Real dlongitude = tuple_longitude[upper_j] - tuple_longitude[lower_j];
   return (delta / dlongitude) / (LATITUDE_LENGTH * c);

}

Real
Model::Uppers::Stage::evaluate_dx (const Met_Element& met_element,
                                   const Lat_Long& lat_long,
                                   const size_t k,
                                   const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dx (met_element, i, j, k, l);
}

Real
Model::Uppers::Stage::evaluate_dx (const Met_Element& met_element,
                                   const size_t i,
                                   const size_t j,
                                   const size_t k,
                                   const size_t l) const
{

   const Integer m = tuple_longitude.size ();
   if (j < 0 || j >= m) { return GSL_NAN; }

   const Integer upper_j = (j == m-1 ? j : j+1);
   const Integer lower_j = (j == 0   ? j : j-1);
   const Real upper = evaluate (met_element, i, upper_j, k, l);
   const Real lower = evaluate (met_element, i, lower_j, k, l);
   const Real delta = upper - lower;
   const Real c = cos (tuple_latitude[i] * DEGREE_TO_RADIAN);
   const Real dlongitude = tuple_longitude[upper_j] - tuple_longitude[lower_j];
   return (delta / dlongitude) / (LATITUDE_LENGTH * c);

}

Real
Model::Uppers::Stage::evaluate_dy (const Met_Element& met_element,
                                   const Lat_Long& lat_long,
                                   const Level& level,
                                   const size_t l) const
{

   size_t i, j;
   size_t upper_k, lower_k;
   acquire_ij (i, j, lat_long);

   const Integer n = tuple_latitude.size ();
   if (i < 0 || i >= n) { return GSL_NAN; }

   const Integer upper_i = (i == n-1 ? i : i+1);
   const Integer lower_i = (i == 0   ? i : i-1);

   acquire_k (upper_k, met_element, upper_i, j, level);
   if (upper_k < 0 || upper_k > 69) { return GSL_NAN; }

   acquire_k (lower_k, met_element, lower_i, j, level);
   if (lower_k < 0 || lower_k > 69) { return GSL_NAN; }

   const Real upper = evaluate (met_element, upper_i, j, upper_k, l);
   const Real lower = evaluate (met_element, upper_i, j, upper_k, l);
   const Real delta = upper - lower;
   const Real c = cos (tuple_latitude[i] * DEGREE_TO_RADIAN);
   const Real dlongitude = tuple_longitude[upper_i] - tuple_longitude[lower_i];
   return (delta / dlongitude) / (LATITUDE_LENGTH * c);

}

Real
Model::Uppers::Stage::evaluate_dy (const Met_Element& met_element,
                                   const Lat_Long& lat_long,
                                   const size_t k,
                                   const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dy (met_element, i, j, k, l);
}

Real
Model::Uppers::Stage::evaluate_dy (const Met_Element& met_element,
                                   const size_t i,
                                   const size_t j,
                                   const size_t k,
                                   const size_t l) const
{

   const Integer n = tuple_latitude.size ();
   if (i < 0 || i >= n) { return GSL_NAN; }

   const Integer upper_i = (i == n-1 ? i : i+1);
   const Integer lower_i = (i == 0   ? i : i-1);
   const Real upper = evaluate (met_element, upper_i, j, k, l);
   const Real lower = evaluate (met_element, lower_i, j, k, l);
   const Real delta = upper - lower;
   const Real dlongitude = tuple_latitude[upper_i] - tuple_latitude[lower_i];
   return (delta / dlongitude) / (LATITUDE_LENGTH);

}

Real
Model::Uppers::Stage::evaluate_dz (const Met_Element& met_element,
                                   const Lat_Long& lat_long,
                                   const Level& level,
                                   const size_t l) const
{

   size_t i, j, k;
   acquire_ij (i, j, lat_long);
   acquire_k (k, met_element, i, j, level);

   return evaluate_dz (met_element, i, j, k, l);

}

Real
Model::Uppers::Stage::evaluate_dz (const Met_Element& met_element,
                                   const Lat_Long& lat_long,
                                   const size_t k,
                                   const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dy (met_element, i, j, k, l);
}

Real
Model::Uppers::Stage::evaluate_dz (const Met_Element& met_element,
                                   const size_t i,
                                   const size_t j,
                                   const size_t k,
                                   const size_t l) const
{

   const Integer n = 70;
   if (k < 1 || k >= n - 1) { return GSL_NAN; }

   const bool is_theta = Model::is_theta (met_element);
   const Tuple& A = model.vertical_coefficients.get_A (met_element);
   const Tuple& B = model.vertical_coefficients.get_B (met_element);

   const Integer k_0 = (k == 0   ? k : k-1);
   const Integer k_1 = k;
   const Integer k_2 = (k == n-1 ? k : k+1);

   const Real topography = get_topography (i, j);
   const Real z_0 = model.get_z (k_0, topography, A, B);
   const Real z_1 = model.get_z (k_1, topography, A, B);
   const Real z_2 = model.get_z (k_2, topography, A, B);

   const Real datum_0 = evaluate (met_element, i, j, k_0, l);
   const Real datum_1 = evaluate (met_element, i, j, k_1, l);
   const Real datum_2 = evaluate (met_element, i, j, k_2, l);

   return Differentiation::d_1 (datum_0, datum_1, datum_2, z_0, z_1, z_2);

}

Real
Model::Uppers::Stage::evaluate_raw (const Dstring& varname,
                                    const size_t i,
                                    const size_t j,
                                    const size_t k,
                                    const size_t l) const
{

   int ret;
   float datum;
   const Nc_File& nc_file = *(nc_file_ptr_map.at (varname));
   const Integer nc_id = nc_file.get_nc_id ();
   const Integer varid = varid_map.at (varname);
   const size_t index[] = { l, (k == -1) ? 0 : k, i, j };

   ret = nc_get_var1 (nc_id, varid, index, &datum);
   if (ret != NC_NOERR) { throw Exception ("nc_get_var1"); }

   return Real (datum);

}

void
Model::Uppers::Stage::fill_sounding (Sounding& sounding,
                                     const Lat_Long& lat_long,
                                     const Dtime& dtime) const
{

   size_t i, j;
   acquire_ij (i, j, lat_long);
   const Integer l = get_l (dtime);

   const Real topography = get_topography (i, j);
   const Tuple& A_rho = model.vertical_coefficients.get_A_rho ();
   const Tuple& B_rho = model.vertical_coefficients.get_B_rho ();
   const Tuple& A_theta = model.vertical_coefficients.get_A_theta ();
   const Tuple& B_theta = model.vertical_coefficients.get_B_theta ();

   for (Integer k = 0; k < 70; k++)
   {

      const Real t = evaluate (T, i, j, k, l);
      const Real t_d = evaluate (TD, i, j, k, l);
      const Real u = evaluate (U, i, j, k, l);
      const Real v = evaluate (V, i, j, k, l);
      const Real z_rho = model.get_z (k, topography, A_rho, B_rho);
      const Real z_theta = model.get_z (k, topography, A_theta, B_theta);
      const Real p_rho = evaluate (P_RHO, i, j, k, l);
      const Real p_theta = evaluate (P_THETA, i, j, k, l);

      sounding.get_t_line ().add (p_theta, t - K);
      sounding.get_t_d_line ().add (p_theta, t_d - K); 
      sounding.get_wind_profile ().add (p_rho, Wind (u, v));
      sounding.get_height_profile ().add (p_rho, z_rho);
      sounding.get_height_profile ().add (p_theta, z_theta);

   }


}

Color
Model::Uppers::Stage::get_color (const Product& product,
                                 const Lat_Long& lat_long,
                                 const Level& level,
                                 const size_t l) const
{

   switch (product.enumeration)
   {

      case Product::WIND:
      {

         size_t i, j, k;
         acquire_ij (i, j, lat_long);
         acquire_k (k, U, i, j, level);
         if (k < 0 || k > 69) { return transparent; }

         const Real u = evaluate (U, i, j, k, l);
         const Real v = evaluate (V, i, j, k, l);
         const Real speed = sqrt (u*u + v*v);
         const Real theta = atan2 (-u, -v);
         const Real hue = (theta < 0 ? theta + 2*M_PI : theta)/ (2*M_PI);
         const Real brightness = std::min (speed / 15, 1.0);
         return Color::hsb (hue, 0.8, brightness);

      }

      case Product::Q_ADVECTION:
      {
         const Real h = evaluate_h_advection (Q, lat_long, level, l);
         const Real v = evaluate_v_advection (Q, lat_long, level, l);
         const Real datum = h + v;
         return Display::get_color (product, datum);
      }

      case Product::Q_H_ADVECTION:
      {
         const Real datum = evaluate_h_advection (Q, lat_long, level, l);
         return Display::get_color (product, datum);
      }

      case Product::Q_V_ADVECTION:
      {
         const Real datum = evaluate_v_advection (Q, lat_long, level, l);
         return Display::get_color (product, datum);
      }

      default:
      {

         const Met_Element met_element = product.get_met_element ();

         size_t i, j, k;
         acquire_ij (i, j, lat_long);
         acquire_k (k, met_element, i, j, level);
         if (k < 0 || k > 69) { return transparent; }

         const Real datum = evaluate (met_element, i, j, k, l);
         return Display::get_color (product, datum);

      }

   }

}

const Model::Uppers::Stage&
Model::Uppers::get_stage (const twiin::Stage& stage) const
{
   const bool no_match = (stage_map.find (stage) == stage_map.end ());
   if (no_match) { cout << "no match" << endl; }
   return (const Model::Uppers::Stage&)(stage_map.at (stage));
}

Model::Uppers::Stage&
Model::Uppers::get_stage (const twiin::Stage& stage)
{
   const bool no_match = (stage_map.find (stage) == stage_map.end ());
   if (no_match) { cout << "no match" << endl; }
   return (Model::Uppers::Stage&)(stage_map.at (stage));
}

Model::Uppers::Uppers (const Model& model)
   : Model::Surface (model)
{
}

void
Model::Uppers::init (const Tokens& stage_tokens)
{

   vector<twiin::Stage> twiin_stage_vector;

   for (auto iterator = stage_tokens.begin ();
        iterator != stage_tokens.end (); iterator++)
   {
      twiin::Stage twiin_stage (*(iterator));
      Model::Uppers::Stage stage (model, twiin_stage);
      stage_map.insert (make_pair (twiin_stage, stage));
   }

}

void
Model::Uppers::init_stage (const twiin::Stage& twiin_stage,
                           const Model::File_Path_Map& file_path_map)
{
   Model::Uppers::Stage& stage = stage_map.at (twiin_stage);
   stage.init (file_path_map);
}

Model::Vertical_Coefficients::Vertical_Coefficients ()
{
}

Model::Vertical_Coefficients::Vertical_Coefficients (const Dstring& file_path)
{
   init (file_path);
}

void
Model::Vertical_Coefficients::init (const Dstring& file_path)
{

   string is;
   ifstream file (file_path);

   while (getline (file, is))
   {

      if (is.size () == 0) { continue; }
      if (is[0] == '#') { continue; }

      const Dstring input_string (is);
      const Tokens tokens (input_string, ":");
      A_theta.push_back (stof (tokens[0]));
      B_theta.push_back (stof (tokens[1]));
      A_rho.push_back (stof (tokens[2]));
      B_rho.push_back (stof (tokens[3]));

   }

   file.close ();

}

const Tuple&
Model::Vertical_Coefficients::get_A_theta () const
{
   return A_theta;
}

const Tuple&
Model::Vertical_Coefficients::get_B_theta () const
{
   return B_theta;
}

const Tuple&
Model::Vertical_Coefficients::get_A_rho () const
{
   return A_rho;
}

const Tuple&
Model::Vertical_Coefficients::get_B_rho () const
{
   return B_rho;
}

const Tuple&
Model::Vertical_Coefficients::get_A (const bool is_theta) const
{
   return (is_theta ? A_theta : A_rho);
}

const Tuple&
Model::Vertical_Coefficients::get_B (const bool is_theta) const
{
   return (is_theta ? B_theta : B_rho);
}

const Tuple&
Model::Vertical_Coefficients::get_A (const Met_Element& met_element) const
{
   return get_A (Model::is_theta (met_element));
}

const Tuple&
Model::Vertical_Coefficients::get_B (const Met_Element& met_element) const
{
   return get_B (Model::is_theta (met_element));
}

Dstring
Model::get_nc_varname (const Varname& varname)
{
   if (varname == "orog") { return "ht"; }
   if (varname == "lsm") { return "lsm"; }
   if (varname == "temp") { return "temp"; }
   if (varname == "qsair") { return "q"; }
   if (varname == "dewpt") { return "field17"; }
   if (varname == "xwind") { return "x-wind"; }
   if (varname == "ywind") { return "y-wind"; }
   if (varname == "ml_xwind") { return "x-wind"; }
   if (varname == "ml_ywind") { return "y-wind"; }
   if (varname == "ml_zwind") { return "dz_dt"; }
   if (varname == "mslp") { return "p"; }
   if (varname == "prcp8p5") { return "precip"; }
   if (varname == "ml_prho") { return "p"; }
   if (varname == "ml_ptheta") { return "p"; }
   if (varname == "prcp8p5") { return "precip"; }
   if (varname == "ml_spechum") { return "q"; }
   if (varname == "ml_theta") { return "theta"; }
   throw Exception ("invalid varname: " + varname);
}

const Real
Model::get_z (const Integer k,
              const Real topography,
              const Tuple& A,
              const Tuple& B) const
{
   return A[k] + B[k] * topography;
}

const size_t
Model::get_k (const Real z,
              const Real topography,
              const Tuple& A,
              const Tuple& B,
              Integer start_k,
              Integer end_k) const
{

   if (start_k == -1) { start_k = 0; }
   if (end_k == -1) { end_k = 69; }

   if (end_k - start_k <= 1)
   {
      const Real start_diff = fabs (z - get_z (start_k, topography, A, B));
      const Real end_diff = fabs (z - get_z (end_k, topography, A, B));
      const bool nearer_to_start = start_diff < end_diff;
      return (nearer_to_start ? start_k : end_k);
   }
   else
   {
      const Real sum = start_k + end_k;
      const bool is_even = denise::is_even (sum);
      const Integer mk = Integer (is_even ? round (sum/2) : floor (sum/2));
      const Real middle_z = get_z (mk, topography, A, B);
      const bool larger = z > middle_z;
      return (larger ?
         get_k (z, topography, A, B, mk, end_k) :
         get_k (z, topography, A, B, start_k, mk));

   }

}


Model::Model (const Config_File& config_file)
   : terrain (*this),
     uppers (*this),
     surface (*this)
{

   Model::File_Path_Map terrain_file_path_3_map;
   Model::File_Path_Map terrain_file_path_4_map;
   Model::File_Path_Map terrain_file_path_5_map;
   Model::File_Path_Map surface_file_path_3_map;
   Model::File_Path_Map surface_file_path_4_map;
   Model::File_Path_Map surface_file_path_5_map;
   Model::File_Path_Map uppers_file_path_3_map;
   Model::File_Path_Map uppers_file_path_4_map;
   Model::File_Path_Map uppers_file_path_5_map;
   Dstring vertical_coefficients_file_path;

   map<Dstring, Model::File_Path_Map> terrain_file_path_map;
   map<Dstring, Model::File_Path_Map> surface_file_path_map;
   map<Dstring, Model::File_Path_Map> uppers_file_path_map;

   for (auto iterator = config_file.begin ();
        iterator != config_file.end (); iterator++)
   {

      const Tokens tokens (*(iterator), " \f\n\t");
      if (tokens.size () != 2 || tokens[0] != "model") { continue; }

      const Dstring& argument = tokens[1];
      const Tokens argument_tokens (argument, ":");
      const Dstring& model_identifier = argument_tokens[0];

      if (model_identifier == "RUN" && argument_tokens.size () == 2)
      {
         basetime = Dtime (argument_tokens[1]);
         continue;
      }

      if (model_identifier == "AB" && argument_tokens.size () == 2)
      {
         vertical_coefficients_file_path = argument_tokens[1].get_trimmed ();
         continue;
      }

      if (model_identifier.substr (0, 5) == "STAGE" &&
          argument_tokens.size () == 3)
      {

         const Dstring& stage_str = argument_tokens[0].get_trimmed ();
         const Dstring& var_str = argument_tokens[1].get_trimmed ();
         const Dstring& file_path = argument_tokens[2].get_trimmed ();

         const bool is_terrain = (var_str == "orog" || var_str == "lsm");
         const bool is_uppers = (var_str.substr (0, 3) == "ml_");

         if (stage_str == "STAGE3")
         {
            if (is_terrain)
            {
               terrain_file_path_3_map.insert (var_str, file_path);
            }
            else
            if (is_uppers)
            {
               uppers_file_path_3_map.insert (var_str, file_path);
            }
            else
            {
               surface_file_path_3_map.insert (var_str, file_path);
            }
         }
         else
         if (stage_str == "STAGE4")
         {
            if (is_terrain)
            {
               terrain_file_path_4_map.insert (var_str, file_path);
            }
            else
            if (is_uppers)
            {
               uppers_file_path_4_map.insert (var_str, file_path);
            }
            else
            {
               surface_file_path_4_map.insert (var_str, file_path);
            }
         }
         else
         if (stage_str == "STAGE5")
         {
            if (is_terrain)
            {
               terrain_file_path_5_map.insert (var_str, file_path);
            }
            else
            if (is_uppers)
            {
               uppers_file_path_5_map.insert (var_str, file_path);
            }
            else
            {
               surface_file_path_5_map.insert (var_str, file_path);
            }
         }

         continue;

      }
 
   }

   vertical_coefficients.init (vertical_coefficients_file_path);

   const Tokens stage_tokens ("STAGE3 STAGE4 STAGE5");

   terrain.init (stage_tokens);
   terrain.init_stage (Dstring ("STAGE3"), terrain_file_path_3_map);
   terrain.init_stage (Dstring ("STAGE4"), terrain_file_path_4_map);
   terrain.init_stage (Dstring ("STAGE5"), terrain_file_path_5_map);

   surface.init (stage_tokens);
   surface.init_stage (Dstring ("STAGE3"), surface_file_path_3_map);
   surface.init_stage (Dstring ("STAGE4"), surface_file_path_4_map);
   surface.init_stage (Dstring ("STAGE5"), surface_file_path_5_map);

   uppers.init (stage_tokens);
   uppers.init_stage (Dstring ("STAGE3"), uppers_file_path_3_map);
   uppers.init_stage (Dstring ("STAGE4"), uppers_file_path_4_map);
   uppers.init_stage (Dstring ("STAGE5"), uppers_file_path_5_map);

}

Model::~Model ()
{
}

bool
Model::is_theta (const Met_Element& met_element)
{
   return ((met_element == THETA) || 
           (met_element == Q) || 
           (met_element == RHO) || 
           (met_element == W) || 
           (met_element == T) || 
           (met_element == TD) || 
           (met_element == RH) || 
           (met_element == THETA_E) ||
           (met_element == THETA_V));
}

const Dtime&
Model::get_basetime () const
{
   return basetime;
}

Domain_2D
Model::get_domain_2d (const twiin::Stage& stage) const
{
   const Model::Terrain::Stage& terrain_stage = terrain.get_stage (stage);
   return terrain_stage.get_domain_2d ();
}

bool
Model::out_of_bounds (const Lat_Long& lat_long,
                      const twiin::Stage& stage) const
{
   const Model::Terrain::Stage& terrain_stage = terrain.get_stage (stage);
   return terrain_stage.out_of_bounds (lat_long);
}

Real
Model::get_topography (const Lat_Long& lat_long,
                       const twiin::Stage& stage) const
{
   const Model::Terrain::Stage& terrain_stage = terrain.get_stage (stage);
   return terrain_stage.get_topography (lat_long);
}

Real
Model::evaluate (const Met_Element& met_element,
                 const Lat_Long& lat_long,
                 const size_t k,
		 const Dtime& dtime,
                 const twiin::Stage& stage) const
{

   if (k < 0)
   {
      const Model::Surface::Stage& surface_stage = surface.get_stage (stage);
      const Integer l = surface_stage.get_l (dtime);
      return surface_stage.evaluate (met_element, lat_long, l);
   }
   else
   {
      const Model::Uppers::Stage& uppers_stage = uppers.get_stage (stage);
      const Integer l = uppers_stage.get_l (dtime);
      return uppers_stage.evaluate (met_element, lat_long, k, l);
   }
}

Real
Model::evaluate (const Met_Element& met_element,
                 const Lat_Long& lat_long,
                 const Level& level,
                 const Dtime& dtime,
                 const twiin::Stage& stage) const
{
   if (met_element == MSLP ||
       met_element == PRECIP_RATE ||
       met_element == FFDI ||
       level.type == Level::SURFACE)
   {
      const Model::Surface::Stage& surface_stage = surface.get_stage (stage);
      const Integer l = surface_stage.get_l (dtime);
      return surface_stage.evaluate (met_element, lat_long, l);
   }
   else
   {
      const Model::Uppers::Stage& uppers_stage = uppers.get_stage (stage);
      const Integer l = uppers_stage.get_l (dtime);
      return uppers_stage.evaluate (met_element, lat_long, level, l);
   }
}

bool
Model::grow_trajectory (Lat_Long& lat_long,
                        Level& level,
                        Dtime& dtime,
                        const twiin::Stage& stage,
                        const Real finish_tau) const
{

   if (out_of_bounds (lat_long, stage)) { return false; }
   const Real topography = get_topography (lat_long, stage);

   if (level.type == Level::HEIGHT)
   {
      if (level.value > 38000) { return false; }
      else
      if (level.value < topography)
      {
         level.value = topography;
         level.type = Level::SURFACE;
      }
   }

   const bool is_surface = (level.type == Level::SURFACE);
   const Real z = (is_surface ? topography : level.value);

   const Real u = evaluate (U, lat_long, level, dtime, stage);
   const Real v = evaluate (V, lat_long, level, dtime, stage);
   const Real w = evaluate (W, lat_long, level, dtime, stage);

   const auto grid_size = terrain.get_stage (stage).get_grid_size (lat_long);
   const Real h_lat = grid_size.first;
   const Real h_long = grid_size.second;

   const Real dt_x = 0.7 * h_long / u;
   const Real dt_y = 0.7 * h_lat / v;
   const Real dt = std::min (dt_x, dt_y);

   const Wind wind_ (u, v);
   const Dtime dtime_ (dtime.t + dt);
   const Lat_Long& lat_long_ = Geodesy::get_destination (lat_long,
      wind_.get_speed () * dt, wind_.get_direction () + 180);
   const Real z_ = (is_surface ? topography: level.value + w * dt);
   const Level& level_ = (is_surface ? level : Level::z_level (z_));

   const Real u_ = evaluate (U, lat_long_, level, dtime_, stage);
   const Real v_ = evaluate (V, lat_long_, level, dtime_, stage);
   const Real w_ = evaluate (W, lat_long_, level, dtime_, stage);

   const Wind wind ((u + u_) / 2.0, (v + v_) / 2.0);
   lat_long = Geodesy::get_destination (lat_long,
      wind.get_speed () * dt, wind.get_direction () + 180);
   level = (is_surface ? level : Level::z_level (z + (w + w_)/2.0 * dt));
   dtime.t += dt;

   return true;

}

Track
Model::get_trajectory (Lat_Long lat_long,
                       Level level,
                       Dtime dtime,
                       const twiin::Stage& stage,
                       const Real finish_tau) const
{

   Track trajectory (dtime);

   while (grow_trajectory (lat_long, level, dtime, stage, finish_tau))
   {
      trajectory.add (dtime, lat_long);
      trajectory.add ("z", dtime, level.value);
   }

   trajectory.okay ();
   return trajectory;

}

Aws::Obs
Model::get_aws_obs (const Lat_Long& lat_long,
                    const Dtime& dtime,
                    const twiin::Stage& stage) const
{

   const Model::Surface::Stage& surface_stage = surface.get_stage (stage);
   return surface_stage.get_aws_obs (lat_long, dtime);
}

const Aws::Repository*
Model::get_aws_repository_ptr (const Lat_Long& lat_long,
                               const twiin::Stage& stage) const

   
{

   Aws::Repository* aws_repository_ptr = new Aws::Repository ();
   const Model::Surface::Stage& surface_stage = surface.get_stage (stage);
   const auto& valid_time_set = surface_stage.get_valid_time_set ();

   for (auto iterator = valid_time_set.begin ();
        iterator != valid_time_set.end (); iterator++)
   {

      const Dtime& dtime = *(iterator);
      //if (!time_set.match (dtime)) { continue; }

      const Aws::Key key (-1, dtime);
      const Aws::Obs& obs = surface_stage.get_aws_obs (lat_long, dtime);
      aws_repository_ptr->insert (key, obs);

   }

   return aws_repository_ptr;

}

Sounding*
Model::get_sounding_ptr (const Lat_Long& lat_long,
                         const Dtime& dtime,
                         const twiin::Stage& stage) const
{

   Sounding* sounding_ptr = new Sounding ();

   const Model::Surface::Stage& surface_stage = surface.get_stage (stage);
   const Model::Uppers::Stage& uppers_stage = uppers.get_stage (stage);

   sounding_ptr->set_time (dtime);
   sounding_ptr->set_basetime (basetime);
   sounding_ptr->set_location_str (lat_long.get_string (4));
   surface_stage.fill_sounding (*sounding_ptr, lat_long, dtime);
   uppers_stage.fill_sounding (*sounding_ptr, lat_long, dtime);

   return sounding_ptr;

}

Sounding*
Model::get_sounding_ptr (const Lat_Long::List& lat_long_list,
                         const Dtime& dtime,
                         const twiin::Stage& stage,
                         const Thermo_Diagram& thermo_diagram) const
{

   const bool is_singular = (lat_long_list.size () == 1);
   if (is_singular)
   {
      const Lat_Long& lat_long = lat_long_list.front ();
      return get_sounding_ptr (lat_long, dtime, stage);
   }
   else
   {

      Dstring location_str ("");
      list<const Sounding*> sounding_ptr_list;

      for (auto iterator = lat_long_list.begin ();
           iterator != lat_long_list.end (); iterator++)
      {
         const Lat_Long& ll = *(iterator);
         const Dstring prefix (iterator == lat_long_list.begin () ? "" : ":");
         location_str += prefix + ll.get_string (4);
         const Sounding* sounding_ptr = get_sounding_ptr (ll, dtime, stage);
         sounding_ptr_list.push_back (sounding_ptr);
      }

      Sounding* mean_sounding_ptr =
         Sounding::get_mean_sounding_ptr (sounding_ptr_list, thermo_diagram);

      for (auto iterator = sounding_ptr_list.begin ();
           iterator != sounding_ptr_list.end (); iterator++)
      {
         const Sounding* sounding_ptr = *(iterator);
         delete sounding_ptr;
      }

      mean_sounding_ptr->set_time (dtime);
      mean_sounding_ptr->set_basetime (basetime);
      mean_sounding_ptr->set_location_str (location_str);

      return mean_sounding_ptr;

   }

}

const set<Dtime>&
Model::get_valid_time_set (const Product& product,
                           const twiin::Stage& stage,
                           const Level& level) const
{

   switch (product.enumeration)
   {

      case Product::TERRAIN:
      case Product::TERRAIN_WATER:
      {
         const Model::Surface::Stage& surface_stage = surface.get_stage (stage);
         return surface_stage.get_valid_time_set ();
      }

      case Product::T:
      case Product::P_THETA:
      case Product::P_RHO:
      case Product::Q:
      case Product::Q_ADVECTION:
      case Product::Q_H_ADVECTION:
      case Product::Q_V_ADVECTION:
      case Product::Q_S_ADVECTION:
      case Product::Q_N_ADVECTION:
      case Product::Q_SV_ADVECTION:
      case Product::Q_NV_ADVECTION:
      case Product::TD:
      case Product::RH:
      case Product::RHO:
      case Product::WIND:
      case Product::SPEED:
      case Product::SPEED_HIGHER:
      case Product::ALONG_SPEED:
      case Product::NORMAL_SPEED:
      case Product::BRUNT_VAISALA:
      case Product::W:
      case Product::W_TRANSLUCENT:
      case Product::VORTICITY:
      case Product::THETA:
      case Product::THETA_E:
      case Product::THETA_V:
      {
         if (level.type == Level::SURFACE)
         {
            auto& surface_stage = surface.get_stage (stage);
            return surface_stage.get_valid_time_set ();
         }
         else
         if (level.type == Level::HEIGHT)
         {
            auto& uppers_stage = uppers.get_stage (stage);
            return uppers_stage.get_valid_time_set ();
         }
      }

      case Product::FFDI:
      case Product::MSLP:
      case Product::PRECIP_RATE:
      {
         auto& surface_stage = surface.get_stage (stage);
         surface_stage.get_valid_time_set ();
         return surface_stage.get_valid_time_set ();
      }

   }

}

vector<Dtime>
Model::get_valid_time_vector (const Product& product,
                              const twiin::Stage& stage,
                              const Level& level,
                              const Dtime::Set& time_set) const
{
   vector<Dtime> valid_time_vector;
   auto valid_time_set = get_valid_time_set (product, stage, level);

   for (auto l = valid_time_set.begin ();
        l != valid_time_set.end (); l++)
   {
      const Dtime& dtime = *(l);
      if (time_set.match (dtime)) { valid_time_vector.push_back (dtime); }
   }

   return valid_time_vector;


}

Tokens
Model::get_marker_tokens (const Lat_Long& lat_long,
                          const Dtime& dtime,
                          const Product& product,
                          const twiin::Stage& stage,
                          const Level& level) const
{

   Tokens tokens;

   switch (product.enumeration)
   {

      case Product::P_THETA:
      {
         const Real datum = evaluate (P_THETA, lat_long, level, dtime, stage);
         if (gsl_isnan (datum)) { return tokens; }
         tokens.push_back (Dstring::render ("%.0fhPa", datum * 1e-2));
         break;
      }

      case Product::P_RHO:
      {
         const Real datum = evaluate (P_RHO, lat_long, level, dtime, stage);
         if (gsl_isnan (datum)) { return tokens; }
         tokens.push_back (Dstring::render ("%.1fhPa", datum * 1e-2));
         break;
      }

      case Product::THETA:
      {
         const Real datum = evaluate (THETA, lat_long, level, dtime, stage);
         if (gsl_isnan (datum)) { return tokens; }
         tokens.push_back (Dstring::render ("%.1f\u00b0C", datum - K));
         break;
      }

      case Product::THETA_V:
      {
         const Real theta_e = evaluate (THETA_V, lat_long, level, dtime, stage);
         if (gsl_isnan (theta_e)) { return tokens; }
         tokens.push_back (Dstring::render ("%.1f\u00b0C", theta_e - K));
         break;
      }

      case Product::Q:
      {
         const Real datum = evaluate (Q, lat_long, level, dtime, stage);
         if (gsl_isnan (datum)) { return tokens; }
         tokens.push_back (Dstring::render ("%.3fg/kg", datum * 1e3));
         break;
      }

      case Product::T:
      {
         const Real datum = evaluate (T, lat_long, level, dtime, stage);
         if (gsl_isnan (datum)) { return tokens; }
         tokens.push_back (Dstring::render ("%.1f\u00b0C", datum - K));
         break;
      }

      case Product::TD:
      {
         const Real t_d = evaluate (TD, lat_long, level, dtime, stage);
         if (gsl_isnan (t_d)) { return tokens; }
         tokens.push_back (Dstring::render ("%.1f\u00b0C", t_d - K));
         break;
      }

      case Product::RH:
      {
         const Real rh = evaluate (RH, lat_long, level, dtime, stage);
         if (gsl_isnan (rh)) { return tokens; }
         tokens.push_back (Dstring::render ("%.0f%%", rh * 100));
         break;
      }

      case Product::THETA_E:
      {
         const Real theta_e = evaluate (THETA_E, lat_long, level, dtime, stage);
         if (gsl_isnan (theta_e)) { return tokens; }
         tokens.push_back (Dstring::render ("%.1f\u00b0C", theta_e - K));
         break;
      }

      case Product::RHO:
      {
         const Real rho = evaluate (RHO, lat_long, level, dtime, stage);
         if (gsl_isnan (rho)) { return tokens; }
         tokens.push_back (Dstring::render ("%.2fkg/m3", rho));
         break;
      }

      case Product::WIND:
      case Product::SPEED:
      {
         const Real u = evaluate (U, lat_long, level, dtime, stage);
         if (gsl_isnan (u)) { return tokens; }
         const Real v = evaluate (V, lat_long, level, dtime, stage);
         if (gsl_isnan (v)) { return tokens; }
         const Wind wind (u, v);
         const Real msknot = 3.6/1.852;
         const Dstring fmt ("%03.0f\u00b0 / %02.1fkt");
         tokens.push_back (wind.get_string (msknot, fmt));
         break;
      }

      case Product::FFDI:
      {
         const Real ffdi = evaluate (FFDI, lat_long, level, dtime, stage);
         if (gsl_isnan (ffdi)) { return tokens; }
         Dstring ffdr ("Low-Moderate");
         if (ffdi > 12) { ffdr = "High"; }
         if (ffdi > 25) { ffdr = "Very High"; }
         if (ffdi > 50) { ffdr = "Severe"; }
         if (ffdi > 75) { ffdr = "Extreme"; }
         if (ffdi > 100) { ffdr = "Catastrophic"; }
         tokens.push_back (Dstring::render ("%02.2f", ffdi));
         tokens.push_back (ffdr);
         break;
      }

      case Product::TERRAIN:
      case Product::TERRAIN_WATER:
      {
         const Model::Terrain::Stage& terrain_stage = terrain.get_stage (stage);
         const Real orog = terrain_stage.evaluate (Dstring ("orog"), lat_long);
         if (gsl_isnan (orog)) { return tokens; }
         tokens.push_back (Dstring::render ("%.2fm", orog));
         break;
      }

      case Product::W:
      {
         const Real w = evaluate (W, lat_long, level, dtime, stage);
         if (gsl_isnan (w)) { return tokens; }
         tokens.push_back (Dstring::render ("%.2fm/s", w));
         break;
      }

      case Product::VORTICITY:
      {
         const Real zeta = evaluate (ZETA, lat_long, level, dtime, stage);
         if (gsl_isnan (zeta)) { return tokens; }
         tokens.push_back (Dstring::render ("%.4e/s", zeta));
         break;
      }

      case Product::MSLP:
      {
         const Real mslp = evaluate (MSLP, lat_long, level, dtime, stage);
         if (gsl_isnan (mslp)) { return tokens; }
         tokens.push_back (Dstring::render ("%0.1fhPa", mslp * 1e-2));
         break;
      }

      case Product::PRECIP_RATE:
      {
         const Real mmhr = evaluate (PRECIP_RATE, lat_long, level, dtime, stage);
         if (gsl_isnan (mmhr)) { return tokens; }
         tokens.push_back (Dstring::render ("%0.1fmm/hr", mmhr * 3600));
         break;
      }

   }

   return tokens;

}

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

