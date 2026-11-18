#include <SFML/Graphics.hpp>
#include <time.h>
#include "MineField.h"
using namespace sf;
using namespace minesweeper_game;

// ===== GUIDE: Minesweeper =====
// Entry point: int minesweeper(). There are no globals; the game is the while (app.isOpen()) loop.
// HALF-SPLIT: where the mines are, and counting the mines round each cell, have already moved into
// MineField (MineField.h), with tests in Tests/minesweeper_test.cpp. Every other rule is still here.
// State that survives between frames: w, mines and sgrid, declared before the loop.
// Suggested reading order: MineField.h -> sgrid -> the setup loop -> mouse input -> the draw loop.
// Refactorings to look for (week 1 needs 2+ seam types and 2+ other types):
//   SEAM inject         - rand()%5 decides where the mines go in the "place the mines" setup loop
//   SEAM input-data     - Mouse::getPosition is read every frame and turned into the cell x, y; the click handling changes
//                         sgrid inside the event loop - it could become plain "reveal cell" / "flag cell" data or calls
//   extract-function    - place mines, the click handling, and the rule in the draw loop that changes sgrid
//                         (then move them into MineField)
//   rename              - w, s, t, sgrid, x, y
//   constant            - 9 (MINE already exists in MineField.h), 10 (hidden), 11 (flag), 32 (tile size), 12 and 10 (grid size)
//   enum                - the cell codes 0-11 stored in sgrid
//   std-array           - sgrid is a plain C array
// =====
int minesweeper()
{
    // ---- SETUP: window and textures ----
    srand(time(0));

    RenderWindow app(VideoMode({400, 400}), "Minesweeper!");

    // ---- GAME STATE: mines = the hidden answer, sgrid = what the player sees ----
    // HINT: the rest of the rules (reveal, flag, hitting a mine) need no SFML either - they could join MineField.
    int w=32;
    MineField mines;
    // HINT: what do the values 0-11 in sgrid mean? Look at the tile image in images/minesweeper.
    int sgrid[12][12] = {}; //for showing

    Texture t("images/minesweeper/tiles.jpg");
    Sprite s(t);

    // ---- SETUP: place the mines ----
    for (int i=1;i<=10;i++)
     for (int j=1;j<=10;j++)
      {
        sgrid[i][j]=10;
        if (rand()%5==0)  mines.setMine(i,j);
      }

    // ---- SETUP: count neighbouring mines for each cell (already in MineField) ----
    mines.countNeighbours();

    while (app.isOpen())
    {
        // ---- INPUT: turn the mouse position into a cell ----
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x/w;
        int y = pos.y/w;

        // ---- INPUT: clicks change sgrid ----
        while (const std::optional e = app.pollEvent())
        {
            if (e->is<Event::Closed>())
                app.close();

            if (const auto* mb = e->getIf<Event::MouseButtonPressed>())
              if (mb->button == Mouse::Button::Left) sgrid[x][y]=mines.valueAt(x,y);
              else if (mb->button == Mouse::Button::Right) sgrid[x][y]=11;
        }

        // ---- DRAW: one sprite per cell ----
        app.clear(Color::White);

        for (int i=1;i<=10;i++)
         for (int j=1;j<=10;j++)
          {
           // HINT: step through this with the debugger after a click - which cell does this test use, and where did x and y come from?
           if (sgrid[x][y]==9) sgrid[i][j]=mines.valueAt(i,j);
           s.setTextureRect(IntRect({sgrid[i][j]*w,0},{w,w}));
           s.setPosition({float(i*w), float(j*w)});
           app.draw(s);
          }

        app.display();
    }

    return 0;
}
