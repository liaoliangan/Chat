//
// Created by 33717 on 2026/1/20.
//

#ifndef TEXTBUBBLE_H
#define TEXTBUBBLE_H

#include <QTextEdit>
#include "BubbleFrame.h"
#include <QHBoxLayout>

class TextBubble : public BubbleFrame
{
    Q_OBJECT

public:
    TextBubble(ChatRole role, const QString& text, QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void adjustTextHeight();
    void setPlainText(const QString& text);
    void initStyleSheet();
    QTextEdit* m_pTextEdit;
};


#endif //TEXTBUBBLE_H
