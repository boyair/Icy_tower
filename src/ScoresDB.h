#pragma once
#include <cstdint>
#include <exception>
#include <iterator>
#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <vector>
class ScoreDB {
public:
  ScoreDB(std::string file);
  void AddScore(std::string name, int score); // fails when name exists
  void DeleteScore(std::string name);
  void Clear();
  void LoadCache();

  int GetScore(std::string name);
  std::pair<std::string, int>
  TopScore(int place); // returns the place(th) hight score (place in range
                       // 1 -> rowcount)
  const std::vector<std::pair<std::string, int>> &GetCache() const;
  int ScoreCount();

  void Process(int (*callback)(void *, int, char **, char **), void *data);
  ~ScoreDB();

private:
  sqlite3 *db = nullptr;
  sqlite3_stmt *CompileStatement(std::string statement);
  std::vector<std::pair<std::string, int>> cache;
};

class DBException : public std::exception {
  std::string message;

public:
  DBException(std::string message = "Data base exception") : message(message) {}
  const char *what() const noexcept override { return message.c_str(); }
};
