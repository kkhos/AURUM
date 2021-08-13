#pragma once

#include "config.h"

#include "AccessibleNode.h"
#include "ISearchable.h"
#include "UiSelector.h"
#include "Waiter.h"

#include <memory>
#include <vector>

class UiDevice;

class Node : public std::enable_shared_from_this<Node> {
public:
    Node(std::shared_ptr<UiObject> node, std::vector<std::shared_ptr<Node>> children)
    :mNode{node}, mChildren{children}{}

    std::shared_ptr<UiObject> mNode;
    std::vector<std::shared_ptr<Node>> mChildren;
};

/**
 * @brief UiObject class
 * @since_tizen 5.5
 */
class UiObject : public ISearchable , public std::enable_shared_from_this<UiObject> {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiObject(const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
             const AccessibleNode *node);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiObject(const std::shared_ptr<UiDevice> device, const std::shared_ptr<UiSelector> selector,
             std::shared_ptr<AccessibleNode> node);
//    UiObject(const UiObject &src);  // copy constroctur

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiObject(UiObject &&src);       // move constructor

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiObject();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual ~UiObject();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<UiSelector> getSelector();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool hasObject(const std::shared_ptr<UiSelector> selector) const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<UiObject> findObject(
        const std::shared_ptr<UiSelector> selector) const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::shared_ptr<UiObject>> findObjects(
        const std::shared_ptr<UiSelector> selector) const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool waitFor(
        const std::function<bool(const ISearchable *)> condition) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<UiObject> waitFor(
        const std::function<std::shared_ptr<UiObject>(const ISearchable *)>
            condition) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool waitFor(const std::function<bool(const UiObject *)> condition) const;

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiObject *                             getParent() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    int                                    getChildCount() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::shared_ptr<UiObject>> getChildren() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<UiObject> getChildAt(int index) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<Node> getDescendant();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getApplicationPackage() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getId() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getAutomationId() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getElementType() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getElementStyle() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getText() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getRole() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void setText(std::string text);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    const Rect<int> getScreenBoundingBox() const;

    /**
     * @brief TBD
     * @since_tizen 6.5
     */
    const Rect<int> getWindowBoundingBox() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isCheckable() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isChecked() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isClickable() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isEnabled() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isFocusable() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isFocused() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isLongClickable() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isScrollable() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isSelectable() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isSelected() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isVisible() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isShowing() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isActive() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void click() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void longClick(const unsigned int intv = LOGNCLICK_INTERVAL) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool DoAtspiActivate() const;

    void updateRoleName() const;

    void updateUniqueId() const;

    void updateName() const;

    void updateApplication() const;

    void updateAttributes() const;

    void updateStates() const;

    void updateExtents() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void refresh() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isValid() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::shared_ptr<AccessibleNode> getAccessibleNode() const;

private:
    /**
     * @brief TBD
     */
    std::shared_ptr<UiDevice> mDevice;

    /**
     * @brief TBD
     */
    std::shared_ptr<UiSelector>  mSelector;

    /**
     * @brief TBD
     */
    std::shared_ptr<AccessibleNode> mNode;

    /**
     * @brief TBD
     */
    const Waiter *mWaiter;

    /**
     * @brief TBD
     */
    static const unsigned int LOGNCLICK_INTERVAL = 500;
};
