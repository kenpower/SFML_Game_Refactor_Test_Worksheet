// Unit tests for Tetris.
//
// Every .cpp in Tests/ is built into the Tests program automatically.
// Only work in the file for YOUR game.
//
// The class under test is in Games/TetrisBoard.h. The Tests program does not link SFML, so if that
// header pulls in SFML, this file will not compile. That is deliberate: it proves your class is free of SFML.
//
// How this file is laid out:
//   1. Finished tests for what is already in TetrisBoard. Read them: they show the pattern.
//   2. Three tests for one rule (does a piece fit?) that get less finished as you go:
//        normal case     - finished
//        edge case       - Arrange and Act are written; you write the Assert
//        does not apply  - only the name; you write all of it
//      The unfinished ones call GTEST_SKIP(), so they show as "skipped" until you finish them.
//      Delete the GTEST_SKIP() line when you write the test.
//   3. Your own tests go at the bottom, named after behaviours, e.g. TEST(TetrisBoard, FullRowIsCleared).
//
// Pictures: TetrisBoard::fromRows() builds a board from a picture, toRows() turns the board back into one,
// and pieceFromRows() reads a piece out of the same picture. So a test can set up and check a board
// in a way you can read:
//   '.' empty   '1'-'7' a landed block of that colour   '@' a cell of the piece

#include <gtest/gtest.h>
#include "TetrisBoard.h"

using namespace tetris_game;

using Rows = std::vector<std::string>;

// ===== 1. Finished examples =====

TEST(TetrisBoard, PictureOfABoardComesBackUnchanged)
{
    // Arrange
    Rows picture = {
        "....",
        "..3.",
        "1..7",
    };

    // Act
    TetrisBoard board = TetrisBoard::fromRows(picture);

    // Assert
    EXPECT_EQ(picture, board.toRows());
}

TEST(TetrisBoard, PiecePastTheRightWallIsNotLegal)
{
    // Arrange - a piece that is partly off the board can't be drawn, so build it from points.
    TetrisBoard board = TetrisBoard::fromRows({
        "....",
        "....",
        "....",
    });
    Piece piece = { geom::Point{3, 0}, geom::Point{4, 0}, geom::Point{3, 1}, geom::Point{4, 1} };

    // Act
    bool legal = board.isLegal(piece);

    // Assert
    EXPECT_FALSE(legal);
}

// ===== 2. One rule, three tests: does a piece fit? =====

// Normal case - finished.
TEST(TetrisBoard, PieceOnTopOfALandedBlockIsNotLegal)
{
    // Arrange
    Rows picture = {
        "....",
        ".@@.",
        ".@@.",
    };
    TetrisBoard board = TetrisBoard::fromRows({
        "....",
        "....",
        "..5.",
    });
    Piece piece = pieceFromRows(picture);

    // Act
    bool legal = board.isLegal(piece);

    // Assert
    EXPECT_FALSE(legal);
}

// Edge case - write the Assert.
TEST(TetrisBoard, PieceOnTheBottomRowIsLegal)
{
    // Arrange - the board and the piece can share one picture: the board ignores the '@'s.
    Rows picture = {
        "....",
        "@...",
        "@@@.",
    };
    TetrisBoard board = TetrisBoard::fromRows(picture);
    Piece piece = pieceFromRows(picture);

    // Act
    bool legal = board.isLegal(piece);

    // Assert
    // TODO: one EXPECT_ line.
    GTEST_SKIP() << "Not finished: write the Assert, then delete this line";
}

// Does not apply - write the whole test.
// Hint: like the normal case, but the landed block is next to the piece, not under it.
TEST(TetrisBoard, PieceNextToALandedBlockIsLegal)
{
    // Arrange

    // Act

    // Assert
    GTEST_SKIP() << "Not finished: write Arrange, Act and Assert, then delete this line";
}

// ===== 3. Your tests =====
