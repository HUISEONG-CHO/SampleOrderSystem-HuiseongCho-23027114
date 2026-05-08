#include "Order.h"

Order::Order() : quantity(0), status(OrderStatus::RESERVED) {}

Order::Order(const std::string& id, const std::string& sampleId,
             const std::string& customer, int quantity,
             const std::string& createdAt)
    : id(id), sampleId(sampleId), customer(customer),
      quantity(quantity), status(OrderStatus::RESERVED), createdAt(createdAt) {}

const std::string& Order::getId()        const { return id; }
const std::string& Order::getSampleId()  const { return sampleId; }
const std::string& Order::getCustomer()  const { return customer; }
int                Order::getQuantity()  const { return quantity; }
OrderStatus        Order::getStatus()    const { return status; }
const std::string& Order::getCreatedAt() const { return createdAt; }
void               Order::setStatus(OrderStatus s) { status = s; }
