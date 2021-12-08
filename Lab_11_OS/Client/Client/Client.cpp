#include <iostream>
#include <vector>
#include <ws2tcpip.h>
#include <string>
#include <charconv>
#pragma comment (lib, "ws2_32.lib")
using namespace std;

int main()
{
    vector<string> ideas;
    ideas.push_back("Start a chatbot agency\n");
    ideas.push_back("Become a translator\n");
    ideas.push_back("Data entry specialist\n");
    ideas.push_back("App tester\n");
    ideas.push_back("Write product reviews\n");
    ideas.push_back("Start your own blog\n");
    ideas.push_back("Offer online courses\n");
    ideas.push_back("Create a popular social media channel\n");
    ideas.push_back("Offer consulting services\n");
    ideas.push_back("Become an online reseller\n");


    string ipAddress = "127.0.0.1";   //IP address of the server
    int port = 54000;   //Listening port on the server

    //Initialize WinSock
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0) {
        cerr << "Can`t initialize winsock! " << wsResult << endl;
        return 1;
    }

    //Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Can`t create a socket! " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    //Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //Connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        cerr << "Can`t connect to server! " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    //Do-while loop to send and recieve data
    char buf[4096];
    string userInput;
    int countOfIdeas = 0;

    do {
        //Prompt the user for some text
        cout << "> ";
        getline(cin, userInput);
        userInput = "a";
        if (userInput.size() > 0) {
            //Send the text
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR) {
                //Wait for response 
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0) {
                    //Echo response to console
                    cout << "SERVER > " << string(buf, 0, bytesReceived) << endl;
                    //Writing to file


                    HANDLE file = CreateFileA(buf, GENERIC_WRITE | GENERIC_READ | TRUNCATE_EXISTING, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);

                    if (file == INVALID_HANDLE_VALUE) {
                        cout << "Cannot open file!";
                        getchar();
                        return 1;
                    }
                    SetFileAttributesA(buf, FILE_ATTRIBUTE_NORMAL);
                    cout << "Start writing!" << endl;
                    while (1) {
                        if (GetFileAttributesA(buf) == FILE_ATTRIBUTE_READONLY) {
                            //WriteFile(file, (void*)"&", 1, 0, NULL);
                            cout << "Stop writing! File is closed!" << endl;
                            //getchar();
                            break;
                        }
                        countOfIdeas++;
                        int randIdea = rand() % 9;
                        //SetFilePointer(file, 2, NULL, FILE_END);
                        WriteFile(file, (void*)ideas[randIdea].c_str(), ideas[randIdea].length(), 0, NULL);
                        send(sock, "b", 1, 0);
                        Sleep(500);
                    }

                    vector<int> checkVect;
                    bool check = true;
                    int count = 0;
                    for (int i = 0; i < 3; i++) {
                        int numb = rand() % countOfIdeas;

                        for (int j = 0; j < checkVect.size(); j++) {
                            if (numb == checkVect[j]) {
                                i--;
                                check = false;
                                break;
                            }
                        }
                        if (check) {
                            string votes;
                            
                            checkVect.push_back(numb);
                            if (numb > 9) {
                                count++;
      
                                votes = "i" + to_string(numb+1);
                                send(sock, votes.c_str(), 3, MSG_DONTROUTE);
                            }
                            else {
                                count++;
                                votes = to_string(numb + 1);
                                send(sock, votes.c_str(), 1, MSG_DONTROUTE);
                            }
                            if (count == 3) {
                                send(sock, "e", 1, MSG_DONTROUTE);
                            }
                            votes = to_string(numb+1);
                            cout << "I vote for " << votes << " idea!" << endl;
                        }
                        check = true;
                    }

                    getchar();
                    CloseHandle(file);
                }
            }
        }
    } while (userInput.size() > 0);

    //Close everything
    closesocket(sock);
    WSACleanup();
    //CloseHandle(file);
}
