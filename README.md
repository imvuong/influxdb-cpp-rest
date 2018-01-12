# influxdb-cpp-rest

[![Build status](https://ci.appveyor.com/api/projects/status/68w68vq3nai4794g/branch/master?svg=true)](https://ci.appveyor.com/project/d-led/influxdb-cpp-rest/branch/master) [![Build Status](https://travis-ci.org/d-led/influxdb-cpp-rest.svg?branch=master)](https://travis-ci.org/d-led/influxdb-cpp-rest)

A naive C++(14) [InfluxDB](https://www.influxdata.com/time-series-platform/influxdb/) client via [C++ REST SDK](https://github.com/Microsoft/cpprestsdk) + a C wrapper of the asynchronous API as a shared library.

See [the demo source](src/demo/main.cpp) for the current api example.

The unbatched aprroach (and without connection reuse) may not be sufficient in some situations, as without batching, about 200 lines/sec can be inserted.

A batching api leans towards thousands inserts per second. Behind the scenes, the API uses [RxCpp](https://github.com/Reactive-Extensions/RxCpp) and [cppformat](https://github.com/fmtlib/fmt).

## Status

Build and test ok on Win10/Ubuntu64/OSX.

Feel free to contribute, as the progress is rather sporadic due to lack of spare time.

## Synchronous insertion

```cpp
influxdb::api::simple_db simpledb("http://localhost:8086", "my_db");
db.insert(
	line("log", key_value_pairs("my_tag", 42L), key_value_pairs("value", "hello world!")));
```

## Asynchronous insertion

```cpp
influxdb::async_api::simple_db asyncdb("http://localhost:8086", "my_db");

for (int i = 0; i < 123456; i++) {
  asyncdb.insert(
    line(
      "my_measurements",
      key_value_pairs("my_count", i % MAX_VALUES_PER_TAG),
      key_value_pairs("value", "hi!")
    ));
}
```

## C API

see [async_c_test.cpp](src/test-shared/async_c_test.cpp) and the related headers.

## Timestamps

Timestamps can be added as the last parameter to the `line` constructor, and only need to return
a serializable value on `TTimestamp::now()`. There is a default `std::chrono`-based implementation:

```cpp
    line(
      "my_measurements",
      key_value_pairs("my_count", i % MAX_VALUES_PER_TAG),
      key_value_pairs("value", "hi!"),
      default_timestamp()
//    ^^^^^^^^^^^^^^^^^^^
    )
```


## Multiple lines in synchronous API

Add lines using the `()` operator on the line:

```cpp
  line
    ("multiple", key_value_pairs("v1", 1), key_value_pairs())
    ("multiple", key_value_pairs("v2", 2), key_value_pairs())
```

## Query

```cpp
influxdb::raw::db_utf8 raw_db("http://localhost:8086", "my_db");
auto query = std::string("select count(*) from my_db..my_measurements";
auto json_response = raw_db.get(query);
```

&darr;

```
{"results":[{"series":[{"name":"asynctest","columns":["time","count_value"],"values":[["...Z",123456]]}]}]}
```

## Authentication

Basic authentication can be used with all API variants

```cpp
influxdb::raw::db_utf8 raw_db("http://localhost:8086", "my_db");
raw_db.with_authentication(username, password);
auto query = ...
```
## Cloning
The library uses git submodules.  The easiest way to ensure that the submodules are included in the clone is to add `--recursive` in the clone command

```
git clone --recursive https://github.com/imvuong/influxdb-cpp-rest
```

## Build & Test

The library should be easy to build, given `RxCpp` and `cpprestsdk` can be found. The Visual Studio 2015 solution is self-contained. A locally running, authentication-free instance of InfluxDB is required to run the test.

### Dependencies on Linux and OS X

cpprestsdk needs to be built and available, which in turn has platform-specific transient dependencies.

The easiest way to install it on MacOS X and Linux turned out to be via [Homebrew](https://brew.sh) and [Linuxbrew](https://linuxbrew.sh) respectively.

Once the install `brew install cpprestsdk` succeeds, build: `make -C build/<platform>/gmake config=release_x64` and run the test.

If the build fails due to failed dependencies, check [premake5.lua](premake5.lua) for the build config, and regenerate makefiles if necessary via `premake/premake5<os-specific> gmake`

## Building - OSX

Installing dependencies using Homebrew

```
brew install fmt cpprestsdk openssl
```

Building influxdb-cpp-rest

```
cd influxdb-cpp-rest
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/path/to/install/dir -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl -DOPENSSL_LIBRARIES=/usr/local/opt/openssl/lib ..
make
make install
```

## Thanks to

- @kirkshoop for indispensable help with [RxCpp](https://github.com/Reactive-Extensions/RxCpp)
- @nikkov for pointing out the missing essential features
- @promgamer for the identifiers PR
