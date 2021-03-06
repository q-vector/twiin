#include <denise/dstring.h>
#include <denise/gzstream.h>
#include <denise/nwp.h>
#include "model.h"

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;

Model::Product::Product (const Product::Enumeration& enumeration)
   : enumeration (enumeration)
{
}

Model::Product::Product (const Product& product)
   : enumeration (product.enumeration)
{
}

Model::Product::Product (const Dstring& str)
{
        if (str == "TERRAIN")            { enumeration = Product::TERRAIN; }
   else if (str == "TERRAIN_MONOCHROME") { enumeration = Product::TERRAIN_MONOCHROME; }
   else if (str == "TERRAIN_WATER")      { enumeration = Product::TERRAIN_WATER; }
   else if (str == "P_THETA")            { enumeration = Product::P_THETA; }
   else if (str == "P_RHO")              { enumeration = Product::P_RHO; }
   else if (str == "ALONG_SPEED")        { enumeration = Product::ALONG_SPEED; }
   else if (str == "NORMAL_SPEED")       { enumeration = Product::NORMAL_SPEED; }
   else if (str == "BRUNT_VAISALA")      { enumeration = Product::BRUNT_VAISALA; }
   else if (str == "RICHARDSON")         { enumeration = Product::RICHARDSON; }
   else if (str == "SCORER")             { enumeration = Product::SCORER; }
   else if (str == "SCORER_A")           { enumeration = Product::SCORER_A; }
   else if (str == "SCORER_B")           { enumeration = Product::SCORER_B; }
   else if (str == "T")                  { enumeration = Product::T; }
   else if (str == "TD")                 { enumeration = Product::TD; }
   else if (str == "RH")                 { enumeration = Product::RH; }
   else if (str == "THETA")              { enumeration = Product::THETA; }
   else if (str == "THETA_MONOCHROME")   { enumeration = Product::THETA_MONOCHROME; }
   else if (str == "THETA_28C")          { enumeration = Product::THETA_28C; }
   else if (str == "Q")                  { enumeration = Product::Q; }
   else if (str == "Q_MONOCHROME")       { enumeration = Product::Q_MONOCHROME; }
   else if (str == "Q_3GKG")             { enumeration = Product::Q_3GKG; }
   else if (str == "THETA_E")            { enumeration = Product::THETA_E; }
   else if (str == "THETA_V")            { enumeration = Product::THETA_V; }
   else if (str == "RHO")                { enumeration = Product::RHO; }
   else if (str == "WIND")               { enumeration = Product::WIND; }
   else if (str == "SPEED")              { enumeration = Product::SPEED; }
   else if (str == "SPEED_HIGHER")       { enumeration = Product::SPEED_HIGHER; }
   else if (str == "VORTICITY")          { enumeration = Product::VORTICITY; }
   else if (str == "W")                  { enumeration = Product::W; }
   else if (str == "W_TRANSLUCENT")      { enumeration = Product::W_TRANSLUCENT; }
   else if (str == "Q_TENDENCY")         { enumeration = Product::Q_TENDENCY; }
   else if (str == "Q_L_TENDENCY")       { enumeration = Product::Q_L_TENDENCY; }
   else if (str == "Q_ADVECTION")        { enumeration = Product::Q_ADVECTION; }
   else if (str == "Q_H_ADVECTION")      { enumeration = Product::Q_H_ADVECTION; }
   else if (str == "Q_V_ADVECTION")      { enumeration = Product::Q_V_ADVECTION; }
   else if (str == "Q_S_ADVECTION")      { enumeration = Product::Q_S_ADVECTION; }
   else if (str == "Q_N_ADVECTION")      { enumeration = Product::Q_N_ADVECTION; }
   else if (str == "Q_SV_ADVECTION")     { enumeration = Product::Q_SV_ADVECTION; }
   else if (str == "Q_NV_ADVECTION")     { enumeration = Product::Q_NV_ADVECTION; }
   else if (str == "THETA_TENDENCY")     { enumeration = Product::THETA_TENDENCY; }
   else if (str == "THETA_L_TENDENCY")   { enumeration = Product::THETA_L_TENDENCY; }
   else if (str == "THETA_ADVECTION")    { enumeration = Product::THETA_ADVECTION; }
   else if (str == "THETA_H_ADVECTION")  { enumeration = Product::THETA_H_ADVECTION; }
   else if (str == "THETA_V_ADVECTION")  { enumeration = Product::THETA_V_ADVECTION; }
   else if (str == "THETA_S_ADVECTION")  { enumeration = Product::THETA_S_ADVECTION; }
   else if (str == "THETA_N_ADVECTION")  { enumeration = Product::THETA_N_ADVECTION; }
   else if (str == "THETA_SV_ADVECTION") { enumeration = Product::THETA_SV_ADVECTION; }
   else if (str == "THETA_NV_ADVECTION") { enumeration = Product::THETA_NV_ADVECTION; }
   else if (str == "FFDI")               { enumeration = Product::FFDI; }
   else if (str == "MSLP")               { enumeration = Product::MSLP; }
   else if (str == "PRECIP_RATE")        { enumeration = Product::PRECIP_RATE; }
   else if (str == "IR1")                { enumeration = Product::IR1; }
   else if (str == "IR2")                { enumeration = Product::IR2; }
   else if (str == "IR3")                { enumeration = Product::IR3; }
   else if (str == "IR4")                { enumeration = Product::IR4; }
   else if (str == "VIS")                { enumeration = Product::VIS; }
   else if (str == "Pseudo")             { enumeration = Product::Pseudo; }
   else                                  { enumeration = Product::NIL; }
}

Dstring
Model::Product::get_string () const
{
   switch (enumeration)
   {
      case Product::TERRAIN:            return "TERRAIN";
      case Product::TERRAIN_MONOCHROME: return "TERRAIN";
      case Product::TERRAIN_WATER:      return "TERRAIN_WATER";
      case Product::P_THETA:            return "P_THETA";
      case Product::P_RHO:              return "P_RHO";
      case Product::ALONG_SPEED:        return "ALONG_SPEED";
      case Product::NORMAL_SPEED:       return "NORMAL_SPEED";
      case Product::BRUNT_VAISALA:      return "BRUNT_VAISALA";
      case Product::RICHARDSON:         return "RICHARDSON";
      case Product::SCORER:             return "SCORER";
      case Product::SCORER_A:           return "SCORER_A";
      case Product::SCORER_B:           return "SCORER_B";
      case Product::T:                  return "T";
      case Product::TD:                 return "TD";
      case Product::RH:                 return "RH";
      case Product::THETA:              return "THETA";
      case Product::THETA_MONOCHROME:   return "THETA";
      case Product::THETA_28C:          return "THETA";
      case Product::Q:                  return "Q";
      case Product::Q_MONOCHROME:       return "Q";
      case Product::Q_3GKG:             return "Q";
      case Product::THETA_E:            return "THETA_E";
      case Product::THETA_V:            return "THETA_V";
      case Product::RHO:                return "RHO";
      case Product::WIND:               return "WIND";
      case Product::SPEED:              return "SPEED";
      case Product::SPEED_HIGHER:       return "SPEED_HIGHER";
      case Product::VORTICITY:          return "VORTICITY";
      case Product::W:                  return "W";
      case Product::W_TRANSLUCENT:      return "W_TRANSLUCENT";
      case Product::Q_TENDENCY:         return "Q_TENDENCY";
      case Product::Q_L_TENDENCY:       return "Q_L_TENDENCY";
      case Product::Q_ADVECTION:        return "Q_ADVECTION";
      case Product::Q_H_ADVECTION:      return "Q_H_ADVECTION";
      case Product::Q_V_ADVECTION:      return "Q_V_ADVECTION";
      case Product::Q_S_ADVECTION:      return "Q_S_ADVECTION";
      case Product::Q_N_ADVECTION:      return "Q_N_ADVECTION";
      case Product::Q_SV_ADVECTION:     return "Q_SV_ADVECTION";
      case Product::Q_NV_ADVECTION:     return "Q_NV_ADVECTION";
      case Product::THETA_TENDENCY:     return "THETA_TENDENCY";
      case Product::THETA_L_TENDENCY:   return "THETA_L_TENDENCY";
      case Product::THETA_ADVECTION:    return "THETA_ADVECTION";
      case Product::THETA_H_ADVECTION:  return "THETA_H_ADVECTION";
      case Product::THETA_V_ADVECTION:  return "THETA_V_ADVECTION";
      case Product::THETA_S_ADVECTION:  return "THETA_S_ADVECTION";
      case Product::THETA_N_ADVECTION:  return "THETA_N_ADVECTION";
      case Product::THETA_SV_ADVECTION: return "THETA_SV_ADVECTION";
      case Product::THETA_NV_ADVECTION: return "THETA_NV_ADVECTION";
      case Product::FFDI:               return "FFDI";
      case Product::MSLP:               return "MSLP";
      case Product::PRECIP_RATE:        return "PRECIP_RATE";
      case Product::IR1:                return "IR1";
      case Product::IR2:                return "IR2";
      case Product::IR3:                return "IR3";
      case Product::IR4:                return "IR4";
      case Product::VIS:                return "VIS";
      case Product::Pseudo:             return "Pseudo";
      default:                          return "";
   }
}

Dstring
Model::Product::get_description () const
{
   switch (enumeration)
   {
      case Product::TERRAIN:            return "Terrain";
      case Product::TERRAIN_MONOCHROME: return "Terrain";
      case Product::TERRAIN_WATER:      return "Terrain Water";
      case Product::P_THETA:            return "P_\u03b8";
      case Product::P_RHO:              return "P_\u03c1";
      case Product::ALONG_SPEED:        return "Along Speed";
      case Product::NORMAL_SPEED:       return "Normal Speed";
      case Product::BRUNT_VAISALA:      return "Brunt-V\u00e4is\u00e4L\u00e4";
      case Product::RICHARDSON:         return "Richardson Number";
      case Product::SCORER:             return "Scorer Parameter";
      case Product::SCORER_A:           return "Scorer Parameter Term A";
      case Product::SCORER_B:           return "Scorer Parameter Term B";
      case Product::T:                  return "Temperature";
      case Product::TD:                 return "Dew Point";
      case Product::RH:                 return "Relative Humidity";
      case Product::THETA:              return "Potential Temperature";
      case Product::THETA_MONOCHROME:   return "Potential Temperature";
      case Product::THETA_28C:          return "Potential Temperature";
      case Product::Q:                  return "Specific Humidity";
      case Product::Q_MONOCHROME:       return "Specific Humidity";
      case Product::Q_3GKG:             return "Specific Humidity";
      case Product::THETA_E:            return "Equivalent Potential Temperature";
      case Product::THETA_V:            return "Virtual Potential Temperature";
      case Product::RHO:                return "Density";
      case Product::WIND:               return "Wind";
      case Product::SPEED:              return "Speed";
      case Product::SPEED_HIGHER:       return "Higher Speed";
      case Product::VORTICITY:          return "Vertical Vorticity";
      case Product::W:                  return "Vertical Velocity";
      case Product::W_TRANSLUCENT:      return "Vertical Velocity";
      case Product::Q_TENDENCY:         return "Q Tendency";
      case Product::Q_L_TENDENCY:       return "Q Lagrangian Tendency";
      case Product::Q_ADVECTION:        return "Q Advection";
      case Product::Q_H_ADVECTION:      return "Horizontal Q Advection";
      case Product::Q_V_ADVECTION:      return "Vertical Q Advection";
      case Product::Q_S_ADVECTION:      return "Stream Q Advection";
      case Product::Q_N_ADVECTION:      return "Normal Q Advection";
      case Product::Q_SV_ADVECTION:     return "Stream + Vertical Q Advection";
      case Product::Q_NV_ADVECTION:     return "Normal + Vertical Q Advection";
      case Product::THETA_TENDENCY:     return "THETA Tendency";
      case Product::THETA_L_TENDENCY:   return "THETA Lagrangian Tendency";
      case Product::THETA_ADVECTION:    return "THETA Advection";
      case Product::THETA_H_ADVECTION:  return "Horizontal THETA Advection";
      case Product::THETA_V_ADVECTION:  return "Vertical THETA Advection";
      case Product::THETA_S_ADVECTION:  return "Stream THETA Advection";
      case Product::THETA_N_ADVECTION:  return "Normal THETA Advection";
      case Product::THETA_SV_ADVECTION: return "Stream + Vertical THETA Advection";
      case Product::THETA_NV_ADVECTION: return "Normal + Vertical THETA Advection";
      case Product::FFDI:               return "FFDI";
      case Product::MSLP:               return "MSLP";
      case Product::PRECIP_RATE:        return "Precipitation Rate";
      case Product::IR1:                return "IR1";
      case Product::IR2:                return "IR2";
      case Product::IR3:                return "IR3";
      case Product::IR4:                return "IR4";
      case Product::VIS:                return "VIS";
      case Product::Pseudo:             return "Pseudo";
   }
}

Met_Element
Model::Product::get_met_element () const
{
   switch (enumeration)
   {
      case Product::T:                  return denise::T;
      case Product::TD:                 return denise::TD;
      case Product::RH:                 return denise::RH;
      case Product::THETA:              return denise::THETA;
      case Product::THETA_MONOCHROME:   return denise::THETA;
      case Product::THETA_28C:          return denise::THETA;
      case Product::Q:                  return denise::Q;
      case Product::Q_MONOCHROME:       return denise::Q;
      case Product::Q_3GKG:             return denise::Q;
      case Product::THETA_E:            return denise::THETA_E;
      case Product::THETA_V:            return denise::THETA_V;
      case Product::RHO:                return denise::RHO;
      case Product::WIND:               return denise::WIND_DIRECTION;
      case Product::SPEED:              return denise::WIND_SPEED;
      case Product::SPEED_HIGHER:       return denise::WIND_SPEED;
      case Product::VORTICITY:          return denise::RELATIVE_VORTICITY;
      case Product::W:                  return denise::W;
      case Product::W_TRANSLUCENT:      return denise::W;
      case Product::Q_TENDENCY:         return denise::Q_TENDENCY;
      case Product::Q_L_TENDENCY:       return denise::Q_L_TENDENCY;
      case Product::Q_ADVECTION:        return denise::Q_ADVECTION;
      case Product::Q_H_ADVECTION:      return denise::Q_H_ADVECTION;
      case Product::Q_V_ADVECTION:      return denise::Q_V_ADVECTION;
      case Product::Q_S_ADVECTION:      return denise::Q_S_ADVECTION;
      case Product::Q_N_ADVECTION:      return denise::Q_N_ADVECTION;
      case Product::Q_SV_ADVECTION:     return denise::Q_SV_ADVECTION;
      case Product::Q_NV_ADVECTION:     return denise::Q_NV_ADVECTION;
      case Product::THETA_TENDENCY:     return denise::THETA_TENDENCY;
      case Product::THETA_L_TENDENCY:   return denise::THETA_L_TENDENCY;
      case Product::THETA_ADVECTION:    return denise::THETA_ADVECTION;
      case Product::THETA_H_ADVECTION:  return denise::THETA_H_ADVECTION;
      case Product::THETA_V_ADVECTION:  return denise::THETA_V_ADVECTION;
      case Product::THETA_S_ADVECTION:  return denise::THETA_S_ADVECTION;
      case Product::THETA_N_ADVECTION:  return denise::THETA_N_ADVECTION;
      case Product::THETA_SV_ADVECTION: return denise::THETA_SV_ADVECTION;
      case Product::THETA_NV_ADVECTION: return denise::THETA_NV_ADVECTION;
      case Product::FFDI:               return denise::FFDI;
      case Product::MSLP:               return denise::MSLP;
      case Product::PRECIP_RATE:        return denise::PRECIP_RATE;
   }
}

Dstring
Model::Product::get_unit () const
{
   switch (enumeration)
   {
      case Product::T:                  return "\u00b0C";
      case Product::TD:                 return "\u00b0C";
      case Product::RH:                 return "%%";
      case Product::THETA:              return "\u00b0C";
      case Product::THETA_MONOCHROME:   return "\u00b0C";
      case Product::THETA_28C:          return "\u00b0C";
      case Product::THETA_V:            return "\u00b0C";
      case Product::THETA_E:            return "\u00b0C";
      case Product::Q:                  return "g kg\u207b\u00b9";
      case Product::Q_MONOCHROME:       return "g kg\u207b\u00b9";
      case Product::Q_3GKG:             return "g kg\u207b\u00b9";
      case Product::RHO:                return "kgm\u207b\u00b9";
      case Product::W:                  return "ms\u207b\u00b9";
      case Product::W_TRANSLUCENT:      return "ms\u207b\u00b9";
      case Product::Q_TENDENCY:         return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_L_TENDENCY:       return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_ADVECTION:        return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_H_ADVECTION:      return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_V_ADVECTION:      return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_S_ADVECTION:      return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_N_ADVECTION:      return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_SV_ADVECTION:     return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::Q_NV_ADVECTION:     return "g kg\u207b\u00b9s\u207b\u00b9";
      case Product::THETA_TENDENCY:     return "K s\u207b\u00b9";
      case Product::THETA_L_TENDENCY:   return "K s\u207b\u00b9";
      case Product::THETA_ADVECTION:    return "K s\u207b\u00b9";
      case Product::THETA_H_ADVECTION:  return "K s\u207b\u00b9";
      case Product::THETA_V_ADVECTION:  return "K s\u207b\u00b9";
      case Product::THETA_S_ADVECTION:  return "K s\u207b\u00b9";
      case Product::THETA_N_ADVECTION:  return "K s\u207b\u00b9";
      case Product::THETA_SV_ADVECTION: return "K s\u207b\u00b9";
      case Product::THETA_NV_ADVECTION: return "K s\u207b\u00b9";
      case Product::SPEED_HIGHER:       return "m s\u207b\u00b9";
      case Product::SPEED:              return "knots";
      case Product::ALONG_SPEED:        return "m s\u207b\u00b9";
      case Product::NORMAL_SPEED:       return "m s\u207b\u00b9";
      case Product::VORTICITY:          return "10\u207b\u00b3 s\u207b\u00b9";
      case Product::FFDI:               return "";
      case Product::MSLP:               return "hPa";
      case Product::PRECIP_RATE:        return "mm hr\u207b\u00b9";
      case Product::WIND:               return "degree";
      case Product::TERRAIN:            return "m";
      case Product::TERRAIN_MONOCHROME: return "m";
      case Product::BRUNT_VAISALA:      return "s\u207b\u00b9";
      case Product::RICHARDSON:         return "";
      case Product::SCORER:             return "m\u207b\u00b2";
      case Product::SCORER_A:           return "m\u207b\u00b2";
      case Product::SCORER_B:           return "m\u207b\u00b2";
      default:                          return "";
   }
}

Tuple
Model::Product::get_tick_tuple () const
{

   switch (enumeration)
   {

      case Model::Product::T:
      {
         return Tuple ("10:15:20:25:30:35");
      }

      case Model::Product::TD:
      {
         return Tuple ("-10:-5:0:5:10:15:20");
      }

      case Model::Product::RH:
      {
         return Tuple ("0:20:40:60:80:100");
      }

      case Model::Product::THETA:
      case Model::Product::THETA_V:
      {
         //return Tuple ("0:10:20:30:40:50:60");
         return Tuple ("10:20:30:40:50:60");
      }

      case Model::Product::THETA_MONOCHROME:
      case Model::Product::THETA_28C:
      {
         //return Tuple ("0:10:20:30:40:50:60");
         return Tuple ("20:30:40");
      }

      case Model::Product::THETA_E:
      {
         return Tuple ("5:15:25:35:45:55:65");
      }

      case Model::Product::Q:
      case Model::Product::Q_MONOCHROME:
      case Model::Product::Q_3GKG:
      {
         return Tuple ("0:1:2:3:4:5:6:7:8:9:10");
      }

      case Model::Product::Q_TENDENCY:
      case Model::Product::Q_L_TENDENCY:
      case Model::Product::Q_ADVECTION:
      case Model::Product::Q_H_ADVECTION:
      case Model::Product::Q_V_ADVECTION:
      case Model::Product::Q_S_ADVECTION:
      case Model::Product::Q_N_ADVECTION:
      case Model::Product::Q_SV_ADVECTION:
      case Model::Product::Q_NV_ADVECTION:
      {
         return Tuple ("1e-7:3.2e-7:1e-6:3.2e-6:1e-5");
      }

      case Model::Product::THETA_TENDENCY:
      case Model::Product::THETA_L_TENDENCY:
      case Model::Product::THETA_ADVECTION:
      case Model::Product::THETA_H_ADVECTION:
      case Model::Product::THETA_V_ADVECTION:
      case Model::Product::THETA_S_ADVECTION:
      case Model::Product::THETA_N_ADVECTION:
      case Model::Product::THETA_SV_ADVECTION:
      case Model::Product::THETA_NV_ADVECTION:
      {
         return Tuple ("0:1e-3:2e-3:3e-3:4e-3:5e-3");
      }

      case Model::Product::RHO:
      {
         return Tuple ("0.0:1.0:2.0;3.0:4.0:5.0:6.0:7.0:8.0:9.0:10.0");
      }

      case Model::Product::W:
      {
         return Tuple ("-4:-3:-2:-1:0:1:2:3:4");
      }

      case Model::Product::W_TRANSLUCENT:
      {
         return Tuple ("-4:-3:-2:-1:0:1:2:3:4");
      }

      case Model::Product::SPEED_HIGHER:
      {
         //return Tuple ("0:5:15:25:35:45:55:65:75:85:95:105:115:125");
         return Tuple (11, 0.0, 50.0);
      }

      case Model::Product::SPEED:
      {
         return Tuple ("0:5:10:15:20:25:30:35:40:45:50:55:60:65");
      }

      case Model::Product::ALONG_SPEED:
      case Model::Product::NORMAL_SPEED:
      {
         //return Tuple (26, -125.0, 125.0);
         return Tuple (11, 0.0, 50.0);
      }

      case Model::Product::VORTICITY:
      {
         return Tuple ("-5:-4:-3:-2:-1:0:1:2:3:4:5");
      }

      case Model::Product::FFDI:
      {
         return Tuple ("1:12:25:50:75:100:150");
      }

      case Model::Product::MSLP:
      {
         return Tuple ("980:990:1000:1010:1020:1030");
      }

      case Model::Product::PRECIP_RATE:
      {
         return Tuple ("0.05:0.1:1:2:5:10:20:30:50:75:100:150");
      }

      case Model::Product::WIND:
      {
         return Tuple ("0:30:60:90:120:150:180:210:240:270:300:330:360");
      }

      case Model::Product::TERRAIN:
      {
         return Tuple ("0:200:400:600:800:1000:1200:1400:1600:1800:2000");
      }

      case Model::Product::TERRAIN_MONOCHROME:
      {
         return Tuple ("0:200:400:600:800:1000:1200");
      }

      case Model::Product::BRUNT_VAISALA:
      {
         return Tuple ("1e-3:1.8e-3:3.2e-3:5.6e-3:1e-2:1.8e-2:3.2e-2:5.6e-2:1e-1");
      }

      case Model::Product::RICHARDSON:
      {
         return Tuple ("0:0.25:0.5:1:2:3:4:5");
      }

      case Model::Product::SCORER:
      case Model::Product::SCORER_A:
      case Model::Product::SCORER_B:
      {
         return Tuple ("1e-8:3.2e-8:1e-7:1e-6:1e-5");
      }

      default:
      {
         return Tuple ();
      }

   }

}

ostream&
Model::Product::operator<< (ostream& out_file) const
{
   out_file << get_string ();
   return out_file;
}


Color
Model::Stage::get_wind_color (const Real direction,
                              const Real speed)
{
   const Real hue = direction / 360.;
   const Real brightness = std::min (speed / 15, 1.0);
   return Color::hsb (hue, 0.8, brightness);
}

Color
Model::Stage::get_color (const Model::Product& product,
                         const Real datum)
{

   switch (product.enumeration)
   {

      case Model::Product::T:
      {
         const Real hue = Domain_1D (35 + K, 10 + K).normalize (datum) * 0.833;
         const Real brightness = (Integer (floor (datum)) % 2) ? 0.83:0.77;
         return Color::hsb (hue, 0.8, brightness);
      }

      case Model::Product::TD:
      {
         const Real saturation = Domain_1D (-10+K, 20+K).normalize (datum);
         const Real brightness = (Integer (floor (datum)) % 2) ? 0.82:0.78;
         return Color::hsb (0.25, saturation, brightness);
      }

      case Model::Product::RH:
      {
         const Real hue = (datum < 0.5 ? 0.08 : 0.35);
         //const Real saturation = std::min ((fabs (datum - 0.5) * 2), 1.0);
         //return Color::hsb (hue, saturation, 1, 0.4);
         const Real saturation = fabs (datum - 0.5) * 0.8;
         return Color::hsb (hue, saturation, 1);
      }

      case Model::Product::THETA:
      case Model::Product::THETA_V:
      {
/* Sigmoid Curve at 30C
         const Real jump = 30 + K;
         const Real deviate = datum - jump;
         const Real x = atan (deviate / 1);
         const Real odd = (Integer (floor ((datum-K) / 1)) % 2);
         const Domain_1D d1d (-M_PI_2, M_PI_2);
         const Real brightness = d1d.normalize (x) * 0.5 + 0.45;
         return Color::hsb (0.0, 0.0, brightness);
*/
/* coloured strips 4K
         const Real fluc_mag = 0.2;
         const Real celsius = datum - K;
         //const Real odd = (Integer (floor (celsius / 1)) % 2);
         //const Real fluctuation = (odd ? fluc_mag : -fluc_mag);
         const Real hue = modulo ((60 - celsius) / 60, 1);
         const Real fluctuation = fluc_mag * sin (celsius * M_PI / 2);
         const Real brightness = 0.75;
         return Color::hsb (hue, 0.2, brightness + fluctuation);
*/
         const Real celsius = datum - K;
         const Real c = floor (celsius / 2) * 2;
         if (celsius < 35)
         {
            const Real saturation = Domain_1D (35, 20).normalize (c);
            const Real brightness = Domain_1D (-5, 35).normalize (c);
            return Color::hsb (0.65, saturation, brightness);
         }
         else
         {
            const Real saturation = Domain_1D (35, 50).normalize (c);
            const Real brightness = Domain_1D (75, 35).normalize (c);
            return Color::hsb (0.02, saturation, brightness);
         }
      }

      case Model::Product::THETA_MONOCHROME:
      {
         const Real celsius = datum - K;
         const Real c = floor (celsius / 2) * 2;
         //const Real saturation = Domain_1D (35, 20).normalize (c);
         const Real brightness = Domain_1D (5, 35).normalize (c);
         return Color::hsb (0.02, 0, brightness);
      }

      case Model::Product::THETA_28C:
      {
         const Real celsius = datum - K;
         const Real c = floor (celsius / 2) * 2;
         const Real saturation = exp (fabs (celsius - 29) / -0.5);
         const Real brightness = Domain_1D (5, 35).normalize (c);
         return Color::hsb (0.06, saturation, brightness);
      }

      case Model::Product::THETA_E:
      {
         const Real saturation = Domain_1D (5+K, 65+K).normalize (datum);
         const Real odd = (Integer (floor ((datum-K) / 4)) % 2);
         const Real brightness = (odd ? 0.82 : 0.78);
         return Color::hsb (0.35, saturation, brightness);
      }

      case Model::Product::Q:
      {
/* Blue 0.2 bands
         const Real d = datum * 1e3;
         const Real saturation = Domain_1D (0, 10).normalize (d);
         const Real odd = (Integer (floor (d / 0.2)) % 2);
         const Real brightness = (odd ? 0.82 : 0.78);
         return Color::hsb (0.58, saturation, brightness);
*/
         const Real d = floor (datum * 1e3);
         const Real saturation = Domain_1D (0, 10).normalize (d);
         const Real brightness = Domain_1D (20, 0).normalize (d);
         return Color::hsb (0.58, saturation, brightness);
      }

      case Model::Product::Q_MONOCHROME:
      {
         const Real d = floor (datum * 1e3);
         const Real brightness = Domain_1D (10, 0).normalize (d);
         return Color::hsb (0.58, 0, brightness);
      }

      case Model::Product::Q_3GKG:
      {
         const Real d = datum * 1e3;
         const Real saturation = exp (fabs (d - 3) / -0.3);
         const Real brightness = Domain_1D (10, 0).normalize (floor (d));
         return Color::hsb (0.58, saturation, brightness);
      }

      case Model::Product::RHO:
      {
         const Real jump = 1.10;
         const Real deviate = datum - jump;
         const Real x = atan (deviate / 0.05);
         const Domain_1D d1d (M_PI_2, -M_PI_2);
         const Real b = d1d.normalize (x) * 0.85 + 0.15;
         const Real s = exp (-fabs (deviate) / 0.01) * 0.8 + 0.2;
         return Color::hsb (0.800, s, b);
      }

      case Model::Product::W:
      {
/*
         const Real hue = (datum < 0 ? 0.667 : 0.000);
         const Real absolute = fabs (datum);
         const Real quantized = floor (absolute / 0.1) * 0.1;
         //const Real quantized = absolute;
         const Real saturation = Domain_1D (0, 3.5).normalize (fabs (datum)) * 0.7;
         //const Real saturation = Domain_1D (0, 3.5).normalize (quantized) * 0.7;
         //const Real saturation = Domain_1D (0, 0.5).normalize (quantized) * 0.7;
         const Real fluc_mag = 0.2;
         const Real fluctuation = fluc_mag * cos (datum * M_PI / 0.5);
         const Real brightness = 0.75 + fluctuation;
         return Color::hsb (hue, saturation, brightness);
*/
         const Real hue = (datum < 0 ? 0.667 : 0.000);
         const Real absolute = fabs (datum);
         const Real saturation = Domain_1D (0, 3.5).normalize (floor (fabs (datum) + 0.5) - 0.5) * 0.8;
         const Real brightness = 1.0;
         return Color::hsb (hue, saturation, brightness);
      }

      case Model::Product::W_TRANSLUCENT:
      {
         const Real hue = (datum < 0 ? 0.667 : 0.000);
         const Real absolute = fabs (datum);
         const Real quantized = floor (absolute / 0.1) * 0.1;
         const Real alpha = Domain_1D (0, 3.5).normalize (quantized) * 0.7;
         return Color::hsb (hue, 1.0, 1.0, alpha);
      }

      case Model::Product::Q_TENDENCY:
      case Model::Product::Q_L_TENDENCY:
      case Model::Product::Q_ADVECTION:
      case Model::Product::Q_H_ADVECTION:
      case Model::Product::Q_V_ADVECTION:
      case Model::Product::Q_S_ADVECTION:
      case Model::Product::Q_N_ADVECTION:
      case Model::Product::Q_SV_ADVECTION:
      case Model::Product::Q_NV_ADVECTION:
      {
         if (!gsl_finite (datum)) { return Color::gray (0.5); }
         const Real min = log (1e-7);
         const Real max = log (1e-5);
         const Real hue = (datum < 0 ? 0.08 : 0.35);
         const Real d = log (fabs (datum));
         const Real saturation = Domain_1D (min, max).normalize (d);
         return Color::hsb (hue, saturation, 1);
      }

      case Model::Product::THETA_TENDENCY:
      case Model::Product::THETA_L_TENDENCY:
      case Model::Product::THETA_ADVECTION:
      case Model::Product::THETA_H_ADVECTION:
      case Model::Product::THETA_V_ADVECTION:
      case Model::Product::THETA_S_ADVECTION:
      case Model::Product::THETA_N_ADVECTION:
      case Model::Product::THETA_SV_ADVECTION:
      case Model::Product::THETA_NV_ADVECTION:
      {
         if (!gsl_finite (datum)) { return Color::gray (0.5); }
         const Real min = 5e-3;
         const Real max = 0.1e-3;
         const Real hue = (datum < 0 ? 0.67 : 0.00);
         //const Real d = log (fabs (datum));
         const Real d = fabs (datum);
         const Real saturation = Domain_1D (max, min).normalize (d);
         return Color::hsb (hue, saturation, 1);
      }

      case Model::Product::SPEED_HIGHER:
      {
/*
         const Real knots = datum * 3.6/1.852;
         const Real m0 = modulo (knots, 5.0) / 5.0 * 0.15 + 0.85;
         const Real m = modulo (knots - 5, 10.0) / 10.0 * 0.15 + 0.85;
              if (knots <   5) { return Color::hsb (0.000, 0.0*m0, 1.0*m0); }
         else if (knots <  15) { return Color::hsb (0.167, 0.30*m, 1.00*m); }
         else if (knots <  25) { return Color::hsb (0.150, 0.35*m, 0.95*m); }
         else if (knots <  35) { return Color::hsb (0.133, 0.40*m, 0.90*m); }
         else if (knots <  45) { return Color::hsb (0.333, 0.40*m, 0.95*m); }
         else if (knots <  55) { return Color::hsb (0.333, 0.40*m, 0.75*m); }
         else if (knots <  65) { return Color::hsb (0.333, 0.40*m, 0.65*m); }
         else if (knots <  75) { return Color::hsb (0.600, 0.40*m, 1.00*m); }
         else if (knots <  85) { return Color::hsb (0.633, 0.40*m, 0.90*m); }
         else if (knots <  95) { return Color::hsb (0.667, 0.40*m, 0.80*m); }
         else if (knots < 105) { return Color::hsb (0.033, 0.40*m, 0.65*m); }
         else if (knots < 115) { return Color::hsb (0.016, 0.45*m, 0.80*m); }
         else if (knots < 125) { return Color::hsb (0.000, 0.50*m, 0.95*m); }
         else                  { return Color (1.000, 1.000, 1.000); }
*/
         const Real m = modulo (datum, 5.0) / 5.0 * 0.15 + 0.85;
              if (datum <  5) { return Color::hsb (0.000, 0.00*m, 1.00*m); }
         else if (datum < 10) { return Color::hsb (0.167, 0.30*m, 1.00*m); }
         else if (datum < 15) { return Color::hsb (0.150, 0.35*m, 0.95*m); }
         else if (datum < 20) { return Color::hsb (0.133, 0.40*m, 0.90*m); }
         else if (datum < 25) { return Color::hsb (0.333, 0.40*m, 0.95*m); }
         else if (datum < 30) { return Color::hsb (0.333, 0.40*m, 0.75*m); }
         else if (datum < 35) { return Color::hsb (0.333, 0.40*m, 0.65*m); }
         else if (datum < 40) { return Color::hsb (0.600, 0.40*m, 1.00*m); }
         else if (datum < 45) { return Color::hsb (0.633, 0.40*m, 0.90*m); }
         else if (datum < 50) { return Color::hsb (0.667, 0.40*m, 0.80*m); }
         else if (datum < 55) { return Color::hsb (0.033, 0.40*m, 0.65*m); }
         else if (datum < 60) { return Color::hsb (0.016, 0.45*m, 0.80*m); }
         else if (datum < 65) { return Color::hsb (0.000, 0.50*m, 0.95*m); }
         else                 { return Color (1.000, 1.000, 1.000); }
      }

      case Model::Product::SPEED:
      {
         const Real knots = datum * 3.6/1.852;
         const Real m = modulo (knots, 5.0) / 5.0 * 0.15 + 0.85;
              if (knots <  5) { return Color::hsb (0.000, 0.00*m, 1.00*m); }
         else if (knots < 10) { return Color::hsb (0.167, 0.30*m, 1.00*m); }
         else if (knots < 15) { return Color::hsb (0.150, 0.35*m, 0.95*m); }
         else if (knots < 20) { return Color::hsb (0.133, 0.40*m, 0.90*m); }
         else if (knots < 25) { return Color::hsb (0.333, 0.40*m, 0.95*m); }
         else if (knots < 30) { return Color::hsb (0.333, 0.40*m, 0.75*m); }
         else if (knots < 35) { return Color::hsb (0.333, 0.40*m, 0.65*m); }
         else if (knots < 40) { return Color::hsb (0.600, 0.40*m, 1.00*m); }
         else if (knots < 45) { return Color::hsb (0.633, 0.40*m, 0.90*m); }
         else if (knots < 50) { return Color::hsb (0.667, 0.40*m, 0.80*m); }
         else if (knots < 55) { return Color::hsb (0.033, 0.40*m, 0.65*m); }
         else if (knots < 60) { return Color::hsb (0.016, 0.45*m, 0.80*m); }
         else if (knots < 65) { return Color::hsb (0.000, 0.50*m, 0.95*m); }
         else                 { return Color (1.000, 1.000, 1.000); }
      }

      case Model::Product::ALONG_SPEED:
      case Model::Product::NORMAL_SPEED:
      {
/*
         const bool negative = (datum < 0);
         const Real knots = fabs (datum * 3.6/1.852);
         const Real m0 = modulo (knots, 5.0) / 5.0 * 0.15 + 0.85;
         const Real m = modulo (knots - 5, 10.0) / 10.0 * 0.15 + 0.85;
         if (!negative)
         {
                 if (knots <  5) { return Color::hsb (0.000, 0.0*m0, 1.0*m0); }
            else if (knots < 15) { return Color::hsb (0.167, 0.30*m, 1.00*m); }
            else if (knots < 25) { return Color::hsb (0.150, 0.35*m, 0.95*m); }
            else if (knots < 35) { return Color::hsb (0.133, 0.40*m, 0.90*m); }
            else if (knots < 45) { return Color::hsb (0.333, 0.40*m, 0.95*m); }
            else if (knots < 55) { return Color::hsb (0.333, 0.40*m, 0.75*m); }
            else if (knots < 65) { return Color::hsb (0.333, 0.40*m, 0.65*m); }
            else if (knots < 75) { return Color::hsb (0.600, 0.40*m, 1.00*m); }
            else if (knots < 85) { return Color::hsb (0.633, 0.40*m, 0.90*m); }
            else if (knots < 95) { return Color::hsb (0.667, 0.40*m, 0.80*m); }
            else if (knots < 105){ return Color::hsb (0.033, 0.40*m, 0.65*m); }
            else if (knots < 115){ return Color::hsb (0.016, 0.45*m, 0.80*m); }
            else if (knots < 125){ return Color::hsb (0.000, 0.50*m, 0.95*m); }
            else                 { return Color (1.000, 1.000, 1.000); }
         }
         else
         {
                 if (knots <  5) { return Color::hsb (0.000, 0.0*m0, 0.8*m0); }
            else if (knots < 15) { return Color::hsb (0.167, 0.20*m, 0.80*m); }
            else if (knots < 25) { return Color::hsb (0.150, 0.25*m, 0.75*m); }
            else if (knots < 35) { return Color::hsb (0.133, 0.30*m, 0.70*m); }
            else if (knots < 45) { return Color::hsb (0.333, 0.30*m, 0.75*m); }
            else if (knots < 55) { return Color::hsb (0.333, 0.30*m, 0.55*m); }
            else if (knots < 65) { return Color::hsb (0.333, 0.30*m, 0.45*m); }
            else if (knots < 75) { return Color::hsb (0.600, 0.30*m, 0.80*m); }
            else if (knots < 85) { return Color::hsb (0.633, 0.30*m, 0.70*m); }
            else if (knots < 95) { return Color::hsb (0.667, 0.30*m, 0.60*m); }
            else if (knots < 105){ return Color::hsb (0.033, 0.30*m, 0.45*m); }
            else if (knots < 115){ return Color::hsb (0.016, 0.35*m, 0.60*m); }
            else if (knots < 125){ return Color::hsb (0.000, 0.40*m, 0.75*m); }
            else                 { return Color (1.000, 1.000, 1.000); }
         }
*/
         const Real m = modulo (fabs (datum), 5.0) / 5.0 * 0.15 + 0.85;
              if (datum <  5) { return Color::hsb (0.000, 0.00*m, 1.00*m); }
         else if (datum < 10) { return Color::hsb (0.167, 0.30*m, 1.00*m); }
         else if (datum < 15) { return Color::hsb (0.150, 0.35*m, 0.95*m); }
         else if (datum < 20) { return Color::hsb (0.133, 0.40*m, 0.90*m); }
         else if (datum < 25) { return Color::hsb (0.333, 0.40*m, 0.95*m); }
         else if (datum < 30) { return Color::hsb (0.333, 0.40*m, 0.75*m); }
         else if (datum < 35) { return Color::hsb (0.333, 0.40*m, 0.65*m); }
         else if (datum < 40) { return Color::hsb (0.600, 0.40*m, 1.00*m); }
         else if (datum < 45) { return Color::hsb (0.633, 0.40*m, 0.90*m); }
         else if (datum < 50) { return Color::hsb (0.667, 0.40*m, 0.80*m); }
         else if (datum < 55) { return Color::hsb (0.033, 0.40*m, 0.65*m); }
         else if (datum < 60) { return Color::hsb (0.016, 0.45*m, 0.80*m); }
         else if (datum < 65) { return Color::hsb (0.000, 0.50*m, 0.95*m); }
         else                 { return Color (1.000, 1.000, 1.000); }
      }

      case Model::Product::VORTICITY:
      {
         const Real hue = (datum < 0 ? 0.667 : 0.000);
         const Real modified_datum = (log10 (fabs (datum)) + 4) / 3;
         const Real saturation = std::max (std::min (modified_datum, 1.0), 0.0);
         //const Real saturation = Domain_1D (0, 1).normalize (modified_datum);
         return Color::hsb (hue, saturation, 1, 1);
      }

      case Model::Product::FFDI:
      {

         const Real alpha = 1.0;

         if (datum < 12)
         {
            const Real delta = datum;
            const Real r = 0.400 + delta * 0.03137;
            const Real g = 0.400 + delta * 0.04549;
            const Real b = 0.400 + delta * 0.03012;
            return Color (r, g, b, alpha);
         }
         else
         if (datum < 25)
         {
            const Real delta = datum - 12;
            const Real r = 0.278 + delta * 0.02133;
            const Real g = 0.400 + delta * 0.03075;
            const Real b = 0.500 + delta * 0.03859;
            return Color (r, g, b, alpha);
         }
         else
         if (datum < 50)
         {
            const Real delta = datum - 25;
            const Real r = 0.600 - delta * 0.02137;
            const Real g = 0.600 - delta * 0.02065;
            const Real b = 0.600 - delta * 0.02012;
            return Color (r, g, b, alpha);
         }
         else
         if (datum < 75)
         {
            const Real delta = datum - 50;
            const Real r = 0.549 + delta * 0.01757;
            const Real g = 0.549 + delta * 0.01757;
            const Real b = 0.329 + delta * 0.02133;
            return Color (r, g, b, alpha);
         }
         else
         if (datum < 100)
         {
            const Real delta = datum - 75;
            const Real r = 0.097 + delta * 0.01553;
            const Real g = 0.357 + delta * 0.00925;
            const Real b = 0.000 + delta * 0.00000;
            return Color (r, g, b, alpha);
         }
         else
         if (datum < 150)
         {
            const Real delta = datum - 100;
            const Real r = 0.647 + delta * 0.00706;
            const Real g = 0.000 + delta * 0.00000;
            const Real b = 0.000 + delta * 0.00000;
            return Color (r, g, b, alpha);
         }
         else
         if (datum < 200)
         {
            const Real delta = datum - 150;
            const Real r = 0.698 + delta * 0.00604;
            const Real g = 0.000 + delta * 0.00000;
            const Real b = 0.463 + delta * 0.00040;
            return Color (r, g, b, alpha);
         }
         else
         {
            return Color::transparent ();
         }

      }

      case Model::Product::MSLP:
      {
         //const Real hue = Domain_1D (990e2, 1025e2).normalize (datum) * 0.833;
         //const Real brightness = (Integer (floor (datum/200)) % 2) ? 0.82:0.78;
         const Real hPa = datum * 1e-2;
         const Real fluc_mag = 0.2;
         const Real fluctuation = fluc_mag * sin (hPa * M_PI / 1);
         const Real brightness = 0.75 + fluctuation;
         return Color::hsb (0, 0, brightness);
      }

      case Model::Product::PRECIP_RATE:
      {
         const Real mmhr = datum * 3600;
              if (mmhr < 0.1) { return Color::hsb (0.000, 0.00, 1.0); }
         else if (mmhr <   1) { return Color::hsb (0.167, 0.60, 1.0); }
         else if (mmhr <   2) { return Color::hsb (0.333, 0.60, 1.0); }
         else if (mmhr <   5) { return Color::hsb (0.333, 0.60, 0.8); }
         else if (mmhr <  10) { return Color::hsb (0.333, 0.60, 0.6); }
         else if (mmhr <  20) { return Color::hsb (0.333, 0.60, 0.4); }
         else if (mmhr <  30) { return Color::hsb (0.600, 0.60, 1.0); }
         else if (mmhr <  50) { return Color::hsb (0.633, 0.60, 0.8); }
         else if (mmhr <  75) { return Color::hsb (0.666, 0.60, 0.6); }
         else if (mmhr < 100) { return Color::hsb (0.083, 0.60, 1.0); }
         else if (mmhr < 150) { return Color::hsb (0.042, 0.60, 1.0); }
         else                 { return Color::hsb (0.000, 0.60, 1.0); }
      }

      case Model::Product::TERRAIN:
      {
         //const Real h = std::min (std::max (datum / 2000.0, 0.0), 1.0);
         //const Real hue = 0.45 - h * 0.4;
         //const Real brightness = h * 0.7 + 0.28;
         //return Color::hsb (hue, 0.34, brightness);
         const Real h = std::min (std::max (datum / 1200.0, 0.0), 1.0);
         const Real hue = 0.45 - h * 0.4;
         const Real brightness = h * 0.3 + 0.2;
         return Color::hsb (hue, 0.54, brightness);
      }

      case Model::Product::TERRAIN_MONOCHROME:
      {
         const Real z = floor (datum / 200.0) * 200.0;
         const Real brightness = (2000 - z) / 2000;
         return Color::hsb (0, 0, brightness);
      }

      case Model::Product::TERRAIN_WATER:
      {
         //const Real h = std::min (std::max (datum / 2000.0, 0.0), 1.0);
         //const Real hue = 0.67;
         //const Real brightness = h * 0.7 + 0.28;
         //return Color::hsb (hue, 0.54, brightness);
         return Color::sea ();
      }

      case Model::Product::BRUNT_VAISALA:
      {
         if (!gsl_finite (datum)) { return Color::white (); }
         const Real e = log10 (datum) - (-3.0);
         const Real x = std::max (std::min (e / 2.0, 1.0), 0.0);
         const Real hue = modulo (0.0 + (floor (e / 0.25)) * 0.15, 1);
         return Color::hsb (hue, x, 1.0 - x * 0.5);
      }

      case Model::Product::RICHARDSON:
      {
         if (datum <= 0.01) { return Color::transparent (); }
         const Real jump = 0.25;
         const Real deviate = datum - jump;
         const Real x = tanh (deviate / 0.05) * 0.2;
         const Real brightness = 0.75 + x;
         const Real h = std::min (std::max (datum / 5.0, 0.0), 1.0);
         const Real hue = 0.7 - h * 0.5;
         return Color::hsb (hue, 0.38, brightness);
      }

      case Model::Product::SCORER:
      case Model::Product::SCORER_A:
      case Model::Product::SCORER_B:
      {
/*
         if (!gsl_finite (datum)) { return Color::gray (0.5); }
         if (datum > 1e-6) { return Color::transparent (); }
         const bool negative = datum < 0;
         //const Real e = (log10 (fabs (datum)) - (-8.0));
         const Real e = log10 (fabs (datum));
         const Integer fe = min (3, floor ((e + 8) * 4) / 4);
         const Real min = log (1e-8);
         const Real max = log (1e-5);
         const Real d = log (fabs (datum));
         const bool strong_positive_e = (e > -6) && !negative;
         const bool small_e = (e < -7);
         const Real hue = (strong_positive_e ? 0.8 : 
                           negative ? 0.46 - fe * 0.15 : 0.65);
         //const Real saturation = Domain_1D (min, max).normalize (d);
         const Real saturation = (small_e ? 0.3 : 0.85);
         const Real brightness = 0.8;
         const Real fluc_mag = 0.0;
         //const Real fluctuation = -fluc_mag * cos (e * M_PI / 0.5);
         //const Real fluctuation = fluc_mag * (e - floor (e)) * 0.98;
         //const Real fluctuation = 0;
         //const Real brightness = 0.98 + fluctuation;
         return Color::hsb (hue, saturation, brightness);
*/
         const Real b_h = 0.8;
         const Real b_l = 0.25;
         if (!gsl_finite (datum)) { return Color::transparent (); }
         if (datum > 1e-6) { return Color::transparent (); }
         if (datum < 3.2e-8) { return Color::hsb (0.05, 0.6, b_l); }
         const Real e = log10 (fabs (datum));
         const Real brightness = (b_h - b_l) / (-6 + 8) * (e + 8) + b_l;
         return Color::hsb (0.3, 0.4, brightness);
      }

      default:
      {
         return Color::transparent ();
      }

   }

}

Color
Model::Stage::get_color (const Model::Product& product,
                         const Real datum,
                         const Dstring& unit)
{

   if (unit == "\u00b0C")
   {
      return get_color (product, datum + K);
   }
   else
   if (unit == "ms\u207b\u00b9" ||
       unit == "m s\u207b\u00b9")
   {
      return get_color (product, datum);
   }
   else
   if (unit == "knots")
   {
      return get_color (product, datum * 1.852/3.6);
   }
   else
   if (unit == "gkg\u207b\u00b9" ||
       unit == "g kg\u207b\u00b9")
   {
      return get_color (product, datum * 1e-3);
   }
   else
   if (unit == "%%")
   {
      return get_color (product, datum * 1e-2);
   }
   else
   if (unit == "hPa")
   {
      return get_color (product, datum * 1e2);
   }
   else
   if (unit == "degree")
   {
      return get_wind_color (datum, 15);
   }
   else
   if (unit == "mm hr\u207b\u00b9")
   {
      return get_color (product, datum / 3600.);
   }
   else
   if (unit == "10\u207b\u00b3 s\u207b\u00b9")
   {
      return get_color (product, datum * 1e-3);
   }

   return get_color (product, datum);

}
Model::Stage::Map::Map (const Model& model,
                        const Rc_File& rc_file)
{

   Tokens stage_tokens;

   for (auto iterator = rc_file.begin ();
        iterator != rc_file.end (); iterator++)
   {

      const Tokens tokens (*(iterator), " \f\n\t");
      if (tokens.size () != 2 || tokens[0] != "model") { continue; }
  
      const Dstring& argument = tokens[1];
      const Tokens argument_tokens (argument, ":");
      const Integer n = argument_tokens.size ();

      if (argument_tokens[0] == "stage")
      {
         for (Integer i = 1; i < argument_tokens.size (); i++)
         {
            const Dstring& model_identifier = argument_tokens[i];
            stage_tokens.push_back (model_identifier);
         }
      }

   }

   for (auto iterator = stage_tokens.begin ();
        iterator != stage_tokens.end (); iterator++)
   {
      const Dstring s (*iterator);
      insert (make_pair (s, new Model::Acncrjbf (model, s, rc_file)));
   }

}

Model::Stage::Map::~Map ()
{
   for (auto iterator = begin (); iterator != end (); iterator++)
   {
      Model::Stage* stage_ptr = iterator->second;
      delete stage_ptr;
   }
}

Model::Acncrjbf::Vertical_Coefficients::Vertical_Coefficients ()
{
}

Model::Acncrjbf::Vertical_Coefficients::Vertical_Coefficients (const Dstring& file_path)
{
   init (file_path);
}

void
Model::Acncrjbf::Vertical_Coefficients::init (const Dstring& file_path)
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
Model::Acncrjbf::Vertical_Coefficients::get_A (const bool is_theta) const
{
   return (is_theta ? A_theta : A_rho);
}

const Tuple&
Model::Acncrjbf::Vertical_Coefficients::get_B (const bool is_theta) const
{
   return (is_theta ? B_theta : B_rho);
}

const Real
Model::Acncrjbf::Vertical_Coefficients::get_z (const Integer k,
                                            const Real topography,
                                            const bool is_theta) const
{
   const Tuple& A = get_A (is_theta);
   const Tuple& B = get_B (is_theta);
   return A[k] + B[k] * topography;
}

const size_t
Model::Acncrjbf::Vertical_Coefficients::get_k (const Real z,
                                            const Real topography,
                                            const bool is_theta,
                                            Integer start_k,
                                            Integer end_k) const
{

   if (start_k == -1) { start_k = 0; }
   if (end_k == -1) { end_k = 69; }

   if (end_k - start_k <= 1)
   {
      const Real start_diff = fabs (z - get_z (start_k, topography, is_theta));
      const Real end_diff = fabs (z - get_z (end_k, topography, is_theta));
      const bool nearer_to_start = start_diff < end_diff;
      return (nearer_to_start ? start_k : end_k);
   }
   else
   {
      const Real sum = start_k + end_k;
      const bool is_even = denise::is_even (sum);
      const Integer mk = Integer (is_even ? round (sum/2) : floor (sum/2));
      const Real middle_z = get_z (mk, topography, is_theta);
      const bool larger = z > middle_z;
      return (larger ?
         get_k (z, topography, is_theta, mk, end_k) :
         get_k (z, topography, is_theta, start_k, mk));

   }

}

void
Model::Acncrjbf::fill_valid_time (set<Dtime>& valid_time_set,
                                  vector<Dtime>& valid_time_vector,
                                  const Dstring& nc_varname)
{

   if (nc_file_ptr_map.size () == 0) { return; }

   try
   {
      const Nc_File& nc_file = *(nc_file_ptr_map.at (nc_varname));
      const Integer nc_id = nc_file.get_nc_id ();
      char attribute_char[128];

      int ret, dim_id, varid;

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

      size_t n;
      nc_type var_type;
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
         valid_time_vector.push_back (dtime);
      }

      delete[] array;

   }
   catch (...) { }

}

void
Model::Acncrjbf::fill_valid_time ()
{
   fill_valid_time (valid_surface_time_set, valid_surface_time_vector, "temp");
   fill_valid_time (valid_uppers_time_set, valid_uppers_time_vector, "ml_theta");
}

void
Model::Acncrjbf::acquire_ij (size_t& i,
                             size_t& j,
                             const Lat_Long& lat_long) const
{
   const Real& latitude = lat_long.latitude;
   const Real& longitude = lat_long.longitude;
   i = Grid_nD::get_nearest_node (tuple_latitude, GSL_NAN, latitude);
   j = Grid_nD::get_nearest_node (tuple_longitude, GSL_NAN, longitude);
}

void
Model::Acncrjbf::acquire_k (size_t& k,
                            const Met_Element& met_element,
                            const size_t i,
                            const size_t j,
                            const Level& level) const
{

   const Model::Acncrjbf::Vertical_Coefficients& vc = vertical_coefficients;

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
         const bool is_theta = Model::Acncrjbf::is_theta (met_element);
         if (z > vc.get_A (is_theta).back ()) { k = -1; return; }

         const bool surface = (z < 0);
         k = surface ? -1 : vc.get_k (z, topography, is_theta);
         break;

      }

      case Level::MAGL:
      {

         const Real topography = get_topography (i, j);
         const Real z = level.value + topography;
         if (z < topography) { k = -1; return; }

         const bool is_w = (met_element == W);
         const bool is_theta = Model::Acncrjbf::is_theta (met_element);
         if (z > vc.get_A (is_theta).back ()) { k = -1; return; }

         const bool surface = (z < 0);
         k = surface ? -1 : vc.get_k (z, topography, is_theta);
         break;

      }

      default:
      {
         k = -1;
         break;
      };

   }

}

Real
Model::Acncrjbf::evaluate_raw (const Dstring& varname,
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

Real
Model::Acncrjbf::evaluate_raw (const Dstring& varname,
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
Model::Acncrjbf::fill_sounding (Sounding& sounding,
                             const Lat_Long& lat_long,
                             const Dtime& dtime,
                             const Real ceiling) const
{


   size_t i, j;
   acquire_ij (i, j, lat_long);

   const Model::Acncrjbf::Vertical_Coefficients& vc = vertical_coefficients;
   const Integer surface_l = get_surface_l (dtime);
   const Integer uppers_l = get_uppers_l (dtime);
   const Real topography = get_topography (i, j);

   // surface points commented out because surface_p cannot be
   // calculated accurately

   //const Real t = evaluate (T, i, j, surface_l);
   //const Real t_d = evaluate (TD, i, j, surface_l);
   //const Real u = evaluate (U, i, j, surface_l);
   //const Real v = evaluate (V, i, j, surface_l);

   //const Real mslp = evaluate (MSLP, i, j, surface_l);
   //const Real surface_p = mslp - 11.76 * topography;

   //sounding.get_t_line ().add (surface_p, t - K);
   //sounding.get_t_d_line ().add (surface_p, t_d - K);
   //sounding.get_wind_profile ().add (surface_p, Wind (u, v));
   //sounding.get_height_profile ().add (surface_p, 0);

   for (Integer k = 0; k < 70; k++)
   {

      const Real z_rho = vc.get_z (k, topography, false);
      const Real z_theta = vc.get_z (k, topography, true);

      if (z_rho > ceiling || z_theta > ceiling) { continue; }

      const Real t = evaluate (T, i, j, k, uppers_l);
      const Real t_d = evaluate (TD, i, j, k, uppers_l);
      const Real u = evaluate (U, i, j, k, uppers_l);
      const Real v = evaluate (V, i, j, k, uppers_l);
      const Real p_rho = evaluate (P_RHO, i, j, k, uppers_l);
      const Real p_theta = evaluate (P_THETA, i, j, k, uppers_l);

      sounding.get_t_line ().add (p_theta, t - K);
      sounding.get_t_d_line ().add (p_theta, t_d - K); 
      sounding.get_wind_profile ().add (p_rho, Wind (u, v));
      sounding.get_height_profile ().add (p_rho, z_rho);
      sounding.get_height_profile ().add (p_theta, z_theta);

   }

}

Model::Acncrjbf::Acncrjbf (const Model& model,
                           const Dstring& stage_str,
                           const Rc_File& rc_file)
   : model (model),
     stage_str (stage_str)
{

   for (auto iterator = rc_file.begin ();
        iterator != rc_file.end (); iterator++)
   {

      const Tokens tokens (*(iterator), " \f\n\t");
      if (tokens.size () != 2 || tokens[0] != "model") { continue; }

      const Dstring& argument = tokens[1];
      const Tokens argument_tokens (argument, ":");
      const Dstring& model_identifier = argument_tokens[0].get_trimmed ();
      if (stage_str != model_identifier) { continue; }

      this->basetime = Dtime (argument_tokens[1].get_trimmed ());

      if (argument_tokens[2] == "AB")
      {
         vertical_coefficients.init (argument_tokens[3].get_trimmed ());
         continue;
      }

//          argument_tokens.size () == 3)
      if (argument_tokens.size () == 4)
      {

         if (stage_str != argument_tokens[0].get_trimmed ()) { continue; }
         const Dstring& varname = argument_tokens[2].get_trimmed ();
         const Dstring& file_path = argument_tokens[3].get_trimmed ();

         ingest (varname, file_path);
         continue;

      }
 
   }

   fill_valid_time ();

}

Model::Acncrjbf::~Acncrjbf ()
{

   for (auto iterator = nc_file_ptr_map.begin ();
        iterator != nc_file_ptr_map.end (); iterator++)
   {
      Nc_File* nc_file_ptr = iterator->second;
      delete nc_file_ptr;
   }

}

bool
Model::Acncrjbf::is_theta (const Met_Element& met_element)
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

Dstring
Model::Acncrjbf::get_nc_varname (const Dstring& varname)
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

void
Model::Acncrjbf::ingest (const Dstring& varname,
                      const Dstring& file_path)
{

   nc_file_ptr_map.insert (make_pair (varname, new Nc_File (file_path)));
   const Nc_File& nc_file = *(nc_file_ptr_map.at (varname));
   const Integer nc_id = nc_file.get_nc_id ();

   this->tuple_latitude = nc_file.get_coordinate_tuple ("latitude");
   this->tuple_longitude = nc_file.get_coordinate_tuple ("longitude");

   const Dstring& nc_varname = Model::Acncrjbf::get_nc_varname (varname);

   int varid;
   int ret = nc_inq_varid (nc_id, nc_varname.get_string ().c_str (), &varid);
   if (ret != NC_NOERR) { throw Exception ("nc_inq_varid " + nc_varname); }
   varid_map[varname] = varid;

}

const Dtime&
Model::Acncrjbf::get_basetime () const
{
   return basetime;
}

Domain_2D
Model::Acncrjbf::get_domain_2d () const
{
   return Domain_2D (tuple_latitude.front (), tuple_latitude.back (),
                     tuple_longitude.front (), tuple_longitude.back ());
}

bool
Model::Acncrjbf::out_of_bounds (const Lat_Long& lat_long) const
{
   return lat_long.latitude < tuple_latitude.front () ||
          lat_long.latitude > tuple_latitude.back () ||
          lat_long.longitude < tuple_longitude.front () ||
          lat_long.longitude > tuple_longitude.back ();
}

pair<Real, Real>
Model::Acncrjbf::get_grid_size (const size_t& i,
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
Model::Acncrjbf::get_grid_size (const Lat_Long& lat_long) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return get_grid_size (i, j);
}

const set<Dtime>&
Model::Acncrjbf::get_valid_surface_time_set () const
{
   return valid_surface_time_set;
}

const set<Dtime>&
Model::Acncrjbf::get_valid_uppers_time_set () const
{
   return valid_uppers_time_set;
}

const set<Dtime>&
Model::Acncrjbf::get_valid_time_set (const Level& level) const
{
   if (level.type == Level::SURFACE)
   {
      return get_valid_surface_time_set ();
   }
   else
   {
      return get_valid_uppers_time_set ();
   }
}

const set<Dtime>&
Model::Acncrjbf::get_valid_time_set (const Product& product,
                                     const Level& level) const
{

   switch (product.enumeration)
   {

      case Product::TERRAIN:
      case Product::TERRAIN_MONOCHROME:
      case Product::TERRAIN_WATER:
      {
         return get_valid_surface_time_set ();
      }

      case Product::T:
      case Product::P_THETA:
      case Product::P_RHO:
      case Product::Q:
      case Product::Q_MONOCHROME:
      case Product::Q_3GKG:
      case Product::Q_TENDENCY:
      case Product::Q_L_TENDENCY:
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
      case Product::RICHARDSON:
      case Product::SCORER:
      case Product::SCORER_A:
      case Product::SCORER_B:
      case Product::W:
      case Product::W_TRANSLUCENT:
      case Product::VORTICITY:
      case Product::THETA:
      case Product::THETA_MONOCHROME:
      case Product::THETA_28C:
      case Product::THETA_E:
      case Product::THETA_V:
      case Product::THETA_TENDENCY:
      case Product::THETA_L_TENDENCY:
      case Product::THETA_ADVECTION:
      case Product::THETA_H_ADVECTION:
      case Product::THETA_V_ADVECTION:
      case Product::THETA_S_ADVECTION:
      case Product::THETA_N_ADVECTION:
      case Product::THETA_SV_ADVECTION:
      case Product::THETA_NV_ADVECTION:
      {
         if (level.type == Level::SURFACE)
         {
            return get_valid_surface_time_set ();
         }
         else
         if ((level.type == Level::HEIGHT) ||
             (level.type == Level::MAGL))
         {
            return get_valid_uppers_time_set ();
         }
      }

      case Product::FFDI:
      case Product::MSLP:
      case Product::PRECIP_RATE:
      {
         return get_valid_surface_time_set ();
      }

   }

}

vector<Dtime>
Model::Acncrjbf::get_valid_time_vector (const Level& level,
                                        const Dtime::Set& time_set) const
{

   vector<Dtime> valid_time_vector;
   auto valid_time_set = get_valid_time_set (level);

   for (auto l = valid_time_set.begin ();
        l != valid_time_set.end (); l++)
   {
      const Dtime& dtime = *(l);
      if (time_set.match (dtime)) { valid_time_vector.push_back (dtime); }
   }

   return valid_time_vector;

}

vector<Dtime>
Model::Acncrjbf::get_valid_time_vector (const Product& product,
                                        const Level& level,
                                        const Dtime::Set& time_set) const
{

   vector<Dtime> valid_time_vector;
   auto valid_time_set = get_valid_time_set (product, level);

   for (auto l = valid_time_set.begin ();
        l != valid_time_set.end (); l++)
   {
      const Dtime& dtime = *(l);
      if (time_set.match (dtime)) { valid_time_vector.push_back (dtime); }
   }

   return valid_time_vector;

}

Real
Model::Acncrjbf::evaluate (const Dstring& varname,
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
Model::Acncrjbf::evaluate (const Dstring& varname,
                           const Lat_Long& lat_long) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate (varname, i, j);
}

Real
Model::Acncrjbf::evaluate (const Met_Element& met_element,
                           const Lat_Long& lat_long,
                           const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate (met_element, i, j, l);
}

Real
Model::Acncrjbf::evaluate (const Met_Element& met_element,
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
         datum = evaluate_raw ("dewpt", i, j, l);
         break;
         //const Real q = evaluate_raw ("qsair", i, j, l);
         //const Real mslp = evaluate_raw ("mslp", i, j, l);
         //const Real topography = get_topography (i, j);
         //const Real surface_p = mslp - 11.76 * topography;
         //const Real r = q / (1 - q);
         //datum = Thermo_Point::p_r_s (surface_p, r).get_t () + K;
         //break;
      };

      case RH:
      {
         const Real t = evaluate_raw ("temp", i, j, l);
         const Real t_d = evaluate_raw ("dewpt", i, j, l);
         //const Real q = evaluate_raw ("qsair", i, j, l);
         //const Real mslp = evaluate_raw ("mslp", i, j, l);
         //const Real topography = get_topography (i, j);
         //const Real surface_p = mslp - 11.76 * topography;
         //const Real r = q / (1 - q);
         //const Real t_d = Thermo_Point::p_r_s (surface_p, r).get_t () + K;
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

         Dstring varname ("");

         switch (met_element)
         {
            case T:           { varname = "temp"; break;    }
            case Q:           { varname = "qsair"; break;   }
            case MSLP:        { varname = "mslp"; break;    }
            case PRECIP_RATE: { varname = "prcp8p5"; break; }
            case U:           { varname = "xwind"; break;   }
            case V:           { varname = "ywind"; break;   }
         }

         datum = evaluate_raw (varname, i, j, l);
         break;

      }

   }

   return Real (datum);

}

Real
Model::Acncrjbf::evaluate (const Met_Element& met_element,
                           const Lat_Long& lat_long,
                           const Level& level,
                           const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate (met_element, i, j, level, l);
}

Real
Model::Acncrjbf::evaluate (const Met_Element& met_element,
                           const size_t i,
                           const size_t j,
                           const Level& level,
                           const size_t l) const
{

   size_t k;
   acquire_k (k, met_element, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   const bool is_w = (met_element == W);
   return evaluate (met_element, i, j, (is_w ? k + 1 : k), l);

}

Real
Model::Acncrjbf::evaluate (const Met_Element& met_element,
                           const Lat_Long& lat_long,
                           const size_t k,
                           const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate (met_element, i, j, k, l);
}

Real
Model::Acncrjbf::evaluate (const Met_Element& met_element,
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
         const Real p = evaluate_raw ("ml_ptheta", i, j, k, l - 1);
         datum = Thermo_Point::theta_p (theta - K, p).get_t () + K;
         break;
      };

      case TD:
      {
         const Real q = evaluate (Q, i, j, k, l);
         const Real p = evaluate_raw ("ml_ptheta", i, j, k, l - 1);
         const Real r = q / (1 - q);
         datum = Thermo_Point::p_r_s (p, r).get_t () + K;
         break;
      };

      case RH:
      {
         const Real t = evaluate (T, i, j, k, l);
         const Real q = evaluate (Q, i, j, k, l);
         const Real p = evaluate_raw ("ml_ptheta", i, j, k, l - 1);
         const Real r = q / (1 - q);
         const Real t_d = Thermo_Point::p_r_s (p, r).get_t () + K;
         datum = Moisture::get_rh (t - K, t_d - K, WATER);
         break;
      };

      case RHO:
      {
         const Real theta = evaluate_raw ("ml_theta", i, j, k, l);
         const Real p = evaluate_raw ("ml_ptheta", i, j, k, l - 1);
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
         const Real p = evaluate_raw ("ml_ptheta", i, j, k, l - 1);
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
         const Real p = evaluate_raw ("ml_ptheta", i, j, k, l - 1);
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

         Dstring varname ("");

         switch (met_element)
         {

            case U:
            {
               varname = "ml_xwind";
               datum = evaluate_raw (varname, i, j, k, l);
               break;
            } 

            case V:
            {
               varname = "ml_ywind";
               datum = evaluate_raw (varname, i, j, k, l);
               break;
            } 

            case W:
            {
               varname = "ml_zwind";
               datum = evaluate_raw (varname, i, j, k, l);
               break;
            } 

            case THETA:
            {
               varname = "ml_theta";
               datum = evaluate_raw (varname, i, j, k, l);
               break;
            } 

            case Q:
            {
               varname = "ml_spechum";
               datum = evaluate_raw (varname, i, j, k, l);
               break;
            } 

            case P_THETA:

            {
               varname = "ml_ptheta";
               datum = evaluate_raw (varname, i, j, k, l - 1);
               break;
            } 

            case P_RHO:
            {
               varname = "ml_prho";
               datum = evaluate_raw (varname, i, j, k, l - 1);
               break;
            } 

         }

         break;

      }

   }

   return Real (datum);

}

Real
Model::Acncrjbf::evaluate (const Met_Element& met_element,
                           const Lat_Long& lat_long,
                           const size_t k,
                           const Dtime& dtime) const
{

   if (k < 0)
   {
      const Integer l = get_surface_l (dtime);
      return evaluate (met_element, lat_long, l);
   }
   else
   {
      const Integer l = get_uppers_l (dtime);
      return evaluate (met_element, lat_long, k, l);
   }

}

Real
Model::Acncrjbf::evaluate (const Met_Element& met_element,
                           const Lat_Long& lat_long,
                           const Level& level,
                           const Dtime& dtime) const
{

   if (level.type == Level::SURFACE)
   {
      const Integer l = get_surface_l (dtime);
      return evaluate (met_element, lat_long, l);
   }

   switch (met_element)
   {

      case MSLP:
      case PRECIP_RATE:
      case FFDI:
      {
         const Integer l = get_surface_l (dtime);
         return evaluate (met_element, lat_long, l);
      }

      default:
      {
         const Integer l = get_uppers_l (dtime);
         return evaluate (met_element, lat_long, level, l);
      }

   }

}

Real
Model::Acncrjbf::evaluate_dt (const Met_Element& met_element,
                              const Lat_Long& lat_long,
                              const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dt (met_element, i, j, l);
}

Real
Model::Acncrjbf::evaluate_dt (const Met_Element& met_element,
                              const size_t i,
                              const size_t j,
                              const size_t l) const
{

   const Integer n = valid_surface_time_vector.size ();
   if (l < 0 || l >= n) { return GSL_NAN; }

   const Integer upper_l = (l == n-1 ? l : l+1);
   const Integer lower_l = (l == 0   ? l : l-1);
   const Real upper = evaluate (met_element, i, j, upper_l);
   const Real lower = evaluate (met_element, i, j, lower_l);
   const Real delta = upper - lower;
   const Real upper_t = valid_surface_time_vector[upper_l].t;
   const Real lower_t = valid_surface_time_vector[lower_l].t;
   const Real dt = (upper_t - lower_t) * 3600;
   return (delta / dt);

}

Real
Model::Acncrjbf::evaluate_dt (const Met_Element& met_element,
                              const Lat_Long& lat_long,
                              const Level& level,
                              const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dt (met_element, i, j, level, l);
}

Real
Model::Acncrjbf::evaluate_dt (const Met_Element& met_element,
                              const size_t i,
                              const size_t j,
                              const Level& level,
                              const size_t l) const
{

   size_t k;
   acquire_k (k, met_element, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   const bool is_w = (met_element == W);
   return evaluate_dt (met_element, i, j, (is_w ? k + 1 : k), l);

}

Real
Model::Acncrjbf::evaluate_dt (const Met_Element& met_element,
                              const Lat_Long& lat_long,
                              const size_t k,
                              const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dx (met_element, i, j, k, l);
}

Real
Model::Acncrjbf::evaluate_dt (const Met_Element& met_element,
                              const size_t i,
                              const size_t j,
                              const size_t k,
                              const size_t l) const
{

   const Integer n = valid_uppers_time_vector.size ();
   if (l < 0 || l >= n) { return GSL_NAN; }

   const Integer upper_l = (l == n-1 ? l : l+1);
   const Integer lower_l = (l == 0   ? l : l-1);
   const Real upper = evaluate (met_element, i, j, k, upper_l);
   const Real lower = evaluate (met_element, i, j, k, lower_l);
   const Real delta = upper - lower;
   const Real upper_t = valid_uppers_time_vector[upper_l].t;
   const Real lower_t = valid_uppers_time_vector[lower_l].t;
   const Real dt = (upper_t - lower_t) * 3600;
   return (delta / dt);

}

Real
Model::Acncrjbf::evaluate_dx (const Met_Element& met_element,
                              const Lat_Long& lat_long,
                              const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dx (met_element, i, j, l);
}

Real
Model::Acncrjbf::evaluate_dx (const Met_Element& met_element,
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
Model::Acncrjbf::evaluate_dx (const Met_Element& met_element,
                              const Lat_Long& lat_long,
                              const Level& level,
                              const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dx (met_element, i, j, level, l);
}

Real
Model::Acncrjbf::evaluate_dx (const Met_Element& met_element,
                              const size_t i,
                              const size_t j,
                              const Level& level,
                              const size_t l) const
{

   // Note k could be different upstream / downstream

   size_t upper_k, lower_k;
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
Model::Acncrjbf::evaluate_dx (const Met_Element& met_element,
                              const Lat_Long& lat_long,
                              const size_t k,
                              const size_t l) const
{
   // calculation along constant k
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dx (met_element, i, j, k, l);
}

Real
Model::Acncrjbf::evaluate_dx (const Met_Element& met_element,
                              const size_t i,
                              const size_t j,
                              const size_t k,
                              const size_t l) const
{

   // calculation along constant k

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
Model::Acncrjbf::evaluate_dy (const Met_Element& met_element,
                              const Lat_Long& lat_long,
                              const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dy (met_element, i, j, l);
}

Real
Model::Acncrjbf::evaluate_dy (const Met_Element& met_element,
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
   const Real dlatitude = tuple_latitude[upper_i] - tuple_latitude[lower_i];
   return (delta / dlatitude) / (LATITUDE_LENGTH);

}

Real
Model::Acncrjbf::evaluate_dy (const Met_Element& met_element,
                              const Lat_Long& lat_long,
                              const Level& level,
                              const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dy (met_element, i, j, level, l);
}

Real
Model::Acncrjbf::evaluate_dy (const Met_Element& met_element,
                              const size_t i,
                              const size_t j,
                              const Level& level,
                              const size_t l) const
{

   // Note k could be different upstream / downstream

   size_t upper_k, lower_k;
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
   const Real dlatitude = tuple_latitude[upper_i] - tuple_latitude[lower_i];
   return (delta / dlatitude) / (LATITUDE_LENGTH);

}

Real
Model::Acncrjbf::evaluate_dy (const Met_Element& met_element,
                           const Lat_Long& lat_long,
                           const size_t k,
                           const size_t l) const
{
   // calculation along constant k
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dy (met_element, i, j, k, l);
}

Real
Model::Acncrjbf::evaluate_dy (const Met_Element& met_element,
                           const size_t i,
                           const size_t j,
                           const size_t k,
                           const size_t l) const
{

   // calculation along constant k

   const Integer n = tuple_latitude.size ();
   if (i < 0 || i >= n) { return GSL_NAN; }

   const Integer upper_i = (i == n-1 ? i : i+1);
   const Integer lower_i = (i == 0   ? i : i-1);
   const Real upper = evaluate (met_element, upper_i, j, k, l);
   const Real lower = evaluate (met_element, lower_i, j, k, l);
   const Real delta = upper - lower;
   const Real dlatitude = tuple_latitude[upper_i] - tuple_latitude[lower_i];
   return (delta / dlatitude) / (LATITUDE_LENGTH);

}

Real
Model::Acncrjbf::evaluate_dz (const Met_Element& met_element,
                           const Lat_Long& lat_long,
                           const Level& level,
                           const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dz (met_element, i, j, level, l);
}

Real
Model::Acncrjbf::evaluate_dz (const Met_Element& met_element,
                           const size_t i,
                           const size_t j,
                           const Level& level,
                           const size_t l) const
{
   size_t k;
   acquire_k (k, met_element, i, j, level);
   return evaluate_dz (met_element, i, j, k, l);
}

Real
Model::Acncrjbf::evaluate_dz (const Met_Element& met_element,
                           const Lat_Long& lat_long,
                           const size_t k,
                           const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_dy (met_element, i, j, k, l);
}

Real
Model::Acncrjbf::evaluate_dz (const Met_Element& met_element,
                           const size_t i,
                           const size_t j,
                           const size_t k,
                           const size_t l) const
{

   const Integer n = 70;
   if (k < 1 || k >= n - 1) { return GSL_NAN; }

   const bool is_theta = Model::Acncrjbf::is_theta (met_element);

   const Integer k_0 = (k == 0   ? k : k-1);
   const Integer k_1 = k;
   const Integer k_2 = (k == n-1 ? k : k+1);

   const Real topography = get_topography (i, j);
   const Real z_0 = vertical_coefficients.get_z (k_0, topography, is_theta);
   const Real z_1 = vertical_coefficients.get_z (k_1, topography, is_theta);
   const Real z_2 = vertical_coefficients.get_z (k_2, topography, is_theta);

   const Real datum_0 = evaluate (met_element, i, j, k_0, l);
   const Real datum_1 = evaluate (met_element, i, j, k_1, l);
   const Real datum_2 = evaluate (met_element, i, j, k_2, l);

   return Differentiation::d_1 (datum_0, datum_1, datum_2, z_0, z_1, z_2);

}

Real
Model::Acncrjbf::evaluate_tendency (const Met_Element& met_element,
                                    const Lat_Long& lat_long,
                                    const size_t l) const
{
   return evaluate_dt (met_element, lat_long, l);
}

Real
Model::Acncrjbf::evaluate_tendency (const Met_Element& met_element,
                                    const Lat_Long& lat_long,
                                    const size_t l,
                                    const Wind& wind_bg) const
{

   const Met_Element& me = met_element;
   const Lat_Long& ll = lat_long;

   const Real t = evaluate_dt (me, ll, l);
   const Real a = evaluate_wind_advection (me, ll, l, wind_bg);

   return t + a;

}

Real
Model::Acncrjbf::evaluate_tendency (const Met_Element& met_element,
                                    const Lat_Long& lat_long,
                                    const Level& level,
                                    const size_t l) const
{
   return evaluate_dt (met_element, lat_long, level, l);
}

Real
Model::Acncrjbf::evaluate_tendency (const Met_Element& met_element,
                                    const Lat_Long& lat_long,
                                    const Level& level,
                                    const size_t l,
                                    const Wind& wind_bg) const
{

   const Met_Element& me = met_element;
   const Lat_Long& ll = lat_long;

   const Real t = evaluate_dt (me, ll, level, l);
   const Real a = evaluate_wind_advection (me, ll, level, l, wind_bg);

   return t + a;

}

Real
Model::Acncrjbf::evaluate_wind_advection (const Met_Element& met_element,
                                          const Lat_Long& lat_long,
                                          const size_t l,
                                          const Wind& wind) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_wind_advection (met_element, i, j, l, wind);
}

Real
Model::Acncrjbf::evaluate_wind_advection (const Met_Element& met_element,
                                          const size_t i,
                                          const size_t j,
                                          const size_t l,
                                          const Wind& wind) const
{
   const Real ddx = evaluate_dx (met_element, i, j, l);
   const Real ddy = evaluate_dy (met_element, i, j, l);
   return -wind.u * ddx - wind.v * ddy;
}

Real
Model::Acncrjbf::evaluate_wind_advection (const Met_Element& met_element,
                                          const Lat_Long& lat_long,
                                          const Level& level,
                                          const size_t l,
                                          const Wind& wind) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_wind_advection (met_element, i, j, level, l, wind);
}

Real
Model::Acncrjbf::evaluate_wind_advection (const Met_Element& met_element,
                                          const size_t i,
                                          const size_t j,
                                          const Level& level,
                                          const size_t l,
                                          const Wind& wind) const
{

   size_t k, k_rho;
   acquire_k (k_rho, U, i, j, level);
   acquire_k (k, met_element, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   const Real ddx = evaluate_dx (met_element, i, j, level, l);
   const Real ddy = evaluate_dy (met_element, i, j, level, l);

   return -wind.u * ddx - wind.v * ddy;

}

Real
Model::Acncrjbf::evaluate_h_advection (const Met_Element& met_element,
                                       const Lat_Long& lat_long,
                                       const size_t l,
                                       const Wind& wind_bg) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_h_advection (met_element, i, j, l, wind_bg);
}

Real
Model::Acncrjbf::evaluate_h_advection (const Met_Element& met_element,
                                       const size_t i,
                                       const size_t j,
                                       const size_t l,
                                       const Wind& wind_bg) const
{

   const Real u = evaluate (U, i, j, l);
   const Real v = evaluate (V, i, j, l);
   const Real ddx = evaluate_dx (met_element, i, j, l);
   const Real ddy = evaluate_dy (met_element, i, j, l);

   return -(u - wind_bg.u) * ddx - (v - wind_bg.v) * ddy;

}

Real
Model::Acncrjbf::evaluate_h_advection (const Met_Element& met_element,
                                       const Lat_Long& lat_long,
                                       const Level& level,
                                       const size_t l,
                                       const Wind& wind_bg) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_h_advection (met_element, i, j, level, l, wind_bg);
}

Real
Model::Acncrjbf::evaluate_h_advection (const Met_Element& met_element,
                                       const size_t i,
                                       const size_t j,
                                       const Level& level,
                                       const size_t l,
                                       const Wind& wind_bg) const
{

   size_t k, k_rho;
   acquire_k (k_rho, U, i, j, level);
   acquire_k (k, met_element, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   const Real u = evaluate (U, i, j, k_rho, l);
   const Real v = evaluate (V, i, j, k_rho, l);
   const Real ddx = evaluate_dx (met_element, i, j, level, l);
   const Real ddy = evaluate_dy (met_element, i, j, level, l);

   return -(u - wind_bg.u) * ddx - (v - wind_bg.v) * ddy;

}

Real
Model::Acncrjbf::evaluate_v_advection (const Met_Element& met_element,
                                       const Lat_Long& lat_long,
                                       const Level& level,
                                       const size_t l) const
{
   size_t i, j, k, k_w;
   acquire_ij (i, j, lat_long);
   return evaluate_v_advection (met_element, i, j, level, l);
}

Real
Model::Acncrjbf::evaluate_v_advection (const Met_Element& met_element,
                                       const size_t i,
                                       const size_t j,
                                       const Level& level,
                                       const size_t l) const
{

   size_t k, k_w;

   acquire_k (k, met_element, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   acquire_k (k_w, W, i, j, level);
   if (k_w < 0 || k_w > 69) { return GSL_NAN; }

   const Real w = evaluate (W, i, j, k_w, l);
   const Real ddz = evaluate_dz (met_element, i, j, k, l);

   return -w * ddz;

}

Real
Model::Acncrjbf::evaluate_s_tendency (const Met_Element& met_element,
                                      const Real azimuth,
                                      const Lat_Long& lat_long,
                                      const Level& level,
                                      const size_t l,
                                      const Real u_bg) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_s_tendency (met_element, azimuth, i, j, level, l, u_bg);
}

Real
Model::Acncrjbf::evaluate_s_tendency (const Met_Element& met_element,
                                      const Real azimuth,
                                      const size_t i,
                                      const size_t j,
                                      const Level& level,
                                      const size_t l,
                                      const Real u_bg) const
{
   const Met_Element& me = met_element;
   const Real t = evaluate_dt (me, i, j, level, l);
   const Real a = evaluate_s_advection (me, azimuth, i, j, level, l, u_bg);
   return t + a;
}

Real
Model::Acncrjbf::evaluate_s_advection (const Met_Element& met_element,
                                       const Real azimuth,
                                       const Lat_Long& lat_long,
                                       const Level& level,
                                       const size_t l,
                                       const Real u_bg) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_s_advection (met_element, azimuth, i, j, level, l, u_bg);
}

Real
Model::Acncrjbf::evaluate_s_advection (const Met_Element& met_element,
                                       const Real azimuth,
                                       const size_t i,
                                       const size_t j,
                                       const Level& level,
                                       const size_t l,
                                       const Real u_bg) const
{

   size_t k, k_rho;
   acquire_k (k, met_element, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   acquire_k (k_rho, U, i, j, level);
   if (k_rho < 0 || k_rho > 69) { return GSL_NAN; }

   const Real theta = azimuth * DEGREE_TO_RADIAN;
   const Real st = sin (theta);
   const Real ct = cos (theta);

   const Real u = evaluate (U, i, j, k_rho, l);
   const Real v = evaluate (V, i, j, k_rho, l);
   const Real ddx = evaluate_dx (met_element, i, j, level, l);
   const Real ddy = evaluate_dy (met_element, i, j, level, l);

   const Real s = u * st + v * ct;
   const Real dds = ddx * st + ddy * ct;
   return -(s - u_bg) * dds;

}

Real
Model::Acncrjbf::evaluate_n_advection (const Met_Element& met_element,
                                       const Real azimuth,
                                       const Lat_Long& lat_long,
                                       const Level& level,
                                       const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_n_advection (met_element, azimuth, i, j, level, l);
}

Real
Model::Acncrjbf::evaluate_n_advection (const Met_Element& met_element,
                                       const Real azimuth,
                                       const size_t i,
                                       const size_t j,
                                       const Level& level,
                                       const size_t l) const
{

   size_t k, k_rho;

   acquire_k (k, met_element, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   acquire_k (k_rho, U, i, j, level);
   if (k_rho < 0 || k_rho > 69) { return GSL_NAN; }

   const Real theta = azimuth * DEGREE_TO_RADIAN;
   const Real st = sin (theta);
   const Real ct = cos (theta);

   const Real u = evaluate (U, i, j, k_rho, l);
   const Real v = evaluate (V, i, j, k_rho, l);
   const Real ddx = evaluate_dx (met_element, i, j, level, l);
   const Real ddy = evaluate_dy (met_element, i, j, level, l);

   const Real n = v * st - u * ct;
   const Real ddn = ddy * st - ddx * ct;
   return -n * ddn;

}

Real
Model::Acncrjbf::evaluate_normal_speed (const Real azimuth,
                                        const Lat_Long& lat_long,
                                        const Level& level,
                                        const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_normal_speed (azimuth, i, j, level, l);
}

Real
Model::Acncrjbf::evaluate_normal_speed (const Real azimuth,
                                        const size_t i,
                                        const size_t j,
                                        const Level& level,
                                        const size_t l) const
{

   size_t k;
   acquire_k (k, U, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   const Real u = evaluate (U, i, j, k, l);
   const Real v = evaluate (V, i, j, k, l);

   const Real theta = azimuth * DEGREE_TO_RADIAN;
   return v * sin (theta) - u * cos (theta);

}

Real
Model::Acncrjbf::evaluate_along_speed (const Real azimuth,
                                       const Lat_Long& lat_long,
                                       const Level& level,
                                       const size_t l,
                                       const Real u_bg) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_along_speed (azimuth, i, j, level, l, u_bg);
}

Real
Model::Acncrjbf::evaluate_along_speed (const Real azimuth,
                                       const size_t i,
                                       const size_t j,
                                       const Level& level,
                                       const size_t l,
                                       const Real u_bg) const
{

   size_t k;
   acquire_k (k, U, i, j, level);
   if (k < 0 || k > 69) { return GSL_NAN; }

   const Real u = evaluate (U, i, j, k, l);
   const Real v = evaluate (V, i, j, k, l);

   const Real theta = azimuth * DEGREE_TO_RADIAN;
   return u * sin (theta) + v * cos (theta) - u_bg;

}

Real
Model::Acncrjbf::evaluate_richardson (const Real azimuth,
                                      const Lat_Long& lat_long,
                                      const Level& level,
                                      const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_richardson (azimuth, i, j, level, l);
}

Real
Model::Acncrjbf::evaluate_richardson (const Real azimuth,
                                      const size_t i,
                                      const size_t j,
                                      const Level& level,
                                      const size_t l) const
{

   size_t k_rho, k_theta;

   acquire_k (k_rho, U, i, j, level);
   if (k_rho <= 0 || k_rho >= 69) { return GSL_NAN; }

   acquire_k (k_theta, THETA, i, j, level);
   if (k_theta <= 0 || k_theta >= 69) { return GSL_NAN; }

   const Model::Acncrjbf::Vertical_Coefficients& vc = vertical_coefficients;
   const Real topography = get_topography (i, j);

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

   const Real z_theta_0 = vc.get_z (k_theta_0, topography, true);
   const Real z_theta_1 = vc.get_z (k_theta_1, topography, true);
   const Real z_theta_2 = vc.get_z (k_theta_2, topography, true);

   const Real z_rho_0 = vc.get_z (k_rho_0, topography, false);
   const Real z_rho_1 = vc.get_z (k_rho_1, topography, false);
   const Real z_rho_2 = vc.get_z (k_rho_2, topography, false);

   typedef Differentiation D;
   const Real dtheta_dz = D::d_1 (theta_0, theta_1,
      theta_2, z_theta_0, z_theta_1, z_theta_2);
   const Real N2 = (g / theta_1 * dtheta_dz );
   const Real du_dz = D::d_1 (u_0, u_1, u_2, z_rho_0, z_rho_1, z_rho_2);
   const Real dv_dz = D::d_1 (v_0, v_1, v_2, z_rho_0, z_rho_1, z_rho_2);

   return N2 / ((du_dz * du_dz) + (dv_dz * dv_dz));

}

Real
Model::Acncrjbf::evaluate_scorer (const Real azimuth,
                                  const Lat_Long& lat_long,
                                  const Level& level,
                                  const size_t l,
                                  const Real u_bg) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_scorer (azimuth, i, j, level, l, u_bg);
}

Real
Model::Acncrjbf::evaluate_scorer (const Real azimuth,
                                  const size_t i,
                                  const size_t j,
                                  const Level& level,
                                  const size_t l,
                                  const Real u_bg) const
{

   size_t k_rho, k_theta;

   acquire_k (k_rho, U, i, j, level);
   if (k_rho <= 0 || k_rho >= 69) { return GSL_NAN; }

   acquire_k (k_theta, THETA, i, j, level);
   if (k_theta <= 0 || k_theta >= 69) { return GSL_NAN; }

   const Model::Acncrjbf::Vertical_Coefficients& vc = vertical_coefficients;
   const Real topography = get_topography (i, j);

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
   const Real along_0 = u_0 * sin (theta) + v_0 * cos (theta);
   const Real along_1 = u_1 * sin (theta) + v_1 * cos (theta);
   const Real along_2 = u_2 * sin (theta) + v_2 * cos (theta);

   const Real z_theta_0 = vc.get_z (k_theta_0, topography, true);
   const Real z_theta_1 = vc.get_z (k_theta_1, topography, true);
   const Real z_theta_2 = vc.get_z (k_theta_2, topography, true);

   const Real z_rho_0 = vc.get_z (k_rho_0, topography, false);
   const Real z_rho_1 = vc.get_z (k_rho_1, topography, false);
   const Real z_rho_2 = vc.get_z (k_rho_2, topography, false);

   typedef Differentiation D;
   const Real dtheta_dz = D::d_1 (theta_0, theta_1,
      theta_2, z_theta_0, z_theta_1, z_theta_2);
   const Real d2along_dz2 = D::d2 (along_0, along_1,
      along_2, z_rho_0, z_rho_1, z_rho_2);

   const Real relative_u = (along_1 - u_bg);
   const Real A = (g / theta_1 * dtheta_dz) / (relative_u * relative_u);
   const Real B = -d2along_dz2 / relative_u;

if (l == 76 && fabs (level.value - 3445) < 2)
{
   cout << level.get_string () << "\t" << along_0 << "\t" << along_1 << "\t" << along_2 << "\t" << z_rho_0 << "\t" << z_rho_1 << "\t" << z_rho_2 << "\t" << relative_u << "\t" << B << endl;
   cout << "   " << u_0 << " " << v_1 << " " << along_1 << endl;
   cout << "   " << azimuth << " " << theta << endl;
}
   return A + B;

}

Real
Model::Acncrjbf::evaluate_scorer_a (const Real azimuth,
                                 const Lat_Long& lat_long,
                                 const Level& level,
                                 const size_t l,
                                 const Real u_bg) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_scorer_a (azimuth, i, j, level, l, u_bg);
}

Real
Model::Acncrjbf::evaluate_scorer_a (const Real azimuth,
                                 const size_t i,
                                 const size_t j,
                                 const Level& level,
                                 const size_t l,
                                 const Real u_bg) const
{

   size_t k_rho, k_theta;

   acquire_k (k_rho, U, i, j, level);
   if (k_rho <= 0 || k_rho >= 69) { return GSL_NAN; }

   acquire_k (k_theta, THETA, i, j, level);
   if (k_theta <= 0 || k_theta >= 69) { return GSL_NAN; }

   const Model::Acncrjbf::Vertical_Coefficients& vc = vertical_coefficients;
   const Real topography = get_topography (i, j);

   const Integer k_theta_0 = k_theta - 1;
   const Integer k_theta_1 = k_theta;
   const Integer k_theta_2 = k_theta + 1;

   const Real theta_0 = evaluate (THETA, i, j, k_theta_0, l);
   const Real theta_1 = evaluate (THETA, i, j, k_theta_1, l);
   const Real theta_2 = evaluate (THETA, i, j, k_theta_2, l);

   const Real u = evaluate (U, i, j, k_rho, l);
   const Real v = evaluate (V, i, j, k_rho, l);

   const Real theta = azimuth * DEGREE_TO_RADIAN;
   const Real along = u * sin (theta) + v * cos (theta) - u_bg;

   const Real z_theta_0 = vc.get_z (k_theta_0, topography, true);
   const Real z_theta_1 = vc.get_z (k_theta_1, topography, true);
   const Real z_theta_2 = vc.get_z (k_theta_2, topography, true);

   typedef Differentiation D;
   const Real dtheta_dz = D::d_1 (theta_0, theta_1,
      theta_2, z_theta_0, z_theta_1, z_theta_2);

   return (g / theta_1 * dtheta_dz ) / (along * along);

}

Real
Model::Acncrjbf::evaluate_scorer_b (const Real azimuth,
                                 const Lat_Long& lat_long,
                                 const Level& level,
                                 const size_t l,
                                 const Real u_bg) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_scorer_b (azimuth, i, j, level, l, u_bg);
}

Real
Model::Acncrjbf::evaluate_scorer_b (const Real azimuth,
                                 const size_t i,
                                 const size_t j,
                                 const Level& level,
                                 const size_t l,
                                 const Real u_bg) const
{

   size_t k_rho;

   acquire_k (k_rho, U, i, j, level);
   if (k_rho <= 0 || k_rho >= 69) { return GSL_NAN; }

   const Model::Acncrjbf::Vertical_Coefficients& vc = vertical_coefficients;
   const Real topography = get_topography (i, j);

   const Integer k_rho_0 = k_rho - 1;
   const Integer k_rho_1 = k_rho;
   const Integer k_rho_2 = k_rho + 1;

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

   const Real z_rho_0 = vc.get_z (k_rho_0, topography, false);
   const Real z_rho_1 = vc.get_z (k_rho_1, topography, false);
   const Real z_rho_2 = vc.get_z (k_rho_2, topography, false);

   typedef Differentiation D;
   return -D::d2 (along_0, along_1, along_2, z_rho_0,
      z_rho_1, z_rho_2) / along_1;

}

Real
Model::Acncrjbf::evaluate_brunt_vaisala (const Lat_Long& lat_long,
                                         const Level& level,
                                         const size_t l) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return evaluate_brunt_vaisala (i, j, level, l);
}

Real
Model::Acncrjbf::evaluate_brunt_vaisala (const size_t i,
                                         const size_t j,
                                         const Level& level,
                                         const size_t l) const
{

   size_t k;
   acquire_k (k, THETA, i, j, level);
   if (k <= 0 || k >= 69) { return GSL_NAN; }

   const Model::Acncrjbf::Vertical_Coefficients& vc = vertical_coefficients;
   const Real topography = get_topography (i, j);

   const Integer k_0 = k - 1;
   const Integer k_1 = k;
   const Integer k_2 = k + 1;

   const Real theta_0 = evaluate (THETA, i, j, k_0, l);
   const Real theta_1 = evaluate (THETA, i, j, k_1, l);
   const Real theta_2 = evaluate (THETA, i, j, k_2, l);

   const Real z_0 = vc.get_z (k_0, topography, true);
   const Real z_1 = vc.get_z (k_1, topography, true);
   const Real z_2 = vc.get_z (k_2, topography, true);

   typedef Differentiation D;
   const Real dtheta_dz = D::d_1 (theta_0, theta_1, theta_2, z_0, z_1, z_2);

   if (dtheta_dz <= 0) { return GSL_NAN; }
   else { return sqrt (g / theta_1 * dtheta_dz); }

}

Real
Model::Acncrjbf::get_topography (const Lat_Long& lat_long) const
{
   size_t i, j;
   acquire_ij (i, j, lat_long);
   return get_topography (i, j);
}

Real
Model::Acncrjbf::get_topography (const size_t i,
                              const size_t j) const
{
   return evaluate ("orog", i, j);
}

Color
Model::Acncrjbf::get_color (const Product& product,
                            const Lat_Long& lat_long,
                            const size_t l) const
{

   switch (product.enumeration)
   {

      case Product::NIL:
      {
         return Color::white ();
      }

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

      case Product::Q_TENDENCY:
      {
         const Real datum = evaluate_tendency (Q, lat_long, l);
         return Model::Stage::get_color (product, datum);
      }

      case Product::Q_ADVECTION:
      case Product::Q_H_ADVECTION:
      {
         const Real datum = evaluate_h_advection (Q, lat_long, l);
         return Model::Stage::get_color (product, datum);
      }

      case Product::THETA_TENDENCY:
      {
         const Real datum = evaluate_tendency (THETA, lat_long, l);
         return Model::Stage::get_color (product, datum);
      }

      case Product::THETA_ADVECTION:
      case Product::THETA_H_ADVECTION:
      {
         const Real datum = evaluate_h_advection (THETA, lat_long, l);
         return Model::Stage::get_color (product, datum);
      }

      case Product::TERRAIN_MONOCHROME:
      {
         const Real datum = get_topography (lat_long);
         return Model::Stage::get_color (product, datum);
      };

      default:
      {
         const Met_Element met_element = product.get_met_element ();
         const Real datum = evaluate (met_element, lat_long, l);
         return Model::Stage::get_color (product, datum);
      }

   }

}

Real
Model::Acncrjbf::get_value (const Product& product,
                            const Lat_Long& lat_long,
                            const size_t l) const
{

   switch (product.enumeration)
   {

      case Product::WIND:
      {
         const Real u = evaluate (U, lat_long, l);
         const Real v = evaluate (V, lat_long, l);
         const Real theta = atan2 (-u, -v);
         const Real speed = sqrt (u*u + v*v);
         const Real direction = theta * RADIAN_TO_DEGREE;
         return direction;
      }

      case Product::Q_TENDENCY:
      {
         const Real datum = evaluate_tendency (Q, lat_long, l);
         return datum;
      }

      case Product::Q_ADVECTION:
      case Product::Q_H_ADVECTION:
      {
         const Real datum = evaluate_h_advection (Q, lat_long, l);
         return datum;
      }

      case Product::THETA_TENDENCY:
      {
         const Real datum = evaluate_tendency (THETA, lat_long, l);
         return datum;
      }

      case Product::THETA_ADVECTION:
      case Product::THETA_H_ADVECTION:
      {
         const Real datum = evaluate_h_advection (THETA, lat_long, l);
         return datum;
      }

      default:
      {
         const Met_Element met_element = product.get_met_element ();
         const Real datum = evaluate (met_element, lat_long, l);
         return datum;
      }

   }

}

Color
Model::Acncrjbf::get_color (const Product& product,
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

      case Product::Q_TENDENCY:
      {
         const Real datum = evaluate_tendency (Q, lat_long, level, l);
         return Model::Stage::get_color (product, datum);
      }

      case Product::Q_ADVECTION:
      {
         const Real h = evaluate_h_advection (Q, lat_long, level, l);
         const Real v = evaluate_v_advection (Q, lat_long, level, l);
         const Real datum = h + v;
         return Model::Stage::get_color (product, datum);
      }

      case Product::Q_H_ADVECTION:
      {
         const Real datum = evaluate_h_advection (Q, lat_long, level, l);
         return Model::Stage::get_color (product, datum);
      }

      case Product::Q_V_ADVECTION:
      {
         const Real datum = evaluate_v_advection (Q, lat_long, level, l);
         return Model::Stage::get_color (product, datum);
      }

      case Product::THETA_TENDENCY:
      {
         const Real datum = evaluate_tendency (THETA, lat_long, level, l);
         return Model::Stage::get_color (product, datum);
      }

      case Product::THETA_ADVECTION:
      {
         const Real h = evaluate_h_advection (THETA, lat_long, level, l);
         const Real v = evaluate_v_advection (THETA, lat_long, level, l);
         const Real datum = h + v;
         return Model::Stage::get_color (product, datum);
      }

      case Product::THETA_H_ADVECTION:
      {
         const Real datum = evaluate_h_advection (THETA, lat_long, level, l);
         return Model::Stage::get_color (product, datum);
      }

      case Product::THETA_V_ADVECTION:
      {
         const Real datum = evaluate_v_advection (THETA, lat_long, level, l);
         return Model::Stage::get_color (product, datum);
      }

      default:
      {
 
         const Met_Element met_element = product.get_met_element ();

         size_t i, j, k;
         acquire_ij (i, j, lat_long);
         acquire_k (k, met_element, i, j, level);
         if (k < 0 || k > 69) { return transparent; }

         const Real datum = evaluate (met_element, i, j, k, l);
         return Model::Stage::get_color (product, datum);

      }

   }

}

Real
Model::Acncrjbf::get_value (const Product& product,
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
         if (k < 0 || k > 69) { return GSL_NAN; }

         const Real u = evaluate (U, i, j, k, l);
         const Real v = evaluate (V, i, j, k, l);
         const Real speed = sqrt (u*u + v*v);
         return speed;

      }

      case Product::Q_TENDENCY:
      {
         const Real datum = evaluate_tendency (Q, lat_long, level, l);
         return datum;
      }

      case Product::Q_ADVECTION:
      {
         const Real h = evaluate_h_advection (Q, lat_long, level, l);
         const Real v = evaluate_v_advection (Q, lat_long, level, l);
         const Real datum = h + v;
         return datum;
      }

      case Product::Q_H_ADVECTION:
      {
         const Real datum = evaluate_h_advection (Q, lat_long, level, l);
         return datum;
      }

      case Product::Q_V_ADVECTION:
      {
         const Real datum = evaluate_v_advection (Q, lat_long, level, l);
         return datum;
      }

      case Product::THETA_TENDENCY:
      {
         const Real datum = evaluate_tendency (THETA, lat_long, level, l);
         return datum;
      }

      case Product::THETA_ADVECTION:
      {
         const Real h = evaluate_h_advection (THETA, lat_long, level, l);
         const Real v = evaluate_v_advection (THETA, lat_long, level, l);
         const Real datum = h + v;
         return datum;
      }

      case Product::THETA_H_ADVECTION:
      {
         const Real datum = evaluate_h_advection (THETA, lat_long, level, l);
         return datum;
      }

      case Product::THETA_V_ADVECTION:
      {
         const Real datum = evaluate_v_advection (THETA, lat_long, level, l);
         return datum;
      }

      default:
      {
 
         const Met_Element met_element = product.get_met_element ();

         size_t i, j, k;
         acquire_ij (i, j, lat_long);
         acquire_k (k, met_element, i, j, level);
         if (k < 0 || k > 69) { return GSL_NAN; }

         const Real datum = evaluate (met_element, i, j, k, l);
         return datum;

      }

   }

}

Raster*
Model::Acncrjbf::get_terrain_raster_ptr (const Size_2D& size_2d,
                                         const Transform_2D& transform) const
{


   Raster* raster_ptr = new Raster (size_2d);
   Raster& raster = *raster_ptr;

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   Real orog, lsm;

   bool oob;
   Real scale_factor = 0.0025;

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i);

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j);
         transform.r (latitude, longitude, x, y);

         if (out_of_bounds (lat_long))
         {
            raster.set_pixel (i, j, Color::transparent ());
            continue;
         }

         const Real topography = get_topography (lat_long);
         const Real lsm = evaluate ("lsm", lat_long);
         const Model::Product product (lsm > 0.5 ?
            Model::Product::TERRAIN : Model::Product::TERRAIN_WATER);
         Color color = Model::Stage::get_color (product, topography);

         if (i != 0 && i != size_2d.i - 1 && j != 0 && i != size_2d.j - 1)
         {

            transform.r (latitude, longitude, Real (i+1), Real (j));
            oob = out_of_bounds (lat_long);
            const Real datum_e = oob ? GSL_NAN : get_topography (lat_long);

            transform.r (latitude, longitude, Real (i-1), Real (j));
            oob = out_of_bounds (lat_long);
            const Real datum_w = oob ? GSL_NAN : get_topography (lat_long);

            transform.r (latitude, longitude, Real (i), Real (j+1));
            oob = out_of_bounds (lat_long);
            const Real datum_n = oob ? GSL_NAN : get_topography (lat_long);

            transform.r (latitude, longitude, Real (i), Real (j-1));
            oob = out_of_bounds (lat_long);
            const Real datum_s = oob ? GSL_NAN : get_topography (lat_long);

            Real gradient = datum_e - datum_n - datum_w + datum_s;
            if (!gsl_finite (gradient)) { gradient = 0; }

            Real scale = 1 + gradient * scale_factor;
            scale = std::max (0.7, std::min (1.3, scale));
            color.scale_brightness (scale);

         }

         raster.set_pixel (i, j, color);

      }

   }

   return raster_ptr;

}

Scalar_Data_2D*
Model::Acncrjbf::get_terrain_scalar_data_2d_ptr (const Size_2D& size_2d,
                                                 const Transform_2D& transform) const
{


   const Domain_2D domain_2d (0, size_2d.i, 0, size_2d.j);
   Scalar_Data_2D* scalar_data_2d_ptr = new Scalar_Data_2D (size_2d, domain_2d);
   Scalar_Data_2D& scalar_data_2d = *scalar_data_2d_ptr;

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   Real orog, lsm;

   bool oob;
   Real scale_factor = 0.0025;

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i);

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j);
         transform.r (latitude, longitude, x, y);

         if (out_of_bounds (lat_long))
         {
            scalar_data_2d.set_datum (i, j, GSL_NAN);
            continue;
         }

         const Real topography = get_topography (lat_long);
         scalar_data_2d.set_datum (i, j, topography);

      }

   }

   return scalar_data_2d_ptr;

}

Raster*
Model::Acncrjbf::get_surface_raster_ptr (const Size_2D& size_2d,
                                         const Transform_2D& transform,
                                         const Model::Product& product,
                                         const Dtime& dtime) const
{

   Raster* raster_ptr = new Raster (size_2d);
   Raster& raster = *raster_ptr;

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   const size_t l = get_surface_l (dtime);

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i);

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j);
         transform.r (latitude, longitude, x, y);

         if (out_of_bounds (lat_long))
         {
            raster.set_pixel (i, j, Color::transparent ());
            continue;
         }

         const Color& color = get_color (product, lat_long, l);
         raster.set_pixel (i, j, color);

      }

   }

   return raster_ptr;

}

Scalar_Data_2D*
Model::Acncrjbf::get_surface_scalar_data_2d_ptr (const Size_2D& size_2d,
                                                 const Transform_2D& transform,
                                                 const Model::Product& product,
                                                 const Dtime& dtime) const
{

   const Domain_2D domain_2d (0, size_2d.i, 0, size_2d.j);
   Scalar_Data_2D* scalar_data_2d_ptr = new Scalar_Data_2D (size_2d, domain_2d);
   Scalar_Data_2D& scalar_data_2d = *scalar_data_2d_ptr;

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   const size_t l = get_surface_l (dtime);

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i);

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j);
         transform.r (latitude, longitude, x, y);

         if (out_of_bounds (lat_long))
         {
            scalar_data_2d.set_datum (i, j, GSL_NAN);
            continue;
         }

         const Real datum = get_value (product, lat_long, l);
         scalar_data_2d.set_datum (i, j, datum);

      }

   }

   return scalar_data_2d_ptr;

}
Raster*
Model::Acncrjbf::get_uppers_raster_ptr (const Size_2D& size_2d,
                                        const Transform_2D& transform,
                                        const Model::Product& product,
                                        const Dtime& dtime,
                                        const Level& level) const
{

   Raster* raster_ptr = new Raster (size_2d);
   Raster& raster = *raster_ptr;

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   const size_t l = get_uppers_l (dtime);

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i);

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j);
         transform.r (latitude, longitude, x, y);

         if (out_of_bounds (lat_long))
         {
            raster.set_pixel (i, j, Color::transparent ());
            continue;
         }

         const Color& color = get_color (product, lat_long, level, l);
         raster.set_pixel (i, j, color);

      }

   }

   return raster_ptr;

}

Scalar_Data_2D*
Model::Acncrjbf::get_uppers_scalar_data_2d_ptr (const Size_2D& size_2d,
                                                const Transform_2D& transform,
                                                const Model::Product& product,
                                                const Dtime& dtime,
                                                const Level& level) const
{

   const Domain_2D domain_2d (0, size_2d.i, 0, size_2d.j);
   Scalar_Data_2D* scalar_data_2d_ptr = new Scalar_Data_2D (size_2d, domain_2d);
   Scalar_Data_2D& scalar_data_2d = *scalar_data_2d_ptr;

   Lat_Long lat_long;
   Real& latitude = lat_long.latitude;
   Real& longitude = lat_long.longitude;

   const size_t l = get_uppers_l (dtime);

   for (Integer i = 0; i < size_2d.i; i++)
   {

      const Real x = Real (i);

      for (Integer j = 0; j < size_2d.j; j++)
      {

         const Real y = Real (j);
         transform.r (latitude, longitude, x, y);

         if (out_of_bounds (lat_long))
         {
            scalar_data_2d.set_datum (i, j, GSL_NAN);
            continue;
         }

         const Real datum = get_value (product, lat_long, level, l);
         scalar_data_2d.set_datum (i, j, datum);

      }

   }

   return scalar_data_2d_ptr;

}

Raster* 
Model::Acncrjbf::get_geo_raster_ptr (const Size_2D& size_2d,
                                     const Transform_2D& transform,
                                     const Product& product,
                                     const Dtime& dtime,
                                     const Level& level) const
{

   if (product.enumeration == Model::Product::TERRAIN)
   {
      return get_terrain_raster_ptr (size_2d, transform);
   }
   else
   if (level.type == Level::SURFACE ||
       product.enumeration == Model::Product::FFDI ||
       product.enumeration == Model::Product::MSLP ||
       product.enumeration == Model::Product::PRECIP_RATE)
   {
      return get_surface_raster_ptr (size_2d, transform, product, dtime);
   }
   else
   {
      return get_uppers_raster_ptr (size_2d, transform, product, dtime, level);
   }

}

Scalar_Data_2D* 
Model::Acncrjbf::get_geo_scalar_data_2d_ptr (const Size_2D& size_2d,
                                             const Transform_2D& transform,
                                             const Product& product,
                                             const Dtime& dtime,
                                             const Level& level) const
{

   if (product.enumeration == Model::Product::TERRAIN)
   {
      return get_terrain_scalar_data_2d_ptr (size_2d, transform);
   }
   else
   if (level.type == Level::SURFACE ||
       product.enumeration == Model::Product::FFDI ||
       product.enumeration == Model::Product::MSLP ||
       product.enumeration == Model::Product::PRECIP_RATE)
   {
      return get_surface_scalar_data_2d_ptr (
         size_2d, transform, product, dtime);
   }
   else
   {
      return get_uppers_scalar_data_2d_ptr (
         size_2d, transform, product, dtime, level);
   }

}

Raster*
Model::Acncrjbf::get_cross_section_raster_ptr (const Box_2D& box_2d,
                                               const Transform_2D& transform,
                                               const Model::Product& product,
                                               const Dtime& dtime,
                                               const Journey& journey,
                                               const Real u_bg) const
{

   Raster* raster_ptr = new Raster (box_2d);
   raster_ptr->paint (Color::white ());

   Color color;
   const size_t l = get_uppers_l (dtime);

   const bool is_speed = (product.enumeration == Model::Product::SPEED);
   const Model::Product& p = (is_speed ?
      Model::Product ("SPEED_HIGHER") : product);

   Real x, ceiling;
   Level level (Level::HEIGHT, GSL_NAN);
   Real& z = level.value;

   const Geodesy geodesy;
   const Index_2D& index_2d = box_2d.index_2d;
   const Size_2D& size_2d = box_2d.size_2d;
   const Real distance = journey.get_distance (geodesy);

   for (Integer i = index_2d.i; i < index_2d.i + size_2d.i; i++)
   {

      const Integer ii = i - index_2d.i;
      //transform.r (x, ceiling, Real (i), Real (index_2d.j));
      transform.r (x, ceiling, Real (i), Real (0));

      if (x < 0 || x > distance) { continue; }
      const Lat_Long& lat_long = journey.get_lat_long (x, geodesy);
      const Lat_Long& ll = lat_long;
      if (out_of_bounds (ll)) { continue; }

      const Real topography = get_topography (ll);
      const Real azimuth = journey.get_azimuth_forward (x, geodesy);

      switch (product.enumeration)
      {

         case Model::Product::SCORER:
         {

            const Sounding* sounding_ptr = get_sounding_ptr (
               lat_long, dtime, ceiling);
            const Real_Profile* profile_ptr =
               sounding_ptr->get_scorer_profile_ptr (azimuth, u_bg);

            for (Integer j = index_2d.j; j < index_2d.j + size_2d.j; j++)
            {

               const Integer jj = j - index_2d.j;
               transform.r (x, z, Real (i), Real (j));

               if (z < topography) { color = Color::black (); }
               else
               {
                  const Real pressure = sounding_ptr->get_pressure (z);
                  const Real datum = profile_ptr->get_ordinate (pressure);
                  color = Model::Stage::get_color (p, datum);
               }

               raster_ptr->set_pixel (ii, jj, color);

            }

            delete profile_ptr;
            delete sounding_ptr;
            break;

         };

         case Model::Product::SCORER_A:
         {

            const Sounding* sounding_ptr = get_sounding_ptr (
               lat_long, dtime, ceiling);
            const Real_Profile* profile_ptr =
               sounding_ptr->get_scorer_a_profile_ptr (azimuth, u_bg);

            for (Integer j = index_2d.j; j < index_2d.j + size_2d.j; j++)
            {

               const Integer jj = j - index_2d.j;
               transform.r (x, z, Real (i), Real (j));

               if (z < topography) { color = Color::black (); }
               else
               {
                  const Real pressure = sounding_ptr->get_pressure (z);
                  const Real datum = profile_ptr->get_ordinate (pressure);
                  color = Model::Stage::get_color (p, datum);
               }

               raster_ptr->set_pixel (ii, jj, color);

            }

            delete profile_ptr;
            delete sounding_ptr;
            break;

         };

         case Model::Product::SCORER_B:
         {

            const Sounding* sounding_ptr = get_sounding_ptr (
               lat_long, dtime, ceiling);
            const Real_Profile* profile_ptr =
               sounding_ptr->get_scorer_b_profile_ptr (azimuth, u_bg);

            for (Integer j = index_2d.j; j < index_2d.j + size_2d.j; j++)
            {

               const Integer jj = j - index_2d.j;
               transform.r (x, z, Real (i), Real (j));

               if (z < topography) { color = Color::black (); }
               else
               {
                  const Real pressure = sounding_ptr->get_pressure (z);
                  const Real datum = profile_ptr->get_ordinate (pressure);
                  color = Model::Stage::get_color (p, datum);
               }

               raster_ptr->set_pixel (ii, jj, color);

            }

            delete profile_ptr;
            delete sounding_ptr;
            break;

         };

         default:
         {

            for (Integer j = index_2d.j; j < index_2d.j + size_2d.j; j++)
            {

               transform.r (x, z, Real (i), Real (j));

               if (z < topography) { color = Color::black (); }
               else
               {

                  switch (product.enumeration)
                  {

                     case Model::Product::WIND:
                     {
                        const Real u = evaluate (U, ll, level, l);
                        const Real v = evaluate (V, ll, level, l);
                        const Wind wind (u, v);
                        const Real direction = wind.get_direction ();
                        const Real speed = wind.get_speed ();
                        color = get_wind_color (direction, speed);
                        break;
                     }

                     case Model::Product::BRUNT_VAISALA:
                     {
                        const Real datum = evaluate_brunt_vaisala (ll, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::RICHARDSON:
                     {
                        const Real datum = evaluate_richardson (
                           azimuth, ll, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::SCORER:
                     {
                        const Real datum = evaluate_scorer (
                           azimuth, ll, level, l, u_bg);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::SCORER_A:
                     {
                        const Real datum = evaluate_scorer_a (
                           azimuth, ll, level, l, u_bg);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::SCORER_B:
                     {
                        const Real datum = evaluate_scorer_b (
                           azimuth, ll, level, l, u_bg);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::ALONG_SPEED:
                     {
                        const Real datum = evaluate_along_speed (
                           azimuth, ll, level, l, u_bg);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::NORMAL_SPEED:
                     {
                        const Real datum = evaluate_normal_speed (
                           azimuth, lat_long, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_TENDENCY:
                     {
                        size_t i, j;
                        const Real datum = evaluate_s_tendency (
                           Q, azimuth, lat_long, level, l, u_bg);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_ADVECTION:
                     {
                        const Real h = evaluate_h_advection (Q, ll, level, l);
                        const Real v = evaluate_v_advection (Q, ll, level, l);
                        const Real datum = h + v;
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_H_ADVECTION:
                     {
                        const Real datum = evaluate_h_advection (Q, ll, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_V_ADVECTION:
                     {
                        const Real datum = evaluate_v_advection (Q, ll, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_S_ADVECTION:
                     {
                        const Real datum = evaluate_s_advection (Q, azimuth, ll, level, l, u_bg);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_N_ADVECTION:
                     {
                        const Real datum = evaluate_n_advection (Q, azimuth, ll, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_SV_ADVECTION:
                     {
                        const Real s = evaluate_s_advection (Q, azimuth, ll, level, l, u_bg);
                        const Real v = evaluate_v_advection (Q, ll, level, l);
                        const Real datum = s + v;
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_NV_ADVECTION:
                     {
                        const Real n = evaluate_n_advection (Q, azimuth, ll, level, l);
                        const Real v = evaluate_v_advection (Q, ll, level, l);
                        const Real datum = n + v;
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_TENDENCY:
                     {
                        size_t i, j;
                        const Real datum = evaluate_s_tendency (
                           THETA, azimuth, lat_long, level, l, u_bg);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_ADVECTION:
                     {
                        const Real h = evaluate_h_advection (THETA, ll, level, l);
                        const Real v = evaluate_v_advection (THETA, ll, level, l);
                        const Real datum = h + v;
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_H_ADVECTION:
                     {
                        const Real datum = evaluate_h_advection (THETA, ll, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_V_ADVECTION:
                     {
                        const Real datum = evaluate_v_advection (THETA, ll, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_S_ADVECTION:
                     {
                        const Real datum = evaluate_s_advection (THETA, azimuth, ll, level, l, u_bg);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_N_ADVECTION:
                     {
                        const Real datum = evaluate_n_advection (THETA, azimuth, ll, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_SV_ADVECTION:
                     {
                        const Real s = evaluate_s_advection (THETA, azimuth, ll, level, l, u_bg);
                        const Real v = evaluate_v_advection (THETA, ll, level, l);
                        const Real datum = s + v;
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_NV_ADVECTION:
                     {
                        const Real n = evaluate_n_advection (THETA, azimuth, ll, level, l);
                        const Real v = evaluate_v_advection (THETA, ll, level, l);
                        const Real datum = n + v;
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     default:
                     {
                        const Met_Element met_element = product.get_met_element ();
                        const Real datum = evaluate (met_element, ll, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                  }

               }

               raster_ptr->set_pixel (i - index_2d.i, j - index_2d.j, color);

            }

         }

      }

   }

   return raster_ptr;

}

Scalar_Data_2D*
Model::Acncrjbf::get_cross_section_scalar_data_2d_ptr (const Box_2D& box_2d,
                                                       const Transform_2D& transform,
                                                       const Model::Product& product,
                                                       const Dtime& dtime,
                                                       const Journey& journey,
                                                       const Real u_bg) const
{

   const Index_2D& index_2d = box_2d.index_2d;
   const Size_2D& size_2d = box_2d.size_2d;
   const Real start_x = index_2d.i;
   const Real end_x = index_2d.i + size_2d.i;
   const Real start_y = index_2d.j;
   const Real end_y = index_2d.j + size_2d.j;
   const Domain_2D domain_2d (start_x, end_x, start_y, end_y);
   Scalar_Data_2D* scalar_data_2d_ptr = new Scalar_Data_2D (size_2d, domain_2d);
   Scalar_Data_2D& scalar_data_2d = *scalar_data_2d_ptr;

   Real datum;
   const size_t l = get_uppers_l (dtime);

   const bool is_speed = (product.enumeration == Model::Product::SPEED);
   const Model::Product& p = (is_speed ?
      Model::Product ("SPEED_HIGHER") : product);

   Real x, ceiling;
   Level level (Level::HEIGHT, GSL_NAN);
   Real& z = level.value;

   const Geodesy geodesy;
   const Real distance = journey.get_distance (geodesy);

   for (Integer i = index_2d.i; i < index_2d.i + size_2d.i; i++)
   {

      const Integer ii = i - index_2d.i;
      //transform.r (x, ceiling, Real (i), Real (index_2d.j));
      transform.r (x, ceiling, Real (i), Real (0));

      if (x < 0 || x > distance) { continue; }
      const Lat_Long& lat_long = journey.get_lat_long (x, geodesy);
      const Lat_Long& ll = lat_long;
      if (out_of_bounds (ll)) { continue; }

      const Real topography = get_topography (ll);
      const Real azimuth = journey.get_azimuth_forward (x, geodesy);

      switch (product.enumeration)
      {

         case Model::Product::SCORER:
         {

            const Sounding* sounding_ptr = get_sounding_ptr (
               lat_long, dtime, ceiling);
            const Real_Profile* profile_ptr =
               sounding_ptr->get_scorer_profile_ptr (azimuth, u_bg);

            for (Integer j = index_2d.j; j < index_2d.j + size_2d.j; j++)
            {

               const Integer jj = j - index_2d.j;
               transform.r (x, z, Real (i), Real (j));

               if (z < topography) { datum = GSL_NAN; }
               else
               {
                  const Real pressure = sounding_ptr->get_pressure (z);
                  datum = profile_ptr->get_ordinate (pressure);
               }

               scalar_data_2d.set_datum (ii, jj, datum);

            }

            delete profile_ptr;
            delete sounding_ptr;
            break;

         };

         case Model::Product::SCORER_A:
         {

            const Sounding* sounding_ptr = get_sounding_ptr (
               lat_long, dtime, ceiling);
            const Real_Profile* profile_ptr =
               sounding_ptr->get_scorer_a_profile_ptr (azimuth, u_bg);

            for (Integer j = index_2d.j; j < index_2d.j + size_2d.j; j++)
            {

               const Integer jj = j - index_2d.j;
               transform.r (x, z, Real (i), Real (j));

               if (z < topography) { datum = GSL_NAN; }
               else
               {
                  const Real pressure = sounding_ptr->get_pressure (z);
                  datum = profile_ptr->get_ordinate (pressure);
               }

               scalar_data_2d.set_datum (ii, jj, datum);

            }

            delete profile_ptr;
            delete sounding_ptr;
            break;

         };

         case Model::Product::SCORER_B:
         {

            const Sounding* sounding_ptr = get_sounding_ptr (
               lat_long, dtime, ceiling);
            const Real_Profile* profile_ptr =
               sounding_ptr->get_scorer_b_profile_ptr (azimuth, u_bg);

            for (Integer j = index_2d.j; j < index_2d.j + size_2d.j; j++)
            {

               const Integer jj = j - index_2d.j;
               transform.r (x, z, Real (i), Real (j));

               if (z < topography) { datum = GSL_NAN; }
               else
               {
                  const Real pressure = sounding_ptr->get_pressure (z);
                  datum = profile_ptr->get_ordinate (pressure);
               }

               scalar_data_2d.set_datum (ii, jj, datum);

            }

            delete profile_ptr;
            delete sounding_ptr;
            break;

         };

         default:
         {

            for (Integer j = index_2d.j; j < index_2d.j + size_2d.j; j++)
            {

               transform.r (x, z, Real (i), Real (j));

               if (z < topography) { datum = GSL_NAN; }
               else
               {

                  switch (product.enumeration)
                  {

                     case Model::Product::WIND:
                     {
                        const Real u = evaluate (U, ll, level, l);
                        const Real v = evaluate (V, ll, level, l);
                        const Wind wind (u, v);
                        datum = wind.get_speed ();
                        break;
                     }

                     case Model::Product::BRUNT_VAISALA:
                     {
                        datum = evaluate_brunt_vaisala (ll, level, l);
                        break;
                     }

                     case Model::Product::RICHARDSON:
                     {
                        datum = evaluate_richardson (azimuth, ll, level, l);
                        break;
                     }

                     case Model::Product::SCORER:
                     {
                        datum = evaluate_scorer (azimuth, ll, level, l, u_bg);
                        break;
                     }

                     case Model::Product::SCORER_A:
                     {
                        datum = evaluate_scorer_a (azimuth, ll, level, l, u_bg);
                        break;
                     }

                     case Model::Product::SCORER_B:
                     {
                        datum = evaluate_scorer_b (azimuth, ll, level, l, u_bg);
                        break;
                     }

                     case Model::Product::ALONG_SPEED:
                     {
                        datum = evaluate_along_speed (
                           azimuth, ll, level, l, u_bg);
                        break;
                     }

                     case Model::Product::NORMAL_SPEED:
                     {
                        datum = evaluate_normal_speed (
                           azimuth, lat_long, level, l);
                        break;
                     }

                     case Model::Product::Q_TENDENCY:
                     {
                        size_t i, j;
                        datum = evaluate_s_tendency (
                           Q, azimuth, lat_long, level, l, u_bg);
                        break;
                     }

                     case Model::Product::Q_ADVECTION:
                     {
                        const Real h = evaluate_h_advection (Q, ll, level, l);
                        const Real v = evaluate_v_advection (Q, ll, level, l);
                        datum = h + v;
                        break;
                     }

                     case Model::Product::Q_H_ADVECTION:
                     {
                        datum = evaluate_h_advection (Q, ll, level, l);
                        break;
                     }

                     case Model::Product::Q_V_ADVECTION:
                     {
                        datum = evaluate_v_advection (Q, ll, level, l);
                        break;
                     }

                     case Model::Product::Q_S_ADVECTION:
                     {
                        datum = evaluate_s_advection (Q, azimuth, ll, level, l, u_bg);
                        break;
                     }

                     case Model::Product::Q_N_ADVECTION:
                     {
                        datum = evaluate_n_advection (Q, azimuth, ll, level, l);
                        break;
                     }

                     case Model::Product::Q_SV_ADVECTION:
                     {
                        const Real s = evaluate_s_advection (Q, azimuth, ll, level, l, u_bg);
                        const Real v = evaluate_v_advection (Q, ll, level, l);
                        datum = s + v;
                        break;
                     }

                     case Model::Product::Q_NV_ADVECTION:
                     {
                        const Real n = evaluate_n_advection (Q, azimuth, ll, level, l);
                        const Real v = evaluate_v_advection (Q, ll, level, l);
                        datum = n + v;
                        break;
                     }

                     case Model::Product::THETA_TENDENCY:
                     {
                        datum = evaluate_s_tendency (
                           THETA, azimuth, lat_long, level, l, u_bg);
                        break;
                     }

                     case Model::Product::THETA_ADVECTION:
                     {
                        const Real h = evaluate_h_advection (THETA, ll, level, l);
                        const Real v = evaluate_v_advection (THETA, ll, level, l);
                        datum = h + v;
                        break;
                     }

                     case Model::Product::THETA_H_ADVECTION:
                     {
                        datum = evaluate_h_advection (THETA, ll, level, l);
                        break;
                     }

                     case Model::Product::THETA_V_ADVECTION:
                     {
                        datum = evaluate_v_advection (THETA, ll, level, l);
                        break;
                     }

                     case Model::Product::THETA_S_ADVECTION:
                     {
                        datum = evaluate_s_advection (THETA, azimuth, ll, level, l, u_bg);
                        break;
                     }

                     case Model::Product::THETA_N_ADVECTION:
                     {
                        datum = evaluate_n_advection (THETA, azimuth, ll, level, l);
                        break;
                     }

                     case Model::Product::THETA_SV_ADVECTION:
                     {
                        const Real s = evaluate_s_advection (THETA, azimuth, ll, level, l, u_bg);
                        const Real v = evaluate_v_advection (THETA, ll, level, l);
                        datum = s + v;
                        break;
                     }

                     case Model::Product::THETA_NV_ADVECTION:
                     {
                        const Real n = evaluate_n_advection (THETA, azimuth, ll, level, l);
                        const Real v = evaluate_v_advection (THETA, ll, level, l);
                        datum = n + v;
                        break;
                     }

                     default:
                     {
                        const Met_Element met_element = product.get_met_element ();
                        datum = evaluate (met_element, ll, level, l);
                        break;
                     }

                  }

               }

               scalar_data_2d.set_datum (i - index_2d.i, j - index_2d.j, datum);

            }

         }

      }

   }

   return scalar_data_2d_ptr;

}

Raster*
Model::Acncrjbf::get_time_cross_raster_ptr (const Box_2D& box_2d,
                                            const Transform_2D& transform,
                                            const Model::Product& product,
                                            const Location& location,
                                            const Dtime::Span& time_span,
                                            const Real azimuth) const
{

   Raster* raster_ptr = new Raster (box_2d);

   Color color;

   const bool is_speed = (product.enumeration == Model::Product::SPEED);
   const Model::Product& p = (is_speed ?
      Model::Product ("SPEED_HIGHER") : product);

   Real t, ceiling;
   Level level (Level::HEIGHT, GSL_NAN);
   Real& z = level.value;

   const Index_2D& index_2d = box_2d.index_2d;
   const Size_2D& size_2d = box_2d.size_2d;

   const Dtime& start_time = time_span.get_start ();
   const Dtime& end_time = time_span.get_end ();

   for (Integer ii = index_2d.i; ii < index_2d.i + size_2d.i; ii++)
   {

      const Integer iii = ii - index_2d.i;
      //transform.r (t, ceiling, Real (ii), Real (index_2d.j));
      transform.r (t, ceiling, Real (ii), Real (0));

      Dtime dtime (t);
      if (dtime < start_time || dtime > end_time) { continue; }

      const size_t l = get_uppers_l (dtime);

      const Lat_Long& lat_long = location;
      const Lat_Long& ll = lat_long;
      if (out_of_bounds (ll)) { continue; }

      size_t i, j;
      acquire_ij (i, j, lat_long);

      const Real topography = get_topography (i, j);

      switch (product.enumeration)
      {

         // Scorer, Scorer_A and Scorer_B needs special treatment
         // because of greater accuracies needed in vertical differentiation

         case Model::Product::SCORER:
         {

            const Sounding* sounding_ptr = get_sounding_ptr (
               lat_long, dtime, ceiling);
            const Real_Profile* profile_ptr =
               sounding_ptr->get_scorer_profile_ptr (azimuth);

            for (Integer jj = index_2d.j; jj < index_2d.j + size_2d.j; jj++)
            {

               const Integer jjj = jj - index_2d.j;
               transform.r (t, z, Real (ii), Real (jj));

               if (z < topography) { color = Color::black (); }
               else
               {
                  const Real pressure = sounding_ptr->get_pressure (z);
                  const Real datum = profile_ptr->get_ordinate (pressure);
                  color = Model::Stage::get_color (p, datum);
               }

               raster_ptr->set_pixel (iii, jjj, color);

            }

            delete profile_ptr;
            delete sounding_ptr;
            break;

         }

         case Model::Product::SCORER_A:
         {

            const Sounding* sounding_ptr = get_sounding_ptr (
               lat_long, dtime, ceiling);
            const Real_Profile* profile_ptr =
               sounding_ptr->get_scorer_a_profile_ptr (azimuth);

            for (Integer jj = index_2d.j; jj < index_2d.j + size_2d.j; jj++)
            {

               const Integer jjj = jj - index_2d.j;
               transform.r (t, z, Real (ii), Real (jj));

               if (z < topography) { color = Color::black (); }
               else
               {
                  const Real pressure = sounding_ptr->get_pressure (z);
                  const Real datum = profile_ptr->get_ordinate (pressure);
                  color = Model::Stage::get_color (p, datum);
               }

               raster_ptr->set_pixel (iii, jjj, color);

            }

            delete profile_ptr;
            delete sounding_ptr;
            break;

         }

         case Model::Product::SCORER_B:
         {

            const Sounding* sounding_ptr = get_sounding_ptr (
               lat_long, dtime, ceiling);
            const Real_Profile* profile_ptr =
               sounding_ptr->get_scorer_b_profile_ptr (azimuth);

            for (Integer jj = index_2d.j; jj < index_2d.j + size_2d.j; jj++)
            {

               const Integer jjj = jj - index_2d.j;
               transform.r (t, z, Real (ii), Real (jj));

               if (z < topography) { color = Color::black (); }
               else
               {
                  const Real pressure = sounding_ptr->get_pressure (z);
                  const Real datum = profile_ptr->get_ordinate (pressure);
                  color = Model::Stage::get_color (p, datum);
               }

               raster_ptr->set_pixel (iii, jjj, color);

            }

            delete profile_ptr;
            delete sounding_ptr;
            break;

         }

         default:
         {

            for (Integer jj = index_2d.j; jj < index_2d.j + size_2d.j; jj++)
            {

               const Integer jjj = jj - index_2d.j;
               transform.r (t, z, Real (ii), Real (jj));

               if (z < topography) { color = Color::black (); }
               else
               {

                  switch (product.enumeration)
                  {

                     // Should not reach here: should be processed earlier with vertical profile
                     case Model::Product::SCORER:
                     {
                        const Real datum = evaluate_scorer (
                           azimuth, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     // Should not reach here: should be processed earlier with vertical profile
                     case Model::Product::SCORER_A:
                     {
                        const Real datum = evaluate_scorer_a ( azimuth, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     // Should not reach here: should be processed earlier with vertical profile
                     case Model::Product::SCORER_B:
                     {
                        const Real datum = evaluate_scorer_b (
                           azimuth, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::WIND:
                     {
                        const Real u = evaluate (U, i, j, level, l);
                        const Real v = evaluate (V, i, j, level, l);
                        const Wind wind (u, v);
                        const Real direction = wind.get_direction ();
                        const Real speed = wind.get_speed ();
                        color = get_wind_color (direction, speed);
                        break;
                     }

                     case Model::Product::BRUNT_VAISALA:
                     {
                        const Real datum = evaluate_brunt_vaisala (i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::RICHARDSON:
                     {
                        const Real datum = evaluate_richardson (
                           azimuth, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::ALONG_SPEED:
                     {
                        const Real datum = evaluate_along_speed (
                           azimuth, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::NORMAL_SPEED:
                     {
                        const Real datum = evaluate_normal_speed (
                           azimuth, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_TENDENCY:
                     {
                        const Real datum = evaluate_s_tendency (
                           Q, azimuth, i, j, level, l, 0);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_ADVECTION:
                     {
                        const Real h = evaluate_h_advection (Q, i, j, level, l);
                        const Real v = evaluate_v_advection (Q, i, j, level, l);
                        const Real datum = h + v;
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_H_ADVECTION:
                     {
                        const Real datum = evaluate_h_advection (Q, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_V_ADVECTION:
                     {
                        const Real datum = evaluate_v_advection (Q, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_S_ADVECTION:
                     {
                        const Real datum = evaluate_s_advection (Q,
                           azimuth, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_N_ADVECTION:
                     {
                        const Real datum = evaluate_n_advection (Q,
                           azimuth, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_SV_ADVECTION:
                     {
                        const Real v = evaluate_v_advection (Q, i, j, level, l);
                        const Real s = evaluate_s_advection (Q,
                           azimuth, i, j, level, l);
                        const Real datum = s + v;
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::Q_NV_ADVECTION:
                     {
                        const Real v = evaluate_v_advection (Q, i, j, level, l);
                        const Real n = evaluate_n_advection (Q,
                           azimuth, i, j, level, l);
                        const Real datum = n + v;
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_TENDENCY:
                     {
                        const Real datum = evaluate_s_tendency (
                           THETA, azimuth, i, j, level, l, 0);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_ADVECTION:
                     {
                        const Real h = evaluate_h_advection (THETA, i, j, level, l);
                        const Real v = evaluate_v_advection (THETA, i, j, level, l);
                        const Real datum = h + v;
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_H_ADVECTION:
                     {
                        const Real datum = evaluate_h_advection (THETA, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_V_ADVECTION:
                     {
                        const Real datum = evaluate_v_advection (THETA, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_S_ADVECTION:
                     {
                        const Real datum = evaluate_s_advection (THETA,
                           azimuth, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_N_ADVECTION:
                     {
                        const Real datum = evaluate_n_advection (THETA,
                           azimuth, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_SV_ADVECTION:
                     {
                        const Real v = evaluate_v_advection (THETA, i, j, level, l);
                        const Real s = evaluate_s_advection (THETA,
                           azimuth, i, j, level, l);
                        const Real datum = s + v;
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     case Model::Product::THETA_NV_ADVECTION:
                     {
                        const Real v = evaluate_v_advection (THETA, i, j, level, l);
                        const Real n = evaluate_n_advection (THETA,
                           azimuth, i, j, level, l);
                        const Real datum = n + v;
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                     default:
                     {
                        const Met_Element met_element = product.get_met_element ();
                        const Real datum = evaluate (met_element, i, j, level, l);
                        color = Model::Stage::get_color (p, datum);
                        break;
                     }

                  }

               }

               raster_ptr->set_pixel (iii, jjj, color);

            }

         }

      }

   }

   return raster_ptr;

}
Raster*
Model::Acncrjbf::get_time_cross_raster_ptr (const Box_2D& box_2d,
                                            const Transform_2D& transform,
                                            const Model::Product& product,
                                            const Track& track,
                                            const bool eulerian) const
{

   Raster* raster_ptr = new Raster (box_2d);

   Color color;

   const bool is_speed = (product.enumeration == Model::Product::SPEED);
   const Model::Product& p = (is_speed ?
      Model::Product ("SPEED_HIGHER") : product);

   Real t;
   Level level (Level::HEIGHT, GSL_NAN);
   Real& z = level.value;

   const Index_2D& index_2d = box_2d.index_2d;
   const Size_2D& size_2d = box_2d.size_2d;

   const Dtime& start_time = track.get_start_time ();
   const Dtime& end_time = track.get_end_time ();

   for (Integer ii = index_2d.i; ii < index_2d.i + size_2d.i; ii++)
   {

      transform.r (t, z, Real (ii), Real (index_2d.j));
      Dtime dtime (t);
      if (dtime < start_time || dtime > end_time) { continue; }

      // The following is necessary: trace lat_long will drift with time
      const size_t l = get_uppers_l (dtime);
      dtime = valid_uppers_time_vector[l];
      if (dtime < start_time || dtime > end_time) { continue; }

      const Lat_Long& lat_long = track.get_lat_long (dtime);
      const Lat_Long& ll = lat_long;
      if (out_of_bounds (ll)) { continue; }

      size_t i, j;
      acquire_ij (i, j, lat_long);

      const Real topography = get_topography (i, j);
      const Motion& motion = track.get_motion (dtime);

      for (Integer jj = index_2d.j; jj < index_2d.j + size_2d.j; jj++)
      {

         transform.r (t, z, Real (ii), Real (jj));

         if (z < topography) { color = Color::black (); }
         else
         {

            switch (product.enumeration)
            {

               case Model::Product::WIND:
               {
                  const Real u = evaluate (U, i, j, level, l);
                  const Real v = evaluate (V, i, j, level, l);
                  const Wind wind (u, v);
                  const Real direction = wind.get_direction ();
                  const Real speed = wind.get_speed ();
                  color = get_wind_color (direction, speed);
                  break;
               }

               case Model::Product::BRUNT_VAISALA:
               {
                  const Real datum = evaluate_brunt_vaisala (i, j, level, l);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::RICHARDSON:
               {
                  const Real u_bg = (eulerian ? 0 : motion.get_speed ());
                  const Real datum = evaluate_richardson (
                     motion.get_direction (), i, j, level, l);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::SCORER:
               {
                  const Real u_bg = (eulerian ? 0 : motion.get_speed ());
                  const Real datum = evaluate_scorer (
                     motion.get_direction (), i, j, level, l, u_bg);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::SCORER_A:
               {
                  const Real u_bg = (eulerian ? 0 : motion.get_speed ());
                  const Real datum = evaluate_scorer_a (
                     motion.get_direction (), i, j, level, l, u_bg);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::SCORER_B:
               {
                  const Real u_bg = (eulerian ? 0 : motion.get_speed ());
                  const Real datum = evaluate_scorer_b (
                     motion.get_direction (), i, j, level, l, u_bg);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::ALONG_SPEED:
               {
                  const Real u_bg = (eulerian ? 0 : motion.get_speed ());
                  const Real datum = evaluate_along_speed (
                     motion.get_direction (), i, j, level, l, u_bg);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::NORMAL_SPEED:
               {
                  const Real datum = evaluate_normal_speed (
                     motion.get_direction (), i, j, level, l);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::Q_TENDENCY:
               {
                  const Real u_bg = (eulerian ? 0 : motion.get_speed ());
                  const Real datum = evaluate_s_tendency (
                     Q, motion.get_direction (), i, j, level, l, u_bg);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::Q_ADVECTION:
               {
                  const Real h = evaluate_h_advection (Q, i, j, level, l);
                  const Real v = evaluate_v_advection (Q, i, j, level, l);
                  const Real datum = h + v;
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::Q_H_ADVECTION:
               {
                  const Real datum = evaluate_h_advection (Q, i, j, level, l);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::Q_V_ADVECTION:
               {
                  const Real datum = evaluate_v_advection (Q, i, j, level, l);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::Q_S_ADVECTION:
               {
                  const Real u_bg = (eulerian ? 0 : motion.get_speed ());
                  const Real datum = evaluate_s_advection (Q,
                     motion.get_direction (), i, j, level, l, u_bg);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::Q_N_ADVECTION:
               {
                  const Real datum = evaluate_n_advection (Q,
                     motion.get_direction (), i, j, level, l);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::Q_SV_ADVECTION:
               {
                  const Real u_bg = (eulerian ? 0 : motion.get_speed ());
                  const Real v = evaluate_v_advection (Q, i, j, level, l);
                  const Real s = evaluate_s_advection (Q,
                     motion.get_direction (), i, j, level, l, u_bg);
                  const Real datum = s + v;
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::Q_NV_ADVECTION:
               {
                  const Real v = evaluate_v_advection (Q, i, j, level, l);
                  const Real n = evaluate_n_advection (Q,
                     motion.get_direction (), i, j, level, l);
                  const Real datum = n + v;
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::THETA_TENDENCY:
               {
                  const Real u_bg = (eulerian ? 0 : motion.get_speed ());
                  const Real datum = evaluate_s_tendency (
                     THETA, motion.get_direction (), i, j, level, l, u_bg);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::THETA_ADVECTION:
               {
                  const Real h = evaluate_h_advection (THETA, i, j, level, l);
                  const Real v = evaluate_v_advection (THETA, i, j, level, l);
                  const Real datum = h + v;
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::THETA_H_ADVECTION:
               {
                  const Real datum = evaluate_h_advection (THETA, i, j, level, l);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::THETA_V_ADVECTION:
               {
                  const Real datum = evaluate_v_advection (THETA, i, j, level, l);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::THETA_S_ADVECTION:
               {
                  const Real u_bg = (eulerian ? 0 : motion.get_speed ());
                  const Real datum = evaluate_s_advection (THETA,
                     motion.get_direction (), i, j, level, l, u_bg);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::THETA_N_ADVECTION:
               {
                  const Real datum = evaluate_n_advection (THETA,
                     motion.get_direction (), i, j, level, l);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::THETA_SV_ADVECTION:
               {
                  const Real u_bg = (eulerian ? 0 : motion.get_speed ());
                  const Real v = evaluate_v_advection (THETA, i, j, level, l);
                  const Real s = evaluate_s_advection (THETA,
                     motion.get_direction (), i, j, level, l, u_bg);
                  const Real datum = s + v;
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               case Model::Product::THETA_NV_ADVECTION:
               {
                  const Real v = evaluate_v_advection (THETA, i, j, level, l);
                  const Real n = evaluate_n_advection (THETA,
                     motion.get_direction (), i, j, level, l);
                  const Real datum = n + v;
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

               default:
               {
                  const Met_Element met_element = product.get_met_element ();
                  const Real datum = evaluate (met_element, i, j, level, l);
                  color = Model::Stage::get_color (p, datum);
                  break;
               }

            }

         }

         raster_ptr->set_pixel (ii - index_2d.i, jj - index_2d.j, color);

      }

   }

   return raster_ptr;

}

size_t
Model::Acncrjbf::get_surface_l (const Dtime& dtime) const
{

   Real min_dt = GSL_POSINF;
   auto min_iterator = valid_surface_time_set.end ();

   for (auto iterator = valid_surface_time_set.begin ();
         iterator != valid_surface_time_set.end (); iterator++)
   {
      const Real dt = fabs (dtime.t - iterator->t);
      if (dt < min_dt)
      {
         min_dt = dt;
         min_iterator = iterator;
      }
   }

   return std::distance (valid_surface_time_set.begin (), min_iterator);

}

size_t
Model::Acncrjbf::get_uppers_l (const Dtime& dtime) const
{

   Real min_dt = GSL_POSINF;
   auto min_iterator = valid_uppers_time_set.end ();

   for (auto iterator = valid_uppers_time_set.begin ();
         iterator != valid_uppers_time_set.end (); iterator++)
   {
      const Real dt = fabs (dtime.t - iterator->t);
      if (dt < min_dt)
      {
         min_dt = dt;
         min_iterator = iterator;
      }
   }

   return std::distance (valid_uppers_time_set.begin (), min_iterator);

}

Track
Model::Acncrjbf::get_trajectory (Lat_Long lat_long,
                                 Level level,
                                 Dtime dtime,
                                 const Real finish_tau,
                                 const vector<Product>& product_vector) const
{

   Track trajectory (dtime);

   while (true)
   {

      if (out_of_bounds (lat_long)) { break; }
      const Real topography = get_topography (lat_long);

      if (level.type == Level::SURFACE) { level.value = topography; }
      if (level.type == Level::MAGL)
      {
         level.value += topography;
         level.type = Level::HEIGHT;
      }

      const Dtime& epoch = trajectory.get_epoch ();
      const Real tau = dtime.t - epoch.t;

      trajectory.add (tau, lat_long);
      trajectory.add ("z", tau, level.value);

      for (auto iterator = product_vector.begin ();
           iterator != product_vector.end (); iterator++)
      {
         const Product& product = *(iterator);
         const Met_Element& met_element = product.get_met_element ();
         const Real datum = evaluate (met_element, lat_long, level, dtime);
         trajectory.add (product.get_string (), tau, datum);
      }

      if (fabs (tau) > fabs (finish_tau)) { break; }

      if (level.type == Level::HEIGHT)
      {
         if (level.value > 38000) { break; }
         if (level.value <= topography)
         {
            level.value = topography;
            level.type = Level::SURFACE;
         }
      }

      const Real u = evaluate (U, lat_long, level, dtime);
      const Real v = evaluate (V, lat_long, level, dtime);

      const auto grid_size = get_grid_size (lat_long);
      const Real h_lat = grid_size.first;
      const Real h_long = grid_size.second;

      const Real sign = (finish_tau > 0 ? 1 : -1);
      const Real dts_x = fabs (0.45 * h_long * LATITUDE_LENGTH / u);
      const Real dts_y = fabs (0.45 * h_lat * LATITUDE_LENGTH / v);
      const Real dts = std::max (1.0, std::min (dts_x, dts_y)) * sign;
      const Real dt = dts / 3600;

      typedef Geodesy G;
      const Wind wind (u, v);
      const Real distance_ = wind.get_speed () * dts;
      const Real azimuth_ = wind.get_direction () + 180;
      const Lat_Long& ll_ = G::get_destination (lat_long, distance_, azimuth_);
      if (out_of_bounds (ll_)) { break; }

      const Real topography_ = get_topography (ll_);
      Level level_ = level;
      Real w = GSL_NAN;

      if (level.type == Level::HEIGHT)
      {
         w = evaluate (W, lat_long, level, dtime); 
         level_.value = level.value + w * dts;
         if (level_.value_ > 38000) { break; }
         if (level_.value <= topography_)
         {
            level.value = topography_;
            level_.value = topography_;
            level.type = Level::SURFACE;
            level_.type = Level::SURFACE;
         }
      }

      const Dtime next_dtime (dtime.t + dt, false);
      const Real u_ = evaluate (U, ll_, level_, next_dtime);
      const Real v_ = evaluate (V, ll_, level_, next_dtime);
      const Wind wind_ (u_, v_);
      const Wind wind_a ((u + u_) * 0.5, (v + v_) * 0.5);

      const Real distance_a = wind_a.get_speed () * dts;
      const Real azimuth_a = wind_a.get_direction () + 180;
      lat_long = G::get_destination (lat_long, distance_a, azimuth_a);

      if (level_.type != Level::SURFACE)
      {
         const Real w_ = evaluate (W, ll_, level_, next_dtime);
         level.value = (level.value + (w + w_) * 0.5 * dts);
      }

      dtime.t = next_dtime.t;

   }

   trajectory.okay ();
   return trajectory;

}

void
Model::Acncrjbf::survey_trajectory (Track& trajectory,
                                    const vector<Product>& product_vector) const
{

   const Level surface ("Surface");
   const Dtime& epoch = trajectory.get_epoch ();
   const set<Real>& tau_set = trajectory.get_tau_set ();

   for (auto p = product_vector.begin (); p != product_vector.end (); p++)
   {

      const Product& product = *(p);

      for (auto t = tau_set.begin (); t != tau_set.end (); t++)
      {

         const Real& tau = *(t);
         const Dtime dtime (epoch.t + tau);
         const Lat_Long& lat_long = trajectory.get_lat_long (tau);
         if (out_of_bounds (lat_long)) { break; }

         const Real topography = get_topography (lat_long);
         const Real z = trajectory.get_datum ("z", tau);

         const bool is_surface = (z <= topography);
         const Level& level = is_surface ? surface : Level::z_level (z);

         Real datum;

         switch (product.enumeration)
         {

            case Product::Q_TENDENCY:
            {
               if (is_surface)
               {
                  const Integer l = get_surface_l (dtime);
                  datum = evaluate_tendency (Q, lat_long, l);
               }
               else
               {
                  const Integer l = get_uppers_l (dtime);
                  datum = evaluate_tendency (Q, lat_long, level, l);
               }
               break;
            }

            case Product::Q_H_ADVECTION:
            {
               if (is_surface)
               {
                  const Integer l = get_surface_l (dtime);
                  datum = evaluate_h_advection (Q, lat_long, l);
               }
               else
               {
                  const Integer l = get_uppers_l (dtime);
                  datum = evaluate_h_advection (Q, lat_long, level, l);
               }
               break;
            }

            case Product::Q_V_ADVECTION:
            {
               if (is_surface) { datum = 0; }
               else
               {
                  const Integer l = get_uppers_l (dtime);
                  datum = evaluate_v_advection (Q, lat_long, level, l);
               }
               break;
            }

            case Product::THETA_TENDENCY:
            {
               if (is_surface)
               {
                  const Integer l = get_surface_l (dtime);
                  datum = evaluate_tendency (THETA, lat_long, l);
               }
               else
               {
                  const Integer l = get_uppers_l (dtime);
                  datum = evaluate_tendency (THETA, lat_long, level, l);
               }
               break;
            }

            case Product::THETA_H_ADVECTION:
            {
               if (is_surface)
               {
                  const Integer l = get_surface_l (dtime);
                  datum = evaluate_h_advection (THETA, lat_long, l);
               }
               else
               {
                  const Integer l = get_uppers_l (dtime);
                  datum = evaluate_h_advection (THETA, lat_long, level, l);
               }
               break;
            }

            case Product::THETA_V_ADVECTION:
            {
               if (is_surface) { datum = 0; }
               else
               {
                  const Integer l = get_uppers_l (dtime);
                  datum = evaluate_v_advection (THETA, lat_long, level, l);
               }
               break;
            }

            default:
            {
               const Met_Element& met_element = product.get_met_element ();
               datum = evaluate (met_element, lat_long, level, dtime);
               break;
            }

         }

         trajectory.add (product.get_string (), tau, datum);

      }

      trajectory.okay (product.get_string ());

   }

}

Aws::Obs
Model::Acncrjbf::get_aws_obs (const Lat_Long& lat_long,
                              const Dtime& dtime) const
{

   size_t i, j;
   acquire_ij (i, j, lat_long);
   const Integer l = get_surface_l (dtime);

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

   return Aws::Obs (t, t_d, wind_direction, wind_speed,
      wind_speed, surface_p, mslp);   

}

const Aws::Repository*
Model::Acncrjbf::get_aws_repository_ptr (const Lat_Long& lat_long,
                                         const Dtime::Set& time_set) const
{

   Aws::Repository* aws_repository_ptr = new Aws::Repository ();
   const auto& valid_time_set = get_valid_surface_time_set ();

   for (auto iterator = valid_time_set.begin ();
        iterator != valid_time_set.end (); iterator++)
   {

      const Dtime& dtime = *(iterator);
      if (!time_set.match (dtime)) { continue; }

      const Aws::Key key (-1, dtime);
      const Aws::Obs& obs = get_aws_obs (lat_long, dtime);
      aws_repository_ptr->insert (key, obs);

   }

   return aws_repository_ptr;

}

Sounding*
Model::Acncrjbf::get_sounding_ptr (const Lat_Long& lat_long,
                                   const Dtime& dtime,
                                   const Real ceiling) const
{

   Sounding* sounding_ptr = new Sounding ();

   sounding_ptr->set_time (dtime);
   sounding_ptr->set_basetime (basetime);
   sounding_ptr->set_location_str (lat_long.get_string (4));
   fill_sounding (*sounding_ptr, lat_long, dtime, ceiling);

   return sounding_ptr;

}

Sounding*
Model::Acncrjbf::get_sounding_ptr (const Lat_Long::List& lat_long_list,
                                   const Dtime& dtime,
                                   const Real ceiling,
                                   const Thermo_Diagram& thermo_diagram) const
{

   const bool is_singular = (lat_long_list.size () == 1);
   if (is_singular)
   {
      const Lat_Long& lat_long = lat_long_list.front ();
      return get_sounding_ptr (lat_long, dtime, ceiling);
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
         const Sounding* sounding_ptr = get_sounding_ptr (ll, dtime, ceiling);
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

Tokens
Model::Acncrjbf::get_marker_tokens (const Lat_Long& lat_long,
                                    const Dtime& dtime,
                                    const Product& product,
                                    const Level& level) const
{
   Tokens tokens;

   switch (product.enumeration)
   {

      case Product::P_THETA:
      {
         const Real datum = evaluate (P_THETA, lat_long, level, dtime);
         if (gsl_isnan (datum)) { return tokens; }
         tokens.push_back (Dstring::render ("%.0fhPa", datum * 1e-2));
         break;
      }

      case Product::P_RHO:
      {
         const Real datum = evaluate (P_RHO, lat_long, level, dtime);
         if (gsl_isnan (datum)) { return tokens; }
         tokens.push_back (Dstring::render ("%.1fhPa", datum * 1e-2));
         break;
      }

      case Product::THETA:
      case Product::THETA_MONOCHROME:
      case Product::THETA_28C:
      {
         const Real datum = evaluate (THETA, lat_long, level, dtime);
         if (gsl_isnan (datum)) { return tokens; }
         tokens.push_back (Dstring::render ("%.1f\u00b0C", datum - K));
         break;
      }

      case Product::THETA_V:
      {
         const Real theta_e = evaluate (THETA_V, lat_long, level, dtime);
         if (gsl_isnan (theta_e)) { return tokens; }
         tokens.push_back (Dstring::render ("%.1f\u00b0C", theta_e - K));
         break;
      }

      case Product::Q:
      case Product::Q_MONOCHROME:
      case Product::Q_3GKG:
      {
         const Real datum = evaluate (Q, lat_long, level, dtime);
         if (gsl_isnan (datum)) { return tokens; }
         tokens.push_back (Dstring::render ("%.3fg/kg", datum * 1e3));
         break;
      }

      case Product::T:
      {
         const Real datum = evaluate (T, lat_long, level, dtime);
         if (gsl_isnan (datum)) { return tokens; }
         tokens.push_back (Dstring::render ("%.1f\u00b0C", datum - K));
         break;
      }

      case Product::TD:
      {
         const Real datum = evaluate (TD, lat_long, level, dtime);
         if (gsl_isnan (datum)) { return tokens; }
         tokens.push_back (Dstring::render ("%.0f%%", datum - K));
         break;
      }

      case Product::RH:
      {
         const Real datum = evaluate (TD, lat_long, level, dtime);
         if (gsl_isnan (datum)) { return tokens; }
         tokens.push_back (Dstring::render ("%.0f%%", datum * 100));
         break;
      }

      case Product::THETA_E:
      {
         const Real theta_e = evaluate (THETA_E, lat_long, level, dtime);
         if (gsl_isnan (theta_e)) { return tokens; }
         tokens.push_back (Dstring::render ("%.1f\u00b0C", theta_e - K));
         break;
      }

      case Product::RHO:
      {
         const Real rho = evaluate (RHO, lat_long, level, dtime);
         if (gsl_isnan (rho)) { return tokens; }
         tokens.push_back (Dstring::render ("%.2fkg/m3", rho));
         break;
      }

      case Product::WIND:
      case Product::SPEED:
      {
         const Real u = evaluate (U, lat_long, level, dtime);
         if (gsl_isnan (u)) { return tokens; }
         const Real v = evaluate (V, lat_long, level, dtime);
         if (gsl_isnan (v)) { return tokens; }
         const Wind wind (u, v);
         const Real msknot = 3.6/1.852;
         const Dstring fmt ("%03.0f\u00b0 / %02.1fkt");
         tokens.push_back (wind.get_string (msknot, fmt));
         break;
      }

      case Product::FFDI:
      {
         const Real ffdi = evaluate (FFDI, lat_long, level, dtime);
         if (gsl_isnan (ffdi)) { return tokens; }
         Dstring ffdr ("Low-Moderate");
              if (ffdi > 12) { ffdr = "High"; }
         else if (ffdi > 25) { ffdr = "Very High"; }
         else if (ffdi > 50) { ffdr = "Severe"; }
         else if (ffdi > 75) { ffdr = "Extreme"; }
         else if (ffdi > 100) { ffdr = "Catastrophic"; }
         tokens.push_back (Dstring::render ("%02.2f", ffdi));
         tokens.push_back (ffdr);
         break;
      }

      case Product::TERRAIN:
      case Product::TERRAIN_WATER:
      {
         const Real orog = evaluate (Dstring ("orog"), lat_long);
         if (gsl_isnan (orog)) { return tokens; }
         tokens.push_back (Dstring::render ("%.2fm", orog));
         break;
      }

      case Product::W:
      {
         const Real w = evaluate (W, lat_long, level, dtime);
         if (gsl_isnan (w)) { return tokens; }
         tokens.push_back (Dstring::render ("%.2fm/s", w));
         break;
      }

      case Product::VORTICITY:
      {
         const Real zeta = evaluate (ZETA, lat_long, level, dtime);
         if (gsl_isnan (zeta)) { return tokens; }
         tokens.push_back (Dstring::render ("%.4e/s", zeta));
         break;
      }

      case Product::MSLP:
      {
         const Real mslp = evaluate (MSLP, lat_long, level, dtime);
         if (gsl_isnan (mslp)) { return tokens; }
         tokens.push_back (Dstring::render ("%0.1fhPa", mslp * 1e-2));
         break;
      }

      case Product::PRECIP_RATE:
      {
         const Real mmhr = evaluate (PRECIP_RATE, lat_long, level, dtime);
         if (gsl_isnan (mmhr)) { return tokens; }
         tokens.push_back (Dstring::render ("%0.1fmm/hr", mmhr * 3600));
         break;
      }

   }

   return tokens;

}

Model::Model (const Rc_File& rc_file)
   : stage_map (*this, rc_file)
{

   for (auto iterator = rc_file.begin ();
        iterator != rc_file.end (); iterator++)
   {

      const Tokens tokens (*(iterator), " \f\n\t");
      if (tokens.size () != 2 || tokens[0] != "model") { continue; }

      const Dstring& argument = tokens[1];
      const Tokens argument_tokens (argument, ":");
      const Dstring& model_identifier = argument_tokens[0];
      const Integer n = argument_tokens.size ();

   }

}

Model::~Model ()
{
}

const Model::Stage&
Model::get_stage (const Dstring& stage) const
{
   return *(stage_map.at (stage));
}

