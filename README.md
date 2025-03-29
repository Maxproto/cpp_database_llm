# CPP Database + LLM Interface

This is a SQLite database with the OpenAI (ChatGPT) API using [cpr](https://github.com/libcpr/cpr) and [nlohmann/json](https://github.com/nlohmann/json)

## Requirements

- C++17 or higher
- CMake 3.10 or higher
- [SQLite3](https://www.sqlite.org/index.html)
- [cpr](https://github.com/libcpr/cpr)
- [nlohmann/json](https://github.com/nlohmann/json)
- An OpenAI API key

On macOS (Homebrew):

```bash
brew install sqlite3
brew install cpr
brew install nlohmann-json
```

## Building

Clone this repository:

```bash
git clone https://github.com/yourname/cpp_database_llm.git
cd cpp_database_llm
```

Create a build directory, run CMake, and build:

```bash
mkdir build
cd build
cmake ..
make
```

You will get an executable `CPPDatabaseLLM`.

## Usage

Add your OpenAI API Key to the file `openai_api_key.txt` (in the repo root). For example:

```
sk-xxxxxx
```

Run the executable:

```bash
./CPPDatabaseLLM
```

Enter your username when prompted.

Type messages to ChatGPT. Type `quit` to end the conversation.

The conversation (including timestamps, user messages, assistant replies) is stored in `chat_history.db`.

## Viewing the Database

This project creates a SQLite database file named `chat_history.db` in the same directory as the executable. To explore the data:

Install sqlite3 if not already installed.

Use the command line to open the database:

```bash
sqlite3 chat_history.db
```

Within the SQLite shell, you can run:

```sql
.tables
SELECT * FROM sessions;
SELECT * FROM messages;
```

To quit SQLite:

```sql
.quit
```

## Notes

- For multi-turn conversation, this demo retrieves all previous messages for the current session and sends them to ChatGPT on each user message.
- This code uses prepared statements and parameter binding to reduce security risks.
- The code is still a basic demo and doesn't handle concurrency or large-scale scenarios. Adapt for production usage accordingly.

## License

This sample is provided as-is under the MIT License (or similar). Feel free to adapt it for your own needs.
