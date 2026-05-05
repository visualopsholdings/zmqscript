/*
  utils.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 22-Mar-2026
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes
*/

#include "utils.hpp"

#include "log.hpp"

#include <numeric> // Required for std::accumulate
#include <boost/log/trivial.hpp>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cxxabi.h>
#include <iostream>

void Utils::systemRun(std::optional<std::string> cmd, bool output) {

  if (!cmd) {
    return;
  }
  
  FILE *pipe = popen(cmd->c_str(), "r");
  if (!pipe) {
    BOOST_LOG_TRIVIAL(error) << "Failed to run command " << *cmd;
    return;
  }

  char buffer[128];
  while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
    if (output) {
      std::cout << buffer << std::flush;
    }
    else {
      L_DEBUG(buffer);
    }
  }
  pclose(pipe);

}

