//
// Created by vorop on 02.11.2023.
//


#include <fcntl.h>
#include "Game.h"

// Главное меню
void Game::start() {
    // Создаю окно и устанавливаю иконку
    window.create(sf::VideoMode(1200, 950), "Chess", sf::Style::Close);
    window.setIcon(32, 32, texture.icon.getPixelsPtr());

    this->drawStartMenu();  // Отрисовка главного меню
    window.display();
    while (this->waitClick()) {     // Ожидание нажатия ЛКМ
        switch (getCoordinatesClickStartMenu()) {   // Получить элемент на который нажали
            case SelectStartMenu::NewGameWithFriend:
                this->newGameWithFriend();    // Начать новую игру с игроком
                break;
            case SelectStartMenu::NewGameWithComputer:
                this->newGameWithComputer();  // Начать новую игру с компьютером
                break;
            case SelectStartMenu::RestoreGameWithFriend:
                this->restoreGameWithFriend();  // Восстановить игру с другом
                break;
            case SelectStartMenu::RestoreGameWithComputer:
                this->restoreGameWithComputer(); // Восстановить игру с компьютером
                break;
            case SelectStartMenu::ViewingCompletedGames:
                this->processWatchGame();       // Просмотр законченных игр
                break;
            case SelectStartMenu::ChangeStyle:
                this->changeStyle();       // Просмотр законченных игр
                this->drawStartMenu();
                window.display();
                break;
            case SelectStartMenu::Exit:         // Выход из программы
                return;
            default:
                break;
        }
        this->drawStartMenu();         // Вывести главное меню
        window.display();
    }
}

// Новая игра с игроком
void Game::newGameWithFriend() {
    // Установка Игроков и состояния игры на Gameon
    this->texture.player1Name.setString("Player 1");
    this->texture.player2Name.setString("Player 2");
    this->texture.player1Name.setPosition(1020, 795);    // Установка координат вывода слова
    this->texture.player2Name.setPosition(1020, 93);
    board.setStateGame(StateGame::GameOn);
    opponentComputer = false;                 // false - игра против друга; true - игра против компьютера
    board.setBoardByFen(board.getStartFEN());   // Установить доску в стартовое состояние
    processGameWithFriend();            // Цикл игры с игроком
}

// Цикл игры с игроком
void Game::processGameWithFriend() {
    while (window.isOpen() && board.getStateGame() != StateGame::Exit) {
        this->drawGame();       // Вывести игровое поле
        window.display();
        board.checkOnMate();    // Проверка на мат
        // Проверка на конец игры
        while (board.getStateGame() != StateGame::GameOn && window.isOpen()) {
            if (this->endGame())    // Подтверждение окончания игры
                return;
            else {
                this->drawGame();
                window.display();
            }
        }
        movePlayer();       // Получить ход от игрока
    }
}


// Получить координаты нажатой клетки шахматной доски
bool Game::getCoordinatesPressedBox(Coordinates &coordinates) const {
    if ((event.mouseButton.y > marginUp && event.mouseButton.y < marginUp + sizeBoardPX) &&
        // Проверка диапазона в границы поля
        (event.mouseButton.x > marginLeft && event.mouseButton.x < marginLeft + sizeBoardPX)) {
        coordinates.y = static_cast<short int>((event.mouseButton.y - marginUp) / sizeBox); // Координата у
        coordinates.x = static_cast<short int>((event.mouseButton.x - marginLeft) / sizeBox); // Координата х
        return true;
    } else return false;
}

// Запуск новой игры против компьютера
void Game::newGameWithComputer() {
    this->newOnlineGame();
//    this->texture.player1Name.setString("Player");
//    this->texture.player2Name.setString("Computer");
//    this->texture.player1Name.setPosition(1020,795);    // Установка координат вывода слова
//    this->texture.player2Name.setPosition(1010,93);     // Поправка для длинного слова computer
//    opponentComputer = true;
//    board.setStateGame(StateGame::GameOn);  // Установка активного режима игры
//    board.setBoardByFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); // Fen Start Board
//    // Запуск "Stockfish"
//    try {
//        if (!stockFish.startStockFish())   // Запуск
//            throw (ExceptionStockFish("Error start StockFish", -2));
//        stockFish.sendStockFishCommand("uci");  // Стартовая команда
//        int difficultyBot = 1;
//        setDifficultyBot(difficultyBot);    // Установка сложности
//        processGameWithComputer();          // Запуск цикла игры с компьютером
//    }
//    catch (ExceptionStockFish& err){        // Обработка исключения
//        std::cout << err.what() << std::endl;
//        std::cout << "Code error: " << err.code() << std::endl;
//        return;
//    }
//    stockFish.closeStockFish();             // Закрыть Stockfish
}

// Цикл игры с компьютером
void Game::processGameWithComputer() {
    while (window.isOpen() && board.getStateGame() != StateGame::Exit) {
        this->drawGame();       // Вывод игрового поля
        window.display();
        board.checkOnMate();    // Проверка на мат
        while (board.getStateGame() != StateGame::GameOn) {
            if (this->endGame())    // Подтверждение окончания игры
                return;
            else {
                this->drawGame();
                window.display();
            }
        }
        if (board.itWhiteMoveNow() == colorPlayer1IsWhite)
            this->movePlayer();     // Ход игрока
        else if (board.itWhiteMoveNow() == colorPlayer2IsWhite)
            this->moveComputer();   // Ход компьютера
    }
}

// Установка сложности компьютера
void Game::setDifficultyBot(int diff) {
//    std::string cmd="setoption name Skill Level value " + std::to_string(diff);
//    stockFish.sendStockFishCommand(cmd);
}

// Ход компьютера
void Game::moveComputer() {
    Coordinates oldCoord{}, newCoord{};
    getMoveComputer(oldCoord, newCoord);         // get координаты хода
    board.move(oldCoord, newCoord);              // Ход на игровой доске
    // Обработка прохода пешкой
    if (board.map[newCoord.y][newCoord.x]->getType() == TypeFigure::Pawn && (newCoord.y == 0 || newCoord.y == 7)) {
        delete board.map[newCoord.y][newCoord.x];
        board.map[newCoord.y][newCoord.x] = new Queen(!board.itWhiteMoveNow(), newCoord.y, newCoord.x, false);
    }
}

//    // Получить ход компьютера
void Game::getMoveComputer(Coordinates &oldCoord, Coordinates &newCoord) {
//    // Генерация FEN строки
//    std::string fenCommand = "position fen " + board.generationFenString();
//    stockFish.sendStockFishCommand(fenCommand);                     // отправляю fen в StockFish
//    stockFish.sendStockFishCommand("go movetime 50 depth 5");       // Ограничения по времени и глубине
//    std::string stockFishOutput = stockFish.readStockFishOutput(800); // Чтение вывода Stockfish
//    std::cout << stockFishOutput <<std::endl;
//    // Перевод строки в координаты
//    oldCoord.y = static_cast<short int>('8' - stockFishOutput[1]);      // example msg = "e2e4"
//    oldCoord.x = static_cast<short int>(stockFishOutput[0] - 'a');
//    newCoord.y = static_cast<short int>('8' - stockFishOutput[3]);
//    newCoord.x = static_cast<short int>(stockFishOutput[2] - 'a');
}

// Ожидание нажатия ЛКМ
bool Game::waitClick() {
    while (window.isOpen()) {
        if (window.waitEvent(event)) {  // Ожидание события
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                return true;    // Если было нажатие ЛКМ - true
            }
        }
    }
    return false;
}


// Получить ход от игрока
void Game::movePlayer() {
    bool flHaveMove{false}; // Флаг был ли ход
    Coordinates coordinatesClick1{}, coordinatesClick2{};
    std::vector<Coordinates> availableCoordinates;

    while (!flHaveMove && this->waitClick()) {                  // Цикл пока не будет хода
        if (getCoordinatesPressedBox(coordinatesClick1) &&
            // Если нажатие было по позиции фигуры, которая сейчас ходит
            (board.map[coordinatesClick1.y][coordinatesClick1.x] != nullptr &&
             board.map[coordinatesClick1.y][coordinatesClick1.x]->isColorWhite() == board.itWhiteMoveNow())) {
            availableCoordinates = board.getMove(coordinatesClick1.y, coordinatesClick1.x);
            // Получить доступные координаты хода и вывести их
            this->drawGame();
            board.drawAvailableMove(window, availableCoordinates);
            window.display();
            // Ожидание второго нажатия, куда ходит фигура
            if (this->waitClick()) {
                if (getCoordinatesPressedBox(coordinatesClick2))
                    for (Coordinates coordinatesMove: availableCoordinates)
                        if (coordinatesClick2 ==
                            coordinatesMove) { // Если координаты нажатия равны доступным для хода координатам
                            flHaveMove = true;
                            board.move(coordinatesClick1, coordinatesClick2);   // Меняю состояние доски
                            // Проверка прохода пешки
                            if (board.map[coordinatesClick2.y][coordinatesClick2.x]->getType() == TypeFigure::Pawn &&
                                (coordinatesClick2.y == 0 || coordinatesClick2.y == 7))
                                board.pawnPromotion(window, coordinatesClick2);
                            break;
                        }
            }
        } else if (clickSidebar())  // Если было нажатие на боковую панель, выйти из цикла
            break;
        this->drawGame();
        window.display();
    }
}

// Считать игру из файла
bool Game::readFromFile(std::string &nameFile) {
    if (board.getStateGame() == StateGame::GameOn)  // Процесс игры
        nameFile = "SaveGames/UnfinishedGame/" + nameFile + ".txt";
    else nameFile = "SaveGames/FinishedGame/" + nameFile + ".txt";  // Процесс просмотра
    std::ifstream fileRead(nameFile);
    if (!fileRead.is_open()) {  // Если файл не открылся
        std::cout << "FILE NOT FOUND" << std::endl;
        return false;
    }
    std::string temp;
    // Добавить в дек историю партии
    while (std::getline(fileRead, temp)) {
        temp += '\n';
        board.addFenInDeq(temp);
    }
    fileRead.close();
    return true;
}

// Сохранение игры в файл
bool Game::writeInFile(std::string &nameFile) {
    if (board.getStateGame() == StateGame::GameOn)
        nameFile = "SaveGames/UnfinishedGame/" + nameFile + ".txt";
    else nameFile = "SaveGames/FinishedGame/" + nameFile + ".txt";
    std::ifstream fileRead(nameFile);   // Если файл уже существует
    if (fileRead.is_open()) {
        std::cout << "FILE EXIST, TRY AGAIN";
        return false;
    }
    std::ofstream fileWrite(nameFile);
    if (!fileWrite.is_open()) {
        throw (ExceptionFile("Error create file", -2));
    }
    board.writeDeqInFile(fileWrite);    // Запись истории партии в файл
    fileWrite.close();
    return true;
}

// Получить нажатие бокового меню
bool Game::clickSidebar() {
    int marginSidebarX = 960;       // Отступ слева по Х
    int marginSidebarYFirst = 266;  // Отступ справа по У для первого эл-та
    int sizeButtonY = 71;           // Размер кнопки У
    int sizeButtonX = 202;          // Размер кнопки Х
    int distanceButton = 100;       // Расстояние между кнопками
    if (event.mouseButton.x > marginSidebarX && event.mouseButton.x < marginSidebarX + sizeButtonX) {
        SidebarInGameProcess choose;
        for (int i = 0; i < 4; i++) {    // Получить тип нажатой кнопки
            if (event.mouseButton.y > marginSidebarYFirst + i * distanceButton &&
                event.mouseButton.y < marginSidebarYFirst + i * distanceButton + sizeButtonY) {
                choose = static_cast<SidebarInGameProcess>(i);
                break;
            }
        }
        switch (choose) {
            case SidebarInGameProcess::GiveUp:  // Сдаться
                if (this->confirmation()) {     // Запрашиваю подтверждение
                    board.giveUp();             // Сдаться
                    return true;
                }
                break;
            case SidebarInGameProcess::StepBack:  // Отмена хода
                this->stepBack();
                return true;
            case SidebarInGameProcess::Save:    // Сохранение игры
                this->saveOrReturnGame(true);
                return true;
            case SidebarInGameProcess::Exit:    // Выход из игры
                if (this->confirmation()) {
                    board.clearBoard();         // Очистка доски
                    board.setStateGame(StateGame::Exit);
                    return true;
                }
                break;
            default:
                break;
        }
    }
    return false;
}

// Конец игры
bool Game::endGame() {
    int marginSidebarX = 960;       // Отступ слева по Х
    int marginSidebarYFirst = 266;  // Отступ справа по У для первого эл-та
    int sizeButtonY = 71;           // Размер кнопки У
    int sizeButtonX = 202;          // Размер кнопки Х
    int distanceButton = 100;       // Расстояние между кнопками

    this->drawGame();
    window.display();
    while (this->waitClick())       // Ожидание нажатия ЛКМ
        // Получить кнопку, на которую нажали
        if (event.mouseButton.x > marginSidebarX &&
            event.mouseButton.x < marginSidebarX + sizeButtonX) {
            SidebarInGameProcess choose{-1};
            for (int i = 1; i < 4; i++) {
                if (event.mouseButton.y > marginSidebarYFirst + i * distanceButton &&
                    event.mouseButton.y < marginSidebarYFirst + i * distanceButton + sizeButtonY) {
                    choose = static_cast<SidebarInGameProcess>(i);
                    break;
                }
            }
            switch (choose) {
                case SidebarInGameProcess::StepBack:    // Отмена хода
                    board.setStateGame(StateGame::GameOn);
                    this->stepBack();               // Отмена хода
                    return false;
                case SidebarInGameProcess::Save:    // Сохранить игру
                    this->saveOrReturnGame(true);
                    return false;
                case SidebarInGameProcess::Exit:    // Выйти из игры
                    board.clearBoard();             // Очистка доски
                    board.setStateGame(StateGame::Exit);    // Изменение состояние игры
                    return true;
                default:
                    break;
            }
        }
    return true;
}

// Процесс просмотра сохраненной игры
void Game::processWatchGame() {
    std::vector<std::string> storyGame;
    board.setStateGame(StateGame::View);    // Установка режима просмотра
    if (!this->saveOrReturnGame(false))     // Получить имя файла и считать из него в дек
        return;
    while (board.deqFenSize())            // Пока дек не пустой извлекаю в вектор
        storyGame.push_back(board.extractFenOfBeginDeq());
    storyGame.push_back(board.generationFenString());
    int numberMove = 0;
    board.setBoardByFen(storyGame[numberMove]); // Устанавливаю доску по FEN

    this->drawGame();                 // Вывожу игровое поле
    window.display();
    while (this->waitClick()) {         // Ожидаю нажатие ЛКМ
        switch (clickSidebarForWatch()) {   // Получаю кнопку нажатия
            case SidebarForWatch::StepForward:      // Шаг вперед
                if (numberMove != storyGame.size() - 2)
                    board.setBoardByFen(storyGame[++numberMove]);   // Устанавливаю доску по FEN
                break;
            case SidebarForWatch::StepBack:     // Шаг назад
                if (numberMove != 0)
                    board.setBoardByFen(storyGame[--numberMove]);   // Устанавливаю доску по FEN
                break;
            case SidebarForWatch::Exit:   // Выход
                return;
            case SidebarForWatch::Default:  // Нажатие не на кнопку
                break;
        }
        this->drawGame();           // Отрисовка игрового поля
        window.display();
    }
}

// Нажатие бокового меню в режиме просмотра
SidebarForWatch Game::clickSidebarForWatch() const {
    int marginSidebarX = 960;       // Отступ слева по Х
    int marginSidebarYFirst = 366;  // Отступ справа по У для первого эл-та
    int sizeButtonY = 71;           // Размер кнопки У
    int sizeButtonX = 202;          // Размер кнопки Х
    int distanceButton = 100;       // Расстояние между кнопками
    // Определяю кнопку нажатия
    if (event.mouseButton.x > marginSidebarX && event.mouseButton.x < marginSidebarX + sizeButtonX) {
        SidebarForWatch choose;
        for (int i = 0; i < 3; i++) {
            if (event.mouseButton.y > marginSidebarYFirst + i * distanceButton &&
                event.mouseButton.y < marginSidebarYFirst + i * distanceButton + sizeButtonY) {
                choose = static_cast<SidebarForWatch>(i);
                break;
            }
        }
        // Возврат типа кнопки
        switch (choose) {
            case SidebarForWatch::StepForward:
                return SidebarForWatch::StepForward;
            case SidebarForWatch::StepBack:
                return SidebarForWatch::StepBack;
            case SidebarForWatch::Exit:
                return SidebarForWatch::Exit;
            case SidebarForWatch::Default:
                break;
        }
    }
    return SidebarForWatch::Default;
}

// Вывод стартового меню
void Game::drawStartMenu() {
    window.clear();     // Очистка экрана
    window.draw(texture.startMenu); // Вывод текстуры стартового меню
}

// Получить координаты нажатия стартового меню
SelectStartMenu Game::getCoordinatesClickStartMenu() const {
    int marginFirstColumnBlockX = 280;      // Отступ по Х для первого столбца кнопок
    int marginSecondColumnBlockX = 640;     // Отступ по Х для второго столбца кнопок
    int sizeBlockX = 285;                   // Размер блоков по Х
    int marginFirstLineBlockY = 351;        // Отступ первой линии блоков по У
    int marginSecondLineBlockY = 547;       // Отступ второй линии блоков по У
    int marginThirdLineBlockY = 692;        // Отступ третьей линии блоков по У
    int sizeFirstLineBlockY = 115;          // Размер первой линии блоков по У
    int sizeNextLineBlockY = 90;            // Размер второй и третьей линии блоков по У
    int marginStyleY = 37;
    int marginStyleX = 1031;                // Style dark or white
    int sizeStyleX = 91;
    int sizeStyleY = 38;
    // Диапазон первого столбца кнопок
    if (event.mouseButton.x > marginFirstColumnBlockX && event.mouseButton.x < marginFirstColumnBlockX + sizeBlockX) {
        if (event.mouseButton.y > marginFirstLineBlockY &&
            event.mouseButton.y < marginFirstLineBlockY + sizeFirstLineBlockY)
            return SelectStartMenu::NewGameWithFriend;
        else if (event.mouseButton.y > marginSecondLineBlockY &&
                 event.mouseButton.y < marginSecondLineBlockY + sizeNextLineBlockY)
            return SelectStartMenu::RestoreGameWithFriend;
        else if (event.mouseButton.y > marginThirdLineBlockY &&
                 event.mouseButton.y < marginThirdLineBlockY + sizeNextLineBlockY)
            return SelectStartMenu::ViewingCompletedGames;
    }   // Диапазон второго столбца кнопок
    else if (event.mouseButton.x > marginSecondColumnBlockX &&
             event.mouseButton.x < marginSecondColumnBlockX + sizeBlockX) {
        if (event.mouseButton.y > marginFirstLineBlockY &&
            event.mouseButton.y < marginFirstLineBlockY + sizeFirstLineBlockY)
            return SelectStartMenu::NewGameWithComputer;
        else if (event.mouseButton.y > marginSecondLineBlockY &&
                 event.mouseButton.y < marginSecondLineBlockY + sizeNextLineBlockY)
            return SelectStartMenu::RestoreGameWithComputer;
        else if (event.mouseButton.y > marginThirdLineBlockY &&
                 event.mouseButton.y < marginThirdLineBlockY + sizeNextLineBlockY)
            return SelectStartMenu::Exit;
    } else if (event.mouseButton.y > marginStyleY && event.mouseButton.y < marginStyleY + sizeStyleY &&
               event.mouseButton.x > marginStyleX && event.mouseButton.x < marginStyleX + sizeStyleX)
        return SelectStartMenu::ChangeStyle;
    return SelectStartMenu::Default;    // Нажатие не на кнопку
}

// Отрисовка процесса игры
void Game::drawGame() {
    window.clear();                     // Очистка окна
    board.drawBoard(window);            // Отрисовка доски и фигур
    switch (board.getStateGame()) {      // Получить состояние игры
        case StateGame::GameOn:
            window.draw(texture.sidebarInProcessGame);  // Вывод текстуры процесса игры
            break;
        case StateGame::Player1Win:     // В зависимости от соперника вывожу соответствующую надпись
            if (opponentComputer) texture.resultGame.setTexture(texture.playerWinImg);
            else texture.resultGame.setTexture(texture.player1WinImg);
            window.draw(texture.sidebarEndGame);
            window.draw(texture.resultGame);
            break;
        case StateGame::Player2Win:     // В зависимости от соперника вывожу соответствующую надпись
            if (opponentComputer) texture.resultGame.setTexture(texture.computerWinImg);
            else texture.resultGame.setTexture(texture.player2WinImg);
            window.draw(texture.sidebarEndGame);
            window.draw(texture.resultGame);
            break;
        case StateGame::Draw:           // Ничья
            texture.resultGame.setTexture(texture.drawImg);
            window.draw(texture.sidebarEndGame);
            window.draw(texture.resultGame);
            break;
        case StateGame::View:       // Вывожу боковое меню режима просмотра
            window.draw(texture.sidebarInProcessWatch);
            return;
        case StateGame::Exit:
            return;
    }
    window.draw(texture.player1Name);
    window.draw(texture.player2Name);
}

// Сохранение или восстановление игры
bool Game::saveOrReturnGame(bool saveGame) {
    // saveGame == true - сохранить игру, иначе восстановить
    if (saveGame) this->drawGame();
    else this->drawStartMenu();
    window.draw(texture.inputFileName);     // Установить текстуру ввода файла
    window.display();
    sf::String playerInput;                 // Отображение вводимых символов
    std::string temp;                       // Передача для записи
    texture.playerText.setString(playerInput);

    while (window.isOpen()) {
        switch (this->clickToInputString()) {          // Получить тип события
            case ClickToSaveOrReturnFile::InputText:
                if (playerInput.getSize() < 20)    // Если размер меньше 20 ввожу символ
                    playerInput += event.text.unicode;
                break;
            case ClickToSaveOrReturnFile::RemoveSymbol:
                if (playerInput.getSize() != 0)     // Если размер не 0, то удаляю элемент
                    playerInput.erase(playerInput.getSize() - 1, 1);
                break;
            case ClickToSaveOrReturnFile::Save:
                temp = playerInput;                 // перевод в string
                try {
                    if (saveGame && this->writeInFile(temp)) return true;
                    else if (!saveGame && this->readFromFile(temp)) return true;
                    else break;
                }
                catch (const ExceptionFile &err) {
                    std::cout << err.what() << std::endl;
                    std::cout << "Code error: " << err.code() << std::endl;
                    break;
                }
            case ClickToSaveOrReturnFile::Exit:
                return false;
            case ClickToSaveOrReturnFile::Default:
                break;
        }
        if (saveGame) this->drawGame();
        else this->drawStartMenu();
        window.draw(texture.inputFileName); // Вывожу текстуру ввода файла
        texture.playerText.setString(playerInput);  // Вывожу введенную строку
        window.draw(texture.playerText);
        window.display();
    }
    return false;
}

// Ввод файла
ClickToSaveOrReturnFile Game::clickToInputString() {
    int sizeSaveOrExitX = 80;   // Размер кнопок "save" and "exit" по Х
    int sizeSaveOrExitY = 35;   // Размер кнопок по У
    int marginSaveX = 482;      // Отступ кнопки "save" по Х
    int marginExitX = 637;      // Отступ кнопки "exit" по Х
    int marginSaveOrExitY = 534;        // Отступ кнопок по У
    while (window.waitEvent(event)) {   // Ожидание события
        if (event.type == sf::Event::TextEntered) {     // Нажатие клавиши
            // Проверка диапазон нажатой клавиши
            if ((event.text.unicode >= '0' && event.text.unicode <= '9') ||
                (event.text.unicode >= 'A' && event.text.unicode <= 'Z') ||
                (event.text.unicode >= 'a' && event.text.unicode <= 'z') ||
                event.text.unicode == '.')
                return ClickToSaveOrReturnFile::InputText;
                // Удаление символа
            else if (event.text.unicode == '\b') return ClickToSaveOrReturnFile::RemoveSymbol;
        } else if (event.type == sf::Event::Closed) {
            window.close();     // Закрытие окна
            return ClickToSaveOrReturnFile::Exit;
        } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (event.mouseButton.y > marginSaveOrExitY && event.mouseButton.y < marginSaveOrExitY + sizeSaveOrExitY) {
                if (event.mouseButton.x > marginSaveX && event.mouseButton.x < marginSaveX + sizeSaveOrExitX)
                    return ClickToSaveOrReturnFile::Save;       // save
                else if (event.mouseButton.x > marginExitX &&
                         event.mouseButton.x < marginExitX + sizeSaveOrExitX)       // Exit
                    return ClickToSaveOrReturnFile::Exit;
            }
        }
    }
    return ClickToSaveOrReturnFile::Default;
}

// Запрос подтверждения на действие
bool Game::confirmation() {
    this->drawGame();               // Вывод игровой доски
    int marginFirstBlockX = 505;    // Отступ кнопки "yes" по Х
    int marginSecondBlockX = 625;   // Отступ кнопки "no" по Х
    int marginBlockY = 520;         // Отступ по У
    int sizeBlockX = 70;            // Размер кнопки по Х
    int sizeBlockY = 35;            // Размер кнопки по У
    window.draw(texture.confirmation);
    window.display();
    while (this->waitClick()) {      // Ожидание нажатия ЛКМ
        if ((event.mouseButton.y > marginBlockY) &&
            (event.mouseButton.y < marginBlockY + sizeBlockY)) {
            if ((event.mouseButton.x > marginFirstBlockX) &&          // yes
                (event.mouseButton.x < marginFirstBlockX + sizeBlockX))
                return true;
            else if ((event.mouseButton.x > marginSecondBlockX) &&          // No
                     (event.mouseButton.x < marginSecondBlockX + sizeBlockX))
                return false;
        }
    }
    return false;
}

// Восстановить игру с компьютером
void Game::restoreGameWithComputer() {
//    // Установка состояния игры, считывание истории игры в дек, установка текстуры и соперника
//    board.setStateGame(StateGame::GameOn);
//    if (this->saveOrReturnGame(false)) {
//        // Установка Игроков
//        this->texture.player1Name.setString("Player");
//        this->texture.player2Name.setString("Computer");
//        this->texture.player1Name.setPosition(1020,795);    // Установка координат вывода слова
//        this->texture.player2Name.setPosition(1010,93);
//        opponentComputer = true;
//        try {
//            if (!stockFish.startStockFish())            // Запуск Stockfish
//                throw (ExceptionStockFish("Error start StockFish", -2));
//            stockFish.sendStockFishCommand("uci");      // Отправка стартовой команды
//            int difficultyBot = 1;
//            setDifficultyBot(difficultyBot);            // Установка сложности игры
//            board.setBoardByFen(board.extractFenOfEndDeq());    // Установка доски по FEN
//            processGameWithComputer();                  // Запуск игрового цикла
//        }
//        catch (ExceptionStockFish& err){
//            std::cout << err.what() << std::endl;
//            std::cout << "Code error: " << err.code() << std::endl;
//            return;
//        }
//        stockFish.closeStockFish();
//    }
}

// Восстановить игру с игроком
void Game::restoreGameWithFriend() {
    // Установка Игроков и состояния игры на Gameon
    this->texture.player1Name.setString("Player 1");
    this->texture.player2Name.setString("Player 2");
    this->texture.player1Name.setPosition(1020, 795);    // Установка координат вывода слова
    this->texture.player2Name.setPosition(1020, 93);
    board.setStateGame(StateGame::GameOn);
    opponentComputer = false;
    // Считывание истории игры из файла в дек, установка доски, запуск игрвого цикла
    if (this->saveOrReturnGame(false)) {
        board.setBoardByFen(board.extractFenOfEndDeq());
        processGameWithFriend();
    }
}

Game::~Game() {
    stockFish.~StockFish(); // Закрываю Stockfish
    if (window.isOpen())
        window.close();     // Закрываю окно
}

// Отмена хода
void Game::stepBack() {
    if (opponentComputer)   // Если игра против компьютера
        board.stepBack();   // Отменяю два хода
    board.stepBack();
}

void Game::changeStyle() {
    texture.changeStyle();
    board.texture.changeStyle();
}










void Game::newOnlineGame() {
    NetworkClient typeClient;
    typeClient = this->selectHost();
//    if (typeClient == NetworkClient::Host)     // true - host
//        network.start(NetworkClient::Host);
//    else network.start(NetworkClient::Client);
    if (typeClient == NetworkClient::Host)
        std::cout << "Choose: host" << std::endl;
    else
        std::cout << "Choose: host" << std::endl;
    this->startNetwork(typeClient);  // new thread
}

NetworkClient Game::selectHost() {
    this->drawStartMenu();
    int marginFirstBlockX = 505;    // Отступ кнопки "yes" по Х
    int marginSecondBlockX = 625;   // Отступ кнопки "no" по Х
    int marginBlockY = 520;         // Отступ по У
    int sizeBlockX = 70;            // Размер кнопки по Х
    int sizeBlockY = 35;            // Размер кнопки по У
    window.draw(texture.chooseHostOrClient);
    window.display();
    while (this->waitClick()) {      // Ожидание нажатия ЛКМ
        if ((event.mouseButton.y > marginBlockY) &&
            (event.mouseButton.y < marginBlockY + sizeBlockY)) {
            if ((event.mouseButton.x > marginFirstBlockX) &&          // yes
                (event.mouseButton.x < marginFirstBlockX + sizeBlockX))
                return NetworkClient::Host;
            else if ((event.mouseButton.x > marginSecondBlockX) &&          // No
                     (event.mouseButton.x < marginSecondBlockX + sizeBlockX))
                return NetworkClient::Client;
        }
    }
    return NetworkClient::Host; /////////////////////////
}


void *Game::establishConnectionHost(int* serverSocket, int *result) {
    sockaddr_in serveAddress{}, clientAddress{};
    int addrLen = sizeof(serveAddress);
    getIpAddress();

    // Создание сокета
    if ((*serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        mutex.lock();
        *result = 2;
        mutex.unlock();
    }
    int optval = 1;
    if (setsockopt(*serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        std::cerr << "Error: Could not set SO_REUSEADDR option." << std::endl;
    }
    // Настройка сервера
    serveAddress.sin_family = AF_INET;
    serveAddress.sin_addr.s_addr = INADDR_ANY;
    serveAddress.sin_port = htons(PORT);  // 9999
    // Привязка сокета к адресу и порту
    if (bind(*serverSocket, (struct sockaddr *) &serveAddress, sizeof(serveAddress)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        mutex.lock();
        *result = 2;
        mutex.unlock();
    }
    // Слушаем входящие соединения
    if (listen(*serverSocket, 1) < 0) {
        std::cerr << "Listen failed" << std::endl;
        mutex.lock();
        *result = 2;
        mutex.unlock();

    }
    std::cout << "Server started, listening on port " << PORT << std::endl;


    int flags = fcntl(*serverSocket, F_GETFL, 0);
    fcntl(*serverSocket, F_SETFL, flags | O_NONBLOCK);
    // Принимаем входящее соединение
    while ((clientSocket = accept(*serverSocket, (struct sockaddr *) &clientAddress, (socklen_t *) &addrLen)) == -1) {
        if (errno == EBADF) {
            mutex.lock();
            *result = 2;
            mutex.unlock();
            pthread_exit(nullptr);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }

    close(*serverSocket);
    mutex.lock();
    *result = 1;
    mutex.unlock();
    pthread_exit(nullptr);
}

bool Game::establishConnectionClient() {
    sockaddr_in serverAddress{};
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return false;
    }
    int optVal = 1;
    setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    // Настройка сервера
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    // input IP
    return this->inputServerIp(serverAddress);

}

std::string Game::getIpAddress() {
    std::string ipAddress;
    system("hostname -I > tmp.txt");
    std::ifstream file("tmp.txt");
    std::getline(file, ipAddress);
    file.close();
    remove("tmp.txt");
    return ipAddress;
}

bool Game::waitConnect(const int *result) {
    this->drawStartMenu();               // Вывод игровой доски
    int marginButtonY = 520;         // Отступ по У
    int marginButtonX = 540;         // Отступ по У
    int sizeButtonX = 120;            // Размер кнопки по Х
    int sizeButtonY = 35;            // Размер кнопки по У
    std::string myIpAddress = this->getIpAddress();
    texture.ipAddress.setString("Your IP: " + myIpAddress);
    texture.ipAddress.setPosition(475,465);
    window.draw(texture.waitConnection);
    window.draw(texture.ipAddress);
    window.display();
    while (true) {
        mutex.lock();
        if (*result == 1) {
            mutex.unlock();
            return true;
        }
        if (*result == 2) {     // error
            mutex.unlock();
            std::cout << "EXIT";
            return false;
        }
        mutex.unlock();
        if (window.pollEvent(event) && event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left &&
            (event.mouseButton.y > marginButtonY && event.mouseButton.y < marginButtonY + sizeButtonY) &&
            (event.mouseButton.x > marginButtonX && event.mouseButton.x < marginButtonX + sizeButtonX)) {
            std::cout << "exitWait";
            fflush(stdout);
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


void Game::startNetwork(NetworkClient typeClient) {
    int result = 0;     // 1 - success / 2 - fail
    if (typeClient == NetworkClient::Host) {
        int serverSocket;
        threadForNetwork = std::thread(&Game::establishConnectionHost, this, &serverSocket, &result);
        if (!this->waitConnect(&result)) {
            std::cout << "Close";
            fflush(stdout);
            close(serverSocket);
            if (threadForNetwork.joinable()) {
                std::cout << "Can join";
                fflush(stdout);
                threadForNetwork.join();
                std::cout << "join";
                fflush(stdout);
            }
            return;
        }
        threadForNetwork.join();
    } else if (typeClient == NetworkClient::Client) {
        if (!this->establishConnectionClient())
            return;
    }

    board.setStateGame(StateGame::GameOn);
    opponentComputer = false;                 // false - игра против друга; true - игра против компьютера
    board.setBoardByFen(board.getStartFEN());
    std::atomic<bool> newMsg = false;
    threadForNetwork = std::thread(&Game::waitOpponentAction, this, &newMsg);
    std::cout << "REad thREAD READY";
    fflush(stdout);
    processNetworkGame(typeClient, &newMsg);
    threadForNetwork.join();
    std::cout << "I end";
}
bool Game::waitClickWithRefresh() {
    while (window.isOpen()) {
        if (window.pollEvent(event)) {  // Ожидание события
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                return true;    // Если было нажатие ЛКМ - true
            }
        }
        this->drawGame();
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return false;
}

void Game::processNetworkGame(NetworkClient typeClient, std::atomic<bool>* newMsg) {
    while (window.isOpen() && board.getStateGame() != StateGame::Exit) {
        this->drawGame();
        window.display();
        board.checkOnMate();
        newMsg->store(false);
        if (newMsg->load()) {
            newMsg->store(false);
            continue;
        }
        playerAction(typeClient, newMsg);
    }
}


void Game::playerAction(NetworkClient typeClient, std::atomic<bool>* newMsg) {
    bool flHaveMove{false}; // Флаг был ли ход
    Coordinates coordinatesClick1{}, coordinatesClick2{};
    std::vector<Coordinates> availableCoordinates;
    while (!flHaveMove && this->waitClickWithRefresh()) {                  // Цикл пока не будет хода
        if (newMsg->load()) {
            newMsg->store(false);
            return;
        }
//        mutex.lock();
        if (getCoordinatesPressedBox(coordinatesClick1) &&
            (typeClient == NetworkClient::Host && board.itWhiteMoveNow() ||
             typeClient == NetworkClient::Client && !board.itWhiteMoveNow()) &&
            // Если нажатие было по позиции фигуры, которая сейчас ходит
            (board.map[coordinatesClick1.y][coordinatesClick1.x] != nullptr &&
             board.map[coordinatesClick1.y][coordinatesClick1.x]->isColorWhite() == board.itWhiteMoveNow())) {
            availableCoordinates = board.getMove(coordinatesClick1.y, coordinatesClick1.x);
            // Получить доступные координаты хода и вывести их
            this->drawGame();
            board.drawAvailableMove(window, availableCoordinates);
            window.display();
            // Ожидание второго нажатия, куда ходит фигура
            if (this->waitClick()) {
                if (getCoordinatesPressedBox(coordinatesClick2))
                    for (Coordinates coordinatesMove: availableCoordinates)
                        if (coordinatesClick2 ==
                            coordinatesMove) { // Если координаты нажатия равны доступным для хода координатам
                            flHaveMove = true;
                            board.move(coordinatesClick1, coordinatesClick2);   // Меняю состояние доски
                            // Проверка прохода пешки
                            if (board.map[coordinatesClick2.y][coordinatesClick2.x]->getType() ==
                                TypeFigure::Pawn &&
                                (coordinatesClick2.y == 0 || coordinatesClick2.y == 7)) {
                                board.pawnPromotion(window, coordinatesClick2);
                            }
                            send(clientSocket, board.generationFenString().c_str(), 100, 0);
                            break;
                        }
            }
        } else if (clickSidebar())  // Если было нажатие на боковую панель, выйти из цикла
            break;
        this->drawGame();
        window.display();
    }
}

void *Game::waitOpponentAction(std::atomic<bool>* newMsg) {
    char msg[100];
    while (1){
        recv(clientSocket, msg, 100, 0);
        for(char i : msg){
            if (i == '/') {
                mutex.lock();
                board.moveTransition();
                board.setBoardByFen(msg);
                mutex.unlock();
                break;
            }
        }
        newMsg->store(true);
    }
}

bool Game::inputServerIp(sockaddr_in serverAddress) {
    this->drawStartMenu();
    window.draw(texture.inputIp);     // Установить текстуру ввода файла
    window.display();
    sf::String serverIP;                 // Отображение вводимых символов
    texture.ipAddress.setString(serverIP);
    texture.ipAddress.setPosition(465,458);

    while (window.isOpen()) {
        switch (this->clickToInputString()) {          // Получить тип события
            case ClickToSaveOrReturnFile::InputText:
                if (serverIP.getSize() < 20)    // Если размер меньше 20 ввожу символ
                    serverIP += event.text.unicode;
                break;
            case ClickToSaveOrReturnFile::RemoveSymbol:
                if (serverIP.getSize() != 0)     // Если размер не 0, то удаляю элемент
                    serverIP.erase(serverIP.getSize() - 1, 1);
                break;
            case ClickToSaveOrReturnFile::Save:
                if (inet_pton(AF_INET, serverIP.toAnsiString().c_str(), &serverAddress.sin_addr) <= 0) {
                    std::cerr << "Invalid address/ Address not supported" << std::endl;
                    break;
                }
                // Подключение к серверу
                if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
                    std::cerr << "Connection failed" << std::endl;
                    break;
                }
                return true;
            case ClickToSaveOrReturnFile::Exit:
                return false;
            case ClickToSaveOrReturnFile::Default:
                break;
        }
        this->drawStartMenu();
        window.draw(texture.inputIp); // Вывожу текстуру ввода файла
        texture.ipAddress.setString(serverIP);  // Вывожу введенную строку
        window.draw(texture.ipAddress);
        window.display();
    }
    return false;
}


