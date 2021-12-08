#include <Windows.h>
#include <iostream>
using namespace std;

STARTUPINFOA arrSi[1000];
PROCESS_INFORMATION arrPi[1000];


int main(){
    cout << "Put a num of clients: ";
    int countOfProcess;
    cin >> countOfProcess;
    string file;
    for(int i = 0; i < countOfProcess; i++){
        ZeroMemory(&arrSi[i], sizeof(STARTUPINFO));
        arrSi[i].cb=sizeof(STARTUPINFO);
        ZeroMemory(&arrPi[i], sizeof(PROCESS_INFORMATION));
        
        CreateProcessA(NULL, "D:\\Studying\\Course 2\\Code\\Operation System\\Lab_11_OS\\Client\\Debug\\Client.exe", NULL, NULL, TRUE,CREATE_NEW_CONSOLE, NULL, NULL, &arrSi[i], &arrPi[i]);
    }

}
