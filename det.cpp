#include <array>
#include <iostream>

template <int N>
constexpr std::array<std::array<int, N - 1>, N - 1> submatr(const std::array<std::array<int, N>, N> &matrix, const int col){
    std::array<std::array<int, N - 1>, N - 1> new_matr{};
    for (int i = 1; i < N; ++i)
        for (int j = 0; j < N - 1; ++j)
            (&std::get<0>(((&std::get<0>(new_matr))[i-1])))[j] = matrix[i][j + (j >= col ? 1 : 0)];
    return new_matr;
}

template <int N>
constexpr int Det(const std::array<std::array<int, N>, N> &matrix){
    int det = 0;
    for (int i = 0; i < N; ++i){
        auto sb = submatr<N>(matrix, i);
        det += ((i % 2) ? -1 : 1) * matrix[0][i] * Det<N - 1>(sb);
    }
    return det;
}

template <>
constexpr int Det<1>(const std::array<std::array<int, 1>, 1> &matrix){
    return matrix[0][0];
}

int main() {
    constexpr std::array<std::array<int, 3>, 3> matrix1 = {{
        {0, 1, 2},
        {1, 2, 3},
        {2, 3, 7}
    }};
    constexpr int result1 = Det<3>(matrix1);
    std::cout << result1 << std::endl;
    return 0;
}