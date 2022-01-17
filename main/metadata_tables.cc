#include <string>
#include <memory>

#include <main/metadata_tables.hh>
#include <main/Connect.hh>
#include <main/macro_util.hh>

std::string
MetaData::Table::metaObject()
{
    return DB::embeddedDB() + "." + Internal::getPrefix() + "MetaObject";
}

std::string
MetaData::Table::bleedingMetaObject()
{
    return DB::embeddedDB() + "." + Internal::getPrefix() +
           "BleedingMetaObject";
}

std::string
MetaData::Table::embeddedQueryCompletion()
{
    return DB::embeddedDB() + "." + Internal::getPrefix() +
           "embeddedQueryCompletion";
}

std::string
MetaData::Table::staleness()
{
    return DB::embeddedDB() + "." + Internal::getPrefix() +
           "staleness";
}

std::string
MetaData::Table::showDirective()
{
    return DB::embeddedDB() + "." + Internal::getPrefix() +
           "showDirective";
}

std::string
MetaData::Table::remoteQueryCompletion()
{
    return DB::remoteDB() + "." + Internal::getPrefix() +
           "remoteQueryCompletion";
}

std::string
MetaData::Table::information_schema_tables()
{
    return DB::information_schema() + "." + "TABLES";
}

std::string
MetaData::Table::information_schema_columns()
{
    return DB::information_schema() + "." + "COLUMNS";
}

std::string 
MetaData::Table::information_schema_routines()
{
    return DB::information_schema() + "." + "ROUTINES";
}

std::string 
MetaData::Table::information_schema_parameters()
{
    return DB::information_schema() + "." + "PARAMETERS";
}

std::string 
MetaData::Table::information_schema_schemata()
{
    return DB::information_schema() + "." + "SCHEMATA";
}

std::string
MetaData::Proc::activeTransactionP()
{
    return DB::remoteDB() + "." + Internal::getPrefix() + "activeTransactionP";
}

std::string
MetaData::DB::embeddedDB()
{
    static const std::string name = "embedded_db";
    return name;
}

std::string
MetaData::DB::information_schema()
{
    // todo: replace information_schema_meta to information_schema
    static const std::string name = "information_schema_meta";
    return name;
}

std::string
MetaData::DB::remoteDB()
{
    static const std::string name = "remote_db";
    return name;
}

bool static
hasWhitespace(const std::string &s)
{
    for (auto it : s) {
        if (isspace(it)) {
            return true;
        }
    }

    return false;
}

bool
MetaData::initialize(const std::unique_ptr<Connect> &conn,
                     const std::unique_ptr<Connect> &e_conn,
                     const std::string &prefix)
{
    // HACK: prevents multiple initialization
    static bool initialized = false;
    if (initialized) {
        return false;
    }

    // Prefix handling must be done first.
    if (hasWhitespace(prefix)) {
        return false;
    }
    MetaData::Internal::initPrefix(prefix);

    // Embedded database.
    const std::string create_db =
        " CREATE DATABASE IF NOT EXISTS " + DB::embeddedDB() + ";";
    RETURN_FALSE_IF_FALSE(e_conn->execute(create_db));

    const std::string create_meta_table =
        " CREATE TABLE IF NOT EXISTS " + Table::metaObject() +
        "   (serial_object VARBINARY(500) NOT NULL,"
        "    serial_key VARBINARY(500) NOT NULL,"
        "    parent_id BIGINT NOT NULL,"
        "    id SERIAL PRIMARY KEY)";
        // " ENGINE=InnoDB;";
    RETURN_FALSE_IF_FALSE(e_conn->execute(create_meta_table));

    const std::string create_bleeding_table =
        " CREATE TABLE IF NOT EXISTS " + Table::bleedingMetaObject() +
        "   (serial_object VARBINARY(500) NOT NULL,"
        "    serial_key VARBINARY(500) NOT NULL,"
        "    parent_id BIGINT NOT NULL,"
        "    id SERIAL PRIMARY KEY)";
        // " ENGINE=InnoDB;";
    RETURN_FALSE_IF_FALSE(e_conn->execute(create_bleeding_table));

    {
        const std::string len(std::to_string(STORED_QUERY_LENGTH));
        const std::string create_embedded_completion =
            " CREATE TABLE IF NOT EXISTS " + Table::embeddedQueryCompletion() +
            "   (complete BOOLEAN NOT NULL,"
            "    original_query VARCHAR(" + len + ") NOT NULL,"
            "    rewritten_query VARCHAR(" + len + ") NOT NULL,"
            "    default_db VARCHAR(500),"      // default database is NULLable
            "    aborted BOOLEAN NOT NULL,"
            "    type VARCHAR(100) NOT NULL,"
            "    id SERIAL PRIMARY KEY)";
            // " ENGINE=InnoDB;";
        RETURN_FALSE_IF_FALSE(e_conn->execute(create_embedded_completion));
    }

    const std::string create_staleness =
        " CREATE TABLE IF NOT EXISTS " + Table::staleness() +
        "   (cache_id BIGINT UNIQUE NOT NULL,"
        "    stale BOOLEAN NOT NULL,"
        "    id SERIAL PRIMARY KEY)";
        // " ENGINE=InnoDB;";
    RETURN_FALSE_IF_FALSE(e_conn->execute(create_staleness));

    const std::string create_show_directive =
        " CREATE TABLE IF NOT EXISTS " + Table::showDirective() +
        "   (_database VARCHAR(500) NOT NULL,"
        "    _table VARCHAR(500) NOT NULL,"
        "    _field VARCHAR(500) NOT NULL,"
        "    _onion VARCHAR(500) NOT NULL,"
        "    _level VARCHAR(500) NOT NULL,"
        "    id SERIAL PRIMARY KEY)";
        // " ENGINE=InnoDB;";
    RETURN_FALSE_IF_FALSE(e_conn->execute(create_show_directive));

    // Remote database.
    const std::string create_remote_db =
        " CREATE DATABASE IF NOT EXISTS " + DB::remoteDB() + ";";
    RETURN_FALSE_IF_FALSE(conn->execute(create_remote_db));

    const std::string create_remote_completion =
        " CREATE TABLE IF NOT EXISTS " + Table::remoteQueryCompletion() +
        "   (embedded_completion_id INTEGER NOT NULL,"
        "    completion_type VARCHAR(100) NOT NULL,"
        "    id SERIAL PRIMARY KEY)";
        // " ENGINE=InnoDB;";
    RETURN_FALSE_IF_FALSE(conn->execute(create_remote_completion));

    // information_schema database.
    const std::string create_information_schema =
        " CREATE DATABASE IF NOT EXISTS " + DB::information_schema() + ";";
    RETURN_FALSE_IF_FALSE(e_conn->execute(create_information_schema));

    const std::string create_information_schema_tables =
        " CREATE TABLE IF NOT EXISTS " + Table::information_schema_tables() +
        "   (TABLE_SCHEMA VARCHAR(64) NOT NULL,"
        "    TABLE_NAME VARCHAR(64) NOT NULL)";
        // " ENGINE=InnoDB;";
    RETURN_FALSE_IF_FALSE(e_conn->execute(create_information_schema_tables));

    const std::string create_information_schema_columns =
        " CREATE TABLE IF NOT EXISTS " + Table::information_schema_columns() +
        "   (TABLE_SCHEMA VARCHAR(64) NOT NULL,"
        "    TABLE_NAME VARCHAR(64) NOT NULL,"
        "    COLUMN_NAME VARCHAR(64) NOT NULL,"
        "    DATA_TYPE VARCHAR(64) NOT NULL,"
        "    ORDINAL_POSITION BIGINT NOT NULL)";
        // " ENGINE=InnoDB;";
    RETURN_FALSE_IF_FALSE(e_conn->execute(create_information_schema_columns));

    const std::string create_information_schema_routines = 
        "CREATE TABLE IF NOT EXISTS " + Table::information_schema_routines() +
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
    RETURN_FALSE_IF_FALSE(e_conn->execute(create_information_schema_routines));

    const std::string create_information_schema_parameters = 
        "CREATE TABLE IF NOT EXISTS " + Table::information_schema_parameters() +
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
    RETURN_FALSE_IF_FALSE(e_conn->execute(create_information_schema_parameters));

    const std::string create_information_schema_schemata =
        "CREATE TABLE IF NOT EXISTS " + Table::information_schema_schemata() +
        "  (CATALOG_NAME varchar(512) NOT NULL DEFAULT '',"
        "   SCHEMA_NAME varchar(64) NOT NULL DEFAULT '',"
        "   DEFAULT_CHARACTER_SET_NAME varchar(32) NOT NULL DEFAULT '',"
        "   DEFAULT_COLLATION_NAME varchar(32) NOT NULL DEFAULT '',"
        "   SQL_PATH varchar(512) DEFAULT NULL)";
    RETURN_FALSE_IF_FALSE(e_conn->execute(create_information_schema_schemata));

    initialized = true;
    return true;
}

void
MetaData::Internal::initPrefix(const std::string &s)
{
    lowLevelPrefix(s.c_str());
}

const std::string &
MetaData::Internal::getPrefix()
{
    return lowLevelPrefix(NULL);
}

const std::string &
MetaData::Internal::lowLevelPrefix(const char *const p)
{
    static const std::string prefix = (assert(p), p);
    return prefix;
}

