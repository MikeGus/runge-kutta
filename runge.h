#ifndef RUNGE_H
#define RUNGE_H

#include "dataholder.h"
#include "datatable.h"

class Runge
{
    typedef double (Runge::* Func)(double, double);

public:
    Runge() = default;
    Runge(const DataHolder& constants, const DataTable& tblVol,
          const DataTable& tblCond, const DataTable& tblITM);

    DataTable calculate(double timeBegin, double timeEnd, double th);

private:
    DataHolder data;
    DataTable tableVoltage;
    DataTable tableConductivity;
    DataTable tableIT;
    DataTable tableIM;

    double f(double x, double u, double v);
    double phi(double x, double u, double v);

    double calcRp(double I);
    double integral(Func func, double a, double b, double I, double p);
    double calcP(Func funcP, double I, double a, double b);
    double P(double I, double p);

    double T(double I, double r);
    double Nt(double T, double p);
    double sigma(double T, double p);
};

#endif // RUNGE_H
