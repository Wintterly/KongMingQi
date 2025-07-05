#include "../include/Board.h"
#include <iostream>

Board::Board() {
    initializeBoard();
}

void Board::initializeBoard() {
    // 初始化棋盘大小
    board.resize(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, false));
    
    // 设置十字形状的棋盘
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            // 设置十字形状的棋盘
            if ((i >= 2 && i <= 4) || (j >= 2 && j <= 4)) {
                board[i][j] = true;
            }
        }
    }
    
    // 设置中心位置为空
    board[3][3] = false;
    
    // 清空移动历史
    moveHistory.clear();
}

// 检查位置是否在有效范围内
bool Board::isValidPosition(int row, int col) const {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return false;
    }
    
    // 检查是否在十字形状的有效区域内
    return (row >= 2 && row <= 4) || (col >= 2 && col <= 4);
}

bool Board::getPeg(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return false;
    }
    return board[row][col];
}

bool Board::isValidMove(int fromRow, int fromCol, int toRow, int toCol) const {
    // 检查起点和终点是否在有效位置
    if (!isValidPosition(fromRow, fromCol) || !isValidPosition(toRow, toCol)) {
        return false;
    }
    
    // 检查起点是否有棋子，终点是否为空
    if (!board[fromRow][fromCol] || board[toRow][toCol]) {
        return false;
    }
    
    // 检查是否是横向或纵向移动两格
    int rowDiff = std::abs(toRow - fromRow);
    int colDiff = std::abs(toCol - fromCol);
    
    if ((rowDiff == 2 && colDiff == 0) || (rowDiff == 0 && colDiff == 2)) {
        // 检查中间位置是否有棋子
        int midRow = (fromRow + toRow) / 2;
        int midCol = (fromCol + toCol) / 2;
        return board[midRow][midCol];
    }
    
    return false;
}

bool Board::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    if (!isValidMove(fromRow, fromCol, toRow, toCol)) {
        return false;
    }
    
    // 执行移动
    board[fromRow][fromCol] = false;
    board[(fromRow + toRow) / 2][(fromCol + toCol) / 2] = false;
    board[toRow][toCol] = true;
    // 记录移动历史
    moveHistory.push_back(std::make_pair(std::make_pair(fromRow, fromCol), std::make_pair(toRow, toCol)));
    
    return true;
}

bool Board::isGameOver() const {
    // 检查是否还有合法移动
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j]) {
                // 检查四个方向
                if (isValidMove(i, j, i+2, j) || isValidMove(i, j, i-2, j) ||
                    isValidMove(i, j, i, j+2) || isValidMove(i, j, i, j-2)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Board::isWin() const {
    // 计算剩余棋子数
    int pegCount = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j]) {
                pegCount++;
            }
        }
    }
    return pegCount == 1;
}

// 打印棋盘状态
void Board::printBoard() const {
    std::cout << "\n  ";
    for (int j = 0; j < BOARD_SIZE; ++j) {
        std::cout << j << " ";
    }
    std::cout << "\n";
    
    for (int i = 0; i < BOARD_SIZE; ++i) {
        std::cout << i << " ";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (!isValidPosition(i, j)) {
                std::cout << "  ";
            } else if (board[i][j]) {
                std::cout << "O ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// 撤销上一步移动
bool Board::undoMove() {
    if (moveHistory.empty()) {
        return false;
    }
    
    // 获取最后一次移动
    auto lastMove = moveHistory.back();
    int fromRow = lastMove.first.first;
    int fromCol = lastMove.first.second;
    int toRow = lastMove.second.first;
    int toCol = lastMove.second.second;
    
    // 恢复棋盘状态
    board[fromRow][fromCol] = true;
    board[(fromRow + toRow) / 2][(fromCol + toCol) / 2] = true;
    board[toRow][toCol] = false;
    
    // 移除最后一次移动记录
    moveHistory.pop_back();
    
    return true;
}

int Board::getRemainingPegs() const {
    int count = 0;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (board[row][col]) {
                count++;
            }
        }
    }
    return count;
}

