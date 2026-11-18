#include <SFML/Graphics.hpp>
#include <time.h>
#include "TetrisBoard.h"
using namespace sf;
using namespace tetris_game;

// ===== GUIDE: Tetris =====
// Entry point: int tetris(). Almost everything happens inside its while (window.isOpen()) loop.
// HALF-SPLIT: the board of landed blocks and the "does this piece fit?" check have already moved into
// TetrisBoard (TetrisBoard.h), with tests in Tests/tetris_test.cpp. Every other rule is still here.
// State that survives between frames: globals board, a, b (below), plus dx, rotate, colorNum, timer, delay (declared before the loop).
// Suggested reading order: TetrisBoard.h -> figures -> setup in tetris() -> the loop, top to bottom.
// Refactorings to look for (week 1 needs 2+ seam types and 2+ other types):
//   SEAM inject         - rand() picks colorNum and the shape index n in the gravity tick; the Clock is read in the loop
//                         (timer accumulates time), so the elapsed time could be passed in instead
//   SEAM de-global      - the loop works on the globals board, a and b
//   SEAM input-data     - Keyboard::isKeyPressed(Down) is read in the middle of the update (it sets delay)
//   rename              - a, b, k, n, s, t1, t2, t3, dx
//   constant            - 18, the (28, 31) offset, the 0.3 and 0.05 delays, the 7 shapes
//   extract-function    - move, rotate, tick and clear-lines sections of the loop (then move them into TetrisBoard)
//   std-array           - figures is a plain C array
// =====
// ---- SHARED STATE: board = landed blocks, a = falling piece, b = its previous position ----
static TetrisBoard board;

static Piece a, b;

// ---- DATA: the seven piece shapes ----
// HINT: each number is a cell index in a 2-column grid - look at how the numbers are decoded when a new piece spawns.
int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};



int tetris()
{
    // ---- SETUP: window, textures, sprites ----
    srand(time(0));     

    RenderWindow window(VideoMode({320, 480}), "The Game!");

    Texture t1("images/tetris/tiles.png");
    Texture t2("images/tetris/background.png");
    Texture t3("images/tetris/frame.png");

    Sprite s(t1), background(t2), frame(t3);

    // ---- GAME STATE: lives across frames, changed by input and the timer ----
    int dx=0; bool rotate=0; int colorNum=1;
    float timer=0,delay=0.3; 

    Clock clock;

    while (window.isOpen())
    {
        // ---- TIMING: how long since the last frame ----
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        // ---- INPUT: events set the dx / rotate flags ----
        while (const std::optional e = window.pollEvent())
        {
            if (e->is<Event::Closed>())
                window.close();

            if (const auto* key = e->getIf<Event::KeyPressed>())
              if (key->code==Keyboard::Key::Up) rotate=true;
              else if (key->code==Keyboard::Key::Left) dx=-1;
              else if (key->code==Keyboard::Key::Right) dx=1;
        }

    if (Keyboard::isKeyPressed(Keyboard::Key::Down)) delay=0.05;

    // ---- UPDATE: move sideways ----
    //// <- Move -> ///
    for (int i=0;i<4;i++)  { b[i]=a[i]; a[i].x+=dx; }
    if (!board.isLegal(a)) for (int i=0;i<4;i++) a[i]=b[i];

    // ---- UPDATE: rotate ----
    //////Rotate//////
    if (rotate)
      {
        // HINT: this rotation uses a[1] as a pivot - try it on paper with a T piece.
        geom::Point p = a[1]; //center of rotation
        for (int i=0;i<4;i++)
          {
            int x = a[i].y-p.y;
            int y = a[i].x-p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
           }
           if (!board.isLegal(a)) for (int i=0;i<4;i++) a[i]=b[i];
      }

    // ---- UPDATE: gravity tick - drop, lock in place, spawn next piece ----
    ///////Tick//////
    if (timer>delay)
      {
        for (int i=0;i<4;i++) { b[i]=a[i]; a[i].y+=1; }

        if (!board.isLegal(a))
        {
         for (int i=0;i<4;i++) board.setCell(b[i].y, b[i].x, colorNum);

         colorNum=1+rand()%7;
         int n=rand()%7;
         for (int i=0;i<4;i++)
           {
            a[i].x = figures[n][i] % 2;
            a[i].y = figures[n][i] / 2;
           }
        }

         timer=0;
      }

    // ---- UPDATE: clear completed lines ----
    ///////check lines//////////
    // HINT: put a breakpoint here and watch k and i - why do they sometimes differ?
    int k=board.rows()-1;
    for (int i=board.rows()-1;i>0;i--)
    {
        int count=0;
        for (int j=0;j<board.cols();j++)
        {
            if (board.cellAt(i,j)) count++;
            board.setCell(k,j,board.cellAt(i,j));
        }
        if (count<board.cols()) k--;
    }

    // ---- RESET: per-frame inputs go back to defaults ----
    dx=0; rotate=0; delay=0.3;

    // ---- DRAW ----
    /////////draw//////////
    window.clear(Color::White);    
    window.draw(background);
          
    for (int i=0;i<board.rows();i++)
     for (int j=0;j<board.cols();j++)
       {
         if (board.cellAt(i,j)==0) continue;
         // HINT: 18 and the offset (28, 31) are pixel sizes - good candidates for named constants.
         s.setTextureRect(IntRect({board.cellAt(i,j)*18,0},{18,18}));
         s.setPosition({float(j*18),float(i*18)});
         s.move({28,31}); //offset
         window.draw(s);
       }

    for (int i=0;i<4;i++)
      {
        s.setTextureRect(IntRect({colorNum*18,0},{18,18}));
        s.setPosition({float(a[i].x*18),float(a[i].y*18)});
        s.move({28,31}); //offset
        window.draw(s);
      }

    window.draw(frame);
    window.display();
    }

    return 0;
}
