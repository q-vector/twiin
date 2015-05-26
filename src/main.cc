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

   const string vertical_coefficients_file_path (argv[1]);
   const string orog_3_file_path (argv[2]);
   const string lsm_3_file_path (argv[3]);
   const string orog_4_file_path (argv[4]);
   const string lsm_4_file_path (argv[5]);
   const string orog_5_file_path (argv[6]);
   const string lsm_5_file_path (argv[7]);
   const string station_file_path (argv[8]);

   Model::Stage::File_Path_Map model_file_path_3_map;
   Model::Stage::File_Path_Map model_file_path_4_map;
   Model::Stage::File_Path_Map model_file_path_5_map;
   model_file_path_3_map.insert (string ("temp"), argv[9]);
   model_file_path_3_map.insert (string ("dewpt"), argv[10]);
   model_file_path_3_map.insert (string ("xwind"), argv[11]);
   model_file_path_3_map.insert (string ("ywind"), argv[12]);
   model_file_path_3_map.insert (string ("mslp"), argv[13]);
   model_file_path_3_map.insert (string ("ml_prho"), argv[14]);
   model_file_path_3_map.insert (string ("ml_ptheta"), argv[15]);
   model_file_path_3_map.insert (string ("ml_theta"), argv[16]);
   model_file_path_3_map.insert (string ("ml_spechum"), argv[17]);
   model_file_path_3_map.insert (string ("ml_xwind"), argv[18]);
   model_file_path_3_map.insert (string ("ml_ywind"), argv[19]);
   model_file_path_4_map.insert (string ("temp"), argv[20]);
   model_file_path_4_map.insert (string ("dewpt"), argv[21]);
   model_file_path_4_map.insert (string ("xwind"), argv[22]);
   model_file_path_4_map.insert (string ("ywind"), argv[23]);
   model_file_path_4_map.insert (string ("mslp"), argv[24]);
   model_file_path_5_map.insert (string ("temp"), argv[25]);
   model_file_path_5_map.insert (string ("dewpt"), argv[26]);
   model_file_path_5_map.insert (string ("xwind"), argv[27]);
   model_file_path_5_map.insert (string ("ywind"), argv[28]);
   model_file_path_5_map.insert (string ("mslp"), argv[29]);

   const string product_str (argv[30]);
   const string stage_str (argv[31]);
   const string level_str (argv[32]);

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
      const Display display (vertical_coefficients_file_path,
         orog_3_file_path, lsm_3_file_path, orog_4_file_path, lsm_4_file_path,
         orog_5_file_path, lsm_5_file_path, station_file_path, model_file_path_3_map,
         model_file_path_4_map, model_file_path_5_map);

      const Level level ("500m");
      const Model& model = display.get_model ();

      for (Tokens::const_iterator i = stage_tokens.begin ();
           i != stage_tokens.end (); i++)
      {

         const twiin::Stage stage (*i);
         const Model::Stage& model_stage = model.get_model_stage (stage);
         const set<Dtime>& valid_time_set = model_stage.get_valid_time_set ();

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

