#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include <string>
#include "PipeGrid.h"
using namespace sf;
using namespace netwalk_game;
// ===== GUIDE: Netwalk =====
// Entry point: int netwalk(), at the bottom. The board is the global `grid` (N x N pipes) and the helpers above work on it.
// HALF-SPLIT: GridPos, the Pipe (rotate, isConnect) and the grid itself have already moved into PipeGrid.h,
// with tests in Tests/netwalk_test.cpp. Every other rule is still here.
// State that survives between frames: grid, plus servPos and the sprites declared before the loop in netwalk().
// Suggested reading order: PipeGrid.h, then generatePuzzle, then drop, then netwalk() from SETUP downwards.
// Refactorings to look for (week 1 needs 2+ seam types and 2+ other types):
//   SEAM de-global   - generatePuzzle(), drop(), cell() and isOut() all work on the global grid instead of being given it
//   SEAM inject      - rand() in generatePuzzle(), in the SETUP shuffle, and when the server is placed
//   SEAM input-data  - the mouse position is turned into a grid cell inside the click handler
//   rename           - tsz, oset, t1..t4, kind, s, n, p
//   constant         - 54, the 65 / 55 offset, 27, 36, 53, 5
//   enum             - kind (0 to 3) picks the pipe image
//   extract-function - finding a pipe's orientation, or resetting the `on` flags
// =====

const int N = 6;
int tsz = 54; //tile size
Vector2f oset(65,55);

// ---- DATA: the board, with small helpers to access it ----
static PipeGrid grid(N);
Pipe& cell(GridPos v) {return grid.cell(v);}
bool isOut(GridPos v) {return grid.isOut(v);}


// ---- SETUP helper: randomly grows a connected network of pipes ----
void generatePuzzle()
{
  std::vector<GridPos> nodes;
  nodes.push_back(GridPos(rand()%N,rand()%N));

  while(!nodes.empty())
  {
    int n = rand()%nodes.size();
    GridPos v = nodes[n];
    GridPos d = DIR[rand()%4];

    if (cell(v).dirs.size()==3) {nodes.erase(nodes.begin() + n); continue;}
    if (cell(v).dirs.size()==2) if (rand()%50) continue;

    bool complete=1;
    for(auto D:DIR)
     if (!isOut(v+D) && cell(v+D).dirs.empty()) complete=0;
    if (complete) {nodes.erase(nodes.begin() + n); continue; }

    if (isOut(v+d)) continue;
    if (!cell(v+d).dirs.empty()) continue;
    cell(v).dirs.push_back(d);
    cell(v+d).dirs.push_back(-d);
    nodes.push_back(v+d);
  }
}


// ---- UPDATE helper: marks every cell reachable from the server ----
void drop(GridPos v)
{
   if (cell(v).on) return;
   cell(v).on=true;

   // HINT: drop() calls itself - trace it on paper for a tiny grid. When does the recursion stop?
   for(auto d:DIR)
    if (!isOut(v+d))
     if (cell(v).isConnect(d) && cell(v+d).isConnect(-d))
       drop(v+d);
}


int netwalk()
{
    // ---- SETUP: window, images and sprites ----
    srand(time(0));

    RenderWindow app(VideoMode({390, 390}), "Netwalk The Pipe Puzzle!");

    Texture t1("images/netwalk/background.png");
    Texture t2("images/netwalk/comp.png");
    Texture t3("images/netwalk/server.png");
    Texture t4("images/netwalk/pipes.png");
    t4.setSmooth(true);

    Sprite sBackground(t1), sComp(t2), sServer(t3), sPipe(t4);
    sPipe.setOrigin({27,27});
    sComp.setOrigin({18,18});
    sServer.setOrigin({20,20});


    // ---- SETUP: build the puzzle, work out each tile's orientation, then shuffle ----
    generatePuzzle();

    for(int i=0;i<N;i++)
     for(int j=0;j<N;j++)
       {
         Pipe &p = cell(GridPos(j,i));

         for(int n=4;n>0;n--) //find orientation//
         {
          std::string s="";
          for(auto d: DIR) s+=p.isConnect(d)? '1':'0';
          if (s=="0011" || s=="0111" || s=="0101" || s=="0010") p.orientation=n;
          p.rotate();
         }

         for(int n=0;n<rand()%4;n++) //shuffle//
          {p.orientation++; p.rotate();}
       }

    // ---- SETUP: place the server ----
    GridPos servPos;
    while(cell(servPos).dirs.size()==1) {servPos = GridPos(rand()%N, rand()%N);}
    sServer.setPosition({float(servPos.x*tsz), float(servPos.y*tsz)});
    sServer.move(oset);

    while (app.isOpen())
    {
        // ---- INPUT: close the window, or click to rotate a pipe and re-check connections ----
        while (const std::optional e = app.pollEvent())
        {
            if (e->is<Event::Closed>())
                app.close();

            if (const auto* mb = e->getIf<Event::MouseButtonPressed>())
                if (mb->button == Mouse::Button::Left)
                  {
                    // HINT: step through with the debugger - how do mouse pixels become a grid cell? Watch pos before and after the divide.
                    Vector2i pos = Mouse::getPosition(app) + Vector2i(tsz/2,tsz/2) - Vector2i(oset);
                    pos/=tsz;
                    GridPos clicked(pos.x,pos.y);
                    if (isOut(clicked)) continue;
                    cell(clicked).orientation++;
                    cell(clicked).rotate();

                    for(int i=0;i<N;i++)
                     for(int j=0;j<N;j++)
                      cell(GridPos(j,i)).on=0;

                    drop(servPos);
                  }
        }

        // ---- DRAW: background, every pipe (animating its rotation), then the server ----
        app.clear();
        app.draw(sBackground);

        for(int i=0;i<N;i++)
         for(int j=0;j<N;j++)
           {
            Pipe &p = cell(GridPos(j,i));

            // HINT: kind picks which image to use from pipes.png - what do 0, 1, 2 and 3 correspond to?
            int kind = p.dirs.size();
            if (kind==2 && p.dirs[0]==-p.dirs[1]) kind=0;

            p.angle+=5;
            if (p.angle>p.orientation*90) p.angle=p.orientation*90;

            sPipe.setTextureRect(IntRect({tsz*kind,0},{tsz,tsz}));
            sPipe.setRotation(degrees(p.angle));
            sPipe.setPosition({float(j*tsz),float(i*tsz)});sPipe.move(oset);
            app.draw(sPipe);

            if (kind==1)
               // HINT: these numbers are texture coordinates and sizes - good candidates for named constants.
               { if (p.on) sComp.setTextureRect(IntRect({53,0},{36,36}));
                 else sComp.setTextureRect(IntRect({0,0},{36,36}));
                 sComp.setPosition({float(j*tsz),float(i*tsz)});sComp.move(oset);
                 app.draw(sComp);
               }
           }

        app.draw(sServer);
        app.display();
    }

    return 0;
}

