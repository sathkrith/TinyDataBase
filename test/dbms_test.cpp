#include <gtest/gtest.h>
#include "dbms.hpp"

// Define a fixture for testing DBMS functionality
class DBMSTest : public ::testing::Test {
protected:
    DBMS dbms;

    // Set up common objects and actions needed for the tests
    void SetUp() override {
        // You can add common setup code here, if any
    }

    // Clean up any resources allocated in the SetUp function
    void TearDown() override {
        // You can add common teardown code here, if any
    }
};

// Test case for creating a database
TEST_F(DBMSTest, CreateDatabase) {
    dbms.createDatabase("test_db");
    ASSERT_TRUE(dbms.databases.find("test_db") != dbms.databases.end());
}

// Test case for creating a table
TEST_F(DBMSTest, CreateTable) {
    dbms.createDatabase("test_db");
    dbms.createTable("test_db", "test_table", {"column1", "column2"});
    ASSERT_TRUE(dbms.databases["test_db"].tables.find("test_table") != dbms.databases["test_db"].tables.end());
}

// Test case for executing a query
TEST_F(DBMSTest, ExecuteQuery) {
    dbms.createDatabase("test_db");
    dbms.createTable("test_db", "test_table", {"column1", "column2"});
    // Add more test cases for executing queries
}

// Entry point for running the tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}