#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "math_lib.h"

namespace Ui {
class MainWindow;
}

enum { NoCurve, BezierCurve, LagrangeCurve , HermitSpline, CubicSpline , PseudoeSpline} ;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    std :: vector<SVec2f> control_points;
    std :: vector<SVec2f> tangent_vectors;
    std :: vector<SVec2f> curve_points;

    int curve_type;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void BuildCurve();
    void BldLagrangeCurve();
    void BldBezierCurve();
    void BldHermitSpline();
    void BldCubicSpline();
    void BldPseudoeSpline();

private slots:
    void on_pbNewControlLine_clicked();

    void OnChangingPoints();
    void OnChangedPoints();

    void on_pdLagrange_clicked();

    void on_pbBezierCurve_clicked();

    void on_pdHermitSpline_clicked();

    void on_pdCubicSpline_clicked();

    void on_pdPseudoeSpline_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
