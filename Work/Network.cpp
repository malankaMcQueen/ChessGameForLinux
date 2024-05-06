//
// Created by bobr on 28/04/24.
//


#include "Network.h"
#define PORT 9999
#include <fcntl.h>
#include <thread>


void *Network::establishConnectionHost(int& serverSocket,std::mutex& mutex, int& result) {
    sockaddr_in serveAddress{}, clientAddress{};
    int addrLen = sizeof(serveAddress);
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        mutex.lock();
        result = 2;
        mutex.unlock();
    }
    int optVal = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)) < 0) {
        std::cerr << "Error: Could not set SO_REUSEADDR option." << std::endl;
    }
    serveAddress.sin_family = AF_INET;
    serveAddress.sin_addr.s_addr = INADDR_ANY;
    serveAddress.sin_port = htons(PORT);  // 9999
    // Привязка сокета к адресу и порту
    if (bind(serverSocket, (struct sockaddr *) &serveAddress, sizeof(serveAddress)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        mutex.lock();
        result = 2;
        mutex.unlock();
    }
    // Слушаем входящие соединения
    if (listen(serverSocket, 1) < 0) {
        std::cerr << "Listen failed" << std::endl;
        mutex.lock();
        result = 2;
        mutex.unlock();
    }
    int flags = fcntl(serverSocket, F_GETFL, 0);
    fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);
    // Принимаем входящее соединение
    while ((this->clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, (socklen_t *) &addrLen)) == -1) {
        if (errno == EBADF) {
            mutex.lock();
            result = 2;
            mutex.unlock();
            pthread_exit(nullptr);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    close(serverSocket);
    mutex.lock();
    result = 1;
    mutex.unlock();
    pthread_exit(nullptr);
}

std::string Network::getIpAddress() {
    std::string ipAddress;
    system("hostname -I > tmp.txt");
    std::ifstream file("tmp.txt");
    std::getline(file, ipAddress);
    file.close();
    std::cout << ipAddress << std::endl << std::endl;
    std::string firstIpAddress;
    for(int i = 0; i < ipAddress.length(); i++)
        if (ipAddress[i] == ' ') {
            firstIpAddress = ipAddress.substr(0, i);
            break;
        }
    remove("tmp.txt");
    return firstIpAddress;
}

bool Network::establishConnectionClient(const std::string& ipAddress) {
    sockaddr_in serverAddress{};
    std::cout << ipAddress;
    fflush(stdout);
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return false;
    }
    int optVal = 1;
    setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    // input IP

    if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddress.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return false;
    }
    // Подключение к серверу
    if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return false;
    }
    std::cout <<"True";
    fflush(stdout);
    return true;
}

