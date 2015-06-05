#include <denise/dstring.h>
#include <denise/nwp.h>
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
cout << "insert " << varname << " " << file_path << endl;
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

}

Model::Terrain::Stage::~Stage ()
{
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
Model::Terrain::init2 (const twiin::Stage& twiin_stage,
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
   const set<Dtime>::const_iterator begin = valid_time_set.begin ();
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

      case RH:
      {
         const Real t = evaluate (T, i, j, l);
         const Real t_d = evaluate (TD, i, j, l);
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
         const Real t = evaluate (T, i, j, l);
         const Real mslp = evaluate (MSLP, i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         datum = Thermo_Point::t_p (t - K, surface_p).get_theta () + K;
         break;
      };

      case THETA_E:
      {
         typedef Thermo_Point Tp;
         const Real t = evaluate (T, i, j, l);
         const Real t_d = evaluate (TD, i, j, l);
         const Real mslp = evaluate (MSLP, i, j, l);
         const Real topography = get_topography (i, j);
         const Real surface_p = mslp - 11.76 * topography;
         datum = Tp::normand (t - K, t_d - K, surface_p).get_theta_e () + K;
         break;
      };

      case FFDI:
      {
         const Real t = evaluate (T, lat_long, l);
         const Real t_d = evaluate (TD, lat_long, l);
         const Real rh = Moisture::get_rh (t - K, t_d - K, WATER);
         const Real u = evaluate (U, lat_long, l);
         const Real v = evaluate (V, lat_long, l);
         const Real speed = sqrt (u*u + v*v);
         datum = Fire::get_ffdi (t - K, rh * 100, speed * 3.6);
      };

      default:
      {

         Varname varname ("");

         switch (nwp_element)
         {
            case T: { varname = string ("temp"); break; }
            case TD: { varname = string ("dewpt"); break; }
            case MSLP: { varname = string ("mslp"); break; }
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

Color
Model::Surface::Stage::get_color (const Product& product,
                                  const Lat_Long& lat_long,
                                  const size_t l) const
{

   // vertical index
   const Color transparent (0, 0, 0, 0);

   if (product == "T")
   {
      const Real t = evaluate (T, lat_long, l);
      const Real hue = Domain_1D (35 + K, 10 + K).normalize (t) * 0.833;
      return Color::hsb (hue, 0.8, 0.8);
   }
   else
   if (product == "TD")
   {
      const Real t_d = evaluate (TD, lat_long, l);
      const Real hue = Domain_1D (20 + K, -5 + K).normalize (t_d) * 0.833;
      return Color::hsb (hue, 0.8, 0.8);
   }
   else
   if (product == "RH")
   {
      const Real rh = evaluate (RH, lat_long, l);
      const Real hue = (rh < 0.5 ? 0.08 : 0.35);
      const Real saturation = std::min ((fabs (rh - 0.5) * 2), 1.0);
      return Color::hsb (hue, saturation, 1, 0.4);
   }
   else
   if (product == "FFDI")
   {
      const Ffdi_Color_Chooser ffdi_color_chooser (0.7);
      const Real ffdi = evaluate (FFDI, lat_long, l);
      return ffdi_color_chooser.get_color (ffdi);
   }
   else
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
   if (product == "VORTICITY")
   {
      const Real zeta = evaluate (ZETA, lat_long, l);
      const Real hue = (zeta < 0 ? 0.667 : 0.000);
      const Real modified_zeta = (log10 (fabs (zeta)) + 4) / 3;
      const Real saturation = std::max (std::min (modified_zeta, 1.0), 0.0);
      return Color::hsb (hue, saturation, 1, 1);
   }
   else
   if (product == "MSLP")
   {
      const Real mslp = evaluate (MSLP, lat_long, l);
      const Real hue = Domain_1D (990e2, 1025e2).normalize (mslp) * 0.833;
      return Color::hsb (hue, 0.8, 0.8);
   }
   else
   if (product == "THETA_E")
   {
      const Real theta_e = evaluate (THETA_E, lat_long, l);
      const Real hue = Domain_1D (65 + K, 5 + K).normalize (theta_e) * 0.833;
      return Color::hsb (hue, 0.8, 0.8);
   }
   else
   {
      return transparent;
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
Model::Surface::init2 (const twiin::Stage& twiin_stage,
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
Model::Uppers::Stage::evaluate (const Nwp_Element& nwp_element,
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
   if (z > A.back ()) { return GSL_NAN; }

   const bool surface = (z < 0);
   const Integer k = surface ? -1 : model.get_k (z, topography, A, B);

   return evaluate (nwp_element, i, j, k, l);

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

            case ZONAL_WIND:
            {
               varname = string ("ml_xwind");
               break;
            }

            case MERIDIONAL_WIND:
            {
               varname = string ("ml_ywind");
               break;
            }

            case VERTICAL_VELOCITY:
            {
               varname = string ("ml_zwind");
               break;
            }

            case THETA:
            {
               varname = string ("ml_theta");
               break;
            }

            case SPECIFIC_HUMIDITY:
            {
               varname = string ("ml_spechum");
               break;
            }

            case P_THETA:
            {
               varname = string ("ml_ptheta");
               break;
            }

            case P_RHO:
            {
               varname = string ("ml_prho");
               break;
            }

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

Color
Model::Uppers::Stage::get_color (const Product& product,
                                 const Lat_Long& lat_long,
                                 const Level& level,
                                 const size_t l) const
{

   // vertical index
   const Real z = level.value;
   const Color transparent (0, 0, 0, 0);

   const Model::Terrain::Stage& terrain_stage = model.terrain.get_stage (*this);

   const Real topography = get_topography (lat_long);
   if (z < topography) { return transparent; }

   if (product == "THETA")
   {
      const Real theta = evaluate (THETA, lat_long, z, l);
      const Real hue = Domain_1D (60 + K, 0 + K).normalize (theta) * 0.833;
      return Color::hsb (hue, 0.8, 0.8);
   }
   else
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
   if (product == "VORTICITY")
   {
      const Real zeta = evaluate (ZETA, lat_long, z, l);
      const Real hue = (zeta < 0 ? 0.667 : 0.000);
      const Real modified_zeta = (log10 (fabs (zeta)) + 4) / 3;
      const Real saturation = std::max (std::min (modified_zeta, 1.0), 0.0);
      return Color::hsb (hue, saturation, 1, 1);
   }
   else
   if (product == "T")
   {
      const Real t = evaluate (T, lat_long, z, l);
      const Real hue = Domain_1D (30 + K, -90 + K).normalize (t) * 0.833;
      return Color::hsb (hue, 0.8, 0.8);
   }
   else
   if (product == "TD")
   {
      const Real t_d = evaluate (TD, lat_long, z, l);
      const Real hue = Domain_1D (30 + K, -90 + K).normalize (t_d) * 0.833;
      return Color::hsb (hue, 0.8, 0.8);
   }
   else
   if (product == "RH")
   {
      const Real rh = evaluate (RH, lat_long, z, l);
      const Real hue = (rh < 0.5 ? 0.08 : 0.35);
      const Real saturation = std::min ((fabs (rh - 0.5) * 2), 1.0);
      return Color::hsb (hue, saturation, 1, 0.4);
   }
   else
   {
      return transparent;
   }

}

const Model::Uppers::Stage&
Model::Uppers::get_stage (const twiin::Stage& stage) const
{
   return (const Model::Uppers::Stage&)(stage_map.at (stage));
}

Model::Uppers::Stage&
Model::Uppers::get_stage (const twiin::Stage& stage)
{
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
Model::Uppers::init2 (const twiin::Stage& twiin_stage,
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
   if (varname == "zwind") { return "dz_dt"; }
   if (varname == "mslp") { return "p"; }
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


Model::Model (const Tokens& config_file_content)
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

   for (auto iterator = config_file_content.begin ();
        iterator != config_file_content.end (); iterator++)
   {

      const Tokens tokens (*(iterator));
      if (tokens.size () != 2 || tokens[0] != "model") { continue; }
      const string& argument = tokens[1];

      const Tokens unstaged_tokens (argument, ":");
      if (unstaged_tokens[0] == "AB" && unstaged_tokens.size () == 2)
      {
         vertical_coefficients_file_path = get_trimmed (unstaged_tokens[1]);
         continue;
      }

      const Tokens staged_tokens (argument, ":");
      if (staged_tokens[0].substr (0, 5) == "STAGE" &&
          staged_tokens.size () == 3)
      {
         const string& stage_str = get_trimmed (staged_tokens[0]);
         const string& var_str = get_trimmed (staged_tokens[1]);
         const string& file_path = get_trimmed (staged_tokens[2]);

         if (stage_str == "STAGE_3")
         {
            if (var_str == "orog" || var_str == "lsm")
            {
               terrain_file_path_3_map.insert (var_str, file_path);
            }
            else
            if (var_str.substr (0, 3) == "ml_")
            {
               uppers_file_path_3_map.insert (var_str, file_path);
            }
            else
            {
               surface_file_path_3_map.insert (var_str, file_path);
            }
         }
         else
         if (stage_str == "STAGE_4")
         {
            if (var_str == "orog" || var_str == "lsm")
            {
               terrain_file_path_4_map.insert (var_str, file_path);
            }
            else
            if (var_str.substr (0, 3) == "ml_")
            {
               uppers_file_path_4_map.insert (var_str, file_path);
            }
            else
            {
               surface_file_path_4_map.insert (var_str, file_path);
            }
         }
         else
         if (stage_str == "STAGE_5")
         {
            if (var_str == "orog" || var_str == "lsm")
            {
               terrain_file_path_5_map.insert (var_str, file_path);
            }
            else
            if (var_str.substr (0, 3) == "ml_")
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

   const Tokens stage_tokens ("STAGE_3 STAGE_4 STAGE_5");

   terrain.init (stage_tokens);
   terrain.init2 (string ("STAGE_3"), terrain_file_path_3_map);
   terrain.init2 (string ("STAGE_4"), terrain_file_path_4_map);
   terrain.init2 (string ("STAGE_5"), terrain_file_path_5_map);

   surface.init (stage_tokens);
   surface.init2 (string ("STAGE_3"), surface_file_path_3_map);
   surface.init2 (string ("STAGE_4"), surface_file_path_4_map);
   surface.init2 (string ("STAGE_5"), surface_file_path_5_map);

   uppers.init (stage_tokens);
   uppers.init2 (string ("STAGE_3"), uppers_file_path_3_map);
   uppers.init2 (string ("STAGE_4"), uppers_file_path_4_map);
   uppers.init2 (string ("STAGE_5"), uppers_file_path_5_map);

}

Model::~Model ()
{
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
      const Model::Uppers::Stage& upper_stage = uppers.get_stage (stage);
      const Integer l = upper_stage.get_l (dtime);
      return upper_stage.evaluate (nwp_element, lat_long, k, l);
   }
}

Real
Model::evaluate (const Nwp_Element& nwp_element,
                 const Lat_Long& lat_long,
                 const Level& level,
                 const Dtime& dtime,
                 const twiin::Stage& stage) const
{
   if (level.type == SURFACE_LEVEL)
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

const set<Dtime>&
Model::get_valid_time_set (const Product& product,
                           const twiin::Stage& stage,
                           const Level& level) const
{

   if (product == "TERRAIN")
   {
   }
   else
   if (product == "T" ||
       product == "P_THETA" ||
       product == "P_RHO" ||
       product == "Q" ||
       product == "TD" ||
       product == "RH" ||
       product == "WIND" ||
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
       product == "MSLP")
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
      tokens.push_back (string_render ("%.3fg/kg", datum * 1e-3));
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
   if (product == "WIND")
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
      const Real t = evaluate (T, lat_long, level, dtime, stage);
      const Real t_d = evaluate (TD, lat_long, level, dtime, stage);
      const Real u = evaluate (U, lat_long, level, dtime, stage);
      const Real v = evaluate (V, lat_long, level, dtime, stage);
      const Real rh = Moisture::get_rh (t - K, t_d - K, WATER);
      const Real speed = sqrt (u*u + v*v);
      const Real ffdi = Fire::get_ffdi (t - K, rh * 100, speed * 3.6);
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
   if (product == "VORTICITY")
   {
      const Real zeta = evaluate (ZETA, lat_long, level, dtime, stage);
      if (gsl_isnan (zeta)) { return tokens; }
      tokens.push_back (string_render ("%.4e", zeta));
   }
   else
   if (product == "MSLP")
   {
      const Real mslp = evaluate (MSLP, lat_long, level, dtime, stage);
      if (gsl_isnan (mslp)) { return tokens; }
      tokens.push_back (string_render ("%0.1fhPa", mslp * 1e-2));
   }

   return tokens;

}

