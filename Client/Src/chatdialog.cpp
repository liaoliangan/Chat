//
// Created by 33717 on 2026/1/18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChatDialog.h" resolved

#include "chatdialog.h"
#include "ui_ChatDialog.h"
#include "ClickedBtn.h"
#include<QAction>
#include"chatuserwid.h"
#include<QRandomGenerator>
#include"loadingdlg.h"

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
    ShowSearch(false);
    connect(ui->chat_user_list, &ChatUserList::sig_loading_chat_user, this, &ChatDialog::slot_loading_chat_user);
    addChatUserList();
}

ChatDialog::~ChatDialog()
{
    delete ui;
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
