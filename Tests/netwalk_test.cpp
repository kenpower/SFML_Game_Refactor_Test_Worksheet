// Unit tests for Netwalk.
//
// Every .cpp in Tests/ is built into the Tests program automatically.
// Only work in the file for YOUR game.
//
// The class under test is in Games/PipeGrid.h. The Tests program does not link SFML, so if that
// header pulls in SFML, this file will not compile. That is deliberate: it proves your class is free of SFML.
//
// How this file is laid out:
//   1. Finished tests for what is already in PipeGrid.h. Read them: they show the pattern.
//   2. Three tests for one rule (turning a pipe) that get less finished as you go:
//        normal case     - finished
//        edge case       - Arrange and Act are written; you write the Assert
//        does not apply  - only the name; you write all of it
//      The unfinished ones call GTEST_SKIP(), so they show as "skipped" until you finish them.
//      Delete the GTEST_SKIP() line when you write the test.
//   3. Your own tests go at the bottom, named after behaviours, e.g. TEST(PipeGrid, PipeJoinedToTheServerIsLit).
//
// Pictures: PipeGrid::fromRows() builds a grid from a picture, and toRows() turns it back into one,
// so a test can set up and check a grid in a way you can read. Each cell is 3 x 3 characters:
//    " | "   '|' above / below the middle: an arm pointing Up / Down
//    "-o-"   '-' left / right of the middle: an arm pointing Left / Right
//    " | "   middle: 'o' a pipe, '*' a lit pipe, ' ' an empty cell
// See the comment above PipeGrid::fromRows() for a full example.

#include <gtest/gtest.h>
#include "PipeGrid.h"

using namespace netwalk_game;

using Rows = std::vector<std::string>;

// ===== 1. Finished examples =====

TEST(PipeGrid, PictureOfAGridComesBackUnchanged)
{
    // Arrange - a 2 x 2 grid: the top two pipes are joined and lit; the bottom-left one points up, but isn't joined.
    Rows picture = {
        "      ",
        " *--* ",
        "      ",
        " |    ",
        " o    ",
        "      ",
    };

    // Act
    PipeGrid grid = PipeGrid::fromRows(picture);

    // Assert
    EXPECT_EQ(picture, grid.toRows());
}

TEST(PipeGrid, PipeConnectsOnlyTheWaysItsArmsPoint)
{
    // Arrange - one cell, with arms pointing Up and Right.
    PipeGrid grid = PipeGrid::fromRows({
        " | ",
        " o-",
        "   ",
    });

    // Act
    const Pipe& pipe = grid.cell(GridPos(0, 0));

    // Assert
    EXPECT_TRUE(pipe.isConnect(Up));
    EXPECT_TRUE(pipe.isConnect(Right));
    EXPECT_FALSE(pipe.isConnect(Down));
    EXPECT_FALSE(pipe.isConnect(Left));
}

// ===== 2. One rule, three tests: turning a pipe =====

// Normal case - finished.
TEST(PipeGrid, PipeTurnsAQuarterClockwise)
{
    // Arrange
    PipeGrid grid = PipeGrid::fromRows({
        " | ",
        " o-",
        "   ",
    });

    // Act
    grid.cell(GridPos(0, 0)).rotate();

    // Assert
    Rows expected = {
        "   ",
        " o-",
        " | ",
    };
    EXPECT_EQ(expected, grid.toRows());
}

// Edge case - write the Assert.
TEST(PipeGrid, FourTurnsBringAPipeBackToWhereItStarted)
{
    // Arrange
    Rows picture = {
        " | ",
        "-o ",
        " | ",
    };
    PipeGrid grid = PipeGrid::fromRows(picture);

    // Act
    for (int turn = 0; turn < 4; turn++)
        grid.cell(GridPos(0, 0)).rotate();

    // Assert
    // TODO: one EXPECT_EQ, using the picture you started with.
    GTEST_SKIP() << "Not finished: write the Assert, then delete this line";
}

// Does not apply - write the whole test.
// Hint: a cell with no arms. The picture of an empty cell is three rows of three spaces.
TEST(PipeGrid, TurningAnEmptyCellLeavesItEmpty)
{
    // Arrange

    // Act

    // Assert
    GTEST_SKIP() << "Not finished: write Arrange, Act and Assert, then delete this line";
}

// ===== 3. Your tests =====
