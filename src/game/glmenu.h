#ifndef GLMENU_H
#define GLMENU_H

#include <QtGui>

struct MenuItem {
    MenuItem(const QString& text, bool enabled = true, int id=0);
    QString Text;
    bool Enabled;
    int  ID;
};

class GLMenu
{
public:
    GLMenu(int itemHeight, const QRect& r);
    ~GLMenu();
    void Add(const MenuItem& add);
    void Remove(size_t i);
    int CurrentTop() const;
    MenuItem CurrentItem() const;
    QRect& Bounds();
    void Draw(QPainter& painter, QPen& pen,const QColor& selectedColor, const QColor &disabledColor = QColor::fromRgb(64,64,64));
    void Resize(const QRect& r);
    MenuItem& operator[](size_t i);
    MenuItem& at(size_t i);
    int operator--();
    int operator++();
private:
    std::vector<MenuItem> items;
    int itemIndex, menuIndex;
    QRect bound;
    int fontHeight, maxItems;
    int* top;
};

#endif // GLMENU_H
