#include <gtest/gtest.h>
#include "jsondb.h"

using namespace jsondb;

TEST(MatchQueryTest, SuccessfulMatch) {
    nlohmann::json document = {{"name", "John"}, {"age", 30}, {"city", "bogota"}};
    nlohmann::json query = {{"name", {{"$eq", "John"}}}, {"age", {{"$eq", 30}}}, {"city", {{"$in", {"bogota", "caracas"}}}}};
    EXPECT_TRUE(match_query(document, query));
}

TEST(MatchQueryTest, FailedMatch) {
    nlohmann::json document = {{"name", "Alice"}, {"age", 25}};
    nlohmann::json query = {{"name", {{"$eq", "John"}}}, {"age", {{"$eq", 30}}}};
    EXPECT_FALSE(match_query(document, query));
}

TEST(MatchQueryTest, MissingColumn) {
    nlohmann::json document = {{"name", "Alice"}};
    nlohmann::json query = {{"name", {{"$eq", "Alice"}}}, {"age", {{"$eq", 30}}}};
    EXPECT_FALSE(match_query(document, query));
}

TEST(MatchQueryTest, ExceptionHandling) {
    nlohmann::json document = {{"name", "Alice"}};
    nlohmann::json query = {{"name", {{"$eq", "Alice"}}}, {"age", {{"$eq", "invalid"}}}};
    EXPECT_FALSE(match_query(document, query));
}

TEST(MatchQueryTest, MixedDataTypes) {
    nlohmann::json document = {{"name", "Alice"}, {"age", 25}};
    nlohmann::json query = {{"name", {{"$eq", "Alice"}}}, {"age", {{"$eq", "25"}}}};
    EXPECT_FALSE(match_query(document, query));
}

TEST(MatchQueryTest, EmptyQuery) {
    nlohmann::json document = {{"name", "Alice"}, {"age", 25}};
    nlohmann::json query = {};
    EXPECT_TRUE(match_query(document, query));
}

TEST(MatchQueryTest, EmptyDocument) {
    nlohmann::json document = {};
    nlohmann::json query = {{"name", {{"$eq", "Alice"}}}, {"age", {{"$eq", 25}}}};
    EXPECT_FALSE(match_query(document, query));
}

TEST(MatchQueryTest, ErrorInComparison) {
    nlohmann::json document = {{"name", "Alice"}, {"age", 25}};
    nlohmann::json query = {{"name", {{"$eq", "Alice"}}}, {"age", {{"$eq", "invalid"}}}};
    EXPECT_FALSE(match_query(document, query));
}

TEST(MatchQueryTest, NullQueryDocument) {
    nlohmann::json document = R"({"name": "Alice","age": null})"_json;
    nlohmann::json query= R"({"age": { "$eq": null}})"_json;
    std::cout << document.dump(4) << std::endl;
    std::cout << query.dump(4) << std::endl;
    EXPECT_TRUE(match_query(document, query));
}
TEST(MatchQueryTest, NullDocumentProperty) {
    nlohmann::json document = R"({"name": "Alice","age": null})"_json;
    nlohmann::json query= R"({"age": { "$eq": "null"}})"_json;
    std::cout << document.dump(4) << std::endl;
    std::cout << query.dump(4) << std::endl;
    EXPECT_FALSE(match_query(document, query));
}
TEST(MatchQueryTest, SuccessBoolQuery) {
    nlohmann::json document = R"({"name": "Alice","age": false})"_json;
    nlohmann::json query= R"({"age": { "$eq": false}})"_json;
    EXPECT_TRUE(match_query(document, query));
}
TEST(MatchQueryTest, FailedBoolQuery) {
    nlohmann::json document = R"({"name": "Alice","age": "false"})"_json;
    nlohmann::json query= R"({"age": { "$eq": false}})"_json;
    EXPECT_FALSE(match_query(document, query));
}
TEST(MatchQueryTest, ObjectNotMatch) {
    nlohmann::json document = R"({"name":"Alice","obj":{"property":"value"}})"_json;
    nlohmann::json query= R"({"obj": { "$eq": {"property":"value"}}})"_json;
    EXPECT_FALSE(match_query(document, query));
}
