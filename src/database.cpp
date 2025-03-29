/**
 * @file database.cpp
 * @brief Implementation of database management using SQLite3 with prepared statements.
 */

 #include "database.h"
 #include <iostream>
 
 bool initDatabase(sqlite3*& db, const std::string& db_name) {
     int rc = sqlite3_open(db_name.c_str(), &db);
     if (rc != SQLITE_OK) {
         std::cerr << "[DB] Error opening database: " << sqlite3_errmsg(db) << std::endl;
         return false;
     }
 
     // Create a "sessions" table to store session IDs.
     // Create a "messages" table for multi-turn conversation.
     // Each message has session_id, role, content, timestamp, etc.
     const char* createTableSQL = R"(
         CREATE TABLE IF NOT EXISTS sessions (
             id INTEGER PRIMARY KEY AUTOINCREMENT,
             created_at DATETIME DEFAULT CURRENT_TIMESTAMP
         );
 
         CREATE TABLE IF NOT EXISTS messages (
             id INTEGER PRIMARY KEY AUTOINCREMENT,
             username TEXT,
             role TEXT,
             content TEXT,
             timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
             session_id INTEGER
         );
     )";
 
     char* errMsg = nullptr;
     rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);
     if (rc != SQLITE_OK) {
         std::cerr << "[DB] Error creating tables: " << errMsg << std::endl;
         sqlite3_free(errMsg);
         return false;
     }
 
     return true;
 }
 
 int createSession(sqlite3* db) {
     const char* sql = "INSERT INTO sessions DEFAULT VALUES;";
     sqlite3_stmt* stmt = nullptr;
 
     int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
     if (rc != SQLITE_OK) {
         std::cerr << "[DB] Error preparing createSession statement: " << sqlite3_errmsg(db) << std::endl;
         return -1;
     }
 
     rc = sqlite3_step(stmt);
     if (rc != SQLITE_DONE) {
         std::cerr << "[DB] Error executing createSession statement: " << sqlite3_errmsg(db) << std::endl;
         sqlite3_finalize(stmt);
         return -1;
     }
 
     sqlite3_finalize(stmt);
     return static_cast<int>(sqlite3_last_insert_rowid(db));
 }
 
 bool insertMessage(sqlite3* db,
                    const std::string& username,
                    const std::string& role,
                    const std::string& content,
                    int session_id) {
     // Use prepared statement with parameter binding to avoid injection.
     const char* sql = R"(
         INSERT INTO messages (username, role, content, session_id)
         VALUES (?, ?, ?, ?);
     )";
 
     sqlite3_stmt* stmt = nullptr;
     int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
     if (rc != SQLITE_OK) {
         std::cerr << "[DB] Error preparing insertMessage: " << sqlite3_errmsg(db) << std::endl;
         return false;
     }
 
     // Bind parameters: index starts at 1
     sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
     sqlite3_bind_text(stmt, 2, role.c_str(), -1, SQLITE_TRANSIENT);
     sqlite3_bind_text(stmt, 3, content.c_str(), -1, SQLITE_TRANSIENT);
     sqlite3_bind_int(stmt, 4, session_id);
 
     rc = sqlite3_step(stmt);
     if (rc != SQLITE_DONE) {
         std::cerr << "[DB] Error executing insertMessage: " << sqlite3_errmsg(db) << std::endl;
         sqlite3_finalize(stmt);
         return false;
     }
 
     sqlite3_finalize(stmt);
     return true;
 }
 
 // Callback approach is replaced by direct step/column reading in prepared statements
 std::vector<Message> getMessagesBySession(sqlite3* db, int session_id) {
     std::vector<Message> result;
     const char* sql = R"(
         SELECT id, username, role, content, timestamp, session_id
         FROM messages
         WHERE session_id = ?
         ORDER BY id ASC;
     )";
 
     sqlite3_stmt* stmt = nullptr;
     int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
     if (rc != SQLITE_OK) {
         std::cerr << "[DB] Error preparing getMessagesBySession: " << sqlite3_errmsg(db) << std::endl;
         return result;
     }
 
     // Bind the session_id parameter
     sqlite3_bind_int(stmt, 1, session_id);
 
     while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
         Message msg;
         msg.id = sqlite3_column_int(stmt, 0);
         if (const unsigned char* text = sqlite3_column_text(stmt, 1)) {
             msg.username = reinterpret_cast<const char*>(text);
         }
         if (const unsigned char* text = sqlite3_column_text(stmt, 2)) {
             msg.role = reinterpret_cast<const char*>(text);
         }
         if (const unsigned char* text = sqlite3_column_text(stmt, 3)) {
             msg.content = reinterpret_cast<const char*>(text);
         }
         if (const unsigned char* text = sqlite3_column_text(stmt, 4)) {
             msg.timestamp = reinterpret_cast<const char*>(text);
         }
         msg.session_id = sqlite3_column_int(stmt, 5);
 
         result.push_back(msg);
     }
 
     if (rc != SQLITE_DONE) {
         std::cerr << "[DB] Error stepping through getMessagesBySession: " << sqlite3_errmsg(db) << std::endl;
     }
 
     sqlite3_finalize(stmt);
     return result;
 }
 