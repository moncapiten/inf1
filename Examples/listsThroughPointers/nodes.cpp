#include <iostream>
#include <vector>
#include <random>


using namespace std;

class Node {
    public:
        int value;
        Node* next;

        Node(int value): value(value), next(nullptr) {}
};

void removeLastElement(Node *head){
    if(head == nullptr) return;
    if(head->next == nullptr) {
        delete head;
        head = nullptr;
        return;
    }
    Node *current = head;
    while(current->next->next != nullptr) current = current->next;
    delete current->next;
    current->next = nullptr;

}

void removeElementByValue(Node *head, int value){
    if(head == nullptr) return;
    if(head->value == value) {
        Node *temp = head->next;
        delete head;
        head = temp;
        return;
    }
    Node *current = head;
    while(current->next != nullptr && current->next->value != value) current = current->next;
    if(current->next == nullptr) return;
    Node *temp = current->next->next;
    delete current->next;
    current->next = temp;
}

void append(Node *head, int value){
    Node *current = head;
    while(current->next != nullptr) current = current->next;
    current->next = new Node(value);
}

void addFirst(Node *&head, int value){
    Node *newNode = new Node(value);
    newNode->next = head;
    head = newNode;
}

void addAfter(Node *node, int value){
    Node *newNode = new Node(value);
    newNode->next = node->next;
    node->next = newNode;
}

void addAfterVal(Node *head, int value, int newValue){
    Node *current = head;
    while(current != nullptr && current->value != value) current = current->next;
    if(current == nullptr) return;
    Node *newNode = new Node(newValue);
    newNode->next = current->next;
    current->next = newNode;
}

void printList(Node *head){
    Node *current = head;
    while(current != nullptr) {
        cout << current->value << " ";
        current = current->next;
    }
    cout << endl;
}



int main(){
    Node *head = new Node(1);
    append(head, 2);
    append(head, 3);
    append(head, 4);
    append(head, 5);
    append(head, 6);
    append(head, 7);
    append(head, 8);
    append(head, 9);
    append(head, 10);

    printList(head);

    addFirst(head, 0);
    addFirst(head, -1);
    addFirst(head, -2);

    printList(head);

    addAfter(head->next->next, 100);
    addAfter(head->next->next->next, 200);

    printList(head);

    addAfterVal(head, 100, 101);
    addAfterVal(head, 200, 201);

    printList(head);

    removeLastElement(head);
    removeLastElement(head);
    removeLastElement(head);

    printList(head);

    removeElementByValue(head, 100);
    removeElementByValue(head, 200);
    removeElementByValue(head, 201);

    printList(head);

    return 0;
}

