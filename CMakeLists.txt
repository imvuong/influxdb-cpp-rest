cmake_minimum_required (VERSION 3.9)

project (influxdb-cpp-rest CXX)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")
endif()

# we need this for tests
# add_subdirectory(deps/catch)

# Prepare "Catch" library
set(CATCH_INCLUDE_DIR deps/catch/single_include)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# todo: remove comments below
# add_subdirectory(deps/fmt)
add_subdirectory(deps/rxcpp EXCLUDE_FROM_ALL)
target_include_directories(RxCpp INTERFACE deps/rxcpp/Rx/v2/src/rxcpp)

include_directories(deps/rxcpp/Rx/v2/src/rxcpp)

# Uncomment when cpprestsdk has support for cmake find_package()
find_package(cpprestsdk REQUIRED)
find_package(Boost COMPONENTS thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

file(GLOB_RECURSE influx_source_files src/influxdb-cpp-rest/*.cpp)
file(GLOB_RECURSE influx_header_files src/influxdb-cpp-rest/*.h)

add_library(influxdb-cpp-rest SHARED ${influx_source_files})
target_link_libraries(influxdb-cpp-rest cpprestsdk::cpprest fmt Boost::thread)

target_include_directories(influxdb-cpp-rest INTERFACE "${THIRD_PARTY_MODULE_PATH}/influxdb-cpp-rest/src/influxdb-cpp-rest")

add_executable (influxdb-cpp-rest-demo src/demo/main.cpp)
target_include_directories(influxdb-cpp-rest-demo PUBLIC src/influxdb-cpp-rest)
target_link_libraries (influxdb-cpp-rest-demo influxdb-cpp-rest)

install (TARGETS influxdb-cpp-rest-demo DESTINATION bin)
install (TARGETS influxdb-cpp-rest DESTINATION lib)
install(FILES ${influx_header_files} DESTINATION include/influxdb-cpp-rest)
