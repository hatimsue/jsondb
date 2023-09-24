#include <gtest/gtest.h>
#include "jsondb.h"
#include <fstream>

using namespace jsondb;

class JsonDBTest : public ::testing::Test
{
protected:
    JsonDB db;
    JsonDB dbFail;

    JsonDBTest() : db("test_db"), dbFail("fail_db","./data/data/data")
    {
    }

    ~JsonDBTest()
    {
        db.Drop();
    }
};

TEST_F(JsonDBTest, TestCreate)
{
    EXPECT_TRUE(db.Create());
    EXPECT_FALSE(db.Create());
}

TEST_F(JsonDBTest, TestExists)
{
    EXPECT_FALSE(db.Exists());
    db.Create();
    EXPECT_TRUE(db.Exists());
}

TEST_F(JsonDBTest, TestDrop)
{
    db.Create();
    EXPECT_TRUE(db.Exists());
    EXPECT_TRUE(db.Drop());
    EXPECT_FALSE(db.Exists());
}

TEST_F(JsonDBTest, TestGetters)
{
    EXPECT_EQ(db.GetName(), "test_db");
    EXPECT_EQ(db.GetPath(), "./data");
    EXPECT_EQ(db.GetFile(), "./data/test_db.json");
    EXPECT_EQ(db.GetTablesPath(), "./data/test_db");
}

TEST_F(JsonDBTest, FailCreate)
{
    EXPECT_FALSE(dbFail.Create());
}

TEST_F(JsonDBTest, ReturnsTables)
{
    db.Create();
    Table users("users", db);
    Table cars("cars", db);
    users.Create();
    cars.Create();

    auto tables = db.Tables();

    ASSERT_EQ(tables.size(), 2);
    EXPECT_EQ(tables[0]["name"], "users");
    EXPECT_EQ(tables[1]["name"], "cars");
}

TEST_F(JsonDBTest, ErrorReturnsTables)
{
    db.Create();
    nlohmann::json data;
    data["name"] = db.GetName();
    data["tables"] = "";
    std::ofstream dbFile(db.GetFile());
    dbFile << data << std::endl;
    ASSERT_THROW(db.Tables(), std::runtime_error);
}

TEST_F(JsonDBTest, HandlesFileNotFound)
{
    jsondb::JsonDB nonExistentDB("nonexistent", "./nonexistent_data");
    ASSERT_THROW(nonExistentDB.Tables(), std::runtime_error);
}

TEST_F(JsonDBTest, HandlesInvalidDatabaseFormat)
{
    std::ofstream dbFile(db.GetFile());
    dbFile << "Invalid JSON data";
    dbFile.close();
    ASSERT_THROW(db.Tables(), std::runtime_error);
}
