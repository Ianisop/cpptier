#include "database.h"

#include <string.h>

#include <iostream>
#include "SQLiteCpp/Statement.h"
namespace ctier
{

    UserDatabase::UserDatabase(const std::string& db_path) : db(db_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
    {
    }

    bool UserDatabase::initialize()
    {
        try
        {
            db.exec(R"(
                CREATE TABLE IF NOT EXISTS users (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    username TEXT UNIQUE NOT NULL,
                    password_hash TEXT NOT NULL,
                    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
                )
            )");
            return true;
        }
        catch (std::exception& e)
        {
            std::cerr << "Database initialization error: " << e.what() << std::endl;
            return false;
        }
    }

    bool UserDatabase::create_user(const std::string& username, const std::string& password)
    {
        SQLite::Statement query(db,"INSERT into users VALUES (?,?)");
        query.bind(1,username);
        query.bind(2,password);
        if(query.executeStep())
        {
            return true;
        }
        return false;
    }

    bool UserDatabase::validate_user(const std::string& username, const std::string& password)
    {
        try
        {
            SQLite::Statement query(db, "SELECT password_hash FROM users WHERE username = ?");
            query.bind(1, username);

            if (query.executeStep())
            {
                std::string stored_hash = query.getColumn(0).getString();
                // TODO: Use proper password hashing in production!
                return password == stored_hash;  // Simple comparison for demo
            }
            return false;
        }
        catch (std::exception& e)
        {
            std::cerr << "Validation error: " << e.what() << std::endl;
            return false;
        }
    }
}  // namespace ctier
