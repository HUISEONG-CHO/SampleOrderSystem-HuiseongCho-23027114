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

static std::string nowTimestamp() {
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

ProductionController::ProductionController(SampleRepository&        sampleRepo,
                                           OrderRepository&         orderRepo,
                                           ProductionJobRepository& jobRepo)
    : sampleRepo(sampleRepo), orderRepo(orderRepo), jobRepo(jobRepo) {}

void ProductionController::tick() {
    auto now  = std::chrono::system_clock::now();
    auto jobs = jobRepo.findAll();

    // FIFO 직렬: 첫 번째 잡만 실행, 완료 시 다음 잡 시작
    if (!jobs.empty()) {
        const auto& first    = jobs.front();
        auto enqueued = parseISO8601(first.getEnqueuedAt());
        auto elapsed  = std::chrono::duration_cast<std::chrono::minutes>(now - enqueued).count();

        if (elapsed >= first.getTotalTime()) {
            Sample sample = sampleRepo.findById(first.getSampleId());
            sample.setStock(sample.getStock() + first.getProductionQty());
            sampleRepo.update(first.getSampleId(), sample);

            Order order = orderRepo.findById(first.getOrderId());
            order.setStatus(OrderStatus::CONFIRMED);
            orderRepo.update(first.getOrderId(), order);

            jobRepo.remove(first.getOrderId());

            // 다음 잡의 enqueuedAt을 지금으로 리셋 → 직렬 시작
            auto remaining = jobRepo.findAll();
            if (!remaining.empty()) {
                const auto& next = remaining.front();
                ProductionJob reset(next.getOrderId(), next.getSampleId(),
                                    next.getProductionQty(), next.getTotalTime(),
                                    nowTimestamp());
                jobRepo.update(next.getOrderId(), reset);
            }
        }
    }

    // PRODUCING 상태인데 대응 Job이 없는 고아 주문 → RESERVED로 복원
    auto currentJobs = jobRepo.findAll();
    for (const auto& order : orderRepo.findByStatus(OrderStatus::PRODUCING)) {
        bool hasJob = false;
        for (const auto& job : currentJobs)
            if (job.getOrderId() == order.getId()) { hasJob = true; break; }
        if (!hasJob) {
            Order o = order;
            o.setStatus(OrderStatus::RESERVED);
            orderRepo.update(order.getId(), o);
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

std::vector<ProductionJob> ProductionController::getAllJobs() const {
    return jobRepo.findAll();
}

int ProductionController::elapsedMinutes(const ProductionJob& job) const {
    auto now      = std::chrono::system_clock::now();
    auto enqueued = parseISO8601(job.getEnqueuedAt());
    return static_cast<int>(
        std::chrono::duration_cast<std::chrono::minutes>(now - enqueued).count());
}
