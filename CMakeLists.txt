cmake_minimum_required(VERSION 3.0)
project(jsondb VERSION 0.0.1 DESCRIPTION "library to use local json files as database")

# Configura el estándar de C++ a C++17
set(CMAKE_CXX_STANDARD 17)

# is Windows
if(WIN32)
    set(IS_WINDOWS TRUE)
else()
    set(IS_WINDOWS FALSE)
endif()

# Set compilation flags for code coverage with GCC, solo en Linux
if(NOT IS_WINDOWS)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    endif()
endif()

# Set compilation flags for code coverage with GCC
#if(CMAKE_COMPILER_IS_GNUCXX)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
#endif()

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_GetProperties(nlohmann_json)
if(NOT ThirdPartyLib_POPULATED)
    FetchContent_Populate(nlohmann_json)
endif()

add_library(jsondb STATIC
	src/jsondb.cpp
)
set_target_properties(jsondb PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(jsondb PROPERTIES PUBLIC_HEADER ${CMAKE_CURRENT_LIST_DIR}/src/jsondb.h)

add_subdirectory(${nlohmann_json_SOURCE_DIR} ${nlohmann_json_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} nlohmann_json)

install(FILES src/jsondb.h DESTINATION ${CMAKE_CURRENT_LIST_DIR}/include)
install(FILES build/libjsondb.a DESTINATION ${CMAKE_CURRENT_LIST_DIR}/lib)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Enable testing and add tests
enable_testing()
add_subdirectory(tests)

# Create a custom target for code coverage
if(NOT IS_WINDOWS)
add_custom_target(coverage

    COMMAND lcov --capture --directory . --output-file coverage.info
    COMMAND lcov --remove coverage.info 'tests/*' 'examples/*' '/usr/*' '${CMAKE_CURRENT_LIST_DIR}/build/_deps/*' --output-file coverage.info
    COMMAND genhtml coverage.info --output-directory coverage-report
    COMMAND gcovr --sonarqube -o coverage.xml -r ${CMAKE_CURRENT_LIST_DIR}
    #COMMAND gcovr --xml -o coverage.xml
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS tests # Assuming your tests target is named "tests"
)
endif()

add_subdirectory(examples)
