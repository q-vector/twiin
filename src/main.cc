#include <denise/dstring.h>
#include "display.h"
#include "data.h"

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

   const Tokens stage_tokens (stage_str, ":");
   const Tokens product_tokens (product_str, ":");

   const Stage stage (stage_tokens[0]);
   const Product product (product_tokens[0]);
   const Level level (level_str);

   try
   {

      const Size_2D size_2d (960, 960);
      const Point_2D centre (size_2d.i/2, size_2d.j/2);
      const Tokens& config_file_content = read_config_file (config_file_path);

      typedef Geodetic_Transform Gt;
      map<string, Transform_2D*> transform_ptr_map;

      for (auto iterator = zoom_tokens.begin ();
           iterator != zoom_tokens.end (); iterator++)
      {
         const Tokens tokens (*(iterator), "/");
         const string& identifier = tokens[0];
         const string& zoom_str = tokens[1];
         Geodetic_Transform* gt_ptr = Gt::get_transform_ptr (zoom_str, centre);
         transform_ptr_map.insert (make_pair (identifier, gt_ptr));
      }

      Title title (size_2d);
      const Model model (config_file_content);

      for (Tokens::const_iterator i = stage_tokens.begin ();
           i != stage_tokens.end (); i++)
      {

         const twiin::Stage stage (*i);
         const Transform_2D& transform = *(transform_ptr_map[stage]);

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


               const Dtime& dtime = *(k);
               Display::render (cr, transform, size_2d,
                  model, dtime, level, stage, product);
               const string& time_str = dtime.get_string ("%Y.%m.%d %H:%M UTC");
               title.set ("", time_str, product, stage, level.get_string ());
               title.cairo (cr);

               const string png_file_path = stage + "_" + product +
                  "_" + dtime.get_string ("%Y%m%d%H%M") + ".png";
               surface->write_to_png (png_file_path);
               cout << png_file_path << " done." << endl;

            }

         }

      }

      for (auto iterator = transform_ptr_map.begin ();
           iterator != transform_ptr_map.end (); iterator++)
      {
         Transform_2D* transform_ptr = iterator->second;
         delete transform_ptr;
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

