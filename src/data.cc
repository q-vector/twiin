#include <denise/dstring.h>
#include <denise/gzstream.h>
#include <denise/nwp.h>
#include "data.h"
#include "display.h"

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;

Stage::Stage (const string& str)
   : string (str)
{
}

Stage::Stage (const Stage& stage)
   : string (stage)
{
}

string
Stage::get_string () const
{
   return *this;
}

Product::Product (const string& str)
   : string (str)
{
}

Product::Product (const Product& product)
   : string (product)
{
}

string
Product::get_string () const
{
   return *this;
}

Nwp_Element
Product::get_nwp_element () const
{
   if (*this == "T") { return T; }
   if (*this == "TD") { return TD; }
   if (*this == "RH") { return RH; }
   if (*this == "THETA") { return THETA; }
   if (*this == "Q") { return Q; }
   if (*this == "THETA_E") { return THETA_E; }
   if (*this == "RHO") { return RHO; }
   if (*this == "WIND") { return WIND_DIRECTION; }
   if (*this == "SPEED") { return WIND_SPEED; }
   if (*this == "SPEED_HIGHER") { return WIND_SPEED; }
   if (*this == "VORTICITY") { return RELATIVE_VORTICITY; }
   if (*this == "W") { return W; }
   if (*this == "W_TRANSLUCENT") { return W; }
   if (*this == "FFDI") { return FFDI; }
   if (*this == "MSLP") { return MSLP; }
   if (*this == "PRECIP_RATE") { return PRECIP_RATE; }
}

Station::Station (const Integer id,
                  const Real latitude,
                  const Real longitude,
                  const Real height,
                  const string& name)
   : Lat_Long (latitude, longitude),
     id (id),
     height (height),
     name (name)
{
}

Station::Map::Map ()
{
}

Station::Map::Map (const string& file_path)
{
   ingest (file_path);
}

void
Station::Map::ingest (const string& file_path)
{

   string input_string;
   ifstream file (file_path.c_str ());

   while (getline (file, input_string))
   {

      if (input_string.size () == 0) { continue; }
      if (input_string.c_str ()[0] == '#') { continue; }

      const Tokens tokens (input_string, ":");
      const Integer id = stoi (tokens[0]);
      const Real latitude = stof (tokens[1]);
      const Real longitude = stof (tokens[2]);
      const Real height = stof (tokens[3]);
      const string& name = tokens[4];
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

pair<string, Lat_Long>
Station::Map::nearest (const Lat_Long& lat_long) const
{
   const Station& station = get_nearest_station (lat_long);
   const string& str = string_render ("%d", station.id);
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

Location::Location (const string& str,
                    const Station::Map& station_map)
   : Lat_Long (GSL_NAN, GSL_NAN),
     station_id (-1),
     str (str),
     long_str ("")
{

   const char c = str[0];

   switch (c)
   {

      case 'a':
      {
         const string& station_id_str = str.substr (1);
         const Integer station_id = stof (station_id_str);
         const Station& station = station_map.at (station_id);
         this->latitude = station.latitude;
         this->longitude = station.longitude;
         this->station_id = station_id;
         this->str = station_id_str;
         this->long_str = station.name + " (" + station_id_str + ")";
         break;
      }

      case 'l':
      {
         const Tokens tokens (str.substr (1), ",");
         this->latitude = stof (tokens[0]);
         this->longitude = stof (tokens[1]);
         this->str = Lat_Long::get_string (true, string ("%.4f"));
         break;
      }

   }

}

Integer
Location::get_station_id () const
{
   return station_id;
}

const string&
Location::get_str () const
{
   return str;
}

const string&
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
Aws::Repository::to_real (const string& token)
{
  return (token == "-9999" ? GSL_NAN : stof (token));
}

Aws::Repository::Repository ()
{
}

Aws::Repository::Repository (const string& file_path)
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
Aws::Repository::ingest (const string& file_path)
{

   igzstream file (file_path);

   for (string input_line; std::getline (file, input_line); )
   {

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

Model::Varname::Varname (const string& str)
   : string (str)
{
}

Model::Varname::Varname (const Model::Varname& varname)
   : string (varname)
{
}

string
Model::Varname::get_string () const
{
   return *this;
}

void
Model::File_Path_Map::insert (const Varname& varname,
                              const string& file_path)
{
   map<Varname, string>::insert (make_pair (varname, file_path));
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
      const string& file_path = iterator->second;

      nc_file_ptr_map.insert (make_pair (varname, new Nc_File (file_path)));
      const Nc_File& nc_file = *(nc_file_ptr_map.at (varname));
      const Integer nc_id = nc_file.get_nc_id ();

      const bool first = (iterator == file_path_map.begin ());
      if (first)
      {
         this->tuple_latitude = nc_file.get_coordinate_tuple ("latitude");
         this->tuple_longitude = nc_file.get_coordinate_tuple ("longitude");
      }

      const string& nc_varname = Model::get_nc_varname (varname);

      ret = nc_inq_varid (nc_id, nc_varname.c_str (), &varid);
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

   string str (attribute_char);
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
Model::Surface::Stage::evaluate (const Nwp_Element& nwp_element,
                                 const Lat_Long& lat_long,
                                 const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate (nwp_element, i, j, l);
}

Real
Model::Surface::Stage::evaluate (const Nwp_Element& nwp_element,
                                 const size_t i,
                                 const size_t j,
                                 const size_t l) const
{

   int ret;
   float datum;

   switch (nwp_element)
   {

      case W:
      {
         datum = 0;
         break;
      };

      case WIND_SPEED:
      {
         const Real u = evaluate_raw (string ("xwind"), i, j, l);
         const Real v = evaluate_raw (string ("ywind"), i, j, l);
         datum = sqrt (u*u + v*v);
         break;
      };

      case WIND_DIRECTION:
      {
         const Real u = evaluate_raw (string ("xwind"), i, j, l);
         const Real v = evaluate_raw (string ("ywind"), i, j, l);
         datum = Wind (u, v).get_direction ();
         break;
      };

      case RH:
      {
         const Real t = evaluate_raw (string ("temp"), i, j, l);
         const Real t_d = evaluate_raw (string ("dewpt"), i, j, l);
         datum = Moisture::get_rh (t - K, t_d - K, WATER);
         break;
      };

      case Q:
      {
         const Real t_d = evaluate_raw (string ("dewpt"), i, j, l);
         const Real mslp = evaluate_raw (string ("mslp"), i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         datum = Moisture::get_q_s (t_d - K, surface_p);
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
         const Real t = evaluate_raw (string ("temp"), i, j, l);
         const Real mslp = evaluate_raw (string ("mslp"), i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         datum = Thermo_Point::t_p (t - K, surface_p).get_theta () + K;
         break;
      };

      case THETA_E:
      {
         typedef Thermo_Point Tp;
         const Real t = evaluate_raw (string ("temp"), i, j, l);
         const Real t_d = evaluate_raw (string ("dewpt"), i, j, l);
         const Real mslp = evaluate_raw (string ("mslp"), i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         datum = Tp::normand (t - K, t_d - K, surface_p).get_theta_e () + K;
         break;
      };

      case RHO:
      {
         const Real t = evaluate_raw (string ("temp"), i, j, l);
         const Real mslp = evaluate_raw (string ("mslp"), i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         datum = surface_p / (R_d * t);
         break;
      };

      case FFDI:
      {
         const Real t = evaluate_raw (string ("temp"), i, j, l);
         const Real t_d = evaluate_raw (string ("dewpt"), i, j, l);
         const Real rh = Moisture::get_rh (t - K, t_d - K, WATER);
         const Real u = evaluate_raw (string ("xwind"), i, j, l);
         const Real v = evaluate_raw (string ("ywind"), i, j, l);
         const Real speed = sqrt (u*u + v*v);
         datum = Fire::get_ffdi (t - K, rh * 100, speed * 3.6);
         break;
      };

      default:
      {

         Varname varname ("");

         switch (nwp_element)
         {
            case T: { varname = string ("temp"); break; }
            case TD: { varname = string ("dewpt"); break; }
            case MSLP: { varname = string ("mslp"); break; }
            case PRECIP_RATE: { varname = string ("prcp8p5"); break; }
            case U: { varname = string ("xwind"); break; }
            case V: { varname = string ("ywind"); break; }
         }

         datum = evaluate_raw (varname, i, j, l);
         break;

      }

   }

   return Real (datum);

}

Real
Model::Surface::Stage::evaluate_raw (const string& varname,
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

   if (product == "WIND")
   {
      const Real u = evaluate (U, lat_long, l);
      const Real v = evaluate (V, lat_long, l);
      const Real speed = sqrt (u*u + v*v);
      const Real theta = atan2 (-u, -v);
      const Real hue = (theta < 0 ? theta + 2*M_PI : theta)/ (2*M_PI);
      const Real brightness = std::min (speed / 15, 1.0);
      return Color::hsb (hue, 0.8, brightness);
   }
   else
   {
      const Nwp_Element nwp_element = product.get_nwp_element ();
      const Real datum = evaluate (nwp_element, lat_long, l);
      return Display::get_color (product, datum);
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

Model::Uppers::Stage::Stage (const Model& model,
                             const twiin::Stage& stage)
   : Model::Surface::Stage (model, stage)
{
}


Real
Model::Uppers::Stage::evaluate_normal_speed (const Real azimuth,
                                             const Lat_Long& lat_long,
                                             const Real z,
                                             const size_t l) const
{

   size_t i, j;
   acquire_ij (i, j, lat_long);

   const Real topography = get_topography (i, j);
   if (z < topography) { return GSL_NAN; }

   const Tuple& A = model.vertical_coefficients.get_A_rho ();
   const Tuple& B = model.vertical_coefficients.get_B_rho ();
   const Integer k = model.get_k (z, topography, A, B);

   const Real u = evaluate (U, i, j, k, l);
   const Real v = evaluate (V, i, j, k, l);

   const Real theta = azimuth * M_PI/180;
   return v * sin (theta) - u * cos (theta);

}

Real
Model::Uppers::Stage::evaluate_streamline_speed (const Real azimuth,
                                                 const Lat_Long& lat_long,
                                                 const Real z,
                                                 const size_t l) const
{

   size_t i, j;
   acquire_ij (i, j, lat_long);

   const Real topography = get_topography (i, j);
   if (z < topography) { return GSL_NAN; }

   const Tuple& A = model.vertical_coefficients.get_A_rho ();
   const Tuple& B = model.vertical_coefficients.get_B_rho ();
   const Integer k = model.get_k (z, topography, A, B);

   const Real u = evaluate (U, i, j, k, l);
   const Real v = evaluate (V, i, j, k, l);

   const Real theta = azimuth * M_PI/180;
   return u * sin (theta) + v * cos (theta);

}

Real
Model::Uppers::Stage::evaluate_scorer (const Real azimuth,
                                       const Lat_Long& lat_long,
                                       const Real z,
                                       const size_t l) const
{

   size_t i, j;
   acquire_ij (i, j, lat_long);

   const Real topography = get_topography (i, j);
   if (z < topography) { return GSL_NAN; }

   const Tuple& A_rho = model.vertical_coefficients.get_A_rho ();
   const Tuple& B_rho = model.vertical_coefficients.get_B_rho ();
   const Integer k_rho = model.get_k (z, topography, A_rho, B_rho);
   if (k_rho == 0 && k_rho == 69) { return GSL_NAN; }

   const Tuple& A_theta = model.vertical_coefficients.get_A_theta ();
   const Tuple& B_theta = model.vertical_coefficients.get_B_theta ();
   const Integer k_theta = model.get_k (z, topography, A_theta, B_theta);
   if (k_theta == 0 && k_theta == 69) { return GSL_NAN; }

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

   const Real theta = azimuth * M_PI/180;
   const Real stream_0 = u_0 * sin (theta) + v_0 * cos (theta);
   const Real stream_1 = u_1 * sin (theta) + v_1 * cos (theta);
   const Real stream_2 = u_2 * sin (theta) + v_2 * cos (theta);

   const Real z_theta_0 = model.get_z (k_theta_0, topography, A_theta, B_theta);
   const Real z_theta_1 = model.get_z (k_theta_1, topography, A_theta, B_theta);
   const Real z_theta_2 = model.get_z (k_theta_2, topography, A_theta, B_theta);

   const Real z_rho_0 = model.get_z (k_rho_0, topography, A_rho, B_rho);
   const Real z_rho_1 = model.get_z (k_rho_1, topography, A_rho, B_rho);
   const Real z_rho_2 = model.get_z (k_rho_2, topography, A_rho, B_rho);

   typedef Differentiation D;
   const Real dtheta_dz = D::d_1 (theta_0, theta_1,
      theta_2, z_theta_0, z_theta_1, z_theta_2);

   const Real A = (g / theta_1 * dtheta_dz ) / (stream_1 * stream_1);
   const Real B = -D::d2 (stream_0, stream_1, stream_2,
      z_rho_0, z_rho_1, z_rho_2) / stream_1;

   return A + B;

}

Real
Model::Uppers::Stage::evaluate_brunt_vaisala (const Lat_Long& lat_long,
                                              const Real z,
                                              const size_t l) const
{

   size_t i, j;
   acquire_ij (i, j, lat_long);

   const Real topography = get_topography (i, j);
   if (z < topography) { return GSL_NAN; }

   const Tuple& A_theta = model.vertical_coefficients.get_A_theta ();
   const Tuple& B_theta = model.vertical_coefficients.get_B_theta ();
   const Integer k = model.get_k (z, topography, A_theta, B_theta);
   if (k == 0 && k == 69) { return GSL_NAN; }

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
   return sqrt (g / theta_1 * dtheta_dz);

}

Real
Model::Uppers::Stage::evaluate (const Nwp_Element& nwp_element,
                                const Lat_Long& lat_long,
                                const Real z,
                                const size_t l) const
{

   size_t i, j;
   acquire_ij (i, j, lat_long);

   const Real topography = get_topography (i, j);
   if (z < topography) { return GSL_NAN; }

   const bool is_w = (nwp_element == W);
   const bool is_theta = ((nwp_element == THETA) || 
                          (nwp_element == Q) || 
                          (nwp_element == RHO) || 
                          (nwp_element == W) || 
                          (nwp_element == T) || 
                          (nwp_element == TD) || 
                          (nwp_element == RH) || 
                          (nwp_element == THETA_E));

   const Tuple& A_rho = model.vertical_coefficients.get_A_rho ();
   const Tuple& B_rho = model.vertical_coefficients.get_B_rho ();
   const Tuple& A_theta = model.vertical_coefficients.get_A_theta ();
   const Tuple& B_theta = model.vertical_coefficients.get_B_theta ();

   const Tuple& A = (is_theta ? A_theta : A_rho);
   const Tuple& B = (is_theta ? B_theta : B_rho);
   if (z > A.back ()) { return GSL_NAN; }

   const bool surface = (z < 0);
   const Integer k = surface ? -1 : model.get_k (z, topography, A, B);

   return evaluate (nwp_element, i, j, (is_w ? k + 1 : k), l);

}

Real
Model::Uppers::Stage::evaluate (const Nwp_Element& nwp_element,
                                const Lat_Long& lat_long,
                                const size_t k,
                                const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate (nwp_element, i, j, k, l);
}

Real
Model::Uppers::Stage::evaluate (const Nwp_Element& nwp_element,
                                const size_t i,
                                const size_t j,
                                const size_t k,
                                const size_t l) const
{

   int ret;
   float datum;

   switch (nwp_element)
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

      default:
      {

         Varname varname ("");

         switch (nwp_element)
         {
            case U:       { varname = string ("ml_xwind"); break; } 
            case V:       { varname = string ("ml_ywind"); break; } 
            case W:       { varname = string ("ml_zwind"); break; } 
            case THETA:   { varname = string ("ml_theta"); break; } 
            case Q:       { varname = string ("ml_spechum"); break; } 
            case P_THETA: { varname = string ("ml_ptheta"); break; } 
            case P_RHO:   { varname = string ("ml_prho"); break; } 
         }

         datum = evaluate_raw (varname, i, j, k, l);
         break;

      }

   }

   return Real (datum);

}

Real
Model::Uppers::Stage::evaluate_raw (const string& varname,
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

   const Real z = level.value;
   const Real topography = get_topography (lat_long);
   if (z < topography) { return transparent; }

   if (product == "WIND")
   {
      const Real u = evaluate (U, lat_long, z, l);
      const Real v = evaluate (V, lat_long, z, l);
      const Real speed = sqrt (u*u + v*v);
      const Real theta = atan2 (-u, -v);
      const Real hue = (theta < 0 ? theta + 2*M_PI : theta)/ (2*M_PI);
      const Real brightness = std::min (speed / 15, 1.0);
      return Color::hsb (hue, 0.8, brightness);
   }
   else
   {
      const Nwp_Element nwp_element = product.get_nwp_element ();
      const Real datum = evaluate (nwp_element, lat_long, z, l);
      return Display::get_color (product, datum);
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

Model::Vertical_Coefficients::Vertical_Coefficients (const string& file_path)
{
   init (file_path);
}

void
Model::Vertical_Coefficients::init (const string& file_path)
{

   string input_string;
   ifstream file (file_path.c_str ());

   while (getline (file, input_string))
   {

      if (input_string.size () == 0) { continue; }
      if (input_string.c_str ()[0] == '#') { continue; }

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

string
Model::get_nc_varname (const Varname& varname)
{
   if (varname == "orog") { return "ht"; }
   if (varname == "lsm") { return "lsm"; }
   if (varname == "temp") { return "temp"; }
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

const Integer
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
   string vertical_coefficients_file_path;

   map<string, Model::File_Path_Map> terrain_file_path_map;
   map<string, Model::File_Path_Map> surface_file_path_map;
   map<string, Model::File_Path_Map> uppers_file_path_map;

   for (auto iterator = config_file.begin ();
        iterator != config_file.end (); iterator++)
   {

      const Tokens tokens (*(iterator));
      if (tokens.size () != 2 || tokens[0] != "model") { continue; }

      const string& argument = tokens[1];
      const Tokens argument_tokens (argument, ":");
      const string& model_identifier = argument_tokens[0];

      if (model_identifier == "RUN" && argument_tokens.size () == 2)
      {
         basetime = Dtime (argument_tokens[1]);
         continue;
      }

      if (model_identifier == "AB" && argument_tokens.size () == 2)
      {
         vertical_coefficients_file_path = get_trimmed (argument_tokens[1]);
         continue;
      }

      if (model_identifier.substr (0, 5) == "STAGE" &&
          argument_tokens.size () == 3)
      {

         const string& stage_str = get_trimmed (argument_tokens[0]);
         const string& var_str = get_trimmed (argument_tokens[1]);
         const string& file_path = get_trimmed (argument_tokens[2]);

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
   terrain.init_stage (string ("STAGE3"), terrain_file_path_3_map);
   terrain.init_stage (string ("STAGE4"), terrain_file_path_4_map);
   terrain.init_stage (string ("STAGE5"), terrain_file_path_5_map);

   surface.init (stage_tokens);
   surface.init_stage (string ("STAGE3"), surface_file_path_3_map);
   surface.init_stage (string ("STAGE4"), surface_file_path_4_map);
   surface.init_stage (string ("STAGE5"), surface_file_path_5_map);

   uppers.init (stage_tokens);
   uppers.init_stage (string ("STAGE3"), uppers_file_path_3_map);
   uppers.init_stage (string ("STAGE4"), uppers_file_path_4_map);
   uppers.init_stage (string ("STAGE5"), uppers_file_path_5_map);

}

Model::~Model ()
{
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
Model::evaluate (const Nwp_Element& nwp_element,
                 const Lat_Long& lat_long,
                 const size_t k,
		 const Dtime& dtime,
                 const twiin::Stage& stage) const
{

   if (k < 0)
   {
      const Model::Surface::Stage& surface_stage = surface.get_stage (stage);
      const Integer l = surface_stage.get_l (dtime);
      return surface_stage.evaluate (nwp_element, lat_long, l);
   }
   else
   {
      const Model::Uppers::Stage& uppers_stage = uppers.get_stage (stage);
      const Integer l = uppers_stage.get_l (dtime);
      return uppers_stage.evaluate (nwp_element, lat_long, k, l);
   }
}

Real
Model::evaluate (const Nwp_Element& nwp_element,
                 const Lat_Long& lat_long,
                 const Level& level,
                 const Dtime& dtime,
                 const twiin::Stage& stage) const
{
   if (nwp_element == MSLP ||
       nwp_element == PRECIP_RATE ||
       nwp_element == FFDI ||
       level.type == SURFACE_LEVEL)
   {
      const Model::Surface::Stage& surface_stage = surface.get_stage (stage);
      const Integer l = surface_stage.get_l (dtime);
      return surface_stage.evaluate (nwp_element, lat_long, l);
   }
   else
   if (level.type == HEIGHT_LEVEL)
   {
      const Real z = level.value;
      const Model::Uppers::Stage& uppers_stage = uppers.get_stage (stage);
      const Integer l = uppers_stage.get_l (dtime);
      return uppers_stage.evaluate (nwp_element, lat_long, z, l);
   }
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

   surface_stage.fill_sounding (*sounding_ptr, lat_long, dtime);
   uppers_stage.fill_sounding (*sounding_ptr, lat_long, dtime);

   return sounding_ptr;

}

const set<Dtime>&
Model::get_valid_time_set (const Product& product,
                           const twiin::Stage& stage,
                           const Level& level) const
{

   if (product == "TERRAIN")
   {
      const Model::Surface::Stage& surface_stage = surface.get_stage (stage);
      return surface_stage.get_valid_time_set ();
   }
   else
   if (product == "T" ||
       product == "P_THETA" ||
       product == "P_RHO" ||
       product == "Q" ||
       product == "TD" ||
       product == "RH" ||
       product == "RHO" ||
       product == "WIND" ||
       product == "SPEED" ||
       product == "SPEED_HIGHER" ||
       product == "W" ||
       product == "W_TRANSLUCENT" ||
       product == "VORTICITY" ||
       product == "THETA" ||
       product == "THETA_E")
   {
      if (level.type == SURFACE_LEVEL)
      {
         const Model::Surface::Stage& surface_stage = surface.get_stage (stage);
         return surface_stage.get_valid_time_set ();
      }
      else
      if (level.type == HEIGHT_LEVEL)
      {
         const Model::Uppers::Stage& uppers_stage = uppers.get_stage (stage);
         return uppers_stage.get_valid_time_set ();
      }
   }
   else
   if (product == "FFDI" ||
       product == "MSLP" ||
       product == "PRECIP_RATE")
   {
      const Model::Surface::Stage& surface_stage = surface.get_stage (stage);
      surface_stage.get_valid_time_set ();
      return surface_stage.get_valid_time_set ();
   }
   else
   if (product == "W")
   {
      const Model::Uppers::Stage& uppers_stage = uppers.get_stage (stage);
      return uppers_stage.get_valid_time_set ();
   }

}

Tokens
Model::get_marker_tokens (const Lat_Long& lat_long,
                          const Dtime& dtime,
                          const Product& product,
                          const twiin::Stage& stage,
                          const Level& level) const
{

   Tokens tokens;

   if (product == "P_THETA")
   {
      const Real datum = evaluate (P_THETA, lat_long, level, dtime, stage);
      if (gsl_isnan (datum)) { return tokens; }
      tokens.push_back (string_render ("%.0fhPa", datum * 1e-2));
   }
   else
   if (product == "P_RHO")
   {
      const Real datum = evaluate (P_RHO, lat_long, level, dtime, stage);
      if (gsl_isnan (datum)) { return tokens; }
      tokens.push_back (string_render ("%.1fhPa", datum * 1e-2));
   }
   else
   if (product == "THETA")
   {
      const Real datum = evaluate (THETA, lat_long, level, dtime, stage);
      if (gsl_isnan (datum)) { return tokens; }
      tokens.push_back (string_render ("%.1f\u00b0C", datum - K));
   }
   else
   if (product == "Q")
   {
      const Real datum = evaluate (Q, lat_long, level, dtime, stage);
      if (gsl_isnan (datum)) { return tokens; }
      tokens.push_back (string_render ("%.3fg/kg", datum * 1e3));
   }
   else
   if (product == "T")
   {
      const Real datum = evaluate (T, lat_long, level, dtime, stage);
      if (gsl_isnan (datum)) { return tokens; }
      tokens.push_back (string_render ("%.1f\u00b0C", datum - K));
   }
   else
   if (product == "TD")
   {
      const Real t_d = evaluate (TD, lat_long, level, dtime, stage);
      if (gsl_isnan (t_d)) { return tokens; }
      tokens.push_back (string_render ("%.1f\u00b0C", t_d - K));
   }
   else
   if (product == "RH")
   {
      const Real rh = evaluate (RH, lat_long, level, dtime, stage);
      if (gsl_isnan (rh)) { return tokens; }
      tokens.push_back (string_render ("%.0f%%", rh * 100));
   }
   else
   if (product == "THETA_E")
   {
      const Real theta_e = evaluate (THETA_E, lat_long, level, dtime, stage);
      if (gsl_isnan (theta_e)) { return tokens; }
      tokens.push_back (string_render ("%.1f\u00b0C", theta_e - K));
   }
   else
   if (product == "RHO")
   {
      const Real rho = evaluate (RHO, lat_long, level, dtime, stage);
      if (gsl_isnan (rho)) { return tokens; }
      tokens.push_back (string_render ("%.2fkg/m3", rho));
   }
   else
   if (product == "WIND" ||
       product == "SPEED")
   {
      const Real u = evaluate (U, lat_long, level, dtime, stage);
      if (gsl_isnan (u)) { return tokens; }
      const Real v = evaluate (V, lat_long, level, dtime, stage);
      if (gsl_isnan (v)) { return tokens; }
      const Wind wind (u, v);
      const Real msknot = 3.6/1.852;
      const string fmt ("%03.0f\u00b0 / %02.1fkt");
      tokens.push_back (wind.get_string (msknot, fmt));
   }
   else
   if (product == "FFDI")
   {
      const Real ffdi = evaluate (FFDI, lat_long, level, dtime, stage);
      if (gsl_isnan (ffdi)) { return tokens; }
      string ffdr = "Low-Moderate";
      if (ffdi > 12) { ffdr = "High"; }
      if (ffdi > 25) { ffdr = "Very High"; }
      if (ffdi > 50) { ffdr = "Severe"; }
      if (ffdi > 75) { ffdr = "Extreme"; }
      if (ffdi > 100) { ffdr = "Catastrophic"; }
      tokens.push_back (string_render ("%02.2f", ffdi));
      tokens.push_back (ffdr);
   }
   else
   if (product == "TERRAIN")
   {
      const Model::Terrain::Stage& terrain_stage = terrain.get_stage (stage);
      const Real orog = terrain_stage.evaluate (string ("orog"), lat_long);
      if (gsl_isnan (orog)) { return tokens; }
      tokens.push_back (string_render ("%.2fm", orog));
   }
   else
   if (product == "W")
   {
      const Real w = evaluate (W, lat_long, level, dtime, stage);
      if (gsl_isnan (w)) { return tokens; }
      tokens.push_back (string_render ("%.2fm/s", w));
   }
   else
   if (product == "VORTICITY")
   {
      const Real zeta = evaluate (ZETA, lat_long, level, dtime, stage);
      if (gsl_isnan (zeta)) { return tokens; }
      tokens.push_back (string_render ("%.4e/s", zeta));
   }
   else
   if (product == "MSLP")
   {
      const Real mslp = evaluate (MSLP, lat_long, level, dtime, stage);
      if (gsl_isnan (mslp)) { return tokens; }
      tokens.push_back (string_render ("%0.1fhPa", mslp * 1e-2));
   }
   else
   if (product == "PRECIP_RATE")
   {
      const Real mmhr = evaluate (PRECIP_RATE, lat_long, level, dtime, stage);
      if (gsl_isnan (mmhr)) { return tokens; }
      tokens.push_back (string_render ("%0.1fmm/hr", mmhr * 3600));
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

      const Tokens tokens (*(iterator));

      if (tokens.size () == 2 && tokens[0] == "aws")
      {
         const string& aws_file_path = tokens[1];
         aws_repository.ingest (aws_file_path);
      }

      if (tokens.size () == 2 && tokens[0] == "stations")
      {
         const string& station_map_file_path = tokens[1];
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
Data::get_lat_long (const string& location_str) const
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

