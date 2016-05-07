int main() {
    Game game = Game();
    int turn = 0;
    while (turn++ < 2000) {
        game.input(cin);
        game.output(cout);
    }
}