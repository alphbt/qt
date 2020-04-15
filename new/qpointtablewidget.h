#ifndef QPOINTTABLEWIDGET_H
#define QPOINTTABLEWIDGET_H

#include <QTableWidget>
#include "math_lib.h"

class QPointTableWidget : public QTableWidget
{
    Q_OBJECT


    std :: vector<SVec2f>* control_points;
    std :: vector<SVec2f>* tangent_vectors;
    std :: vector<SVec2f>* curve_points;

public:
    explicit QPointTableWidget(QWidget *parent = nullptr);

    void set_pointers(std :: vector<SVec2f>* cp,
                      std :: vector<SVec2f>* tv,
                      std :: vector<SVec2f>* curve_p)
    {
      control_points = cp;
      tangent_vectors = tv;
      curve_points = curve_p;
    }

    void fill_list();


signals:

public slots:
    void PointRedraw(int pts);

};

#endif // QPOINTTABLEWIDGET_H
