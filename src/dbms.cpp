#include "dbms.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>
// Function to tokenize a string into words
std::vector<std::string> tokenize(const std::string& str, char delim = ' ') {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to parse a SQL query
void parseQuery(const std::string& query) {
    // Tokenize the query
    std::vector<std::string> tokens = tokenize(query);

    // Display the parsed tokens
    std::cout << "Parsed Tokens:\n";
    for (const auto& token : tokens) {
        std::cout << token << '\n';
    }
}


// Function to execute the query and call the appropriate function based on the operation
std::vector<Row> executeQuery(const std::string& query) {
    // Dummy query parsing, replace with actual query parsing logic
    std::string operation;
    std::vector<std::string> tokens;

    size_t pos = 0;
    while ((pos = query.find(' ')) != std::string::npos) {
        tokens.push_back(query.substr(0, pos));
        query.erase(0, pos + 1);
    }
    tokens.push_back(query);

    operation = tokens[0];

    // Execute the appropriate operation
    if (operation == "SELECT") {
        if (tokens.size() != 2) {
            std::cerr << "Invalid SELECT query\n";
            return {};
        }
        return executeSelectQuery(tokens[1]);
    } else if (operation == "INNER_JOIN") {
        if (tokens.size() != 4) {
            std::cerr << "Invalid INNER_JOIN query\n";
            return {};
        }
        return executeInnerJoin(tokens[1], tokens[2], tokens[3]);
    } else if (operation == "LEFT_OUTER_JOIN") {
        if (tokens.size() != 4) {
            std::cerr << "Invalid LEFT_OUTER_JOIN query\n";
            return {};
        }
        return executeLeftOuterJoin(tokens[1], tokens[2], tokens[3]);
    } else if (operation == "RIGHT_OUTER_JOIN") {
        if (tokens.size() != 4) {
            std::cerr << "Invalid RIGHT_OUTER_JOIN query\n";
            return {};
        }
        return executeRightOuterJoin(tokens[1], tokens[2], tokens[3]);
    } else {
        std::cerr << "Unsupported operation: " << operation << '\n';
        return {};
    }
}


// Function to execute a SELECT query
std::vector<Row> executeSelectQuery(const std::string& query, const std::vector<Row>& data) {
    // Tokenize the query
    // For simplicity, assume the query is in the format: "SELECT column1, column2, ... FROM table_name"
    std::vector<std::string> tokens;
    size_t pos = query.find("SELECT") + 6; // Skip "SELECT" keyword
    std::string columns = query.substr(pos, query.find("FROM") - pos); // Extract column names
    std::string tableName = query.substr(query.find("FROM") + 4); // Extract table name
    size_t tokenStart = 0;
    while ((pos = columns.find(",", tokenStart)) != std::string::npos) {
        tokens.push_back(columns.substr(tokenStart, pos - tokenStart));
        tokenStart = pos + 1;
    }
    tokens.push_back(columns.substr(tokenStart)); // Add last column

    // Execute the query and retrieve the result set
    std::vector<Row> resultSet;
    for (const auto& row : data) {
        if (tableName == "employees") { // Assuming data is stored in the "employees" table
            Row newRow;
            for (const auto& column : tokens) {
                newRow.values[newRow.table->columns[column]] = row.values[newRow.table->columns[column]];
            }
            resultSet.push_back(newRow);
        }
    }

    // Return the result set
    return resultSet;
}

// Function to perform an INNER JOIN operation between two tables
std::vector<Row> executeInnerJoin(const std::vector<Row>& table1, const std::vector<Row>& table2, const std::string& condition) {
    // Extract the join condition
    std::string joinColumn1 = condition.substr(0, condition.find('='));
    std::string joinColumn2 = condition.substr(condition.find('=') + 1);

    // Perform the INNER JOIN operation
    std::vector<Row> result;
    for (const auto& row1 : table1) {
        for (const auto& row2 : table2) {
            if (row1.columns.at(joinColumn1) == row2.columns.at(joinColumn2)) {
                Row newRow;
                newRow.columns.insert(row1.columns.begin(), row1.columns.end());
                newRow.columns.insert(row2.columns.begin(), row2.columns.end());
                result.push_back(newRow);
            }
        }
    }

    // Return the result set
    return result;
}

// Function to perform a LEFT JOIN operation between two tables
std::vector<Row> executeLeftJoin(const std::vector<Row>& table1, const std::vector<Row>& table2, const std::string& condition) {
    // Extract the join condition
    std::string joinColumn1 = condition.substr(0, condition.find('='));
    std::string joinColumn2 = condition.substr(condition.find('=') + 1);

    // Perform the LEFT JOIN operation
    std::vector<Row> result;
    for (const auto& row1 : table1) {
        bool matched = false;
        for (const auto& row2 : table2) {
            if (row1.columns.at(joinColumn1) == row2.columns.at(joinColumn2)) {
                Row newRow;
                newRow.columns.insert(row1.columns.begin(), row1.columns.end());
                newRow.columns.insert(row2.columns.begin(), row2.columns.end());
                result.push_back(newRow);
                matched = true;
            }
        }
        if (!matched) {
            Row newRow;
            newRow.columns.insert(row1.columns.begin(), row1.columns.end());
            result.push_back(newRow);
        }
    }

    // Return the result set
    return result;
}

// Function to perform a RIGHT JOIN operation between two tables
std::vector<Row> executeRightJoin(const std::vector<Row>& table1, const std::vector<Row>& table2, const std::string& condition) {
    // Extract the join condition
    std::string joinColumn1 = condition.substr(0, condition.find('='));
    std::string joinColumn2 = condition.substr(condition.find('=') + 1);

    // Perform the RIGHT JOIN operation
    std::vector<Row> result;
    for (const auto& row2 : table2) {
        bool matched = false;
        for (const auto& row1 : table1) {
            if (row1.columns.at(joinColumn1) == row2.columns.at(joinColumn2)) {
                Row newRow;
                newRow.columns.insert(row1.columns.begin(), row1.columns.end());
                newRow.columns.insert(row2.columns.begin(), row2.columns.end());
                result.push_back(newRow);
                matched = true;
            }
        }
        if (!matched) {
            Row newRow;
            newRow.columns.insert(row2.columns.begin(), row2.columns.end());
            result.push_back(newRow);
        }
    }

    // Return the result set
    return result;
}

// Function to perform a FULL OUTER JOIN operation between two tables
std::vector<Row> executeOuterJoin(const std::vector<Row>& table1, const std::vector<Row>& table2, const std::string& condition) {
    // Extract the join condition
    std::string joinColumn1 = condition.substr(0, condition.find('='));
    std::string joinColumn2 = condition.substr(condition.find('=') + 1);

    // Perform the OUTER JOIN operation
    std::vector<Row> result;
    std::unordered_set<std::string> matchedIds;
    
    // Join rows from table1 with matching rows from table2
    for (const auto& row1 : table1) {
        bool matched = false;
        for (const auto& row2 : table2) {
            if (row1.columns.at(joinColumn1) == row2.columns.at(joinColumn2)) {
                Row newRow;
                newRow.columns.insert(row1.columns.begin(), row1.columns.end());
                newRow.columns.insert(row2.columns.begin(), row2.columns.end());
                result.push_back(newRow);
                matchedIds.insert(row2.columns.at(joinColumn2));
                matched = true;
            }
        }
        if (!matched) {
            Row newRow;
            newRow.columns.insert(row1.columns.begin(), row1.columns.end());
            result.push_back(newRow);
        }
    }

    // Join remaining rows from table2 with unmatched rows from table1
    for (const auto& row2 : table2) {
        if (matchedIds.find(row2.columns.at(joinColumn2)) == matchedIds.end()) {
            Row newRow;
            newRow.columns.insert(row2.columns.begin(), row2.columns.end());
            result.push_back(newRow);
        }
    }

    // Return the result set
    return result;
}

// Function to perform a LEFT OUTER JOIN operation between two tables
std::vector<Row> executeLeftOuterJoin(const std::vector<Row>& table1, const std::vector<Row>& table2, const std::string& condition) {
    // Extract the join condition
    std::string joinColumn1 = condition.substr(0, condition.find('='));
    std::string joinColumn2 = condition.substr(condition.find('=') + 1);

    // Perform the LEFT OUTER JOIN operation
    std::vector<Row> result;
    std::unordered_set<std::string> matchedIds;
    
    // Join rows from table1 with matching rows from table2
    for (const auto& row1 : table1) {
        bool matched = false;
        for (const auto& row2 : table2) {
            if (row1.columns.at(joinColumn1) == row2.columns.at(joinColumn2)) {
                Row newRow;
                newRow.columns.insert(row1.columns.begin(), row1.columns.end());
                newRow.columns.insert(row2.columns.begin(), row2.columns.end());
                result.push_back(newRow);
                matchedIds.insert(row2.columns.at(joinColumn2));
                matched = true;
            }
        }
        if (!matched) {
            Row newRow;
            newRow.columns.insert(row1.columns.begin(), row1.columns.end());
            result.push_back(newRow);
        }
    }

    // Return the result set
    return result;
}

// Function to perform a RIGHT OUTER JOIN operation between two tables
std::vector<Row> executeRightOuterJoin(const std::vector<Row>& table1, const std::vector<Row>& table2, const std::string& condition) {
    // Extract the join condition
    std::string joinColumn1 = condition.substr(0, condition.find('='));
    std::string joinColumn2 = condition.substr(condition.find('=') + 1);

    // Perform the RIGHT OUTER JOIN operation
    std::vector<Row> result;
    std::unordered_set<std::string> matchedIds;
    
    // Join rows from table2 with matching rows from table1
    for (const auto& row2 : table2) {
        bool matched = false;
        for (const auto& row1 : table1) {
            if (row1.columns.at(joinColumn1) == row2.columns.at(joinColumn2)) {
                Row newRow;
                newRow.columns.insert(row1.columns.begin(), row1.columns.end());
                newRow.columns.insert(row2.columns.begin(), row2.columns.end());
                result.push_back(newRow);
                matchedIds.insert(row1.columns.at(joinColumn1));
                matched = true;
            }
        }
        if (!matched) {
            Row newRow;
            newRow.columns.insert(row2.columns.begin(), row2.columns.end());
            result.push_back(newRow);
        }
    }

    // Return the result set
    return result;
}

// Function to execute a UNION operation between tables
std::vector<Row> executeUnion(const std::vector<Row>& table1, const std::vector<Row>& table2) {
    // Combine the tables
    std::vector<Row> result = table1;
    result.insert(result.end(), table2.begin(), table2.end());

    // Remove duplicate rows
    std::unordered_set<std::map<std::string, std::string>> uniqueRows;
    std::vector<Row> uniqueResult;
    for (const auto& row : result) {
        if (uniqueRows.insert(row.columns).second) {
            uniqueResult.push_back(row);
        }
    }

    // Return the result set
    return uniqueResult;
}


// Implement functions declared in dbms.h
void DBMS::createDatabase(const std::string& name) {
    databases[name] = Database{name};
}

void DBMS::createTable(const std::string& dbName, const std::string& tableName, const std::vector<std::string>& columns) {
    databases[dbName].tables[tableName] = Table{tableName, columns, {}};
}

void DBMS::executeQuery(const std::string& query) {
    // Parse and execute the query
    // Example: SELECT * FROM table_name;
    // Example: INSERT INTO table_name (column1, column2) VALUES (value1, value2);
    // Example: JOIN...
    // Example: UNION...
}