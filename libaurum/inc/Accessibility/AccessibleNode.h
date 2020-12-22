#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <mutex>

#include "AccessibleUtils.h"
#include "IEventConsumer.h"

#include "Rect.h"
#include "config.h"

/**
 * @brief AccessibleNodeInterface enum class
 * @since_tizen 5.5
 */
enum class AccessibleNodeInterface {
    ACTION          = 0x0001,
    COLLECTION      = 0X0002,
    COMPONENT       = 0X0004,
    DOCUMENT        = 0X0008,

    EDITABLETEXT    = 0X0010,
    HYPERTEXT       = 0X0020,
    IMAGE           = 0X0040,
    SELECTION       = 0X0080,

    TEXT            = 0X0100,
    VALUE           = 0X0200,
    ACCESSIBLE      = 0X0400,
    TABLE           = 0X0800,

    TABLECELL       = 0X1000,
};

/**
 * @brief NodeFeatureProperties enum class
 * @since_tizen 5.5
 */
enum class NodeFeatureProperties {
    CHECKABLE       = 0x0001,
    CHECKED         = 0X0002,
    CLICKABLE       = 0X0004,
    ENABLED         = 0X0008,

    FOCUSABLE       = 0X0010,
    FOCUSED         = 0X0020,
    LONGCLICKABLE   = 0X0040,
    SCROLLABLE      = 0X0080,

    SELECTABLE      = 0X0100,
    SELECTED        = 0X0200,
    VISIBLE         = 0X0400,
    SHOWING         = 0X0800,
    ACTIVE          = 0X1000,
    INVALID         = 0X2000,
};

class IAccessibleNode {
public:
    virtual ~IAccessibleNode() = 0;

    
};

/**
 * @brief AccessibleNode Class
 * @since_tizen 5.5
 */
class AccessibleNode : public std::enable_shared_from_this<AccessibleNode>, public IEventConsumer  {
public:  // Constructor & Destructor
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    AccessibleNode();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual ~AccessibleNode();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual int getChildCount() const = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::shared_ptr<AccessibleNode> getChildAt(int index) const = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::vector<std::shared_ptr<AccessibleNode>> getChildren() const = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::shared_ptr<AccessibleNode> getParent() const = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void notify(int type, int type2, void *src) override;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void invalidate();

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getText() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getPkg() const;

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
    std::string getRole() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getType() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getStyle() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Rect<int> getBoundingBox() const;

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

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void print(int);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void print(int, int);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual void* getRawHandler(void) const = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual void refresh() = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual std::vector<std::string> getActions() const = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool doAction(std::string action) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual void setValue(std::string text) = 0;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    virtual bool isValid() const;

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool isSupporting(AccessibleNodeInterface thisIface) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool hasFeatureProperty(NodeFeatureProperties prop) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void setFeatureProperty(NodeFeatureProperties prop, bool has);

protected:
    /**
     * @brief TBD
     */
    std::string mText;

    /**
     * @brief TBD
     */
    std::string mPkg;

    /**
     * @brief TBD
     */
    std::string mRole;

    /**
     * @brief TBD
     */
    std::string mId;

    /**
     * @brief TBD
     */
    std::string mAutomationId;

    /**
     * @brief TBD
     */
    std::string mType;

    /**
     * @brief TBD
     */
    std::string mStyle;

    /**
     * @brief TBD
     */
    Rect<int> mBoundingBox;

    /**
     * @brief TBD
     */
    int mSupportingIfaces;

    /**
     * @brief TBD
     */
    int mFeatureProperty;

private:
    /**
     * @brief TBD
     */
    bool mValid;

    /**
     * @brief TBD
     */
    mutable std::mutex mLock;
};