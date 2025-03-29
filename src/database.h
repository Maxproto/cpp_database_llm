/**
 * @file database.h
 * @brief Declarations for database management using SQLite3 (prepared statements).
 *
 * This header provides functions for initializing the database,
 * creating tables, and inserting/retrieving conversation messages
 * using safer parameter binding.
 */

 #ifndef DATABASE_H
 #define DATABASE_H
 
 #include <string>
 #include <vector>
 #include <sqlite3.h>
 
 /**
  * @struct Message
  * @brief Holds data for a chat message record in the database.
  */
 struct Message {
     int id;
     std::string username;
     std::string role; // "user" or "assistant"
     std::string content;
     std::string timestamp;
     int session_id;
 };
 
 /**
  * @brief Initialize the database and create necessary tables if not exist.
  * 
  * @param db A pointer to an SQLite3 instance pointer (will be allocated).
  * @param db_name The filename for the SQLite database.
  * @return True on success, false on failure.
  */
 bool initDatabase(sqlite3*& db, const std::string& db_name);
 
 /**
  * @brief Create a new session, returning the session's ID.
  * 
  * @param db The SQLite3 database pointer.
  * @return The newly created session ID, or -1 on error.
  */
 int createSession(sqlite3* db);
 
 /**
  * @brief Insert a new message into the database using prepared statements.
  * 
  * @param db The SQLite3 database pointer.
  * @param username The username for the message.
  * @param role The role ("user" or "assistant").
  * @param content The text content of the message.
  * @param session_id The session ID this message belongs to.
  * @return True on success, false on failure.
  */
 bool insertMessage(sqlite3* db,
                    const std::string& username,
                    const std::string& role,
                    const std::string& content,
                    int session_id);
 
 /**
  * @brief Retrieve all messages for a given session ID, sorted by ID ascending.
  * 
  * @param db The SQLite3 database pointer.
  * @param session_id The session ID.
  * @return A vector of Message structs for that session.
  */
 std::vector<Message> getMessagesBySession(sqlite3* db, int session_id);
 
 #endif
 