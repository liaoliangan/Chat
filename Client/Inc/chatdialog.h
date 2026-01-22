//
// Created by 33717 on 2026/1/18.
//

#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global.h"
#include "StateWidget.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class ChatDialog;
}

QT_END_NAMESPACE

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget* parent = nullptr);
    ~ChatDialog() override;
    void AddLBGroup(StateWidget* label);
    void addChatUserList(); //TODO 输入数据用于测试，记得删除
protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
private:
    Ui::ChatDialog* ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    void ShowSearch(bool bsearch);
    QList<StateWidget*> _lb_list;
    void ClearLabelState(StateWidget* lb);
    void handleGlobalMousePress(QMouseEvent* mouse_event);
private slots:
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
    void slot_text_changed(const QString& str);
};


#endif //CHATDIALOG_H
