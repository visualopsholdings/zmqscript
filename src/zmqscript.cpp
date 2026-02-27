/*
  zmqscript.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Feb-2026
    
  ZMQ based script runner.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/zmqscript
*/

#include "server.hpp"

#include <boost/program_options.hpp> 
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vops;

int main(int argc, char *argv[]) {

  string version = "ZMQScript v0.1.0, 24-Feb-2026.";

  string logLevel;
  int pullPort;
  string commands;
  
  po::options_description desc("Allowed options");
  desc.add_options()
    ("pullPort", po::value<int>(&pullPort)->default_value(3008), "Port for consumer, normally 3008.")
    ("commands", po::value<string>(&commands)->default_value("../test/commands.json"), "List of commands to execute.")
    ("logLevel", po::value<string>(&logLevel)->default_value("info"), "Logging level [trace, debug, warn, info].")
    ("help", "produce help message")
    ;
  po::positional_options_description p;

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
          options(desc).positional(p).run(), vm);
  po::notify(vm);   

  if (logLevel == "trace") {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::trace);
  }
  else if (logLevel == "debug") {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
  }
  else if (logLevel == "warn") {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::warning);
  }
  else {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
  }

  boost::log::formatter logFmt =
         boost::log::expressions::format("%1%\tZScript [%2%]\t%3%")
        %  boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") 
        %  boost::log::expressions::attr< boost::log::trivial::severity_level>("Severity")
        %  boost::log::expressions::smessage;
  boost::log::add_common_attributes();
  boost::log::add_console_log(clog)->set_formatter(logFmt);

  if (vm.count("help")) {
    cout << desc << endl;
    return 1;
  }
  
  BOOST_LOG_TRIVIAL(info) << version;
  
  Server server(pullPort, commands);
  server.run();
  
}
