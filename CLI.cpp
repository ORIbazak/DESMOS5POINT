#include "imports.h"
#include "CLI.h"




CLI::CLI()
{
    IP = "10.0.0.19";
    conn = INVALID_SOCKET;
    port = PORT;
}

bool CLI::connectToServer(void)
{
    WORD ver = MAKEWORD(2, 2);
    WSAData data;

    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0) {
        std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
        return false;
    }

    conn = socket(AF_INET, SOCK_STREAM, 0); //init socket
    if (conn == INVALID_SOCKET) {
        std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET; 
    hint.sin_port = htons(port);
    inet_pton(AF_INET, IP.c_str(), &(hint.sin_addr));

    int connResult = connect(conn, (sockaddr*)&hint, sizeof(hint)); //connect
    if (connResult == SOCKET_ERROR) {
        std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
        closesocket(conn);
        WSACleanup();
        return false;
    }
    std::cout << "Connected on ADDR: " << IP << ", port: " << port << std::endl;
    return true;
}

