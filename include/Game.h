#ifndef KONGMINGQI_GAME_H
#define KONGMINGQI_GAME_H

#include <string>
#include "Board.h"

class Game {
public:
    // 构造函数
    Game();
    
    // 开始游戏
    void start();
    
private:
    Board board;
    
    // 获取用户输入
    bool getUserInput(int& fromRow, int& fromCol, int& toRow, int& toCol);
    
    // 显示游戏说明
    void showInstructions() const;
    
    // 处理用户命令
    bool handleCommand(const std::string& command);
};

#endif // KONGMINGQI_GAME_H 