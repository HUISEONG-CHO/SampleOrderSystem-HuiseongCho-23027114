#include "MainView.h"
#include <iostream>
#include <string>

// UTF-8 문자열 표시 너비 계산 (한글/CJK = 2칸, ASCII = 1칸)
static size_t displayWidth(const std::string& s) {
    size_t w = 0;
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = s[i];
        if      (c < 0x80) { w += 1; i += 1; }
        else if (c < 0xE0) { w += 1; i += 2; }
        else if (c < 0xF0) { w += 2; i += 3; }
        else               { w += 2; i += 4; }
    }
    return w;
}

static void printBox(const std::string& msg, char borderChar = '-') {
    const size_t pad = 2;
    size_t inner = displayWidth(msg) + pad * 2;
    std::string hLine(inner, borderChar);
    std::string spaces(pad, ' ');
    std::cout << "+" << hLine << "+\n";
    std::cout << "|" << spaces << msg << spaces << "|\n";
    std::cout << "+" << hLine << "+\n";
}

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
    std::cout << "\n";
    printBox(msg, '-');
}

void MainView::showError(const std::string& err) const {
    std::cout << "\n";
    printBox("오류: " + err, '-');
}
