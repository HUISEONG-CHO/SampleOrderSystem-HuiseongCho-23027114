#include <gtest/gtest.h>
#include <filesystem>
#include "controller/ProductionController.h"
#include "repository/ProductionJobRepository.h"

class ProductionControllerTest : public ::testing::Test {
protected:
    SampleRepository        sampleRepo{"test_pc_samples.json"};
    OrderRepository         orderRepo{"test_pc_orders.json"};
    ProductionJobRepository jobRepo{"test_pc_jobs.json"};
    ProductionController    controller{sampleRepo, orderRepo, jobRepo};

    void SetUp() override {
        sampleRepo.save(Sample("1", "SampleA", 30, 0.80, 50));
        orderRepo.save(Order("1", "1", "CustA", 100, "2026-05-08T10:00:00"));
        Order o = orderRepo.findById("1");
        o.setStatus(OrderStatus::PRODUCING);
        orderRepo.update("1", o);
        // totalTime=0 → tick() 즉시 완료 처리
        jobRepo.save(ProductionJob("1", "1", 140, 0, "2000-01-01T00:00:00"));
    }
    void TearDown() override {
        std::filesystem::remove("test_pc_samples.json");
        std::filesystem::remove("test_pc_orders.json");
        std::filesystem::remove("test_pc_jobs.json");
    }
};

TEST_F(ProductionControllerTest, TickCompletesJob) {
    controller.tick();
    EXPECT_EQ(orderRepo.findById("1").getStatus(), OrderStatus::CONFIRMED);
}

TEST_F(ProductionControllerTest, StockIncreasedAfterTick) {
    int stockBefore = sampleRepo.findById("1").getStock();
    controller.tick();
    int stockAfter = sampleRepo.findById("1").getStock();
    EXPECT_GT(stockAfter, stockBefore);
}

TEST_F(ProductionControllerTest, FifoOrder) {
    orderRepo.save(Order("2", "1", "CustB", 50, "2026-05-08T10:01:00"));
    Order o2 = orderRepo.findById("2");
    o2.setStatus(OrderStatus::PRODUCING);
    orderRepo.update("2", o2);
    // O001: totalTime=0 (완료), O002: totalTime=99999 (미완료)
    jobRepo.save(ProductionJob("2", "1", 70, 99999, "2026-05-08T10:01:00"));

    controller.tick();
    EXPECT_EQ(orderRepo.findById("1").getStatus(), OrderStatus::CONFIRMED);
    EXPECT_EQ(orderRepo.findById("2").getStatus(), OrderStatus::PRODUCING);
}

TEST_F(ProductionControllerTest, EmptyRepoDoesNothing) {
    ProductionJobRepository emptyRepo{"test_pc_empty_jobs.json"};
    ProductionController ctrl{sampleRepo, orderRepo, emptyRepo};
    EXPECT_NO_THROW(ctrl.tick());
    std::filesystem::remove("test_pc_empty_jobs.json");
}
