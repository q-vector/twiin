#include "enhancement.h"

using namespace std;
using namespace denise;
using namespace Cairo;

Enhancement::Segment::Segment (const uint16_t index_a,
                               const uint16_t index_b,
                               const Color& color_a,
                               const Color& color_b)
   : index_a (index_a),
     index_b (index_b),
     color_a (color_a),
     color_b (color_b)
{
}

bool
Enhancement::Segment::contains (const uint16_t datum) const
{  
   return ((datum - index_a) * (datum - index_b) <= 0);
}

Color
Enhancement::Segment::get_color (const uint16_t datum) const
{

   const Real ar = color_a.r;
   const Real ag = color_a.g;
   const Real ab = color_a.b;
   const Real aa = color_a.a;
   const Real br = color_b.r;
   const Real bg = color_b.g;
   const Real bb = color_b.b;
   const Real ba = color_b.a;

   const Real d = Real (datum); 
   const Real t = (d - index_a) / (index_b - index_a);

   const Real r = t * (br - ar) + ar;
   const Real g = t * (bg - ag) + ag;
   const Real b = t * (bb - ab) + ab;
   const Real a = t * (ba - aa) + aa;

   return Color (r, g, b, a);
}

void
Enhancement::add (const uint16_t index_a,
                  const uint16_t index_b,
                  const Color& color_a,
                  const Color& color_b)
{  
   segment_vector.push_back (Segment (index_a, index_b, color_a, color_b));
}

Color
Enhancement::get_color (const uint16_t datum) const
{

   for (vector<Segment>::const_iterator iterator = segment_vector.begin ();
        iterator != segment_vector.end (); iterator++)
   {
      const Segment& segment = *(iterator);
      if (segment.contains (datum)) { return segment.get_color (datum); }
   }

   return Enhancement::get_color (datum);

}

Enhancement
Enhancement::get_enhancement (const string& str)
{

   string s (str);
   denise::to_upper_case (s);

   if (s == "BRIGHT") { return get_enhancement ("GAMMA 0.8"); }
   if (s == "BRIGHTER") { return get_enhancement ("GAMMA 0.6"); }
   if (s == "BRIGHTEST") { return get_enhancement ("GAMMA 0.4"); }

   if (s.substr (0, 5) == "GAMMA")
   {
      const Real gamma = atof (s.substr (6).c_str ());
      return Enhancement_Gamma (gamma);
   }

   if (s == "WVJL3") { return Enhancement_Wvjl3 (); }
   return Enhancement_Plain ();

}

Enhancement_Plain::Enhancement_Plain ()
{
   const Integer max = 1023;
   //const Integer max = 255;
   add (0, max, Color (0, 0, 0), Color (1, 1, 1));
}

Enhancement_Gamma::Enhancement_Gamma (const Real gamma,
                                      const Integer n)
{  

   const Integer max = 1023;
   //const Integer max = 255; 

   for (Integer i = 0; i < n; i++)
   {
      const Real x = Real (i) / Real (n);
      const Real xp1 = Real (i + 1) / Real (n);
      const Integer ii = Integer (round (x * max));
      const Integer iip1 = Integer (round (xp1 * max)) - 1;
      const Real g = pow (x, gamma);
      const Real gp1 = pow (xp1, gamma);
      add (ii, iip1, Color (g, g, g), Color (gp1, gp1, gp1));
   }
}

Enhancement_Wvjl3::Enhancement_Wvjl3 ()
{

   //const Integer max = 1023;
   const Integer max = 255;
   const Real f = Real (1) / Real (max);

   add (  0, 607, Color (  0*f,   0*f,   0*f), Color (  0*f,   0*f,   0*f));
   add (608, 787, Color (245*f, 140*f,  70*f), Color ( 73*f,  73*f,  73*f));
   add (788, 871, Color ( 74*f,  74*f,  74*f), Color (210*f, 210*f, 210*f));
   add (872, 943, Color (210*f, 210*f, 210*f), Color (  0*f, 130*f,  80*f));
   add (944, 975, Color (  0*f, 121*f, 121*f), Color (  0*f, 212*f, 212*f));
   add (976, 1011, Color (160*f, 150*f,   0*f), Color (250*f,  50*f,   0*f));
   add (1012, 1015, Color (150*f,   0*f, 220*f), Color (250*f,   0*f, 220*f));
   add (1016, 1023, Color (  0*f,   0*f, 226*f), Color (  0*f,   0*f, 134*f));

}

