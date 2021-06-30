#include "Aurum.h"
#include <utility>

std::shared_ptr<UiSelector> Sel::text(std::string text)
{
    std::shared_ptr<UiSelector> sel = std::make_shared<UiSelector>();
    sel->text(text);
    return sel;
}

std::shared_ptr<UiSelector> Sel::type(std::string text)
{
    std::shared_ptr<UiSelector> sel = std::make_shared<UiSelector>();
    sel->type(text);
    return sel;
}

std::shared_ptr<UiSelector> Sel::style(std::string text)
{
    std::shared_ptr<UiSelector> sel = std::make_shared<UiSelector>();
    sel->style(text);
    return sel;
}

std::shared_ptr<UiSelector> Sel::depth(int depth)
{
    std::shared_ptr<UiSelector> sel = std::make_shared<UiSelector>();
    sel->depth(depth);
    return sel;
}