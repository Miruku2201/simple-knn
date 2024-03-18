#include "main.hpp"

/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */

template <typename T>
class List
{
public:
    virtual ~List() = default;
    virtual void push_back(T value) = 0;
    virtual void push_front(T value) = 0;
    virtual void insert(int index, T value) = 0;
    virtual void remove(int index) = 0;
    virtual T& get(int index) const = 0;
    virtual int length() const = 0;
    virtual void clear() = 0;
    virtual void print() const = 0;
    virtual void reverse() = 0;
};

template <typename T>
class Node
{
private:
    Node<T>* next = nullptr;
    T data;
public:
    Node(T data, Node<T>* next = nullptr) : data(data), next(next) {}
    T& getData() { return data; }
    Node<T>* getNext() const { return next; }
    void setNext(Node<T>* next) { this->next = next; }
    void setData(T data) { this->data = data; }
};

template<typename T>
void Swap(Node<T>& s1, Node<T>& s2) {
    Node<T> temp = s1;
    s1 = s2;
    s2 = temp;
}

// Generate the Override class that inherits from the 'List' class
template <typename T>
class OList : public List<T>
{
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    OList() : head(nullptr), tail(nullptr), size(0) {}
    ~OList()
    {
        clear();
    }
    OList(const OList<T>& other) {
        head = nullptr;
        tail = nullptr;
        size = 0;
        Node<T>* temp = other.head;
        while (temp != nullptr)
        {
            push_back(temp->getData());
            temp = temp->getNext();
        }
    }
    OList& operator=(const OList<T>& other) {
        if (this != &other)
        {
            clear();
            Node<T>* temp = other.head;
            while (temp != nullptr)
            {
                push_back(temp->getData());
                temp = temp->getNext();
            }
        }
        return *this;
    }
    void push_back(T value) override;
    void push_front(T value) override;
    void insert(int index, T value) override;
    void remove(int index) override;
    T& get(int index) const override;
    int length() const override;
    void clear() override;
    void print() const override;
    void reverse() override;
};

class Dataset {
private:
    List<List<int>*>* data;
    //You may need to define more
    List<std::string>* column_name;
public:
    Dataset() {
        data = new OList<List<int>*>();
        column_name = new OList<std::string>();
    }
    Dataset(List<List<int>*>* data, List<std::string>* column_name) {
        this->data = data;
        this->column_name = column_name;
    }
    ~Dataset() {
        //// Delete data
        for (int i = 0; i < data->length(); i++) {
            data[i].clear();
        }
        delete[]data;

        //// Delete column_name
        column_name->clear();
    }
    Dataset(const Dataset& other);
    Dataset& operator=(const Dataset& other);
    bool loadFromCSV(const char* fileName);
    void printHead(int nRows = 5, int nCols = 5) const;
    void printTail(int nRows = 5, int nCols = 5) const;
    void getShape(int& nRows, int& nCols) const;
    void columns() const;
    bool drop(int axis = 0, int index = 0, std::string columns = "");
    Dataset extract(int startRow = 0, int endRow = -1, int startCol = 0, int endCol = -1) const;
    List<List<int>*>* getData() const;
};

float Euclidean(List<int>* sample1, List<int>* sample2);

 class kNN {
 private:
     int k;
     //You may need to define more
     Dataset X_train;
     Dataset y_train;

     int find_label(List<float>* Euclidean_row);
 public:
     kNN(int k = 5);
     void fit(const Dataset& X_train, const Dataset& y_train);
     Dataset predict(const Dataset& X_test);
     double score(const Dataset& y_test, const Dataset& y_pred);
 };


 void train_test_split(Dataset& X, Dataset& y, double test_size,
                         Dataset& X_train, Dataset& X_test, Dataset& y_train, Dataset& y_test);

// Please add more or modify as needed


template<typename T>
inline void OList<T>::push_back(T value){
    Node<T>* newNode = new Node<T>(value);
    if (head == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->setNext(newNode);
        tail = newNode;
    }
    size++;
}


template <typename T>
inline void OList<T>::push_front(T value)
{
    Node<T>* newNode = new Node<T>(value);
    if (head == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        newNode->setNext(head);
        head = newNode;
    }
    size++;
}

template <typename T>
inline void OList<T>::insert(int index, T value)
{
    if (index < 0 || index > size)
    {
        return;
    }
    if (index == 0)
    {
        push_front(value);
    }
    else if (index == size)
    {
        push_back(value);
    }
    else
    {
        Node<T>* newNode = new Node<T>(value);
        Node<T>* current = head;
        for (int i = 0; i < index - 1; i++)
        {
            current = current->getNext();
        }
        newNode->setNext(current->getNext());
        current->setNext(newNode);
        size++;
    }
}

template <typename T>
inline void OList<T>::remove(int index)
{
    if (index < 0 || index >= size)
    {
        return;
    }
    if (index == 0)
    {
        Node<T>* temp = head;
        head = head->getNext();
        delete temp;
        size--;
    }
    else
    {
        Node<T>* current = head;
        for (int i = 0; i < index - 1; i++)
        {
            current = current->getNext();
        }
        Node<T>* temp = current->getNext();
        current->setNext(temp->getNext());
        delete temp;
        size--;
    }
}

template <typename T>
inline T& OList<T>::get(int index) const
{
    if (index < 0 || index >= size)
    {
        throw std::out_of_range("get(): Out of range");
    }
    Node<T>* current = head;
    for (int i = 0; i < index; i++)
    {
        current = current->getNext();
    }
    return current->getData();
}

template <typename T>
inline int OList<T>::length() const
{
    return size;
}

template <typename T>
inline void OList<T>::clear()
{
    while (head != nullptr)
    {
        Node<T>* temp = head;
        head = head->getNext();
        delete temp;
    }
    size = 0;
}

template <typename T>
inline void OList<T>::print() const
{
    Node<T>* current = head;
    while (current != nullptr)
    {
        cout << current->getData() << " ";
        current = current->getNext();
    }
}

template <typename T>
inline void OList<T>::reverse()
{
    Node<T>* prev = nullptr;
    Node<T>* current = head;
    Node<T>* next = nullptr;
    while (current != nullptr)
    {
        next = current->getNext();
        current->setNext(prev);
        prev = current;
        current = next;
    }
    head = prev;
}
