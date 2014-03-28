#include "matrix.h"
#include <iostream>
using namespace std;

Matrix::Matrix(int rows, int columns) {
    this->_rows = rows;
    this->_columns = columns;
}

Matrix::Matrix(int rows, int columns, bool resize) :
Matrix::Matrix(rows, columns) {
    if (resize) {
        _data.resize(rows);
        for (auto &col: _data) col.resize(columns);
    }
}

Matrix::Matrix(vector<vector<float>> data) :
Matrix::Matrix(data.size(), data.size() ? data.begin()->size() : 0) {
    this->_data = data;
}

Matrix::Matrix(std::initializer_list<std::initializer_list<float>> lst) :
Matrix::Matrix(lst.size(), lst.size() ? lst.begin()->size() : 0, true) {
    int i = 0, j = 0;
    for (const auto &row : lst) {
        for (const auto &obj : row) {
            _data[i][j] = obj;
            ++j;
        }
        j = 0;
        ++i;
    }
}

vector<float> Matrix::operator[](int index) {
    return _data[index];
}

void Matrix::printMatrix() {
    for (auto &row : _data) {
        for (auto &obj : row) {
            cout << obj << " ";
        }
        cout << endl;
    }
}

int Matrix::getRows() {
    return this->_rows;
}

int Matrix::getColumns() {
    return this->_columns;
}

bool Matrix::operator*=(Matrix rhs) {
    if (this->getColumns() != rhs.getRows()) {
        return false;
    } else {
        vector<vector<float>> res;
        res.resize(this->getRows());
        for (auto &v : res) v.resize(rhs.getColumns());
        for (int i = 0; i < this->getRows(); i++) {
            for (int j = 0; j < rhs.getColumns(); j++) {
                // bunka [i][j] vo vyslednej matici
                float r = 0;
                for (int k = 0; k < this->getColumns(); k++) {
                    r += _data[i][k] * rhs[k][j];
                }
                res[i][j] = r;
            }
        }
        _data = res;
        _rows = res.size();
        _columns = res[0].size();
        return true;
    }
}

bool Matrix::operator+=(Matrix rhs) {
    if (rhs.getRows() != this->getRows() || rhs.getColumns() != this->getColumns()) {
        return false;
    } else {
        for (int i = 0; i < _rows; i++) {
            for (int j = 0; j < _rows; j++) {
                _data[i][j] += rhs[i][j];
            }
        }
        return true;
    }
}

Matrix operator+(Matrix lhs, const Matrix& rhs) {
        lhs += rhs;
        return lhs;
}

Matrix operator*(Matrix lhs, const Matrix& rhs) {
        lhs *= rhs;
        return lhs;
}
