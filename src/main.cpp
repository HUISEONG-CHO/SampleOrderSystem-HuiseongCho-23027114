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
#include <queue>
#include <iostream>
#include <string>
#include <limits>

static void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void menuSample(SampleController& sc, SampleView& sv, MainView& mv) {
    std::cout << "\n[시료관리]\n  1. 등록\n  2. 목록\n  3. 검색\n  0. 돌아가기\n> ";
    std::string cmd; std::getline(std::cin, cmd);
    if (cmd == "1" || cmd == "등록") {
        std::string name; int t; double y;
        std::cout << "이름: "; std::getline(std::cin, name);
        std::cout << "평균생산시간(분): "; std::cin >> t; clearInput();
        std::cout << "수율(0~1): "; std::cin >> y; clearInput();
        sc.registerSample(name, t, y);
        mv.showMessage("시료 등록 완료");
    } else if (cmd == "2" || cmd == "목록") {
        sv.showAllSamples(sc.getAllSamples());
    } else if (cmd == "3" || cmd == "검색") {
        std::string kw;
        std::cout << "검색어 (ID/이름/생산시간/수율/재고): "; std::getline(std::cin, kw);
        sv.showAllSamples(sc.searchByKeyword(kw));
    }
}

static void menuOrder(OrderController& oc, MainView& mv) {
    std::string sid, cust; int qty;
    std::cout << "시료 ID: "; std::getline(std::cin, sid);
    std::cout << "고객명: ";  std::getline(std::cin, cust);
    std::cout << "수량: ";    std::cin >> qty; clearInput();
    try {
        std::string oid = oc.placeOrder(sid, cust, qty);
        mv.showMessage("주문 접수 완료: " + oid);
    } catch (const std::exception& e) { mv.showError(e.what()); }
}

static void menuApprove(OrderController& oc, OrderView& ov,
                        OrderRepository& or_, MainView& mv) {
    ov.showAllOrders(or_.findByStatus(OrderStatus::RESERVED));
    std::cout << "주문 ID: "; std::string oid; std::getline(std::cin, oid);
    std::cout << "a.승인  r.거절\n> "; std::string cmd; std::getline(std::cin, cmd);
    try {
        if      (cmd == "a") { oc.approveOrder(oid); mv.showMessage("승인 완료"); }
        else if (cmd == "r") { oc.rejectOrder(oid);  mv.showMessage("거절 완료"); }
    } catch (const std::exception& e) { mv.showError(e.what()); }
}

static void menuMonitor(MonitorController& mc, SampleController& sc,
                         MonitorView& monv) {
    monv.showOrderCounts(mc.getOrderCounts());
    for (const auto& s : sc.getAllSamples())
        monv.showStockLabel(s.getId(), s.getName(), s.getStock(),
                            mc.getStockLabel(s.getId()));
}

static void menuProduction(ProductionController& pc, ProductionView& pv, MainView& mv) {
    if (pc.hasJobs()) {
        pv.showCurrentJob(*pc.currentJob());
        std::cout << "c.생산완료  x.돌아가기\n> ";
        std::string cmd; std::getline(std::cin, cmd);
        if (cmd == "c") { pc.completeProduction(); mv.showMessage("생산 완료 처리됨"); }
    } else {
        mv.showMessage("현재 생산 중인 작업 없음");
    }
}

static void menuRelease(ReleaseController& rc, ReleaseView& rv, MainView& mv) {
    auto list = rc.getConfirmedOrders();
    rv.showConfirmedOrders(list);
    if (list.empty()) return;
    std::cout << "출고할 주문 ID: "; std::string oid; std::getline(std::cin, oid);
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
    SampleRepository          sampleRepo("samples.json");
    OrderRepository           orderRepo("orders.json");
    std::queue<ProductionJob> jobQueue;

    SampleController     sc(sampleRepo);
    OrderController      oc(sampleRepo, orderRepo, jobQueue);
    ProductionController pc(sampleRepo, orderRepo, jobQueue);
    MonitorController    mc(sampleRepo, orderRepo);
    ReleaseController    rc(sampleRepo, orderRepo);

    MainView       mv;
    SampleView     sv;
    OrderView      ov;
    MonitorView    monv;
    ProductionView pv;
    ReleaseView    rv;

    while (true) {
        mv.showMenu();
        std::string sel; std::getline(std::cin, sel);
        if      (sel == "0") { mv.showMessage("종료합니다."); break; }
        else if (sel == "1") menuSample(sc, sv, mv);
        else if (sel == "2") menuOrder(oc, mv);
        else if (sel == "3") menuApprove(oc, ov, orderRepo, mv);
        else if (sel == "4") menuMonitor(mc, sc, monv);
        else if (sel == "5") menuProduction(pc, pv, mv);
        else if (sel == "6") menuRelease(rc, rv, mv);
        else mv.showMessage("잘못된 입력입니다.");
    }
    return 0;
}
