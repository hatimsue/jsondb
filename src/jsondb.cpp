/**
 * @author hatimsue
 * @sa https://github.com/hatimsue/j
 *
 * MIT License

 * Copyright (c) 2023 @hatimsue
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "jsondb.h"

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

namespace jsondb {
const char DEFAULT_DBPATH[] = "./data";

//TODO: improve the comparison algorith
bool generic_comparator(std::string op, nlohmann::json  documentValue, nlohmann::json queryValue){
    bool result = false;
    switch (documentValue.type()) {
        case nlohmann::json::value_t::string:
            if(queryValue.is_string()){
                result = compare(op, (std::string) documentValue, (std::string) queryValue);
            }else if(queryValue.is_array()){
                result =compare(op, (std::string) documentValue, (std::vector<std::string>) queryValue);
            }
            break;
        case nlohmann::json::value_t::number_integer:
        case nlohmann::json::value_t::number_unsigned:
        case nlohmann::json::value_t::number_float:
            if(queryValue.is_number()){
                result = compare(op, (float) documentValue, (float) queryValue);
            }else if(queryValue.is_array()){
                result = compare(op, (float) documentValue, (std::vector<float>) queryValue);
            }
            break;
        case nlohmann::json::value_t::boolean:
            if(queryValue.is_boolean()){
                result = compare(op, (bool) documentValue, (bool) queryValue);
            }
            break;
        case nlohmann::json::value_t::null:
            if(queryValue.is_null()){
                result = compare(op, documentValue, queryValue);
            }
            break;
        default:
            result = false;
    }
    return result;
}
bool match_query(const nlohmann::json &document, const nlohmann::json &query) {
    nlohmann::json emptyQuery = {};
    if(emptyQuery == query){
        return true;
    }
    bool result = true;
    for (auto q = query.begin(); q != query.end(); ++q) {
        std::string column = q.key();
        if (document.find(column) != document.end()) {
            nlohmann::json columnStatements = query.at(column);
            for (auto s = columnStatements.begin(); s != columnStatements.end();
                 ++s) {
                try{
                    std::string statement = s.key();
                    auto propertyValue = document.at(column);
                    auto queryValue = columnStatements.at(statement);
                    result = result && generic_comparator(statement, propertyValue, queryValue);
                } catch (std::exception &e){
                    std::cerr << "Error in comparison: " << e.what() << std::endl;
                    return false;
                }
                if (result == false) {
                    return false;
                }
            }
        } else {
            return false;
        }
    }
    return result;
}
JsonDB::~JsonDB() = default;
JsonDB::JsonDB(const std::string &dbName, const std::string &dbPath)
    : name(dbName),
      path(dbPath),
      file(dbPath + "/" + dbName + ".json"),
      tablesPath(dbPath + "/" + dbName) {}

std::string JsonDB::GetFile() const { return file; }
std::string JsonDB::GetName() const { return name; }
std::string JsonDB::GetPath() const { return path; }
std::string JsonDB::GetTablesPath() const { return tablesPath; }

bool JsonDB::Create() {
    if(this->Exists()){
        return false;
    }
    try {
        nlohmann::json db;
        db["name"] = name;
        db["tables"] = {};

        std::filesystem::create_directory(path);
        std::filesystem::create_directory(tablesPath);
        std::ofstream dbFile(file);
        dbFile << db << std::endl;
        return true;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool JsonDB::Exists() {
    std::ifstream dbFile(file);
    if (dbFile.good()) {
        dbFile.close();
        return true;
    } else {
        return false;
    }
}

bool JsonDB::Drop() {
    try {
        if (std::filesystem::exists(file)) {
            std::filesystem::remove(file);
            std::cout << "Database file deleted: " << file << std::endl;
        } else {
            throw std::runtime_error("The database file does not exist: " + file);
        }

        if (std::filesystem::exists(tablesPath)) {
            std::filesystem::remove_all(tablesPath);
            std::cout << "Database folder deleted: " << tablesPath << std::endl;
        } else {
            throw std::runtime_error("The database folder does not exist: " + tablesPath);
        }

        return true;
    } catch (const std::exception &e) {
        std::cerr << "Error during database deletion: " << e.what()
                  << std::endl;
        return false;
    }
}

std::vector<std::unordered_map<std::string, std::string>> JsonDB::Tables() {
    std::vector<std::unordered_map<std::string, std::string>> tables;
    std::ifstream dbFile(file);

    if (!dbFile.is_open()) {
        throw std::runtime_error("Unable to open database file");
    }

    try {
        nlohmann::json data;
        dbFile >> data;
        if (data["tables"].is_array()) {
            for(auto &tb: data["tables"]){
                tables.push_back(tb);
            }
        } else {
            throw std::runtime_error("Invalid database format: 'tables' not found or not an array");
        }
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("Error parsing database file: " + std::string(e.what()));
    }

    return tables;
}


Table::~Table() = default;
Table::Table(const std::string &tableName, const JsonDB &database)
    : name(tableName),
      db(database),
      file(database.GetTablesPath() + "/" + tableName + ".json") {}

std::string Table::GetName() const { return name; }

bool Table::Create() {
    try {
        nlohmann::json data;
        std::ifstream dbFileIn(db.GetFile());

        if (!dbFileIn.is_open()) {
            throw std::runtime_error("Error opening: " + db.GetFile());
        }

        dbFileIn >> data;
        dbFileIn.close();

        auto tables = data["tables"];

        std::set<std::string> existingTables;
        for (const auto &tb : tables) {
            existingTables.insert(tb["name"]);
        }

        if (existingTables.find(name) != existingTables.end()) {
            throw std::runtime_error("Error: " + name + "'s table already exists");
        }

        nlohmann::json newTable = {{"name", name}};
        tables.push_back(newTable);
        data["tables"] = tables;

        std::ofstream dbFileOut(db.GetFile(), std::ios::out | std::ios::trunc);
        dbFileOut << data << std::endl;

        std::ofstream tbFile(file);
        tbFile << "[]";

        return true;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool Table::Exists() {
    try{
        std::ifstream dbFile(db.GetFile());

        if (!dbFile.is_open()) {
            std::cerr << "Error opening: " + db.GetFile() << std::endl;
            return false;
        }

        nlohmann::json data;
        dbFile >> data;
        auto &tables = data["tables"];
        for (const auto &tb : tables) {
            if (tb["name"] == name) {
                dbFile.close();
                return true;
            }
        }
        return false;
    } catch (const std::exception &e){
        std::cerr << "Error reading JSON data from: " + db.GetFile()
                  << std::endl;
        return false;
    }
}

bool Table::Drop() {
    std::fstream dbFile(db.GetFile(), std::ios::in | std::ios::out);

    if (!dbFile.is_open()) {
        std::cerr << "Error opening: " + db.GetFile() << std::endl;
        return false;
    }

    nlohmann::json data;
    dbFile >> data;
    dbFile.close();
    auto &tables = data["tables"];
    for (auto it = tables.begin(); it != tables.end(); ++it) {
        if ((*it)["name"] == name) {
            tables.erase(it);
            dbFile.open(db.GetFile(),
                        std::ofstream::out | std::ofstream::trunc);
            dbFile.seekp(0, std::ios::beg);
            dbFile << data;
            dbFile.close();

            if (std::filesystem::remove(file)) {
                std::cout << "Table file deleted: " << file << std::endl;
            } else {
                std::cerr << "Error deleting table file: " << file << std::endl;
            }
            return true;
        }
    }

    std::cerr << "Error: table does not exist " + name << std::endl;
    dbFile.close();
    return false;
}
bool Table::InsertDocument(const nlohmann::json &document) {
    try {
        std::fstream tbFile(file, std::ios::in | std::ios::out);

        if (!tbFile.is_open()) {
            throw std::runtime_error("Error opening: " + file);
        }

        nlohmann::json data;
        tbFile >> data;
        data.push_back(document);

        tbFile.close();

        std::ofstream tbFileOut(file,
                                std::ofstream::out | std::ofstream::trunc);
        tbFileOut << data;

        if (!tbFileOut) {
            throw std::runtime_error("Error writing to: " + file);
        }

        tbFileOut.close();
        return true;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}
nlohmann::json Table::FindDocument() {
    try {
        std::ifstream tbFile(file);

        if (!tbFile.is_open()) {
            throw std::runtime_error("Error opening: " + file);
        }

        nlohmann::json data;
        tbFile >> data;
        tbFile.close();
        if (data.size() < 1) {
            return nlohmann::json::object();
        }
        return data[0];
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return nlohmann::json::object();
    }
}
nlohmann::json Table::FindDocument(const nlohmann::json &query) {
    try {
        std::ifstream tbFile(file);

        if (!tbFile.is_open()) {
            throw std::runtime_error("Error opening: " + file);
        }

        nlohmann::json data;
        tbFile >> data;
        tbFile.close();
        if (data.size() < 1) {
            return nlohmann::json::object();
        }
        for (int i = 0; i < data.size(); i++) {
            if (match_query(data[i], query)) {
                return data[i];
            }
        }
        return nlohmann::json::object();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return nlohmann::json::object();
    }
}
nlohmann::json Table::FindDocuments() {
    try {
        std::ifstream tbFile(file);

        if (!tbFile.is_open()) {
            throw std::runtime_error("Error opening: " + file);
        }

        nlohmann::json data;
        tbFile >> data;
        tbFile.close();
        return data;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return nlohmann::json::array();
    }
}
nlohmann::json Table::FindDocuments(const nlohmann::json &query) {
    nlohmann::json result = nlohmann::json::array();
    try {
        std::ifstream tbFile(file);

        nlohmann::json data;
        tbFile >> data;
        tbFile.close();
        if(!data.is_array()||data.size()<1){
            return result;
        }
        for (int i = 0; i < data.size(); i++) {
            if (match_query(data[i], query)) {
                result.push_back(data[i]);
            }
        }

        return result;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return nlohmann::json::array();
    }
}

bool Table::UpdateDocument(const nlohmann::json &valuesToUpdate, const nlohmann::json &query){
    bool result = false;
    try {
        std::ifstream tbFile(file);

        nlohmann::json data;
        tbFile >> data;
        tbFile.close();
        if(!data.is_array()||data.size()<1){
            return false;
        }
        for (int i = 0; i < data.size(); i++) {
            if (match_query(data[i], query)) {
                for (auto q = valuesToUpdate.begin(); q != valuesToUpdate.end(); ++q) {
                    data[i][q.key()]=q.value();
                }
                break;
            }
        }
        std::ofstream tbFileOut(file);
        tbFileOut << data;
        result = true;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return result;
    }
    return result;
}
bool Table::UpdateDocuments(const nlohmann::json &valuesToUpdate, const nlohmann::json &query){
    bool result = false;
    try {
        std::ifstream tbFile(file);

        nlohmann::json data;
        tbFile >> data;
        tbFile.close();
        if(!data.is_array()||data.size()<1){
            return false;
        }
        for (int i = 0; i < data.size(); i++) {
            if (match_query(data[i], query)) {
                for (auto q = valuesToUpdate.begin(); q != valuesToUpdate.end(); ++q) {
                    data[i][q.key()]=q.value();
                }
            }
        }
        std::ofstream tbFileOut(file);
        tbFileOut << data;
        result = true;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return result;
    }
    return result;
}
bool Table::DeleteDocument(const nlohmann::json &query){
    bool result = false;
    try {
        std::ifstream tbFile(file);

        nlohmann::json data;
        tbFile >> data;
        tbFile.close();
        if(!data.is_array()||data.size()<1){
            return false;
        }
        for (int i = 0; i < data.size(); i++) {
            if (match_query(data[i], query)) {
                data.erase(data.begin() + i);
                break;
            }
        }
        std::ofstream tbFileOut(file);
        tbFileOut << data;
        result = true;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return result;
    }
    return result;
}
bool Table::DeleteDocuments(const nlohmann::json &query){
    bool result = false;
    try {
        std::ifstream tbFile(file);

        nlohmann::json data;
        tbFile >> data;
        tbFile.close();
        nlohmann::json copy = data;
        if(!data.is_array()||data.size()<1){
            return false;
        }
        for (int i = 0; i < data.size(); i++) {
            if (match_query(data[i], query)) {
                copy.erase(copy.begin() + i);
            }
        }
        std::ofstream tbFileOut(file);
        tbFileOut << copy;
//        std::cout << copy.dump(4) <<std::endl;
        result = true;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return result;
    }
    return result;

}

}  // namespace jsondb
