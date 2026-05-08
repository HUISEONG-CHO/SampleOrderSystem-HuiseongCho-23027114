#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
#include "controller/SampleController.h"
#include "controller/OrderController.h"
#include "controller/ProductionController.h"
#include "controller/MonitorController.h"
#include "controller/ReleaseController.h"
#include "view/MainView.h"
#include "view/SampleView.h"
#include "view/OrderView.h"
#include "view/MonitorView.h"
#include "view/ProductionView.h"
#include "view/ReleaseView.h"
#include "repository/SampleRepository.h"
#include "repository/OrderRepository.h"
#include "repository/ProductionJobRepository.h"
#include "view/ui.h"
#include <iostream>
#include <string>
#include <limits>

static void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void menuSample(SampleController& sc, SampleView& sv, MainView& mv) {
    std::cout << "\n  ┌──────────────────────────┐\n";
    std::cout << "  │  " << UI::BOLD << "시료관리" << UI::RESET << "                  │\n";
    std::cout << "  ├──────────────────────────┤\n";
    std::cout << "  │  [1]  등록               │\n";
    std::cout << "  │  [2]  목록               │\n";
    std::cout << "  │  [3]  검색               │\n";
    std::cout << "  ├──────────────────────────┤\n";
    std::cout << "  │  [0]  돌아가기           │\n";
    std::cout << "  └──────────────────────────┘\n";
    std::cout << "  " << UI::CYAN << "▶" << UI::RESET << " ";
    std::string cmd; std::getline(std::cin, cmd);
    if (cmd == "1" || cmd == "등록") {
        std::string name; int t; double y;
        std::cout << "\n  이름            : "; std::getline(std::cin, name);
        std::cout << "  평균생산시간(분): "; std::cin >> t; clearInput();
        std::cout << "  수율(0~1)       : "; std::cin >> y; clearInput();
        sc.registerSample(name, t, y);
        mv.showMessage("시료 등록 완료");
    } else if (cmd == "2" || cmd == "목록") {
        sv.showAllSamples(sc.getAllSamples());
    } else if (cmd == "3" || cmd == "검색") {
        std::string kw;
        std::cout << "\n  검색어 (ID/이름/생산시간/수율/재고) : "; std::getline(std::cin, kw);
        sv.showAllSamples(sc.searchByKeyword(kw));
    }
}

static void menuOrder(OrderController& oc, SampleController& sc, MainView& mv) {
    std::string sid, cust; int qty;
    std::cout << "\n  시료 ID : "; std::getline(std::cin, sid);

    bool exists = false;
    for (const auto& s : sc.getAllSamples())
        if (s.getId() == sid) { exists = true; break; }
    if (!exists) { mv.showError("입력한 시료가 존재하지 않습니다"); return; }

    std::cout << "  고객명  : "; std::getline(std::cin, cust);
    std::cout << "  수량    : "; std::cin >> qty; clearInput();
    try {
        std::string oid = oc.placeOrder(sid, cust, qty);
        mv.showMessage("주문 접수 완료: " + oid);
    } catch (const std::exception& e) { mv.showError(e.what()); }
}

static void menuApprove(OrderController& oc, OrderView& ov,
                        OrderRepository& or_, MainView& mv) {
    auto reserved = or_.findByStatus(OrderStatus::RESERVED);
    ov.showAllOrders(reserved);
    std::cout << "\n  주문 ID : "; std::string oid; std::getline(std::cin, oid);

    bool exists = false;
    for (const auto& o : reserved)
        if (o.getId() == oid) { exists = true; break; }
    if (!exists) { mv.showError("입력한 주문이 존재하지 않습니다"); return; }

    std::cout << "\n  ┌──────────────────────────┐\n";
    std::cout << "  │  [1]  " << UI::B_GREEN << "승인" << UI::RESET << "               │\n";
    std::cout << "  │  [2]  " << UI::B_RED   << "거절" << UI::RESET << "               │\n";
    std::cout << "  ├──────────────────────────┤\n";
    std::cout << "  │  [0]  돌아가기           │\n";
    std::cout << "  └──────────────────────────┘\n";
    std::cout << "  " << UI::CYAN << "▶" << UI::RESET << " ";
    std::string cmd; std::getline(std::cin, cmd);
    try {
        if      (cmd == "1" || cmd == "승인") { oc.approveOrder(oid); mv.showMessage("승인 완료"); }
        else if (cmd == "2" || cmd == "거절") { oc.rejectOrder(oid);  mv.showMessage("거절 완료"); }
    } catch (const std::exception& e) { mv.showError(e.what()); }
}

static void menuMonitor(MonitorController& mc, SampleController& sc,
                         ProductionController& pc, MonitorView& monv) {
    monv.showOrderCounts(mc.getOrderCounts());
    monv.showStockHeader();
    auto jobs = pc.getAllJobs();
    for (const auto& s : sc.getAllSamples()) {
        monv.showStockLabel(s.getId(), s.getName(), s.getStock(),
                            mc.getStockLabel(s.getId()));
        for (size_t i = 0; i < jobs.size(); ++i) {
            if (jobs[i].getSampleId() != s.getId()) continue;
            if (i == 0)
                monv.showProductionProgress(jobs[i], pc.elapsedMinutes(jobs[i]));
            else
                monv.showProductionWaiting(jobs[i], static_cast<int>(i + 1));
        }
    }
    monv.showStockFooter();
}

static void menuProduction(ProductionController& pc, ProductionView& pv, MainView& mv) {
    auto jobs = pc.getAllJobs();
    if (jobs.empty()) {
        mv.showMessage("현재 생산 중인 작업 없음");
        return;
    }
    // 첫 번째: 현재 생산 중
    pv.showCurrentJob(jobs.front(), pc.elapsedMinutes(jobs.front()));
    // 나머지: 대기 큐 (FIFO)
    std::vector<ProductionJob> waiting(jobs.begin() + 1, jobs.end());
    pv.showQueue(waiting);
}

static void menuRelease(ReleaseController& rc, ReleaseView& rv, MainView& mv) {
    auto list = rc.getConfirmedOrders();
    rv.showConfirmedOrders(list);
    if (list.empty()) return;
    std::cout << "\n  출고할 주문 ID : "; std::string oid; std::getline(std::cin, oid);

    bool exists = false;
    for (const auto& o : list)
        if (o.getId() == oid) { exists = true; break; }
    if (!exists) { mv.showError("입력한 주문이 존재하지 않습니다"); return; }

    try {
        rc.release(oid);
        rv.showReleaseResult(oid);
    } catch (const std::exception& e) { mv.showError(e.what()); }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    SampleRepository        sampleRepo("samples.json");
    OrderRepository         orderRepo("orders.json");
    ProductionJobRepository jobRepo("jobs.json");

    SampleController     sc(sampleRepo);
    OrderController      oc(sampleRepo, orderRepo, jobRepo);
    ProductionController pc(sampleRepo, orderRepo, jobRepo);
    MonitorController    mc(sampleRepo, orderRepo);
    ReleaseController    rc(sampleRepo, orderRepo);

    MainView       mv;
    SampleView     sv;
    OrderView      ov;
    MonitorView    monv;
    ProductionView pv;
    ReleaseView    rv;

    while (true) {
        pc.tick();
        mv.showMenu();
        std::string sel; std::getline(std::cin, sel);
        if (sel == "0" || sel == "종료") { mv.showMessage("종료합니다."); break; }

        bool acted = true;
        if      (sel == "1" || sel == "시료관리")  menuSample(sc, sv, mv);
        else if (sel == "2" || sel == "주문접수"  || sel == "시료주문 접수") menuOrder(oc, sc, mv);
        else if (sel == "3" || sel == "승인거절"  || sel == "주문 승인·거절") menuApprove(oc, ov, orderRepo, mv);
        else if (sel == "4" || sel == "모니터링"  || sel == "현황 모니터링") menuMonitor(mc, sc, pc, monv);
        else if (sel == "5" || sel == "생산라인")  menuProduction(pc, pv, mv);
        else if (sel == "6" || sel == "출고처리")  menuRelease(rc, rv, mv);
        else { mv.showMessage("잘못된 입력입니다."); acted = false; }

        if (acted) {
            std::cout << "\n  " << UI::DIM << "[ 엔터를 누르면 메뉴로 돌아갑니다 ]" << UI::RESET;
            std::cin.get();
        }
    }
    return 0;
}
