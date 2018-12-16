#pragma once

#include <algorithm>
#include <utility>
#include <vector>

template<typename T>
class Grid {
public:
    using value_type = T;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    using coord = std::pair<std::size_t, std::size_t>;

    explicit Grid(std::size_t num);

    explicit Grid(std::size_t rows, std::size_t cols);

    explicit Grid(std::vector<T>&& vec, std::size_t rows, std::size_t cols);

    // Default Constructors
    Grid() = default;

    Grid(Grid const&) = default;

    Grid& operator=(Grid const&) = default;

    Grid(Grid&&) noexcept = default;

    Grid& operator=(Grid&&) noexcept = default;

    ~Grid() = default;

    // provide direct acces to elements
    T* data();

    const T* data() const;

    // iterators so grid can support algorithms
    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

    const_iterator cbegin() const;

    const_iterator cend() const;

    // returns a reference to the element at row r and column c
    T& get(std::size_t r, std::size_t c);

    // returns a reference to the element at coord c
    T& get(coord c);

    // returns the number of rows
    std::size_t rows();

    // returns the number of columns
    std::size_t cols();

    std::size_t index(std::size_t r, std::size_t c);

    coord position(std::size_t i);

    std::vector<coord> all_positions();

    const std::vector<coord> all_positions() const;

    bool valid_position(std::size_t r, std::size_t c);

    bool valid_position(coord c);

    std::vector<coord> neighbours(std::size_t row, std::size_t col);

    std::vector<coord> neighbours(coord c);

    template<typename F>
    Grid& apply(F f);

private:
    std::vector<T> elems{};
    std::size_t num_rows{}, num_cols{};
};


template<typename T>
Grid<T>::Grid(std::size_t num) : num_rows{num}, num_cols{num} {
    elems.resize(num_rows * num_cols);
}

template<typename T>
Grid<T>::Grid(std::size_t rows, std::size_t cols)
    : num_rows{rows}, num_cols{cols} {
    elems.resize(num_rows * num_cols);
}

template<typename T>
Grid<T>::Grid(std::vector<T>&& vec, std::size_t rows, std::size_t cols)
    : elems{std::forward(vec)}, num_rows{rows}, num_cols{cols} {}

template<typename T>
inline T* Grid<T>::data() {
    return elems.data();
}

template<typename T>
inline const T* Grid<T>::data() const {
    return elems.data();
}

template<typename T>
inline auto Grid<T>::begin() -> iterator {
    return elems.begin();
}

template<typename T>
inline auto Grid<T>::end() -> iterator {
    return elems.end();
}

template<typename T>
inline auto Grid<T>::begin() const -> const_iterator {
    return elems.begin();
}

template<typename T>
inline auto Grid<T>::end() const -> const_iterator {
    return elems.end();
}

template<typename T>
inline auto Grid<T>::cbegin() const -> const_iterator {
    return elems.cbegin();
}

template<typename T>
inline auto Grid<T>::cend() const -> const_iterator {
    return elems.cend();
}

template<typename T>
inline T& Grid<T>::get(std::size_t r, std::size_t c) {
    return elems.at(index(r, c));
}

template<typename T>
inline T& Grid<T>::get(coord c) {
    return elems.at(index(c.first, c.second));
}

template<typename T>
inline std::size_t Grid<T>::rows() {
    return num_rows;
}

template<typename T>
inline std::size_t Grid<T>::cols() {
    return num_cols;
}

template<typename T>
inline std::size_t Grid<T>::index(std::size_t r, std::size_t c) {
    return r * num_cols + c;
}

template<typename T>
inline auto Grid<T>::position(std::size_t i) -> coord {
    return std::make_pair(i % num_rows, i / num_cols);
}

template<typename T>
auto Grid<T>::all_positions() -> std::vector<coord> {
    std::vector<coord> res(elems.size());
    std::generate(res.begin(), res.end(), [i = 0, this]() mutable -> coord {
        return position(i++);
    });
    return res;
}

template<typename T>
auto Grid<T>::all_positions() const -> const std::vector<Grid::coord> {
    std::vector<coord> res(elems.size());
    std::generate(res.begin(), res.end(),
                  [i = 0, this]() mutable { return position(i++); });
    return res;
}

template<typename T>
inline bool Grid<T>::valid_position(std::size_t r, std::size_t c) {
    return (r < num_rows && c < num_cols);
}

template<typename T>
inline bool Grid<T>::valid_position(coord c) {
    return (c.first < num_rows && c.second < num_cols);
}

template<typename T>
auto Grid<T>::neighbours(std::size_t row, std::size_t col) -> std::vector<Grid::coord> {
    std::vector<coord> res;
    std::vector<coord> neighbours{
        {row - 1, col - 1},
        {row - 1, col},
        {row - 1, col + 1},
        {row,     col - 1},
        {row,     col + 1},
        {row + 1, col - 1},
        {row + 1, col},
        {row + 1, col + 1}
    };
    for (auto co : neighbours) {
        if (valid_position(co))
            res.push_back(co);
    }
    return res;
}

template<typename T>
inline auto Grid<T>::neighbours(coord c) -> std::vector<Grid::coord> {
    return neighbours(c.first, c.second);
}

template<typename T>
template<typename F>
Grid<T>& Grid<T>::apply(F f) {
    std::for_each(elems.begin(), elems.end(), f);
    return *this;
}
