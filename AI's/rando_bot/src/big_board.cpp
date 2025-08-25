
#include <vector>
#include <iostream>
#include "globals.hpp"
#include "big_board.hpp"
#include "sub_board.hpp"
#include <vector>

big_board::big_board(){
        sub_boards.resize(9);
    for (int i = 0; i<9; i++){
        for (int n = 0; n < 9; n++){
            sub_boards[i].get_spots()[n] = EMPTY;
        }
    };
    turn = CROSS;
    current = -1;
    winner = EMPTY;
};


big_board::big_board(std::vector<sub_board> set_board, piece set_turn, short cur){
    sub_boards.resize(9);
    for (int i = 0; i<9; i++){
        sub_boards[i] = set_board[i];
    };
    turn = set_turn;
    current = cur;
    winner = EMPTY;
    check_wins();
};

big_board::big_board(raw_boardstate boardstate){
    sub_boards.resize(9);
    turn = boardstate.turn;
    current = boardstate.current;
    winner = EMPTY;

    for (int sub = 0; sub<9; sub++){
        sub_boards[sub] = sub_board(boardstate.board[sub], true ? current == sub || current == -1 : false);
    }

    check_wins();
};

raw_boardstate big_board::get_raw_boardstate(){
    raw_boardstate boardstate;
    for (int sub = 0; sub<9; sub++){
        boardstate.board[sub] = sub_boards[sub].get_spots();
    }
    boardstate.turn = turn;
    boardstate.current = this->current; 


    return boardstate;
};

void big_board::next_turn(){
    check_wins();
    turn = static_cast<piece>(-turn);
}

void big_board::check_wins(){
    for (int sub = 0; sub<9; sub++){
        sub_boards[sub].check_wins();
    };

    for(int i = 0; i<8; i++){
        int sum = 0;
        for (int n = 0; n<3; n++){
            sum += sub_boards[WINNING_SETS[i][n]].get_winner();
        };

        if(abs(sum)==3){
            winner = static_cast<piece>(sum/3.f);
            std::cout << "winner: " << winner << std::endl;
            return;
        };
    };
};

void big_board::place_piece(int sub, int spot, piece p){
    sub_boards[sub].print();
    std::cout << "current-1: " << current << std::endl;
    if(!is_valid_move((move){sub, spot})){
        std::cerr << '{' << sub << ", " << spot << '}' << " is not at valid move" << std::endl;
        winner = static_cast<piece>(-turn);
        return;
    }

    sub_boards[sub].place_piece(spot, p);
    check_wins();

    if(sub_boards[spot].get_winner() != EMPTY){
        current = -1;
    } else {
        current = spot;
    };

    for (int i = 0; i<9; i++){
        if (i == spot || current == -1){
            sub_boards[i].set_active(true);
        } else {
            sub_boards[i].set_active(false);
        }
    }
    next_turn();
    std::cout << "current-2: " << current << std::endl;
};

std::vector<move> big_board::get_allowed_moves(){
    std::vector<move> moves;

    for(int sub = 0; sub < 9; sub++){
        for(int spot = 0; spot < 9; spot++){
            if(is_valid_move((move){sub, spot})){
                moves.push_back((move){sub, spot});
            }
        }
    }

    return moves; 
};

bool big_board::is_valid_move(move _move){
    if(!(_move.sub >= 0 && _move.sub < 9)){
        return false;
    }
    if (get_sub_boards()[_move.sub].get_winner() != EMPTY){
        return false;
    }
    if (_move.sub != current){
        if(current != -1){
            return false;
        };
    } 
    if(!(_move.spot >= 0 && _move.spot < 9)){
        return false;
    } 
    if(sub_boards[_move.sub].get_spots()[_move.spot] != EMPTY){
        sub_boards[_move.sub].print();
        return false;
    }
    return true;
    
}

void big_board::print(){
    for (int big_rows = 0; big_rows<3; big_rows++){
        std::cout << "-------------------------------------\n";
        for (int line = 0; line<3; line++){
            std::cout << "| ";
            for (int block = 0; block<3; block++){
                for (int spot = 0; spot<3; spot++){
                    std::cout << ' ';
                    std::cout << sub_boards[big_rows*3+block].get_spots()[line*3+spot];
                    std::cout << ' ';
                };
                if (sub_boards[big_rows*3+block].get_active()){
                    std::cout << " < ";
                } else {
                    std::cout << " | ";
                };
            }
            std::cout << '\n';
        }
    }
    std::cout << "-------------------------------------\n";
};

        