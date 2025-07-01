#pragma once
#include <iostream>
#include <vector>
#include "globals.hpp"

class sub_board{
    private:
        std::vector<piece> spots;
        bool active;
        piece winner;
    public:
        sub_board();
        sub_board(std::vector<piece> set_board, bool active);

        bool get_active(){
            return active;
        };
        void set_active(bool val){
            active = val;
        };

        std::vector<piece> get_spots(){
            return spots;
        };

        piece get_winner(){
            return winner;
        };

        void print();
        void place_piece(int spot, piece p);
        void check_wins();
};

