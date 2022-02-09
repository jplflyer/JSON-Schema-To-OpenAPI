#pragma once

#include <showlib/StringVector.h>
#include "JSONSchema.h"
#include "OpenAPI.h"

class JSON_Schema_To_OpenAPI
{
public:
    void processArgs(int argc, char **argv);
    void doRead();
    void copy();
    void doWrite();

protected:
    ShowLib::StringVector inputFiles;
    std::string outputFile;

    JSONSchema::Vector schemas;
    OpenAPI openAPI;
};

