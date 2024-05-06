//
// Created by vorop on 02.12.2023.
//

#ifndef COURSEWORK_STOCKFISH_H
#define COURSEWORK_STOCKFISH_H

//#include <Windows.h>
#include <mutex>
#include "iostream"
#include "chrono"
#include "vector"
//class StockFish{};

class StockFish {                   // Класс взаимодействия с "Stockfish"
private:
//    std::mutex mutex;
//    std::thread threadForNetwork;
    std::string sendMsg;
    std::string readMsg;
    int stdinWrite;
    int stdoutRead;
public:
    StockFish()= default;
    bool startStockFish();          // Создание потока и запуск Stockfish
    void sendStockFishCommand(std::string msg) const;
    std::string readStockFishOutput() const;
    void closeStockFish() const;          // Закрыть поток
    ~StockFish();                   // деструктор закрывает поток
};


#endif //COURSEWORK_STOCKFISH_H
