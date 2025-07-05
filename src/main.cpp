#include "../include/GameGUI.h"
#include <iostream>

int main() {
    try {
        GameGUI game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    return 0;
} 