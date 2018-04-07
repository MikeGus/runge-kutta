#include "runge.h"
#include <cmath>
#include "interpolator.h"
#include <QDebug>

#define _USE_MATH_DEFINES
#define EPS 1e-4

Runge::Runge(const DataHolder &constants, const DataTable &tblVol,
             const DataTable &tblCond, const DataTable &tblITM):
    data(constants), tableVoltage(tblVol), tableConductivity(tblCond)
{
    tableIT.addRow(tblITM.getRow(0));
    tableIT.addRow(tblITM.getRow(1));
    tableIM.addRow(tblITM.getRow(0));
    tableIM.addRow(tblITM.getRow(2));
}


double Runge::f(double x, double u, double v)
{
    Q_UNUSED(x)
    return (- (calcRp(u) + data.Rk) * u + v) / data.Lk;
}

double Runge::phi(double x, double u, double v)
{
    Q_UNUSED(x)
    Q_UNUSED(v)
    return - u / data.Ck;
}

double Runge::calcRp(double I)
{
    double p = calcP(&Runge::P, I, 0, 30);
    return data.l / (2 * M_PI * integral(&Runge::sigma, 0, data.R, I, p));
}

double Runge::integral(Func func, double a, double b, double I, double p)
{
    double H = (b - a) / 6;
    double c = (a + b) / 2;
    double S = (this->*func)(T(I, a), p) * a + (this->*func)(T(I, b), p) * b
            + 4 * (this->*func)(T(I, c), p) * c;
    return S * H;
}

double Runge::calcP(Func funcP, double I, double a, double b)
{
    double p_t = data.P0 * data.R * data.R * 7242 / 2 / data.Tn;
    double c = (a + b) / 2;
    while (fabs(b - a) >= EPS + EPS * fabs(c)) {
        if ((p_t - (this->*funcP)(I, a)) * (p_t - (this->*funcP)(I, c)) < 0) {
            b = c;
        } else {
            a = c;
        }
        c = (a + b) / 2;
    }
    return c;
}

double Runge::T(double I, double r)
{
    double T0 = Interpolator::interpolate(I, 1, tableIT);
    double n = Interpolator::interpolate(I, 1, tableIM);

    double Tw = 2000;
    return (Tw - T0) * pow(r / data.R, n) + T0;
}

double Runge::Nt(double T, double p)
{
    return Interpolator::multiInterpolate(T, p, 3, 2, tableVoltage);
}

double Runge::P(double I, double p)
{
    return integral(&Runge::Nt, 0, data.R, I, p);
}

double Runge::sigma(double T, double p)
{
    return Interpolator::multiInterpolate(T, p, 3, 2, tableVoltage);
}

DataTable Runge::calculate(double timeBegin, double timeEnd, double th)
{
    double yn = data.I0;
    double zn = data.UC0;
    QVector<double> I;
    QVector<double> U;
    QVector<double> R;

    for (double time = timeBegin; time < timeEnd; time += th)
    {
        I << yn;
        U << zn;
        R << calcRp(yn);

        double k1 = f(time, yn, zn);
        double p1 = phi(time, yn, zn);
        double k2 = f(time + th / 2, yn + th / 2 * k1, zn + th / 2 * p1);
        double p2 = phi(time + th / 2, yn + th / 2 * k1, zn + th / 2 * p1);
        double k3 = f(time + th / 2, yn + th / 2 * k2, zn + th / 2 * p2);
        double p3 = phi(time + th / 2, yn + th / 2 * k2, zn + th / 2 * p2);
        double k4 = f(time + th, yn + th * k3, zn + th * p3);
        double p4 = phi(time + th, yn + th * k3, zn + th * p3);

        yn += th / 6. * (k1 + 2 * k2 + 2 * k3 + k4);
        zn += th / 6. * (p1 + 2 * p2 + 2 * p3 + p4);

        qDebug() << time;
    }

    DataTable table;
    table.addRow(I);
    table.addRow(U);
    table.addRow(R);
    return table;
}
