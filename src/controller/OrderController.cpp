#include "OrderController.h"
#include <cmath>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

OrderController::OrderController(SampleRepository& sampleRepo,
                                 OrderRepository&  orderRepo,
                                 std::queue<ProductionJob>& productionQueue)
    : sampleRepo(sampleRepo), orderRepo(orderRepo), productionQueue(productionQueue) {}

std::string OrderController::generateOrderId() const {
    return std::to_string(orderRepo.findAll().size() + 1);
}

std::string OrderController::currentTimestamp() const {
    auto now  = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return oss.str();
}

std::string OrderController::placeOrder(const std::string& sampleId,
                                        const std::string& customer,
                                        int quantity) {
    sampleRepo.findById(sampleId); // 존재 검증 (없으면 예외)
    std::string id = generateOrderId();
    orderRepo.save(Order(id, sampleId, customer, quantity, currentTimestamp()));
    return id;
}

void OrderController::approveOrder(const std::string& orderId) {
    Order   order  = orderRepo.findById(orderId);
    Sample  sample = sampleRepo.findById(order.getSampleId());

    int stock    = sample.getStock();
    int quantity = order.getQuantity();

    if (stock >= quantity) {
        order.setStatus(OrderStatus::CONFIRMED);
        orderRepo.update(orderId, order);
    } else {
        int shortage      = quantity - stock;
        int productionQty = static_cast<int>(
            std::ceil(static_cast<double>(shortage) / (sample.getYieldRate() * 0.9)));
        int totalTime     = sample.getAvgProductionTime() * productionQty;

        ProductionJob job(orderId, order.getSampleId(),
                          productionQty, totalTime, currentTimestamp());
        productionQueue.push(job);

        order.setStatus(OrderStatus::PRODUCING);
        orderRepo.update(orderId, order);
    }
}

void OrderController::rejectOrder(const std::string& orderId) {
    Order order = orderRepo.findById(orderId);
    order.setStatus(OrderStatus::REJECTED);
    orderRepo.update(orderId, order);
}
