#ifndef UI_OBJECT_H
#define UI_OBJECT_H
#include "config.h"

#include "AccessibleNode.h"
#include "ISearchable.h"
#include "UiSelector.h"
#include "Waiter.h"

#include <memory>
#include <vector>

class UiDevice;

class UiObject : public ISearchable {
public:
    UiObject(const UiDevice *device, const UiSelector *selector,
             const AccessibleNode *node);
    UiObject(const UiObject &src);  // copy constroctur
    UiObject(UiObject &&src);       // move constructor

    virtual ~UiObject();

    bool hasObject(const UiSelector *selector) const override;
    std::unique_ptr<UiObject> findObject(
        const UiSelector *selector) const override;
    std::vector<std::unique_ptr<UiObject>> findObjects(
        const UiSelector *selector) const override;

    bool waitFor(
        const std::function<bool(const ISearchable *)> condition) const;
    std::unique_ptr<UiObject> waitFor(
        const std::function<std::unique_ptr<UiObject>(const ISearchable *)>
            condition) const;
    bool waitFor(const std::function<bool(const UiObject *)> condition) const;

public:
    UiObject *                             getParent() const;
    int                                    getChildCount() const;
    std::vector<std::unique_ptr<UiObject>> getChildren() const;

    std::string getContentDescription() const;
    std::string getApplicationPackage() const;
    std::string getResourceName() const;

    std::string getText() const;
    void        setText(std::string &text);

    bool isCheckable() const;
    bool isChecked() const;
    bool isClickable() const;
    bool isEnabled() const;
    bool isFocusable() const;
    bool isFocused() const;
    bool isLongClickable() const;
    bool isScrollable() const;
    bool isSelectable() const;
    bool isSelected() const;

    void click() const;
    void refresh() const;

private:
    UiObject();
    const AccessibleNode *getAccessibleNode() const;

private:
    const UiDevice *      mDevice;
    const UiSelector *    mSelector;
    const AccessibleNode *mNode;
    const Waiter *        mWaiter;
};

#endif