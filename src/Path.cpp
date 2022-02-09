#include "OpenAPI.h"

void OpenAPI::Path::fromJSON(const JSON &) {
}

JSON & OpenAPI::Path::toJSON(JSON &json) const {
    setStringValue(json, "$ref", ref);
    setStringValue(json, "summary", summary);
    setStringValue(json, "description", description);

    if (getOperation != nullptr) {
        translateAndSet(json, "get", *getOperation);
    }

    if (putOperation != nullptr) {
        translateAndSet(json, "put", *putOperation);
    }

    if (postOperation != nullptr) {
        translateAndSet(json, "post", *postOperation);
    }

    if (deleteOperation != nullptr) {
        translateAndSet(json, "delete", *deleteOperation);
    }

    if (optionsOperation != nullptr) {
        translateAndSet(json, "options", *optionsOperation);
    }

    if (headOperation != nullptr) {
        translateAndSet(json, "head", *headOperation);
    }

    if (patchOperation != nullptr) {
        translateAndSet(json, "patch", *patchOperation);
    }

    if (traceOperation != nullptr) {
        translateAndSet(json, "trace", *traceOperation);
    }

    translateAndSet(json, "servers", servers);

    return json;
}
