#pragma once

//
// This classes represent the original schema as read from a file.
//

#include <unordered_map>
#include <memory>
#include <string>

#include <showlib/JSONSerializable.h>

/**
 * This represents JSON Schema as read from a file.
 */
class JSONSchema: public ShowLib::JSONSerializable {
public:
    typedef std::shared_ptr<JSONSchema> Pointer;
    typedef ShowLib::JSONSerializableVector<JSONSchema> Vector;

    class Definition;
    class Definition_Map: public std::unordered_map<std::string, std::shared_ptr<Definition>>, public JSONSerializable
    {
    public:
        void fromJSON(const JSON &);
        JSON &toJSON(JSON &) const;
    };

    /**
     * The definition of one value type.
     */
    class Definition: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Definition> Pointer;
        typedef ShowLib::JSONSerializableVector<Definition> Vector;
        typedef Definition_Map Map;

        void fromJSON(const JSON &);
        JSON &toJSON(JSON &) const;

        std::string name;
        std::string description;

        /** Type is stored as an array, but each example I have is a single value. */
        std::string type;
        std::string format;
        std::string example;
        bool readOnly = false;
    };

    class Property;
    class Property_Map: public std::unordered_map<std::string, std::shared_ptr<Property>>, public JSONSerializable
    {
    public:
        void fromJSON(const JSON &);
        JSON &toJSON(JSON &) const;
    };

    /**
     * A particular property, which might reference a Definition. These
     * can be defined at the top level or via targetSchema and schema from
     * inside a link.
     */
    class Property: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Property> Pointer;
        typedef ShowLib::JSONSerializableVector<Property> Vector;
        typedef Property_Map Map;

        void fromJSON(const JSON &);
        JSON &toJSON(JSON &) const;

        std::string name;
        std::string type;
        std::string ref;
        std::string example;
        Definition::Pointer refPtr = nullptr;

        // For array or object types.
        Map properties;
    };


    /**
     * A particular call.
     */
    class Link: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Link> Pointer;
        typedef ShowLib::JSONSerializableVector<Link> Vector;

        void fromJSON(const JSON &);
        JSON &toJSON(JSON &) const;

        std::string title;
        std::string description;
        std::string href;
        std::string method;
        std::string rel;

        /** targetSchema is what is returned. */
        Property targetSchema;

        /** schema is input body for PUT and POST. */
        Property schema;
    };

    void fromJSON(const JSON &);
    JSON &toJSON(JSON &) const;

    Definition::Map definitions;
    Property::Map properties;
    Link::Vector links;

    std::string id;
    std::string title;
    std::string description;
    std::string type;
};

