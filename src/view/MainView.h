#pragma once
#include <string>

class MainView {
public:
    void showMenu() const;
    void showMessage(const std::string& msg) const;
    void showError(const std::string& err) const;
};
