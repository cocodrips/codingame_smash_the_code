#ifndef SMASH_MAIN
#define SMASH_MAIN

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define FOR(i, a, b) for(int i=(a);i<(b);++i)
#define REP(i, n)  FOR(i,0,n)
#define RREP(i, n) for (int i=(n)-1;i>=0;i--)
#define ALL(a) (a).begin(),(a).end()

using namespace std;

const int ROW = 12;
const int COL = 6;
const int DROP = 8;
const int MINCONNECT = 4;

enum Character {
    Me,
    Enemy
};

struct Field {
    int cell[ROW][COL];
};

struct Drop {
    int color[2];
};

class FieldController {
public:
    static void dump(const Field &field, ostream &cerr) {
//        filter(field, [](int x) { cerr << (char) (x + '0'); }, []() { cerr << endl; });
        REP(r, ROW) {
            REP (c, COL) {
                cerr << (char) (field.cell[r][c] + '0');
            }
            cerr << endl;
        }

    }

    static void filter(const Field &field, void (*funcCell)(int), void(*funcRow)()) {
        REP(r, ROW) {
            REP (c, COL) {
                funcCell(field.cell[r][c]);
            }
            funcRow();
        }
    }

    static Field next(Field field, const Drop &drop, int dropCol, int *outScore) {
        *outScore = -1;
        bool OK = false;
        REP (r, ROW - 1) {
            if (field.cell[r][dropCol] < 0) {
                field.cell[r][dropCol] = drop.color[0];
                field.cell[r + 1][dropCol] = drop.color[1];
                OK = true;
                break;
            }
        }

        if (!OK) {
            return field;
        }

        return field;
    }

    static void vanish(Field *field, int *score) {
        // 横
        int prev = -1;
        int connect = 1;

        bool isVanish[ROW][COL] = {};

        REP(col, COL) {
            prev = -1;
            REP(row, ROW) {
                int cell = field->cell[row][col];
                if ((cell != prev || row == ROW - 1) && connect >= MINCONNECT) {
                    FOR(r, row - connect, row) {
                        isVanish[r][col] = true;
                    }
                    connect = 0;
                }
            }
        }

    }
};

class Game {
private:
    Field fields[2];
    Drop drops[DROP];

public:
    int getNext() {
        int bestScore = 0;
        int bestCol = 0;
        int score = 0;

        REP (col, COL) {
            Field nextField = FieldController::next(fields[Me], drops[0], col, &score);
            score = getEvaluation(nextField, 1);
            if (bestScore < score) {
                bestScore = score;
                bestCol = col;
            }
        }

        return bestCol;
    }

    int getEvaluation(Field currentField, int turn) {
        int score;
        if (turn == DROP) {
            return score;
        }

        REP(col, COL) {
            Field nextField = FieldController::next(fields[Me], drops[turn], col, &score);
            score += getEvaluation(nextField, turn + 1);
        }

        return score;
    }

    void setField(int character, int row, int col, int value) {
        fields[character].cell[row][col] = value;
    }

    Field getField(int character) {
        return fields[character];
    }

    Drop getDrop(int i) {
        return drops[i];
    }

    void input(istream& cin) {
        for (int i = 0; i < 8; i++) {
            int colorA, colorB;
            cin >> colorA >> colorB;
            drops[i].color[0] = colorA;
            drops[i].color[1] = colorA;
//            cout << colorA << colorB << endl;
            cin.ignore();
        }
        REP (character, 2) {
            string row;
            REP (r, ROW) {
                cin >> row;
                REP (c, COL) {
                    fields[character].cell[r][c] = row[c] - '0';
                }
            }
//            FieldController::dump(fields[character], cerr);
        }
    }

    void output() {
//        cout << getNext() << endl;
        cout << 1 << endl;
    }


};

#endif //SMASH_MAIN

