#pragma once
#include "model/Order.h"
#include <map>
#include <string>

class MonitorView {
public:
    void showOrderCounts(const std::map<OrderStatus, int>& counts) const;
    void showStockLabel(const std::string& sampleId, const std::string& name,
                        int stock, const std::string& label) const;
};
