// Unit tests for Minesweeper.
//
// Every .cpp in Tests/ is built into the Tests program automatically.
// Only work in the file for YOUR game.
//
// The class under test is in Games/MineField.h. The Tests program does not link SFML, so if that
// header pulls in SFML, this file will not compile. That is deliberate: it proves your class is free of SFML.
//
// How this file is laid out:
//   1. Finished tests for what is already in MineField. Read them: they show the pattern.
//   2. Three tests for one rule (counting neighbouring mines) that get less finished as you go:
//        normal case     - finished
//        edge case       - Arrange and Act are written; you write the Assert
//        does not apply  - only the name; you write all of it
//      The unfinished ones call GTEST_SKIP(), so they show as "skipped" until you finish them.
//      Delete the GTEST_SKIP() line when you write the test.
//   3. Your own tests go at the bottom, named after behaviours, e.g. TEST(MineField, ClickingAMineRevealsIt).
//
// Pictures: MineField::fromRows() builds a field from a picture, and toRows() turns it back into one,
// so a test can set up and check a field in a way you can read:
//   '*' a mine   '.' no mine (count 0)   '1'-'8' no mine, with that count
// The first character of the first row is the cell x = 1, y = 1.

#include <gtest/gtest.h>
#include "MineField.h"

using namespace minesweeper_game;

using Rows = std::vector<std::string>;

// ===== 1. Finished examples =====

TEST(MineField, PictureOfAFieldComesBackUnchanged)
{
    // Arrange
    Rows picture = {
        "*..",
        ".2.",
        "..*",
    };

    // Act
    MineField field = MineField::fromRows(picture);

    // Assert
    EXPECT_EQ(picture, field.toRows());
}

TEST(MineField, PictureCountsCellsFromOne)
{
    // Arrange
    MineField field = MineField::fromRows({
        "...",
        "..*",
    });

    // Act
    bool mineAtThirdColumnSecondRow = field.isMine(3, 2);

    // Assert
    EXPECT_TRUE(mineAtThirdColumnSecondRow);
    EXPECT_FALSE(field.isMine(2, 1));
}

// ===== 2. One rule, three tests: counting neighbouring mines =====

// Normal case - finished.
TEST(MineField, EachCellCountsTheMinesAroundIt)
{
    // Arrange
    MineField field = MineField::fromRows({
        "*..",
        "...",
        "..*",
    });

    // Act
    field.countNeighbours();

    // Assert
    Rows expected = {
        "*1.",
        "121",
        ".1*",
    };
    EXPECT_EQ(expected, field.toRows());
}

// Edge case - write the Assert.
TEST(MineField, CornerCellCountsOnlyItsThreeNeighbours)
{
    // Arrange - the top-left cell has only 3 neighbours, and all of them are mines.
    MineField field = MineField::fromRows({
        ".*",
        "**",
    });

    // Act
    field.countNeighbours();

    // Assert
    // TODO: check the top-left cell, either with field.valueAt(1, 1) or with a picture and toRows().
    GTEST_SKIP() << "Not finished: write the Assert, then delete this line";
}

// Does not apply - write the whole test.
// Hint: a mine is not counted. What should a mine cell still hold after countNeighbours()?
TEST(MineField, MineCellKeepsItsMineAfterCounting)
{
    // Arrange

    // Act

    // Assert
    GTEST_SKIP() << "Not finished: write Arrange, Act and Assert, then delete this line";
}

// ===== 3. Your tests =====
