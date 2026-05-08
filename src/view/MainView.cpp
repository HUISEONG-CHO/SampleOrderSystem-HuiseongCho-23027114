#include "MainView.h"
#include <iostream>

void MainView::showMenu() const {
    std::cout << "\n=============================\n";
    std::cout << "   S-Semi 시료 생산주문관리\n";
    std::cout << "=============================\n";
    std::cout << "  1. 시료관리\n";
    std::cout << "  2. 시료주문 접수\n";
    std::cout << "  3. 주문 승인·거절\n";
    std::cout << "  4. 현황 모니터링\n";
    std::cout << "  5. 생산라인\n";
    std::cout << "  6. 출고처리\n";
    std::cout << "  0. 종료\n";
    std::cout << "=============================\n";
    std::cout << "선택: ";
}

void MainView::showMessage(const std::string& msg) const {
    std::cout << msg << "\n";
}

void MainView::showError(const std::string& err) const {
    std::cout << "[오류] " << err << "\n";
}
