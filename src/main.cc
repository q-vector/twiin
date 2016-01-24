#include <getopt.h>
#include <denise/dstring.h>
#include <denise/gzstream.h>
#include "main.h"
#include "twiin.h"

#ifndef ENABLE_GTKMM
#else /* ENABLE_GTKMM */
#include "cross_section.h"
#endif /* ENABLE_GTKMM */

using namespace std;
using namespace Cairo;
using namespace denise;
using namespace twiin;

int
main (int argc,
      char** argv)
{

   static struct option long_options[] =
   {
      { "azimuth",                      1, 0, 'A' },
      { "annotation",                   1, 0, 'a' },
      { "bludge",                       0, 0, 'b' },
      { "color-bar",                    1, 0, 'C' },
      { "config",                       1, 0, 'c' },
      { "filename",                     1, 0, 'F' },
      { "format",                       1, 0, 'f' },
      { "geometry",                     1, 0, 'g' },
      { "gui",                          0, 0, 'G' },
      { "height",                       1, 0, 'h' },
      { "plot-first-char-of-track-id",  0, 0, 'I' },
      { "interactive",                  0, 0, 'i' },
      { "journey",                      0, 0, 'J' },
      { "track",                        0, 0, 'j' },
      { "trajectory",                   0, 0, 'j' },
      { "scale-bar",                    1, 0, 'K' },
      { "level",                        1, 0, 'l' },
      { "track-map",                    1, 0, 'M' },
      { "trajectory-map",               1, 0, 'M' },
      { "meteogram",                    0, 0, 'm' },
      { "no-nwp",                       0, 0, 'N' },
      { "location",                     1, 0, 'O' },
      { "output-dir",                   1, 0, 'o' },
      { "ignore-pressure",              0, 0, 'P' },
      { "product",                      1, 0, 'p' },
      { "no-stage",                     0, 0, 'S' },
      { "stage",                        1, 0, 's' },
      { "title",                        1, 0, 'T' },
      { "time",                         1, 0, 't' },
      { "u_bg",                         1, 0, 'u' },
      { "vertical-profile",             1, 0, 'v' },
      { "no-wind-barb",                 0, 0, 'W' },
      { "time-cross",                   0, 0, 'X' },
      { "cross-section",                0, 0, 'x' },
      { "zoom",                         1, 0, 'z' },
      { "help",                         0, 0, '?' },
      { NULL, 0, 0, 0 }
   };

   Size_2D size_2d (960, 960);
   Dstring output_dir (".");
   Dstring product_str ("WIND");
   Dstring stage_str ("STAGE3");
   Dstring level_str ("");
   Dstring time_str;
   Dstring zoom_str ("");
   Dstring format ("png");
   Real azimuth = 0;
   Tokens annotation_tokens;

   Dstring filename ("");
   Tokens title_tokens;
   bool is_bludge = false;
   bool is_cross_section = false;
   bool is_time_cross = false;
   bool is_gui = false;
   bool is_interactive = false;
   bool is_meteogram = false;
   bool lagrangian = false;
   bool ignore_pressure = false;
   bool no_nwp = false;
   bool no_stage = false;
   bool no_wind_barb = false;
   Dstring color_bar_str = "r:10";
   Dstring scale_bar_str = "lb:10";
   bool is_vertical_profile = false;
   Real u_bg = 0;
   Real distance = 100e3;
   Real height = 8000;
   Dstring location_str ("");
   Tokens journey_tokens;
   Track::Map track_map;
   Dstring track_id_str ("");
   bool track_id_initial = false;
   Dstring config_file_path (Dstring (getenv ("HOME")) + "/.twiin.rc");

   int c;
   int option_index = 0;
   char optstring[] = "A:a:bC:c:d:F:f:Gg:h:IiJ:j:K:Ll:M:mNO:o:Pp:Ss:T:t:u:vVWXxz:?";
   while ((c = getopt_long (argc, argv, optstring,
          long_options, &option_index)) != -1)
   {

      switch (c)
      {

         case 'A':
         {
            azimuth = stof (Dstring (optarg));
            break;
         }

         case 'a':
         {
            annotation_tokens.push_back (Dstring (optarg));
            break;
         }

         case 'b':
         {
            is_bludge = true;
            break;
         }

         case 'C':
         {
            color_bar_str = Dstring (optarg);
            break;
         }

         case 'c':
         {
            config_file_path = (Dstring (optarg));
            break;
         }

         case 'd':
         {
            distance = stof (Dstring (optarg));
            break;
         }

         case 'F':
         {
            filename = (Dstring (optarg));
            break;
         }

         case 'f':
         {
            format = (Dstring (optarg));
            break;
         }

         case 'G':
         {
            is_gui = true;
            break;
         }

         case 'g':
         {
            const Tokens tokens (Dstring (optarg), "x");
            size_2d.i = stof (tokens[0]);
            size_2d.j = stof (tokens[1]);
            break;
         }

         case 'h':
         {
            height = stof (Dstring (optarg));
            break;
         }

         case 'I':
         {
            track_id_initial = true;
            break;
         }

         case 'i':
         {
            is_interactive = true;
            break;
         }

         case 'J':
         {
            const Tokens tokens (Dstring (optarg), ":");
            for (auto iterator = tokens.begin ();
                 iterator != tokens.end (); iterator++)
            {
               const Dstring& journey_str = *(iterator);
               journey_tokens.push_back (journey_str);
            }
            break;
         }

         case 'j':
         {
            track_id_str = Dstring (optarg);
            break;
         }

         case 'K':
         {
            scale_bar_str = Dstring (optarg);
            break;
         }

         case 'L':
         {
            lagrangian = true;
            break;
         }

         case 'l':
         {
            level_str = (Dstring (optarg));
            break;
         }

         case 'M':
         {
            const Dstring file_path (optarg);
            igzstream file (file_path);
            if (!file.is_open ())
            {
               throw IO_Exception ("Can't open " + file_path);
            }
            track_map.ingest (file);
            file.close ();
            break;
         }

         case 'm':
         {
            is_gui = false;
            is_interactive = false;
            is_meteogram = true;
            break;
         }

         case 'N':
         {
            no_nwp = true;
            break;
         }

         case 'O':
         {
            location_str = (Dstring (optarg));
            break;
         }

         case 'o':
         {
            output_dir = (Dstring (optarg));
            break;
         }

         case 'P':
         {
            ignore_pressure = true;
            break;
         }

         case 'p':
         {
            product_str = (Dstring (optarg));
            break;
         }

         case 'S':
         {
            no_stage = true;
            break;
         }

         case 's':
         {
            stage_str = (Dstring (optarg));
            break;
         }

         case 'T':
         {
            title_tokens = Tokens (Dstring (optarg), ":");
            break;
         }

         case 't':
         {
            time_str = (Dstring (optarg));
            break;
         }

         case 'u':
         {
            u_bg = stof (Dstring (optarg));
            break;
         }

         case 'v':
         {
            is_gui = false;
            is_interactive = false;
            is_vertical_profile = true;
            break;
         }

         case 'W':
         {
            no_wind_barb = true;
            break;
         }

         case 'x':
         {
            is_gui = false;
            is_interactive = false;
            is_cross_section = true;
            break;
         }

         case 'X':
         {
            is_gui = false;
            is_interactive = false;
            is_time_cross = true;
            break;
         }

         case 'z':
         {
            zoom_str = (Dstring (optarg));
            break;
         }

         case '?':
         {
            Twiin::usage ();
            exit (0);
            break;
         }

         default:
         {
            cerr << "Error options " << c << endl;
            break;
         }

      }

   }

   try
   {

      const bool level_specified = (level_str != "");
      const bool journey_specified = (journey_tokens.size () != 0);
      const bool location_specified = (location_str != "");
      const bool track_specified = (track_id_str != "");

      const Config_File config_file (config_file_path);
      Twiin twiin (size_2d, config_file, output_dir);

      if (is_interactive)
      {
         Twiin::Andrea andrea (config_file);
         andrea.loop ();
      }
      else
      if (is_gui)
      {
#ifndef ENABLE_GTKMM
         cerr << "GUI mode not available" << endl;
#else /* ENABLE_GTKMM */
         Gtk::Main gtk_main (argc, argv);
         twiin.gui (stage_str, product_str, (level_specified ? level_str
            : "Surface"), time_str, journey_tokens, zoom_str);
#endif /* ENABLE_GTKMM */
      }
      else
      {

         if (is_cross_section)
         {
            if (journey_specified)
            {
               twiin.cross_section (stage_str, product_str, journey_tokens,
                  time_str, height, format, title_tokens, filename,
                  u_bg, is_bludge);
            }
            if (track_specified)
            {
               twiin.cross_section (stage_str, product_str, track_id_str,
                  track_map, distance, time_str, height, format,
                  title_tokens, filename, lagrangian, is_bludge);
            }
         }
         else
         if (is_time_cross)
         {
            if (location_specified)
            {
               twiin.time_cross (stage_str, product_str, azimuth, location_str,
                  time_str, height, format, title_tokens, filename, is_bludge);
            }
            if (track_specified)
            {
               twiin.time_cross (stage_str, product_str, track_id_str,
                  track_map, height, format, title_tokens, filename, lagrangian,
                  is_bludge);
            }
         }
         else
         if (is_meteogram)
         {
            if (location_specified)
            {
               twiin.meteogram (stage_str, location_str, time_str,
                  format, title_tokens, filename, ignore_pressure,
                  no_nwp, is_bludge);
            }
         }
         else
         if (is_vertical_profile)
         {
            if (location_specified)
            {
               twiin.vertical_profile (stage_str, location_str,
                  time_str, format, title_tokens, filename, is_bludge);
            }
            if (track_specified)
            {
               twiin.vertical_profile (stage_str, track_id_str, track_map,
                  time_str, format, title_tokens, filename, is_bludge);
            }
         }
         else
         {
            if (level_specified)
            {
               twiin.plan (stage_str, product_str, level_str, time_str,
                  zoom_str, track_id_str, track_id_initial, track_map,
                  annotation_tokens, format, title_tokens, filename,
                  color_bar_str, scale_bar_str, no_stage, no_wind_barb,
                  is_bludge);
            }
            else
            if (track_specified)
            {
               twiin.plan (stage_str, product_str, time_str, zoom_str,
                  track_id_str, track_id_initial, track_map, annotation_tokens,
                  format, title_tokens, filename, color_bar_str, scale_bar_str,
                  no_stage, no_wind_barb, is_bludge);
            }
         }
      }

   }
   catch (const Exception& e)
   {
      cerr << "Exception " << e.what () << endl;
   }
   catch (const std::exception& se)
   {
      cerr << "std::exception " << se.what () << endl;
   }

}

