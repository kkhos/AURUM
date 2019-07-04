#ifndef SEL_H
#define SEL_H
#include "config.h"

#include <memory>
#include <string>

#include "UiSelector.h"

class Sel {
public:
    static std::unique_ptr<UiSelector> text(const std::string &text);
    static std::unique_ptr<UiSelector> type(const std::string &text);
    static std::unique_ptr<UiSelector> depth(const int &depth);
};

#endif