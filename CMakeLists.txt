cmake_minimum_required(VERSION 3.10)
project(DBMSTest)

# Locate Google Test package
find_package(GTest REQUIRED)

# Include directories
include_directories(${GTEST_INCLUDE_DIRS} include)

# Add sources
set(SOURCES src/dbms.cpp test/dbms_test.cpp)

# Create executable for tests
add_executable(${PROJECT_NAME} ${SOURCES})

# Link against Google Test and pthread library (for Unix systems)
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)

# Add test target
enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})