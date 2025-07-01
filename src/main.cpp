#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <iostream>
#include <filesystem>
#include <dlfcn.h>
#include <cstring>

#include "globals.hpp"
#include "sub_board.hpp"
#include "big_board.hpp"
#include "grid.hpp"
#include "player_banner.hpp"

float screen_width = 800;
float screen_height = 800;

Color sub_color = {173, 151, 104, 255}; 

Vector2 main_pos = {screen_width*0.125f, screen_height*0.2f};
grid main_grid(main_pos, screen_width*0.75f, screen_height*0.75f, {133, 125, 103, 255}, 0.05f);   
big_board board;
std::vector<grid> sub_grids;

float hand_place_aq;

int get_closest(Vector2 spot, std::vector<Vector2> candidates){
    std::vector<float> dist_list;
    dist_list.resize(candidates.size());

    if(spot.x < 0 || spot.y < 0 || spot.x > screen_width || spot.y > screen_height){
        return -1;
    }

    int target = 0;
    
    for (int i = 0; i<candidates.size(); i++){
        dist_list[i] = dist(spot, candidates[i]);
        if (dist_list[i]<dist(spot, candidates[target])){
            target = static_cast<int>(i);
        };
    };
    return target;
};

move hand_place(raw_boardstate b){

    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        std::cout << GetMousePosition().x << ", " << GetMousePosition().y << std::endl;
        Vector2 mouse_pos = GetMousePosition();
        int sub = get_closest(mouse_pos, main_grid.get_global_grid_spots());

        int spot = get_closest(mouse_pos, sub_grids[sub].get_global_grid_spots());

        if(!board.is_valid_move((move){sub, spot})){
            //std::cerr << '{' << sub << ", " << spot << '}' << " is not at valid move" << std::endl;
            return {-1,-1};
        };


        if (dist(mouse_pos, sub_grids[sub].get_global_grid_spots()[spot]) <= hand_place_aq){
            move _move = {sub, spot};
            return _move;
            //std::cout << board.get_sub_boards()[sub].get_spots()[spot] << std::endl;
            /*
            for (int i = 0; i<9; i++){
                board.get_sub_boards()[i].print();
            };
            */
        };
    };
    return {-1,-1};
};

move get_move(raw_boardstate boardstate, std::string so_file_path, std::string func_name){
    //std::cout << so_file_path << std::endl;
    //std::cout << func_name << std::endl;
    move _move;
    move (*func)(raw_boardstate);
    void* so_file = dlopen(so_file_path.c_str(), RTLD_LAZY);
    if(!so_file){
        func = hand_place;
    } else {
        dlerror();
        void* sym = dlsym(so_file, func_name.c_str());
        if (!sym) {
            std::cerr << "dlsym failed for " << func_name << std::endl;
            // fallback or exit
        }
        std::memcpy(&func, &sym, sizeof(sym));
    }
    //std::cout << board.get_allowed_moves()[0].sub << ", " << board.get_allowed_moves()[0].spot << std::endl;
    _move = func(boardstate);
    dlclose(so_file);
    return _move;
}
    

int main() {
    InitWindow(screen_width, screen_height, "TTT-masters");
    SetTargetFPS(60);

    //std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    const std::string config_path_cross = "CROSS/config.csv";
    const std::string config_path_dot = "DOT/config.csv";

    player_banner cross_banner(config_path_cross);
    player_banner dot_banner(config_path_dot);


    std::string so_path_cross = "./CROSS/lib"+cross_banner.get_player_name()+".so";
    if(!std::filesystem::exists(so_path_cross)){
        std::cout << so_path_cross << " does not exist";
        so_path_cross = "";
        cross_banner.set_player_name("Hand");
    }
    
    std::string so_path_dot = "./DOT/lib"+dot_banner.get_player_name()+".so";
    if(!std::filesystem::exists(so_path_dot)){
        std::cout << so_path_dot << " does not exist";
        so_path_dot = "";
        dot_banner.set_player_name("Hand");
    }

    sub_grids.resize(9);

    for (int sub = 0; sub<9; sub++){
        float sub_pole_width = main_grid.get_rel_pole_width();
        float sub_width = main_grid.get_area().width/3.f - main_grid.get_global_pole_width();
        float sub_height = main_grid.get_area().height/3.f - main_grid.get_global_pole_width();
        Vector2 offset_vector = {sub_width/2.f, sub_height/2.f};
        Vector2 sub_pos = Vector2Subtract(main_grid.get_global_grid_spots()[sub], offset_vector);
        sub_grids[sub] = grid(sub_pos, sub_width, sub_height, sub_color, sub_pole_width);
    };

    hand_place_aq = sub_grids[0].get_area().width/3.f - sub_grids[0].get_global_pole_width();

    board.print();
    //std::cout << cross_func(board.get_raw_boardstate()).sub << ", " << cross_func(board.get_raw_boardstate()).spot << std::endl;

    while (!WindowShouldClose()) {

        //Drawing
        BeginDrawing();
        ClearBackground(BG_COLOR);

        cross_banner.draw_right({screen_width*0.025f, screen_height*0.06875f}, screen_width*0.375f, screen_height*0.1125f, true ? board.get_turn() == CROSS : false);
        dot_banner.draw_left({screen_width-cross_banner.get_position().x-cross_banner.get_width(), cross_banner.get_position().y}, cross_banner.get_width(), cross_banner.get_height(), true ? board.get_turn() == DOT : false);

        DrawText("Tic Tac Toe Masters", screen_width/2.f - MeasureText("Tic Tac Toe Masters", 20)/2.f, 10, 20, BLACK);

        int status_font_size = 30;
        int status_margin = 15;
        int status_x = status_margin;
        int status_y = status_margin;
        Color status_color = BLACK;

        if (board.get_winner() == EMPTY) {
            const char* ongoing_text = "Ongoing...";
            DrawText(ongoing_text, status_x, status_y, status_font_size, status_color);
        } else {
            const char* winner_label = "Winner: ";
            DrawText(winner_label, status_x, status_y, status_font_size, status_color);

            int label_width = MeasureText(winner_label, status_font_size);
            int winner_x = status_x + label_width + 5;

            if (board.get_winner() == CROSS) {
                DrawText("X", winner_x, status_y, status_font_size, RED);
            } else if (board.get_winner() == DOT) {
                DrawText("O", winner_x, status_y, status_font_size, BLUE);
            } else {
                DrawText("None", winner_x, status_y, status_font_size, status_color);
            }
        }

        const char* turn_label = "Turn:";
        int turn_label_font_size = 30;
        int turn_label_margin = 15;
        int turn_label_width = MeasureText(turn_label, turn_label_font_size);

        // Calculate dynamic positions
        int turn_label_x = screen_width - turn_label_width - 2 * turn_label_margin - turn_label_font_size;
        int turn_label_y = turn_label_margin;

        DrawText(turn_label, turn_label_x, turn_label_y, turn_label_font_size, BLACK);

        char turn_char = (board.get_turn() == CROSS) ? 'X' : 'O';
        char turn_str[2] = { turn_char, '\0' };
        Color turn_color = (board.get_turn() == CROSS) ? RED : BLUE;

        // Place the turn character right after the label, with a margin
        int turn_char_x = turn_label_x + turn_label_width + turn_label_margin;
        int turn_char_y = turn_label_y;

        DrawText(turn_str, turn_char_x, turn_char_y, turn_label_font_size, turn_color);

        if (board.get_winner() == EMPTY){
            if (board.get_current() != -1){
                Vector2 indicator_pos = sub_grids[board.get_current()].get_global_position();
                Vector2 indicator_size = {sub_grids[board.get_current()].get_area().width, sub_grids[board.get_current()].get_area().height};
                Color indicator_color = GREEN;

                DrawRectangleV(indicator_pos, indicator_size, indicator_color);

            } else {
                Vector2 indicator_pos = main_grid.get_global_position();
                Vector2 indicator_size = {main_grid.get_area().width, main_grid.get_area().height};
                Color indicator_color = GREEN;

                DrawRectangleV(indicator_pos, indicator_size, indicator_color);
            };
        };
        
        for (int sub = 0; sub<9; sub++){
            if (board.get_sub_boards()[sub].get_winner() == EMPTY){
                sub_grids[sub].draw_grid();
                for (int spot = 0; spot < 9; spot++){
                    piece curr = board.get_sub_boards()[sub].get_spots()[spot];
                    if(curr != EMPTY){
                        if(curr == CROSS){
                            sub_grids[sub].draw_X(spot);
                        } else {
                            sub_grids[sub].draw_O(spot);
                        };
                    };
                };
            } else {
                sub_grids[sub].draw_winner(board.get_sub_boards()[sub].get_winner());
            };
        };

        main_grid.draw_grid();
        //main_grid.draw_spots();
        
        EndDrawing();

        if(board.get_winner() == EMPTY){
            raw_boardstate current_boardstate = board.get_raw_boardstate();
            move _move;
            if(board.get_turn() == CROSS){
                if(so_path_cross == ""){
                    _move = hand_place((raw_boardstate){});
                } else {
                    _move = get_move(current_boardstate, so_path_cross, cross_banner.get_player_name());
                }
            } else if( board.get_turn() == DOT){
                if(so_path_dot == ""){
                    _move = hand_place((raw_boardstate){});
                } else {
                    _move = get_move(current_boardstate, so_path_dot, dot_banner.get_player_name());
                }
            };


            if(board.is_valid_move(_move)){
                board.place_piece(_move.sub, _move.spot, board.get_turn());
            } else if(_move.sub != -1 && _move.spot != -1){
                std::cerr << '{' << _move.sub << ", " << _move.spot << '}' << " is not at valid move" << std::endl;
                board.set_winner(static_cast<piece>(-board.get_turn()));
            };
            //////////////////////////////
        };
    }
    CloseWindow();
    return 0;
}