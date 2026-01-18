//
// Created by 33717 on 2026/1/18.
//

#include "ClickedBtn.h"

#include "global.h"

ClickedBtn::ClickedBtn(QWidget* parent): QPushButton(parent)
{
    setCursor(Qt::PointingHandCursor);//经典小手
}

ClickedBtn::~ClickedBtn()= default;

void ClickedBtn::SetState(QString normal, QString hover, QString press)
{
    _hover = hover;
    _press = press;
    _normal = normal;
    setProperty("state",normal);
    LA::repolish(this);
    update();
}

void ClickedBtn::enterEvent(QEnterEvent* event)
{
    setProperty("state",_hover);
    LA::repolish(this);
    update();
    QPushButton::enterEvent(event);
}

void ClickedBtn::leaveEvent(QEvent* event)
{
    setProperty("state",_normal);
    LA::repolish(this);
    update();
    QPushButton::leaveEvent(event);
}

void ClickedBtn::mousePressEvent(QMouseEvent* event)
{
    setProperty("state",_press);
    LA::repolish(this);
    update();
    QPushButton::mousePressEvent(event);
}

void ClickedBtn::mouseReleaseEvent(QMouseEvent* event)
{
    setProperty("state",_hover);
    LA::repolish(this);
    update();
    QPushButton::mouseReleaseEvent(event);
}
