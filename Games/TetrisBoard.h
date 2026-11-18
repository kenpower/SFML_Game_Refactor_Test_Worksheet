#pragma once
// TetrisBoard: the rules of Tetris, with no SFML.
//
// HALF-SPLIT. A small part of the game is already here, with tests in Tests/tetris_test.cpp:
//   - the board of landed blocks
//   - checking whether a piece fits on the board (the old check())
// Everything else is still in tetris.cpp, on the wrong side of the seam: the falling piece, spawning,
// moving sideways, rotating, the gravity tick (drop and lock), and clearing full rows.
// Moving those rules in here is your job. Methods marked SCAFFOLD only exist so tetris.cpp can reach
// the board while those rules are still outside. Delete each one when nothing outside needs it.
//
// Rules for this file:
//   - no #include <SFML/...> (directly or through another header)
//   - no global variables, no rand(), no clock, no keyboard or mouse
//   - everything stays inside namespace tetris_game
// You may rename the class (that is a refactoring too) - update the test file if you do.

#include <array>
#include <stdexcept>
#include <string>
#include <vector>
#include "Geometry.h"

namespace tetris_game
{
    const int ROWS = 20;
    const int COLS = 10;
    const int EMPTY = 0;

    // A piece: four cells, each an (x = column, y = row) position on the board.
    using Piece = std::array<geom::Point, 4>;

    class TetrisBoard
    {
    public:
        // An empty board.
        TetrisBoard(int rows = ROWS, int cols = COLS)
            : field_(rows, std::vector<int>(cols, EMPTY))
        {
        }

        // ---- Pictures: build a board from a picture, or turn it back into one ----
        // One string per row, top row first. The board is as big as the picture.
        //   '.'        an empty cell
        //   '1' - '7'  a landed block of that colour
        //   '@'        a cell of a piece. The board treats it as empty: use pieceFromRows() to get the piece.
        static TetrisBoard fromRows(const std::vector<std::string>& rows)
        {
            if (rows.empty())
                throw std::invalid_argument("fromRows: the picture has no rows");

            TetrisBoard board(int(rows.size()), int(rows[0].size()));

            for (int row = 0; row < board.rows(); row++)
            {
                if (rows[row].size() != rows[0].size())
                    throw std::invalid_argument("fromRows: every row must be the same length");

                for (int col = 0; col < board.cols(); col++)
                {
                    char c = rows[row][col];
                    if (c >= '1' && c <= '7')
                        board.field_[row][col] = c - '0';
                    else if (c != '.' && c != '@')
                        throw std::invalid_argument(std::string("fromRows: unknown character '") + c + "'");
                }
            }
            return board;
        }

        // The landed blocks as a picture, in the same format as fromRows() (no '@').
        std::vector<std::string> toRows() const
        {
            std::vector<std::string> picture;
            for (const auto& fieldRow : field_)
            {
                std::string row;
                for (int cell : fieldRow)
                    row += (cell == EMPTY) ? '.' : char('0' + cell);
                picture.push_back(row);
            }
            return picture;
        }

        // ---- The board ----
        int rows() const { return int(field_.size()); }
        int cols() const { return int(field_[0].size()); }

        // The colour of the landed block at (row, col), or EMPTY.
        int cellAt(int row, int col) const { return field_[row][col]; }

        // SCAFFOLD: tetris.cpp uses this to lock a piece and to clear rows.
        // Delete it when those rules have moved into this class.
        void setCell(int row, int col, int colour) { field_[row][col] = colour; }

        // ---- The rule ----
        // True when every cell of the piece is inside the board and on an empty cell.
        // HINT: what happens to a cell with y < 0?
        bool isLegal(const Piece& piece) const
        {
            for (geom::Point cell : piece)
            {
                if (cell.x < 0 || cell.x >= cols() || cell.y >= rows()) return false;
                if (field_[cell.y][cell.x] != EMPTY) return false;
            }
            return true;
        }

    private:
        std::vector<std::vector<int>> field_;   // field_[row][col]: colour of a landed block, or EMPTY
    };

    // The piece drawn with '@' in a picture (see TetrisBoard::fromRows). There must be exactly 4 '@'s.
    // They become piece[0] to piece[3] in reading order: left to right, then top to bottom.
    inline Piece pieceFromRows(const std::vector<std::string>& rows)
    {
        Piece piece{};
        int found = 0;
        for (int row = 0; row < int(rows.size()); row++)
            for (int col = 0; col < int(rows[row].size()); col++)
                if (rows[row][col] == '@')
                {
                    if (found < 4) piece[found] = {col, row};
                    found++;
                }

        if (found != 4)
            throw std::invalid_argument("pieceFromRows: a piece needs exactly 4 '@' cells");
        return piece;
    }
}
