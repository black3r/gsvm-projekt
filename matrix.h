#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>
using namespace std;

class Matrix {
private:
    vector<vector<float>> _data;
    int _rows;
    int _columns;

public:
    Matrix(int rows, int columns);
    Matrix(int rows, int columns, bool resize);
    Matrix(vector<vector<float>> data);
    Matrix(std::initializer_list<std::initializer_list<float>> lst);
    vector<float> operator[](int index);
    void printMatrix();

    bool operator*=(Matrix rhs);
    bool operator+=(Matrix rhs);
    int getRows();
    int getColumns();
};

Matrix operator+(Matrix lhs, const Matrix& rhs);
Matrix operator*(Matrix lhs, const Matrix& rhs);

#endif
