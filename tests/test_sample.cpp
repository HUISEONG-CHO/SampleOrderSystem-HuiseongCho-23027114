#include <gtest/gtest.h>
#include "model/Sample.h"

TEST(SampleTest, ConstructorSetsFields) {
    Sample s("S001", "SampleA", 30, 0.85, 100);
    EXPECT_EQ(s.getId(), "S001");
    EXPECT_EQ(s.getName(), "SampleA");
    EXPECT_EQ(s.getAvgProductionTime(), 30);
    EXPECT_DOUBLE_EQ(s.getYieldRate(), 0.85);
    EXPECT_EQ(s.getStock(), 100);
}

TEST(SampleTest, DefaultConstructor) {
    Sample s;
    EXPECT_EQ(s.getId(), "");
    EXPECT_EQ(s.getStock(), 0);
}

TEST(SampleTest, SetStock) {
    Sample s("S001", "SampleA", 30, 0.85, 100);
    s.setStock(200);
    EXPECT_EQ(s.getStock(), 200);
}
