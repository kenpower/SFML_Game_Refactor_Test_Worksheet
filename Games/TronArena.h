#pragma once
// TronArena: the rules of Tron, with no SFML.
//
// HALF-SPLIT. A small part of the game is already here, with tests in Tests/tron_test.cpp:
//   - the trail map: which pixels already have a trail on them (the old global field)
//   - a Bike, and how it moves one step, wrapping round the edges
// Everything else is still in tron.cpp, on the wrong side of the seam: where the bikes start, steering
// (and the rule that a bike can't turn straight back), crashing into a trail, leaving a trail, and game over.
// Moving those rules in here is your job.
//
// Rules for this file:
//   - no #include <SFML/...> (directly or through another header)
//   - no global variables, no rand(), no clock, no keyboard or mouse
//   - everything stays inside namespace tron_game
// You may rename the class (that is a refactoring too) - update the test file if you do.

#include <stdexcept>
#include <string>
#include <vector>

namespace tron_game
{
    // A player's bike: where it is, and which way it is going.
    struct Bike
    {
        int x = 0;
        int y = 0;
        int dir = 0;

        // Moves one pixel in direction dir, wrapping round to the other side of a width x height arena.
        void tick(int width, int height)
        {
            // HINT: what do the dir values 0-3 mean? Compare with the steering keys in tron.cpp.
            if (dir==0) y+=1;
            if (dir==1) x-=1;
            if (dir==2) x+=1;
            if (dir==3) y-=1;

            if (x>=width) x=0;  if (x<0) x=width-1;
            if (y>=height) y=0;  if (y<0) y=height-1;
        }
    };

    class TronArena
    {
    public:
        // An arena with no trails. The game's arena is 600 x 480; tests can use a tiny one.
        // The trails are kept in a std::vector (on the heap): 600 x 480 bools inside an object
        // created as a local variable could overflow the stack.
        TronArena(int width, int height)
            : width_(width), height_(height), trail_(width * height, false)
        {
        }

        // ---- Pictures: build an arena from a picture, or turn it back into one ----
        // One string per row, top row (y = 0) first. The arena is as big as the picture.
        //   '.'  no trail     '#'  a trail
        static TronArena fromRows(const std::vector<std::string>& rows)
        {
            if (rows.empty())
                throw std::invalid_argument("fromRows: the picture has no rows");

            TronArena arena(int(rows[0].size()), int(rows.size()));

            for (int y = 0; y < arena.height(); y++)
            {
                if (rows[y].size() != rows[0].size())
                    throw std::invalid_argument("fromRows: every row must be the same length");

                for (int x = 0; x < arena.width(); x++)
                {
                    char c = rows[y][x];
                    if (c == '#')
                        arena.markTrail(x, y);
                    else if (c != '.')
                        throw std::invalid_argument(std::string("fromRows: unknown character '") + c + "'");
                }
            }
            return arena;
        }

        // The arena as a picture, in the same format as fromRows().
        std::vector<std::string> toRows() const
        {
            std::vector<std::string> picture;
            for (int y = 0; y < height_; y++)
            {
                std::string row;
                for (int x = 0; x < width_; x++)
                    row += isTrail(x, y) ? '#' : '.';
                picture.push_back(row);
            }
            return picture;
        }

        int width() const { return width_; }
        int height() const { return height_; }

        // ---- The trail map ----
        bool isTrail(int x, int y) const { return trail_[y * width_ + x]; }
        void markTrail(int x, int y) { trail_[y * width_ + x] = true; }

    private:
        int width_;
        int height_;
        std::vector<bool> trail_;   // trail_[y * width_ + x]
    };
}
