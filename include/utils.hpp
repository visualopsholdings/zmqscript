/*
  utils.hpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 22-Mar-2026
    
  Utilities for Nodes.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes
*/

#ifndef H_utils
#define H_utils

#include <string>
#include <vector>
#include <optional>
#include <map>
#include <list>

class Utils {

public:
  
  static void systemRun(std::optional<std::string> cmd, bool output=false);

};

#endif // H_utils
