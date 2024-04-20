//
// Created by vorop on 02.12.2023.
//
#include <thread>
#include "StockFish.h"
#include "Exceptions.h"

//bool StockFish::startStockFish() {
//    // Обнуление памяти
//    ZeroMemory(&si, sizeof(si));
//    si.cb = sizeof(si);
//    ZeroMemory(&pi, sizeof(pi));
//    // Установка необходимых параметров
//    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
//    saAttr.bInheritHandle=TRUE;
//    saAttr.lpSecurityDescriptor=nullptr;
//    // Создание канала для стандартного ввода
//    if (!CreatePipe(&hStdinRead, &hStdinWrite, &saAttr, 0)) {
//        std::cout << "CreatePipeInp failed (error code:" << GetLastError() << ")" << std::endl;
//        return false;
//    }
//    // Создание канала для стандартного вывода
//    if (!CreatePipe(&hStdoutRead, &hStdoutWrite, &saAttr, 0)) {
//        std::cout << "CreatePipeOut failed (error code:" << GetLastError() << ")" << std::endl;
//        return false;
//    }
//    // Создание канала для стандартного вывода ошибки
//    if(!CreatePipe(&hStderrRead,&hStderrWrite,&saAttr,0))
//    {
//        std::cout << "CreatePipeErr failed (error code:" << GetLastError() << ")" << std::endl;
//        return false;
//    }
//    // Задание дескрипторов файловых потоков для стандартного ввода, вывода и ошибок нового процесса.
//    si.hStdOutput = hStdoutWrite;       // Мне выводится, то что пишет тот поток
//    si.hStdInput = hStdinRead;          // я ввожу туда, где читает тот поток
//    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
//    si.dwFlags |= STARTF_USESTDHANDLES;
//    // Запуск процесса
//    if (!CreateProcessA
//            (
//                    "stockfish/stockfish-windows-x86-64-avx2.exe",   // the path
//                    nullptr,                       // Command line
//                    nullptr,                   // Process handle not inheritable
//                    nullptr,                   // Thread handle not inheritable
//                    TRUE,                  // Set handle inheritance to FALSE
//                    CREATE_NO_WINDOW,             // Opens file in a separate console
//                    nullptr,           // Use parent's environment block
//                    nullptr,           // Use parent's starting directory
//                    &si,            // Pointer to STARTUPINFO structure
//                    &pi           // Pointer to PROCESS_INFORMATION structure
//            )) {
//        std::cout << "CreateProcess failed" << GetLastError() << std::endl;
//        return false;
//    }
//    return true;
//}
//// Завершение работы Stockfish и освобождение занимаемых ресурсов
//void StockFish::closeStockFish() {
//    if(WriteFile(hStdinWrite, "quit\n", 5,nullptr, nullptr))
//        std::cout << "StockFish End" << std::endl;
//    if(hStdinRead != nullptr) CloseHandle(hStdinRead);
//    if(hStdinWrite != nullptr) CloseHandle(hStdinWrite);
//    if(hStdoutRead != nullptr) CloseHandle(hStdoutRead);
//    if(hStdoutWrite != nullptr) CloseHandle(hStdoutWrite);
//    if(hStderrRead != nullptr) CloseHandle(hStderrRead);
//    if(hStderrWrite != nullptr) CloseHandle(hStderrWrite);
//    if(pi.hProcess != nullptr) CloseHandle(pi.hProcess);
//    if(pi.hThread != nullptr) CloseHandle(pi.hThread);
//}
//
//void StockFish::sendStockFishCommand(std::string msg) {
//    msg += '\n';
//    DWORD bytesWritten;
//    if ( WriteFile(hStdinWrite,msg.c_str(),msg.length(),&bytesWritten,nullptr))
//        std::cout << "msg: " << msg;
//    else throw(Exception("Error send command StockFish", -4));
//}
//
//
//// Чтение вывода Stockfish
//std::string StockFish::readStockFishOutput(DWORD delayMillis) {
//    // Время ожидания, пауза
//    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis));
//    DWORD bytesRead = 0;
//    char buffer[2048];
//    std::string str;
//    // Чтение всего вывода в str
//    do {
//        if (!ReadFile(hStdoutRead, buffer, sizeof(buffer), &bytesRead, nullptr))
//            throw (ExceptionStockFish("Error read StockFish output", -3));
//        str += buffer;
//    } while (sizeof(buffer)  == bytesRead);
//    // Поиска подстроки bestmove
//    int n = static_cast<int>(str.find("bestmove"));
//    if (n == -1)    // Если подстрока не найдена
//        throw (ExceptionStockFish("Error read StockFish output", -3));
//    return str.substr(n+9,4); // Извлечение данных о ходе (e4e6)
//}
//// Деструктор
//StockFish::~StockFish() {
//    closeStockFish();
//}
