#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <string>

class Square {
public:
    enum Type { BLANK, MINE };

    Square() = default;
    Square(Square&) = default;
    Square& operator=(const Square&) = default;
    Square(Square&&) = default;
    Square& operator=(Square&&) = default;
    ~Square() = default;

    bool is_mine();
    bool is_flag();
    bool is_known();

    virtual void reset();
    virtual void reveal();
    virtual void set_mine();
    virtual void mark_flag();
    virtual void remove_flag();
    virtual void toggle_flag();

    int adjacent_mines {0};

private:
    bool known{false};
    bool flagged{false};
    Type type{Type::BLANK};
};


inline void Square::reveal() {
    known = true;
}

inline void Square::set_mine() {
    type = Type::MINE;
}

inline void Square::mark_flag() {
    flagged = true;
}

inline void Square::remove_flag() {
    flagged = false;
}

inline void Square::toggle_flag() {
    flagged = !flagged;
}

inline void Square::reset() {
    known = false;
    flagged = false;
}

inline bool Square::is_known() {
    return known;
}

inline bool Square::is_flag() {
    return flagged;
}

inline bool Square::is_mine() {
    return type == Type::MINE;
}



class MineSquare : public Square, public sf::RectangleShape {
public:
    MineSquare() : sf::RectangleShape{} {};
    MineSquare(MineSquare&) = default;
    MineSquare& operator=(MineSquare const&) = default;
    MineSquare(MineSquare&&) = default;
    MineSquare& operator=(MineSquare&&) = default;

};
