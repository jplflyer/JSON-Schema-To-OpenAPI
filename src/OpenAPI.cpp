#include "OpenAPI.h"

OpenAPI::OpenAPI()
{
}

void OpenAPI::fromJSON(const JSON &) {
}

/**
 * Write to JSON.
 */
JSON &OpenAPI::toJSON(JSON &json) const {
    setStringValue(json, "openapi", openapi);
    setStringValue(json, "jsonSchemaDialect", jsonSchemaDialect);

    JSON infoJSON;
    setJSONValue(json, "info", info.toJSON(infoJSON));

    JSON serversJSON;
    setJSONValue(json, "servers", servers.toJSON(serversJSON));

    JSON pathsJSON;
    setJSONValue(json, "paths", paths.toJSON(pathsJSON));

    JSON webhooksJSON;
    setJSONValue(json, "webhooks", webhooks.toJSON(webhooksJSON));

    JSON componentsJSON;
    setJSONValue(json, "components", components.toJSON(componentsJSON));

    JSON securitiesJSON;
    setJSONValue(json, "security", securities.toJSON(securitiesJSON));

    JSON tagsJSON;
    setJSONValue(json, "tags", tags.toJSON(tagsJSON));

    JSON externalDocsJSON;
    setJSONValue(json, "externalDocs", externalDocs.toJSON(externalDocsJSON));

    return json;
}
