#include "OpenAPI.h"

void OpenAPI::Operation::fromJSON(const JSON &json) {
}

JSON & OpenAPI::Operation::toJSON(JSON &json) const {
    json = JSON::object();

    translateAndSet(json, "tags", tags);
    if (externalDocs != nullptr) {
        translateAndSet(json, "externalDocs", *externalDocs);
    }
    translateAndSet(json, "parameters", parameters);
    translateAndSet(json, "requestBody", requestBody);
    translateAndSet(json, "reponses", responses);
    translateAndSet(json, "security", securities);
    translateAndSet(json, "servers", servers);

    setStringValue(json, "summary", summary);
    setStringValue(json, "description", description);
    setStringValue(json, "operationId", operationId);

    if (deprecated) {
        json["deprecated"] = deprecated;
    }
    return json;
}
