#include "influxdb_raw_db.h"

using namespace web;
using namespace web::http;

inline influxdb::raw::db::db(string_t const & url) :client(url) {}

inline void influxdb::raw::db::post(string_t const & query) {
    uri_builder builder(U("/query"));

    builder.append_query(U("q"), query);

    // synchronous for now
    client.request(methods::POST, builder.to_string()).get();
}

inline json::value influxdb::raw::db::get(string_t const & query) {
    uri_builder builder(U("/query"));

    builder.append_query(U("q"), query);

    // synchronous for now
    auto response = client.request(methods::POST, builder.to_string()).get();
    if (response.status_code() == status_codes::OK)
    {
        return response.extract_json().get();
    }
    else
    {
        return json::value();
    }
}

inline void influxdb::raw::db::measure(string_t const & db, string_t const & lines) {
    uri_builder builder(client.base_uri());
    builder.append(U("/write"));
    builder.append_query(U("db"), db);

    http_request request;

    request.set_request_uri(builder.to_uri());
    request.set_method(methods::POST);
    request.set_body(lines);

    // synchronous for now
    client.request(request).get();
}