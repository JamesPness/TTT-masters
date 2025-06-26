#pragma once
#include <vector>
#include "globals.hpp"
#include "sub_board.hpp"

class big_board{
    private:
        std::vector<sub_board> sub_boards;
        short current;
        piece turn;
        piece winner;

    public:
        big_board();
        big_board(std::vector<sub_board> set_board, piece set_turn, short cur);
        big_board(raw_boardstate boardstate);
        
        std::vector<sub_board> get_sub_boards(){
            return sub_boards;
        };

        short get_current(){
            return current;
        };

        piece get_winner(){
            return winner;
        };

        piece get_turn(){
            return turn;
        };

        raw_boardstate get_raw_boardstate();
        std::vector<move> get_allowed_moves();
        void place_piece(int sub_board_index, int spot, piece p);
        void check_wins();
        bool is_valid_move(move _move);
        void print();

        void next_turn();
    };
        