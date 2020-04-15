#include "sgraph2dwidget.h"
#include <QPainter>
#include <QMouseEvent>

SGraph2dWidget::SGraph2dWidget(QWidget *parent) : QWidget(parent)
{
  control_points = nullptr;
  curve_points = nullptr;
  tangent_vectors = nullptr;

  SetupNewControlLine = false;
  changing_point_num = -1;

  cursor_green = QCursor(QPixmap(":/cursor/cursorarrovgreen.png"));
  cursor_target = QCursor(QPixmap(":/cursor/cursortarget.png"));
  setMouseTracking(true) ;

}

void SGraph2dWidget::BeginCurve()
{
  SetupNewControlLine = true;
  setCursor(cursor_target);
  repaint();

}

void SGraph2dWidget::EndCurve()
{
  SetupNewControlLine = false;
  setCursor(Qt::ArrowCursor);
  repaint();

}

const size_t vertex_mark_size = 10;

static SVec2f QPoint2SVec2f(const QPoint& pt)
{
  return SVec2f(pt.x(), pt.y());
}

static QPoint SVec2f2QPoint(const SVec2f& pt)
{
  return QPoint(static_cast<int> (pt.x),
                static_cast<int> (pt.y));
}

void DrawVertex(QPainter& painter, const QPoint& pt, size_t in_number)
{

  painter.drawEllipse(QRect(pt.x() - vertex_mark_size / 2,
                            pt.y() - vertex_mark_size / 2,
                            vertex_mark_size,
                            vertex_mark_size));

  painter.drawText(pt.x() + vertex_mark_size,
                   pt.y() + vertex_mark_size,
                   QString::number(in_number));
}

void DrawVertex(QPainter& painter, const SVec2f& ptf, size_t in_number)
{
  DrawVertex(painter, SVec2f2QPoint(ptf), in_number);
}


void DrawControlLine(QPainter& painter, const QPoint& beg, const QPoint& end)
{

  QPen old_pen = painter.pen();
  QPen dashed_pen(Qt::DashLine);
  painter.setPen(dashed_pen);
  painter.drawLine(beg, end);
  painter.setPen(old_pen);
}


void DrawControlLine(QPainter& painter, const SVec2f& beg, const SVec2f& end)
{
  DrawControlLine(painter, SVec2f2QPoint(beg), SVec2f2QPoint(end));
}

void DrawTangentVector(QPainter& painter, const SVec2f& beg, const SVec2f& end)
{
    QPoint pt_end = SVec2f2QPoint(end);
    painter.drawLine(SVec2f2QPoint(beg), SVec2f2QPoint(end));
    painter.drawRect(QRect(pt_end.x() - vertex_mark_size / 2,
                              pt_end.y() - vertex_mark_size / 2,
                              vertex_mark_size,
                              vertex_mark_size));
}

void SGraph2dWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    for(size_t q(0); q < control_points->size(); q++)
    {
      DrawVertex(painter, (*control_points)[q], q + 1);
      if(q > 0)
      {
        DrawControlLine(painter, (*control_points)[q - 1], (*control_points)[q]);
      }
    }

    if(SetupNewControlLine)
    {
      QPoint pt_text(30, 30); ///width() + 10, height() - 30);
      painter.drawText(pt_text, QString("Добавляем точки левой кнопкой мыши. Выход из режима - правая кнопка мыши."));
      if(control_points->size())
      {
        DrawControlLine(painter, SVec2f2QPoint(control_points->back()), current_pointer);
      }
    }
    else
    if(curve_points->size())
    {
        {
          QPen t_pen(Qt::red);
          QPen old = painter.pen();
          painter.setPen(t_pen);
          ///

          pts_todraw.resize(curve_points->size());
          for(size_t q(0); q < curve_points->size(); q++)
             pts_todraw[q] = SVec2f2QPoint((*curve_points)[q]);
          painter.drawPolyline(&(pts_todraw[0]), pts_todraw.size());
          ///
          painter.setPen(old);
        }

      /*if (tangent_vectors->size())
      {
          QPen t_pen(Qt::blue);
          QPen old = painter.pen();
          painter.setPen(t_pen);
          ///

          //pts_todraw.resize(control_points->size());
          for(size_t q(0); q < control_points->size(); q++)
              DrawTangentVector(painter,(*control_points)[q], (*control_points)[q] + (*tangent_vectors)[q]);
            // pts_todraw[q] = SVec2f2QPoint((*curve_points)[q]);
          //painter.drawPolyline(&(pts_todraw[0]), pts_todraw.size());
          ///
          painter.setPen(old);

      }*/
    }

    //painter.fillRect(event->rect(), Qt::white);
    QWidget::paintEvent(event);

}

void SGraph2dWidget::mousePressEvent( QMouseEvent * event )
{
  if(event->button() == Qt::LeftButton)
  {
    if(SetupNewControlLine)
    {
      // добавляем новую точку
      QPoint pt = event->pos();
      control_points->push_back(QPoint2SVec2f(pt));
      tangent_vectors->push_back(SVec2f(1, 0));
      emit PointsChanged(-1);
      repaint();
    }
    else
    {
      // режим редактирования
      SVec2f pr_pt(QPoint2SVec2f(event->pos()));

      for(size_t q(0); q < control_points->size(); q++)
      {
        if(((*control_points)[q] - pr_pt).length() < vertex_mark_size)
        {
          // захватываем точку
          changing_point_num = q;
          setCursor(cursor_target);
        }
      }
    }
  }
  else if(event->button() == Qt::RightButton)
  {
    if(SetupNewControlLine)
    {
      EndCurve();
    }
  }
}

void SGraph2dWidget::mouseReleaseEvent( QMouseEvent * event )
{
  if(changing_point_num >= 0)
  {
    changing_point_num = -1;
    setCursor(Qt::ArrowCursor);
    emit ChangedPoints();
  }
}

void SGraph2dWidget::mouseMoveEvent( QMouseEvent * event )
{
 current_pointer = event->pos();
 if(changing_point_num >= 0)
 {
   (*control_points)[changing_point_num] = QPoint2SVec2f(current_pointer);
   emit PointsChanged(changing_point_num);
   repaint();

 }

 if(SetupNewControlLine && control_points->size())
    repaint();

}


void SGraph2dWidget::leaveEvent( QEvent * event )
{
  if(changing_point_num >= 0)
  {
    changing_point_num = -1;
    setCursor(Qt::ArrowCursor);
  }
  QWidget::leaveEvent(event);
}
