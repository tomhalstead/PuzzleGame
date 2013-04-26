#include "glmenu.h"

MenuItem::MenuItem(const QString &text, bool enabled): Text(text),Enabled(enabled){}

GLMenu::GLMenu(int itemHeight, const QRect &r): itemIndex(-1), menuIndex(-1), fontHeight(itemHeight),maxItems(0)
{
    Resize(r);
}

GLMenu::~GLMenu()
{
    if(top)
        delete[] top;
}

void GLMenu::Add(const MenuItem &add)
{
    items.push_back(add);
    if(itemIndex == -1)
        itemIndex = 0;
}

void GLMenu::Remove(size_t i)
{
    if(unsigned(itemIndex) < items.size())
        items.erase(items.begin()+i);
    if(int(i) == itemIndex)
        this->operator --();
}

int GLMenu::CurrentTop() const
{
    if(menuIndex >= 0)
        return top[menuIndex];
    else
        return -1;
}

int GLMenu::CurrentItem() const
{
    return itemIndex;
}

QRect &GLMenu::Bounds()
{
    return bound;
}

void GLMenu::Draw(QPainter &painter, QPen &pen,const QColor& selectedColor, const QColor& disabledColor)
{
    QColor normalColor = pen.color();
    int startIndex = itemIndex;
    while(int(items.size())-startIndex < maxItems)
        startIndex--;
    if(startIndex < 0)
        startIndex = 0;
    for(int i = 0; i < maxItems && i+startIndex < int(items.size()) ; i++) {
        if(i + startIndex == itemIndex)
            pen.setColor(selectedColor);
        else if(items[i+startIndex].Enabled)
            pen.setColor(normalColor);
        else
            pen.setColor(disabledColor);
        painter.setPen(pen);
        painter.drawText(bound.left(), top[i], bound.width(), fontHeight,Qt::AlignCenter, items[i+startIndex].Text);
    }
    pen.setColor(normalColor);
    painter.setPen(pen);
}

void GLMenu::Resize(const QRect &r)
{
    bound = r;
    int newMax = (r.height()+16)/(fontHeight+16);
    if(newMax != maxItems) {
        maxItems = newMax;
        if(top)
            delete[] top;
        top = new int[maxItems];
        for(int i = 0; i < maxItems; i++)
            top[i] = (i*16) + (i * fontHeight) + bound.top();

    }
}

MenuItem &GLMenu::operator [](size_t i)
{
    return items[itemIndex];
}

int GLMenu::operator --()
{
    if(itemIndex < 0)
        return -1;
    if(items.size()>1) {
        do {
            itemIndex--;
            if(itemIndex < 0)
                itemIndex = items.size()-1;
        }while(!items[itemIndex].Enabled);
    }
    return top[itemIndex];
}

int GLMenu::operator ++()
{
    if(itemIndex < 0)
        return -1;
    if(items.size()>1) {
        do {
            itemIndex++;
            if(unsigned(itemIndex) >= items.size())
                itemIndex=0;
        }while(!items[itemIndex].Enabled);
    }
    return top[itemIndex];
}
