#ifndef KONGMINGQI_BOARD_H
#define KONGMINGQI_BOARD_H

#include <vector>
#include <utility>

class Board {
public:
    // 构造函数
    Board();
    
    // 获取棋盘状态
    bool getPeg(int row, int col) const;
    
    // 检查移动是否合法
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol) const;
    
    // 执行移动
    bool makeMove(int fromRow, int fromCol, int toRow, int toCol);
    
    // 检查游戏是否结束
    bool isGameOver() const;
    
    // 检查是否获胜
    bool isWin() const;
    
    // 打印棋盘
    void printBoard() const;
    
    // 撤销上一步移动
    bool undoMove();
    // 获取剩余棋子数量
    int getRemainingPegs() const;

    // 获取移动历史
    const std::vector<std::pair<std::pair<int, int>, std::pair<int, int> > >& getMoveHistory() const {
        return moveHistory;
    }

private:
    static const int BOARD_SIZE = 7;  // 棋盘大小
    std::vector<std::vector<bool> > board;  // 棋盘状态
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int> > > moveHistory;  // 移动历史
    
    // 检查坐标是否在棋盘内
    bool isValidPosition(int row, int col) const;
    
    // 初始化棋盘
    void initializeBoard();
};

#endif // KONGMINGQI_BOARD_H

