//
// Created by 33717 on 2026/1/24.
//

#ifndef FINDFAILDIALOG_H
#define FINDFAILDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class FindFailDialog; }
QT_END_NAMESPACE

class FindFailDialog : public QDialog {
Q_OBJECT

public:
    explicit FindFailDialog(QWidget *parent = nullptr);
    ~FindFailDialog() override;
private slots:
    void on_fail_sure_btn_clicked();
private:
    Ui::FindFailDialog *ui;
};


#endif //FINDFAILDIALOG_H
