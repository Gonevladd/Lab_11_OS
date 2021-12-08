#include <iostream>
#include <vector>
using namespace ::std;

void t(vector<int> v){
    int arr[3];
    for (int i = 0; i < 3; i++){
        arr[i] = 0;
    }
    int temp = 0, maxCount = 0, index = 0;

    for (int i = 0; i < v.size(); i++){
        for (int j = i; j < v.size(); j++){
            if(i != j && v[i] == v[j]){
                temp++;
            }
            if(temp > maxCount){
                maxCount = temp;
                index = j;
            }
        }
        for (int h = 0; h < 3; h++){
            if(maxCount > arr[h]){
                arr[h] = index;
                break;
            }
        }
        maxCount = 0;
        temp = 0;
    }

    for (int i = 0; i < 3; i++){
        cout << v[arr[i]] << " ";
    }
}

int main(){
    vector<int> v = {18, 7, 12, 3, 9, 8, 6, 4, 11, 12, 11, 3, 2, 4, 12};
    t(v);
}