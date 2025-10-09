
#pragma once
#include <SQLiteCpp/SQLiteCpp.h>

#include <optional>
#include <string>

namespace ctier
{
    class UserDatabase
    {
      public:
        UserDatabase(const std::string& db_path);
        bool initialize();
        bool validate_user(const std::string& username, const std::string& password);
        bool create_user(const std::string& username, const std::string& password);

      private:
        SQLite::Database db;
    };

}  // namespace ctier
