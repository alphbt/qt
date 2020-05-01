#include "linewidget.h"


void lineWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::red,3,Qt::DashLine));
    
    painter.drawLine(QPoint(p1.x,p1.y),QPoint(p2.x,p2.y));
    QWidget::paintEvent(event);

}

void lineWidget::set_point(const point2& a,const point2& b)
{
    p1 = a;
    p2 = b;
}

void lineWidget::on_pushButton_clicked()
{
    repaint();
}

void lineWidget::beginLine()
{
    setCursor(cursor_target);
    repaint();
}
