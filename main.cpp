#include <iostream>
using namespace std;

#include <QCoreApplication>
#include <QTimer>
#include <stdio.h>
#include <hiredis.h>
#include <WinSock2.h>
#include "client-qt.h"

#pragma comment(lib, "Ws2_32.lib")
int main (int argc, char **argv) {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    QCoreApplication app(argc, argv);


    ClientQt client;
    client.connect("127.0.0.1");
    QTimer timer;
    timer.start(100);
    QObject::connect(&timer,&QTimer::timeout,&client,&ClientQt::run);

    QObject::connect(&client, &ClientQt::finished, &timer,&QTimer::stop);
    return app.exec();
}
