#include "MockAccessibleWindow.h"

MockAccessibleWindow::MockAccessibleWindow(std::shared_ptr<AccessibleApplication> app, std::shared_ptr<AccessibleNode> node)
: AccessibleWindow(app, node)
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
}

MockAccessibleWindow::~MockAccessibleWindow()
{
    printf("%s:%d / %s\n",__FILE__, __LINE__, __PRETTY_FUNCTION__);
}