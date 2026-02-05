//
// Created by 33717 on 2026/2/5.
//

// You may need to build the project (run Qt uic code generator) to get "ui_qrCodeDialog.h" resolved

#include "qrcodedialog.h"

#include <QPushButton>
#include <fstream>
#include "ui_qrCodeDialog.h"


qrCodeDialog::qrCodeDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::qrCodeDialog) {
    ui->setupUi(this);
    connect(this,&qrCodeDialog::qrCodeSuccess,this,&qrCodeDialog::switchChatDialog);
    connect(ui->cancel_btn,&QPushButton::clicked,this,&qrCodeDialog::qrCodeFailed);
    std::string url = "http://192.168.1.6:8888";
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(url.c_str(), qrcodegen::QrCode::Ecc::HIGH);
    displayQrCode(qr, ui->qrcode_label, 3);
}

qrCodeDialog::~qrCodeDialog() {
    delete ui;
}

void qrCodeDialog::displayQrCode(const qrcodegen::QrCode& qr, QLabel* label, int scale) {
    //填充满Label
    int labelWidth = label->width();
    int qrSizeWithBorder = qr.getSize() + 8;
    int autoScale = labelWidth / qrSizeWithBorder;

    int qrSize = qr.getSize();
    int border = 4; // 留白
    int totalSize = qrSize + (border * 2);

    // 1. 创建 QImage 对象 (单色格式或 RGB)
    // 使用 Format_RGB32 性能较好且兼容性强
    QImage qrImage(totalSize, totalSize, QImage::Format_RGB32);
    qrImage.fill(Qt::white); // 默认背景为白色

    // 2. 遍历二维码模块，设置像素颜色
    for (int y = 0; y < qrSize; y++) {
        for (int x = 0; x < qrSize; x++) {
            // 注意：qrX/qrY 要加上留白的偏移量
            QRgb color = qr.getModule(x, y) ? qRgb(0, 0, 0) : qRgb(255, 255, 255);
            qrImage.setPixel(x + border, y + border, color);
        }
    }

    // 3. 缩放处理
    // 使用 FastTransformation (近邻插值) 保证二维码边缘锐利，不模糊
    int displaySize = totalSize * scale;
    QPixmap pixmap = QPixmap::fromImage(qrImage).scaled(
        displaySize, displaySize,
        Qt::KeepAspectRatio,
        Qt::FastTransformation
    );

    // 4. 设置给 QLabel
    label->setPixmap(pixmap);
    label->setFixedSize(displaySize, displaySize); // 可选，确保 Label 大小匹配
}
