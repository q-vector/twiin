#include "nc_file.h"

using namespace std;
using namespace denise;
using namespace twiin;

Nc_File::Nc_File (const string& file_path)
   : file_path (file_path)
{
   int ret = nc_open (file_path.c_str (), NC_NOWRITE, &nc_id);
   if (ret != NC_NOERR) { throw Exception ("nc_open " + file_path); }
cout << nc_id << " " << file_path << endl;
}

Nc_File::~Nc_File ()
{
   int ret = nc_close (nc_id);
   if (ret != NC_NOERR) { throw Exception ("nc_close"); }
}

int
Nc_File::get_nc_id () const
{
   return nc_id;
}

Integer
Nc_File::get_datum_size (const nc_type var_type)
{

   switch (var_type)
   {
      case NC_BYTE:   return 1;
      case NC_CHAR:   return 1;
      case NC_SHORT:  return 2;
      case NC_INT:    return 4;
      case NC_FLOAT:  return 4;
      case NC_DOUBLE: return 8; 
   };

   throw Exception ("Invalid nc_type");

}

Real
Nc_File::get_datum (const void* pointer,
                    const nc_type var_type)
{

   switch (var_type)
   {
      case NC_BYTE:   return (Real)(*((int8_t*)pointer));
      case NC_CHAR:   return (Real)(*((char*)pointer));
      case NC_SHORT:  return (Real)(*((short*)pointer));
      case NC_INT:    return (Real)(*((int*)pointer));
      case NC_FLOAT:  return (Real)(*((float*)pointer));
      case NC_DOUBLE: return (Real)(*((double*)pointer));
   };

   throw Exception ("Invalid nc_type");

}

Tuple
Nc_File::get_coordinate_tuple (const string& dim_str,
                               string var_str) const
{

   if (var_str == "") { var_str = dim_str; }

   int ret, dim_id, var_id;
   nc_type var_type;
   size_t n;

   ret = nc_inq_dimid (nc_id, dim_str.c_str (), &dim_id);
   if (ret != NC_NOERR) { throw Exception ("nc_inq_dimid " + dim_str); }

   ret = nc_inq_dimlen (nc_id, dim_id, &n);
   if (ret != NC_NOERR) { throw Exception ("nc_inq_dimlen " + dim_str); }

   ret = nc_inq_varid (nc_id, var_str.c_str (), &var_id);
   if (ret != NC_NOERR) { throw Exception ("nc_inq_varid " + var_str); }

   ret = nc_inq_vartype (nc_id, var_id, &var_type);
   if (ret != NC_NOERR) { throw Exception ("nc_inq_vartype " + var_str); }

   Integer datum_size = Nc_File::get_datum_size (var_type);
   uint8_t* array = new uint8_t[n * datum_size];

   ret = nc_get_var (nc_id, var_id, array);
   if (ret != NC_NOERR) { throw Exception ("nc_get_var " + var_str); }

   Tuple coordinate_tuple;

   for (Integer i = 0; i < n; i++)
   {
      const void* pointer = array + (i * datum_size);
      const Real datum = Nc_File::get_datum (pointer, var_type);
      coordinate_tuple.push_back (datum);
   }

   delete[] array;

   return coordinate_tuple;

}

