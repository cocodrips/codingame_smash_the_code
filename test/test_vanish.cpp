#include "gtest/gtest.h"
#include "../game.cpp"


TEST(VanishTest, all_direction) {
    Game game = Game();
    ifstream f("input/all_direction_chain.txt");
    game.input(f);


    int score;
    Field field = game.getField(Me);

    FieldController::vanish(&field);
    stringstream ss;
    FieldController::dump(field, ss);
    EXPECT_EQ(ss.str(), ""
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "....1.\n"
            "5...1.\n"
            "545122\n");
}

TEST(VanishTest, next_block) {
    Game game = Game();
    ifstream f("input/simple_chain.txt");
    game.input(f);

    int score = 0;
    Field field = game.getField(Me);
    FieldController::next(&field, game.getDrop(0), 0, &score);
    stringstream ss;
    FieldController::dump(field, ss);

    EXPECT_EQ(ss.str(), ""
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n"
            "......\n");

    EXPECT_EQ(score, 120);
}

