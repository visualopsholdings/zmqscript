/*
  testargs.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 28-Feb-2026
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/nodes
*/

#include <iostream>

#include "server.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace vops;

BOOST_AUTO_TEST_CASE( noArgs )
{
  cout << "=== noArgs ===" << endl;
  
  Server s(0, "../test/commands.json");
  auto obj = dictO({
    { "type", "hello" }
  });
  auto r = s.process_reply(obj);
  BOOST_CHECK(r);
  BOOST_CHECK_EQUAL(*r, "echo Hello World");
  
}

BOOST_AUTO_TEST_CASE( args )
{
  cout << "=== args ===" << endl;
  
  Server s(0, "../test/commands.json");
  auto obj = dictO({
    { "type", "hellowithargs" },
    { "args", dictO({
        { "msg", "World" }
      })
    }
  });
  auto r = s.process_reply(obj);
  BOOST_CHECK(r);
  BOOST_CHECK_EQUAL(*r, "echo Hello World");
  
}



