#include <gtest/gtest.h>
#include <filesystem>
#include "controller/MonitorController.h"

class MonitorControllerTest : public ::testing::Test {
protected:
    SampleRepository  sampleRepo{"test_mc_samples.json"};
    OrderRepository   orderRepo{"test_mc_orders.json"};
    MonitorController controller{sampleRepo, orderRepo};

    void SetUp() override {
        sampleRepo.save(Sample("S001", "SampleA", 30, 0.80, 100));
        orderRepo.save(Order("O001", "S001", "CustA", 50, "2026-05-08T10:00:00"));
        orderRepo.save(Order("O002", "S001", "CustB", 80, "2026-05-08T10:01:00"));
        Order o2 = orderRepo.findById("O002");
        o2.setStatus(OrderStatus::CONFIRMED);
        orderRepo.update("O002", o2);
        Order o3("O003", "S001", "CustC", 30, "2026-05-08T10:02:00");
        o3.setStatus(OrderStatus::REJECTED);
        orderRepo.save(o3);
    }
    void TearDown() override {
        std::filesystem::remove("test_mc_samples.json");
        std::filesystem::remove("test_mc_orders.json");
    }
};

TEST_F(MonitorControllerTest, CountByStatus) {
    auto counts = controller.getOrderCounts();
    EXPECT_EQ(counts.at(OrderStatus::RESERVED),  1);
    EXPECT_EQ(counts.at(OrderStatus::CONFIRMED),  1);
    EXPECT_EQ(counts.count(OrderStatus::REJECTED), 0u); // REJECTED 제외
}

TEST_F(MonitorControllerTest, StockLabelSufficient) {
    // stock=100 > totalOrderQty(50+80=130)? No → 부족
    // stock=200 > 130 → 여유
    sampleRepo.update("S001", Sample("S001", "SampleA", 30, 0.80, 200));
    std::string label = controller.getStockLabel("S001");
    EXPECT_EQ(label, "여유");
}

TEST_F(MonitorControllerTest, StockLabelShortage) {
    // stock=100, totalQty=130 → 부족
    std::string label = controller.getStockLabel("S001");
    EXPECT_EQ(label, "부족");
}

TEST_F(MonitorControllerTest, StockLabelDepleted) {
    sampleRepo.update("S001", Sample("S001", "SampleA", 30, 0.80, 0));
    std::string label = controller.getStockLabel("S001");
    EXPECT_EQ(label, "고갈");
}
