#include <iostream>
#include <fstream>

#include <showlib/FileUtilities.h>
#include <showlib/OptionHandler.h>

#include "JSON_Schema_To_OpenAPI.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

/**
 * Entry point.
 */
int main(int argc, char **argv) {
    JSON_Schema_To_OpenAPI worker;

    worker.processArgs(argc, argv);
    worker.doRead();
    worker.copy();
    worker.doWrite();
}

/**
 * Process command line options.
 */
void JSON_Schema_To_OpenAPI::processArgs(int argc, char **argv)
{
    ShowLib::OptionHandler::ArgumentVector args;

    args.addArg("input", [=](const char *value) { inputFiles.add(value); }, "foo.json", "Add a JSON Schema input file");
    args.addArg("output", [=](const char *value) { outputFile = value; }, "foo.json", "The output location");
    args.addArg("title", [=](const char *value) { openAPI.info.title = value; }, "A title", "Set the title");
    args.addArg("summary", [=](const char *value) { openAPI.info.summary = value; }, "A summary", "Set the summary");
    args.addArg("description", [=](const char *value) { openAPI.info.description = value; }, "A description", "Set the description");
    args.addArg("version", [=](const char *value) { openAPI.info.version = value; }, "1.0.1", "Set the version");

    if ( !ShowLib::OptionHandler::handleOptions(argc, argv, args) ) {
        exit(0);
    }

    if (inputFiles.empty()) {
        cerr << "One or more --input filename is required." << endl;
        exit(0);
    }
    if (outputFile.empty()) {
        cout << "--output filename is required." << endl;
        exit(0);
    }
}

/**
 * Read the input files.
 */
void JSON_Schema_To_OpenAPI::doRead() {
    for (const std::shared_ptr<string> & ptr: inputFiles) {
        JSONSchema::Pointer schema = std::make_shared<JSONSchema>();
        std::string contents = ShowLib::FileUtilities::readFile(*ptr);
        JSON json = JSON::parse(contents);
        schema->fromJSON(json);
        schemas.push_back(schema);
    }
}

/**
 * Copy our JSON Schemas into our OpenAPI object.
 */
void JSON_Schema_To_OpenAPI::copy() {
    OpenAPI::Info & info = openAPI.info;

    for (const JSONSchema::Pointer &ptr: schemas) {
        if ( !ptr->title.empty() && info.title.empty() ) {
            info.title = ptr->title;
        }
        if ( !ptr->description.empty() && info.description.empty() ) {
            info.description = ptr->description;
        }

        //======================================================================
        // Links in the JSONSchema turn into Paths.
        //======================================================================
        OpenAPI::Path::Map & paths = openAPI.paths;
        for (const JSONSchema::Link::Pointer &link: ptr->links) {
            OpenAPI::Path::Pointer path = nullptr;

            for (const auto & it: paths) {
                if (it.first == link->href) {
                    path = it.second;
                    break;
                }
            }

            if (path == nullptr) {
                path = OpenAPI::Path::make();
                paths.insert(std::make_pair(link->href, path));
            }

            OpenAPI::Operation::Pointer op = OpenAPI::Operation::make();
            if (link->method == "GET") {
                path->getOperation = op;
            }
            else if (link->method == "PUT") {
                path->putOperation = op;
            }
            else if (link->method == "POST") {
                path->postOperation = op;
            }
            else if (link->method == "DELETE" || link->method == "DEL") {
                path->deleteOperation = op;
            }
            else {
                cout << "Unrecognized method: " << link->method << endl;
            }

            op->summary = link->title;
            op->description = link->description;

            // I believe we're expecting either object or array.
            if ( ! link->targetSchema.type.empty()) {
            }
            if ( ! link->schema.type.empty()) {
                if (link->schema.type == "object") {
                }
                else if (link->schema.type == "array") {
                }
                else {
                    cout << "Unrecognized schema.type: " << link->schema.type << endl;
                }

                for (auto it = link->schema.properties.begin(); it != link->schema.properties.end(); ++it) {
                    string key = it->first;
                    JSONSchema::Property::Pointer prop = it->second;
                }
            }
        }
    }

}

/**
 * Write the output file.
 */
void JSON_Schema_To_OpenAPI::doWrite() {
    std::ofstream out(outputFile);

    JSON json;
    openAPI.toJSON(json);

    out << json.dump(2) << endl;
}
