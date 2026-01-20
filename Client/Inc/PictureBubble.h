//
// Created by 33717 on 2026/1/20.
//

#ifndef PICTUREBUBBLE_H
#define PICTUREBUBBLE_H

#include"BubbleFrame.h"

class PictureBubble : public BubbleFrame
{
    Q_OBJECT

public:
    PictureBubble(const QPixmap& pixmap, ChatRole role, QWidget* parent = nullptr);
    ~PictureBubble();
};


#endif //PICTUREBUBBLE_H
