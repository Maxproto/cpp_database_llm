cmake_minimum_required(VERSION 3.10)
project(CPPDatabaseLLM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(cpr REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/database.cpp
    src/database.h
    src/llm.cpp
    src/llm.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SQLite3_INCLUDE_DIRS}
    ${nlohmann_json_INCLUDE_DIRS}
    ${cpr_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${SQLite3_LIBRARIES}
    nlohmann_json::nlohmann_json
    cpr::cpr
)
