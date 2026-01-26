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
    void ShowSearch(bool bsearch);
    void ClearLabelState(StateWidget* lb);
    void handleGlobalMousePress(QMouseEvent* mouse_event);
    void SetSelectChatItem(int uid = 0);
    void SetSelectChatPage(int uid = 0);
    void loadMoreChatUser();
    void loadMoreConUser();
    Ui::ChatDialog* ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    int _cur_chat_uid;
    QList<StateWidget*> _lb_list;
    QMap<int, QListWidgetItem*> _chat_items_added;

private slots:
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
    void slot_text_changed(const QString& str);
    void slot_apply_friend(std::shared_ptr<AddFriendApply> apply);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_info);
    void slot_jump_chat_item(std::shared_ptr<SearchInfo> si);
    void slot_loading_contact_user();
};


#endif //CHATDIALOG_H
