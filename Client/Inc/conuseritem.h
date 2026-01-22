//
// Created by 33717 on 2026/1/22.
//

#ifndef CONUSERITEM_H
#define CONUSERITEM_H

#include <QWidget>
#include "ListItemBase.h"
#include "userdata.h"
QT_BEGIN_NAMESPACE

namespace Ui
{
    class ConUserItem;
}

QT_END_NAMESPACE

class ConUserItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit ConUserItem(QWidget *parent = nullptr);
    ~ConUserItem();
    QSize sizeHint() const override;
    void SetInfo(std::shared_ptr<AuthInfo> auth_info);
    void SetInfo(std::shared_ptr<AuthRsp> auth_rsp);
    void SetInfo(int uid, QString name, QString icon);
    void ShowRedPoint(bool show = false);
private:
    Ui::ConUserItem *ui;
    std::shared_ptr<UserInfo> _info;
};


#endif //CONUSERITEM_H
