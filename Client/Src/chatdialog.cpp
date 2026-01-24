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
#include "loadingdlg.h"

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
    // 创建QListWidgetItem，并设置自定义的widget
    for (int i = 0; i < 13; i++)
    {
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue % strs.size();
        int head_i = randomValue % heads.size();
        int name_i = randomValue % names.size();

        auto* chat_user_wid = new ChatUserWid();
        chat_user_wid->SetInfo(names[name_i], heads[head_i], strs[str_i]);
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
    LoadingDlg* loadingDialog = new LoadingDlg(this);
    loadingDialog->setModal(true);
    loadingDialog->show(); //模态对话框，代码不会阻塞在这里
    qDebug() << "add new data to list.....";
    addChatUserList();
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
