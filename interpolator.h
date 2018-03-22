#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include "datatable.h"

class Interpolator
{
public:
    Interpolator();
    static double interpolate(double x, int s,
                              const DataTable& interpolationTable);
    static double multiInterpolate(double x, double y, int sx, int sy,
                                   const DataTable& tableXYZ);

private:
    static int findX(const QVector<double>& arr, double x);
    static DataTable getPolimoneTable(const DataTable& configTable);
    static double polinome(double x, const DataTable& table);
};

#endif // INTERPOLATOR_H
