cmake_minimum_required(VERSION 3.15)
project(remote_config CXX)

set(CMAKE_CXX_STANDARD 17)

if (USE_CLION)
    set(CMAKE_PREFIX_PATH ${CMAKE_CACHEFILE_DIR} ${CMAKE_PREFIX_PATH})
endif ()

add_library(${PROJECT_NAME} STATIC
        src/remote_config.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC src/include)

target_include_directories(${PROJECT_NAME} PRIVATE src/private)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if (NOT SKIP_TEST)
    add_executable(main_test test/remote_config_test.cpp)
    enable_testing()

    find_package(GTest CONFIG REQUIRED)
    target_link_libraries(main_test PUBLIC ${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

    set_target_properties(main_test PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    )
    add_test(AllTestsInMain main_test)
endif ()

find_package(nlohmann_json REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS ${PROJECT_NAME})
