#include "OrderView.h"
#include "ui.h"
#include <iostream>

using namespace UI;

static const int OW_ID      = 3;
static const int OW_CUST    = 12;
static const int OW_SID     = 6;
static const int OW_QTY     = 5;
static const int OW_STATUS  = 10;

static void orderTableSep(const char* l, const char* m, const char* r) {
    std::cout << "  " << l
              << rep("─", OW_ID+2)     << m
              << rep("─", OW_CUST+2)   << m
              << rep("─", OW_SID+2)    << m
              << rep("─", OW_QTY+2)    << m
              << rep("─", OW_STATUS+2) << r << "\n";
}

std::string OrderView::statusToStr(OrderStatus s) {
    switch (s) {
        case OrderStatus::RESERVED:  return "● 접수";
        case OrderStatus::REJECTED:  return "✖ 거절";
        case OrderStatus::PRODUCING: return "◆ 생산중";
        case OrderStatus::CONFIRMED: return "✔ 확정";
        case OrderStatus::RELEASE:   return "▲ 출고";
    }
    return "";
}

static const char* statusColor(OrderStatus s) {
    switch (s) {
        case OrderStatus::RESERVED:  return UI::YELLOW;
        case OrderStatus::REJECTED:  return UI::RED;
        case OrderStatus::PRODUCING: return UI::CYAN;
        case OrderStatus::CONFIRMED: return UI::B_GREEN;
        case OrderStatus::RELEASE:   return UI::BLUE;
    }
    return UI::RESET;
}

void OrderView::showOrder(const Order& o) const {
    std::string stat = statusToStr(o.getStatus());
    std::cout << "  │ "
              << padLeft(o.getId(), OW_ID)            << " │ "
              << padRight(o.getCustomer(), OW_CUST)   << " │ "
              << padLeft(o.getSampleId(), OW_SID)     << " │ "
              << padLeft(std::to_string(o.getQuantity()), OW_QTY) << " │ "
              << statusColor(o.getStatus()) << padRight(stat, OW_STATUS) << RESET
              << " │\n";
}

void OrderView::showAllOrders(const std::vector<Order>& orders) const {
    std::cout << "\n  " << B_WHITE << "주문 목록 (" << orders.size() << "건)" << RESET << "\n";
    orderTableSep("┌", "┬", "┐");
    std::cout << "  │ "
              << BOLD << padRight("ID",    OW_ID)     << RESET << " │ "
              << BOLD << padRight("고객명", OW_CUST)   << RESET << " │ "
              << BOLD << padLeft("시료 ID", OW_SID)   << RESET << " │ "
              << BOLD << padLeft("수량",   OW_QTY)    << RESET << " │ "
              << BOLD << padRight("상태",  OW_STATUS) << RESET << " │\n";
    orderTableSep("├", "┼", "┤");
    if (orders.empty()) {
        std::cout << "  │ " << DIM
                  << padRight("(주문 없음)", OW_ID+OW_CUST+OW_SID+OW_QTY+OW_STATUS+8)
                  << RESET << " │\n";
    } else {
        for (const auto& o : orders) showOrder(o);
    }
    orderTableSep("└", "┴", "┘");
}
