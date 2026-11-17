// Libraries are linked via CMake (see CMakeLists.txt).
#include<iostream>
#include<exception>

int tetris();
int minesweeper();
int xonix();
int netwalk();
int tron();


using namespace std;
int main()
{
    char key;
    while (true) {
        cout << "Choose a game. Enter the uppercase key in game name :\n";
        cout << "============================================\n";
        cout << "Tetris \n";
        cout << "Minesweeper\n";
        cout << "Xonix\n";
        cout << "Netwalk (Pipes game)\n";
        cout << "tron U\n";



        cin >> key;

        try {
        switch (key) {
            case 'T':
            case 't':
                tetris();
                break;
            case 'M':
            case 'm':
                minesweeper();
                break;
            case 'X':
            case 'x':
                xonix();
                break;
            case 'n':
            case 'N':
                netwalk();
                break;
            case 'u':
            case 'U':
                tron();
                break;
        }
        } catch (const std::exception& ex) {
            // SFML 3 throws when a resource (e.g. an image) cannot be loaded or a window cannot be created.
            cerr << "Error: " << ex.what() << endl;
        }

    }

}
