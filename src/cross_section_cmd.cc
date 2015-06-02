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

   const string station_file_path (argv[1]);
   const string model_config_file_path (argv[2]);
   const string product_str (argv[3]);
   const string stage_str (argv[4]);
   const string level_str (argv[5]);

   try
   {

      const Size_2D size_2d (1280, 720);
      const Display display (station_file_path, model_config_file_path);

      const twiin::Stage stage (stage_str);
      const Product product (product_str);
      const Affine_Transform_2D transform ();

      const Level level (level_str);
      const Model& model = display.get_model ();
      const set<Dtime>& valid_time_set =
         model.get_valid_time_set (product, stage, level);

      const Model::Terrain::Stage& terrain_stage =
         model.terrain.get_stage (stage);

      Multi_Journey multi_journey;
      const Lat_Long lat_long_a (-38.5, 443.0);
      const Lat_Long lat_long_b (-28.0, 157.0);
      multi_journey.push_back (lat_long_a);
      multi_journey.push_back (lat_long_b);
      const Geodesy geodesy;
      const Tuple& tuple_x = multi_journey.get_tuple_x (geodesy);

      Title title (size_2d);
      const Real margin_l = 50;
      const Real margin_r = 50;
      const Real margin_t = title.get_height () + 50;
      const Real margin_b = 50;
      const Real w = size_2d.i - margin_l - margin_r;
      const Real h = size_2d.j - margin_t - margin_b;

      const Domain_1D domain_x (tuple_x.front (), tuple_x.back ());
      const Domain_1D domain_z (0, 5000);
      const Domain_2D domain_2d (domain_x, domain_z);

      for (set<Dtime>::const_iterator j = valid_time_set.begin ();
           j != valid_time_set.end (); j++)
      {

         const Dtime& dtime = *(j);
         const string& time_string = dtime.get_string ("%Y%m%d%H%M");
         const string png_file_path = stage + "_" +
                  product + "_" + time_string + ".png";

         cout << png_file_path << endl;

         RefPtr<ImageSurface> surface = ImageSurface::create (
            FORMAT_ARGB32, size_2d.i, size_2d.j);
         RefPtr<Context> cr = Context::create (surface);

         cr->save ();
         //Color::hsb (0.67, 0.67, 0.85).cairo (cr);
         Color::hsb (0.0, 0.0, 1.0).cairo (cr);
         cr->paint ();

         const Simple_Mesh_2D ma0 (Color (0, 0, 0, 0.05), 1e8, 10);
         const Simple_Mesh_2D ma1 (Color (0, 0, 0, 0.1), 1e8, 100);
         const Simple_Mesh_2D ma2 (Color (0, 0, 0, 0.4), 1e8, 1000);

         Affine_Transform_2D transform;
         const Real span_x = domain_x.get_span ();
         const Real span_z = domain_z.get_span ();
         transform.scale (1, -1);
         transform.scale (w / span_x, h / span_z);
         transform.translate (0, size_2d.j);
         transform.translate (margin_l, -margin_b);

         const Index_2D i2d (margin_l, margin_t);
         const Size_2D s2d (size_2d.i-margin_l-margin_r, size_2d.j-margin_t-margin_b);

         Real x, z;


         const Box_2D box_2d (i2d, s2d);
         Raster* raster_ptr = new Raster (box_2d);
         Raster& raster = *raster_ptr;

         Color color;

         for (Integer i = i2d.i; i < i2d.i + s2d.i; i++)
         {

            transform.reverse (x, z, Real (i), 0);
            const Lat_Long lat_long = multi_journey.get_lat_long (x, geodesy);
            const Real latitude = lat_long.latitude;
            const Real longitude = lat_long.longitude;
            const Real topography = terrain_stage.evaluate (string ("orog"), latitude, longitude);

            for (Integer j = i2d.j; j < i2d.j + s2d.j; j++)
            {
               transform.reverse (x, z, Real (i), Real (j));
               if (z < topography) { color = Color::hsb (0.0, 0.0, 0.0); }
               else
               {
                  const Real datum = model.evaluate (THETA,
                     latitude, longitude, z, dtime, stage);
                  const Real hue = Domain_1D (60+K, K).normalize (datum)*0.833;
                  color = Color::hsb (hue, 0.8, 0.8);
               }
               raster.set_pixel (i - i2d.i, j - i2d.j, color);
            }

         }

         raster.blit (cr);
         delete raster_ptr;




         const Mesh_2D mesh_2d (domain_2d, ma2);
         cr->set_line_width (2);
         mesh_2d.render (cr, transform, Size_2D (2, 2));
         Color (0, 0, 0, 0.3).cairo (cr);

         title.set (dtime.get_string ("%Y.%m.%d %H:%M UTC"), product, stage);
         title.cairo (cr);

         surface->write_to_png (png_file_path);

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

