TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += src

SOURCES += \
    src/JSON_Schema_To_OpenAPI.cpp \
    src/Callback.cpp \
    src/Component.cpp \
    src/Contact.cpp \
    src/Example.cpp \
    src/ExternalDocs.cpp \
    src/Header.cpp \
    src/Info.cpp \
    src/JSONSchema.cpp \
    src/License.cpp \
    src/Link.cpp \
    src/OAuthFlow.cpp \
    src/OpenAPI.cpp \
    src/Operation.cpp \
    src/Parameter.cpp \
    src/Path.cpp \
    src/RequestBody.cpp \
    src/Response.cpp \
    src/Schema.cpp \
    src/Security.cpp \
    src/Server.cpp \
    src/Tag.cpp \
    src/Variable.cpp \
    src/WebHook.cpp

HEADERS += \
    src/JSON_Schema_To_OpenAPI.h \
    src/JSONSchema.h \
    src/OpenAPI.h
