#ifndef DATATABLE_H
#define DATATABLE_H

#include <QString>
#include <QVector>

class DataTable
{
public:
    DataTable() = default;
    DataTable(const size_t size, const size_t number = 0);
    DataTable(const QString& path);

    const QVector<double>& getRow(const size_t row) const;
    QVector<double>& getRow(const size_t row);
    const QVector<double> getColumn(const size_t column) const;

    const QVector<double>& getHeaderColumn() const;
    const QVector<double>& getHeaderRow() const;

    void setRow(const QVector<double> newRow, const size_t row);

    void addRow(const QVector<double> row);
    void removeLastRow();
    void clear();

private:
    QVector<QVector<double>> values;
    QVector<double> headerColumn;
    QVector<double> headerRow;

};

#endif // DATATABLE_H
