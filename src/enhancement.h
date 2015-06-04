#ifndef ENHANCEMENT_H
#define ENHANCEMENT_H

#include <fstream>
#include <denise/util.h>
#include <denise/geodesy.h>

using namespace std;
using namespace denise;

class Enhancement
{

   private:

      class Segment
      {

         public:

            uint16_t
            index_a;

            uint16_t
            index_b;

            Color
            color_a;

            Color
            color_b;

            Segment (const uint16_t index_a,
                     const uint16_t index_b,
                     const Color& color_a,
                     const Color& color_b);

            bool
            contains (const uint16_t datum) const;

            virtual Color
            get_color (const uint16_t datum) const;

      };

      vector<Segment>
      segment_vector;

   public:

      void
      add (const uint16_t index_a,
           const uint16_t index_b,
           const Color& color_a,
           const Color& color_b);

      Color
      get_color (const uint16_t datum) const;

      static Enhancement
      get_enhancement (const string& str);

};

class Enhancement_Plain : public Enhancement
{

   public:

      Enhancement_Plain ();

};

class Enhancement_Gamma : public Enhancement
{

   public:

      Enhancement_Gamma (const Real gamma,
                         const Integer n = 20);

};

class Enhancement_Wvjl3 : public Enhancement
{

   public:

      Enhancement_Wvjl3 ();

};

#endif /* ENHANCEMENT_H */
