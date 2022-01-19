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

    namespace InformationSchemaSQL1 {
        std::string tables =
        "  (TABLE_SCHEMA VARCHAR(64) NOT NULL,"
        "   TABLE_NAME VARCHAR(64) NOT NULL)";
        std::string columns =   
        "  (TABLE_SCHEMA VARCHAR(64) NOT NULL,"
        "   TABLE_NAME VARCHAR(64) NOT NULL,"
        "   COLUMN_NAME VARCHAR(64) NOT NULL,"
        "   DATA_TYPE VARCHAR(64) NOT NULL,"
        "   ORDINAL_POSITION BIGINT NOT NULL)";
        std::string routines =  
        "  (SPECIFIC_NAME varchar(64) NOT NULL DEFAULT '',"
        "   ROUTINE_CATALOG varchar(512) NOT NULL DEFAULT '',"
        "   ROUTINE_SCHEMA varchar(64) NOT NULL DEFAULT '',"
        "   ROUTINE_NAME varchar(64) NOT NULL DEFAULT '',"
        "   ROUTINE_TYPE varchar(9) NOT NULL DEFAULT '',"
        "   DATA_TYPE varchar(64) NOT NULL DEFAULT '',"
        "   CHARACTER_MAXIMUM_LENGTH int(21) DEFAULT NULL,"
        "   CHARACTER_OCTET_LENGTH int(21) DEFAULT NULL,"
        "   NUMERIC_PRECISION int(21) DEFAULT NULL,"
        "   NUMERIC_SCALE int(21) DEFAULT NULL,"
        "   CHARACTER_SET_NAME varchar(64) DEFAULT NULL,"
        "   COLLATION_NAME varchar(64) DEFAULT NULL,"
        "   DTD_IDENTIFIER longtext,"
        "   ROUTINE_BODY varchar(8) NOT NULL DEFAULT '',"
        "   ROUTINE_DEFINITION longtext,"
        "   EXTERNAL_NAME varchar(64) DEFAULT NULL,"
        "   EXTERNAL_LANGUAGE varchar(64) DEFAULT NULL,"
        "   PARAMETER_STYLE varchar(8) NOT NULL DEFAULT '',"
        "   IS_DETERMINISTIC varchar(3) NOT NULL DEFAULT '',"
        "   SQL_DATA_ACCESS varchar(64) NOT NULL DEFAULT '',"
        "   SQL_PATH varchar(64) DEFAULT NULL,"
        "   SECURITY_TYPE varchar(7) NOT NULL DEFAULT '',"
        "   CREATED datetime NOT NULL DEFAULT '0000-00-00 00:00:00',"
        "   LAST_ALTERED datetime NOT NULL DEFAULT '0000-00-00 00:00:00',"
        "   SQL_MODE varchar(8192) NOT NULL DEFAULT '',"
        "   ROUTINE_COMMENT longtext NOT NULL,"
        "   DEFINER varchar(77) NOT NULL DEFAULT '',"
        "   CHARACTER_SET_CLIENT varchar(32) NOT NULL DEFAULT '',"
        "   COLLATION_CONNECTION varchar(32) NOT NULL DEFAULT '',"
        "   DATABASE_COLLATION varchar(32) NOT NULL DEFAULT '')";
        std::string parameters = 
        "  (SPECIFIC_CATALOG varchar(512) NOT NULL DEFAULT '',"
        "   SPECIFIC_SCHEMA varchar(64) NOT NULL DEFAULT '',"
        "   SPECIFIC_NAME varchar(64) NOT NULL DEFAULT '',"
        "   ORDINAL_POSITION int(21) NOT NULL DEFAULT '0',"
        "   PARAMETER_MODE varchar(5) DEFAULT NULL,"
        "   PARAMETER_NAME varchar(64) DEFAULT NULL,"
        "   DATA_TYPE varchar(64) NOT NULL DEFAULT '',"
        "   CHARACTER_MAXIMUM_LENGTH int(21) DEFAULT NULL,"
        "   CHARACTER_OCTET_LENGTH int(21) DEFAULT NULL,"
        "   NUMERIC_PRECISION int(21) DEFAULT NULL,"
        "   NUMERIC_SCALE int(21) DEFAULT NULL,"
        "   CHARACTER_SET_NAME varchar(64) DEFAULT NULL,"
        "   COLLATION_NAME varchar(64) DEFAULT NULL,"
        "   DTD_IDENTIFIER longtext NOT NULL,"
        "   ROUTINE_TYPE varchar(9) NOT NULL DEFAULT '')"; 
        std::string schemata = 
        "  (CATALOG_NAME varchar(512) NOT NULL DEFAULT '',"
        "   SCHEMA_NAME varchar(64) NOT NULL DEFAULT '',"
        "   DEFAULT_CHARACTER_SET_NAME varchar(32) NOT NULL DEFAULT '',"
        "   DEFAULT_COLLATION_NAME varchar(32) NOT NULL DEFAULT '',"
        "   SQL_PATH varchar(512) DEFAULT NULL)";
        std::string engines = 
        "  (ENGINE varchar(64) NOT NULL DEFAULT '',"
        "   SUPPORT varchar(8) NOT NULL DEFAULT '',"
        "   COMMENT varchar(80) NOT NULL DEFAULT '',"
        "   TRANSACTIONS varchar(3) DEFAULT NULL,"
        "   XA varchar(3) DEFAULT NULL,"
        "   SAVEPOINTS varchar(3) DEFAULT NULL)";
    }
};

