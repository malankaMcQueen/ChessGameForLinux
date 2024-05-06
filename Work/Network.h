//
// Created by bobr on 28/04/24.
//

#ifndef COURSEWORK_NETWORK_H
#define COURSEWORK_NETWORK_H
#include "Header.h"
#include <fstream>
#include <condition_variable>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "pthread.h"
#include <csignal>
#include <atomic>
#include <mutex>



class Network {
private:
    NetworkClient typeClient{};
    int clientSocket{};
//    std::mutex mutexSocket;
public:
    void setTypeClient(NetworkClient client) {  typeClient = client;    }
//    void lockSocketMutex() {     mutexSocket.lock();     }
//    void unlockSocketMutex() {   mutexSocket.unlock();    }
    NetworkClient getTypeClient() const { return typeClient;      }
    int getClientSocket() const {     return clientSocket;     }

    static std::string getIpAddress();
    void* establishConnectionHost(int& serverSocket, std::mutex& mutex, int& result);

    bool establishConnectionClient(const std::string& ipAddress);
};


#endif //COURSEWORK_NETWORK_H
