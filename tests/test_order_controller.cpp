#include <gtest/gtest.h>
#include <cmath>
#include <filesystem>
#include "controller/OrderController.h"

class OrderControllerTest : public ::testing::Test {
protected:
    SampleRepository  sampleRepo{"test_oc_samples.json"};
    OrderRepository   orderRepo{"test_oc_orders.json"};
    std::queue<ProductionJob> productionQueue;
    OrderController   controller{sampleRepo, orderRepo, productionQueue};

    void SetUp() override {
        sampleRepo.save(Sample("S001", "SampleA", 30, 0.80, 100));
    }
    void TearDown() override {
        std::filesystem::remove("test_oc_samples.json");
        std::filesystem::remove("test_oc_orders.json");
    }
};

TEST_F(OrderControllerTest, PlaceOrderReserved) {
    std::string oid = controller.placeOrder("S001", "CustomerA", 50);
    Order o = orderRepo.findById(oid);
    EXPECT_EQ(o.getStatus(), OrderStatus::RESERVED);
}

TEST_F(OrderControllerTest, ApproveWithSufficientStock) {
    std::string oid = controller.placeOrder("S001", "CustomerA", 50);
    controller.approveOrder(oid);
    EXPECT_EQ(orderRepo.findById(oid).getStatus(), OrderStatus::CONFIRMED);
}

TEST_F(OrderControllerTest, ApproveWithInsufficientStock) {
    // stock=100, order=150 → shortage=50
    std::string oid = controller.placeOrder("S001", "CustomerA", 150);
    controller.approveOrder(oid);
    EXPECT_EQ(orderRepo.findById(oid).getStatus(), OrderStatus::PRODUCING);
    EXPECT_FALSE(productionQueue.empty());
}

TEST_F(OrderControllerTest, ProductionQtyCalculation) {
    // stock=100, order=200 → shortage=100, yieldRate=0.80
    // productionQty = ceil(100 / (0.80 * 0.9)) = ceil(100/0.72) = ceil(138.88) = 139
    std::string oid = controller.placeOrder("S001", "CustomerA", 200);
    controller.approveOrder(oid);
    ASSERT_FALSE(productionQueue.empty());
    int expected = static_cast<int>(std::ceil(100.0 / (0.80 * 0.9)));
    EXPECT_EQ(productionQueue.front().getProductionQty(), expected);
}

TEST_F(OrderControllerTest, RejectOrder) {
    std::string oid = controller.placeOrder("S001", "CustomerA", 50);
    controller.rejectOrder(oid);
    EXPECT_EQ(orderRepo.findById(oid).getStatus(), OrderStatus::REJECTED);
}

TEST_F(OrderControllerTest, PlaceOrderInvalidSample) {
    EXPECT_THROW(controller.placeOrder("NONE", "CustomerA", 50), std::runtime_error);
}
