#include "../include/Game.h"
#include <iostream>
#include <string>
#include <sstream>

Game::Game() {}

void Game::showInstructions() const {
    std::cout << "\n=== 孔明棋游戏说明 ===\n";
    std::cout << "移动规则：\n";
    std::cout << "1. 选择一个棋子跳过相邻的棋子到空位\n";
    std::cout << "2. 被跳过的棋子将被移除\n";
    std::cout << "3. 只能水平或垂直方向移动\n";
    std::cout << "4. 游戏目标是最后只剩下一个棋子\n\n";
    std::cout << "输入格式：'fromRow fromCol toRow toCol'\n";
    std::cout << "特殊命令：\n";
    std::cout << "- 'undo': 撤销上一步移动\n";
    std::cout << "- 'quit': 退出游戏\n";
    std::cout << "- 'help': 显示此帮助信息\n\n";
}

bool Game::getUserInput(int& fromRow, int& fromCol, int& toRow, int& toCol) {
    std::string input;
    std::cout << "请输入移动坐标 (fromRow fromCol toRow toCol) 或命令: ";
    std::getline(std::cin, input);
    
    if (input == "quit" || input == "undo" || input == "help") {
        return handleCommand(input);
    }
    
    std::istringstream iss(input);
    if (iss >> fromRow >> fromCol >> toRow >> toCol) {
        return true;
    }
    
    std::cout << "输入格式错误！请重试。\n";
    return false;
}

bool Game::handleCommand(const std::string& command) {
    if (command == "quit") {
        std::cout << "游戏结束！\n";
        exit(0);
    } else if (command == "undo") {
        if (board.undoMove()) {
            std::cout << "已撤销上一步移动。\n";
            board.printBoard();
        } else {
            std::cout << "无法撤销：没有之前的移动记录。\n";
        }
    } else if (command == "help") {
        showInstructions();
    }
    return false;
}

void Game::start() {
    showInstructions();
    board.printBoard();
    
    while (true) {
        if (board.isGameOver()) {
            if (board.isWin()) {
                std::cout << "恭喜你赢了！\n";
            } else {
                std::cout << "游戏结束！没有更多可行的移动了。\n";
            }
            break;
        }
        
        int fromRow, fromCol, toRow, toCol;
        if (getUserInput(fromRow, fromCol, toRow, toCol)) {
            if (board.makeMove(fromRow, fromCol, toRow, toCol)) {
                board.printBoard();
            } else {
                std::cout << "无效的移动！请重试。\n";
            }
        }
    }
} 