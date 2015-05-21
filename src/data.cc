#include "data.h"

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

Ffdi_Color_Chooser::Ffdi_Color_Chooser (const Real alpha)
   : alpha (alpha)
{
}

Color
Ffdi_Color_Chooser::get_color (const Real ffdi) const
{

   if (ffdi < 12)
   {
      const Real delta = ffdi;
      const Real r = 0.400 + delta * 0.03137;
      const Real g = 0.400 + delta * 0.04549;
      const Real b = 0.400 + delta * 0.03012;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 25)
   {
      const Real delta = ffdi - 10;
      const Real r = 0.278 + delta * 0.02133;
      const Real g = 0.400 + delta * 0.03075;
      const Real b = 0.500 + delta * 0.03859;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 50)
   {
      const Real delta = ffdi - 25;
      const Real r = 0.600 - delta * 0.02137;
      const Real g = 0.600 - delta * 0.02065;
      const Real b = 0.600 - delta * 0.02012;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 75)
   {
      const Real delta = ffdi - 50;
      const Real r = 0.549 + delta * 0.01757 * 2;
      const Real g = 0.549 + delta * 0.01757 * 2;
      const Real b = 0.329 + delta * 0.02133 * 2;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 100)
   {
      const Real delta = ffdi - 75;
      const Real r = 0.097 + delta * 0.01553 * 2;
      const Real g = 0.357 + delta * 0.00925 * 2;
      const Real b = 0.000 + delta * 0.00000 * 2;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 150)
   {
      const Real delta = ffdi - 100;
      const Real r = 0.647 + delta * 0.01396;
      const Real g = 0.000 + delta * 0.00000;
      const Real b = 0.000 + delta * 0.00000;
      return Color (r, g, b, alpha);
   }
   else
   if (ffdi < 200)
   {
      const Real delta = ffdi - 150;
      const Real r = 0.698 + delta * 0.01176;
      const Real g = 0.000 + delta * 0.00000;
      const Real b = 0.463 + delta * 0.00784;
      return Color (r, g, b, alpha);
   }
   else
   {
      return Color (0, 0, 0, 0);
   }

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

Station::Map::Map (const string& file_path)
   : stage_3 (-47.2, -20.236, 132.416, 168.6824),
     stage_4 (-39.46, -27.952, 143.648, 157.4648),
     stage_5 (-35.38, -32.024, 148.544, 152.5712)
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

void
Station::Map::cairo (const RefPtr<Context>& cr,
                     const Transform_2D& transform) const
{

   const Color stage_5_color = Color::hsb (0.00, 0.9, 0.9, 0.2);
   const Color stage_4_color = Color::hsb (0.33, 0.9, 0.9, 0.2);
   const Color stage_3_color = Color::hsb (0.67, 0.9, 0.9, 0.2);

   for (Station::Map::const_iterator iterator = begin ();
        iterator != end (); iterator++)
   {

      const Station& station = iterator->second;
                                                stage_5_color.cairo (cr);
      if (stage_5.is_out_of_bounds (station)) { stage_4_color.cairo (cr); }
      if (stage_4.is_out_of_bounds (station)) { stage_3_color.cairo (cr); }

      Ring (1).cairo (cr, transform.transform (station));
      cr->fill ();

   }

}

void
Station::Map::render_stages (const RefPtr<Context>& cr,
                             const Transform_2D& transform) const
{

   cr->set_line_width (4);
   Color::hsb (0.00, 0.00, 0.00, 0.5).cairo (cr);

   Polygon (stage_3).cairo (cr, transform);
   cr->stroke ();

   Polygon (stage_4).cairo (cr, transform);
   cr->stroke ();

   Polygon (stage_5).cairo (cr, transform);
   cr->stroke ();

}

void
Terrain::Stage::acquire_ij (size_t& i,
                            size_t& j,
                            const Real latitude,
                            const Real longitude) const
{
   i = Grid_nD::get_nearest_node (tuple_latitude, GSL_NAN, latitude);
   j = Grid_nD::get_nearest_node (tuple_longitude, GSL_NAN, longitude);
}

Terrain::Stage::Stage (const twiin::Stage& stage,
                       const string& orog_file_path,
                       const string& lsm_file_path)
   : twiin::Stage (stage),
     orog_file (orog_file_path),
     lsm_file (lsm_file_path),
     tuple_latitude (orog_file.get_coordinate_tuple ("latitude")),
     tuple_longitude (orog_file.get_coordinate_tuple ("longitude"))
{

   int ret;
   const size_t n = tuple_latitude.size () * tuple_longitude.size ();

   const Integer orog_ncid = orog_file.get_nc_id ();
   const Integer lsm_ncid = lsm_file.get_nc_id ();

   ret = nc_inq_varid (orog_ncid, "ht", &orog_varid);
   if (ret != NC_NOERR) { throw Exception ("nc_inq_varid orog"); }
   ret = nc_inq_varid (lsm_ncid, "lsm", &lsm_varid);
   if (ret != NC_NOERR) { throw Exception ("nc_inq_varid lsm"); }

}

Terrain::Stage::~Stage ()
{
}

bool
Terrain::Stage::out_of_bounds (const Real latitude,
                               const Real longitude) const
{
   return latitude < tuple_latitude.front () ||
          latitude > tuple_latitude.back () ||
          longitude < tuple_longitude.front () ||
          longitude > tuple_longitude.back ();
}


Real
Terrain::Stage::get_orog (const size_t& i,
                          const size_t& j) const
{

   float datum;
   size_t index[] = { i, j, 0, 0 };

   const Integer ncid = orog_file.get_nc_id ();
   int ret = nc_get_var1 (ncid, orog_varid, index, &datum);
   if (ret != NC_NOERR) { throw Exception ("nc_get_var1 orog"); }
   return Real (datum);

}

Real
Terrain::Stage::get_lsm (const size_t& i,
                         const size_t& j) const
{

   float datum;
   size_t index[] = { i, j, 0, 0 };

   const Integer ncid = lsm_file.get_nc_id ();
   int ret = nc_get_var1 (ncid, lsm_varid, index, &datum);
   if (ret != NC_NOERR) { throw Exception ("nc_get_var1 lsm"); }
   return Real (datum);

}

void
Terrain::Stage::acquire_orog_lsm (Real& orog,
                                  Real& lsm,
                                  const Real latitude,
                                  const Real longitude) const
{

   int ret;
   float datum;
   size_t index[] = { 0, 0, 0, 0 };
   acquire_ij (index[2], index[3], latitude, longitude);

   const Integer orog_ncid = orog_file.get_nc_id ();
   ret = nc_get_var1 (orog_ncid, orog_varid, index, &datum);
   if (ret != NC_NOERR) { throw Exception ("nc_get_var1 orog"); }
   orog = Real (datum);

   const Integer lsm_ncid = lsm_file.get_nc_id ();
   ret = nc_get_var1 (lsm_ncid, lsm_varid, index, &datum);
   if (ret != NC_NOERR) { throw Exception ("nc_get_var1 lsm"); }
   lsm = Real (datum);

}

Real
Terrain::Stage::get_orog (const Real latitude,
                          const Real longitude) const
{
   size_t i, j;
   acquire_ij (i, j, latitude, longitude);
   return get_orog (i, j);
}

Real
Terrain::Stage::get_lsm (const Real latitude,
                         const Real longitude) const
{
   size_t i, j;
   acquire_ij (i, j, latitude, longitude);
   return get_lsm (i, j);
}

const Terrain::Stage&
Terrain::get_terrain_stage (const twiin::Stage& stage) const
{
   if (stage == "STAGE_3") { return stage_3; }
   else
   if (stage == "STAGE_4") { return stage_4; }
   else
   if (stage == "STAGE_5") { return stage_5; }
}

Terrain::Terrain (const string& orog_3_file_path,
                  const string& lsm_3_file_path,
                  const string& orog_4_file_path,
                  const string& lsm_4_file_path,
                  const string& orog_5_file_path,
                  const string& lsm_5_file_path)
   : stage_3 (twiin::Stage ("STAGE_3"), orog_3_file_path, lsm_3_file_path),
     stage_4 (twiin::Stage ("STAGE_4"), orog_4_file_path, lsm_4_file_path),
     stage_5 (twiin::Stage ("STAGE_5"), orog_5_file_path, lsm_5_file_path)
{
}

Raster*
Terrain::get_raster_ptr (const Size_2D& size_2d,
                         const Transform_2D& transform,
                         const twiin::Stage& stage) const
{

   Raster* raster_ptr = new Raster (size_2d);
   Raster& raster = *raster_ptr;

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   Real orog, lsm;
   const Color transparent (0, 0, 0, 0);
   const Terrain::Stage& terrain_stage = get_terrain_stage (stage);

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i); 

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j); 
         transform.reverse (latitude, longitude, x, y);

         if (terrain_stage.out_of_bounds (latitude, longitude))
         {
            raster.set_pixel (i, j, transparent);
            continue;
         }

         terrain_stage.acquire_orog_lsm (orog, lsm, latitude, longitude);
         const Real h = std::min (std::max (orog / 2000.0, 0.0), 1.0);

         const bool land = (lsm > 0.5);
         const Real hue = (land ? 0.4 - h * 0.4 : 0.67);
         const Real brightness = h * 0.7 + 0.28;

         const Color& color = Color::hsb (hue, 0.34, brightness);
         raster.set_pixel (i, j, color);

      }

   }

   return raster_ptr;

}

void
Terrain::cairo (const RefPtr<Context>& cr,
                const Transform_2D& transform,
                const Size_2D& size_2d,
                const twiin::Stage& stage) const
{
   Raster* raster_ptr = get_raster_ptr (size_2d, transform, stage);
   raster_ptr->blit (cr);
   delete raster_ptr;
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
Model::Stage::fill_valid_time_set ()
{

   int ret, dim_id, varid;
   nc_type var_type;
   size_t n;

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

Model::Stage::Stage (const twiin::Stage& stage,
                     const map<Model::Varname, string>& file_path_map)
   : twiin::Stage (stage)
{

   int ret;
   int varid;

   typedef map<Model::Varname, string>::const_iterator Iterator;
   for (Iterator iterator = file_path_map.begin ();
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

   fill_valid_time_set ();

}

Model::Stage::~Stage ()
{
   typedef map<Model::Varname, Nc_File*>::iterator Iterator;
   for (Iterator iterator = nc_file_ptr_map.begin ();
        iterator != nc_file_ptr_map.end (); iterator++)
   {
      Nc_File* nc_file_ptr = iterator->second;
      delete nc_file_ptr;
   }

}

const set<Dtime>&
Model::Stage::get_valid_time_set () const
{
   return valid_time_set;
}

Integer
Model::Stage::get_l (const Dtime& dtime) const
{
   const set<Dtime>::const_iterator begin = valid_time_set.begin ();
   return std::distance (begin, valid_time_set.find (dtime));
}

bool
Model::Stage::out_of_bounds (const Real latitude,
                             const Real longitude) const
{
   return latitude < tuple_latitude.front () ||
          latitude > tuple_latitude.back () ||
          longitude < tuple_longitude.front () ||
          longitude > tuple_longitude.back ();
}

Real
Model::Stage::evaluate (const Nwp_Element& nwp_element,
                        const Real latitude,
                        const Real longitude,
                        const size_t k,
                        const size_t l) const
{

   const Real& x = latitude;
   const Real& y = longitude;
   const size_t i = Grid_nD::get_nearest_node (tuple_latitude, GSL_NAN, x);
   const size_t j = Grid_nD::get_nearest_node (tuple_longitude, GSL_NAN, y);

   return evaluate (nwp_element, i, j, k, l);

}

Real
Model::Stage::evaluate (const Nwp_Element& nwp_element,
                        const size_t i,
                        const size_t j,
                        const size_t k,
                        const size_t l) const
{

   int ret;
   float datum;
   const size_t index[] = { l, k, i, j };

   switch (nwp_element)
   {

      case WIND_SPEED:
      {
         const Real u = evaluate (ZONAL_WIND, i, j, k, l);
         const Real v = evaluate (MERIDIONAL_WIND, i, j, k, l);
         datum = sqrt (u*u + v*v);
         break;
      };

      case RELATIVE_HUMIDITY:
      {
         const Real t = evaluate (TEMPERATURE, i, j, k, l);
         const Real t_d = evaluate (DEW_POINT, i, j, k, l);
         datum = Moisture::get_rh (t - K, t_d - K, WATER);
         break;
      };

      case FFDI:
      {
         const Real t = evaluate (TEMPERATURE, i, j, k, l);
         const Real t_d = evaluate (DEW_POINT, i, j, k, l);
         const Real rh = Moisture::get_rh (t - K, t_d - K, WATER);
         const Real speed = evaluate (WIND_SPEED, i, j, k, l);
         datum = Fire::get_ffdi (t - K, rh * 100, speed * 3.6);
         break;
      };

      case RELATIVE_VORTICITY:
      {
         if (i <= 0 || i >= tuple_latitude.size () - 1 ||
             j <= 0 || j >= tuple_longitude.size () - 1) { return GSL_NAN; }
         const Real upper_u = evaluate (ZONAL_WIND, i+1, j, k, l);
         const Real lower_u = evaluate (ZONAL_WIND, i-1, j, k, l);
         const Real upper_v = evaluate (MERIDIONAL_WIND, i, j+1, k, l);
         const Real lower_v = evaluate (MERIDIONAL_WIND, i, j-1, k, l);
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

      case THETA_E:
      {
         typedef Thermo_Point Tp;
         const Real t = evaluate (TEMPERATURE, i, j, k, l);
         const Real t_d = evaluate (DEW_POINT, i, j, k, l);
         const Real mslp = evaluate (MEAN_SEA_LEVEL_PRESSURE, i, j, k, l);
         //const Real orog = terrain.get_orog (i, j);
         //const Real surface_p = mslp - 11.76 * orog;
         const Real surface_p = mslp;
         datum = Tp::normand (t - K, t_d - K, surface_p).get_theta_e () + K;
         break;
      };

      default:
      {

         Varname varname ("");
         switch (nwp_element)
         {
            case TEMPERATURE:             varname = string ("temp"); break;
            case DEW_POINT:               varname = string ("dewpt"); break;
            case MEAN_SEA_LEVEL_PRESSURE: varname = string ("mslp"); break;
            case ZONAL_WIND:              varname = string ("xwind"); break;
            case MERIDIONAL_WIND:         varname = string ("ywind"); break;
         }

         const Nc_File& nc_file = *(nc_file_ptr_map.at (varname));
         const Integer nc_id = nc_file.get_nc_id ();
         const Integer varid = varid_map.at (varname);
         ret = nc_get_var1 (nc_id, varid, index, &datum);
         if (ret != NC_NOERR)
         {
            throw Exception ("nc_get_var1");
         }
         break;
      }

   }

   return Real (datum);

}

Color
Model::Stage::get_color (const Product& product,
                         const Lat_Long& lat_long,
                         const Real z,
                         const Integer l) const
{

   // vertical index
   const Color transparent (0, 0, 0, 0);
   const Real& latitude = lat_long.latitude;
   const Real& longitude = lat_long.longitude;

   if (product == "T")
   {
      const Integer k = 0;
      const Real t = evaluate (TEMPERATURE, latitude, longitude, k, l);
      const Real hue = Domain_1D (35 + K, 10 + K).normalize (t) * 0.833;
      return Color::hsb (hue, 0.8, 0.8);
   }
   else
   if (product == "TD")
   {
      const Integer k = 0;
      const Real t_d = evaluate (DEW_POINT, latitude, longitude, k, l);
      const Real hue = Domain_1D (20 + K, -5 + K).normalize (t_d) * 0.833;
      return Color::hsb (hue, 0.8, 0.8);
   }
   else
   if (product == "RH")
   {
      const Integer k = 0;
      const Real rh = evaluate (RELATIVE_HUMIDITY, latitude, longitude, k, l);
      const Real hue = (rh < 0.5 ? 0.08 : 0.35);
      const Real saturation = std::min ((fabs (rh - 0.5) * 2), 1.0);
      return Color::hsb (hue, saturation, 1, 0.4);
   }
   else
   if (product == "FFDI")
   {
      const Integer k = 0;
      const Ffdi_Color_Chooser ffdi_color_chooser (0.7);
      const Real ffdi = evaluate (FFDI, latitude, longitude, k, l);
      return ffdi_color_chooser.get_color (ffdi);
   }
   else
   if (product == "WIND")
   {
      const Integer k = 0;
      const Real u = evaluate (ZONAL_WIND, latitude, longitude, 0, l);
      const Real v = evaluate (MERIDIONAL_WIND, latitude, longitude, 0, l);
      const Real speed = sqrt (u*u + v*v);
      const Real theta = atan2 (-u, -v);
      const Real hue = (theta < 0 ? theta + 2*M_PI : theta)/ (2*M_PI);
      const Real brightness = std::min (speed / 15, 1.0);
      return Color::hsb (hue, 0.8, brightness);

   }
   else
   if (product == "VORTICITY")
   {
      const Integer k = 0;
      const Real z = evaluate (RELATIVE_VORTICITY, latitude, longitude, k, l);
      const Real hue = (z < 0 ? 0.667 : 0.000);
      const Real modified_z = (log10 (fabs (z)) + 4) / 3;
      const Real saturation = std::max (std::min (modified_z, 1.0), 0.0);
      return Color::hsb (hue, saturation, 1, 1);
   }
   else
   if (product == "MSLP")
   {
      const Integer k = 0;
      const Real mslp = evaluate (MEAN_SEA_LEVEL_PRESSURE, latitude, longitude, k, l);
      const Real hue = Domain_1D (990e2, 1025e2).normalize (mslp) * 0.833;
      return Color::hsb (hue, 0.8, 0.8);
   }
   else
   if (product == "THETA_E")
   {
      const Integer k = 0;
      const Real theta_e = evaluate (THETA_E, latitude, longitude, k, l);
      const Real hue = Domain_1D (65 + K, 5 + K).normalize (theta_e) * 0.833;
      return Color::hsb (hue, 0.8, 0.8);
   }
   else
   {
      return transparent;
   }

}

string
Model::get_nc_varname (const Varname& varname)
{
   if (varname == "temp") { return "temp"; }
   if (varname == "dewpt") { return "field17"; }
   if (varname == "xwind") { return "x-wind"; }
   if (varname == "ywind") { return "y-wind"; }
   if (varname == "zwind") { return "dz_dt"; }
   if (varname == "mslp") { return "p"; }
   if (varname == "prho") { return "p"; }
   if (varname == "ptheta") { return "p"; }
   if (varname == "prcp8p5") { return "precip"; }
   if (varname == "spechum") { return "q"; }
   if (varname == "theta") { return "theta"; }
   throw Exception ("invalid varname: " + varname);
}

const Model::Stage&
Model::get_model_stage (const twiin::Stage& stage) const
{
   if (stage == "STAGE_3") { return stage_3; }
   else
   if (stage == "STAGE_4") { return stage_4; }
   else
   if (stage == "STAGE_5") { return stage_5; }
}

Model::Model (const map<Model::Varname, string>& file_path_3_map,
              const map<Model::Varname, string>& file_path_4_map,
              const map<Model::Varname, string>& file_path_5_map)
   : stage_3 (twiin::Stage ("STAGE_3"), file_path_3_map),
     stage_4 (twiin::Stage ("STAGE_4"), file_path_4_map),
     stage_5 (twiin::Stage ("STAGE_5"), file_path_5_map)
{
}

Model::~Model ()
{
}

Raster*
Model::get_raster_ptr (const Product& product,
                       const Dtime& dtime,
                       const Size_2D& size_2d,
                       const Transform_2D& transform,
                       const twiin::Stage& stage) const
{

   Raster* raster_ptr = new Raster (size_2d);
   Raster& raster = *raster_ptr;

   const Model::Stage& model_stage = get_model_stage (stage);
   const Integer l = model_stage.get_l (dtime);
cout << dtime.get_string () << " " << l << endl;

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   const Real z = 0;

cout << "size_2d = " << size_2d << endl;
   const Color transparent (0, 0, 0, 0);

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i); 

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j); 
         transform.reverse (latitude, longitude, x, y);

         if (model_stage.out_of_bounds (latitude, longitude))
         {
            raster.set_pixel (i, j, transparent);
            continue;
         }

         const Color& color = model_stage.get_color (product, lat_long, z, l);
         raster.set_pixel (i, j, color);

      }

   }

   return raster_ptr;

}

Real
Model::evaluate (const Nwp_Element& nwp_element,
                 const Real latitude,
                 const Real longitude,
                 const size_t k,
		 const Dtime& dtime,
                 const twiin::Stage& stage) const
{
   const Model::Stage& model_stage = get_model_stage (stage);
   const Integer l = model_stage.get_l (dtime);
   return model_stage.evaluate (nwp_element, latitude, longitude, k, l);
}

