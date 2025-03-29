/**
 * @file llm.h
 * @brief Declarations for interfacing with the OpenAI ChatGPT API using cpr and nlohmann/json.
 *
 * This header defines a function that sends a multi-turn conversation context to ChatGPT
 * and retrieves the assistant's reply.
 */

 #ifndef LLM_H
 #define LLM_H
 
 #include <string>
 #include <vector>
 #include "database.h"
 
 /**
  * @brief Send the entire conversation (list of messages) to OpenAI ChatGPT and get a reply.
  *
  * @param api_key The OpenAI API key.
  * @param messages A vector of Message structs representing the conversation so far.
  *                 Each Message should have role = "user" or "assistant".
  * @return The assistant's response content (empty string if there's an error).
  */
 std::string getChatGPTResponse(const std::string& api_key,
                                const std::vector<Message>& messages);
 
 #endif
 