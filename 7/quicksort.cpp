#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;


int partition(vector<int> &arr, int left, int right, int pivot){
    while(left <= right){
        while(arr[left] < pivot) left++;
        while(arr[right] > pivot) right--;
        if(left <= right){
            swap(arr[left], arr[right]);
            left++;
            right--;
        }
    }
    return left;
}

void quickSort(vector<int> &arr, int left, int right){
    if(left >= right) return;
    int pivot = arr[(left + right) / 2];
    int index = partition(arr, left, right, pivot);
    quickSort(arr, left, index - 1);
    quickSort(arr, index, right);
}





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

void swap(int &a, int &b){
    int temp = a;
    a = b;
    b = temp;
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



int random(int min, int max){
    return rand() % (max - min + 1) + min;
}

vector<int> generateRandomArray(int size, int min, int max){
    vector<int> arr;
    for(int i = 0; i < size; i++){
        arr.push_back(random(min, max));
    }
    return arr;
}



//reverse function
void reverse(vector<int>::iterator start, vector<int>::iterator end){
    while(start < end){
        swap(*start, *end);
        start++;
        end--;
    }
}



constexpr int SIZE = 1000000000;
constexpr int MIN = 0;
constexpr int MAX = 10;

vector<bool> chooseWhich = {true, false, false};


int main(){

    vector<int> quickarr = generateRandomArray(SIZE, MIN, MAX);
    vector<int> selectionarr = quickarr;
    vector<int> insertionarr = quickarr;


    if(chooseWhich[0]){
        auto start = high_resolution_clock::now();
        quickSort(quickarr, 0, SIZE - 1);
        auto stop = high_resolution_clock::now();
        auto randomQuickDuration = duration_cast<microseconds>(stop - start);

        start = high_resolution_clock::now();
        quickSort(quickarr, 0, SIZE - 1);
        stop = high_resolution_clock::now();
        auto orderedQuickDuration = duration_cast<microseconds>(stop - start);

        reverse(quickarr.begin(), quickarr.end());
        start = high_resolution_clock::now();
        quickSort(quickarr, 0, SIZE - 1);
        stop = high_resolution_clock::now();
        auto reverseQuickDuration = duration_cast<microseconds>(stop - start);


        cout << "QuickSort took" << endl;
        cout << "\tRandom case\t" << randomQuickDuration.count()/1000 << " milliseconds" << endl;
        cout << "\tOrdered case\t" << orderedQuickDuration.count()/1000 << " milliseconds" << endl;
        cout << "\tReverse ordered case\t" << reverseQuickDuration.count()/1000 << " milliseconds" << endl << endl;
    }





    if(chooseWhich[1]){
        auto start = high_resolution_clock::now();
        selectionSort(selectionarr);
        auto stop = high_resolution_clock::now();
        auto randomSelectionDuration = duration_cast<microseconds>(stop - start);

        start = high_resolution_clock::now();
        selectionSort(selectionarr);
        stop = high_resolution_clock::now();
        auto orderedSelectionDuration = duration_cast<microseconds>(stop - start);

        reverse(selectionarr.begin(), selectionarr.end());
        start = high_resolution_clock::now();
        selectionSort(selectionarr);
        stop = high_resolution_clock::now();
        auto reverseSelectionDuration = duration_cast<microseconds>(stop - start);


        cout << "SelectionSort took" << endl;
        cout << "\tRandom case\t" << randomSelectionDuration.count()/1000 << " milliseconds" << endl;
        cout << "\tOrdered case\t" << orderedSelectionDuration.count()/1000 << " milliseconds" << endl;
        cout << "\tReverse ordered case\t" << reverseSelectionDuration.count()/1000 << " milliseconds" << endl << endl;
    }




    if(chooseWhich[2]){
        auto start = high_resolution_clock::now();
        insertionSort(insertionarr);
        auto stop = high_resolution_clock::now();
        auto randomInsertionDuration = duration_cast<microseconds>(stop - start);

        start = high_resolution_clock::now();
        insertionSort(insertionarr);
        stop = high_resolution_clock::now();
        auto orderedInsertionDuration = duration_cast<microseconds>(stop - start);

        reverse(insertionarr.begin(), insertionarr.end());
        start = high_resolution_clock::now();
        insertionSort(insertionarr);
        stop = high_resolution_clock::now();
        auto reverseInsertionDuration = duration_cast<microseconds>(stop - start);


        cout << "Insertion took" << endl;
        cout << "\tRandom case\t" << randomInsertionDuration.count()/1000 << " milliseconds" << endl;
        cout << "\tOrdered case\t" << orderedInsertionDuration.count()/1000 << " milliseconds" << endl;
        cout << "\tReverse ordered case\t" << reverseInsertionDuration.count()/1000 << " milliseconds" << endl << endl;
    }






    return 1;
}