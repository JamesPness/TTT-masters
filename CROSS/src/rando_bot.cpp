#include <iostream>
#include <vector>
#include <random>

#include "rando_bot.hpp"
#include "globals.hpp"
#include "sub_board.hpp"
#include "globals.hpp"

move rando_bot(raw_boardstate boardstate) {
    big_board board(boardstate);
    
    std::vector<move> moves = board.get_allowed_moves();
    int pick = rand() % (moves.size() + 1);

    std::cout << moves[0].sub << " " << moves[0].spot << std::endl;

    return moves[pick];
}

