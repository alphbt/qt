#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graph_widget->set_pointers(&control_points, &tangent_vectors, &curve_points);
    ui->twPoints->set_pointers(&control_points, &tangent_vectors, &curve_points);

    curve_type = NoCurve;

    connect(ui->graph_widget, SIGNAL(PointsChanged(int)),
            ui->twPoints, SLOT(PointRedraw(int)));

    connect(ui->graph_widget, SIGNAL(PointsChanged(int)),
            this, SLOT(OnChangingPoints()));

    connect(ui->graph_widget, SIGNAL(ChangedPoints()),
            this, SLOT(OnChangedPoints()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbNewControlLine_clicked()
{
  control_points.clear();
  curve_points.clear();
  tangent_vectors.clear();

  ui->graph_widget->BeginCurve();
}

void MainWindow::OnChangingPoints()
{
 if(ui->cbOnFlyRebuild->isChecked())
   BuildCurve();
}

void MainWindow::OnChangedPoints()
{
  BuildCurve();
}

void MainWindow::BuildCurve()
{
  switch(curve_type)
  {
    case LagrangeCurve:
         BldLagrangeCurve();
         break;

    case BezierCurve:
         BldBezierCurve();
         break;

    case HermitSpline:
         BldHermitSpline();
         break;

    case CubicSpline:
         BldCubicSpline();
         break;

    case PseudoeSpline:
         BldPseudoeSpline();
         break;

    case NoCurve:
    default: ;
  }

  if(curve_type != NoCurve)
    repaint();
}

const size_t number_curve_points = 100;

SVec2f DeCasteljoVector(double t, size_t i, int num, const std :: vector<SVec2f>& cp)
{
  if(num == 0)
  {
    return cp[i];
  }
  else
  {
    return (1 - t) * DeCasteljoVector(t, i, num - 1, cp) + t * DeCasteljoVector(t, i + 1, num - 1, cp);
  }
}

void MainWindow:: BldBezierCurve()
{
  curve_points.resize(number_curve_points);
  double t = 0.0;
  double step = 1.0 / (number_curve_points - 1);
  for(size_t q(0); q < number_curve_points; q++, t += step)
    curve_points[q] = DeCasteljoVector(t, 0, control_points.size() - 1, control_points);
}

double lagrange_mult(double t, int i, int n)
{
  double ret = 1.0;
  for(int q(0); q < n; q++)
  if(q != i)
  {
     ret *= static_cast<double>(t - q) / (i - q);
  }
  return ret;
}

SVec2f lagrange_vector(double t, const std :: vector<SVec2f>& cp)
{
  SVec2f ret(0, 0);
  for(size_t q(0); q < cp.size(); q++)
    ret = ret + lagrange_mult(t, q, cp.size()) * cp[q];

  return ret;
}

void MainWindow:: BldLagrangeCurve()
{
  curve_points.resize(number_curve_points);
  double step = static_cast<double>(control_points.size() - 1) / (number_curve_points - 1);
  double t(0.0);
  for(size_t q(0); q < number_curve_points; q++, t += step)
    curve_points[q] = lagrange_vector(t, control_points);
}

double g0(double w)
{
    double w2 = w * w;
    return 1 - 3 * w2 + 2 * w * w2;
}

double g1(double w)
{
    double w2 = w * w;
    return w - 2 * w2 + w * w2;
}

double h0(double w)
{
    double w2 = w * w;
    return 3 * w2 - 2 * w * w2;
}

double h1(double w)
{
    double w2 = w * w;
    return -w2 + w * w2;
}

SVec2f hermit_vector(double t, const std :: vector<SVec2f>& control_points, const std :: vector<SVec2f> tangent_vectors)
{
    int num = static_cast<int>(t);
    if(num == control_points.size() - 1)
    {
        num--;
    }

    double w = t - num;

    return control_points[num] * g0(w) + tangent_vectors[num] * g1(w) +
            control_points[num + 1] * h0(w) + tangent_vectors[num +1] * h1(w);
}

void MainWindow::BldHermitSpline()
{
    curve_points.resize(number_curve_points);
    double t=0.0;
    double step = static_cast<double>(control_points.size() - 1) / (number_curve_points - 1);

    tangent_vectors.resize(control_points.size());
    size_t N = control_points.size() - 1;

    for (size_t q = 1; q < N; q++)
    {
        tangent_vectors[q] = control_points[q + 1] - control_points[q - 1];
    }

    tangent_vectors.front() = 2 * (control_points[1] - control_points[0] - tangent_vectors[1]);
    tangent_vectors.back() = 2 * (control_points[N] - control_points[N - 1] - tangent_vectors[N - 1]);

    for(size_t q(0); q < number_curve_points; q++, t += step)
    {
        curve_points[q] = hermit_vector(t, control_points , tangent_vectors);
    }
}

SVec2f cubic_spline_vector(double t,const std :: vector<SVec2f>& control_points, const std :: vector<SVec2f> tangent_vectors )
{
    int num = static_cast<int>(t);
    if(num == control_points.size() - 1)
    {
        num--;
    }

    double w = t - num;
    double w2 = w * w;

    return (1 - w) * control_points[num] + w * control_points[num + 1] +
            ((-2 * w + 3 * w2 - w * w2 ) * tangent_vectors[num] + (-w + w2 * w) * tangent_vectors[num + 1]) / 6;
}

void MainWindow::BldCubicSpline()
{
    curve_points.resize(number_curve_points);
    double t=0.0;
    double step = static_cast<double>(control_points.size() - 1) / (number_curve_points - 1);

    tangent_vectors.resize(control_points.size());
    size_t N = control_points.size() - 1;

    if(N > 2)
    {
        std::vector<SVec2f> f(N , 0);
        std::vector<double> c(N , 0);

        double a = 1, b = 4;

        f[1] =  6 * (control_points[2] - 2 * control_points[1] + control_points[0]);
        c[1] = 1;

        for(size_t q = 2; q < N; q++)
        {
            c[q] = 1 - a / c [q - 1] * b;
            f[q] = 6 * (control_points[q + 1] - 2 * control_points[q] + control_points[q - 1]) -
                    a / c[q - 1] * f[q - 1];
        }

        tangent_vectors[N - 1] = f[N - 1] / c[N - 1];

        for(size_t q = N - 2; q >= 1 ; q--)
        {
            tangent_vectors[q] = (f[q] - b * tangent_vectors[q + 1]) / c[q];
        }
      }

        tangent_vectors[0] = tangent_vectors[1];
        tangent_vectors[N] = tangent_vectors[N - 1];

    for(size_t q(0); q < number_curve_points; q++, t += step)
    {
        curve_points[q] = cubic_spline_vector(t, control_points , tangent_vectors);
    }
}

SVec2f pseudoe_spline_vector(double t,const std :: vector<double>& k,const std :: vector<SVec2f>& control_points, const std :: vector<SVec2f> tangent_vectors)
{

    int num = static_cast<int>(t);
    if(num == control_points.size() - 1)
    {
        num--;
    }

    double w = t - num;

    return control_points[num] * g0(w) + tangent_vectors[num] * g1(w) +
            control_points[num + 1] * h0(w) + tangent_vectors[num +1] * k[num + 1] * h1(w);

}

void MainWindow::BldPseudoeSpline()
{
    curve_points.resize(number_curve_points);
    double t = 0.0;
    double step = static_cast<double>(control_points.size() - 1) / (number_curve_points - 1);

    tangent_vectors.resize(control_points.size());
    size_t N = control_points.size() - 1;



    for(size_t q(0); q < number_curve_points; q++, t += step)
    {
        //curve_points[q] = pseudoe_spline_vector(t, k , control_points , tangent_vectors);
    }
}

void MainWindow::on_pdLagrange_clicked()
{
  if(control_points.size() > 1)
  {
    curve_type = LagrangeCurve;
    BuildCurve();
  }
}

void MainWindow::on_pdHermitSpline_clicked()
{
    if(control_points.size() > 1)
    {
        curve_type = HermitSpline;
        BuildCurve();
    }
}

void MainWindow::on_pdCubicSpline_clicked()
{
    if(control_points.size() > 1)
    {
        curve_type = CubicSpline;
        BuildCurve();
    }
}

void MainWindow::on_pdPseudoeSpline_clicked()
{
    if(control_points.size() > 1)
    {
        curve_type = PseudoeSpline;
        BuildCurve();
    }
}

void MainWindow::on_pbBezierCurve_clicked()
{
    if(control_points.size() > 1)
    {
      curve_type = BezierCurve;
      BuildCurve();
    }
}
