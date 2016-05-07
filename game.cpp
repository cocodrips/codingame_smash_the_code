#ifndef SMASH_MAIN
#define SMASH_MAIN

//#define DUMP_INPUT

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <random>
#include <queue>

#define FOR(i, a, b) for(int i=(a);i<(b);++i)
#define REP(i, n)  FOR(i,0,n)
#define RREP(i, n) for (int i=(n)-1;i>=0;i--)
#define ALL(a) (a).begin(),(a).end()

const int DS = 4;
const int dx[DS] = {0, 1, 0, -1};
const int dy[DS] = {1, 0, -1, 0};
const int secondDropPosition[DS] = {1, 0, -1, 0};

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
    int rotate;
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

    static bool next(Field *field, const Drop &drop, int dropRotation, int dropCol, int *outScore) {

        *outScore = 0;
        bool OK = false;

        REP(i, 2) {

            int d = i;
            if (dropRotation == 3) {
                d = (i + 1) % 2;
            }

            int col = dropCol;
            if (i == 1) {
                col += secondDropPosition[dropRotation];
            }
            if (col < 0 || COL <= col) return false;

            RREP (r, ROW - 1) {
                if (field->cell[r][col] < 0) {
                    field->cell[r][col] = drop.color[d];
                    OK = true;
                    break;
                }
            }

            if (!OK) {
                return false;
            }
        }


        for (int i = 1; ; i++) {
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
        bool visited[ROW * COL] = {};
        bool isVanish[ROW * COL] = {};
        int connect[ROW * COL] = {};
        int connectCounter = 0;
        int connectColorCounter = 0;

        REP (row, ROW) {
            REP (col, COL) {
                connectCounter = 0;
                connectColorCounter = 1;


                // 既に消える予定ならそこをスタート地点にしない
                if (visited[row * COL + col]) continue;

                int color = field->cell[row][col];
                if (color < 1) continue;


                queue<int> queue;
                connect[connectCounter++] = row * COL + col;

                bool v[ROW * COL] = {};


                queue.push(row * COL + col);
                v[row * COL + col] = true;


                while (!queue.empty()) {

                    int q = queue.front();
                    queue.pop();
                    int r = q / COL;
                    int c = q % COL;

                    REP (i, DS) {
                        int rr = r + dy[i];
                        int cc = c + dx[i];
                        int linerRC = rr * COL + cc;

                        if (rr < 0 || rr >= ROW || cc < 0 || cc >= COL) continue;
                        if (v[linerRC]) continue;

                        v[linerRC] = true;
                        int _color = field->cell[rr][cc];

                        if (_color == color || _color == 0) {
                            connect[connectCounter] = linerRC;
                            connectCounter++;
                            visited[linerRC] = true;
                            if (_color == color) {
                                queue.push(linerRC);
                                connectColorCounter++;
                            }
                        }

                    }

                }


                if (connectColorCounter >= 4) {
//                    cerr << "counter:" << connectColorCounter << endl;
                    REP (i, connectCounter) {
//                        cerr << "--" << connect[i] << endl;
                        isVanish[connect[i]] = true;
                    }
                }


            }
        }


        int vanishedNum = 0;
        REP (r, ROW) {
            REP(c, COL) {
                if (isVanish[r * COL + c]) {
                    vanishedNum++;
                    field->cell[r][c] = -2;
                }
            }
        }

//        dump
//        cerr << "DUMP" << endl;
//        REP (r, ROW) {
//            REP(c, COL) {
//                cerr << (int)isVanish[r*COL+c];
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
    clock_t start, end;

    // return best score
    int bestNextScore(Field *_field, int bestScore, int prevScore, int turn) {
        if (turn == 3) return prevScore;

        int score;
        REP (col, COL) {
            REP (rot, 4) {
                Field field = *_field;
                FieldController::next(&field, drops[turn], rot, col, &score);
                int _score = bestNextScore(&field, bestScore, prevScore + score, turn + 1);
                if (bestScore < _score) {
                    bestScore = _score;
                }
            }

        }

        return bestScore;
    }

    int getNext(int *rot) {
        int bestScore = 0;
        int bestCol = 0;
        int bestRotation = 0;
        int score = 0;

        REP (col, COL) {
            REP (rotation, 4) {
                Field field = fields[Me];
                bool hasSpace = FieldController::next(&field, drops[0], rotation, col, &score);
                if (!hasSpace) continue;
                int _score = bestNextScore(&field, 0, score, 1);

                if (bestScore < _score || (bestScore == _score && rand() % 2 == 0)) {
                    bestScore = _score;
                    bestCol = col;
                    bestRotation = rotation;
                }
            }
        }
        *rot = bestRotation;
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
        start = clock();
        for (int i = 0; i < 8; i++) {
            int colorA, colorB;
            cin >> colorA >> colorB;
            drops[i].color[0] = colorA;
            drops[i].color[1] = colorA;
#ifdef DUMP_INPUT
            cerr << colorA << " " << colorB << endl;
#endif
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
#ifdef DUMP_INPUT
            FieldController::dump(fields[character], cerr);
#endif
        }
    }

    void output(ostream &cout) {
        int rot = 0;
        int pos = getNext(&rot);
        cout << pos << " " << rot << endl;
        end = clock();
        cerr << "duration = " << (double) (end - start) / 1000 << "ms.\n";
    }


};

#endif //SMASH_MAIN
//
//
//int main() {
//    Game game = Game();
//    int turn = 0;
//    while (turn++ < 2000) {
//        game.input(cin);
//        game.output(cout);
//    }
//}
