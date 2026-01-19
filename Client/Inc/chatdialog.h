//
// Created by 33717 on 2026/1/18.
//

#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include<global.h>

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
    void addChatUserList(); //TODO 输入数据用于测试，记得删除
private:
    Ui::ChatDialog* ui;
    void ShowSearch(bool bsearch);
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;

    //TODO 测试数据，记得删
    //TODO ----------------------------------------
    std::vector<QString> strs = {
        "hello world !",
        "nice to meet u",
        "New year，new life",
        "You have to love yourself",
        "My love is written in the wind ever since the whole world is you"
    };

    std::vector<QString> heads = {
        ":/image/head_1.jpg",
        ":/image/head_2.jpg",
        ":/image/head_3.jpg",
        ":/image/head_4.jpg",
        ":/image/head_5.jpg"
    };

    std::vector<QString> names = {
        "llfc",
        "zack",
        "golang",
        "cpp",
        "java",
        "nodejs",
        "python",
        "rust"
    };
    //TODO ----------------------------------------
};


#endif //CHATDIALOG_H
