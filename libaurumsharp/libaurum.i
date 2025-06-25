%module aurum

%include <std_string.i>
%include <std_vector.i>
%include <std_shared_ptr.i>

%ignore Aurum::AccessibleWatcher::IEventSource;
%ignore Aurum::AccessibleWatcher::getXMLDoc;
%ignore Aurum::AccessibleWatcher::getXMLDocMap;
%ignore Aurum::AccessibleWatcher::attach;
%ignore Aurum::AccessibleWatcher::detach;
%ignore Aurum::AccessibleWatcher::notifyAll;
%ignore Aurum::AccessibleWatcher::registerCallback;
%ignore Aurum::AccessibleWatcher::executeAndWaitForEvents;

%ignore Aurum::AccessibleNode::notify;
%ignore Aurum::AccessibleNode::getActions;
%ignore Aurum::AccessibleNode::getChildren;
%ignore Aurum::AccessibleNode::getMatches;
%ignore Aurum::AccessibleNode::getMatchesInMatches;
%ignore Aurum::AccessibleNode::getRawHandler;
%ignore Aurum::AccessibleNode::setText;

%ignore Aurum::IDevice::getWindowRoot;

%ignore Aurum::UiSelector::UiSelector(UiSelector &selector);

%ignore Aurum::UiDevice::IDevice;
%ignore Aurum::UiDevice::getWindowRoot;
%ignore Aurum::UiDevice::registerCallback;
%ignore Aurum::UiDevice::executeAndWaitForEvents;

%ignore Aurum::Until::~Until;

%ignore Aurum::Rect::operator==;
%ignore Aurum::Rect::operator!=;

%rename(equal) Aurum::Point2D::operator==;
%rename(diff) Aurum::Point2D::operator!=;

%rename(equal) Aurum::Rect::operator==;
%rename(diff) Aurum::Rect::operator!=;

%rename(equal) Aurum::Size2D::operator==;
%rename(diff) Aurum::Size2D::operator!=;

%{
#include "Misc/Point2D.h"
#include "Misc/Rect.h"
#include "Misc/Size2D.h"
#include "UiSelector.h"
#include "Sel.h"
#include "Interface/ISearchable.h"
#include "Until.h"
#include "Waiter.h"
#include "Accessibility/AccessibleNode.h"
#include "A11yEvent.h"
#include "A11yEventHandler.h"
#include "Accessibility/AccessibleWatcher.h"
#include "Interface/IDevice.h"
#include "UiObject.h"
#include "UiDevice.h"
%}

%shared_ptr(Aurum::AccessibleNode)
%shared_ptr(Aurum::AccessibleWatcher)
%shared_ptr(Aurum::UiSelector)
%shared_ptr(Aurum::UiObject)
%shared_ptr(Aurum::Node)
%shared_ptr(Aurum::UiDevice)

%template(NodeVector) std::vector<std::shared_ptr<Aurum::Node>>;
%template(UiObjectVector) std::vector<std::shared_ptr<Aurum::UiObject>>;
%template(UiSelectorVector) std::vector<std::shared_ptr<Aurum::UiSelector>>;

%include "Misc/Size2D.h"
%template(Size2D) Aurum::Size2D<int>;
%include "Misc/Point2D.h"
%template(Point2D) Aurum::Point2D<int>;
%include "Misc/Rect.h"
%template(Rect) Aurum::Rect<int>;

%include "UiSelector.h"
%include "Sel.h"
%include "Interface/ISearchable.h"
%include "Until.h"
%include "Waiter.h"
%include "A11yEvent.h"
%include "Accessibility/AccessibleNode.h"
%include "Accessibility/AccessibleWatcher.h"
%include "Interface/IDevice.h"
%include "UiObject.h"
%include "UiDevice.h"
