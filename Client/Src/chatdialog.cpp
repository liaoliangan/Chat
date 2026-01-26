//
// Created by 33717 on 2026/1/18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChatDialog.h" resolved

#include "chatdialog.h"
#include "ui_ChatDialog.h"
#include "ClickedBtn.h"
#include <QAction>
#include "chatuserwid.h"
#include <QRandomGenerator>

#include "conuseritem.h"
#include "loadingdlg.h"
#include "TcpMgr.h"
#include "UserMgr.h"

ChatDialog::ChatDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::ChatDialog), _state(ChatUIMode::ChatMode), _b_loading(false)
{
    ui->setupUi(this);
    ui->add_btn->SetState("normal", "hover", "press");
    ui->search_edit->SetMaxLength(15); //限制输入长度

    auto* searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/image/search.png"));
    ui->search_edit->addAction(searchAction, QLineEdit::LeadingPosition); //将搜索图标添加到输入框的左侧
    //QStringLiteral:在编译期就构建好Unicode字符串字面量
    ui->search_edit->setPlaceholderText(QStringLiteral("搜索")); //输入框提示语

    //创建清除图标
    auto* clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/image/close_transparent.png"));
    //初始时不显示清除图标
    //将清除图标添加到LineEdit的末尾位置
    ui->search_edit->addAction(clearAction, QLineEdit::TrailingPosition);
    // 当需要显示清除图标时，更改为实际的清除图标
    connect(ui->search_edit, &QLineEdit::textChanged, [clearAction](const QString& text)
    {
        if (!text.isEmpty())
        {
            clearAction->setIcon(QIcon(":/image/close_search.png"));
        }
        else
        {
            clearAction->setIcon(QIcon(":/image/close_transparent.png")); // 文本为空时，切换回透明图标
        }
    });

    // 连接清除动作的触发信号到槽函数，用于清除文本
    connect(clearAction, &QAction::triggered, [this, clearAction]()
    {
        ui->search_edit->clear();
        clearAction->setIcon(QIcon(":/image/close_transparent.png")); // 清除文本后，切换回透明图标
        ui->search_edit->clearFocus();
        //清除按钮被按下则不显示搜索框
        ShowSearch(false);
    });
    ui->search_edit->setMaxLength(15);
    ShowSearch(false);
    connect(ui->chat_user_list, &ChatUserList::sig_loading_chat_user, this, &ChatDialog::slot_loading_chat_user);
    addChatUserList();


    QPixmap pixmap(":/image/head_1.jpg");
    ui->side_head_label->setPixmap(pixmap); // 将图片设置到QLabel上
    QPixmap scaledPixmap = pixmap.scaled(ui->side_head_label->size(), Qt::KeepAspectRatio); // 将图片缩放到label的大小
    ui->side_head_label->setPixmap(scaledPixmap); // 将缩放后的图片设置到QLabel上
    ui->side_head_label->setScaledContents(true); // 设置QLabel自动缩放图片内容以适应大小

    ui->side_chat_label->setProperty("state", "normal");

    ui->side_chat_label->SetState("normal", "hover", "pressed", "selected_normal", "selected_hover",
                                  "selected_pressed");

    ui->side_contact_label->SetState("normal", "hover", "pressed", "selected_normal", "selected_hover",
                                     "selected_pressed");

    AddLBGroup(ui->side_chat_label);
    AddLBGroup(ui->side_contact_label);

    connect(ui->side_chat_label, &StateWidget::clicked, this, &ChatDialog::slot_side_chat);
    connect(ui->side_contact_label, &StateWidget::clicked, this, &ChatDialog::slot_side_contact);

    //连接搜索框输入变化
    connect(ui->search_edit, &QLineEdit::textChanged, this, &ChatDialog::slot_text_changed);

    //检测鼠标点击的位置判断是否清楚搜索框
    this->installEventFilter(this);

    //设置聊天label选中状态
    ui->side_chat_label->SetSelected(true);

    //为search_list设置search_edit
    ui->search_list->SetSearchEdit(ui->search_edit);
    //连接申请添加好友信号
    connect(TcpMgr::getInstance().get(), &TcpMgr::sig_friend_apply, this, &ChatDialog::slot_apply_friend);

    //连接自己认证回复信号
    connect(TcpMgr::getInstance().get(), &TcpMgr::sig_auth_rsp, this, &ChatDialog::slot_auth_rsp);

    //连接searchlist跳转聊天界面信号
    connect(ui->search_list, &SearchList::sig_jump_chat_item, this, &ChatDialog::slot_jump_chat_item);

    //连接加载联系人的信号
    connect(ui->con_user_list, &ContactUserList::sig_loading_contact_user, this,
            &ChatDialog::slot_loading_contact_user);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::AddLBGroup(StateWidget* label)
{
    _lb_list.push_back(label);
}

//TODO 测试用的，记得删除
void ChatDialog::addChatUserList()
{
    //先按照好友列表加载聊天记录，等以后客户端实现聊天记录数据库之后再按照最后信息排序
    auto friend_list = UserMgr::getInstance()->GetChatListPerPage();
    if (friend_list.empty() == false)
    {
        for (auto& friend_ele : friend_list)
        {
            auto find_iter = _chat_items_added.find(friend_ele->_uid);
            if (find_iter != _chat_items_added.end())
            {
                continue;
            }
            auto* chat_user_wid = new ChatUserWid();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem* item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->addItem(item);
            ui->chat_user_list->setItemWidget(item, chat_user_wid);
            _chat_items_added.insert(friend_ele->_uid, item);
        }

        //更新已加载条目
        UserMgr::getInstance()->UpdateChatLoadedCount();
    }
    // 创建QListWidgetItem，并设置自定义的widget
    for (int i = 0; i < 13; i++)
    {
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue % strs.size();
        int head_i = randomValue % heads.size();
        int name_i = randomValue % names.size();

        auto* chat_user_wid = new ChatUserWid();
        chat_user_wid->SetInfo(std::make_shared<UserInfo>(0, names[name_i],
                                                          names[name_i], heads[head_i], 0, strs[str_i]));
        QListWidgetItem* item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item, chat_user_wid);
    }
}

bool ChatDialog::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        handleGlobalMousePress(mouseEvent);
    }
    return QDialog::eventFilter(watched, event);
}

void ChatDialog::ShowSearch(bool bsearch)
{
    if (bsearch)
    {
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        _mode = SearchMode;
    }
    else if (_state == ChatMode)
    {
        ui->chat_user_list->show();
        ui->con_user_list->hide();
        ui->search_list->hide();
        _mode = ChatMode;
    }
    else if (_state == ContactMode)
    {
        ui->chat_user_list->hide();
        ui->search_list->hide();
        ui->con_user_list->show();
        _mode = ContactMode;
    }
}

void ChatDialog::slot_loading_chat_user()
{
    if (_b_loading)
    {
        return;
    }

    _b_loading = true;
    auto* loadingDialog = new LoadingDlg(this);
    loadingDialog->setModal(true);
    loadingDialog->show(); //模态对话框，代码不会阻塞在这里
    qDebug() << "add new data to list.....";
    loadMoreChatUser();
    // 加载完成后关闭对话框
    loadingDialog->deleteLater();

    _b_loading = false;
}

void ChatDialog::slot_side_chat()
{
#ifdef LADEBUG
    qDebug() << "receive side chat clicked";
#endif
    ClearLabelState(ui->side_chat_label);
    ui->stackedWidget->setCurrentWidget(ui->chat_page);
    _state = ChatUIMode::ChatMode;
    ShowSearch(false);
}

void ChatDialog::slot_side_contact()
{
#ifdef LADEBUG
    qDebug() << "receive side contact clicked";
#endif
    ClearLabelState(ui->side_contact_label);
    ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);
    _state = ChatUIMode::ContactMode;
    ShowSearch(false);
}

void ChatDialog::slot_text_changed(const QString& str)
{
#ifdef LADEBUG
    qDebug() << "receive slot text changed str is " << str;
#endif
    if (!str.isEmpty())
        ShowSearch(true);
}

void ChatDialog::ClearLabelState(StateWidget* lb)
{
    for (auto& ele : _lb_list)
    {
        //在组里找到label
        if (ele == lb)
        {
            continue;
        }

        ele->ClearState();
    }
}

void ChatDialog::handleGlobalMousePress(QMouseEvent* mouse_event)
{
    // 实现点击位置的判断和处理逻辑
    // 先判断是否处于搜索模式，如果不处于搜索模式则直接返回
    if (_mode != ChatUIMode::SearchMode)
    {
        return;
    }

    // 将鼠标点击位置转换为搜索列表坐标系中的位置
    QPoint posInSearchList = ui->search_list->mapFromGlobal(mouse_event->globalPos());
    // 判断点击位置是否在聊天列表的范围内
    if (!ui->search_list->rect().contains(posInSearchList))
    {
        // 如果不在聊天列表内，清空输入框
        ui->search_edit->clear();
        ShowSearch(false);
    }
}

void ChatDialog::slot_apply_friend(std::shared_ptr<AddFriendApply> apply)
{
    qDebug() << "receive apply friend slot, applyuid is " << apply->_from_uid << " name is "
        << apply->_name << " desc is " << apply->_desc;

    if (UserMgr::getInstance()->AlreadyApply(apply->_from_uid))
        return; //如果已经申请个一次了

    UserMgr::getInstance()->AddApplyList(std::make_shared<ApplyInfo>(apply));
    ui->side_contact_label->ShowRedPoint(true);
    ui->con_user_list->ShowRedPoint(true);
    ui->friend_apply_page->AddNewApply(apply);
}

void ChatDialog::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    qDebug() << "receive slot_auth_rsp uid is " << auth_rsp->_uid
        << " name is " << auth_rsp->_name << " nick is " << auth_rsp->_nick;

    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::getInstance()->CheckFriendById(auth_rsp->_uid);
    if (bfriend)
    {
        return;
    }

    UserMgr::getInstance()->AddFriend(auth_rsp);
    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int str_i = randomValue % strs.size();
    int head_i = randomValue % heads.size();
    int name_i = randomValue % names.size();

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(auth_rsp);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(auth_rsp->_uid, item);
}

void ChatDialog::slot_jump_chat_item(std::shared_ptr<SearchInfo> si)
{
    qDebug() << "slot jump chat item ";
    auto find_iter = _chat_items_added.find(si->_uid);
    if (find_iter != _chat_items_added.end())
    {
        qDebug() << "jump to chat item , uid is " << si->_uid;
        ui->chat_user_list->scrollToItem(find_iter.value());
        ui->side_chat_label->SetSelected(true);
        SetSelectChatItem(si->_uid);
        //更新聊天界面信息
        SetSelectChatPage(si->_uid);
        slot_side_chat();
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(si);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);

    _chat_items_added.insert(si->_uid, item);

    ui->side_chat_label->SetSelected(true);
    SetSelectChatItem(si->_uid);
    //更新聊天界面信息
    SetSelectChatPage(si->_uid);
    slot_side_chat();
}

void ChatDialog::slot_loading_contact_user()
{
    qDebug() << "slot loading contact user";
    if (_b_loading)
    {
        return;
    }

    _b_loading = true;
    LoadingDlg* loadingDialog = new LoadingDlg(this);
    loadingDialog->setModal(true);
    loadingDialog->show();
    qDebug() << "add new data to list.....";
    loadMoreConUser();
    // 加载完成后关闭对话框
    loadingDialog->deleteLater();

    _b_loading = false;
}

void ChatDialog::SetSelectChatItem(int uid)
{
    if (ui->chat_user_list->count() <= 0)
    {
        return;
    }

    if (uid == 0)
    {
        ui->chat_user_list->setCurrentRow(0);
        QListWidgetItem* firstItem = ui->chat_user_list->item(0);
        if (!firstItem)
        {
            return;
        }

        //转为widget
        QWidget* widget = ui->chat_user_list->itemWidget(firstItem);
        if (!widget)
        {
            return;
        }

        auto con_item = qobject_cast<ChatUserWid*>(widget);
        if (!con_item)
        {
            return;
        }

        _cur_chat_uid = con_item->GetUserInfo()->_uid;
        return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if (find_iter == _chat_items_added.end())
    {
        qDebug() << "uid " << uid << " not found, set curent row 0";
        ui->chat_user_list->setCurrentRow(0);
        return;
    }

    ui->chat_user_list->setCurrentItem(find_iter.value());

    _cur_chat_uid = uid;
}

void ChatDialog::SetSelectChatPage(int uid)
{
    if (ui->chat_user_list->count() <= 0)
    {
        return;
    }

    if (uid == 0)
    {
        auto item = ui->chat_user_list->item(0);
        //转为widget
        QWidget* widget = ui->chat_user_list->itemWidget(item);
        if (!widget)
        {
            return;
        }

        auto con_item = qobject_cast<ChatUserWid*>(widget);
        if (!con_item)
        {
            return;
        }

        //设置信息
        auto user_info = con_item->GetUserInfo();
        ui->chat_page->SetUserInfo(user_info);
        return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if (find_iter == _chat_items_added.end())
    {
        return;
    }

    //转为widget
    QWidget* widget = ui->chat_user_list->itemWidget(find_iter.value());
    if (!widget)
    {
        return;
    }

    //判断转化为自定义的widget
    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase* customItem = qobject_cast<ListItemBase*>(widget);
    if (!customItem)
    {
        qDebug() << "qobject_cast<ListItemBase*>(widget) is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if (itemType == CHAT_USER_ITEM)
    {
        auto con_item = qobject_cast<ChatUserWid*>(customItem);
        if (!con_item)
        {
            return;
        }

        //设置信息
        auto user_info = con_item->GetUserInfo();
        ui->chat_page->SetUserInfo(user_info);

        return;
    }
}

void ChatDialog::loadMoreChatUser()
{
    auto friend_list = UserMgr::getInstance()->GetChatListPerPage();
    if (friend_list.empty() == false)
    {
        for (auto& friend_ele : friend_list)
        {
            auto find_iter = _chat_items_added.find(friend_ele->_uid);
            if (find_iter != _chat_items_added.end())
            {
                continue;
            }
            auto* chat_user_wid = new ChatUserWid();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem* item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->addItem(item);
            ui->chat_user_list->setItemWidget(item, chat_user_wid);
            _chat_items_added.insert(friend_ele->_uid, item);
        }

        //更新已加载条目
        UserMgr::getInstance()->UpdateChatLoadedCount();
    }
}

void ChatDialog::loadMoreConUser()
{
    auto friend_list = UserMgr::getInstance()->GetConListPerPage();
    if (friend_list.empty() == false)
    {
        for (auto& friend_ele : friend_list)
        {
            auto* chat_user_wid = new ConUserItem();
            chat_user_wid->SetInfo(friend_ele->_uid, friend_ele->_name,
                                   friend_ele->_icon);
            QListWidgetItem* item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->con_user_list->addItem(item);
            ui->con_user_list->setItemWidget(item, chat_user_wid);
        }

        //更新已加载条目
        UserMgr::getInstance()->UpdateContactLoadedCount();
    }
}
