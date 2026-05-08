#include <gtest/gtest.h>
#include <filesystem>
#include "controller/ReleaseController.h"

class ReleaseControllerTest : public ::testing::Test {
protected:
    SampleRepository  sampleRepo{"test_rc_samples.json"};
    OrderRepository   orderRepo{"test_rc_orders.json"};
    ReleaseController controller{sampleRepo, orderRepo};

    void SetUp() override {
        sampleRepo.save(Sample("S001", "SampleA", 30, 0.80, 200));
        Order o("O001", "S001", "CustA", 50, "2026-05-08T10:00:00");
        o.setStatus(OrderStatus::CONFIRMED);
        orderRepo.save(o);
    }
    void TearDown() override {
        std::filesystem::remove("test_rc_samples.json");
        std::filesystem::remove("test_rc_orders.json");
    }
};

TEST_F(ReleaseControllerTest, GetConfirmedOrders) {
    auto list = controller.getConfirmedOrders();
    ASSERT_EQ(list.size(), 1u);
    EXPECT_EQ(list[0].getId(), "O001");
}

TEST_F(ReleaseControllerTest, ReleaseDecreasesStock) {
    int stockBefore = sampleRepo.findById("S001").getStock();
    controller.release("O001");
    int stockAfter = sampleRepo.findById("S001").getStock();
    EXPECT_EQ(stockAfter, stockBefore - 50);
}

TEST_F(ReleaseControllerTest, ReleaseChangesStatus) {
    controller.release("O001");
    EXPECT_EQ(orderRepo.findById("O001").getStatus(), OrderStatus::RELEASE);
}

TEST_F(ReleaseControllerTest, ReleaseNonConfirmedThrows) {
    Order o("O002", "S001", "CustB", 30, "2026-05-08T10:01:00");
    orderRepo.save(o); // RESERVED
    EXPECT_THROW(controller.release("O002"), std::runtime_error);
}
