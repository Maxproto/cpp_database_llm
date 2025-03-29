/**
 * @file llm.cpp
 * @brief Implementation for interfacing with the OpenAI ChatGPT API using cpr and nlohmann/json.
 */

 #include "llm.h"
 #include <cpr/cpr.h>
 #include <nlohmann/json.hpp>
 #include <iostream>
 
 /**
  * @brief Utility function that converts our Message vector into the JSON format required by ChatGPT.
  */
 static nlohmann::json messagesToJSON(const std::vector<Message>& messages) {
     nlohmann::json j_msgs = nlohmann::json::array();
 
     for (const auto& msg : messages) {
         // The LLM expects roles "user", "assistant", or "system".
         nlohmann::json item;
         item["role"] = msg.role;
         item["content"] = msg.content;
         j_msgs.push_back(item);
     }
 
     return j_msgs;
 }
 
 std::string getChatGPTResponse(const std::string& api_key,
                                const std::vector<Message>& messages) {
     // Construct the request body for ChatGPT
     nlohmann::json request_body;
     request_body["model"] = "gpt-3.5-turbo";
     // Convert local messages to ChatGPT's message format
     request_body["messages"] = messagesToJSON(messages);
 
     try {
         cpr::Response r = cpr::Post(
             cpr::Url{"https://api.openai.com/v1/chat/completions"},
             cpr::Header{
                 {"Content-Type", "application/json"},
                 {"Authorization", "Bearer " + api_key}
             },
             cpr::Body{request_body.dump()}
         );
 
         if (r.status_code == 200) {
             nlohmann::json response_json = nlohmann::json::parse(r.text);
             std::string content = response_json["choices"][0]["message"]["content"];
             return content;
         } else {
             std::cerr << "[LLM] OpenAI API error. HTTP Code: " << r.status_code << "\n"
                       << r.text << std::endl;
         }
     } catch (const std::exception& e) {
         std::cerr << "[LLM] Exception: " << e.what() << std::endl;
     }
 
     return "";
 }
 