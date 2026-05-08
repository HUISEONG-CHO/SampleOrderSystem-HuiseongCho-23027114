#pragma once
#include <string>

class Sample {
public:
    Sample();
    Sample(const std::string& id, const std::string& name,
           int avgProductionTime, double yieldRate, int stock);

    const std::string& getId()                const;
    const std::string& getName()              const;
    int                getAvgProductionTime() const;
    double             getYieldRate()         const;
    int                getStock()             const;
    void               setStock(int stock);

private:
    std::string id;
    std::string name;
    int         avgProductionTime;
    double      yieldRate;
    int         stock;
};
