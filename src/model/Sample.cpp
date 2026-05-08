#include "Sample.h"

Sample::Sample() : avgProductionTime(0), yieldRate(0.0), stock(0) {}

Sample::Sample(const std::string& id, const std::string& name,
               int avgProductionTime, double yieldRate, int stock)
    : id(id), name(name), avgProductionTime(avgProductionTime),
      yieldRate(yieldRate), stock(stock) {}

const std::string& Sample::getId()                const { return id; }
const std::string& Sample::getName()              const { return name; }
int                Sample::getAvgProductionTime() const { return avgProductionTime; }
double             Sample::getYieldRate()         const { return yieldRate; }
int                Sample::getStock()             const { return stock; }
void               Sample::setStock(int s)              { stock = s; }
