#include <iostream>
#include "DynamicArray.h"
#include "LinkedList.h"
#include "Matrix.h"
#include "SquareMatrix.h"
#include <iostream>


template<typename T>
class Sequence{
public:
    virtual Sequence<T>* GetSubsequence(int startIndex,int endIndex) = 0;
    virtual Sequence<T>* Concat(Sequence<T>* list) = 0;

    virtual T GetFirst() = 0;
    virtual T GetLast() = 0;
    virtual T Get(int index) = 0;
    virtual int GetLength() = 0;

    virtual void Append(T item) = 0;
    virtual void Prepend(T item) = 0;
    virtual void InsertAt(T item,int index) = 0;

    virtual void Print() = 0;
};

template<typename T>
class ArraySequence : public Sequence<T>{
public:
    ArraySequence(){
        DynamicArray<T>* tmp = new DynamicArray<T>();
        data = *tmp;
    }

    ArraySequence(T* item,int count){

        DynamicArray<T>* tmp = new DynamicArray<T>(item,count);
        data = *tmp;

    }

    ArraySequence(ArraySequence<T>& array){//копиконструктор для параметра ArraySequence

        DynamicArray<T>* tmp = new DynamicArray<T>(array.data);//конструктор копирует соответствующее поле
        data = *tmp;
    }

    ArraySequence(LinkedList<T>& list){//копиконструктор для параметра LinkedList
        T* items;
        items = (T*)malloc(sizeof(T)*(list.GetLength()));
        for (int i = 0; i < list.GetLength(); ++i) {
            items[i] = list.Get(i);
        }

        DynamicArray<T>* tmp = new DynamicArray<T>(items,list.GetLength());
        data = *tmp;
    }
    int GetLength(){
        return data.GetSize();
    }
    T Get(int index){
        return data.Get(index);
    }
    T GetFirst(){
        return data.Get(0);
    }
    T GetLast(){
        return data.Get(data.GetSize()-1);
    }

    void Append(T item){
        data.Append(item);
    }
    void Prepend(T item){
        data.Prepend(item);
    }
    void InsertAt(T item,int index){//void Set(int index,T value)
        if(index >= data.GetSize() || index < 0)
            throw "\nInsertAt Message : Index Out Of Range";
        data.Resize(data.GetSize()+1);
        for (int i = data.GetSize()-2; i >= index ; --i) {
            data.Set(i+1,data.Get(i));
        }
        data.Set(index,item);
    }
    void Print(){
        data.Print();
    }

    ArraySequence<T>* GetSubsequence(int startIndex,int endIndex){
        T* items = (T*)malloc(sizeof(T)*(endIndex-startIndex));
        for (int i = startIndex; i < endIndex; ++i) {
            items[i-startIndex] = data.Get(i);
        }
        ArraySequence<T>* result = new ArraySequence<T>(items,endIndex-startIndex);
        return result;
    }

    ArraySequence<T>* Concat(Sequence<T>* list){
        if(list->GetLength() != 0){
            T* items = (T*)malloc(sizeof(T)*(data.GetSize()+list->GetLength()));
            for (int i = 0; i < data.GetSize(); ++i) {
                items[i] = data.Get(i);
            }
            for (int i = 0; i < list->GetLength(); ++i) {
                items[i+data.GetSize()] = list->Get(i);
            }
            ArraySequence<T>* result = new ArraySequence<T>(items,data.GetSize()+list->GetLength());

            return result;
        }else{
            ArraySequence<T>* result = new ArraySequence<T>(*this);
            return result;
        }

    }
private:
    DynamicArray<T> data;
};

template<typename T>
class LinkedListSequence : public Sequence<T>{
public://информация о размере хранится в поле data
    LinkedListSequence(){//конструктор по умолчанию
        LinkedList<T>* tmp = new LinkedList<T>();
        data = *tmp;
    }
    LinkedListSequence(T* items,int count){//конструктор по массиву
        LinkedList<T>* tmp = new LinkedList<T>(items,count);
        data = *tmp;

    }

    //переопределение не охватывает соответствующую функцию из LinkedList,
    // так как требуется общность первого параметра,вследствие чего нет возможности получить данные
    LinkedListSequence<T>* Concat(Sequence<T>* list){
        for (int i = 0; i < list->GetLength(); ++i) {//добавляю в data все элементы из list,её Size увеличивается
            data.Append(list->Get(i));
        }
        T* items;
        items = (T*)malloc(sizeof(T*)*(data.GetLength()+list->GetLength()));
        for (int i = 0; i < data.GetLength(); ++i) {//записываю все элементы из data  в массив
            items[i] = data.Get(i);
        }

        LinkedListSequence<T>* result = new LinkedListSequence<T>(items,data.GetLength());//создание нового экземпляра
        return result;
    }

    LinkedListSequence(const LinkedList<T>& list){//конструктор по объекту
        LinkedList<T>* tmp = new LinkedList<T>(list);
        std::cout<< "Constructor by object . . .\n";
        data = *tmp;
    }
    LinkedListSequence<T>* GetSubsequence(int startIndex,int endIndex){//вернуть Sequence<T>*
        LinkedList<T>* tmp = data.GetSubLinkedList(startIndex,endIndex);//создаем экземляр с указанными индексами
        T* items;
        items = (T*)malloc(sizeof(T*)*(endIndex-startIndex));
        for (int i = 0; i < endIndex-startIndex; ++i) {
            items[i] = tmp->Get(i);
        }

        LinkedListSequence<T>* result = new LinkedListSequence<T>(items,endIndex-startIndex);

        return result;
    }
    int GetLength(){
        return data.GetLength();
    }
    T GetFirst(){
        data.GetFirst();
    }
    T GetLast(){
        data.GetLast();
    }
    T Get(int index){
        data.Get(index);
    }

    void Append(T item){
        data.Append(item);
    }
    void Prepend(T item){
        data.Prepend(item);
    }
    void InsertAt(T item,int index){
        data.InsertAt(item,index);
    }
    void removeAt(int index){
        data.removeAt(index);
    }
    void Print(){
        data.print_from_begin();
    }
private:
    LinkedList<T> data;
};


//Тестирование
template<typename T>
void ArraySequenceTesting(T* items,int count,int index,T data,int start,int end){
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
    std::cout << "\t\tSTART TESTING FOR ARRAYSEQUENCE";
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
    ArraySequence<T> first(items,count);
    first.Print();
    std::cout << "\nDemonstration of CopyConstructor , copied from first :";
    ArraySequence<T> copy(first);
    copy.Print();

    try{
        std::cout << "\n" << first.Get(index);
    }catch(const char* msg){
        std::cout << msg;
    }
    try{
        std::cout << first.GetFirst();
    }catch(const char* msg){
        std::cout << msg;
    }
    try{
        std::cout << first.GetLast() << "\n";
    }catch(const char* msg){
        std::cout << msg;
    }

    first.Append(data);
    first.Print();

    first.Prepend(data);
    first.Print();

    try{
        first.InsertAt(index,data);
        first.Print();
    }catch(const char* msg){
        std::cout << msg;
    }
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
    std::cout << "\n\nConcatenation of empty and non-empty objects";
    ArraySequence<T>* e = new ArraySequence<T>();//создание пустого объекта для демонстрации конкатенации
    ArraySequence<T>* q = new ArraySequence<T>();//попытка склеить его с пустым
    ArraySequence<T>* a = e->Concat(&first);
    std::cout << "\nResult of concatenation of empty and non-empty objects:";
    a->Print();
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
    std::cout << "\n\nConcatenation of non-empty and non-empty objects";
    ArraySequence<T>* c = first.Concat(&first);
    std::cout << "\nResult of concatenation of non-empty and non-empty objects:";
    c->Print();
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";

    try {
        std::cout << "\n\nGetSubsequence start = " << start << " end = " << end << "\n";
        ArraySequence<T>* sub = first.GetSubsequence(start,end);
        sub->Print();
    }catch(const char* msg){
        std::cout << msg;
    }
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
    std::cout << "\t\tEND TESTING FOR ARRAYSEQUENCE";
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
}

template<typename T>
void LinkedListSequenceTesting(T* items,int count,int index,T data,int start,int end){
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
    std::cout << "\t\tSTART TESTING FOR LINKEDLISTSEQUENCE";
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
    LinkedListSequence<T> first(items,count);
    first.Print();
    //сначала попытки узнать первый,последний и элемент по индексу
    try{
        std::cout << "\nGet index = " << index << "\n" <<first.Get(index);
    }catch(const char* msg) {
        std::cout << msg;
    }

    try{
        std::cout << first.GetFirst();
    }catch(const char* msg) {
        std::cout << msg;
    }

    try{
        std::cout << first.GetLast();
    }catch(const char* msg) {
        std::cout << msg;
    }
    std::cout<<"\n\nAppend data = " << data << "\n";
    std::cout<<"\nPrepend data = " << data << "\n";
    first.Append(data);
    first.Prepend(data);
    first.Print();

    try{
        first.InsertAt(data,index);
        first.Print();
    }catch (const char* msg){
        std::cout << msg;
    }


    try{
        std::cout << "\n\nGetSubsequence start = " << start << " end = " << end << "\n";
        LinkedListSequence<T>* second = first.GetSubsequence(start,end);
        second->Print();
        std::cout << "\n\nConcat :";
        first.Print();
        std::cout << "\nand";
        second->Print();
        std::cout << "\n";
        std::cout << "Result of Concat:";
        LinkedListSequence<T>* third = first.Concat(second);
        third->Print();
    }catch(const char* msg){
        std::cout << msg;
    }
    std::cout << "\n\nDemonstration of CopyConstructor , copied from first :";
    first.Print();
    std::cout << "\nCopy of first:";
    LinkedListSequence<T> copy(first);
    copy.Print();
    std::cout << std::endl;
    try{
        copy.removeAt(index);
    }
    catch(const char* msg){
        std::cout << msg;
    }
    copy.Print();
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
    std::cout << "\t\tEND TESTING FOR LINKEDLISTSEQUENCE";
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
}


int main() {
    int flag;
    std::cout << "Start testing classes(1) or matrices(2)?\n";
    std::cin >> flag;

    if(flag == 1){
        int* t;
        t = (int*)malloc(sizeof(int)*4);
        for (int i = 0; i < 4; ++i) {
            t[i] = 10 + i;
        }

        ArraySequenceTesting(t,4,10,100,2,3);//стандартное использование
        ArraySequenceTesting((int*)nullptr,2,4,100,-100,100);//рассмотрение всех граничных случаев

        LinkedListSequenceTesting(t,4,10,100,2,3);//стандартное использование
        LinkedListSequenceTesting((int*)nullptr,2,4,100,-100,100);//рассмотрение всех граничных случаев
    }
    else{
        if(flag == 2){
            std::cout << "Get started with regular matrices(1) or square ones(2)?\n";
            std::cin >> flag;
            if(flag == 1){
                std::cout << "Enter the numbers of rows and columns for a first regular matrix :\n";
                int m,n;
                std::cin >> m >> n;//ввод первой матрицы
                std::cout << "Enter values for the first regular matrix:\n";
                Matrix<int> first(m,n);
                first.Print();
                std::cout << "MartixNorm : " <<first.MatrixNorm() << "\n";

                std::cout << "Enter the numbers of rows and columns for a second regular matrix :\n";
                std::cin >> m >> n;//ввод второй матрицы
                std::cout << "Enter values for the second regular matrix:\n";
                Matrix<int> second(m,n);
                second.Print();
                std::cout << "MartixNorm : " <<second.MatrixNorm() << "\n";

                //демонстрация смены строк и столбцов для первой матрицы
                std::cout << "First matrix :\n";
                first.Print();
                std::cout << "Enter the indices of the rows to be swapped : \n";
                std::cin >> m >> n;
                first.swapStrings(m,n);
                first.Print();

                std::cout << "Enter the indices of the columns to be swapped : \n";
                std::cin >> m >> n;
                first.swapColumns(m,n);
                first.Print();

                //транспонирование изменённой матрицы
                std::cout << "Transposed modified first matrix :\n";
                first.Transposition();
                first.Print();

                double k;//умножение строки и столбца на число
                std::cout << "Enter the number to multiply by row and column :\n";
                std::cin >> k;
                std::cout << "Enter the indices of the row and column to be multiply on this number : \n";
                std::cin >> m >> n;
                first.stringsScalarMultiplication(k,m);
                first.columnsScalarMultiplication(k,n);
                first.Print();

                //сложение двух строк и двух столбцов
                std::cout << "Enter the indices of the rows to be summed : \n";
                std::cin >> m >> n;
                first.additionoftwoStrings(m,n);
                first.Print();

                std::cout << "Enter the indices of the columns to be summed : \n";
                std::cin >> m >> n;
                first.additionoftwoColumns(m,n);
                first.Print();

                std::cout << "Enter the number to multiply by first matrix :\n";
                std::cin >> k;
                first.matrixMultiplicationByNumber(k);
                first.Print();

                //сложение первой и второй матрицы
                std::cout << "Sum of the first and second matrix :\n";
                Matrix<int>* sum = first.Sum(first,second);
                sum->Print();

                //умножение первой матрицы на вторую
                std::cout << "Multiply of the first and second matrix :\n";
                Matrix<int>* multiply = first.Multiply(first,second);
                multiply->Print();
            }
            else{
                if(flag == 2){
                    std::cout << "Enter the number of rows and columns for a first square matrix :\n";
                    int m,n;
                    std::cin >> m;//ввод первой матрицы
                    std::cout << "Enter values for the first square matrix:\n";
                    SquareMatrix<int> first(m);
                    first.Print();
                    std::cout << "MartixNorm : " <<first.MatrixNorm() << "\n";

                    std::cout << "Enter the number of rows and columns for a first square matrix :\n";
                    std::cin >> m;//ввод второй матрицы
                    std::cout << "Enter values for the second square matrix:\n";
                    SquareMatrix<int> second(m);
                    second.Print();
                    std::cout << "MartixNorm : " <<second.MatrixNorm() << "\n";

                    //демонстрация смены строк и столбцов для первой матрицы
                    std::cout << "First matrix :\n";
                    first.Print();
                    std::cout << "Enter the indices of the rows to be swapped : \n";
                    std::cin >> m >> n;
                    first.swapStrings(m,n);
                    first.Print();

                    std::cout << "Enter the indices of the columns to be swapped : \n";
                    std::cin >> m >> n;
                    first.swapColumns(m,n);
                    first.Print();

                    //транспонирование изменённой матрицы
                    std::cout << "Transposed modified first matrix :\n";
                    first.Transposition();
                    first.Print();

                    double k;//умножение строки и столбца на число
                    std::cout << "Enter the number to multiply by row and column :\n";
                    std::cin >> k;
                    std::cout << "Enter the indices of the row and column to be multiply on this number : \n";

                    std::cin >> m >> n;
                    first.stringsScalarMultiplication(k,m);
                    first.columnsScalarMultiplication(k,n);
                    first.Print();

                    //сложение двух строк и двух столбцов
                    std::cout << "Enter the indices of the rows to be summed : \n";
                    std::cin >> m >> n;
                    first.additionoftwoStrings(m,n);
                    first.Print();

                    std::cout << "Enter the indices of the columns to be summed : \n";
                    std::cin >> m >> n;
                    first.additionoftwoColumns(m,n);
                    first.Print();

                    std::cout << "Enter the number to multiply by first matrix :\n";
                    std::cin >> k;
                    first.matrixMultiplicationByNumber(k);
                    first.Print();

                    //сложение первой и второй матрицы
                    std::cout << "Sum of the first and second matrix :\n";
                    SquareMatrix<int>* sum = first.Sum(first,second);
                    sum->Print();

                    //умножение первой матрицы на вторую
                    std::cout << "Multiply of the first and second matrix :\n";
                    SquareMatrix<int>* multiply = first.Multiply(first,second);
                    multiply->Print();
                }
            }
        }
    }
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
    std::cout << "\t\t\tEND OF WORK";
    std::cout << "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";

    return 0;
}
