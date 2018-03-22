#ifndef DATATABLE_H
#define DATATABLE_H

#include <QString>
#include <QVector>

class DataTable
{
public:
    DataTable() = default;
    DataTable(const QString& path);

    const QVector<double>& getRow(const size_t row) const;
    const QVector<double> getColumn(const size_t column) const;

    const QVector<double>& getHeaderColumn() const;
    const QVector<double>& getHeaderRow() const;

private:
    QVector<QVector<double>> values;
    QVector<double> headerColumn;
    QVector<double> headerRow;

};

#endif // DATATABLE_H
