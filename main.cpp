#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#define FOR(i, a, b) for(int i=(a);i<(b);++i)
#define REP(i, n)  FOR(i,0,n)
#define RREP(i, n) for (int i=(n)-1;i>=0;i--)
#define ALL(a) (a).begin(),(a).end()

const int ROW = 12;
const int COL = 6;

enum Character {
    Me,
    Enemy
};



struct Field {
    int cell[ROW][COL];
};

class FieldController {
public:
    static void dump(const Field &field) {
        filter(field, [](int x){cerr << (char)(x + '0'); }, [](){cerr << endl; });
    }

    static void filter(const Field &field, void (*funcCell)(int), void(*funcRow)()) {
        REP(r, ROW) {
            REP (c, COL) {
                funcCell(field.cell[r][c]);
            }
            funcRow();
        }
    }

    static Field next(Field field, int color, int dropCol, int* outScore) {
        *outScore = -1;
        bool OK = false;
        REP (r, ROW - 1) {
            if (field.cell[r][dropCol] < 0) {
                field.cell[r][dropCol] = color;
                field.cell[r + 1][dropCol] = color;
                OK = true;
                break;
            }
        }

        if (!OK) {
            return field;
        }

        return field;

    }
};

Field fields[2] = {};

int getNext() {
    return 0;
}

int main() {

    int turn = 0;
    while (turn++ < 1) {
        for (int i = 0; i < 8; i++) {
            int colorA;
            int colorB;
            cin >> colorA >> colorB;
            cerr << colorA << " " << colorB << endl;
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
            FieldController::dump(fields[character]);
        }

        cout << getNext() << endl;
    }
}