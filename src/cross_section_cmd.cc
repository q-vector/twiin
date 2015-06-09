#include <denise/dstring.h>
#include "display.h"

using namespace std;
using namespace denise;
using namespace Cairo;
using namespace twiin;

int
main (int argc,
      char** argv)
{

   const string zoom_str_3 ("LAMBERT_CONIC_SOUTH:3000:-33.5:150.5");
   const string zoom_str_4 ("LAMBERT_CONIC_SOUTH:1200:-33.75:150.5");
   const string zoom_str_5 ("LAMBERT_CONIC_SOUTH:380:-33.7:150.55");

   Tokens zoom_tokens;
   zoom_tokens.push_back ("STAGE3/" + zoom_str_3);
   zoom_tokens.push_back ("STAGE4/" + zoom_str_4);
   zoom_tokens.push_back ("STAGE5/" + zoom_str_5);

   const string config_file_path (argv[1]);
   const string product_str (argv[2]);
   const string stage_str (argv[3]);
   const string level_str (argv[4]);
   const Real origin_latitude = stof (argv[5]);
   const Real origin_longitude = stof (argv[6]);
   const Real destination_latitude = stof (argv[7]);
   const Real destination_longitude = stof (argv[8]);

   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");

   const Stage stage (stage_tokens[0]);
   const Product product (product_tokens[0]);
   const Level level (level_str);


   try
   {

      Multi_Journey multi_journey;
      const Lat_Long origin (origin_latitude, origin_longitude);
      const Lat_Long destination (destination_latitude, destination_longitude);
      multi_journey.push_back (origin);
      multi_journey.push_back (destination);
      const Geodesy geodesy;
      const Real distance = multi_journey.get_distance (geodesy);

      const Domain_1D domain_x (0, distance);
      const Domain_1D domain_z (0, 8000);
      const Domain_2D domain_2d (domain_x, domain_z);

      const Size_2D size_2d (960, 480);
      const Tokens& config_file_content = read_config_file (config_file_path);

      Title title (size_2d);
      const Model model (config_file_content);

      const Real margin_l = 50;
      const Real margin_r = 50;
      const Real margin_t = title.get_height () + 50;
      const Real margin_b = 50;
      const Real w = size_2d.i - margin_l - margin_r;
      const Real h = size_2d.j - margin_t - margin_b;

      Affine_Transform_2D transform;
      const Real span_x = domain_x.get_span ();
      const Real span_z = domain_z.get_span ();
      transform.scale (1, -1);
      transform.scale (w / span_x, h / span_z);
      transform.translate (0, size_2d.j);
      transform.translate (margin_l, -margin_b);

      for (Tokens::const_iterator i = stage_tokens.begin ();
           i != stage_tokens.end (); i++)
      {

         const twiin::Stage stage (*i);

         for (Tokens::const_iterator j = product_tokens.begin ();
              j != product_tokens.end (); j++)
         {

            const Product product (*j);
            const auto& valid_time_set = model.get_valid_time_set (
               product, stage, level);

            for (auto k = valid_time_set.begin ();
                 k != valid_time_set.end (); k++)
            {

               RefPtr<ImageSurface> surface = ImageSurface::create (
                  FORMAT_ARGB32, size_2d.i, size_2d.j);
               RefPtr<Context> cr = Context::create (surface);
               cr->select_font_face ("Verdana", FONT_SLANT_NORMAL,
                  FONT_WEIGHT_NORMAL);

               const Index_2D i2d (margin_l, margin_t);
               const Size_2D s2d (size_2d.i - margin_l - margin_r,
                                  size_2d.j - margin_t - margin_b);
               const Box_2D box_2d (i2d, s2d);

               if (s2d.i < 0 || s2d.j < 0) { continue; }

               const Dtime& dtime = *(k);
               Display::render_cross_section (cr, transform, box_2d,
                  domain_z, model, stage, product, dtime, multi_journey);
               const string& time_str = dtime.get_string ("%Y.%m.%d %H:%M UTC");
               title.set ("", time_str, product, stage, "");
               title.cairo (cr);

               const string png_file_path = "crosssection_" + stage + "_" +
                  product + "_" + dtime.get_string ("%Y%m%d%H%M") + ".png";
               surface->write_to_png (png_file_path);

            }
         }
      }

   }
   catch (const Exception& e)
   {
      cerr << "Exception " << e << endl;
   }
   catch (const std::exception& se)
   {
      cerr << "std::exception " << se.what () << endl;
   }

}

