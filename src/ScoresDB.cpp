#include "ScoresDB.h"
#include <cstdint>
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <utility>
int score_capture;
ScoreDB::ScoreDB(std::string file) {
  sqlite3_open(file.c_str(), &db);
  if (!db)
    throw "failed to find or create data base file";

  std::string create_table_statement = "CREATE TABLE IF NOT EXISTS USERSCORES(\
    NAME TEXT UNIQUE NOT NULL, \
    SCORE INTEGER NOT NULL);";
  char *error_message = nullptr;
  sqlite3_exec(db, create_table_statement.c_str(), 0, 0, &error_message);
  if (error_message) {
    std::cerr << error_message << std::endl;
    throw error_message;
  }
}
void ScoreDB::AddScore(std::string name, int score) {
  sqlite3_stmt *stmt =
      CompileStatement("INSERT INTO USERSCORES (NAME, SCORE) VALUES (?, ?);");
  // Bind parameters
  sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 2, score);
  // Execute the statement
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cerr << "Error executing statement: " << sqlite3_errmsg(db)
              << std::endl;
  }

  sqlite3_finalize(stmt);
}
void ScoreDB::DeleteScore(std::string name) {
  sqlite3_stmt *stmt =
      CompileStatement("DELETE FROM USERSCORES WHERE NAME = ?;");
  sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
  // Execute the statement
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cerr << "Error executing statement: " << sqlite3_errmsg(db)
              << std::endl;
  }
  sqlite3_finalize(stmt);
}
void ScoreDB::Clear() {
  std::string statement = "DELETE FROM USERSCORES;";
  char *error_message;
  sqlite3_exec(db, statement.c_str(), 0, 0, &error_message);
  if (error_message) {
    std::cerr << error_message << std::endl;
    throw error_message;
  }
}

int ScoreDB::GetScore(std::string name) {
  sqlite3_stmt *stmt =
      CompileStatement("SELECT SCORE FROM USERSCORES WHERE NAME = ?;");
  sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
  // Execute the statement
  int score = 0;
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    return sqlite3_column_int(stmt, 0);
  }
  sqlite3_finalize(stmt);
  throw std::invalid_argument("name " + name +
                              " could not be found in data base");
  return 0;
}
std::pair<std::string, int> ScoreDB::TopScore(int place) {
  if (place == 0) // first place is number one therefor 0 is meaningless
    place = 1;
  sqlite3_stmt *stmt = CompileStatement(
      "SELECT NAME,SCORE FROM USERSCORES ORDER BY SCORE DESC LIMIT ?;");
  sqlite3_bind_int(stmt, 1, place);
  // Execute the statement
  std::string name = "";
  int score = 0;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    name = std::string((char *)sqlite3_column_text(stmt, 0));
    score = sqlite3_column_int(stmt, 1);
  }

  sqlite3_finalize(stmt);
  return {name, score};
}

int ScoreDB::ScoreCount() {

  sqlite3_stmt *stmt = CompileStatement("SELECT COUNT(*) FROM USERSCORES;");
  // Execute the statement
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    return sqlite3_column_int(stmt, 0);
  }
  sqlite3_finalize(stmt);
  return 0;
}

void ScoreDB::Process(int (*callback)(void *, int, char **, char **),
                      void *data) {

  std::string statement =
      "SELECT NAME,SCORE FROM USERSCORES ORDER BY SCORE DESC;";
  char *error_message = nullptr;
  sqlite3_exec(db, statement.c_str(), callback, data, &error_message);
  if (error_message) {
    std::cerr << error_message << std::endl;
    throw error_message;
  }
}

ScoreDB::~ScoreDB() { sqlite3_close(db); }

// optional learn variadic templates to allow adding binds in the function
// parameters
//  (https://www.youtube.com/watch?v=j1qpRgUhXtk&pp=ygUXdmFyYWlhZGljIHRhbXBsYXRlcyBjKys%3D)
sqlite3_stmt *ScoreDB::CompileStatement(std::string statement) {
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, statement.c_str(), -1, &stmt, nullptr) !=
      SQLITE_OK) {
    throw DBException("failed to compile statement: " + statement +
                      "\n error: " + sqlite3_errmsg(db));
  }
  return stmt;
}
