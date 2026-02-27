/*
  send.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 25-Feb-2026
    
  Command line driver for Nodes testing.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes
*/

#include "log.hpp"
#include "dict.hpp"

#include <iostream>
#include <boost/program_options.hpp> 
#include <zmq.hpp>
#include <boost/algorithm/string.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vops;

void send(zmq::socket_t &socket, const DictG &obj);
DictG receive(zmq::socket_t &socket);

int main(int argc, char *argv[]) {

  int pushPort;
  string logLevel;
  string jsonstr;
  
  po::options_description desc("Allowed options");
  desc.add_options()
    ("pushPort", po::value<int>(&pushPort)->default_value(3008), "Port for producer, normally 3008.")
    ("logLevel", po::value<string>(&logLevel)->default_value("info"), "Logging level [trace, debug, warn, info].")
    ("json", po::value<string>(&jsonstr)->required(), "The JSON to send")
    ("help", "produce help message")
    ;
  po::positional_options_description p;
  p.add("json", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
          options(desc).positional(p).run(), vm);
  po::notify(vm);   

  Log::config("Send     ", logLevel);

  if (vm.count("help")) {
    cout << desc << endl;
    return 1;
  }
  
  zmq::context_t context(1);
  zmq::socket_t push(context, ZMQ_PUSH);
  push.connect("tcp://127.0.0.1:" + to_string(pushPort));
	L_TRACE("Connect to ZMQ as Local PUSH on " << pushPort);
  
  // input is JSON.
  auto j = Dict::parseString(jsonstr, ".json");
  send(push, *j);
    
  return 0;
 
}

void send(zmq::socket_t &socket, const DictG &json) {

  L_DEBUG("-> " << Dict::toString(json));

  // send JSON for now.
  string m = Dict::toString(json, false, ".json");
  zmq::message_t msg(m.length());
  memcpy(msg.data(), m.c_str(), m.length());
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
    socket.send(msg);
#else
    socket.send(msg, zmq::send_flags::none);
#endif

}
