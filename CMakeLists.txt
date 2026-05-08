cmake_minimum_required(VERSION 3.15)
project(SampleOrderSystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JSON_SOURCES
    src/json/JsonValue.cpp
    src/json/JsonParser.cpp
    src/json/JsonSerializer.cpp
)

set(MODEL_SOURCES
    src/model/Sample.cpp
    src/model/Order.cpp
    src/model/ProductionJob.cpp
)

add_executable(SampleOrderSystem src/main.cpp ${JSON_SOURCES} ${MODEL_SOURCES})
target_include_directories(SampleOrderSystem PRIVATE src)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)
