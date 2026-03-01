/*
  server.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Feb-2026
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/zmqscript
*/

#include "server.hpp"
#include "log.hpp"
#include "processor.hpp"
#include "functions.hpp"

#include <boost/log/trivial.hpp>
#include <cstdlib>

namespace vops {

using namespace flo;

Server::Server(int pullPort, const string &commands): 
    _context(1), _pull(_context, ZMQ_PULL) {
  
  // load up all of the commands.  
  auto cmds = Dict::parseFile(commands);
  if (!cmds) {
    return;
  }
  
  auto v = Dict::getVector(*cmds);
  if (!v) {
    L_ERROR("no vector in " << commands);
    return;
  }
  transform(v->begin(), v->end(), inserter(_commands, _commands.end()), [](auto e) {
    auto o = Dict::getObject(e);
    if (!o) {
      L_ERROR("not object");
      return pair<string, DictO>("???", dictO({}));
    }
    L_TRACE(Dict::toString(*o));
    auto type = Dict::getString(*o, "type");
    if (!type) {
      L_ERROR("missing type");
      return pair<string, DictO>("???", dictO({}));
    }
    auto cmd = Dict::getObject(*o, "command");
    if (!cmd) {
      L_ERROR("missing command");
      return pair<string, DictO>(*type, dictO({}));
    }
    return pair<string, DictO>(*type, *cmd);
  });
  
  _pull.bind("tcp://*:" + to_string(pullPort));
  L_INFO("Consumer connected to port " << pullPort);

}
    
void Server::run() {

  int major, minor, patch;
  zmq_version(&major, &minor, &patch);
  BOOST_LOG_TRIVIAL(info) << "zmq version " << major << "." << minor << "." << patch;

  BOOST_LOG_TRIVIAL(trace) << "start receiving";

  zmq::pollitem_t items [] = {
      { _pull, 0, ZMQ_POLLIN, 0 }
  };
  const std::chrono::milliseconds timeout{500};
  while (1) {
    
//    BOOST_LOG_TRIVIAL(debug) << "polling for messages";
    zmq::message_t message;
    zmq::poll(items, 1, timeout);

//    BOOST_LOG_TRIVIAL(trace) << "sub events " << items[0].revents;

    if (items[0].revents & ZMQ_POLLIN) {
      BOOST_LOG_TRIVIAL(trace) << "got _pull message";
      zmq::message_t reply;
      try {
#if CPPZMQ_VERSION == ZMQ_MAKE_VERSION(4, 3, 1)
        _pull.recv(&reply);
#else
        auto res = _pull.recv(reply, zmq::recv_flags::none);
#endif
        // convert to JSON
        string r((const char *)reply.data(), reply.size());
      
        auto doc = Dict::getObject(Dict::parseString(r));
        if (!doc) {
          L_ERROR("could not parse body to JSON object.");
          return;
        }
        L_DEBUG("<- " << Dict::toString(*doc));
        handle_reply(*doc);
      }
      catch (zmq::error_t &e) {
        BOOST_LOG_TRIVIAL(warning) << "got exc with _sub recv " << e.what() << "(" << e.num() << ")";
      }
    }
  }
  
}

optional<string> Server::process_reply(const DictO &doc) {

  // switch the handler based on the message type.
  auto type = Dict::getString(doc, "type");
  if (!type) {
    L_ERROR("no type");
    return nullopt;
  }

  // lookup the type in the commands.
  map<string, DictO>::iterator cmd = _commands.find(*type);
  if (cmd == _commands.end()) {
    L_ERROR("unknown reply type " << *type);
    return nullopt;
  }
  
  auto bash = Dict::getObject(cmd->second, "bash");
  if (!bash) {
    L_ERROR("unknown command bash");
    return nullopt;
  }
  
  auto input = dictO({});
  
  Functions f(*bash);
  Processor p(f);
  auto result = p.transform(*bash, input, Dict::getObject(doc, "args"));

  if (!result) {
     return nullopt;
  }
  
  return Dict::getString(*result);
  
}

void Server::handle_reply(const DictO &doc) {

  auto result = process_reply(doc);  
  if (!result) {
     return;
  }
  
  // and execute in Bash.
  stringstream ss;
  ss << "/bin/bash -c ";
  ss << Dict::toString(*result);
  L_TRACE(ss.str());
  auto _ = system(ss.str().c_str());
  
}

} // vops