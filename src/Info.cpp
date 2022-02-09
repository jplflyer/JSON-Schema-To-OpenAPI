#include "OpenAPI.h"

void OpenAPI::Info::fromJSON(const JSON &json) {
}

JSON & OpenAPI::Info::toJSON(JSON &json) const {
    setStringValue(json, "title", title);
    setStringValue(json, "summary", summary);
    setStringValue(json, "description", description);
    setStringValue(json, "termsOfService", termsOfService);
    setStringValue(json, "version", version);

    JSON contactJSON;
    setJSONValue(json, "contact", contact.toJSON(contactJSON));

    JSON licenseJSON;
    setJSONValue(json, "license", license.toJSON(licenseJSON));

    return json;
}
