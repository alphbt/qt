#include "mainwindow.h"
#include "ui_mainwindow.h"

enum turn_t {left = 1, right = -1 , collinear = 0};

double cross(point2 const& a, point2 const& b)
{
    return a.x * b.y - b.x * a.y;
}

turn_t turn(point2 const& a, point2 const& b, point2 const& c)
{
    double l = (b.x - a.x) * (c.y - a.y);
    double r = (c.x - a.x) * (b.y - a.y);

    double e = (abs(1) + abs(r)) * std::numeric_limits<double>::epsilon() * 4;
    double det = l - r;
    if (det > e)
        return left;
    if (det < -e)
        return right;

    interval_t ia(a), ib(b), ic(c);
    interval_t idet = cross(ib - ia, ic - ia);

    if (!zero_in(idet))
    {
        if (idet > 0)
            return left;
        return right;
    }

    return collinear;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    double q1_x = ui->lineEdit->text().toDouble();
    double q1_y = ui->lineEdit_5->text().toDouble();
    double q2_x = ui->lineEdit_2->text().toDouble();
    double q2_y = ui->lineEdit_6->text().toDouble();
    double q3_x = ui->lineEdit_3->text().toDouble();
    double q3_y = ui->lineEdit_7->text().toDouble();

    ui->line->beginLine();

    //painter.end();

    switch(turn(point2(q1_x,q1_y),point2(q2_x,q2_y),point2(q3_x,q3_y)))
    {
    case right:
        ui->lineEdit_4->setText("RIGHT");
        break;
    case left:
        ui->lineEdit_4->setText("LEFT");
        break;
    case collinear:
        ui->lineEdit_4->setText("COLLINEAR");
        break;
    }
}
