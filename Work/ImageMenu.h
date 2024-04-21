//
// Created by vorop on 09.12.2023.
//

#ifndef COURSEWORK_IMAGEMENU_H
#define COURSEWORK_IMAGEMENU_H
#include <SFML/Graphics.hpp>

class ImageMenu{                // Класс изображений меню
private:
    sf::Texture startMenuImg, sidebarInProcessWatchImg, confirmationImg, inputFileNameImg, sidebarInProcessGameImg,
    sidebarEndGameImg, chooseHostOrClientImg, waitConnectionImg, inputIpImg; // текстуры
    bool blackStyle;
public:
    sf::Texture player1WinImg,player2WinImg,playerWinImg,computerWinImg,drawImg;
    sf::Font fontInput,fontPlayer;                                                                      // Шрифт
    sf::Image icon;                                                                     // Иконка
    sf::Text playerText, player1Name,player2Name, ipAddress;                                       // Текст
    sf::Sprite startMenu, sidebarInProcessGame, sidebarInProcessWatch,                    // Спрайты
    confirmation, inputFileName, sidebarEndGame, resultGame ,  chooseHostOrClient, waitConnection, inputIp;

    ImageMenu(){                                       // Загрузка, установка изображений и координат
        blackStyle = true;
        icon.loadFromFile("image/icon.png");
        fontInput.loadFromFile("image/font/arial_bolditalicmt.ttf");
        fontPlayer.loadFromFile("image/font/ofont.ru_Open Sans.ttf");

        playerText.setFont(fontInput);
        playerText.setCharacterSize(24);
        playerText.setFillColor(sf::Color::Black);

        ipAddress.setFont(fontInput);
        ipAddress.setCharacterSize(23);
        ipAddress.setFillColor(sf::Color::Black);

        player1Name.setFont(fontPlayer);
        player1Name.setCharacterSize(23);
        player1Name.setFillColor(sf::Color::White);

        player2Name.setFont(fontPlayer);
        player2Name.setCharacterSize(23);
        player2Name.setFillColor(sf::Color::Black);

        startMenuImg.loadFromFile("image/startMenuDarkStyle.png");

        sidebarInProcessGameImg.loadFromFile("image/sidebarInProcessGameDarkStyle.png");
        sidebarEndGameImg.loadFromFile("image/sidebarEndGameDarkStyle.png");
        sidebarInProcessWatchImg.loadFromFile("image/sidebarInProcessWatchDarkStyle.png");
        confirmationImg.loadFromFile("image/confirmationDarkStyle.png");
        inputFileNameImg.loadFromFile("image/inputFileNameDarkStyle.png");
        player1WinImg.loadFromFile("image/winPlayer1.png");
        player2WinImg.loadFromFile("image/winPlayer2.png");
        computerWinImg.loadFromFile("image/winComputer.png");
        playerWinImg.loadFromFile("image/winPlayer.png");
        drawImg.loadFromFile("image/draw.png");

        chooseHostOrClientImg.loadFromFile("image/chooseHostOrClient.png");
        waitConnectionImg.loadFromFile("image/waitConnection.png");
        inputIpImg.loadFromFile("image/inputIp.png");


        startMenu.setTexture(startMenuImg);
        sidebarInProcessWatch.setTexture(sidebarInProcessWatchImg);
        confirmation.setTexture(confirmationImg);
        inputFileName.setTexture(inputFileNameImg);
        sidebarInProcessGame.setTexture(sidebarInProcessGameImg);
        sidebarEndGame.setTexture(sidebarEndGameImg);

        chooseHostOrClient.setTexture(chooseHostOrClientImg);
        waitConnection.setTexture(waitConnectionImg);
        inputIp.setTexture(inputIpImg);

        startMenu.setPosition(-4,0);
        sidebarInProcessWatch.setPosition(920,0);
        sidebarEndGame.setPosition(920, 0);
        sidebarInProcessGame.setPosition(920, 0);
        confirmation.setPosition(0,0);
        inputFileName.setPosition(0,0);
        chooseHostOrClient.setPosition(0,0);
        waitConnection.setPosition(0,0);
        inputIp.setPosition(0,0);

        playerText.setPosition(465,458);
        ipAddress.setPosition(465,458);

        resultGame.setPosition(920,200);
    }

    void changeStyle(){
        if (blackStyle){
            startMenuImg.loadFromFile("image/startMenuWhiteStyle.png");
            sidebarInProcessGameImg.loadFromFile("image/sidebarInProcessGameWhiteStyle.png");
            sidebarEndGameImg.loadFromFile("image/sidebarEndGameWhiteStyle.png");
            sidebarInProcessWatchImg.loadFromFile("image/sidebarInProcessWatchWhiteStyle.png");
            confirmationImg.loadFromFile("image/confirmationWhiteStyle.png");
            inputFileNameImg.loadFromFile("image/inputFileNameWhiteStyle.png");
        }
        else{
            startMenuImg.loadFromFile("image/startMenuDarkStyle.png");
            sidebarInProcessGameImg.loadFromFile("image/sidebarInProcessGameDarkStyle.png");
            sidebarEndGameImg.loadFromFile("image/sidebarEndGameDarkStyle.png");
            sidebarInProcessWatchImg.loadFromFile("image/sidebarInProcessWatchDarkStyle.png");
            confirmationImg.loadFromFile("image/confirmationDarkStyle.png");
            inputFileNameImg.loadFromFile("image/inputFileNameDarkStyle.png");
        }
        blackStyle = !blackStyle;
    }
};

#endif //COURSEWORK_IMAGEMENU_H
