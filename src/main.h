#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <denise/geodesy.h>
#include "display.h"

using namespace std;
using namespace denise;
using namespace Cairo;

namespace twiin
{

   void
   interactive (const Size_2D& size_2d,
                const Tokens& zoom_tokens,
                const Tokens& config_file_content,
                const string& stage_str,
                const string& product_str,
                const string& level_str,
                const string& time_str);

   void
   command_line (const Size_2D& size_2d,
                 const Tokens& zoom_tokens,
                 const Tokens& config_file_content,
                 const string& stage_str,
                 const string& product_str,
                 const string& level_str,
                 const string& time_str,
                 const bool is_test);

   void
   cross_section (const Size_2D& size_2d,
                  const Tokens& zoom_tokens,
                  const Tokens& config_file_content,
                  const string& stage_str,
                  const string& product_str,
                  const Multi_Journey& multi_journey,
                  const string& time_str,
                  const bool is_test);

};

#endif /* MAIN_H */
