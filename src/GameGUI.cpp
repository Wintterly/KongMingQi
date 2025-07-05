#include "../include/GameGUI.h"
#include <iostream>


GameGUI::GameGUI() :
    window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), sf::String(L"孔明棋")),
    board(),
    pegShape(CELL_SIZE / 3.0f),  // 调整棋子大小
    cellShape(sf::Vector2f(CELL_SIZE, CELL_SIZE)),
    font(),
    messageText(),  // 先默认初始化
    remainingPegsText(),
    resetButton(),
    resetButtonText(),
    undoButton(),
    undoButtonText(),
    helpButton(),
    helpButtonText(),
    selectedRow(-1),
    selectedCol(-1),
    isSelected(false),
    showDialog(false),
    isResetButtonEnabled(false),
    showInitialInstructions(true)

{
    window.setFramerateLimit(60);

    // 设置棋子形状
    pegShape.setFillColor(sf::Color::Red);
    pegShape.setOrigin(pegShape.getRadius(), pegShape.getRadius());

    
    // 设置格子形状
    cellShape.setFillColor(sf::Color::Transparent);
    cellShape.setOutlineThickness(1.0f);
    cellShape.setOutlineColor(sf::Color::White);
    
    // 加载字体
    if (!font.loadFromFile("assets/fonts/msyh.ttc")) {  // 首先尝试从assets目录加载
        // 如果assets目录加载失败，尝试从系统目录加载
        if (!font.loadFromFile("C:\\Windows\\Fonts\\msyh.ttc")) {
            std::cerr << "Error loading font" << std::endl;
            return;
        }
    }

    // 在字体加载后设置文本
    messageText.setFont(font);
    messageText.setString("");
    messageText.setCharacterSize(30);
    messageText.setFillColor(sf::Color::White);
    messageText.setPosition(10, WINDOW_SIZE - 50);

    // 设置剩余棋子数量文本
    remainingPegsText.setFont(font);
    remainingPegsText.setCharacterSize(24);
    remainingPegsText.setFillColor(sf::Color::Yellow);
    remainingPegsText.setPosition(10, 10);  // 显示在左上角

    // 设置复原按钮
    resetButton.setSize(sf::Vector2f(100, 40));
    resetButton.setPosition(WINDOW_SIZE - 110, 10);  // 右上角位置
    resetButton.setFillColor(sf::Color(100, 100, 100));
    resetButton.setOutlineColor(sf::Color::White);
    resetButton.setOutlineThickness(2);

    // 设置复原按钮文本
    resetButtonText.setFont(font);
    resetButtonText.setString(L"复原");
    resetButtonText.setCharacterSize(20);
    resetButtonText.setFillColor(sf::Color::White);
    // 居中显示文本
    sf::FloatRect resetTextBounds = resetButtonText.getLocalBounds();
    resetButtonText.setPosition(
        WINDOW_SIZE - 110 + (100 - resetTextBounds.width) / 2,
        10 + (40 - resetTextBounds.height) / 2
    );

    // 设置撤销按钮
    undoButton.setSize(sf::Vector2f(100, 40));
    undoButton.setPosition(WINDOW_SIZE - 110, 60);  // 放在复原按钮下方
    undoButton.setFillColor(sf::Color(100, 100, 100));
    undoButton.setOutlineColor(sf::Color::White);
    undoButton.setOutlineThickness(2);

    // 设置撤销按钮文本
    undoButtonText.setFont(font);
    undoButtonText.setString(L"撤销");
    undoButtonText.setCharacterSize(20);
    undoButtonText.setFillColor(sf::Color::White);
    // 居中显示文本
    sf::FloatRect undoTextBounds = undoButtonText.getLocalBounds();
    undoButtonText.setPosition(
        WINDOW_SIZE - 110 + (100 - undoTextBounds.width) / 2,
        60 + (40 - undoTextBounds.height) / 2
    );

    // 设置提示按钮
    helpButton.setSize(sf::Vector2f(100, 40));
    helpButton.setPosition(WINDOW_SIZE - 110, 110);  // 放在撤销按钮下方
    helpButton.setFillColor(sf::Color(100, 100, 100));
    helpButton.setOutlineColor(sf::Color::White);
    helpButton.setOutlineThickness(2);

    // 设置提示按钮文本
    helpButtonText.setFont(font);
    helpButtonText.setString(L"提示");
    helpButtonText.setCharacterSize(20);
    helpButtonText.setFillColor(sf::Color::White);
    // 居中显示文本
    sf::FloatRect helpTextBounds = helpButtonText.getLocalBounds();
    helpButtonText.setPosition(
        WINDOW_SIZE - 110 + (100 - helpTextBounds.width) / 2,
        110 + (40 - helpTextBounds.height) / 2
    );

    // 初始化对话框
    initializeDialog();
    initializeInstructionsDialog();
}

// 处理事件函数
void GameGUI::handleEvents() {
    sf::Event event{};
    if (window.pollEvent(event)) {
        switch (event.type) {  // 删除 Type::
            case sf::Event::Closed:  // 修改事件类型引用
                window.close();
                break;

            case sf::Event::KeyPressed:  // 修改事件类型引用
                if (event.key.code == sf::Keyboard::Key::Escape) {
                    window.close();
                } else if (event.key.code == sf::Keyboard::Key::U) {
                    board.undoMove();
                    resetSelection();  // 撤销时重置选择状态
                }
                break;

            case sf::Event::MouseButtonPressed:  // 修改事件类型引用
                if (event.mouseButton.button == sf::Mouse::Button::Left) {
                    handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                }
                break;

            default:
                break;
        }
    }
}

// 运行游戏循环
void GameGUI::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}
// 渲染函数
void GameGUI::render() {
    window.clear(sf::Color::Black);
    drawBoard();
    drawPegs();
    
    // 计算并显示剩余棋子数量
    int remainingPegs = board.getRemainingPegs();
    std::wstring text = L"剩余棋子: " + std::to_wstring(remainingPegs);
    remainingPegsText.setString(text);
    window.draw(remainingPegsText);

    // 显示游戏状态
    std::string message;
    if (board.isGameOver()) {
        showDialog = true;
        if (board.isWin()) {
            dialogText.setString(L"恭喜你赢了！");
        } else {
            dialogText.setString(L"游戏结束！没有更多可行的移动了。");
        }
    } else if (isSelected) {
        message = "选中棋子，请选择目标位置";
        drawMessage(message);
    } else {
        message = "请选择要移动的棋子";
        drawMessage(message);
    }

    // 绘制按钮
    drawResetButton();
    drawUndoButton();
    drawHelpButton();

    // 如果游戏结束，绘制对话框
    if (showDialog) {
        drawGameOverDialog();
    }

    // 如果需要显示初始说明，绘制说明对话框
    if (showInitialInstructions) {
        drawInstructionsDialog();
    }

    window.display();
}

// 判断是否为中心格
bool isCenter(int row, int col) {
    return row == 3 && col == 3;
}

// 判断是否为角落，即不需要放棋子的格子直接不画
bool isCorner(int row, int col) {
    // 检查是否在左上、右上、左下、右下四个 2x2 的角落区域内
    return (row <= 1 && col <= 1) ||    // 左上角
           (row <= 1 && col >= 5) ||    // 右上角
           (row >= 5 && col <= 1) ||    // 左下角
           (row >= 5 && col >= 5);      // 右下角
}

// 绘制棋盘
void GameGUI::drawBoard() {
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 7; ++col) {
            if (isCorner(row, col)) continue; // 跳过不不需要绘制的位置
            // 只绘制有效格子（如果有无效格子需判断）
            auto pos = boardToScreen(row, col);
            cellShape.setPosition(pos);

            // 高亮显示可移动的位置
            if (isSelected && board.isValidMove(selectedRow, selectedCol, row, col)) {
                cellShape.setFillColor(sf::Color(0, 255, 0, 64));  // 半透明绿色
            } else {
                cellShape.setFillColor(sf::Color::Transparent);
            }

            window.draw(cellShape);
        }
    }
}

// 绘制棋子
void GameGUI::drawPegs() {
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 7; ++col) {
            if (isCorner(row, col)) continue; // 先判断是否是角落位置
            if (board.getPeg(row, col)) {     // 再判断是否有棋子
                auto pos = boardToScreen(row, col);
                // 将棋子位置调整到格子中心
                pegShape.setPosition(
                    pos.x + static_cast<float>(CELL_SIZE) / 2.0f,
                    pos.y + static_cast<float>(CELL_SIZE) / 2.0f
                );

                // 如果是选中的棋子，改变颜色
                if (row == selectedRow && col == selectedCol) {
                    pegShape.setFillColor(sf::Color::Yellow);
                } else {
                    pegShape.setFillColor(sf::Color::Red);
                }

                window.draw(pegShape);
            }
        }
    }
}

// 绘制消息文本
void GameGUI::drawMessage(const std::string& message) {
    sf::String sfString = sf::String::fromUtf8(message.begin(), message.end());
    messageText.setString(sfString);
    window.draw(messageText);
}

// 绘制复原按钮
void GameGUI::drawResetButton() {
    // 根据棋盘状态设置按钮颜色
    if (board.getMoveHistory().empty()) {
        resetButton.setFillColor(sf::Color(100, 100, 100)); // 灰色
        resetButton.setOutlineColor(sf::Color(150, 150, 150)); // 浅灰色边框
        resetButtonText.setFillColor(sf::Color(150, 150, 150)); // 浅灰色文字
    } else {
        resetButton.setFillColor(sf::Color(50, 150, 50)); // 绿色
        resetButton.setOutlineColor(sf::Color::White);
        resetButtonText.setFillColor(sf::Color::White);
    }
    
    window.draw(resetButton);
    window.draw(resetButtonText);
}

// 绘制撤销按钮
void GameGUI::drawUndoButton() {
    // 根据棋盘状态设置按钮颜色
    if (board.getMoveHistory().empty()) {
        undoButton.setFillColor(sf::Color(100, 100, 100)); // 灰色
        undoButton.setOutlineColor(sf::Color(150, 150, 150)); // 浅灰色边框
        undoButtonText.setFillColor(sf::Color(150, 150, 150)); // 浅灰色文字
    } else {
        undoButton.setFillColor(sf::Color(50, 150, 50)); // 绿色
        undoButton.setOutlineColor(sf::Color::White);
        undoButtonText.setFillColor(sf::Color::White);
    }
    
    window.draw(undoButton);
    window.draw(undoButtonText);
}

// 绘制提示按钮
void GameGUI::drawHelpButton() {
    // 根据说明对话框状态设置按钮颜色
    if (showInitialInstructions) {
        helpButton.setFillColor(sf::Color(100, 100, 100)); // 灰色
        helpButton.setOutlineColor(sf::Color(150, 150, 150)); // 浅灰色边框
        helpButtonText.setFillColor(sf::Color(150, 150, 150)); // 浅灰色文字
    } else {
        helpButton.setFillColor(sf::Color(50, 150, 50)); // 绿色
        helpButton.setOutlineColor(sf::Color::White);
        helpButtonText.setFillColor(sf::Color::White);
    }
    
    window.draw(helpButton);
    window.draw(helpButtonText);
}

// 检查是否点击了复原按钮
bool GameGUI::isResetButtonClicked(int x, int y) {
    return resetButton.getGlobalBounds().contains(static_cast<float>(x), static_cast<float>(y));
}

// 检查是否点击了撤销按钮
bool GameGUI::isUndoButtonClicked(int x, int y) {
    return undoButton.getGlobalBounds().contains(static_cast<float>(x), static_cast<float>(y));
}

// 检查是否点击了提示按钮
bool GameGUI::isHelpButtonClicked(int x, int y) {
    return helpButton.getGlobalBounds().contains(static_cast<float>(x), static_cast<float>(y));
}

// 检查是否点击了重新开始按钮
bool GameGUI::isRestartButtonClicked(int x, int y) {
    return restartButton.getGlobalBounds().contains(static_cast<float>(x), static_cast<float>(y));
}

// 检查是否点击了退出按钮
bool GameGUI::isExitButtonClicked(int x, int y) {
    return exitButton.getGlobalBounds().contains(static_cast<float>(x), static_cast<float>(y));
}

// 处理鼠标点击事件
void GameGUI::handleMouseClick(int x, int y) {
    if (showInitialInstructions) {
        showInitialInstructions = false;
        return;
    }

    if (showDialog) {
        if (isRestartButtonClicked(x, y)) {
            board = Board();  // 创建新的棋盘，重置游戏
            resetSelection();
            showDialog = false;
            return;
        }
        if (isExitButtonClicked(x, y)) {
            window.close();
            return;
        }
        return;  // 如果对话框显示时，不处理其他点击
    }

    // 检查是否点击了提示按钮
    if (isHelpButtonClicked(x, y)) {
        showInitialInstructions = true;
        return;
    }

    // 检查是否点击了复原按钮
    if (isResetButtonClicked(x, y)) {
        board = Board();  // 创建新的棋盘，重置游戏
        resetSelection();
        return;
    }

    // 检查是否点击了撤销按钮
    if (isUndoButtonClicked(x, y)) {
        board.undoMove();
        resetSelection();
        return;
    }

    auto [row, col] = screenToBoard(x, y);
    
    if (row < 0 || row >= 7 || col < 0 || col >= 7) {
        return;
    }
    
    if (!isSelected) {
        if (board.getPeg(row, col)) {
            selectedRow = row;
            selectedCol = col;
            isSelected = true;
        }
    } else {
        if (board.isValidMove(selectedRow, selectedCol, row, col)) {
            board.makeMove(selectedRow, selectedCol, row, col);
            resetSelection();
        } else if (board.getPeg(row, col)) {
            selectedRow = row;
            selectedCol = col;
        } else {
            resetSelection();
        }
    }
}

// 绘制游戏结束对话框
void GameGUI::drawGameOverDialog() {
    window.draw(dialogBox);
    window.draw(dialogText);
    window.draw(restartButton);
    window.draw(exitButton);
    window.draw(restartButtonText);
    window.draw(exitButtonText);
}

// 将棋盘坐标转换为屏幕坐标
sf::Vector2f GameGUI::boardToScreen(int row, int col) {
    return {
        static_cast<float>(col * CELL_SIZE + BOARD_OFFSET),  // 移除 CELL_SIZE/2
        static_cast<float>(row * CELL_SIZE + BOARD_OFFSET)   // 移除 CELL_SIZE/2
    };
}

// 将屏幕坐标转换为棋盘坐标
std::pair<int, int> GameGUI::screenToBoard(int x, int y) {
    int row = (y - BOARD_OFFSET) / CELL_SIZE;
    int col = (x - BOARD_OFFSET) / CELL_SIZE;
    
    // 检查点击位置是否在格子的有效范围内
    int cellX = x - BOARD_OFFSET;
    int cellY = y - BOARD_OFFSET;
    if (cellX < 0 || cellY < 0 || 
        cellX >= CELL_SIZE * 7 || 
        cellY >= CELL_SIZE * 7) {
        return {-1, -1};
    }
    
    return {row, col};
}

// 重置选择状态
void GameGUI::resetSelection() {
    selectedRow = -1;
    selectedCol = -1;
    isSelected = false;
}

// 初始化对话框
void GameGUI::initializeDialog() {
    // 初始化对话框背景
    dialogBox.setSize(sf::Vector2f(400, 200));
    dialogBox.setFillColor(sf::Color(50, 50, 50, 255));  // 将 alpha 值改为 255，使其完全不透明
    dialogBox.setOutlineColor(sf::Color::White);
    dialogBox.setOutlineThickness(2);
    dialogBox.setPosition((WINDOW_SIZE - 400) / 2, (WINDOW_SIZE - 200) / 2);

    // 初始化对话框文本
    dialogText.setFont(font);
    dialogText.setCharacterSize(24);
    dialogText.setFillColor(sf::Color::White);
    dialogText.setPosition((WINDOW_SIZE - 350) / 2, (WINDOW_SIZE - 150) / 2);

    // 初始化重新开始按钮
    restartButton.setSize(sf::Vector2f(150, 40));
    restartButton.setFillColor(sf::Color(0, 150, 0));
    restartButton.setPosition((WINDOW_SIZE - 320) / 2, (WINDOW_SIZE + 20) / 2);

    restartButtonText.setFont(font);
    restartButtonText.setString(L"重新开始");
    restartButtonText.setCharacterSize(20);
    restartButtonText.setFillColor(sf::Color::White);
    sf::FloatRect restartBounds = restartButtonText.getLocalBounds();
    restartButtonText.setPosition(
        (WINDOW_SIZE - 320) / 2 + (150 - restartBounds.width) / 2,
        (WINDOW_SIZE + 20) / 2 + (40 - restartBounds.height) / 2
    );

    // 初始化退出按钮
    exitButton.setSize(sf::Vector2f(150, 40));
    exitButton.setFillColor(sf::Color(150, 0, 0));
    exitButton.setPosition((WINDOW_SIZE + 20) / 2, (WINDOW_SIZE + 20) / 2);

    exitButtonText.setFont(font);
    exitButtonText.setString(L"退出游戏");
    exitButtonText.setCharacterSize(20);
    exitButtonText.setFillColor(sf::Color::White);
    sf::FloatRect exitBounds = exitButtonText.getLocalBounds();
    exitButtonText.setPosition(
        (WINDOW_SIZE + 20) / 2 + (150 - exitBounds.width) / 2,
        (WINDOW_SIZE + 20) / 2 + (40 - exitBounds.height) / 2
    );
}

void GameGUI::initializeInstructionsDialog() {
    // 初始化对话框背景 - 增加高度以容纳所有内容
    dialogBox.setSize(sf::Vector2f(600, 500));
    dialogBox.setFillColor(sf::Color(50, 50, 50, 255));
    dialogBox.setOutlineColor(sf::Color::White);
    dialogBox.setOutlineThickness(2);
    dialogBox.setPosition((WINDOW_SIZE - 600) / 2, (WINDOW_SIZE - 500) / 2);

    // 初始化规则文本
    sf::Text rulesText;
    rulesText.setFont(font);
    rulesText.setCharacterSize(24);
    rulesText.setFillColor(sf::Color::White);
    rulesText.setString(L"游戏规则：\n"
                       L"1. 选择一个棋子跳过相邻的棋子到空位\n"
                       L"2. 被跳过的棋子将被移除\n"
                       L"3. 只能水平或垂直方向移动\n"
                       L"4. 游戏目标是最后只剩下一个棋子\n"
                       L"操作方法：\n"
                       L"- 点击要移动的棋子，然后点击目标位置\n"
                       L"- 使用撤销按钮可以撤销上一步移动\n"
                       L"- 使用复原按钮可以重新开始游戏\n"
                       L"- 使用提示按钮可以再次查看此说明\n");
    rulesText.setPosition((WINDOW_SIZE - 550) / 2, (WINDOW_SIZE - 450) / 2);

    // 初始化开始游戏提示文本
    sf::Text startText;
    startText.setFont(font);
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::Red);  // 设置为红色
    startText.setString(L"点击任意位置开始游戏");
    
    // 计算开始游戏文本的位置（在规则文本下方居中）
    sf::FloatRect rulesBounds = rulesText.getLocalBounds();
    startText.setPosition(
        (WINDOW_SIZE - startText.getLocalBounds().width) / 2,
        rulesText.getPosition().y + rulesBounds.height + 40  // 在规则文本下方留出一定间距
    );

    // 将两个文本组合
    dialogText = rulesText;
    dialogText.setString(rulesText.getString() + L"\n\n" + startText.getString());
    dialogText.setPosition((WINDOW_SIZE - 550) / 2, (WINDOW_SIZE - 450) / 2);
}

void GameGUI::drawInstructionsDialog() {
    window.draw(dialogBox);
    window.draw(dialogText);
}
