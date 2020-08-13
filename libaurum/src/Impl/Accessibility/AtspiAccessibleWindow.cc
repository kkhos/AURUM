#include "AtspiAccessibleWindow.h"


AtspiAccessibleWindow::AtspiAccessibleWindow(std::shared_ptr<AccessibleApplication> app, std::shared_ptr<AccessibleNode> node)
: AccessibleWindow(app, node)
{

}

AtspiAccessibleWindow::~AtspiAccessibleWindow()
{

}