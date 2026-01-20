//
// Created by 33717 on 2026/1/20.
//

#ifndef MSGINFO_H
#define MSGINFO_H
#include<QString>
#include<QPixmap>

struct MsgInfo
{
    QString msgFlag; //"text,image,file"
    QString content; //表示文件和图像的url
    QPixmap pixmap; //文件和图片的缩略图
};

#endif //MSGINFO_H
