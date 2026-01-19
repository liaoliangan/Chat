//
// Created by 33717 on 2026/1/19.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LoadingDlg.h" resolved

#include "loadingdlg.h"
#include "ui_LoadingDlg.h"
#include<QMovie>

LoadingDlg::LoadingDlg(QWidget* parent) :
    QDialog(parent), ui(new Ui::LoadingDlg)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground); //设置背景透明

    //获取屏幕尺寸
    setFixedSize(parent->size()); //设置对话框为全屏尺寸
    QMovie* movie = new QMovie(":/image/loading.gif"); //加载动画的资源文件
    ui->loading_label->setMovie(movie);
    movie->start();
}

LoadingDlg::~LoadingDlg()
{
    delete ui;
}
