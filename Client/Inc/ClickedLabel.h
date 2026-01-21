//
// Created by 33717 on 2026/1/15.
//

#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include<QLabel>
#include"global.h"
#include<QMouseEvent>

class ClickedLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickedLabel(QWidget* parent = nullptr);
    ~ClickedLabel();
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    /**
     * @brief 设置鼠标进入label的各种状态
     * @param normal 初始闭眼图标的状态
     * @param hover 鼠标进入label后的状态
     * @param press 鼠标点击label后的状态
     * @param select 点击后切换为睁眼的状态
     * @param select_hover 鼠标进入label后的睁眼的状态
     * @param select_press 鼠标点击label后的睁眼的状态
     */
    void SetState(QString normal = "", QString hover = "", QString press = "",
                  QString select = "", QString select_hover = "", QString select_press = "");
    LA::ClickLbState GetCurState();
    bool SetCurState(LA::ClickLbState state);
    void ResetNormalState();
private:
    QString _normal;
    QString _hover;
    QString _press;
    QString _selected;
    QString _selected_hover;
    QString _selected_press;
    LA::ClickLbState _cur_state;
signals:
    void clicked(QString, LA::ClickLbState);
};


#endif //CLICKEDLABEL_H
