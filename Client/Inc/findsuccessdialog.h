//
// Created by 33717 on 2026/1/21.
//

#ifndef FINDSUCCESSDIALOG_H
#define FINDSUCCESSDIALOG_H

#include <QDialog>
#include"SearchInfo.h"
#include<memory>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class FindSuccessDialog;
}

QT_END_NAMESPACE

class FindSuccessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindSuccessDialog(QWidget* parent = nullptr);
    ~FindSuccessDialog() override;
    auto SetSearchInfo(std::shared_ptr<SearchInfo> si) -> void;

private slots:
    auto on_add_friend_btn_clicked() -> void;

private:
    Ui::FindSuccessDialog* ui;
    QWidget* _parent;
    std::shared_ptr<SearchInfo> _si;
};


#endif //FINDSUCCESSDIALOG_H
