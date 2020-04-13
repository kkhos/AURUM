#ifndef SEL_H
#define SEL_H
#include "config.h"

#include <memory>
#include <string>

#include "UiSelector.h"

class Sel {
public:
    static std::shared_ptr<UiSelector> text(std::string text);
    static std::shared_ptr<UiSelector> type(std::string text);
    static std::shared_ptr<UiSelector> style(std::string text);
    static std::shared_ptr<UiSelector> depth(int depth);
};

#endif