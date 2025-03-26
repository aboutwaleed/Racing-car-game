#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <string>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define WIN_WIDTH 70

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

struct Car {
    char body[4][4] = {
        ' ', '0', '0', ' ',
        '0', '0', '0', '0',
        ' ', '0', '0', ' ',
        '0', '0', '0', '0'
    };
    int x;
    int y;
};

struct Coin {
    char symbol = '*';
    int x;
    int y;
    bool flag;
};

struct OpponentCar {
    char body[4][4] = {
        'X', 'X', 'X', 'X',
        ' ', 'X', 'X', ' ',
        'X', 'X', 'X', 'X',
        ' ', 'X', 'X', ' '
    };
    int x;
    int y;
    int speed;
};

Car playerCar;
Coin coins[200];
OpponentCar opponentCars[5];

int score = 0;
string playerName; // Variable to store the player's name

void setCursorPosition(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void DisplayBorder() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        setCursorPosition(0, i);
        cout << "|";
        setCursorPosition(WIN_WIDTH, i);
        cout << "|";
    }

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        setCursorPosition(SCREEN_WIDTH / 2, i);
        cout << "|";
    }
}

void DisplayCar(const Car& car) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            setCursorPosition(car.x + j + 25, car.y + i + 1);
            cout << car.body[i][j];
        }
    }
}

void DisplayCoins() {
    for (int i = 0; i < 200; i++) {
        if (coins[i].flag) {
            setCursorPosition(coins[i].x, coins[i].y);
            cout << coins[i].symbol;
        }
    }
}

void DisplayScore() {
    setCursorPosition(SCREEN_WIDTH - 20, 1);
    cout << "Score: " << score;
}

void GenerateCoins() {
    for (int i = 0; i < 200; i++) {
        coins[i].x = rand() % (SCREEN_WIDTH - 4) + 2;
        coins[i].y = rand() % (SCREEN_HEIGHT - 4) + 2;
        coins[i].flag = false;
    }
}

void UpdateCoins() {
    static int coinIndex = 0; // Track the current coin to be displayed

    if (coinIndex < 200) {
        coins[coinIndex].flag = true; // Set the flag for the current coin
        coinIndex++;
    }

    for (int i = 0; i < 200; i++) {
        if (coins[i].flag) {
            if (coins[i].y >= SCREEN_HEIGHT - 2) {
                coins[i].flag = false;
                score++;
            }
            else {
                setCursorPosition(coins[i].x, coins[i].y);
                cout << " ";
                coins[i].y++;
                setCursorPosition(coins[i].x, coins[i].y);
                cout << coins[i].symbol;
            }
        }
    }
}

void UpdateOpponentCars() {
    for (int i = 0; i < 5; i++) {
        if (opponentCars[i].y >= SCREEN_HEIGHT - 2) {
            opponentCars[i].x = rand() % (SCREEN_WIDTH - 4) + 2;
            opponentCars[i].y = 1;
            opponentCars[i].speed = rand() % 2 + 1; // Randomize the opponent car's speed
        }
        else {
            setCursorPosition(opponentCars[i].x, opponentCars[i].y);
            cout << " ";
            opponentCars[i].y += opponentCars[i].speed; // Move the opponent car down based on its speed
            setCursorPosition(opponentCars[i].x, opponentCars[i].y);
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    cout << opponentCars[i].body[row][col];
                }
                setCursorPosition(opponentCars[i].x, opponentCars[i].y + row + 1);
            }
        }
    }
}

void ChooseTrack(int& track) {
    while (true) {
        system("cls");
        cout << "\n\n\n\t-----------------------------------";
        cout << "\n\n\n\tChoose a Track:";
        cout << "\n\n\n\t1. Track 1";
        cout << "\n\n\n\t2. Track 2";
        cout << "\n\n\n\t3. Track 3";
        cout << "\n\n\n\t4. Track 4";
        cout << "\n\n\n\t-----------------------------------";

        char option = _getche();

        if (option >= '1' && option <= '4') {
            track = option - '0';
            break;
        }
    }
}

void SaveScore(const string& playerName, int score) {
    ofstream file("scores.txt", ios::app);

    if (file.is_open()) {
        file << playerName << " " << score << endl;
        file.close();
    }
}


void DisplayScores() {
    system("cls");
    cout << "High Scores:" << endl;

    ifstream file("scores.txt");

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cout << "No scores available." << endl;
    }

    _getch();
}

void PlayGame(int track) {
    playerCar.x = SCREEN_WIDTH / 2 - 2;
    playerCar.y = SCREEN_HEIGHT - 5;
    GenerateCoins();

    for (int i = 0; i < 5; i++) {
        opponentCars[i].x = rand() % (SCREEN_WIDTH - 4) + 2;
        opponentCars[i].y = rand() % (SCREEN_HEIGHT - 10) + 1;
        opponentCars[i].speed = rand() % 2 + 1;
    }

    while (true) {
        system("cls");
        DisplayBorder();
        DisplayCar(playerCar);
        DisplayCoins();
        DisplayScore();

        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' || ch == 'A') {
                if (playerCar.x > 2) {
                    playerCar.x--;
                }
            }
            else if (ch == 'd' || ch == 'D') {
                if (playerCar.x < SCREEN_WIDTH - 6) {
                    playerCar.x++;
                }
            }
        }

        UpdateCoins();
        UpdateOpponentCars();

        for (int i = 0; i < 5; i++) {
            if (playerCar.x >= opponentCars[i].x && playerCar.x <= opponentCars[i].x + 3 &&
                playerCar.y >= opponentCars[i].y && playerCar.y <= opponentCars[i].y + 3) {
                setCursorPosition(SCREEN_WIDTH / 2 - 6, SCREEN_HEIGHT / 2);
                cout << "Game Over!";
                setCursorPosition(SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 2 + 1);
                cout << "You collided with an opponent car.";
                _getch();
                SaveScore(playerName, score);  // Save score with the player's name

                return;
            }
        }
        if (score >= 200) {
            setCursorPosition(SCREEN_WIDTH / 2 - 4, SCREEN_HEIGHT / 2);
            cout << "Congratulations!";
            setCursorPosition(SCREEN_WIDTH / 2 - 11, SCREEN_HEIGHT / 2 + 1);
            cout << "You collected all the coins!";
            _getch();
            SaveScore(playerName, score);  // Save score with the player's name

            return;
        }
    }
}


void Instructions() {
    system("cls");
    cout << "\n\n\n\t-----------------------------------";
    cout << "\n\n\n\tInstructions:";
    cout << "\n\n\n\t- Use 'A' to move the car left.";
    cout << "\n\n\n\t- Use 'D' to move the car right.";
    cout << "\n\n\n\t- Collect all the coins to win!";
    cout << "\n\n\n\t- Avoid opponent cars!";
    cout << "\n\n\n\t-----------------------------------";
    _getch();
}

int main() {
    srand((unsigned)time(NULL));

    int track = 2;
    string playerName;

    do {
        system("cls");
        cout << "\t---------------------------- " << endl;
        cout << "\t|   Racing Car Game        | " << endl;
        cout << "\t--------------------------- " << endl;

        cout << "\n\n\n\n\tEnter your name: ";
        getline(cin, playerName);

        cout << "\n\n\n\n\t1. Choose Track" << endl;
        cout << "\t2. Instructions" << endl;
        cout << "\t3. Display High Scores" << endl;
        cout << "\t4. Exit" << endl;
        cout << "\n\n\tSelect option: ";

        char option = _getche();

        if (option == '1') {
            ChooseTrack(track);

            while (true) {
                system("cls");
                cout << "\t---------------------------- " << endl;
                cout << "\t|   Racing Car Game        | " << endl;
                cout << "\t--------------------------- " << endl;

                cout << "\n\n\n\n\t1. Play Game" << endl;
                cout << "\t2. Instructions" << endl;
                cout << "\t3. Display High Scores" << endl;
                cout << "\t4. Exit" << endl;
                cout << "\n\n\tSelect option: ";

                option = _getche();

                if (option == '1') {
                    score = 0; // Reset score before starting the game
                    PlayGame(track);
                }
                else if (option == '2') {
                    Instructions();
                }
                else if (option == '3') {
                    DisplayScores();
                }
                else if (option == '4') {
                    break;
                }
            }
        }
        else if (option == '2') {
            Instructions();
        }
        else if (option == '3') {
            DisplayScores();
        }
        else if (option == '4') {
            break;
        }

    } while (true);

    return 0;
}