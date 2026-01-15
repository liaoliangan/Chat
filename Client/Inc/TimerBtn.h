//
// Created by 33717 on 2026/1/15.
//

#ifndef TIMERBTN_H
#define TIMERBTN_H

#include<QPushButton>
#include<QMouseEvent>
#include<QTimer>

class TimerBtn:public QPushButton
{
public:
  TimerBtn(QWidget *parent = nullptr);
  ~TimerBtn();
  void mouseReleaseEvent(QMouseEvent *e) override;
private:
  QTimer *_timer;
  int _counter;
};



#endif //TIMERBTN_H
