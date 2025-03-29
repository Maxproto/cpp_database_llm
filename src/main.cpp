/**
 * @file main.cpp
 * @brief Main entry point that ties together database functions and LLM interactions
 *        for a multi-turn conversation application.
 *
 * The user:
 *  1) Is assigned an API key read from a local file (openai_api_key.txt).
 *  2) Enters a username.
 *  3) Enters messages until typing "quit".
 * The conversation is stored in SQLite (chat_history.db).
 */

 #include <iostream>
 #include <fstream>
 #include <string>
 #include "database.h"
 #include "llm.h"
 
 static std::string readAPIKeyFromFile() {
    std::ifstream ifs;

    // Try current directory
    ifs.open("openai_api_key.txt");
    if (!ifs.is_open()) {
        // Try parent directory (project root)
        ifs.open("../openai_api_key.txt");
    }

    if (!ifs.is_open()) {
        std::cerr << "Could not find openai_api_key.txt in current or parent directory.\n";
        return "";
    }

    std::string key;
    std::getline(ifs, key);
    ifs.close();
    return key;
}

 
 int main() {
     std::cout << "=====================================\n";
     std::cout << "  C++ Database + LLM Demo (Improved)\n";
     std::cout << "=====================================\n\n";
 
     // 1. Read API Key from local file
     std::string api_key = readAPIKeyFromFile("openai_api_key.txt");
     if (api_key.empty()) {
         std::cerr << "No API key found. Please put your key in openai_api_key.txt\n";
         return 1;
     }
 
     // 2. Open or create the database
     sqlite3* db = nullptr;
     if (!initDatabase(db, "chat_history.db")) {
         return 1;
     }
 
     // 3. Ask for a username
     std::cout << "Enter your username: ";
     std::string username;
     std::getline(std::cin, username);
     if (username.empty()) {
         std::cerr << "No username provided. Exiting...\n";
         sqlite3_close(db);
         return 1;
     }
 
     // 4. Create a new session
     int session_id = createSession(db);
     if (session_id < 0) {
         std::cerr << "Unable to create session. Exiting...\n";
         sqlite3_close(db);
         return 1;
     }
     std::cout << "[DB] Session created (ID = " << session_id << ")\n\n";
 
     // 5. Multi-turn conversation loop
     while (true) {
         std::cout << username << ": ";
         std::string user_input;
         std::getline(std::cin, user_input);
 
         // Check for quit
         if (user_input == "quit") {
             std::cout << "Ending conversation. Goodbye!\n";
             break;
         }
 
         // Insert user's message
         if (!insertMessage(db, username, "user", user_input, session_id)) {
             std::cerr << "Failed to insert user message.\n";
             break;
         }
 
         // Retrieve conversation so far
         std::vector<Message> convo = getMessagesBySession(db, session_id);
 
         // Ask the LLM
         std::string assistant_reply = getChatGPTResponse(api_key, convo);
         if (assistant_reply.empty()) {
             std::cerr << "Received empty reply from the assistant.\n";
             break;
         }
 
         // Insert assistant's reply
         if (!insertMessage(db, username, "assistant", assistant_reply, session_id)) {
             std::cerr << "Failed to insert assistant message.\n";
             break;
         }
 
         // Print assistant's reply
         std::cout << "Assistant: " << assistant_reply << "\n\n";
     }
 
     // 6. Clean up
     sqlite3_close(db);
     return 0;
 }
 