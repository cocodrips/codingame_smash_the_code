#include <fstream>
#include "gtest/gtest.h"
#include "../game.cpp"

TEST(IOTest, input) {
    Game game = Game();
    ifstream f("input/simple_chain.txt");
    game.input(f);
    stringstream ss;
    FieldController::dump(game.getField(Me), ss);
    EXPECT_EQ(ss.str(), ""
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "6.....\n"
            "6.....\n"
            "6.....\n"
            "6.....\n"
            "5.....\n"
            "5.....\n"
    );
}
