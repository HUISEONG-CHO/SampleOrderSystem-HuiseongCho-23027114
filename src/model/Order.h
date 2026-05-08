#pragma once
#include <string>

enum class OrderStatus {
    RESERVED,
    REJECTED,
    PRODUCING,
    CONFIRMED,
    RELEASE
};

class Order {
public:
    Order();
    Order(const std::string& id, const std::string& sampleId,
          const std::string& customer, int quantity,
          const std::string& createdAt);

    const std::string& getId()        const;
    const std::string& getSampleId()  const;
    const std::string& getCustomer()  const;
    int                getQuantity()  const;
    OrderStatus        getStatus()    const;
    const std::string& getCreatedAt() const;
    void               setStatus(OrderStatus status);

private:
    std::string id;
    std::string sampleId;
    std::string customer;
    int         quantity;
    OrderStatus status;
    std::string createdAt;
};
