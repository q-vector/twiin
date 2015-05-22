#ifndef NC_FILE_H
#define NC_FILE_H

#include <iostream>
#include <netcdf.h>
#include <denise/analysis.h>

using namespace std;
using namespace denise;

namespace twiin
{

   class Nc_File
   {

      protected:

         int
         nc_id;

      public:

         const string
         file_path;

         Nc_File (const string& file_path);

         ~Nc_File ();

         int
         get_nc_id () const;

         static Integer
         get_datum_size (const nc_type nctype);

         static Real
         get_datum (const void* pointer,
                    const nc_type nc_type);

         Tuple
         get_coordinate_tuple (const string& dim_str,
                               string var_str = "") const;

   };

};

#endif /* NC_FILE_H */
