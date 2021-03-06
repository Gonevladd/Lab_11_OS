#include <iostream>
#include  <string>
#include <vector>
#include <fstream>
#include <ws2tcpip.h>
#pragma comment (lib, "ws2_32.lib")
using namespace std;

HANDLE file;
char path[] = "D:\\Studying\\Course 2\\Code\\Operation System\\Lab_11_OS\\VirtualDesk.txt";
string bestIdeas;
bool voting = false;

void TimeIsUp() {
    Sleep(3000);
    cout << "Time is up! Close file" << endl;

    SetFileAttributesA(path, FILE_ATTRIBUTE_READONLY);
    file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        cout << "Cannot open file!";
        getchar();
        return;
    }
    int counter = 1;
    char ReadBuf[1];
    string output;
    output.clear();
    DWORD dwRead;


    ifstream in(path); 
    if (in.is_open())
    {
        while (getline(in, output))
        {
            cout << to_string(counter) << ". " << output << endl;
            counter++;
        }
    }
    in.close();     
    

    cout << "Start voiting!";
    voting = true;
    getchar();

    CloseHandle(file);
}


int main()
{
    //Create file
    file = CreateFileA(path, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    SetFileAttributesA(path, FILE_ATTRIBUTE_NORMAL);
    //CloseHandle(file);

    //Initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        cerr << "Can`t initialize winsock!" << endl;
        return 1;
    }

    //Create socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        cerr << "Can`t create a socket!" << endl;
        return 1;
    }

    //Bind ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; //Could also use inet_pton

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    //Tell winsock the socket is for listening 
    listen(listening, SOMAXCONN);

    fd_set master;
    FD_ZERO(&master);

    FD_SET(listening, &master);



    clock_t start, stop;
    double result = 0;
    start = clock();
    int countTime = 0;
    while (1) {
        fd_set copy = master;
        int count = 1;
        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
        socketCount = 2;    

        for (int i = 0; i < socketCount; i++) {
            SOCKET sock = copy.fd_array[i];
            if (sock == listening) {
                //Accept a new conncetion
                SOCKET client = accept(listening, nullptr, nullptr);

                //Add the new connection to the list of connected clients
                FD_SET(client, &master);

                //Send a welcome message to the connected client
                string welcomeMsg = "D:\\Studying\\Course 2\\Code\\Operation System\\Lab_11_OS\\VirtualDesk.txt";
                send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
                cout << "Connected on port " << client << endl;

            }
            else {
                //Recieve message
                char buf[4096];
                ZeroMemory(buf, 4096);
                int bytesIn = recv(sock, buf, 4096, 0);
                if (countTime == 0 && buf[0] == 'b') {
                    countTime++;
                    TimeIsUp();
                }
                
//--------------------------------------------------------------------------------------------------------------
            for (int i = 0; i < strlen(buf); i++) {
                 if (buf[i] >= 48 && buf[i] <= 57) {
                     int numb = buf[i] - 48;

                     int tempCount = 1;
                     ifstream in(path);
                     if (in.is_open())
                     {
                         while (getline(in, bestIdeas))
                         {
                             int numb = buf[i] - 48;
                            if (tempCount == numb) {
                                cout << endl << to_string(tempCount) << ". " << bestIdeas << endl;
                            }
                            tempCount++;
                         }
                     }
                     in.close();
                 }
            }
//-------------------------------------------------------------------------------------------------------------------            


                if (bytesIn <= 0) {
                    //Drop the client
                    closesocket(sock);
                    FD_CLR(sock, &master);
                }
                else {
                    //Send message to other clients
                    for (int i = 0; i < master.fd_count; i++) {
                        SOCKET outSock = master.fd_array[i];
                        if (outSock != listening && outSock != sock) {
                            send(outSock, buf, bytesIn, 0);
                        }
                    }
                }
            }
        }
    }

    cout << "FUCK";

    //Cleanup winsock
    WSACleanup();

    system("pause");
}







//Wait for a connection
    //sockaddr_in client;
    //int clientSize = sizeof(client);

    //SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    //if (clientSocket == INVALID_SOCKET) {
    //    cerr << "Can`t create a socket!" << endl;
    //    return 1;
    //}

    //char host[NI_MAXHOST];  //Client`s remote name
    //char service[NI_MAXSERV];   //Service port the client is connect on

    //ZeroMemory(host, NI_MAXHOST);
    //ZeroMemory(service, NI_MAXSERV);

    //if (!getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0)) {
    //    cout << host << " connected on port " << service << endl;
    //}
    //else {
    //    inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
    //    cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    //}

    ////Close listening socket
    //closesocket(listening);

    ////While loop: accept and echo message back to client
    //char buf[4096];

    //while (1) {
    //    ZeroMemory(buf, 4096);

    //    //Wait for client to send data
    //    int bytesReceived = recv(clientSocket, buf, 4096, 0);
    //    if (bytesReceived == SOCKET_ERROR) {
    //        cerr << "Error in recv()!" << endl;
    //        break;
    //    }

    //    if (!bytesReceived) {
    //        cout << "Client disconnected!" << endl;
    //        break;
    //    }
    //    //My code==============================================================
    //    string userInput;
    //    cout << "User 1 > " << buf << endl;
    //    cout << "> ";
    //    getline(cin, userInput);


    //    send(clientSocket, userInput.c_str(), userInput.size() + 1, 0);

    //    //My code==============================================================

    //    //Echo message back to client
    //    
    //    //send(clientSocket, buf, bytesReceived + 1, 0);        !!!
    //}

