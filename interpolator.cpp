#include "interpolator.h"
#include <cmath>

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

    QVector<double> x(configTable.getRow(0));
    QVector<double> y(configTable.getRow(1));

    double dx = 0, dy = 0;
    for (int i = 1; i < n; ++i) {
        dx = x[0] - x[i];
        for (int j = n - 1; j >= i; --j) {
            dy = y[j - 1] - y[j];
            y[j] = dy / dx;
        }
    }

    DataTable polinomeTable;
    polinomeTable.addRow(x);
    polinomeTable.addRow(y);
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
    double y = 0;
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

    QVector<double> first;
    QVector<double> second;

    for (int i = 0; i < s + 1; i++) {
        first.append(interpolTable.getRow(0)[pos + i]);
        second.append(interpolTable.getRow(1)[pos + i]);
    }

    DataTable configTable;
    configTable.addRow(first);
    configTable.addRow(second);

    DataTable polinomeTable = getPolimoneTable(configTable);
    y = polinome(x, polinomeTable);

    return y;
}


double Interpolator::multiInterpolate(double x, double y, int sx, int sy, \
                                      const DataTable& tableXYZ)
{
    DataTable interpolTable;
    interpolTable.addRow(tableXYZ.getHeaderRow());

    QVector<double> ArrZ;
    int n = tableXYZ.getHeaderColumn().size();
    for (int i = 0; i < n; i++) {
        interpolTable.addRow(tableXYZ.getRow(i));
        ArrZ << interpolate(y, sy, interpolTable);
        interpolTable.removeLastRow();
    }

    interpolTable.clear();
    interpolTable.addRow(tableXYZ.getHeaderColumn());
    interpolTable.addRow(ArrZ);
    double z = interpolate(x, sx, interpolTable);

    return z;
}
