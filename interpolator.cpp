#include "interpolator.h"
#include <cmath>
#include <QDebug>

int Interpolator::findX(const QVector<double>& arr, double x)
{
    int size = arr.size();

    if (x < arr[0]) {
        return -1;
    }

    if (x >= arr[size - 1]) {
        return size - 1;
    }

    int left = 0;
    int right = size - 1;
    int m = (left + right) / 2;

    while(!(arr[m + 1] > x && arr[m] <= x))
    {
        if (arr[m] < x) {
            left = m + 1;
        } else {
            right = m - 1;
        }
        m = (left + right) / 2;
    }
    return m;
}

//get polinome for Newton interpolation
DataTable Interpolator::getPolimoneTable(const DataTable& configTable)
{
    int n = qMin(configTable.getRow(0).size(), configTable.getRow(1).size());
    DataTable polinomeTable(2);

    polinomeTable.setRow(configTable.getRow(0), 0);
    polinomeTable.setRow(configTable.getRow(1), 1);

    double dx = 0, dy = 0;
    for (int i = 1; i < n; i++) {
        dx = polinomeTable.getRow(0)[0] - polinomeTable.getRow(0)[i];

        for (int j = n - 1; j >= i; j--) {
            dy = polinomeTable.getRow(1)[j - 1] - polinomeTable.getRow(1)[j];
            polinomeTable.getRow(1)[j] = dy / dx;
        }
    }

    return polinomeTable;
}

//get value using Newton interpolation
double Interpolator::polinome(double x, const DataTable& table)
{
    double y = table.getRow(1).first();
    double tmp = 1;

    int n = qMin(table.getRow(0).size(), table.getRow(1).size());
    for (int i = 0; i < n - 1; ++i) {
        tmp *= x - table.getRow(0)[i];
        y += tmp * table.getRow(1)[i+1];
    }
    return y;
}

// Интерполяция
double Interpolator::interpolate(double x, int s,
                                 const DataTable& interpolTable)
{
    int n = qMin(interpolTable.getRow(0).size(),
                 interpolTable.getRow(1).size());
    int pos = findX(interpolTable.getRow(0), x);

    if (pos == -1 || pos - floor((s + 1) / 2.0) + 1 < 0) {
        pos = 0;    /*value close to beginning*/
    } else if (pos + ceil((s + 1) / 2.0) > n - 1) {
        pos = n - (s + 1); /*value close to end*/
    } else {
        pos -= floor((s + 1) / 2.0) - 1; /*usual value*/
    }

    DataTable configTable(2, s + 1);
    for (int i = 0; i < s + 1; i++) {
        configTable.getRow(0)[i] = interpolTable.getRow(0)[pos + i];
        configTable.getRow(1)[i] = interpolTable.getRow(1)[pos + i];
    }

    // таблица для вычисления полинома
    DataTable polinomeTable = getPolimoneTable(configTable);
    return polinome(x, polinomeTable);
}


double Interpolator::multiInterpolate(double x, double y, int sx, int sy, \
                                      const DataTable& tableXYZ)
{
    DataTable interpolTable(2);
    interpolTable.setRow(tableXYZ.getHeaderRow(), 0);

    QVector<double> ArrZ;
    int n = tableXYZ.getHeaderColumn().size();
    for (int i = 0; i < n; i++) {
        interpolTable.setRow(tableXYZ.getRow(i), 1);
        ArrZ << interpolate(y, sy, interpolTable);
    }

    interpolTable.setRow(tableXYZ.getHeaderColumn(), 0);
    interpolTable.setRow(ArrZ, 1);
    return interpolate(x, sx, interpolTable);
}
