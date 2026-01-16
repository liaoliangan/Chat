//
// Created by 33717 on 2026/1/15.
//

#include "ClickedLabel.h"

ClickedLabel::ClickedLabel(QWidget* parent): QLabel(parent), _cur_state(LA::ClickLbState::Normal)
{
    this->setCursor(Qt::PointingHandCursor);
}

ClickedLabel::~ClickedLabel() = default;

void ClickedLabel::enterEvent(QEnterEvent* event)
{
    // 在这里处理鼠标悬停进入的逻辑
    if (_cur_state == LA::ClickLbState::Normal)
    {
        COUT << "enter , change to normal hover: " << _hover.toStdString() << ENDL;
        setProperty("state", _hover);
        LA::repolish(this);
        update();
    }
    else
    {
        COUT << "enter , change to selected hover: " << _selected_hover.toStdString() << ENDL;
        setProperty("state", _selected_hover);
        LA::repolish(this);
        update();
    }

    QLabel::enterEvent(event);
}

void ClickedLabel::leaveEvent(QEvent* event)
{
    // 在这里处理鼠标悬停离开的逻辑
    if (_cur_state == LA::ClickLbState::Normal)
    {
        COUT << "leave , change to normal : " << _normal.toStdString() << ENDL;
        setProperty("state", _normal);
        LA::repolish(this);
        update();
    }
    else
    {
        COUT << "leave , change to normal hover: " << _selected.toStdString() << ENDL;
        setProperty("state", _selected);
        LA::repolish(this);
        update();
    }
    QLabel::leaveEvent(event);
}

void ClickedLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (_cur_state == LA::ClickLbState::Normal)
        {
            COUT << "clicked , change to selected hover: " << _selected_hover.toStdString() << ENDL;
            _cur_state = LA::ClickLbState::Selected;
            setProperty("state", _selected_hover);
            LA::repolish(this);
            update();
        }
        else
        {
            COUT << "clicked , change to normal hover: " << _hover.toStdString() << ENDL;
            _cur_state = LA::ClickLbState::Normal;
            setProperty("state", _hover);
            LA::repolish(this);
            update();
        }
        emit clicked();
    }
    // 调用基类的mousePressEvent以保证正常的事件处理
    QLabel::mousePressEvent(event);
}

void ClickedLabel::SetState(QString normal, QString hover, QString press, QString select, QString select_hover,
                            QString select_press)
{
    _normal = normal;
    _hover = hover;
    _press = press;

    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;

    setProperty("state", normal);
    LA::repolish(this);
}

LA::ClickLbState ClickedLabel::GetCurState()
{
    return _cur_state;
}
