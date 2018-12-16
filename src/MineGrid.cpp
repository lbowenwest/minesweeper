#include "MineGrid.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <iterator>
#include <numeric>
#include <string>


/**
 *
 */
MineGrid::MineGrid()
    : SFMLWidget{sf::VideoMode(800, 800)},
      grid{new Grid<MineSquare>(10, 10)},
      width{renderWindow.getSize().x / static_cast<float>(grid->rows())},
      height{renderWindow.getSize().y / static_cast<float>(grid->cols())} {

    spritesheet.loadFromFile("../../images/spritesheet.png");
    grid->apply([this](MineSquare& square) {
        square.setTexture(&this->spritesheet);
        square.setTextureRect(texture_rects["unknown"]);
    });

    signal_size_allocate().connect(
        sigc::hide(sigc::mem_fun(*this, &MineGrid::resize_view)));

    signal_draw().connect(sigc::bind_return(
        sigc::hide(sigc::mem_fun(*this, &MineGrid::draw)), true));

    add_events(Gdk::EventMask::BUTTON_PRESS_MASK);

    show();
}


MineGrid::~MineGrid() {
    grid.release();
}


void MineGrid::draw() {
    renderWindow.clear();
    this->display_squares();
    this->display();
}


/**
 * Overridden function from Gtk Widget to handle button presses
 *
 * @param event - Triggering event
 * @return
 */
bool MineGrid::on_button_press_event(GdkEventButton* event) {
    auto row = static_cast<std::size_t>(event->y / height);
    auto col = static_cast<std::size_t>(event->x / width);

    switch (event->button) {
        case Click::LEFT:
            left_click(row, col);
            break;
        case Click::RIGHT:
            right_click(row, col);
            break;
        default:
            break;
    }
    invalidate();
    return true;
}


void MineGrid::left_click(std::size_t row, std::size_t col) {
    // initialise board on first click
    if (!board_setup)
        initialise_board(row, col, 10);

    // if we clicked a flag don't do anythin
    if (grid->get(row, col).is_flag())
        return;

    // if we clicked on a known square reveal the adjacent squares,
    // provided we have marked as many flags as that square has mines
    if (grid->get(row, col).is_known()) {
        auto neigh = grid->neighbours(row, col);
        // calculates number of squares marked as flags
        // around clicked square
        long num_flags = std::count_if(neigh.begin(), neigh.end(), [this](coord c) {
            return is_flag(c);
        });
        if (num_flags >= grid->get(row, col).adjacent_mines) {
            for (auto& co : grid->neighbours(row, col)) {
                if (is_flag(co))
                    continue;
                reveal_square(co.first, co.second);
            }
        }
        return;
    }


    reveal_square(row, col);
}


void MineGrid::right_click(std::size_t row, std::size_t col) {
    if (grid->get(row, col).is_known())
        return;

    grid->get(row, col).toggle_flag();
    if (grid->get(row, col).is_flag()) {
        grid->get(row, col).setTextureRect(texture_rects.at("flag"));
        flag_locations.emplace_back(coord{row, col});
    } else {
        grid->get(row, col).setTextureRect(texture_rects.at("unknown"));
        flag_locations.erase(std::remove_if(
            flag_locations.begin(),
            flag_locations.end(),
            [&](coord c) {
                return c.first == row && c.second == col;
            }), flag_locations.end());
    }
}


void MineGrid::resize_view() {
    sf::Vector2f lower_right{static_cast<float>(renderWindow.getSize().x),
                             static_cast<float>(renderWindow.getSize().y)};
    sf::View view{lower_right * 0.5f, lower_right};
    renderWindow.setView(view);
    invalidate();
}


void MineGrid::display_squares() {
    update_squares();
    grid->apply([this](MineSquare& square) { renderWindow.draw(square); });
}


void MineGrid::update_squares() {
    width = renderWindow.getSize().x / static_cast<float>(grid->rows());
    height = renderWindow.getSize().y / static_cast<float>(grid->cols());

    for (auto& co : grid->all_positions()) {
        MineSquare& square = grid->get(co.first, co.second);
        square.setSize(sf::Vector2f(width, height));
        square.setPosition(co.second * width, co.first * height);
    }

}


void MineGrid::initialise_board(std::size_t row, std::size_t col, std::size_t num_mines) {
    // TODO encapsulate random mine generator
    std::random_device rd;
    std::mt19937 gen(rd());

    auto square_positions = grid->all_positions();
    std::shuffle(square_positions.begin(), square_positions.end(), gen);

    for (std::size_t i = 0; i < num_mines; ++i) {
        auto pos = square_positions[i];
        if (pos.first == row && pos.second == col) continue;
        grid->get(pos.first, pos.second).set_mine();
        mine_locations.push_back(pos);
    }

    // iterate over all mine squares and get their valid neighbours to count the number of mines
    // allocate this number to the mine square so that the right texture is set on reveal
    for (coord& co : mine_locations) {
        for (coord& i : grid->neighbours(co)) {
            grid->get(i.first, i.second).adjacent_mines++;
        }
    }

    board_setup = true;
}


void MineGrid::game_over(std::size_t row, std::size_t col) {
    // iterate over all mines and reveal ones that weren't flagged
    for (auto& c : mine_locations) {
        if (!is_flag(c))
            grid->get(c).setTextureRect(texture_rects.at("mine"));
    }
    // iterate over all flags and mark all wrongly flagged squares
    for (auto& c : flag_locations) {
        if (!is_mine(c))
            grid->get(c).setTextureRect(texture_rects.at("mine_wrong"));
    }
    grid->get(row, col).setTextureRect(texture_rects.at("mine_red"));
}


void MineGrid::reveal_square(std::size_t row, std::size_t col) {
    grid->get(row, col).reveal();
    grid->get(row, col).setTextureRect(
        texture_rects.at(std::to_string(grid->get(row, col).adjacent_mines)));

    if (grid->get(row, col).is_mine()) {
        game_over(row, col);
        return;
    }
    if (grid->get(row, col).adjacent_mines == 0) {
        for (auto& c : grid->neighbours(row, col)) {
            if (!grid->get(c).is_known())
                reveal_square(c.first, c.second);
        }
    }
}


inline bool MineGrid::is_flag(std::size_t row, std::size_t col) {
    return is_flag(coord{row, col});
}


bool MineGrid::is_flag(MineGrid::coord co) {
    auto result = std::find(flag_locations.begin(), flag_locations.end(), co);
    return result != flag_locations.end();
}


inline bool MineGrid::is_mine(std::size_t row, std::size_t col) {
    return is_mine(coord{row, col});
}


bool MineGrid::is_mine(MineGrid::coord co) {
    auto result = std::find(mine_locations.begin(), mine_locations.end(), co);
    return result != mine_locations.end();
}


