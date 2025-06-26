
#include "iostream"
#include "globals.hpp"
#include <vector>
#include "sub_board.hpp"


sub_board::sub_board(){
    spots.resize(9);
    for (int i = 0; i<9; i++){
        spots[i] = EMPTY;
    };
    active = false;
};

sub_board::sub_board(std::vector<piece> set_board, bool active){
    spots.resize(9);
    for (int i = 0; i<9; i++){
        spots[i] = set_board[i];
    };
    this->active = active;
};

void sub_board::set_active(bool val){
    active = val;
};

void sub_board::print(){
    std::cout << "Status: ";
    if(active){
        std::cout << "Active :)" << std::endl;
    } else {
        std::cout << "Not Active :(" << std::endl;
    };
    for (int i = 0; i<9; i++){
        if(i%3 == 0 && i != 0){
            std::cout << '\n';
        };
        std::cout << spots[i];
    }
    std::cout << std::endl;
}

void sub_board::place_piece(int spot, piece p){
    if (spot < 0 || spot > 8) {
        std::cout << "Invalid spot index: " << spot << std::endl;
        return;
    }
    if (spots[spot] != EMPTY) {
        std::cout << "Spot already occupied!" << std::endl;
        return;
    }
    spots[spot] = p;
};

void sub_board::check_wins(){
    for(int i = 0; i<8; i++){
        int sum = 0;
        for (int n = 0; n<3; n++){
            sum += spots[WINNING_SETS[i][n]];
        };

        if(abs(sum)==3){
            winner = static_cast<piece>(sum/3.f);
            std::cout << "winner: " << winner << std::endl;
            return;
        };
    };
};