#   CMakeLists.txt
#
#   Author: Paul Hamilton (paul@visualops.com)
#   Date: 24-Feb-2026
#
#   Input file for ZMQ based script runner.
#
#   Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
#   
#   https://github.com/visualopsholdings/zmqscript

cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0167 NEW) # Boost
cmake_policy(SET CMP0169 OLD) # FetchContent

set(USE_MONGO ON)

project (zmqscript)
  find_package(Boost 1.90.0 COMPONENTS program_options filesystem unit_test_framework chrono thread log_setup log REQUIRED)
  find_package(cppzmq REQUIRED)
  add_definitions(-DBOOST_ALL_DYN_LINK) 
  enable_testing()
  
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX AND NOT APPLE)
  add_definitions(-funwind-tables) 
endif ()

# fetch from other repos.

include(FetchContent)

FetchContent_Declare(
  dict
  GIT_REPOSITORY https://github.com/visualopsholdings/dict.git
  GIT_TAG main
)
FetchContent_MakeAvailable(dict)

include_directories(${dict_SOURCE_DIR}/include)

FetchContent_Declare(
  dates
  GIT_REPOSITORY https://github.com/visualopsholdings/dates.git
  GIT_TAG main
)
FetchContent_MakeAvailable(dates)

include_directories(${dates_SOURCE_DIR}/include)

FetchContent_Declare(
  flo
  GIT_REPOSITORY https://github.com/visualopsholdings/flo.git
  GIT_TAG main
)
FetchContent_MakeAvailable(flo)

include_directories(${flo_SOURCE_DIR}/include)

set(LIBS zmq)
set(ZMQ
  src/pgm.cpp
)
set(BOOSTLIBS ${Boost_LOG_LIBRARY})

include_directories(include)

add_executable(ZMQScript src/zmqscript.cpp src/server.cpp src/utils.cpp ${ZMQ})
    target_link_libraries(ZMQScript DictLib FloLib DatesLib ${Boost_PROGRAM_OPTIONS_LIBRARY} ${LIBS})

add_executable(Send test/send.cpp ${ZMQ})
  target_link_libraries(Send DictLib DatesLib ${Boost_PROGRAM_OPTIONS_LIBRARY} ${BOOSTLIBS} ${LIBS})

# Tests

add_executable(TestArgs test/testargs.cpp src/server.cpp src/utils.cpp ${ZMQ})
  target_link_libraries(TestArgs DictLib FloLib DatesLib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${BOOSTLIBS} ${LIBS})

add_test(TestArgs TestArgs)

