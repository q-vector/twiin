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

   const string station_file_path (argv[1]);
   const string model_config_file_path (argv[2]);
   const string product_str (argv[3]);
   const string stage_str (argv[4]);
   const string level_str (argv[5]);

   try
   {

      const Tokens stage_tokens (stage_str, ":");
      const Tokens product_tokens (product_str, ":");

      const Size_2D size_2d (960, 960);
      const Point_2D centre (size_2d.i/2, size_2d.j/2);

      const string zoom_str_3 ("LAMBERT_CONIC_SOUTH:3000:-33.5:150.5");
      const string zoom_str_4 ("LAMBERT_CONIC_SOUTH:1200:-33.75:150.5");
      const string zoom_str_5 ("LAMBERT_CONIC_SOUTH:380:-33.7:150.55");

      typedef Geodetic_Transform Gt;
      map<string, Transform_2D*> transform_ptr_map;
      transform_ptr_map.insert (make_pair ("STAGE_3",
         Gt::get_transform_ptr (zoom_str_3, centre)));
      transform_ptr_map.insert (make_pair ("STAGE_4",
         Gt::get_transform_ptr (zoom_str_4, centre)));
      transform_ptr_map.insert (make_pair ("STAGE_5",
         Gt::get_transform_ptr (zoom_str_5, centre)));

      Title title (size_2d);
      const Display display (station_file_path, model_config_file_path);

      const Level level ("500m");
      const Model& model = display.get_model ();

      for (Tokens::const_iterator i = stage_tokens.begin ();
           i != stage_tokens.end (); i++)
      {

         const twiin::Stage stage (*i);
         const Model::Uppers::Stage& uppers_stage = model.uppers.get_uppers_stage (stage);
         const set<Dtime>& valid_time_set = uppers_stage.get_valid_time_set ();

         const Transform_2D& transform = *(transform_ptr_map[stage]);

         for (set<Dtime>::const_iterator j = valid_time_set.begin ();
              j != valid_time_set.end (); j++)
         {

            const Dtime& dtime = *(j);

            for (Tokens::const_iterator k = product_tokens.begin ();
                 k != product_tokens.end (); k++)
            {
               const Product product (*k);

               const string& time_string = dtime.get_string ("%Y%m%d%H%M");
               const string png_file_path = stage + "_" +
                  product + "_" + time_string + ".png";

               cout << png_file_path << endl;

               RefPtr<ImageSurface> surface = ImageSurface::create (
                  FORMAT_ARGB32, size_2d.i, size_2d.j);
               RefPtr<Context> cr = Context::create (surface);

               display.cairo (cr, transform, size_2d,
                  dtime, level, stage, product);
               title.set (dtime.get_string ("%Y.%m.%d %H:%M UTC"), product, stage);
               title.cairo (cr);

               surface->write_to_png (png_file_path);
               continue;

            }

            continue;
         }

         continue;

      }

      for (map<string, Transform_2D*>::iterator iterator = transform_ptr_map.begin ();
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

