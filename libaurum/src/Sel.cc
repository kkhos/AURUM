#include "Sel.h"
#include <utility>

std::unique_ptr<UiSelector> Sel::text(const std::string &text)
{
    std::unique_ptr<UiSelector> sel = std::make_unique<UiSelector>();
    sel->text(text);
    return sel;
}

std::unique_ptr<UiSelector> Sel::type(const std::string &text)
{
    std::unique_ptr<UiSelector> sel = std::make_unique<UiSelector>();
    sel->type(text);
    return sel;
}

std::unique_ptr<UiSelector> Sel::depth(const int &depth)
{
    std::unique_ptr<UiSelector> sel = std::make_unique<UiSelector>();
    sel->depth(depth);
    return sel;
}