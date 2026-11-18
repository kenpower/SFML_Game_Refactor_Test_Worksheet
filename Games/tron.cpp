#include <SFML/Graphics.hpp>
#include <time.h>
#include "TronArena.h"
using namespace sf;
using namespace tron_game;

// ===== GUIDE: Tron =====
// Entry point is int tron(). State that survives between frames: the globals W, H, speed and arena (the trail map),
// plus the locals p1, p2 (and their colours), t (a RenderTexture holding the picture of the trails) and Game, declared before the loop.
// HALF-SPLIT: the trail map and the Bike (moving one step and wrapping round) have already moved into
// TronArena.h, with tests in Tests/tron_test.cpp. Every other rule is still here.
// Suggested reading order: TronArena.h, then newBike(), then SETUP in tron(), then the loop: INPUT, UPDATE, DRAW.
// Refactorings to look for (week 1 needs 2+ seam types and 2+ other types):
//   SEAM inject      - newBike() calls rand() for the starting x, y and dir
//   SEAM de-global   - newBike() and the loop in tron() read the globals W, H, speed and arena
//   SEAM input-data  - Keyboard::isKeyPressed is called inside the steering code for both players
//   rename           - p1, p2, t, c, Game
//   constant         - speed 4, the radius 3, the dir values 0-3, the key lists
//   enum             - dir as 0, 1, 2, 3
//   dedupe           - the two blocks of key checks for p1 and p2
//   struct           - the four keys that belong to one player
// =====
// ---- GLOBAL STATE: arena size, speed, trail map ----
const int W=600;
const int H=480;
int speed = 4;
static TronArena arena(W, H);

// ---- SETUP helper: a bike at a random place, going a random way ----
Bike newBike()
{
    Bike bike;
    bike.x=rand() % W;
    bike.y=rand() % H;
    bike.dir=rand() % 4;
    return bike;
}

int tron()
{
    // ---- SETUP: window, background, players, off-screen texture ----
    srand(time(0));

    RenderWindow window(VideoMode({W, H}), "The Tron Game!");
    window.setFramerateLimit(60);

    Texture texture("images/tron/background.jpg");
    Sprite sBackground(texture);

    Bike p1 = newBike(), p2 = newBike();
    Color p1Color = Color::Red, p2Color = Color::Green;

    RenderTexture t({W, H});
    t.setSmooth(true);
    Sprite sprite(t.getTexture());
    t.clear();  t.draw(sBackground);

    bool Game=1;

    while (window.isOpen())
    {
        // ---- INPUT: window events ----
        while (const std::optional e = window.pollEvent())
        {
            if (e->is<Event::Closed>())
                window.close();
        }

        // ---- INPUT: steering keys for both players ----
        // HINT: the two blocks of key checks are near-duplicates - what differs? A small struct of keys per player might help.
        if (Keyboard::isKeyPressed(Keyboard::Key::Left)) if (p1.dir!=2) p1.dir=1;
        if (Keyboard::isKeyPressed(Keyboard::Key::Right)) if (p1.dir!=1)  p1.dir=2;
        if (Keyboard::isKeyPressed(Keyboard::Key::Up)) if (p1.dir!=0) p1.dir=3;
        if (Keyboard::isKeyPressed(Keyboard::Key::Down)) if (p1.dir!=3) p1.dir=0;

        if (Keyboard::isKeyPressed(Keyboard::Key::A)) if (p2.dir!=2) p2.dir=1;
        if (Keyboard::isKeyPressed(Keyboard::Key::D)) if (p2.dir!=1)  p2.dir=2;
        if (Keyboard::isKeyPressed(Keyboard::Key::W)) if (p2.dir!=0) p2.dir=3;
        if (Keyboard::isKeyPressed(Keyboard::Key::S)) if (p2.dir!=3) p2.dir=0;

        // ---- UPDATE: move both players and check the trails ----
        if (!Game)    continue;

        // HINT: why loop 'speed' times per frame? Put a breakpoint inside and watch p1.x, p1.y and Game.
        for(int i=0;i<speed;i++)
        {
            p1.tick(W,H); p2.tick(W,H);
            // HINT: arena is the real memory of the trails, the RenderTexture is only the picture. These rules could join TronArena.
            if (arena.isTrail(p1.x,p1.y)) Game=0;
            if (arena.isTrail(p2.x,p2.y)) Game=0;
            arena.markTrail(p1.x,p1.y);
            arena.markTrail(p2.x,p2.y);
    
            // HINT: the drawing below only needs to know where each bike is after each step.
            CircleShape c(3);
            c.setPosition({float(p1.x),float(p1.y)}); c.setFillColor(p1Color);    t.draw(c);
            c.setPosition({float(p2.x),float(p2.y)}); c.setFillColor(p2Color);    t.draw(c);
            t.display();    
        }

       ////// draw  ///////
        // ---- DRAW ----
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
