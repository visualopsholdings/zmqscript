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

BOOST_AUTO_TEST_CASE( externArgs )
{
  cout << "=== externArgs ===" << endl;
  
  Server s(0, "../test/commands.json");
  auto obj = dictO({
    { "name", "hello" }
  });
  auto r = s.process_reply(obj);
  BOOST_CHECK(r);
  BOOST_CHECK_EQUAL(*r, "echo \"Hello World\"");
  
}

BOOST_AUTO_TEST_CASE( compoundArgs )
{
  cout << "=== compoundArgs ===" << endl;
  
  Server s(0, "../test/commands.json");
  auto obj = dictO({
    { "name", "hellowithargs" },
    { "args", dictO({
        { "msg", "World" }
      })
    }
  });
  auto r = s.process_reply(obj);
  BOOST_CHECK(r);
  BOOST_CHECK_EQUAL(*r, "echo \"Hello World\"");
  
}



