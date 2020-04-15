#include "qpointtablewidget.h"

QPointTableWidget::QPointTableWidget(QWidget *parent) : QTableWidget (parent)
{
  control_points = nullptr;
  curve_points = nullptr;
  tangent_vectors = nullptr;

  setColumnCount(2);
  //setColumnWidth(0, 20);
  //setColumnWidth(1, 50);
  //setColumnWidth(2, 50);

  QStringList horizontalHeader;
  //horizontalHeader.append("№");
  horizontalHeader.append("X");
  horizontalHeader.append("Y");

  setShowGrid(true);


  setHorizontalHeaderLabels(horizontalHeader);
}


void QPointTableWidget::fill_list()
{
  if(!control_points->empty())
  {
    setRowCount(control_points->size());
    for(size_t q(0); q < control_points->size(); q++)
    {
      const SVec2f& pt((*control_points)[q]);
      //setItem(q, 0,  new QTableWidgetItem(QString::number(q)));
      setItem(q, 0,  new QTableWidgetItem(QString::number(pt.x)));
      setItem(q, 1,  new QTableWidgetItem(QString::number(pt.y)));
    }
  }
}


void QPointTableWidget::PointRedraw(int pts)
{
  if(pts == -1)
  {
    fill_list();
  }
  else
  {
    const SVec2f& pt((*control_points)[pts]);
    setItem(pts, 0,  new QTableWidgetItem(QString::number(pt.x)));
    setItem(pts, 1,  new QTableWidgetItem(QString::number(pt.y)));
  }
}
