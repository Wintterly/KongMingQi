class Board;
#ifndef KONGMINGQI_GAMEGUI_H
#define KONGMINGQI_GAMEGUI_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"

class GameGUI {
public:
    GameGUI();
    void run();

private:
    static const int WINDOW_SIZE = 1024;  // 增加窗口大小
    static const int CELL_SIZE = 100;     // 增加格子大小
    static const int BOARD_SIZE = 7;
    static const int BOARD_OFFSET = (WINDOW_SIZE - BOARD_SIZE * CELL_SIZE) / 2;
    static const int BUTTON_WIDTH = 120;   // 增加按钮宽度
    static const int BUTTON_HEIGHT = 50;   // 增加按钮高度
    bool isResetButtonEnabled;  // 标记复原按钮是否可用
    bool showInitialInstructions; // 是否显示初始游戏说明

    sf::Text remainingPegsText;
    sf::RenderWindow window;
    Board board;
    sf::CircleShape pegShape;
    sf::RectangleShape cellShape;
    sf::Font font;
    sf::Text messageText;

    // 选中的棋子位置
    int selectedRow;
    int selectedCol;
    bool isSelected;

    sf::RectangleShape resetButton;
    sf::Text resetButtonText;

    sf::RectangleShape undoButton;      // 撤销按钮
    sf::Text undoButtonText;            // 撤销按钮文字

    sf::RectangleShape helpButton;      // 提示按钮
    sf::Text helpButtonText;            // 提示按钮文字

    // 游戏结束对话框相关
    sf::RectangleShape dialogBox;
    sf::Text dialogText;
    sf::RectangleShape restartButton;
    sf::RectangleShape exitButton;
    sf::Text restartButtonText;
    sf::Text exitButtonText;
    bool showDialog;

    void handleEvents();
    void render();
    void drawBoard();
    void drawPegs();
    void drawMessage(const std::string& message);
    void drawResetButton();
    void drawUndoButton();              // 绘制撤销按钮
    void drawHelpButton();              // 绘制提示按钮
    void drawGameOverDialog();
    void drawInstructionsDialog();      // 绘制游戏说明对话框
    bool isResetButtonClicked(int x, int y);
    bool isUndoButtonClicked(int x, int y);  // 检查是否点击了撤销按钮
    bool isHelpButtonClicked(int x, int y);  // 检查是否点击了提示按钮
    bool isRestartButtonClicked(int x, int y);
    bool isExitButtonClicked(int x, int y);
    void initializeDialog();
    void initializeInstructionsDialog(); // 初始化游戏说明对话框

    // 处理鼠标点击
    void handleMouseClick(int x, int y);
    
    // 坐标转换函数
    static sf::Vector2f boardToScreen(int row, int col);
    static std::pair<int, int> screenToBoard(int x, int y);

    // 重置选择状态
    void resetSelection();
};

#endif // KONGMINGQI_GAMEGUI_H

