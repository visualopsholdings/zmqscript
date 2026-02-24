/*
  server.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Feb-2026
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/zmqscript
*/

#include "server.hpp"

#include <boost/log/trivial.hpp>

namespace vops {

Server::Server(int pullPort, int pushPort): 
    _context(1), _pull(_context, ZMQ_PULL), _push(_context, ZMQ_PUSH) {
    
  _pull.connect("tcp://127.0.0.1:" + to_string(pullPort));
  BOOST_LOG_TRIVIAL(info) << "Deploy consumer connected to port " << pullPort;
    
  _push.connect("tcp://127.0.0.1:" + to_string(pushPort));
   BOOST_LOG_TRIVIAL(info) << "Deploy producer connected to port " << pushPort;

}
    
void Server::run() {

  int major, minor, patch;
  zmq_version(&major, &minor, &patch);
  BOOST_LOG_TRIVIAL(info) << "zmq version " << major << "." << minor << "." << patch;

}

} // vops