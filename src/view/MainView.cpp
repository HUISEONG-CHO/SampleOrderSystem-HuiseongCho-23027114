#include "MainView.h"
#include "ui.h"
#include <iostream>

using namespace UI;

void MainView::showMenu() const {
    std::cout << "\n";
    std::cout << "  ╔══════════════════════════════════════╗\n";
    std::cout << "  ║                                      ║\n";
    std::cout << "  ║    " << B_CYAN << "S-Semi 생산주문관리 시스템" << RESET << "     ║\n";
    std::cout << "  ║                                      ║\n";
    std::cout << "  ╠══════════════════════════════════════╣\n";
    std::cout << "  ║   [1]  시료관리                      ║\n";
    std::cout << "  ║   [2]  시료주문 접수                 ║\n";
    std::cout << "  ║   [3]  주문 승인·거절                ║\n";
    std::cout << "  ║   [4]  현황 모니터링                 ║\n";
    std::cout << "  ║   [5]  생산라인                      ║\n";
    std::cout << "  ║   [6]  출고처리                      ║\n";
    std::cout << "  ╠══════════════════════════════════════╣\n";
    std::cout << "  ║   [0]  종료                          ║\n";
    std::cout << "  ╚══════════════════════════════════════╝\n";
    std::cout << "\n  선택 " << CYAN << "▶" << RESET << " ";
}

void MainView::showMessage(const std::string& msg) const {
    std::string inner = "  " + msg + "  ";
    size_t w = displayWidth(inner);
    std::string top = "┌" + rep("─", (int)w) + "┐";
    std::string bot = "└" + rep("─", (int)w) + "┘";
    std::cout << "\n  " << B_GREEN << top << RESET << "\n";
    std::cout << "  " << B_GREEN << "│" << RESET << inner << B_GREEN << "│" << RESET << "\n";
    std::cout << "  " << B_GREEN << bot << RESET << "\n";
}

void MainView::showError(const std::string& err) const {
    std::string inner = "  ✖  " + err + "  ";
    size_t w = displayWidth(inner);
    std::string top = "┌" + rep("─", (int)w) + "┐";
    std::string bot = "└" + rep("─", (int)w) + "┘";
    std::cout << "\n  " << B_RED << top << RESET << "\n";
    std::cout << "  " << B_RED << "│" << RESET << inner << B_RED << "│" << RESET << "\n";
    std::cout << "  " << B_RED << bot << RESET << "\n";
}
