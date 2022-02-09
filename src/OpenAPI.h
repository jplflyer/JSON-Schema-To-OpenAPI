#pragma once

#include <string>
#include <showlib/JSONSerializable.h>
#include <showlib/StringVector.h>

class OpenAPI: public ShowLib::JSONSerializable
{
public:

    /**
     * Contact information.
     */
    class Contact: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Contact> Pointer;
        typedef ShowLib::JSONSerializableVector<Contact> Vector;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        /** Name of the contact person / org. */
        std::string name;

        /** A URL pointing to the contact info. */
        std::string url;

        /** Email address. */
        std::string email;
    };

    /**
     * License info.
     */
    class License: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<License> Pointer;
        typedef ShowLib::JSONSerializableVector<License> Vector;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        /** Required. The license name such as "Apache 2.0". */
        std::string name;

        /** An SPDX license expression. Mutually exclusive with URL. */
        std::string identifier;

        /** URL to the license. Mutually exclusive with identifier. */
        std::string url;
    };

    /**
     * The object provides metadata about the API. The metadata MAY
     * be used by the clients if needed, and MAY be presented in
     * editing or documentation generation tools for convenience.
     */
    class Info: public ShowLib::JSONSerializable {
    public:
        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        /** Required. The title of the API. */
        std::string	title;

        /** A short summary of the API. */
        std::string	summary;

        /** A description of the API. CommonMark syntax may be used. */
        std::string	description;

        /** A URL to the Terms of service. */
        std::string	termsOfService;

        /** Required. The OpenAPI version such as "3.0.0". */
        std::string	version;

        /** The contact info for the exposed API. */
        Contact contact;

        /** The license information for the exposed API. */
        License license;
    };

    /**
     * Allows referencing external documentation.
     */
    class ExternalDocs: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<ExternalDocs> Pointer;
        typedef ShowLib::JSONSerializableVector<ExternalDocs> Vector;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        /** Description. CommonMark syntax allowed. */
        std::string description;

        /** Required. The URL to the docs. */
        std::string url;
    };

    /**
     * Just a simple map.
     */
    typedef ShowLib::JSONSerializableMap<std::string> Scope_Map;

    /**
     * A single OAuthFlow.
     */
    class OAuthFlow: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<OAuthFlow> Pointer;
        typedef ShowLib::JSONSerializableVector<OAuthFlow> Vector;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        /** Required. The authoriztion URL. */
        std::string authorizationUrl;

        /** Required. The token URL. */
        std::string tokenUrl;

        /** URL to use for refreshing tokens. */
        std::string refreshUrl;

        /** Available scopes. */
        Scope_Map scopes;
    };

    /**
     * Configuration of the supported OAuth flows. This is a
     * collection of OAuthFlow objects.
     */
    class OAuthFlows: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<OAuthFlows> Pointer;
        typedef ShowLib::JSONSerializableVector<OAuthFlows> Vector;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        OAuthFlow::Pointer implicit = nullptr;
        OAuthFlow::Pointer password = nullptr;
        OAuthFlow::Pointer clientCredentials = nullptr;
        OAuthFlow::Pointer authorizationCode = nullptr;
    };

    /**
     * Defines a security scheme that can be used by the operations.
     *
     * Supported schemes are HTTP authentication, an API key (either
     * as a header, a cookie parameter or as a query parameter),
     * mutual TLS (use of a client certificate), OAuth2’s common
     * flows (implicit, password, client credentials and authorization
     * code) as defined in [RFC6749], and OpenID Connect Discovery.
     * Please note that as of 2020, the implicit flow is about to
     * be deprecated by OAuth 2.0 Security Best Current Practice.
     * Recommended for most use case is Authorization Code Grant flow
     * with PKCE.
     */
    class Security: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Security> Pointer;
        typedef ShowLib::JSONSerializableVector<Security> Vector;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        /**
         * Required. Type of the security scheme. Valid values
         * are: apiKey, http, mutualTLS, oauth2, openIdConnect.
         */
        std::string type;

        /** A description. CommonMark syntax allowed. */
        std::string description;

        /* Required. The name. */
        std::string name;

        /** Required. Location of the API key. Valid values are
         * query, header, or cookie. */
        std::string in;

        /** Required. The name of the HTTP AUthorization scheme. */
        std::string scheme;

        /** A hint to the client how the bearer format is formatted. */
        std::string bearerFormat;

        /** Required. OpenID Connect URL to discover OAuth2 configuration values. */
        std::string openIdConnectUrl;

        OAuthFlows oauthFlows;
    };

    /**
     * A variable that can be substituted.
     */
    class Variable: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Variable> Pointer;
        typedef ShowLib::JSONSerializableVector<Variable> Vector;
        typedef ShowLib::JSONSerializableMap<Variable> Map;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        /** We're stored as a map, but this is our key. */
        std::string key;

        /** A list of specific values that can be used. */
        ShowLib::StringVector	enumValues;

        /** Required. The default value. If enum exists, this value must appear in the list. */
        std::string defaultValue;

        /** Optional description. */
        std::string description;
    };

    /**
     * Represents a server.
     */
    class Server: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Server> Pointer;
        typedef ShowLib::JSONSerializableVector<Server> Vector;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        /** Required. A URL to the server. May use the variables and may be relative. */
        std::string url;

        /** Optional description. */
        std::string description;

        /** Fields in the url inside {brackets} are replaced by the variables. */
        Variable::Map variables;
    };

    /**
     * Describes a single operation parameter.
     *
     * A unique parameter is defined by a combination of a name and location.
     * 4.8.12.1 Parameter Locations §
     *
     * There are four possible parameter locations specified by the in field:
     *
     *     path - Used together with Path Templating, where the parameter value
     *     			is actually part of the operation’s URL. This does not
     *     			include the host or base path of the API. For example,
     *     			in /items/{itemId}, the path parameter is itemId.
     *
     *     query - Parameters that are appended to the URL. For example, in
     *     			/items?id=###, the query parameter is id.
     *
     *     header - Custom headers that are expected as part of the request.
     *     			Note that [RFC7230] states header names are case insensitive.
     *
     *     cookie - Used to pass a specific cookie value to the API.
     */
    class Parameter: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Parameter> Pointer;
        typedef ShowLib::JSONSerializableVector<Parameter> Vector;
        typedef ShowLib::JSONSerializableMap<Parameter> Map;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        std::string name;
        std::string in;
        std::string description;
        std::string style;
        std::string explode;

        bool required = false;
        bool deprecated = false;
        bool allowEmptyValue = false;
        bool allowReserved = false;

        // Not sure how to handle:
        // example
        // examples
        // content
    };

    class RequestBody: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<RequestBody> Pointer;
        typedef ShowLib::JSONSerializableVector<RequestBody> Vector;
        typedef ShowLib::JSONSerializableMap<RequestBody> Map;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;
    };

    class Response: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Response> Pointer;
        typedef ShowLib::JSONSerializableVector<Response> Vector;
        typedef ShowLib::JSONSerializableMap<Response> Map;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;
    };

    /**
     * Definition of what happens with get, put, etc.
     */
    class Operation: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Operation> Pointer;
        typedef ShowLib::JSONSerializableVector<Operation> Vector;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        ShowLib::StringVector tags;
        std::string summary;
        std::string description;
        std::string operationId;
        bool deprecated = false;

        ExternalDocs::Pointer externalDocs;
        Parameter::Vector parameters;
        RequestBody requestBody;
        Response::Vector responses;
        Security::Vector securities;
        Server::Vector servers;
    };

    /**
     * Holds the relative paths to the individual endpoints.
     * We store this as a map with the path itself as the key.
     */
    class Path: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Path> Pointer;
        typedef ShowLib::JSONSerializableVector<Path> Vector;
        typedef ShowLib::JSONSerializableMap<Path> Map;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        std::string ref;
        std::string summary;
        std::string description;

        Operation::Pointer getOperation = nullptr;
        Operation::Pointer putOperation = nullptr;
        Operation::Pointer postOperation = nullptr;
        Operation::Pointer deleteOperation = nullptr;
        Operation::Pointer optionsOperation = nullptr;
        Operation::Pointer headOperation = nullptr;
        Operation::Pointer patchOperation = nullptr;
        Operation::Pointer traceOperation = nullptr;

        Server::Vector servers;
    };

    /**
     *
     */
    class WebHook: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<WebHook> Pointer;
        typedef ShowLib::JSONSerializableVector<WebHook> Vector;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;
    };

    class Schema: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Schema> Pointer;
        typedef ShowLib::JSONSerializableVector<Schema> Vector;
        typedef ShowLib::JSONSerializableMap<Schema> Map;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;
    };

    class Example: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Example> Pointer;
        typedef ShowLib::JSONSerializableVector<Example> Vector;
        typedef ShowLib::JSONSerializableMap<Example> Map;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;
    };


    class Header: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Header> Pointer;
        typedef ShowLib::JSONSerializableVector<Header> Vector;
        typedef ShowLib::JSONSerializableMap<Header> Map;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;
    };

    class Link: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Link> Pointer;
        typedef ShowLib::JSONSerializableVector<Link> Vector;
        typedef ShowLib::JSONSerializableMap<Link> Map;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;
    };

    class Callback: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Callback> Pointer;
        typedef ShowLib::JSONSerializableVector<Callback> Vector;
        typedef ShowLib::JSONSerializableMap<Callback> Map;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;
    };

    /**
     * Holds a set of reusable objects for different aspects of the OAS.
     * All objects defined within the components object will have no effect
     * on the API unless explicitely references from properties outside
     * the components object.
     */
    class Component: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Component> Pointer;
        typedef ShowLib::JSONSerializableVector<Component> Vector;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;

        Schema::Map schemas;
        Response::Map responses;
        Parameter::Map parameters;
        Example::Map examples;
        RequestBody::Map requestBodies;
        Header::Map headers;
        Link::Map links;
        Callback::Map callbacks;
        Path::Map pathItems;
    };

    class Tag: public ShowLib::JSONSerializable {
    public:
        typedef std::shared_ptr<Tag> Pointer;
        typedef ShowLib::JSONSerializableVector<Tag> Vector;

        void fromJSON(const JSON &) override;
        JSON &toJSON(JSON &) const override;
    };

    //======================================================================
    // Methods
    //======================================================================
    OpenAPI();
    void fromJSON(const JSON &) override;
    JSON &toJSON(JSON &) const override;

    //======================================================================
    // Fields
    //======================================================================

    /** The version of the standard. */
    std::string	openapi = "3.0.0";

    /** The info objected. Required. */
    Info info;

    std::string jsonSchemaDialect;

    Server::Vector servers;
    Path::Map paths;
    WebHook::Vector webhooks;
    Component::Vector components;
    Security::Vector securities;
    Tag::Vector tags;
    ExternalDocs externalDocs;
};

