#pragma once
// PipeGrid: the rules of Netwalk, with no SFML.
//
// HALF-SPLIT. A small part of the game is already here, with tests in Tests/netwalk_test.cpp:
//   - GridPos and the four directions
//   - a Pipe: which sides it connects to, and turning it a quarter clockwise
//   - the grid of pipes (the old global grid), and whether a position is off the grid
// Everything else is still in netwalk.cpp, on the wrong side of the seam: generating the puzzle,
// working out each pipe's orientation and shuffling it, placing the server, rotating the pipe the player
// clicks, and finding which pipes connect to the server. Moving those rules in here is your job.
// Methods marked SCAFFOLD only exist so netwalk.cpp can reach the grid while those rules are still outside.
//
// Rules for this file:
//   - no #include <SFML/...> (directly or through another header)
//   - no global variables, no rand(), no clock, no keyboard or mouse
//   - everything stays inside namespace netwalk_game
// You may rename the class (that is a refactoring too) - update the test file if you do.

#include <stdexcept>
#include <string>
#include <vector>

namespace netwalk_game
{
    // ---- A cell on the board, or a step from one cell to the next ----
    struct GridPos
    {
        int x, y;
        GridPos() : x(0), y(0) {}
        GridPos(int x, int y) : x(x), y(y) {}
    };

    inline GridPos operator+(GridPos a, GridPos b) { return GridPos(a.x + b.x, a.y + b.y); }
    inline GridPos operator-(GridPos a) { return GridPos(-a.x, -a.y); }
    inline bool operator==(GridPos a, GridPos b) { return a.x == b.x && a.y == b.y; }

    inline const GridPos Up(0, -1);
    inline const GridPos Down(0, 1);
    inline const GridPos Right(1, 0);
    inline const GridPos Left(-1, 0);

    inline const GridPos DIR[4] = {Up, Right, Down, Left};

    // ---- One pipe tile: which sides it connects to, plus rotation state ----
    // HINT: orientation is part of the puzzle; angle is only the animated picture of it (see DRAW in netwalk.cpp).
    struct Pipe
    {
        std::vector<GridPos> dirs;
        int orientation = 0;
        float angle = 0;
        bool on = false;

        // Turns the pipe a quarter clockwise.
        void rotate()
        {
            for (int i = 0; i < int(dirs.size()); i++)
                if (dirs[i] == Up) dirs[i] = Right;
                else if (dirs[i] == Right) dirs[i] = Down;
                else if (dirs[i] == Down) dirs[i] = Left;
                else if (dirs[i] == Left) dirs[i] = Up;
        }

        // True when the pipe has an arm pointing in direction dir.
        bool isConnect(GridPos dir) const
        {
            for (auto d : dirs)
                if (d == dir) return true;
            return false;
        }
    };

    class PipeGrid
    {
    public:
        // A size x size grid of empty cells (no pipe arms).
        explicit PipeGrid(int size = 6) : size_(size), cells_(size * size) {}

        // ---- Pictures: build a grid from a picture, or turn it back into one ----
        // Each cell is 3 x 3 characters, so a grid of size n is 3n strings of 3n characters:
        //
        //    " | "    '|' above the middle: an arm pointing Up
        //    "-o-"    '-' left or right of the middle: an arm pointing Left or Right
        //    " | "    '|' below the middle: an arm pointing Down
        //
        // The middle is 'o' for a pipe, '*' for a pipe that is on (lit), or ' ' for an empty cell.
        // An arm only says which way THIS pipe points: its neighbour may not point back.
        // Example: a 2 x 2 grid where the top two pipes join, and the bottom-left one points up but isn't joined:
        //    "      "
        //    " o--o "
        //    "      "
        //    " |    "
        //    " o    "
        //    "      "
        //   (the top-left pipe has no Down arm, so the bottom-left pipe's Up arm meets nothing)
        static PipeGrid fromRows(const std::vector<std::string>& rows)
        {
            int size = int(rows.size()) / 3;
            if (rows.empty() || int(rows.size()) % 3 != 0)
                throw std::invalid_argument("fromRows: a grid of size n needs 3n rows");

            PipeGrid grid(size);
            for (const std::string& row : rows)
                if (int(row.size()) != 3 * size)
                    throw std::invalid_argument("fromRows: a grid of size n needs 3n characters in every row");

            for (int x = 0; x < size; x++)
                for (int y = 0; y < size; y++)
                {
                    auto at = [&](int dx, int dy) { return rows[3 * y + 1 + dy][3 * x + 1 + dx]; };
                    Pipe& pipe = grid.cell(GridPos(x, y));

                    if (at(0, -1) == '|') pipe.dirs.push_back(Up);
                    if (at(1, 0) == '-') pipe.dirs.push_back(Right);
                    if (at(0, 1) == '|') pipe.dirs.push_back(Down);
                    if (at(-1, 0) == '-') pipe.dirs.push_back(Left);

                    char middle = at(0, 0);
                    if (middle == '*') pipe.on = true;
                    else if (middle != 'o' && middle != ' ')
                        throw std::invalid_argument(std::string("fromRows: the middle of a cell can't be '") + middle + "'");
                }
            return grid;
        }

        // The grid as a picture, in the same format as fromRows().
        std::vector<std::string> toRows() const
        {
            std::vector<std::string> picture(3 * size_, std::string(3 * size_, ' '));
            for (int x = 0; x < size_; x++)
                for (int y = 0; y < size_; y++)
                {
                    auto at = [&](int dx, int dy) -> char& { return picture[3 * y + 1 + dy][3 * x + 1 + dx]; };
                    const Pipe& pipe = cell(GridPos(x, y));

                    if (pipe.isConnect(Up)) at(0, -1) = '|';
                    if (pipe.isConnect(Right)) at(1, 0) = '-';
                    if (pipe.isConnect(Down)) at(0, 1) = '|';
                    if (pipe.isConnect(Left)) at(-1, 0) = '-';

                    if (pipe.on) at(0, 0) = '*';
                    else if (!pipe.dirs.empty()) at(0, 0) = 'o';
                }
            return picture;
        }

        int size() const { return size_; }

        // True when v is off the grid.
        bool isOut(GridPos v) const { return !(v.x >= 0 && v.x < size_ && v.y >= 0 && v.y < size_); }

        // SCAFFOLD: netwalk.cpp changes pipes through this (generating, shuffling, rotating, lighting them up).
        // When those rules have moved into this class, make it private or delete it.
        Pipe& cell(GridPos v) { return cells_[v.x * size_ + v.y]; }

        const Pipe& cell(GridPos v) const { return cells_[v.x * size_ + v.y]; }

    private:
        int size_;
        std::vector<Pipe> cells_;   // cells_[x * size_ + y]
    };
}
