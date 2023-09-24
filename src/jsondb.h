/**
 * @author hatimsue
 * @sa https://github.com/hatimsue/jsondb
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
#ifndef SRC_JSONDB_H_
#define SRC_JSONDB_H_

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// #include "nlohmann/json.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
/**
 * @namespace jsondb
 * @brief Namespace for the JSON Database Library.
 *
 * The `jsondb` namespace contains classes and functions for working with
 * JSON-based databases.
 *
 * @note This namespace is part of the JSON Database Library (jsondb).
 *
 * @author hatimsue
 * @date 2023-09-09
 */
namespace jsondb {
/**
 * @brief Global constant defining the default database path.
 */
extern const char DEFAULT_DBPATH[];

/**
 * @brief Performs a generic comparison between two JSON values.
 *
 * This function allows you to compare two JSON values generically
 * using a specified comparison operator.
 *
 * @param comparisonOperator The comparison operator (e.g., "eq" for equality).
 * @param documentValue The first JSON value to compare.
 * @param queryValue The second JSON value to compare.
 *
 * @return true if the comparison is successful and the result is true,
 * false otherwise.
 *
 * @note This function is capable of handling JSON values of different
 * types, such as strings, numbers, booleans, and nulls.
 * @warning Data types must be compatible with the specified comparison
 * operator to obtain meaningful results.
 */
bool generic_comparator(std::string comparisonOperator, nlohmann::json documentValue, nlohmann::json queryValue);

/**
 * @brief Matches a JSON document against a query.
 *
 * This function performs a comparison between a JSON document and a query JSON,
 * determining if the document matches the query based on specific criteria.
 *
 * @param document A JSON document to be compared against the query.
 * @param query A JSON query specifying the criteria for matching the document.
 *
 * @return true if the document matches the query, false otherwise.
 *
 * @note This function is designed to compare JSON documents against
 * query criteria and return a boolean result indicating whether the
 * document meets the specified criteria.
 */
bool match_query(const nlohmann::json &document, const nlohmann::json &query);

/**
 * @brief Class for managing JSON databases.
 */
class JsonDB {
   private:
    std::string name;
    std::string path;
    std::string file;
    std::string tablesPath;

   public:
    /**
     * @brief Constructor for JsonDB class.
     * @param dbName The name of the database.
     * @param dbPath The path to the database (default is "./data").
     */
    JsonDB(const std::string &dbName,
           const std::string &dbPath = DEFAULT_DBPATH);

    /**
     * @brief Destructor for JsonDB class.
     */
    ~JsonDB();

    /**
     * @brief Get the name of the database.
     * @return The name of the database.
     */
    std::string GetName() const;

    /**
     * @brief Get the path to the database.
     * @return The path to the database.
     */
    std::string GetPath() const;

    /**
     * @brief Get the filename of the database.
     * @return The filename of the database.
     */
    std::string GetFile() const;

    /**
     * @brief Get the path of tables files.
     * @return The path of the tables files.
     */
    std::string GetTablesPath() const;

    /**
     * @brief Create a new database.
     * @return True if the database is created successfully, false otherwise.
     */
    bool Create();

    /**
     * @brief Check if the database exists.
     * @return True if the database exists, false otherwise.
     */
    bool Exists();

    /**
     * @brief Drop (delete) the database.
     * @return True if the database is successfully dropped, false otherwise.
     */
    bool Drop();

    /**
     * @brief Get a vector of tables in the database.
     * @return A vector of table in the database.
     */
    std::vector<std::unordered_map<std::string, std::string>> Tables();
};

/**
 * @brief Class for managing database tables.
 */
class Table {
   private:
    std::string name;
    const JsonDB &db;
    std::string file;

   public:
    /**
     * @brief Constructor for Table class.
     * @param tableName The name of the table.
     * @param db A reference to the associated JsonDB instance.
     */
    Table(const std::string &tableName, const JsonDB &db);

    /**
     * @brief Destructor for Table class.
     */
    ~Table();

    /**
     * @brief Get the name of the table.
     * @return The name of the table.
     */
    std::string GetName() const;

    /**
     * @brief Create a new table.
     * @return True if the table is created successfully, false otherwise.
     */
    bool Create();

    /**
     * @brief Check if the table exists.
     * @return True if the table exists, false otherwise.
     */
    bool Exists();

    /**
     * @brief Drop (delete) the table.
     * @return True if the table is successfully dropped, false otherwise.
     */
    bool Drop();

    /**
     * @brief Insert a JSON document into the table.
     * @param document The JSON document to insert.
     * @return True if the document is inserted successfully, false otherwise.
     */
    bool InsertDocument(const nlohmann::json &document);

    /**
     * @brief Find the first JSON document in the table.
     * @return The found JSON document or an empty JSON object if not found.
     */
    nlohmann::json FindDocument();

    /**
     * @brief Find the first JSON document in the table that match the filter.
     * @return The found JSON document or an empty JSON object if not found.
     */
    nlohmann::json FindDocument(const nlohmann::json &query);

    /**
     * @brief Find multiple JSON documents in the table.
     * @return A JSON array containing the found documents.
     */
    nlohmann::json FindDocuments();

   /**
     * @brief Find multiple JSON documents in the table.
     * @return A JSON array containing the found documents.
     */
    nlohmann::json FindDocuments(const nlohmann::json &query);

    /**
     * @brief Update a JSON document in the table.
     * @return True if the document is updated successfully, false otherwise.
     */
    bool UpdateDocument(const nlohmann::json &valuesToUpdate, const nlohmann::json &query);

    /**
     * @brief Update a JSON document in the table.
     * @return True if the document is updated successfully, false otherwise.
     */
    bool UpdateDocuments(const nlohmann::json &valuesToUpdate, const nlohmann::json &query);

    /**
     * @brief Delete a JSON document from the table.
     * @return True if the document is deleted successfully, false otherwise.
     */
    bool DeleteDocument(const nlohmann::json &query);

    /**
     * @brief Delete a JSON document from the table.
     * @return True if the document is deleted successfully, false otherwise.
     */
    bool DeleteDocuments(const nlohmann::json &query);

};

/**
 * @brief A utility class for performing various comparison operations on generic types.
 * 
 * This class provides static methods for performing common comparison operations
 * such as equality, greater than, greater than or equal to, less than, less than or
 * equal to, and membership check in a vector.
 * 
 * @tparam T The data type to perform comparisons on.
 */
template <typename T>
class ComparisonUtils {
   public:
    /**
     * @brief Check if two values are equal.
     * 
     * @param val1 The first value.
     * @param val2 The second value.
     * @return True if val1 is equal to val2, false otherwise.
     */
    static bool Eq(const T &val1, const T &val2);

    /**
     * @brief Check if the first value is greater than the second value.
     * 
     * @param val1 The first value.
     * @param val2 The second value.
     * @return True if val1 is greater than val2, false otherwise.
     */
    static bool Gt(const T &val1, const T &val2);

    /**
     * @brief Check if the first value is greater than or equal to the second value.
     * 
     * @param val1 The first value.
     * @param val2 The second value.
     * @return True if val1 is greater than or equal to val2, false otherwise.
     */
    static bool Gte(const T &val1, const T &val2);

    /**
     * @brief Check if a value is present in a vector.
     * 
     * @param val The value to check for.
     * @param arr The vector to search in.
     * @return True if val is found in arr, false otherwise.
     */
    static bool In(const T &val, const std::vector<T> &arr);

    /**
     * @brief Check if the first value is less than the second value.
     * 
     * @param val1 The first value.
     * @param val2 The second value.
     * @return True if val1 is less than val2, false otherwise.
     */
    static bool Lt(const T &val1, const T &val2);

    /**
     * @brief Check if the first value is less than or equal to the second value.
     * 
     * @param val1 The first value.
     * @param val2 The second value.
     * @return True if val1 is less than or equal to val2, false otherwise.
     */
    static bool Lte(const T &val1, const T &val2);

    /**
     * @brief Check if a value is not present in a vector.
     * 
     * @param val The value to check for.
     * @param arr The vector to search in.
     * @return True if val is not found in arr, false otherwise.
     */
    static bool Ne(const T &val, const std::vector<T> &arr);
};

}  // namespace jsondb

namespace jsondb {
template <typename T>
bool ComparisonUtils<T>::Eq(const T &val1, const T &val2) {
    return val1 == val2;
}
template <typename T>
bool ComparisonUtils<T>::Gt(const T &val1, const T &val2) {
    return val1 > val2;
}
template <typename T>
bool ComparisonUtils<T>::Gte(const T &val1, const T &val2) {
    return val1 >= val2;
}
template <typename T>
bool ComparisonUtils<T>::In(const T &val, const std::vector<T> &arr) {
    return (std::find(arr.begin(), arr.end(), val) != arr.end());
}
template <typename T>
bool ComparisonUtils<T>::Lt(const T &val1, const T &val2) {
    return val1 < val2;
}
template <typename T>
bool ComparisonUtils<T>::Lte(const T &val1, const T &val2) {
    return val1 <= val2;
}
template <typename T>
bool ComparisonUtils<T>::Ne(const T &val, const std::vector<T> &arr) {
    return (std::find(arr.begin(), arr.end(), val) == arr.end());
}

/**
 * @brief Compare two values using a specified logical operator.
 * 
 * This function allows you to compare two values using a logical operator
 * specified by the `logicOperator` parameter. Supported operators include:
 *   - "$eq": Equal to
 *   - "$gt": Greater than
 *   - "$gte": Greater than or equal to
 *   - "$lt": Less than
 *   - "$lte": Less than or equal to
 * 
 * @tparam T The data type of the values to compare.
 * @param logicOperator The logical operator to use for comparison.
 * @param val1 The first value.
 * @param val2 The second value (for comparison operators other than "$in").
 * @return True if the comparison specified by the logical operator is true,
 *         false otherwise.
 *
 */
template <typename T>
bool compare(const std::string &logicOperator, const T &val1, const T &val2) {
    using ComparisonFunction = std::function<bool(const T &, const T &)>;
    std::unordered_map<std::string, ComparisonFunction> ops(
        {{"$eq", ComparisonUtils<T>::Eq},
         {"$gt", ComparisonUtils<T>::Gt},
         {"$gte", ComparisonUtils<T>::Gte},
         {"$lt", ComparisonUtils<T>::Lt},
         {"$lte", ComparisonUtils<T>::Lte}});
    return ops[logicOperator](val1, val2);
}

template <typename T>
bool compare(const std::string &logicOperator, const T &val1, const std::vector<T> &val2) {
    using ComparisonFunction = std::function<bool(const T &, const std::vector<T> &)>;
    std::unordered_map<std::string, ComparisonFunction> ops(
        {{"$in", ComparisonUtils<T>::In},
         {"$ne", ComparisonUtils<T>::Ne}});
    return ops[logicOperator](val1, val2);
}

}  // namespace jsondb

#endif // SRC_JSONDB_H_
