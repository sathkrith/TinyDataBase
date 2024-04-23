#pragma once

#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <unordered_map>

// Define structures to represent databases, tables, rows, and columns
struct Row {
    std::shared_ptr<Table> table;
    std::vector<std::string> values;
    std::unordered_map<std::string, std::string> columns;
};

struct Column {
    std::string name;
    std::string type;
};


struct Table {
    std::string name;
    std::unordered_map<std::string, int> columns;
    std::vector<std::string> types;
    std::vector<Row> rows;
};

struct Database {
    std::string name;
    std::map<std::string, Table> tables;
};

// Define a class to represent the DBMS
class DBMS {
private:
    std::map<std::string, Database> databases;
public:
    // Functions to create databases, tables, execute queries, etc.
    void createDatabase(const std::string& name);
    void createTable(const std::string& dbName, const std::string& tableName, const std::vector<std::string>& columns);
    void executeQuery(const std::string& query);
    bool parseScript(const std::filesystem::path& path);
};

