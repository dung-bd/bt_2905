#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

void handleClient(SOCKET clientSocket);

int main() {
    WSADATA wsaData;
    SOCKET listenSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Khong the khoi tao Winsock");
        return 1;
    }

    if ((listenSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Khong the tao socket");
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Khong the lien ket socket");
        closesocket(listenSocket);
        return 1;
    }

    if (listen(listenSocket, MAX_CLIENTS) == SOCKET_ERROR) {
        printf("Khong the lang nghe ket noi");
        closesocket(listenSocket);
        return 1;
    }

    printf("Dang lang nghe ket noi...\n");

    while (1) {
        
        clientAddrLen = sizeof(clientAddr);
        clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            printf("Khong the chap nhan ket noi");
            closesocket(listenSocket);
            return 1;
        }

        printf("Client da ket noi: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)handleClient, (LPVOID)clientSocket, 0, NULL);
        if (hThread == NULL) {
            printf("Khong the tao tieu trinh con");
            closesocket(clientSocket);
            continue;
        }

        
        CloseHandle(hThread);
    }

    
    closesocket(listenSocket);

    
    WSACleanup();

    return 0;
}

void handleClient(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    int ret;

    ret = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (ret == SOCKET_ERROR || ret == 0) {
        printf("Khong the nhan lenh tu client");
        closesocket(clientSocket);
        return;
    }

    buffer[ret] = '\0';

    if (strcmp(buffer, "GET_TIME dd/mm/yyyy") == 0) {
        time_t currentTime;
        struct tm* localTime;
        char timeStr[BUFFER_SIZE];

        currentTime = time(NULL);
        localTime = localtime(&currentTime);

        strftime(timeStr, BUFFER_SIZE, "%d/%m/%Y", localTime);

        send(clientSocket, timeStr, strlen(timeStr), 0);
    } else if (strcmp(buffer, "GET_TIME dd/mm/yy") == 0) {
        time_t currentTime;
        struct tm* localTime;
        char timeStr[BUFFER_SIZE];

        currentTime = time(NULL);
        localTime = localtime(&currentTime);

        strftime(timeStr, BUFFER_SIZE, "%d/%m/%y", localTime);

        send(clientSocket, timeStr, strlen(timeStr), 0);
    } else if (strcmp(buffer, "GET_TIME mm/dd/yyyy") == 0) {
       
        time_t currentTime;
        struct tm* localTime;
        char timeStr[BUFFER_SIZE];

        currentTime = time(NULL);
        localTime = localtime(&currentTime);

        strftime(timeStr, BUFFER_SIZE, "%m/%d/%Y", localTime);

        send(clientSocket, timeStr, strlen(timeStr), 0);
    } else if (strcmp(buffer, "GET_TIME mm/dd/yy") == 0) {
        time_t currentTime;
        struct tm* localTime;
        char timeStr[BUFFER_SIZE];

        currentTime = time(NULL);
        localTime = localtime(&currentTime);

        
        strftime(timeStr, BUFFER_SIZE, "%m/%d/%y", localTime);

        
        send(clientSocket, timeStr, strlen(timeStr), 0);
    } else {
        
        char errorStr[] = "Lệnh không hợp lệ!";
        send(clientSocket, errorStr, strlen(errorStr), 0);
    }

    closesocket(clientSocket);
}