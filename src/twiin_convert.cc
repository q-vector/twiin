#include <getopt.h>
#include <denise/dstring.h>
#include "data.h"

using namespace std;
using namespace denise;
using namespace twiin;

int
main (int argc,
      char** argv)
{

/**/
   const string& aws_data_file_path (argv[1]);
   Aws::Repository aws_repository;
   aws_repository.ingest_binary (aws_data_file_path);
/**/

/*
   const string& aws_data_file_path (argv[1]);
   const string& aws_data_binary_file_path (argv[2]);

   Aws::Repository aws_repository;
   aws_repository.ingest (aws_data_file_path);
   aws_repository.write_binary (aws_data_binary_file_path);
*/

/*
   static struct option long_options[] =
   {
      { "annotation",       1, 0, 'a' },
      { "bludge",           0, 0, 'b' },
      { "config",           1, 0, 'c' },
      { "filename",         1, 0, 'F' },
      { "format",           1, 0, 'f' },
      { "geometry",         1, 0, 'g' },
      { "interactive",      0, 0, 'i' },
      { "level",            1, 0, 'l' },
      { "meteogram",        1, 0, 'm' },
      { "output-dir",       1, 0, 'o' },
      { "ignore-pressure",  0, 0, 'P' },
      { "product",          1, 0, 'p' },
      { "no-stage",         0, 0, 'S' },
      { "stage",            1, 0, 's' },
      { "title",            1, 0, 'T' },
      { "time",             1, 0, 't' },
      { "cross-section",    1, 0, 'x' },
      { "vertical-profile", 1, 0, 'v' },
      { "no-wind-barb",     0, 0, 'W' },
      { "zoom",             1, 0, 'z' },
      { NULL, 0, NULL, 0 }
   };

   Size_2D size_2d (960, 960);
   string output_dir (".");
   string product_str ("WIND");
   string stage_str ("STAGE3");
   string level_str ("Surface");
   string time_str;
   string zoom_str ("");
   string format ("png");
   Tokens annotation_tokens;

   string filename ("");
   Tokens title_tokens;
   bool is_bludge = false;
   bool is_cross_section = false;
   bool is_interactive = false;
   bool is_meteogram = false;
   bool ignore_pressure = false;
   bool no_stage = false;
   bool no_wind_barb = false;
   bool is_vertical_profile = false;
   string location_str ("");
   Multi_Journey multi_journey;
   string config_file_path (string (getenv ("HOME")) + "/.twiin.rc");

   int c;
   int option_index = 0;
   while ((c = getopt_long (argc, argv, "a:bc:F:f:g:il:m:o:Pp:Ss:T:t:x:v:Wz:",
          long_options, &option_index)) != -1)
   {

      switch (c)
      {

         case 'a':
         {
            annotation_tokens.push_back (string (optarg));
            break;
         }

         case 'b':
         {
            is_bludge = true;
            break;
         }

         case 'c':
         {
            config_file_path = (string (optarg));
            break;
         }

         case 'F':
         {
            filename = (string (optarg));
            break;
         }

         case 'f':
         {
            format = (string (optarg));
            break;
         }

         case 'g':
         {
            const Tokens tokens (string (optarg), "x");
            size_2d.i = stof (tokens[0]);
            size_2d.j = stof (tokens[1]);
            break;
         }

         case 'i':
         {
            is_interactive = true;
            break;
         }

         case 'l':
         {
            level_str = (string (optarg));
            break;
         }

         case 'm':
         {
            is_interactive = false;
            is_meteogram = true;
            location_str = (string (optarg));
            break;
         }

         case 'o':
         {
            output_dir = (string (optarg));
            break;
         }

         case 'P':
         {
            ignore_pressure = true;
            break;
         }

         case 'p':
         {
            product_str = (string (optarg));
            break;
         }

         case 'S':
         {
            no_stage = true;
            break;
         }

         case 's':
         {
            stage_str = (string (optarg));
            break;
         }

         case 'T':
         {
            title_tokens = Tokens (string (optarg), ":");
            break;
         }

         case 't':
         {
            time_str = (string (optarg));
            break;
         }

         case 'v':
         {
            is_interactive = false;
            is_vertical_profile = true;
            location_str = (string (optarg));
            break;
         }

         case 'W':
         {
            no_wind_barb = true;
            break;
         }

         case 'x':
         {
            is_interactive = false;
            is_cross_section = true;
            multi_journey = Multi_Journey (string (optarg));
            break;
         }

         case 'z':
         {
            zoom_str = (string (optarg));
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
   }
   catch (const Exception& e)
   {
      cerr << "Exception " << e << endl;
   }
   catch (const std::exception& se)
   {
      cerr << "std::exception " << se.what () << endl;
   }
*/

}

