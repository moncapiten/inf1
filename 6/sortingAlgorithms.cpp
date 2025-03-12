#include <iostream>
#include <chrono>
#include <vector>

using namespace std;


// Selection Sort
void selectionSort(vector<int> &arr){
    int n = arr.size();
    for(int i=0; i<n-1; i++){
        int minIndex = i;
        for(int j=i+1; j<n; j++){
            if(arr[j] < arr[minIndex]){
                minIndex = j;
            }
        }
        swap(arr[i], arr[minIndex]);
    }
}



// Bubble Sort
void bubbleSort(vector<int> &arr){
    int n = arr.size();
    for(int i=0; i<n-1; i++){
        for(int j=0; j<n-i-1; j++){
            if(arr[j] > arr[j+1]){
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

void swap(int &a, int &b){
    int temp = a;
    a = b;
    b = temp;

    // could also do 
    // a = a + b;
    // b = a - b;
    // a = a - b;
}



// Insertion Sort
void insertionSort(vector<int> &arr){
    int n = arr.size();
    for(int i=1; i<n; i++){
        int key = arr[i];
        int j = i-1;
        while(j > 0 && arr[j] > key){
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}










void printVector(const vector<int> &arr){
    for(int i=0; i<arr.size(); i++){
        cout << arr[i] << " ";
    }
    cout << endl;
}

int random(int min, int max){
    return min + rand() % (max - min + 1);
}

vector<int> generateRandomArray(int n, int min, int max){
    vector<int> arr(n);
    for(int i=0; i<n; i++){
        arr[i] = random(min, max);
    }
    return arr;
}



int main(){
//    vector<int> arr = {5, 2, 9, 1, 5, 6};
    constexpr int n = 100000;
    constexpr int min = 1;
    constexpr int max = 10;

    vector<int> selectionArr = generateRandomArray(n, min, max);
    vector<int> bubbleArr = selectionArr;
    vector<int> insertionArr = selectionArr;


    

    using namespace std::chrono;

    auto start = high_resolution_clock::now();
    selectionSort(selectionArr);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Selection Sort took " << duration.count()/1000 << " milliseconds" << endl;

    start = high_resolution_clock::now();
    bubbleSort(bubbleArr);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Bubble Sort took " << duration.count()/1000 << " milliseconds" << endl;

    start = high_resolution_clock::now();
    insertionSort(insertionArr);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Insertion Sort took " << duration.count()/1000 << " milliseconds" << endl;
/*  cout << "Selection Sort" << endl;
    printVector(selectionArr);
    selectionSort(selectionArr);
    printVector(selectionArr);

    cout << "Bubble Sort" << endl;
    printVector(bubbleArr);
    bubbleSort(bubbleArr);
    printVector(bubbleArr);

    cout << "Insertion Sort" << endl;
    printVector(insertionArr);
    insertionSort(insertionArr);
    printVector(insertionArr);
*/



    return 0;
}