#include <gtest/gtest.h>
#include <filesystem>
#include "repository/OrderRepository.h"

namespace fs = std::filesystem;

class OrderRepositoryTest : public ::testing::Test {
protected:
    const std::string testFile = "test_orders.json";
    void SetUp()    override { if (fs::exists(testFile)) fs::remove(testFile); }
    void TearDown() override { if (fs::exists(testFile)) fs::remove(testFile); }
    OrderRepository makeRepo() { return OrderRepository(testFile); }
};

TEST_F(OrderRepositoryTest, SaveAndFindAll) {
    auto repo = makeRepo();
    repo.save(Order("O001", "S001", "CustA", 50, "2026-05-08T10:00:00"));
    repo.save(Order("O002", "S001", "CustB", 30, "2026-05-08T10:01:00"));
    ASSERT_EQ(repo.findAll().size(), 2u);
}

TEST_F(OrderRepositoryTest, FindById) {
    auto repo = makeRepo();
    repo.save(Order("O001", "S001", "CustA", 50, "2026-05-08T10:00:00"));
    auto o = repo.findById("O001");
    EXPECT_EQ(o.getCustomer(), "CustA");
    EXPECT_EQ(o.getStatus(), OrderStatus::RESERVED);
}

TEST_F(OrderRepositoryTest, Update) {
    auto repo = makeRepo();
    repo.save(Order("O001", "S001", "CustA", 50, "2026-05-08T10:00:00"));
    Order updated("O001", "S001", "CustA", 50, "2026-05-08T10:00:00");
    updated.setStatus(OrderStatus::CONFIRMED);
    repo.update("O001", updated);
    EXPECT_EQ(repo.findById("O001").getStatus(), OrderStatus::CONFIRMED);
}

TEST_F(OrderRepositoryTest, Remove) {
    auto repo = makeRepo();
    repo.save(Order("O001", "S001", "CustA", 50, "2026-05-08T10:00:00"));
    repo.remove("O001");
    EXPECT_TRUE(repo.findAll().empty());
}

TEST_F(OrderRepositoryTest, FindByStatus) {
    auto repo = makeRepo();
    repo.save(Order("O001", "S001", "CustA", 50, "2026-05-08T10:00:00"));
    Order o2("O002", "S001", "CustB", 30, "2026-05-08T10:01:00");
    o2.setStatus(OrderStatus::CONFIRMED);
    repo.save(o2);
    auto confirmed = repo.findByStatus(OrderStatus::CONFIRMED);
    ASSERT_EQ(confirmed.size(), 1u);
    EXPECT_EQ(confirmed[0].getId(), "O002");
}

TEST_F(OrderRepositoryTest, Persistence) {
    { OrderRepository r(testFile); r.save(Order("O001", "S001", "CustA", 50, "2026-05-08T10:00:00")); }
    { OrderRepository r(testFile); EXPECT_EQ(r.findAll().size(), 1u); }
}
