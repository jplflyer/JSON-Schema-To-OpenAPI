#include "OpenAPI.h"

void OpenAPI::Operation::fromJSON(const JSON &json) {
}

JSON & OpenAPI::Operation::toJSON(JSON &json) const {
    json = JSON::object();
    return json;
}
