#pragma once
// MineField: the rules of Minesweeper, with no SFML.
//
// HALF-SPLIT. A small part of the game is already here, with tests in Tests/minesweeper_test.cpp:
//   - where the mines are (the old grid)
//   - counting how many mines touch each cell
// Everything else is still in minesweeper.cpp, on the wrong side of the seam: placing the mines at random,
// what the player can see (sgrid), revealing and flagging cells, and showing every cell when a mine is hit.
// Moving those rules in here is your job.
//
// Rules for this file:
//   - no #include <SFML/...> (directly or through another header)
//   - no global variables, no rand(), no clock, no keyboard or mouse
//   - everything stays inside namespace minesweeper_game
// You may rename the class (that is a refactoring too) - update the test file if you do.

#include <stdexcept>
#include <string>
#include <vector>

namespace minesweeper_game
{
    const int MINE = 9;   // the value of a cell with a mine in it (it is also the mine's picture in tiles.jpg)

    // Cells are numbered x = 1 to width (left to right) and y = 1 to height (top to bottom).
    // Round the outside there is a border of cells (x or y = 0, or width + 1 / height + 1) that never
    // holds a mine, so counting a cell's neighbours never goes off the edge.
    class MineField
    {
    public:
        MineField(int width = 10, int height = 10)
            : width_(width), height_(height), cells_(width + 2, std::vector<int>(height + 2, 0))
        {
        }

        // ---- Pictures: build a field from a picture, or turn it back into one ----
        // One string per row, top row (y = 1) first; the first character of a row is x = 1.
        // The field is as big as the picture. The border is not drawn.
        //   '*'        a mine
        //   '.'        no mine, and a count of 0
        //   '1' - '8'  no mine, and a count of that many
        static MineField fromRows(const std::vector<std::string>& rows)
        {
            if (rows.empty())
                throw std::invalid_argument("fromRows: the picture has no rows");

            MineField field(int(rows[0].size()), int(rows.size()));

            for (int y = 1; y <= field.height(); y++)
            {
                const std::string& row = rows[y - 1];
                if (row.size() != rows[0].size())
                    throw std::invalid_argument("fromRows: every row must be the same length");

                for (int x = 1; x <= field.width(); x++)
                {
                    char c = row[x - 1];
                    if (c == '*')
                        field.cells_[x][y] = MINE;
                    else if (c >= '1' && c <= '8')
                        field.cells_[x][y] = c - '0';
                    else if (c != '.')
                        throw std::invalid_argument(std::string("fromRows: unknown character '") + c + "'");
                }
            }
            return field;
        }

        // The field as a picture, in the same format as fromRows().
        std::vector<std::string> toRows() const
        {
            std::vector<std::string> picture;
            for (int y = 1; y <= height_; y++)
            {
                std::string row;
                for (int x = 1; x <= width_; x++)
                {
                    int value = cells_[x][y];
                    if (value == MINE) row += '*';
                    else if (value == 0) row += '.';
                    else row += char('0' + value);
                }
                picture.push_back(row);
            }
            return picture;
        }

        int width() const { return width_; }
        int height() const { return height_; }

        // ---- The mines ----
        void setMine(int x, int y) { cells_[x][y] = MINE; }
        bool isMine(int x, int y) const { return valueAt(x, y) == MINE; }

        // MINE, or how many mines touch the cell (0 to 8, once countNeighbours() has run).
        // Cells outside the field and its border count as 0.
        int valueAt(int x, int y) const
        {
            if (x < 0 || x > width_ + 1 || y < 0 || y > height_ + 1) return 0;
            return cells_[x][y];
        }

        // ---- The rule ----
        // Every cell without a mine gets the number of mines in the 8 cells around it.
        void countNeighbours()
        {
            for (int x = 1; x <= width_; x++)
                for (int y = 1; y <= height_; y++)
                {
                    if (isMine(x, y)) continue;

                    int count = 0;
                    for (int dx = -1; dx <= 1; dx++)
                        for (int dy = -1; dy <= 1; dy++)
                        {
                            bool isTheCellItself = (dx == 0 && dy == 0);
                            if (!isTheCellItself && isMine(x + dx, y + dy)) count++;
                        }
                    cells_[x][y] = count;
                }
        }

    private:
        int width_;
        int height_;
        std::vector<std::vector<int>> cells_;   // cells_[x][y], including the border
    };
}
