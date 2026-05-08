#pragma once
#include <vector>
#include <string>

template <typename T>
class IRepository {
public:
    virtual void           save(const T& item)                   = 0;
    virtual T              findById(const std::string& id) const = 0;
    virtual std::vector<T> findAll() const                       = 0;
    virtual void           update(const std::string& id, const T& item) = 0;
    virtual void           remove(const std::string& id)         = 0;
    virtual ~IRepository() = default;
};
