#pragma once
#include "IRepository.h"
#include "model/Order.h"
#include <string>
#include <vector>

class OrderRepository : public IRepository<Order> {
public:
    explicit OrderRepository(const std::string& filePath);

    void           save(const Order& item)                    override;
    Order          findById(const std::string& id) const      override;
    std::vector<Order> findAll() const                        override;
    void           update(const std::string& id, const Order& item) override;
    void           remove(const std::string& id)              override;

    std::vector<Order> findByStatus(OrderStatus status) const;

private:
    std::string        filePath;
    std::vector<Order> orders;

    void loadFromFile();
    void saveToFile() const;

    static std::string    statusToString(OrderStatus s);
    static OrderStatus    stringToStatus(const std::string& s);
};
