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
