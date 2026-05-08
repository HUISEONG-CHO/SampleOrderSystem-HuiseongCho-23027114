#include <gtest/gtest.h>
#include <filesystem>
#include <queue>
#include "controller/ProductionController.h"

class ProductionControllerTest : public ::testing::Test {
protected:
    SampleRepository          sampleRepo{"test_pc_samples.json"};
    OrderRepository           orderRepo{"test_pc_orders.json"};
    std::queue<ProductionJob> jobQueue;
    ProductionController      controller{sampleRepo, orderRepo, jobQueue};

    void SetUp() override {
        sampleRepo.save(Sample("S001", "SampleA", 30, 0.80, 50));
        orderRepo.save(Order("O001", "S001", "CustA", 100, "2026-05-08T10:00:00"));
        Order o = orderRepo.findById("O001");
        o.setStatus(OrderStatus::PRODUCING);
        orderRepo.update("O001", o);
        jobQueue.push(ProductionJob("O001", "S001", 140, 4200, "2026-05-08T10:00:00"));
    }
    void TearDown() override {
        std::filesystem::remove("test_pc_samples.json");
        std::filesystem::remove("test_pc_orders.json");
    }
};

TEST_F(ProductionControllerTest, CompleteProduction) {
    controller.completeProduction();
    EXPECT_EQ(orderRepo.findById("O001").getStatus(), OrderStatus::CONFIRMED);
}

TEST_F(ProductionControllerTest, StockIncreasedAfterCompletion) {
    int stockBefore = sampleRepo.findById("S001").getStock();
    controller.completeProduction();
    int stockAfter = sampleRepo.findById("S001").getStock();
    EXPECT_GT(stockAfter, stockBefore);
}

TEST_F(ProductionControllerTest, FifoOrder) {
    orderRepo.save(Order("O002", "S001", "CustB", 50, "2026-05-08T10:01:00"));
    Order o2 = orderRepo.findById("O002");
    o2.setStatus(OrderStatus::PRODUCING);
    orderRepo.update("O002", o2);
    jobQueue.push(ProductionJob("O002", "S001", 70, 2100, "2026-05-08T10:01:00"));

    controller.completeProduction();
    EXPECT_EQ(orderRepo.findById("O001").getStatus(), OrderStatus::CONFIRMED);
    EXPECT_EQ(orderRepo.findById("O002").getStatus(), OrderStatus::PRODUCING);
}

TEST_F(ProductionControllerTest, EmptyQueueDoesNothing) {
    std::queue<ProductionJob> emptyQ;
    ProductionController ctrl{sampleRepo, orderRepo, emptyQ};
    EXPECT_NO_THROW(ctrl.completeProduction());
}
