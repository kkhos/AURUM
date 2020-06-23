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
/**
 * @brief UiObject class
 * @since_tizen 5.5
 */
class UiObject : public ISearchable {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiObject(const UiDevice *device, const std::shared_ptr<UiSelector> selector,
             const AccessibleNode *node);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    UiObject(const UiDevice *device, const std::shared_ptr<UiSelector> selector,
             std::unique_ptr<AccessibleNode> node);
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
    std::unique_ptr<UiObject> findObject(
        const std::shared_ptr<UiSelector> selector) const override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::unique_ptr<UiObject>> findObjects(
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
    std::unique_ptr<UiObject> waitFor(
        const std::function<std::unique_ptr<UiObject>(const ISearchable *)>
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
    std::vector<std::unique_ptr<UiObject>> getChildren() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getContentDescription() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getApplicationPackage() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getResourceName() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getText() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void        setText(std::string text);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    const Rect<int> getBoundingBox() const;

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

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void refresh() const;

private:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    const AccessibleNode *getAccessibleNode() const;

private:
    /**
     * @brief TBD
     */
    const UiDevice *      mDevice;

    /**
     * @brief TBD
     */
    std::shared_ptr<UiSelector>  mSelector;

    /**
     * @brief TBD
     */
    std::unique_ptr<AccessibleNode> mNode;

    /**
     * @brief TBD
     */
    const Waiter *        mWaiter;
    //std::unique_ptr<AccessibleNode> mNode_src;

    /**
     * @brief TBD
     */
    static const unsigned int LOGNCLICK_INTERVAL = 50;
};

#endif