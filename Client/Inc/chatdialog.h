//
// Created by 33717 on 2026/1/18.
//

#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class ChatDialog; }
QT_END_NAMESPACE

class ChatDialog : public QDialog {
Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog() override;

private:
    Ui::ChatDialog *ui;
};


#endif //CHATDIALOG_H
