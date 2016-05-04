#include "gtest/gtest.h"
#include "../game.cpp"


TEST(VanishTest, all_direction) {
    Game game = Game();
    ifstream f("input/all_direction_chain.txt");
    game.input(f);


    int score;
    Field field = game.getField(Me);

    FieldController::vanish(&field, &score);
    stringstream ss;
    FieldController::dump(field, ss);
    EXPECT_EQ(ss.str(),
              "......\n......\n......\n......\n......\n......\n......\n......\n......\n....1.\n5...1.\n5.5122\n");
}
