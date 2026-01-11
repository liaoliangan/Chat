//
// Created by 33717 on 2026/1/10.
//
#include "global.h"

namespace LA
{
    std::function<void(QWidget*)> repolish = [](QWidget* widget)-> void
    {
        widget->style()->unpolish(widget); //移除样式
        widget->style()->polish(widget); //重新刷新样式
    };
}
