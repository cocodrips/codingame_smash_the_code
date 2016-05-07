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

    static bool next(Field *field, const Drop &drop, int dropCol, int *outScore) {

        *outScore = 0;
        bool OK = false;
        RREP (r, ROW - 1) {
            if (field->cell[r][dropCol] < 0) {
                field->cell[r][dropCol] = drop.color[0];
                field->cell[r - 1][dropCol] = drop.color[1];
                OK = true;
                break;
            }
        }

        if (!OK) {
            return false;
        }

        for(int i = 1;;i++) {
//            FieldController::dump(*field, cerr);
            int score = vanish(field);
            if (score == 0) {
                break;
            }
            *outScore += i * 10 * score;
        }
        return true;
    }

    static int vanish(Field *field) {
        // 消えたぷよがあればtrueを返す
        int vanishedNum = 0;

        int prev = -1;
        int counter = 1;
        bool isVanish[ROW][COL] = {};

        // たて
        REP(col, COL) {
            prev = -1;
            counter = 0;
            REP(row, ROW) {
                int cell = field->cell[row][col];
                connect(cell, prev, row * COL + col, -COL,
                        row == ROW - 1, &counter, (bool *) isVanish);
                prev = cell;
            }
        }

        // 横
        REP(row, ROW) {
            prev = -1;
            counter = 0;
            REP(col, COL) {
                int cell = field->cell[row][col];
                connect(cell, prev, row * COL + col, -1,
                        col == COL - 1, &counter, (bool *) isVanish);
                prev = cell;
            }
        }

        // 右上に斜め
        REP(sum, ROW + COL) {
            prev = -1;
            counter = 0;
            REP(col, COL) {
                int row = sum - col;
                if (col < 0 || COL <= col || row < 0 || ROW <= row) continue;
                int cell = field->cell[row][col];
                connect(cell, prev, row * COL + col, COL - 1,
                        col == COL - 1, &counter, (bool *) isVanish);
                prev = cell;

            }
        }

        REP(sum, ROW + COL) {
            prev = -1;
            counter = 0;
            RREP(col, COL) {
                int row = sum - (COL - col + 1);
                if (col < 0 || COL <= col || row < 0 || ROW <= row) continue;
                int cell = field->cell[row][col];
                connect(cell, prev, row * COL + col, COL + 1,
                        col == COL - 1, &counter, (bool *) isVanish);
                prev = cell;

            }
        }


        REP (r, ROW) {
            REP(c, COL) {
                if (isVanish[r][c]) {
                    vanishedNum++;
                    field->cell[r][c] = -2;
                }
            }
        }

//        dump
//        REP (r, ROW) {
//            REP(c, COL) {
//                cerr << (int)isVanish[r][c];
//            }
//            cerr << endl;
//        }

        // 落とす
        REP (col, COL) {
            int bottomRow = ROW - 1;
            RREP(row, ROW) {
                int cell = field->cell[row][col];
                if (cell >= 0) {
                    field->cell[bottomRow--][col] = cell;
                }
            }
            while (bottomRow >= 0) {
                field->cell[bottomRow][col] = -2;
                bottomRow--;
            }
        }
        return vanishedNum;
    }


    static void connect(int cell, int prev, int cell_index, int prev_cell,
                        bool isLast, int *counter, bool *isVanish) {
        if (cell == prev) {
            (*counter)++;
        }

        if (cell != prev || isLast) {
            int adjust = isLast ? -1 : 0;
            if (*counter >= MINCONNECT && prev > 0) {
                REP(i, *counter) {
                    isVanish[cell_index + prev_cell * (i + 1 + adjust)] = true;
                }
            }
            *counter = 1;
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
            Field field = fields[Me];
            if (FieldController::next(&field, drops[0], col, &score)) {
                if (bestScore < score) {
                    bestScore = score;
                    bestCol = col;
                }
            }
        }

        return bestCol;
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

    void input(istream &cin) {
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

