#ifndef SGRAPH2DWIDGET_H
#define SGRAPH2DWIDGET_H

#include <QWidget>
#include "math_lib.h"

class SGraph2dWidget : public QWidget
{
    Q_OBJECT

    std :: vector<SVec2f>* control_points;
    std :: vector<SVec2f>* tangent_vectors;
    std :: vector<SVec2f>* curve_points;

    std :: vector<QPoint> pts_todraw;

    bool SetupNewControlLine;
    int changing_point_num;

    QPoint current_pointer;

    QCursor  cursor_target;
    QCursor  cursor_green;

public:
    explicit SGraph2dWidget(QWidget *parent = nullptr);

    void set_pointers(std :: vector<SVec2f>* cp,
                      std :: vector<SVec2f>* tv,
                      std :: vector<SVec2f>* curve_p)
    {
      control_points = cp;
      tangent_vectors = tv;
      curve_points = curve_p;
    }

    void BeginCurve();
    void EndCurve();

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent( QMouseEvent * event ) Q_DECL_OVERRIDE;
    void mouseReleaseEvent( QMouseEvent * event ) Q_DECL_OVERRIDE;
    void mouseMoveEvent( QMouseEvent * event ) Q_DECL_OVERRIDE;
    void leaveEvent( QEvent * event ) Q_DECL_OVERRIDE;

signals:
    void PointsChanged(int pts);
    void ChangedPoints();


public slots:
};

#endif // SGRAPH2DWIDGET_H
