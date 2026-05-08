#include "ReleaseController.h"
#include <stdexcept>

ReleaseController::ReleaseController(SampleRepository& sampleRepo,
                                     OrderRepository&  orderRepo)
    : sampleRepo(sampleRepo), orderRepo(orderRepo) {}

std::vector<Order> ReleaseController::getConfirmedOrders() const {
    return orderRepo.findByStatus(OrderStatus::CONFIRMED);
}

void ReleaseController::release(const std::string& orderId) {
    Order order = orderRepo.findById(orderId);
    if (order.getStatus() != OrderStatus::CONFIRMED)
        throw std::runtime_error("Order is not CONFIRMED: " + orderId);

    Sample sample = sampleRepo.findById(order.getSampleId());
    sample.setStock(sample.getStock() - order.getQuantity());
    sampleRepo.update(order.getSampleId(), sample);

    order.setStatus(OrderStatus::RELEASE);
    orderRepo.update(orderId, order);
}
