//
// Created by 33717 on 2026/1/15.
//
#include "global.h"
#include "TimerBtn.h"

TimerBtn::TimerBtn(QWidget* parent): QPushButton(parent), _counter(10)
{
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, [this]()
    {
        _counter--;
        if (_counter <= 0)
        {
            _timer->stop();
            _counter = 10;
            this->setText("获取");
            this->setEnabled(true);
            return;
        }
        this->setText(QString::number(_counter));
    });
}

TimerBtn::~TimerBtn()
{
    _timer->stop();
}

void TimerBtn::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        COUT << "My Left Button was released!" << ENDL;
        this->setEnabled(false);
        this->setText(QString::number(_counter));
        _timer->start(1000);
        emit clicked(); //亲测，必须手动发送clicked信号，基类不会发送
    }
    //调用基类的鼠标释放事件
    QPushButton::mouseReleaseEvent(e);
}
