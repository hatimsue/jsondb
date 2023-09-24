# JSONDBLib - C++
JSONDBLib is a C++ library that allows you to use JSON files as a local database in your projects. It was developed as a personal project to learn more about C++, CMake, Doxygen, Lcov, SonarQube, and Google Tests.

### Dependencies

This project is built using [nlohmann/json](https://github.com/nlohmann/json), a powerful JSON library for C++. You can find more information about `nlohmann/json` on its GitHub repository.

### Disclaimer

This library was created for learning purposes. While it may be useful for personal or small projects, it is not recommended for use in critical or production applications without proper evaluation. It's important to consider the following:

- **No Security Guarantee**: JSONDBLib is not designed to be secure in production environments and does not provide robust security mechanisms. Data stored in JSON files is vulnerable to manipulation and unauthorized access.

- **Performance Limitations**: Using JSON files as a database may not be efficient in applications with large amounts of data or high read/write speed requirements. Consider other more suitable solutions for those situations.

### Contributions and Improvements

If you're interested in contributing to or improving this library, you are welcome to do so! Feel free to open issues, submit pull requests, and collaborate on its development.

Thank you for your interest in JSONDBLib!

### Build Instructions

To build this project on a Linux system, follow these steps:

1. Create a build directory:

```bash
    mkdir build
```

2. Navigate to the build directory:

```bash
    cd build
```

3. Run CMake to configure the build:

```bash
    cmake ..
```

4. Build the project using `make`:

```bash
    make
```

5. Optionally, install the project:

```bash
    make install
```

### Generate Documentation

The documentation for this project is generated using Doxygen. To generate the documentation, follow these steps:

1. Make sure you have Doxygen installed on your system. If you don't have it installed yet, you can download it [here](http://www.doxygen.nl/download.html).

2. Open a terminal and navigate to the root directory of your project.

3. Run the following command to generate the documentation:

```bash
    doxygen Doxygen
```

### Example

```cpp
// examples/basic.cpp

#include <iostream>
#include "jsondb.h"
#include "nlohmann/json_fwd.hpp"

int main() {
    using namespace jsondb;

    // Database path and name
    const char DB_PATH[] = "data";
    const char DB_NAME[] = "mydb";

    // Create an instance of JsonDB
    JsonDB db(DB_NAME, DB_PATH);

    // Create the database if it doesn't exist
    if (!db.Exists()) {
        db.Create();
    }

    // Create a table named "users" in the database if it doesn't exist
    Table usersTable("users", db);
    if (!usersTable.Exists()) {
        usersTable.Create();
    }

    // Create JSON documents for users
    nlohmann::json user1 = R"({"age":30,"city":"LA","name":"john doe"})"_json;
    nlohmann::json user2 = R"({"age":28,"city":"NY","name":"jane doe"})"_json;

    // Insert documents into the table
    usersTable.InsertDocument(user1);
    usersTable.InsertDocument(user2);

    // Find the first document in the table
    std::cout << usersTable.FindDocument() << std::endl;
    // Output: {"age":30,"city":"LA","name":"john doe"}

    // Query 1: Find a user with age 28 and a city in ["NY", "NJ", "LA"]
    nlohmann::json query1 = R"(
    {
        "age": {
            "$eq": 28
        },
        "city": {
            "$in": [ "NY", "NJ", "LA" ]
        }
    })"_json;

    // Display the first document that matches the query
    std::cout << usersTable.FindDocument(query1) << std::endl;
    // Output: {"age":28,"city":"NY","name":"jane doe"}

    // Display all documents in the table
    std::cout << usersTable.FindDocuments() << std::endl;
    // Output: [{"age":30,"city":"LA","name":"john doe"},{"age":28,"city":"NY","name":"jane doe"}]

    // Query 2: Find users with ages in [27, 28, 29, 30, 31] and cities in ["NY", "NJ", "LA"]
    nlohmann::json query2 = R"(
    {
        "age": {
            "$in": [27,28,29,30,31]
        },
        "city": {
            "$in": [ "NY", "NJ", "LA" ]
        }
    })"_json;

    // Display all documents that match the query
    std::cout << usersTable.FindDocuments(query2) << std::endl;
    // Output: [{"age":30,"city":"LA","name":"john doe"},{"age":28,"city":"NY","name":"jane doe"}]

    // Update a document in the table
    nlohmann::json newData = R"({"city":"NJ","phone":"+1234567890"})"_json;
    // Update the first document that matches the query
    usersTable.UpdateDocument(newData, query1);
    std::cout << usersTable.FindDocument(query1) << std::endl;
    // Output: {"age":28,"city":"NJ","name":"jane doe","phone":"+1234567890"}

    // Query 3: Find and update documents with cities in ["NY", "NJ", "LA"] by adding a "country" field set to "USA"
    nlohmann::json query3 = R"({"city": {"$in": [ "NY", "NJ", "LA" ] } })"_json;
    nlohmann::json newCountry = R"({"country":"USA"})"_json;

    // Update all documents that match the query in the table
    usersTable.UpdateDocuments(newCountry, query3);
    std::cout << usersTable.FindDocuments(query3) << std::endl;
    // Output: [{"age":30,"city":"LA","country":"USA","name":"john doe"},{"age":28,"city":"NJ","country":"USA","name":"jane doe","phone":"+1234567890"}]

    // Query 4: Delete documents with ages 28 or 30
    nlohmann::json query4 = R"({"age": {"$in": [ 28, 30] } })"_json;

    // Delete the first document that matches the query
    usersTable.DeleteDocument(query4);
    std::cout << usersTable.FindDocuments() << std::endl;
    // Output: [{"age":28,"city":"NJ","country":"USA","name":"jane doe","phone":"+1234567890"}]

    // Delete all documents that match the query
    usersTable.DeleteDocuments(query4);
    std::cout << usersTable.FindDocuments() << std::endl;
    // Output: []

    // Drop the table
    usersTable.Drop();

    // Drop the database
    db.Drop();

    return 0;
}

```


