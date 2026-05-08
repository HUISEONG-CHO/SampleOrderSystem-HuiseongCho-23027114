#include <gtest/gtest.h>
#include "model/ProductionJob.h"

TEST(ProductionJobTest, ConstructorSetsFields) {
    ProductionJob job("O001", "S001", 120, 3600, "2026-05-08T10:00:00");
    EXPECT_EQ(job.getOrderId(), "O001");
    EXPECT_EQ(job.getSampleId(), "S001");
    EXPECT_EQ(job.getProductionQty(), 120);
    EXPECT_EQ(job.getTotalTime(), 3600);
    EXPECT_EQ(job.getEnqueuedAt(), "2026-05-08T10:00:00");
}
