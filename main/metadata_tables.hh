#pragma once

#include <string>
#include <memory>

#include <main/Connect.hh>

#define STORED_QUERY_LENGTH 10000

namespace MetaData {
    bool initialize(const std::unique_ptr<Connect> &conn,
                    const std::unique_ptr<Connect> &e_conn,
                    const std::string &prefix);

    namespace Table {
        std::string metaObject();
        std::string bleedingMetaObject();
        std::string embeddedQueryCompletion();
        std::string staleness();
        std::string showDirective();
        std::string remoteQueryCompletion();
        std::string information_schema_tables();
        std::string information_schema_columns();
        std::string information_schema_routines();
        std::string information_schema_parameters();
        std::string information_schema_schemata();
        std::string information_schema_engines();
        std::string information_schema_createTable();
    };

    namespace Proc {
        std::string activeTransactionP();
    };

    namespace DB {
        std::string embeddedDB();
        std::string remoteDB();
        std::string information_schema();
    };

    namespace Internal {
        void initPrefix(const std::string &prefix);
        const std::string &getPrefix();
        const std::string &lowLevelPrefix(const char *const p);
    };

    namespace InformationSchemaSQL {
        std::string tables();
        std::string columns();
        std::string routines();
        std::string parameters();
        std::string schemata();
        std::string engines();
    }
};

