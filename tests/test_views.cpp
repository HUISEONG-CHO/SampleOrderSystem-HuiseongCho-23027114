#include <gtest/gtest.h>
#include <queue>
#include "view/SampleView.h"
#include "view/OrderView.h"
#include "view/MonitorView.h"
#include "view/ProductionView.h"
#include "view/ReleaseView.h"
#include "view/MainView.h"

// ── SampleView ────────────────────────────────────────────────
TEST(SampleViewTest, ShowSampleContainsNameAndStock) {
    SampleView view;
    testing::internal::CaptureStdout();
    view.showSample(Sample("S001", "SampleA", 30, 0.85, 100));
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("SampleA"), std::string::npos);
    EXPECT_NE(out.find("100"),     std::string::npos);
}

TEST(SampleViewTest, ShowAllSamplesContainsAll) {
    SampleView view;
    std::vector<Sample> samples = {
        Sample("S001", "Alpha", 30, 0.85, 50),
        Sample("S002", "Beta",  45, 0.90, 80)
    };
    testing::internal::CaptureStdout();
    view.showAllSamples(samples);
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("Alpha"), std::string::npos);
    EXPECT_NE(out.find("Beta"),  std::string::npos);
}

// ── OrderView ─────────────────────────────────────────────────
TEST(OrderViewTest, ShowOrderContainsCustomerAndQuantity) {
    OrderView view;
    Order o("O001", "S001", "CustomerA", 50, "2026-05-08T10:00:00");
    testing::internal::CaptureStdout();
    view.showOrder(o);
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("CustomerA"), std::string::npos);
    EXPECT_NE(out.find("50"),        std::string::npos);
}

TEST(OrderViewTest, ShowAllOrdersContainsAll) {
    OrderView view;
    std::vector<Order> orders = {
        Order("O001", "S001", "CustA", 50, "2026-05-08T10:00:00"),
        Order("O002", "S001", "CustB", 80, "2026-05-08T10:01:00")
    };
    testing::internal::CaptureStdout();
    view.showAllOrders(orders);
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("CustA"), std::string::npos);
    EXPECT_NE(out.find("CustB"), std::string::npos);
}

// ── MonitorView ───────────────────────────────────────────────
TEST(MonitorViewTest, ShowCountsContainsStatus) {
    MonitorView view;
    std::map<OrderStatus, int> counts = {
        {OrderStatus::RESERVED, 2}, {OrderStatus::CONFIRMED, 1}
    };
    testing::internal::CaptureStdout();
    view.showOrderCounts(counts);
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("2"), std::string::npos);
}

TEST(MonitorViewTest, ShowStockLabelContainsLabel) {
    MonitorView view;
    testing::internal::CaptureStdout();
    view.showStockLabel("S001", "SampleA", 100, "부족");
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("SampleA"), std::string::npos);
    EXPECT_NE(out.find("100"),     std::string::npos);
}

// ── ProductionView ────────────────────────────────────────────
TEST(ProductionViewTest, ShowJobContainsOrderId) {
    ProductionView view;
    ProductionJob job("O001", "S001", 140, 4200, "2026-05-08T10:00:00");
    testing::internal::CaptureStdout();
    view.showCurrentJob(job, 0);
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("O001"), std::string::npos);
}

TEST(ProductionViewTest, ShowEmptyQueue) {
    ProductionView view;
    std::vector<ProductionJob> emptyVec;
    testing::internal::CaptureStdout();
    view.showQueue(emptyVec);
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(out.empty());
}

// ── ReleaseView ───────────────────────────────────────────────
TEST(ReleaseViewTest, ShowConfirmedListContainsOrders) {
    ReleaseView view;
    Order o("O001", "S001", "CustA", 50, "2026-05-08T10:00:00");
    o.setStatus(OrderStatus::CONFIRMED);
    testing::internal::CaptureStdout();
    view.showConfirmedOrders({o});
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("CustA"), std::string::npos);
}

// ── MainView ──────────────────────────────────────────────────
TEST(MainViewTest, ShowMenuContainsOptions) {
    MainView view;
    testing::internal::CaptureStdout();
    view.showMenu();
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("1"), std::string::npos);
    EXPECT_NE(out.find("0"), std::string::npos);
}

TEST(MainViewTest, ShowMessageContainsText) {
    MainView view;
    testing::internal::CaptureStdout();
    view.showMessage("TestMessage");
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_NE(out.find("TestMessage"), std::string::npos);
}
