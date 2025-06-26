#!/bin/bash
g++ src/main.cpp src/grid.cpp src/big_board.cpp src/sub_board.cpp src/globals.cpp src/player_banner.cpp -I./src/include -L./src/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o TTT-masters -g
