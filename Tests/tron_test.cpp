// Unit tests for Tron.
//
// Every .cpp in Tests/ is built into the Tests program automatically.
// Only work in the file for YOUR game.
//
// The classes under test are in Games/TronArena.h. The Tests program does not link SFML, so if that
// header pulls in SFML, this file will not compile. That is deliberate: it proves your class is free of SFML.
//
// How this file is laid out:
//   1. Finished tests for what is already in TronArena.h. Read them: they show the pattern.
//   2. Three tests for one rule (a bike moving one step) that get less finished as you go:
//        normal case     - finished
//        edge case       - Arrange and Act are written; you write the Assert
//        does not apply  - only the name; you write all of it
//      The unfinished ones call GTEST_SKIP(), so they show as "skipped" until you finish them.
//      Delete the GTEST_SKIP() line when you write the test.
//   3. Your own tests go at the bottom, named after behaviours, e.g. TEST(TronArena, BikeCrashesIntoATrail).
//
// Pictures: TronArena::fromRows() builds an arena from a picture, and toRows() turns it back into one,
// so a test can set up and check the trails in a way you can read. Use a tiny arena, not 600 x 480:
//   '.' no trail   '#' a trail      (top-left is x = 0, y = 0)

#include <gtest/gtest.h>
#include "TronArena.h"

using namespace tron_game;

using Rows = std::vector<std::string>;

// ===== 1. Finished examples =====

TEST(TronArena, PictureOfAnArenaComesBackUnchanged)
{
    // Arrange
    Rows picture = {
        ".....",
        ".###.",
        "...#.",
    };

    // Act
    TronArena arena = TronArena::fromRows(picture);

    // Assert
    EXPECT_EQ(picture, arena.toRows());
}

TEST(TronArena, MarkedTrailShowsInThePicture)
{
    // Arrange
    TronArena arena = TronArena::fromRows({
        ".....",
        ".....",
        ".....",
    });

    // Act
    arena.markTrail(3, 1);

    // Assert
    Rows expected = {
        ".....",
        "...#.",
        ".....",
    };
    EXPECT_EQ(expected, arena.toRows());
}

// ===== 2. One rule, three tests: a bike moving one step =====

// Normal case - finished.
TEST(Bike, BikeGoingRightMovesOnePixelRight)
{
    // Arrange
    Bike bike;
    bike.x = 5;
    bike.y = 5;
    bike.dir = 2;   // 2 is right

    // Act
    bike.tick(10, 10);

    // Assert
    EXPECT_EQ(6, bike.x);
    EXPECT_EQ(5, bike.y);
}

// Edge case - write the Assert.
TEST(Bike, BikeGoingRightOffTheRightEdgeComesBackOnTheLeft)
{
    // Arrange - in a 10 x 10 arena, x = 9 is the last column.
    Bike bike;
    bike.x = 9;
    bike.y = 5;
    bike.dir = 2;

    // Act
    bike.tick(10, 10);

    // Assert
    // TODO: where should the bike be now? Check x and y.
    GTEST_SKIP() << "Not finished: write the Assert, then delete this line";
}

// Does not apply - write the whole test.
// Hint: like the edge case, but start one pixel further from the edge.
TEST(Bike, BikeOneStepFromTheEdgeDoesNotWrapYet)
{
    // Arrange

    // Act

    // Assert
    GTEST_SKIP() << "Not finished: write Arrange, Act and Assert, then delete this line";
}

// ===== 3. Your tests =====
