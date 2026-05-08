#include "ProductionController.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

static std::chrono::system_clock::time_point parseISO8601(const std::string& s) {
    std::tm tm{};
    std::istringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

ProductionController::ProductionController(SampleRepository&        sampleRepo,
                                           OrderRepository&         orderRepo,
                                           ProductionJobRepository& jobRepo)
    : sampleRepo(sampleRepo), orderRepo(orderRepo), jobRepo(jobRepo) {}

void ProductionController::tick() {
    auto now  = std::chrono::system_clock::now();
    auto jobs = jobRepo.findAll();
    for (const auto& job : jobs) {
        auto enqueued = parseISO8601(job.getEnqueuedAt());
        auto elapsed  = std::chrono::duration_cast<std::chrono::minutes>(now - enqueued).count();
        if (elapsed >= job.getTotalTime()) {
            Sample sample = sampleRepo.findById(job.getSampleId());
            sample.setStock(sample.getStock() + job.getProductionQty());
            sampleRepo.update(job.getSampleId(), sample);

            Order order = orderRepo.findById(job.getOrderId());
            order.setStatus(OrderStatus::CONFIRMED);
            orderRepo.update(job.getOrderId(), order);

            jobRepo.remove(job.getOrderId());
        }
    }
}

bool ProductionController::hasJobs() const {
    return !jobRepo.findAll().empty();
}

const ProductionJob* ProductionController::currentJob() const {
    static thread_local std::vector<ProductionJob> cache;
    cache = jobRepo.findAll();
    if (cache.empty()) return nullptr;
    return &cache.front();
}

int ProductionController::elapsedMinutes(const ProductionJob& job) const {
    auto now      = std::chrono::system_clock::now();
    auto enqueued = parseISO8601(job.getEnqueuedAt());
    return static_cast<int>(
        std::chrono::duration_cast<std::chrono::minutes>(now - enqueued).count());
}
