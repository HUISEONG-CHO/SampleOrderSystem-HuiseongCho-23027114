#include <gtest/gtest.h>
#include "model/Order.h"

TEST(OrderTest, ConstructorSetsFields) {
    Order o("O001", "S001", "CustomerA", 50, "2026-05-08T10:00:00");
    EXPECT_EQ(o.getId(), "O001");
    EXPECT_EQ(o.getSampleId(), "S001");
    EXPECT_EQ(o.getCustomer(), "CustomerA");
    EXPECT_EQ(o.getQuantity(), 50);
    EXPECT_EQ(o.getStatus(), OrderStatus::RESERVED);
    EXPECT_EQ(o.getCreatedAt(), "2026-05-08T10:00:00");
}

TEST(OrderTest, DefaultStatus) {
    Order o("O001", "S001", "CustomerA", 50, "2026-05-08T10:00:00");
    EXPECT_EQ(o.getStatus(), OrderStatus::RESERVED);
}

TEST(OrderTest, SetStatus) {
    Order o("O001", "S001", "CustomerA", 50, "2026-05-08T10:00:00");
    o.setStatus(OrderStatus::CONFIRMED);
    EXPECT_EQ(o.getStatus(), OrderStatus::CONFIRMED);
}

TEST(OrderTest, AllStatusValues) {
    Order o("O001", "S001", "CustomerA", 50, "2026-05-08T10:00:00");
    o.setStatus(OrderStatus::RESERVED);   EXPECT_EQ(o.getStatus(), OrderStatus::RESERVED);
    o.setStatus(OrderStatus::REJECTED);   EXPECT_EQ(o.getStatus(), OrderStatus::REJECTED);
    o.setStatus(OrderStatus::PRODUCING);  EXPECT_EQ(o.getStatus(), OrderStatus::PRODUCING);
    o.setStatus(OrderStatus::CONFIRMED);  EXPECT_EQ(o.getStatus(), OrderStatus::CONFIRMED);
    o.setStatus(OrderStatus::RELEASE);    EXPECT_EQ(o.getStatus(), OrderStatus::RELEASE);
}
