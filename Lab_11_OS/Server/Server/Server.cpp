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


int findMin(vector<int>& v) {
    int min = v[0];

    for (int i = 0; i < v.size(); i++) {
        if (v[i] < min) min = v[i];
    }

    return min;
}

int findMax(vector<int>& v) {
    int max = v[0];

    for (auto val : v) {
        if (val > max) max = val;
    }

    return max;
}

void CountingSearch(vector<int> v) {
    int min = findMin(v);
    int max = findMax(v);
    int best_ideas[3];
    int range = max - min + 1;

    vector<int> table(range);

    for (int i = 0; i < v.size(); i++) {
        table[v[i] - min]++;
    }
    for (int i = 0; i < 3; i++) {
        max = table[0];
        best_ideas[i] = 1;
        for (int j = 0; j < table.size(); j++) {
            if (table[j] != 0)
                if (max < table[j]) {
                    max = table[j];
                    best_ideas[i] = j + 1;
                }
            table[best_ideas[i] - 1] = 0;
        }
    }
    //for (int i = 0; i < 3; i++) {
    //    cout << best_ideas[i] << " ";
    //}
    
    cout << "Best ideas: ";
    int tempCount = 1;
    ifstream in(path);
    
        if (in.is_open())
        {
            while (getline(in, bestIdeas))
            {
                for (int i = 0; i < 3; i++) {
                    if (tempCount == best_ideas[i]) {
                        cout << endl << to_string(tempCount) << ". " << bestIdeas << endl;
                    }
                }
                tempCount++;
            }
        }
        in.close();



        //Write to File

}


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

    vector<char> res;
    vector<int> indexOfIdeas;
    string tempStr;
    int isEndCounter = 0;
    clock_t start, stop;
    double result = 0;
    start = clock();
    int countTime = 0;
    while (1) {
        fd_set copy = master;
        int count = 1;
        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
        //socketCount = 3;

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
                    getchar();
                    TimeIsUp();
                }

                //--------------------------------------------------------------------------------------------------------------
                for (int i = 0; i < bytesIn; i++) {
                    
                    if (buf[i] >= 48 && buf[i] <= 57 || buf[i] == 105 || buf[i] == 101) {     
                        //cout << buf[i] << " ";
                        if (buf[i] == 101) {
                            isEndCounter++;
                        }
                        if (buf[i] >= 48 && buf[i] <= 57 || buf[i] == 105) {
                            res.push_back(buf[i]);
                        }
                    }
                    
                }

                if (isEndCounter == socketCount) {
                    for (int j = 0; j < res.size(); j++) {
                        //cout << res[j] << " ";
                        if (res[j] != 105) {
                            indexOfIdeas.push_back(res[j] - 48);
                        }
                        else {
                            j++;
                            tempStr += res[j];
                            j++;
                            tempStr += res[j];
                            indexOfIdeas.push_back(stoi(tempStr));
                            tempStr.clear();
                        }
                    }
                    cout << endl << "Voting ideas: ";
                    for (int i = 0; i < indexOfIdeas.size(); i++) {
                        cout << indexOfIdeas[i] << " ";
                    }

                    cout << endl;
                    CountingSearch(indexOfIdeas);
                }
                //-------------------------------------------------------------------------------------------------------------------            


                if (bytesIn <= 0) {
                    //Drop the client
                    closesocket(sock);
                    FD_CLR(sock, &master);
                }
                //else {
                //    //Send message to other clients
                //    for (int i = 0; i < master.fd_count; i++) {
                //        SOCKET outSock = master.fd_array[i];
                //        if (outSock != listening && outSock != sock) {
                //            send(outSock, buf, bytesIn, 0);
                //        }
                //    }
                //}
            }
        }
    }


    //Cleanup winsock
    WSACleanup();

    system("pause");
}
