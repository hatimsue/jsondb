#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "jsondb.h"
#include "nlohmann/json.hpp"

using namespace jsondb;

class TableTestFixture : public ::testing::Test
{
protected:
    JsonDB db;
    Table table;
    nlohmann::json document;

    TableTestFixture() : db("test_db"), table("test_table", db)
    {
    }

    ~TableTestFixture()
    {
        db.Drop();
    }
};

TEST_F(TableTestFixture, TestCreate)
{
    db.Create();
    EXPECT_TRUE(table.Create());
}

TEST_F(TableTestFixture, FailTestCreate)
{
    EXPECT_FALSE(table.Create());
    db.Create();
    table.Create();
    EXPECT_FALSE(table.Create());
}

TEST_F(TableTestFixture, TestExists)
{
    db.Create();
    EXPECT_FALSE(table.Exists());

    table.Create();
    EXPECT_TRUE(table.Exists());
}

TEST_F(TableTestFixture, FailTestExists)
{
    EXPECT_FALSE(table.Exists());
    std::ofstream dbFile(db.GetFile());
    dbFile << "Invalid JSON data";
    dbFile.close();
    EXPECT_FALSE(table.Exists());
}

TEST_F(TableTestFixture, TestGetName)
{
    db.Create();
    table.Create();
    EXPECT_EQ(table.GetName(), "test_table");
}

TEST_F(TableTestFixture, TestDrop)
{
    db.Create();
    table.Create();
    EXPECT_TRUE(table.Exists());

    EXPECT_TRUE(table.Drop());

    EXPECT_FALSE(table.Exists());
}

TEST_F(TableTestFixture, FailTestDrop)
{
    EXPECT_FALSE(table.Drop());

    db.Create();
    EXPECT_FALSE(table.Drop());
}

TEST_F(TableTestFixture, TestInsertDocument)
{
    db.Create();
    table.Create();
    nlohmann::json doc = {{"key", "value1"}};
    bool insertResult = table.InsertDocument(doc);
    EXPECT_TRUE(insertResult);
}

TEST_F(TableTestFixture, TestFindDocumentEmptyTest)
{
    db.Create();
    table.Create();
    nlohmann::json result = table.FindDocument();
    ASSERT_TRUE(result.is_object());
    ASSERT_EQ(result.size(), 0);
}

TEST_F(TableTestFixture, TestFindDocument)
{
    db.Create();
    table.Create();
    nlohmann::json doc1 = {{"key", "value1"}};
    nlohmann::json doc2 = {{"key", "value2"}};
    table.InsertDocument(doc1);
    table.InsertDocument(doc2);
    nlohmann::json result = table.FindDocument();
    ASSERT_TRUE(result.is_object());
    EXPECT_EQ(result["key"], "value1");
}

TEST_F(TableTestFixture, TestFindDocuments)
{
    db.Create();
    table.Create();
    nlohmann::json doc1 = {{"key", "value1"}};
    nlohmann::json doc2 = {{"key", "value2"}};
    table.InsertDocument(doc1);
    table.InsertDocument(doc2);
    nlohmann::json result = table.FindDocuments();
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0]["key"], "value1");
    EXPECT_EQ(result[1]["key"], "value2");
}

TEST_F(TableTestFixture, TestFindDocumentQuery)
{
    db.Create();
    table.Create();
    nlohmann::json doc1 = {{"key", "value1"}};
    nlohmann::json doc2 = {{"key", "value2"}};
    nlohmann::json query = {{
        "key", {
            {"$eq", "value2"}
        }
    }};
    table.InsertDocument(doc1);
    table.InsertDocument(doc2);
    nlohmann::json result = table.FindDocument(query);
    ASSERT_TRUE(result.is_object());
    EXPECT_EQ(result["key"], "value2");
}

TEST_F(TableTestFixture, TestFindDocumentsQuery)
{
    db.Create();
    table.Create();
    nlohmann::json doc1 = {{"key", "value1"},{"filterkey", 4}};
    nlohmann::json doc2 = {{"key", "value2"},{"filterkey", 5}};
    nlohmann::json doc3 = {{"key", "value1"},{"filterkey", 5}};
    nlohmann::json doc4 = {{"key", "value2"},{"filterkey", 6}};
    nlohmann::json query0 = {{"filterkey", {{"$eq", 3}}}};
    nlohmann::json query1 = {{"filterkey", {{"$eq", 5}}}};
    nlohmann::json query2 = {{"filterkey", {{"$lt", 5}}}};
    nlohmann::json query3 = {{"filterkey", {{"$gt", 5}}}};
    nlohmann::json query4 = {{"filterkey", {{"$in", {5,6}}}}};
    table.InsertDocument(doc1);
    table.InsertDocument(doc2);
    table.InsertDocument(doc3);
    table.InsertDocument(doc4);
    EXPECT_EQ(table.FindDocuments(query0).size(), 0);
    EXPECT_EQ(table.FindDocuments(query1).size(), 2);
    EXPECT_EQ(table.FindDocuments(query2).size(), 1);
    EXPECT_EQ(table.FindDocuments(query3).size(), 1);
    EXPECT_EQ(table.FindDocuments(query4).size(), 3);
}

TEST_F(TableTestFixture, TestUpdateDocument)
{
    db.Create();
    table.Create();
    nlohmann::json doc1 = {{"key", "value1"},{"filterkey", 4}};
    nlohmann::json doc2 = {{"key", "value2"},{"filterkey", 5}};
    nlohmann::json doc3 = {{"key", "value1"},{"filterkey", 5}};
    nlohmann::json query0 = {{"filterkey", {{"$eq", 4}}}};
    nlohmann::json query1 = {{"filterkey", {{"$eq", 5}}}};
    table.InsertDocument(doc1);
    table.InsertDocument(doc2);
    table.InsertDocument(doc3);
    table.UpdateDocument({{"filterkey", 7}}, query0);
    EXPECT_EQ(table.FindDocuments({{"filterkey", {{"$eq", 7}}}}).size(), 1);
    table.UpdateDocument({{"filterkey", 8}}, query1);
    EXPECT_EQ(table.FindDocuments({{"filterkey", {{"$eq", 8}}}}).size(), 1);
    table.UpdateDocument({{"filterkey", 8}}, query1);
    EXPECT_EQ(table.FindDocuments({{"filterkey", {{"$eq", 8}}}}).size(), 2);
}

TEST_F(TableTestFixture, TestUpdateDocuments)
{
    db.Create();
    table.Create();
    nlohmann::json doc1 = {{"key", "value1"},{"filterkey", 4}};
    nlohmann::json doc2 = {{"key", "value2"},{"filterkey", 5}};
    nlohmann::json doc3 = {{"key", "value1"},{"filterkey", 5}};
    nlohmann::json query0 = {{"filterkey", {{"$eq", 4}}}};
    nlohmann::json query1 = {{"filterkey", {{"$eq", 5}}}};

    table.InsertDocument(doc1);
    table.InsertDocument(doc2);
    table.InsertDocument(doc3);

    table.UpdateDocuments({{"filterkey", 8}}, query0);
    EXPECT_EQ(table.FindDocuments({{"filterkey", {{"$eq", 8}}}}).size(), 1);
    table.UpdateDocuments({{"filterkey", 8}}, query1);
    EXPECT_EQ(table.FindDocuments({{"filterkey", {{"$eq", 8}}}}).size(), 3);


}

TEST_F(TableTestFixture, TestDeleteDocument)
{
    db.Create();
    table.Create();
    nlohmann::json doc2 = {{"key", "value2"},{"filterkey", 5}};
    nlohmann::json doc3 = {{"key", "value1"},{"filterkey", 5}};
    nlohmann::json query1 = {{"filterkey", {{"$eq", 5}}}};
    table.InsertDocument(doc2);
    table.InsertDocument(doc3);
    EXPECT_EQ(table.FindDocuments({{"filterkey", {{"$eq", 5}}}}).size(), 2);
    EXPECT_TRUE(table.DeleteDocument(query1));
    EXPECT_EQ(table.FindDocuments({{"filterkey", {{"$eq", 5}}}}).size(), 1);
}

TEST_F(TableTestFixture, TestDeleteDocuments)
{
    db.Create();
    table.Create();
    nlohmann::json doc2 = {{"key", "value2"},{"filterkey", 5}};
    nlohmann::json doc3 = {{"key", "value1"},{"filterkey", 5}};
    nlohmann::json query1 = {{"filterkey", {{"$eq", 5}}}};
    table.InsertDocument(doc2);
    table.InsertDocument(doc3);
    EXPECT_EQ(table.FindDocuments({{"filterkey", {{"$eq", 5}}}}).size(), 2);
    EXPECT_TRUE(table.DeleteDocuments(query1));
    EXPECT_EQ(table.FindDocuments({{"filterkey", {{"$eq", 5}}}}).size(), 0);
}
