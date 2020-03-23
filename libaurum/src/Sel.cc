#include "Sel.h"
#include <utility>

std::shared_ptr<UiSelector> Sel::text(const std::string &text)
{
    std::shared_ptr<UiSelector> sel = std::make_shared<UiSelector>();
    sel->text(text);
    return sel;
}

std::shared_ptr<UiSelector> Sel::type(const std::string &text)
{
    std::shared_ptr<UiSelector> sel = std::make_shared<UiSelector>();
    sel->type(text);
    return sel;
}

std::shared_ptr<UiSelector> Sel::depth(const int &depth)
{
    std::shared_ptr<UiSelector> sel = std::make_shared<UiSelector>();
    sel->depth(depth);
    return sel;
}