#ifndef LINEWIDGET_H
#define LINEWIDGET_H

#include<QWidget>
#include<QPainter>
#include"point2.h"

namespace Ui{
class lineWidget;
}

class lineWidget : public QWidget
{
    Q_OBJECT
    
    point2 p1;
    point2 p2;

    QCursor  cursor_target;
    
public:
    explicit lineWidget(QWidget *parent = 0);
    void set_point(const point2& a,const point2& b);
    void beginLine();
    
    void paintEvent(QPaintEvent *event);
private slots:
    void on_pushButton_clicked();
};



#endif // LINEWIDGET_H
