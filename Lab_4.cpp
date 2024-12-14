// Lab_4.cpp 

#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <cmath>

// Шаблонный класс для разреженного вектора
template<typename T>
class SparseVector {
private:
    std::unordered_map<size_t, T> data;
    size_t size;

public:
    SparseVector(size_t n) : size(n) {}

    T& operator[](size_t index) {
        if (index >= size)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T operator[](size_t index) const {
        auto it = data.find(index);
        return (it != data.end()) ? it->second : T();
    }

    size_t getSize() const { return size; }

    T dotProduct(const SparseVector<T>& other) const {
        if (size != other.size)
            throw std::invalid_argument("Vectors must be of the same size");
        T result = T();
        for (const auto& [index, value] : data) {
            auto it = other.data.find(index);
            if (it != other.data.end()) {
                result += value * it->second;
            }
        }
        return result;
    }

    void print() const {
        for (const auto& [index, value] : data) {
            std::cout << "Element [" << index << "] = " << value << "\n";
        }
    }
};

// Хэш-функция для пар индексов
struct pair_hash {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

// Шаблонный класс для разреженной матрицы
template<typename T>
class SparseMatrix {
private:
    std::unordered_map<std::pair<size_t, size_t>, T, pair_hash> data;
    size_t rows, cols;

public:
    SparseMatrix(size_t r, size_t c) : rows(r), cols(c) {}

    T& operator()(size_t row, size_t col) {
        if (row >= rows || col >= cols)
            throw std::out_of_range("Index out of range");
        return data[{row, col}];
    }

    const T operator()(size_t row, size_t col) const {
        auto it = data.find({ row, col });
        return (it != data.end()) ? it->second : T();
    }

    SparseMatrix<T> transpose() const {
        SparseMatrix<T> result(cols, rows);
        for (const auto& [key, value] : data) {
            result(key.second, key.first) = value;
        }
        return result;
    }

    SparseMatrix<T> operator+(const SparseMatrix<T>& other) const {
        if (rows != other.rows || cols != other.cols)
            throw std::invalid_argument("Matrix dimensions must match");
        SparseMatrix<T> result(rows, cols);
        result.data = data;

        for (const auto& [key, value] : other.data) {
            result.data[key] += value;
        }
        return result;
    }

    void print() const {
        for (const auto& [key, value] : data) {
            std::cout << "Element (" << key.first << ", " << key.second << ") = " << value << "\n";
        }
    }
};

int main() {
    // Тест разреженного вектора
    std::cout << "Sparse Vector Test:\n";
    SparseVector<double> vec1(5), vec2(5);
    vec1[1] = 2.0;
    vec1[3] = 3.5;

    vec2[1] = 1.5;
    vec2[2] = 4.0;

    std::cout << "Vector 1:\n";
    vec1.print();

    std::cout << "Vector 2:\n";
    vec2.print();

    std::cout << "Dot Product: " << vec1.dotProduct(vec2) << "\n";

    // Тест разреженной матрицы
    std::cout << "\nSparse Matrix Test:\n";
    SparseMatrix<double> mat1(3, 3), mat2(3, 3);
    mat1(0, 1) = 2.0;
    mat1(1, 2) = 3.5;

    mat2(0, 1) = 1.5;
    mat2(2, 2) = 4.0;

    std::cout << "Matrix 1:\n";
    mat1.print();

    std::cout << "Matrix 2:\n";
    mat2.print();

    SparseMatrix<double> matSum = mat1 + mat2;
    std::cout << "Matrix Sum:\n";
    matSum.print();

    SparseMatrix<double> matTransposed = mat1.transpose();
    std::cout << "Matrix 1 Transposed:\n";
    matTransposed.print();

    return 0;
}
