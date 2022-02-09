#include <iostream>

#include "JSONSchema.h"

using std::cout;
using std::endl;
using std::string;

/**
 * Parse JSON.
 */
void JSONSchema::fromJSON(const JSON &json) {
    id = stringValue(json, "id");
    title = stringValue(json, "title");
    description = stringValue(json, "description");

    JSON typeJSON = jsonArray(json, "type");
    if (typeJSON.is_array() && typeJSON.size() > 0) {
        type = typeJSON[0];
    }
    else {
        cout << "Not sure what to do with this JSONSchema type." << endl;
    }

    JSON defJSON = jsonArray(json, "definitions");
    JSON propsJSON = jsonArray(json, "properties");
    JSON linksJSON = jsonArray(json, "links");

    cout << "Definitions..." << endl;
    definitions.fromJSON(defJSON);

    cout << "Properties..." << endl;
    properties.fromJSON(propsJSON);

    cout << "Links..." << endl;
    links.fromJSON(linksJSON);
}

/**
 * Return JSON.
 */
JSON & JSONSchema::toJSON(JSON &json) const {
    return json;
}

/**
 * Parse JSON.
 */
void JSONSchema::Definition::fromJSON(const JSON &json) {
    JSON anyOf = jsonArray(json, "anyOf");
    if ( !anyOf.empty() ) {
    }

    else {
        description = stringValue(json, "description");
        format = stringValue(json, "format");
        example = stringValue(json, "example");
        readOnly = boolValue(json, "readOnly");

        JSON typeJSON = jsonArray(json, "type");
        if (typeJSON.is_array() && typeJSON.size() > 0) {
            type = typeJSON[0];
        }
        else {
            cout << "Not sure what to do with this Definition type: " << name << endl;
        }
    }
}

/**
 * Return JSON.
 */
JSON & JSONSchema::Definition::toJSON(JSON &json) const {
    return json;
}

/**
 * Parse JSON.
 */
void JSONSchema::Definition_Map::fromJSON(const JSON &json) {
    for (auto it = json.begin(); it != json.end(); ++it) {
        string key = it.key();

        Definition::Pointer def = std::make_shared<Definition>();
        def->name = key;
        def->fromJSON(it.value());
    }
}

/**
 * Return JSON.
 */
JSON & JSONSchema::Definition_Map::toJSON(JSON &json) const {
    return json;
}

/**
 * Parse JSON. We're going to have a name, but we're probably part of
 * a map, not an array, so our parent gives us our name. Then we'll either
 * have a type or ref plus possibly an example.
 */
void JSONSchema::Property::fromJSON(const JSON &json) {
    ref = stringValue(json, "$ref");
    type = stringValue(json, "type");
    example = stringValue(json, "example");

    // If type is "array", then there is an "items" field that contains a map of "properties".
    // If type is "object", then there is a "properties" field with another map.
    if (type == "array") {
        JSON itemsJSON = jsonValue(json, "items");
        JSON propsJSON = jsonValue(itemsJSON, "properties");
        properties.fromJSON(propsJSON);
    }

    else if (type == "object") {
        JSON propsJSON = jsonValue(json, "properties");
        properties.fromJSON(propsJSON);
    }
}

/**
 * Return JSON.
 */
JSON & JSONSchema::Property::toJSON(JSON &json) const {
    return json;
}

/**
 * Parse JSON.
 */
void JSONSchema::Property_Map::fromJSON(const JSON &json) {
    for (auto it = json.begin(); it != json.end(); ++it) {
        string key = it.key();
        Property::Pointer prop = std::make_shared<Property>();
        prop->name = key;
        prop->fromJSON(it.value());
    }
}

/**
 * Return JSON.
 */
JSON & JSONSchema::Property_Map::toJSON(JSON &json) const {
    return json;
}

/**
 * Parse JSON.
 */
void JSONSchema::Link::fromJSON(const JSON &json) {
    title = stringValue(json, "title");
    description = stringValue(json, "description");
    href = stringValue(json, "href");
    method = stringValue(json, "method");
    rel = stringValue(json, "rel");

    JSON targetJSON = jsonValue(json, "targetSchema");
    JSON schemaJSON = jsonValue(json, "schema");
    targetSchema.fromJSON(targetJSON);
    schema.fromJSON(schemaJSON);
}

/**
 * Return JSON.
 */
JSON & JSONSchema::Link::toJSON(JSON &json) const {
    return json;
}
