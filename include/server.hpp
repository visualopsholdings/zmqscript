/*
  server.hpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Feb-2026
    
  A server for the ZMQ scripter.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/zmqscript
*/

#ifndef H_server
#define H_server

#include "dict.hpp"

#include <string>
#include <map>
#include <zmq.hpp>

using namespace std;

namespace vops {

class Server {

public:
  Server(int pullPort, int pushPort, const string &commands);
    
  void run();
    // run the server.
    
private:
  zmq::context_t _context;
  zmq::socket_t _pull;
  zmq::socket_t _push;
  map<string, DictO> _commands;
  
  void handle_reply(const zmq::message_t &reply);
  
};

} // vops

#endif // H_server
