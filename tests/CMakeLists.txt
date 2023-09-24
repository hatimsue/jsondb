# Download and configure Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0  # Adjust to the version you want
)

FetchContent_MakeAvailable(googletest)

# Add test source files
file(GLOB TEST_SOURCES "*Test.cpp")

# Create a test executable
add_executable(jsondbTests ${TEST_SOURCES})

# Include the directory containing jsondb.h
target_include_directories(jsondbTests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link with Google Test and your project's library
target_link_libraries(jsondbTests gtest gtest_main jsondb)

# Register tests
include(GoogleTest)
gtest_discover_tests(jsondbTests)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
