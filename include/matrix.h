#ifndef MATRIX_H
#define MATRIX_H
#include <vector>

enum MATRIX_ERROR { INDEX_OOB, SIZE_MISMATCH, NOT_INVERTIBLE };

template <typename T>
class Matrix
{
public:
    //Matrix(size_t n);
    Matrix(size_t n, const T& value);
    //Matrix(size_t rows, size_t cols);
    Matrix(size_t rows, size_t cols, const T &value);
    T Determinant() const;
    Matrix<T> ref() const;
    Matrix<T> rref() const;
    size_t Rows() const;
    size_t Cols() const;
    static Matrix<T> Identity(size_t n, const T& multIdent, const T& addIdent);
    Matrix<T> Augment(const Matrix<T>& RHS) const;
    T& Item(size_t row, size_t col);
    const T valueAt(size_t row, size_t col) const;
    T& operator()(size_t row, size_t col);
    Matrix<T> operator +(const Matrix<T>& RHS) const;
    Matrix<T> operator -(const Matrix<T>& RHS) const;
    Matrix<T> operator *(const Matrix<T>& RHS) const;
    Matrix<T> operator -() const; //Negation
    Matrix<T> operator !() const; //Inverse
    Matrix<T> operator ~() const; //Transpose
    Matrix<T> operator *(const T& RHS) const;

    template <typename U>
    friend Matrix<U> operator *(const U& LHS, const Matrix<U>& RHS);
private:
    void init(const T& value);
    size_t numRows,numCols;
    std::vector<T> data;
};

/*
template <typename T>
Matrix<T>::Matrix(size_t n): numRows(n), numCols(n) {
    if(!n)
        throw SIZE_MISMATCH;
    T temp;
    init(temp);
}
*/
template <typename T>
Matrix<T>::Matrix(size_t n, const T &value): numRows(n), numCols(n) {
    if(!n)
        throw SIZE_MISMATCH;
    init(value);
}
/*
template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols): numRows(rows), numCols(cols) {
    if(!(rows && cols))
        throw SIZE_MISMATCH;
    T temp;
    init(temp);
}
*/
template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols, const T& value): numRows(rows), numCols(cols)
{
    init(value);
}

template <typename T>
T Matrix<T>::Determinant() const {
    T temp;
    return temp;
}

template <typename T>
Matrix<T> Matrix<T>::ref() const{
    Matrix<T> temp(numRows,numCols);
    return temp;
}

template <typename T>
Matrix<T> Matrix<T>::rref() const{
    Matrix<T> temp(numRows,numCols);
    return temp;
}

template <typename T>
size_t Matrix<T>::Rows() const {
    return numRows;
}

template <typename T>
size_t Matrix<T>::Cols() const {
    return numCols;
}

template <typename T>
Matrix<T> Matrix<T>::Identity(size_t n, const T& multIdent, const T& addIdent) {
    Matrix<T> result(n,n);
    for(size_t i = 0; i < n; i++)
        for(size_t j = 0; j < n; j++)
            result(i,j) = (i==j) ? multIdent : addIdent;
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::Augment(const Matrix<T>& RHS) const{
    if(numRows != RHS.numRows)
        throw SIZE_MISMATCH;
    Matrix<T> result = *this;
    result.numCols += RHS.numCols;
    typename std::vector<T>::iterator it;
    typename std::vector<T>::const_iterator start = RHS.data.begin();
    for(size_t i = 0; i < numRows; i++) {
        it = result.data.begin() + (i*result.numCols) + numCols;
        result.data.insert(it,start,start + RHS.numCols);
        start += RHS.numCols;
    }
    return result;
}

template <typename T>
T& Matrix<T>::Item(size_t row, size_t col) {
    if(row > numRows - 1  || col > numCols - 1)
        throw INDEX_OOB;
    return data[row*numCols + col];
}

template <typename T>
const T Matrix<T>::valueAt(size_t row, size_t col) const{
    if(row > numRows - 1  || col > numCols - 1)
        throw INDEX_OOB;
    return data[row*numCols + col];
}

template <typename T>
Matrix<T> Matrix<T>::operator +(const Matrix<T>& RHS) const{
    if(!(numRows == RHS.numRows || numCols != RHS.numCols) )
        throw SIZE_MISMATCH;
    Matrix<T> result(numRows, numCols);
    for(size_t i = 0; i < numRows; i++)
        for(size_t j = 0; j < numCols; j++)
            result(i,j) = valueAt(i,j) + RHS.valueAt(i,j);
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator -(const Matrix<T>& RHS) const{
    if(!(numRows == RHS.numRows || numCols != RHS.numCols) )
        throw SIZE_MISMATCH;
    Matrix<T> result(numRows, numCols);
    for(size_t i = 0; i < numRows; i++)
        for(size_t j = 0; j < numCols; j++)
            result(i,j) = valueAt(i,j) - RHS.valueAt(i,j);
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator *(const Matrix<T>& RHS) const{
    if(numCols != RHS.numRows)
        throw SIZE_MISMATCH;
    Matrix<T> result(numRows,RHS.numRows);
    T temp;
    for(size_t i = 0; i < result.numRows; i++) {
        for(size_t j = 0; j < result.numCols; j++) {
            temp = valueAt(i,0) * RHS.valueAt(0,j);
            for(size_t k = 1; k < numCols; k++)
                temp = temp + (valueAt(i,k)*RHS.valueAt(k,j));
            result(i,j) = temp;
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator -() const{
    Matrix<T> result(numRows,numCols);
    for(size_t i = 0; i < numRows; i++)
        for(size_t j = 0; j < numCols; j++)
            result(i,j) = -valueAt(i,j);
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator !() const{
    if(numRows != numCols)
        throw NOT_INVERTIBLE;
    Matrix<T> result(numRows,numCols);
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator ~() const{
    Matrix<T> result(numRows,numCols);
    for(size_t i = 0; i < numRows; i++)
        for(size_t j = 0; j < numCols; j++)
            result(i,j) = valueAt(j,i);
    return result;
}

template <typename T>
T& Matrix<T>::operator()(size_t row, size_t col) {
    return Item(row,col);
}

template <typename T>
Matrix<T> Matrix<T>::operator *(const T& RHS) const{
    Matrix<T> result(numRows,numCols);
    for(size_t i = 0; i < numRows; i++)
        for(size_t j = 0; j < numCols; j++)
            result(i,j) = valueAt(i,j) * RHS;
    return result;
}

template <typename T>
void Matrix<T>::init(const T &value)
{
    for(size_t i = 0; i < numRows * numCols; i++ )
        data.push_back(value);
}

template <typename T>
Matrix<T> operator *(const T& LHS, const Matrix<T>& RHS) {
    return RHS * LHS;
}

#endif // MATRIX_H
