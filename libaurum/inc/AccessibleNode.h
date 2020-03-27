#ifndef ACCESSIBLE_NODE_H
#define ACCESSIBLE_NODE_H
#include "config.h"

#include "AccessibleUtils.h"

#include <atspi/atspi.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

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

template <typename T>
class Point2D {
public:
    Point2D() : x{0}, y{0} {}
    Point2D(const Point2D &src)
    {
        x = src.x;
        y = src.y;
    }
    Point2D(const T &x, const T &y)
    {
        this->x = x;
        this->y = y;
    }
    T x;
    T y;
};

template <typename T>
class Rect {
public:
    Rect() : mTopLeft{0, 0}, mBottomRight{0, 0} {}
    Rect(const Point2D<T> &tl, const Point2D<T> &br)
        : mTopLeft(tl), mBottomRight(br)
    {
    }
    Rect(const T &x1, const T &y1, const T &x2, const T &y2)
        : mTopLeft{x1, y1}, mBottomRight{x2, y2}
    {
    }
    Rect(const Rect<T> &src)

    {
        this->mTopLeft = Point2D<int>{src.mTopLeft};
        this->mBottomRight = Point2D<int>{src.mBottomRight};
    }
    Point2D<T> midPoint() const
    {
        return Point2D<T>{mTopLeft.x + static_cast<T>(width() / 2),
                          mTopLeft.y + static_cast<T>(height() / 2)};
    }
    T width() const { return mBottomRight.x - mTopLeft.x; }

    T          height() const { return mBottomRight.y - mTopLeft.y; }
    Point2D<T> mTopLeft;
    Point2D<T> mBottomRight;
};

class AccessibleNode {
public:
    AccessibleNode();
    AccessibleNode(AtspiAccessible *node);
    ~AccessibleNode();
    static std::unique_ptr<AccessibleNode> get(AtspiAccessible *node);

public:
    int              getChildCount() const;
    std::unique_ptr<AccessibleNode> getChildAt(int index) const;
    std::unique_ptr<AccessibleNode> getParent() const;
    AtspiAccessible *getAccessible() const;

public:
    std::string getDesc() const;
    std::string getText() const;
    std::string getPkg() const;
    std::string getRes() const;
    std::string getType() const;
    std::string getStyle() const;
    Rect<int>   getBoundingBox() const;

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
    bool isVisible() const;

public:
    void print(int) const;
    void print(int, int) const;
    void refresh() const;

    void setValue(std::string &text) const;

private:
    bool isSupporting(AccessibleNodeInterface thisIface) const;
    bool hasFeatureProperty(NodeFeatureProperties prop) const;
    void setFeatureProperty(NodeFeatureProperties prop, bool has) const;
    void setFeatureProperty(AtspiStateType type) const;
    static std::map<AtspiAccessible *, AccessibleNode *> mNodeMap;

private:
    unique_ptr_gobj<AtspiAccessible> mNode;

    mutable std::string mText;
    mutable std::string mPkg;
    mutable std::string mRole;
    mutable std::string mDesc;
    mutable std::string mRes;
    mutable std::string mType;
    mutable std::string mStyle;

    mutable Rect<int> mBoundingBox;

    int  mSupportingIfaces;
    mutable int  mFeatureProperty;
    bool mIsAlive;
};

#endif
