//
// Created by 33717 on 2026/1/20.
//

#ifndef SEARCHLIST_H
#define SEARCHLIST_H

#include <QListWidget>
#include <QWheelEvent>//
#include <QEvent>
#include <QScroller>
#include <QDebug>
#include <QDialog>
#include <memory>
#include <QScrollBar>
#include "SearchInfo.h"
#include "logindialog.h"

class SearchList : public QListWidget
{
    Q_OBJECT

public:
    explicit SearchList(QWidget* parent = nullptr);
    void CloseFindDlg();
    void SetSearchEdit(QWidget* edit);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void waitPending(bool pending = true);
    bool _send_pending;//pending为true说明在等待服务器回包，应该阻塞不更新页面
    void addTipItem();
    std::shared_ptr<QDialog> _find_dlg;
    QWidget* _search_edit;
    LoginDialog* _loadingDialog;

private slots:
    void slot_item_clicked(QListWidgetItem* item);
    void slot_user_search(std::shared_ptr<SearchInfo> si);
signals:
};


#endif //SEARCHLIST_H
