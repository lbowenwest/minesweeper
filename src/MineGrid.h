//
// Created by lygon on 28/09/18.
//

#pragma once

#include <memory>
#include <map>

#include "Grid.h"
#include "Square.h"
#include "SFMLWidget.h"


class MineGrid : public SFMLWidget {
public:
    using coord = std::pair<std::size_t, std::size_t>;
    enum Click : int {
        LEFT = 1, MIDDLE = 2, RIGHT = 3
    };

//    explicit MineGrid(Grid<Square>& g);
    MineGrid();
    ~MineGrid() override;
    void draw();

    /**
     *
     * @param row
     * @param col
     */
    void reveal_square(std::size_t row, std::size_t col);

    void game_over(std::size_t row, std::size_t col);

    std::vector<coord> mine_locations{};
    std::vector<coord> flag_locations{};
protected:
    bool on_button_press_event(GdkEventButton* event) override;

private:
    std::unique_ptr<Grid<MineSquare>> grid;
    float width{}, height{};
    bool board_setup{false};

    sf::Texture spritesheet;
    std::map<std::string, sf::IntRect> texture_rects{
        {"unknown",    {2,   53, 16, 16}},
        {"blank",      {19,  53, 16, 16}},
        {"0",          {19,  53, 16, 16}},
        {"1",          {2,   70, 16, 16}},
        {"2",          {19,  70, 16, 16}},
        {"3",          {36,  70, 16, 16}},
        {"4",          {53,  70, 16, 16}},
        {"5",          {70,  70, 16, 16}},
        {"6",          {87,  70, 16, 16}},
        {"7",          {104, 70, 16, 16}},
        {"8",          {121, 70, 16, 16}},
        {"flag",       {36,  53, 16, 16}},
        {"mine",       {87,  53, 16, 16}},
        {"mine_red",   {104, 53, 16, 16}},
        {"mine_wrong", {121, 53, 16, 16}}
    };

    /// Recalculates view window and invalidates widget triggering redraw
    void resize_view();

    /// Updates and then draws squares onto render window
    void display_squares();

    /// Resizes and repositions squares based on current view
    void update_squares();


    /// Allocates set number of squares as mines.
    /**
     * Triggered on first click on board
     * chooses random squares to be assigned as mines,
     * ignoring the initially clicked square.
     * Once mines are assigned triggers a count of all squares
     * adjacent to mines and increments a variable indicating
     * the number of mines a square has as neighbours.
     *
     * @param row Row of initially clicked square
     * @param col Column of initially clicked square
     * @param num_mines Number of mines to allocate
     */
    void initialise_board(std::size_t row, std::size_t col, std::size_t num_mines);


    /// Triggers a left click on a square.
    /**
     * Triggers a left clck of a square,
     * reveals the square unless it is a flag.
     * If the square is already known, and there are
     * as many marked flags around it as mines, then
     * reveal all neighbouring squares,
     * this is for convenience when playing.
     *
     * @param row - Row of clicked square
     * @param col - Column of clicked square
     */
    void left_click(std::size_t row, std::size_t col);

    /**
     * Triggered on a right click of a square,
     * this toggles the square as a flag
     *
     * @param row - Row of clicked square
     * @param col - Column of clicked square
     */
    void right_click(std::size_t row, std::size_t col);

    /**
     * Searches vector of flag locations for square at
     * given position
     *
     * @param row - Row of square to check for flag
     * @param col - Column of square to check for flag
     * @return True if flag, false if not
     */
    bool is_flag(std::size_t row, std::size_t col);

    /**
     * Searches vector of flag locations for square at
     * given position
     *
     * @param co - Coordinates to search for flag
     * @return True if flag, false if not
     */
    bool is_flag(coord co);

    /**
     * Searches vector of mine locations for given square
     *
     * @param row - Row of square to check
     * @param col - Column of square to check
     * @return true if mine, false if not
     */
    bool is_mine(std::size_t row, std::size_t col);

    /**
     * Searches vector of mine locations for given square
     *
     * @param co - Coordinates of square to check
     * @return true if mine, false if not
     */
    bool is_mine(coord co);

};

