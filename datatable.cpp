#include "datatable.h"
#include <QFile>

DataTable::DataTable(const size_t size, const size_t number): values(size) {
    for (auto& row : values) {
        row.resize(number);
    }
}


DataTable::DataTable(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::ios_base::failure("Can't open given file.");
    }

    const size_t maxLength = 256;
    const char separator = '\t';
    const char endLine = '\n';

    QList<QByteArray> str = f.readLine(maxLength).split(separator);
    if (str.back().back() == endLine) {
        str.back().remove(str.back().length() - 1, 1);
    }

    for (auto number : str) {
        headerRow << number.toDouble();
    }

    while (!f.atEnd()) {
        str = f.readLine(maxLength).split(separator);
        if (str.back().back() == endLine) {
            str.back().remove(str.back().length() - 1, 1);
        }

        QVector<double> row;
        headerColumn << str.first().toDouble();

        for (auto it = str.begin() + 1; it != str.end(); ++it) {
            row << (*it).toDouble();
        }
        values.append(row);
    }

    f.close();
}

const QVector<double>& DataTable::getRow(const size_t row) const
{
    return values[row];
}


QVector<double>& DataTable::getRow(const size_t row) {
    return values[row];
}

const QVector<double> DataTable::getColumn(const size_t column) const
{
    QVector<double> result;
    for (auto row : values) {
        result.append(row[column]);
    }
    return result;
}

const QVector<double>& DataTable::getHeaderColumn() const
{
    return headerColumn;
}

const QVector<double>& DataTable::getHeaderRow() const
{
    return headerRow;
}

void DataTable::addRow(const QVector<double> row)
{
    values.append(row);
}

void DataTable::removeLastRow()
{
    values.pop_back();
}

void DataTable::clear()
{
    values.clear();
}

void DataTable::setRow(const QVector<double> newRow, const size_t row) {
    values[row] = newRow;
}
