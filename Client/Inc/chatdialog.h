//
// Created by 33717 on 2026/1/18.
//

#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

#include "userdata.h"
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
    QMap<int, QListWidgetItem*> _chat_items_added;
private slots:
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
    void slot_text_changed(const QString& str);
    void slot_apply_friend(std::shared_ptr<AddFriendApply> apply);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_info);
};


#endif //CHATDIALOG_H
