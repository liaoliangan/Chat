//
// Created by 33717 on 2026/2/5.
//

#ifndef QRCODEDIALOG_H
#define QRCODEDIALOG_H

#include <QDialog>
#include "qrcodegen.hpp"
#include "QLabel"

// BMP 文件头结构体 (强制 1 字节对齐)
#pragma pack(push, 1)

struct BMPHeader {
    uint16_t type{0x4D42}; // "BM"
    uint32_t size{0};
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offset{54};
};

struct BMPInfoHeader {
    uint32_t size{40};
    int32_t  width{0};
    int32_t  height{0};
    uint16_t planes{1};
    uint16_t bitCount{24}; // 24位真彩色 (RGB)
    uint32_t compression{0};
    uint32_t sizeImage{0};
    int32_t  xPelsPerMeter{0};
    int32_t  yPelsPerMeter{0};
    uint32_t clrUsed{0};
    uint32_t clrImportant{0};
};
#pragma pack(pop)

QT_BEGIN_NAMESPACE
namespace Ui { class qrCodeDialog; }
QT_END_NAMESPACE

class qrCodeDialog : public QDialog {
Q_OBJECT

public:
    explicit qrCodeDialog(QWidget *parent = nullptr);
    ~qrCodeDialog() override;
signals:
    void qrCodeSuccess();
    void qrCodeFailed();
    void switchChatDialog();
private:
    Ui::qrCodeDialog *ui;
    void displayQrCode(const qrcodegen::QrCode& qr, QLabel* label, int scale);
};


#endif //QRCODEDIALOG_H
