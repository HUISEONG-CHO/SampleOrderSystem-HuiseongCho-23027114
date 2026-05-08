add_executable(SampleOrderSystem_Tests
    test_placeholder.cpp
    test_sample.cpp
    test_order.cpp
    test_production_job.cpp
    ${CMAKE_SOURCE_DIR}/src/json/JsonValue.cpp
    ${CMAKE_SOURCE_DIR}/src/json/JsonParser.cpp
    ${CMAKE_SOURCE_DIR}/src/json/JsonSerializer.cpp
    ${CMAKE_SOURCE_DIR}/src/model/Sample.cpp
    ${CMAKE_SOURCE_DIR}/src/model/Order.cpp
    ${CMAKE_SOURCE_DIR}/src/model/ProductionJob.cpp
)
target_include_directories(SampleOrderSystem_Tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(SampleOrderSystem_Tests GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(SampleOrderSystem_Tests)
