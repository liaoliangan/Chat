//
// Created by 33717 on 2026/1/22.
//

#ifndef APPLYFRIENDITEM_H
#define APPLYFRIENDITEM_H

#include <QWidget>
#include "ListItemBase.h"
#include <memory>
#include "userdata.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class ApplyFriendItem;
}

QT_END_NAMESPACE

class ApplyFriendItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit ApplyFriendItem(QWidget* parent = nullptr);
    ~ApplyFriendItem();
    void SetInfo(std::shared_ptr<ApplyInfo> apply_info);
    void ShowAddBtn(bool bshow);

    QSize sizeHint() const override
    {
        return QSize(250, 80); // 返回自定义的尺寸
    }

    int GetUid();

private:
    Ui::ApplyFriendItem* ui;
    std::shared_ptr<ApplyInfo> _apply_info;
    bool _added;
signals:
    void sig_auth_friend(std::shared_ptr<ApplyInfo> apply_info);
};


#endif //APPLYFRIENDITEM_H
