project(jsondb_examples)

# Lista de archivos de ejemplo
set(EXAMPLE_SOURCES
    basic.cpp
)

# Crea un ejecutable para cada ejemplo
foreach(example_source ${EXAMPLE_SOURCES})
    get_filename_component(example_name ${example_source} NAME_WE)
    add_executable(${example_name} ${example_source})
    target_include_directories(${example_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${example_name} PRIVATE jsondb)
endforeach()
