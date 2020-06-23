#ifndef ACCESSIBLE_NODE_H
#define ACCESSIBLE_NODE_H
#include <atspi/atspi.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "AccessibleUtils.h"
#include "config.h"

/**
 * @brief AccessibleNodeInterface enum class
 * @since_tizen 5.5
 */
enum class AccessibleNodeInterface {
    ACTION = 0x0001,
    COLLECTION = 0X0002,
    COMPONENT = 0X0004,
    DOCUMENT = 0X0008,

    EDITABLETEXT = 0X0010,
    HYPERTEXT = 0X0020,
    IMAGE = 0X0040,
    SELECTION = 0X0080,

    TEXT = 0X0100,
    VALUE = 0X0200,
    ACCESSIBLE = 0X0400,
    TABLE = 0X0800,

    TABLECELL = 0X1000,
};

/**
 * @brief NodeFeatureProperties enum class
 * @since_tizen 5.5
 */
enum class NodeFeatureProperties {
    CHECKABLE = 0x0001,
    CHECKED = 0X0002,
    CLICKABLE = 0X0004,
    ENABLED = 0X0008,
    FOCUSABLE = 0X0010,
    FOCUSED = 0X0020,
    LONGCLICKABLE = 0X0040,
    SCROLLABLE = 0X0080,

    SELECTABLE = 0X0100,
    SELECTED = 0X0200,
    VISIBLE = 0X0400,
    SHOWING = 0X0800,
    ACTIVE = 0X0800,
};

/**
 * @brief Point2d Class
 * @since_tizen 5.5
 */
template <typename T>
class Point2D {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Point2D() : x{0}, y{0} {}

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Point2D(const Point2D &src)
    {
        x = src.x;
        y = src.y;
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Point2D(const T &x, const T &y)
    {
        this->x = x;
        this->y = y;
    }

    /**
     * @brief TBD
     */
    T x;

    /**
     * @brief TBD
     */
    T y;
};

/**
 * @brief Rect Class
 * @since_tizen 5.5
 */
template <typename T>
class Rect {
public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Rect() : mTopLeft{0, 0}, mBottomRight{0, 0} {}

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Rect(const Point2D<T> &tl, const Point2D<T> &br)
        : mTopLeft(tl), mBottomRight(br)
    {
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Rect(const T &x1, const T &y1, const T &x2, const T &y2)
        : mTopLeft{x1, y1}, mBottomRight{x2, y2}
    {
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Rect(const Rect<T> &src)

    {
        this->mTopLeft = Point2D<int>{src.mTopLeft};
        this->mBottomRight = Point2D<int>{src.mBottomRight};
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    Point2D<T> midPoint() const
    {
        return Point2D<T>{mTopLeft.x + static_cast<T>(width() / 2),
                          mTopLeft.y + static_cast<T>(height() / 2)};
    }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    T width() const { return mBottomRight.x - mTopLeft.x; }

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    T height() const { return mBottomRight.y - mTopLeft.y; }

    /**
     * @brief TBD
     */
    Point2D<T> mTopLeft;

    /**
     * @brief TBD
     */
    Point2D<T> mBottomRight;
};

/**
 * @brief AccessibleNode Class
 * @since_tizen 5.5
 */
class AccessibleNode {
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
    AccessibleNode(AtspiAccessible *node);

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    ~AccessibleNode();

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::unique_ptr<AccessibleNode> get(AtspiAccessible *node);

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    int getChildCount() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<AccessibleNode> getChildAt(int index) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::unique_ptr<AccessibleNode> getParent() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    AtspiAccessible *getAccessible() const;

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::string getDesc() const;

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
    std::string getRes() const;

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

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    std::vector<std::string> getActions() const;

public:
    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void print(int) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void print(int, int) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void refresh() const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void setValue(std::string text) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    bool doAction(std::string action) const;

private:
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
    void setFeatureProperty(NodeFeatureProperties prop, bool has) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    void setFeatureProperty(AtspiStateType type) const;

    /**
     * @brief TBD
     * @since_tizen 5.5
     */
    static std::map<AtspiAccessible *, AccessibleNode *> mNodeMap;

private:
    /**
     * @brief TBD
     */
    unique_ptr_gobj<AtspiAccessible> mNode;

    mutable std::string mText;

    /**
     * @brief TBD
     */
    mutable std::string mPkg;

    /**
     * @brief TBD
     */
    mutable std::string mRole;

    /**
     * @brief TBD
     */
    mutable std::string mDesc;

    /**
     * @brief TBD
     */
    mutable std::string mRes;

    /**
     * @brief TBD
     */
    mutable std::string mType;

    /**
     * @brief TBD
     */
    mutable std::string mStyle;

    /**
     * @brief TBD
     */
    mutable Rect<int> mBoundingBox;

    /**
     * @brief TBD
     */
    int mSupportingIfaces;

    /**
     * @brief TBD
     */
    mutable int mFeatureProperty;

    /**
     * @brief TBD
     */
    bool mIsAlive;
};

#endif
