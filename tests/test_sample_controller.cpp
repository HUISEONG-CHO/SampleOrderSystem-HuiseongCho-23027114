#include <gtest/gtest.h>
#include <filesystem>
#include "controller/SampleController.h"

class SampleControllerTest : public ::testing::Test {
protected:
    SampleRepository repo{"test_sc_samples.json"};
    SampleController controller{repo};
    void TearDown() override {
        std::filesystem::remove("test_sc_samples.json");
    }
};

TEST_F(SampleControllerTest, RegisterAndGetAll) {
    controller.registerSample("SampleA", 30, 0.85);
    controller.registerSample("SampleB", 45, 0.90);
    auto all = controller.getAllSamples();
    ASSERT_EQ(all.size(), 2u);
}

TEST_F(SampleControllerTest, GetSampleById) {
    controller.registerSample("SampleA", 30, 0.85);
    auto all = controller.getAllSamples();
    std::string id = all[0].getId();
    Sample s = controller.getSample(id);
    EXPECT_EQ(s.getName(), "SampleA");
}

TEST_F(SampleControllerTest, SearchByName) {
    controller.registerSample("AlphaTest", 30, 0.85);
    controller.registerSample("BetaTest",  45, 0.90);
    controller.registerSample("Gamma",     20, 0.75);
    auto results = controller.searchByKeyword("Test");
    ASSERT_EQ(results.size(), 2u);
}

TEST_F(SampleControllerTest, SearchById) {
    controller.registerSample("SampleA", 30, 0.85);
    auto results = controller.searchByKeyword("1");
    ASSERT_EQ(results.size(), 1u);
    EXPECT_EQ(results[0].getName(), "SampleA");
}

TEST_F(SampleControllerTest, SearchByAvgProductionTime) {
    controller.registerSample("SampleA", 30, 0.85);
    controller.registerSample("SampleB", 45, 0.90);
    auto results = controller.searchByKeyword("45");
    ASSERT_EQ(results.size(), 1u);
    EXPECT_EQ(results[0].getName(), "SampleB");
}

TEST_F(SampleControllerTest, SearchByStock) {
    controller.registerSample("SampleA", 30, 0.85);
    // stock 초기값 0 → 검색어 "0" 으로 검색
    auto results = controller.searchByKeyword("0");
    EXPECT_FALSE(results.empty());
}

TEST_F(SampleControllerTest, NotFoundThrows) {
    EXPECT_THROW(controller.getSample("NONEXISTENT"), std::runtime_error);
}
