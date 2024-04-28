//
// Created by bobr on 28/04/24.
//

#ifndef COURSEWORK_NETWORK_H
#define COURSEWORK_NETWORK_H
#include "Header.h"

class Network {
private:
    NetworkClient typeClient;
    int clientSocket{};

public:
    void setTypeClient();
    NetworkClient getTypeClient();
};


#endif //COURSEWORK_NETWORK_H
