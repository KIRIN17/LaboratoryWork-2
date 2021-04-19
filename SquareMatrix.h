//
// Created by Dom on 11.04.2021.
//

#ifndef UNIT_SQUAREMATRIX_H
#define UNIT_SQUAREMATRIX_H
#include <iostream>
#include "DynamicArray.h"

template<typename T>
class SquareMatrix{
private:
    DynamicArray<DynamicArray<T>> data;
    int m;//количество строк или столбцов
public:
    int isFailed = 0;
    SquareMatrix();

    ~SquareMatrix(){
        for (int i = 0; i < m; ++i) {
            data[i].Delete();
        }
        std::cout << "\nDestructor call . . ." << this << std::endl;
    }

    //в конструкторе происходит чтение чисел из массива+
    SquareMatrix(int m,T** items)
    {
        this->m = m;
        data.Resize(m);
        for (int i = 0; i < m; ++i) {
            data[i].Resize(m);
        }

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                data[i].Set(j,items[i][j]);
            }
        }
        std::cout << "Constructor by array done for " << this << std::endl;
    }

    //в конструкторе происходит ввод чисел+
    SquareMatrix(int m){
        this->m = m;
        data.Resize(m);
        for (int i = 0; i < m; ++i) {
            data[i].Resize(m);
        }
        T value;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                if((std::cin >> value)){
                    data[i].Set(j,value);
                }else{
                    std::cout << "Incorrect input";
                    isFailed = 1;//контрольное значение ошибки изменяется в случае некорректного ввода
                    return;
                }
            }
        }
        std::cout << "Constructor done for " <<this<<std::endl;
    }

    int getM(){return m;}
    T Get(int i,int j){return data[i].Get(j);}

    //транспонирование матрицы+
    void Transposition(){
        DynamicArray<DynamicArray<T>> tmp;
        tmp.Resize(m);
        for (int i = 0; i < m; ++i) {
            tmp[i].Resize(m);
        }

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < m; ++j) {
                tmp[i].Set(j,data[j].Get(i));
            }
        }

        this->data = tmp;
        this->m = m;
    }

    //смена строк местами+
    void swapStrings(int first,int second){
        if(first >= m || second >= m){
            isFailed = 1;
            std::cout << "Incorrect indexes for swap strings";
            return;
        }
        else{
            DynamicArray<T> tmp = data[first];
            data[first] = data[second];
            data[second] = tmp;
        }
    }

    //смена столбцов местами+
    void swapColumns(int first,int second){
        if(first >= m || second >= m){
            isFailed = 1;
            std::cout << "Incorrect indexes for swap columns";
            return;
        }else{
            this->Transposition();
            this->swapStrings(first,second);
            this->Transposition();
        }
    }

    //умножение строки на число+
    void stringsScalarMultiplication(T k,int index){
        if(index >= m){
            isFailed = 1;
            std::cout << "Incorrect index for string scalar multiplication";
            return;
        }else{
            for (int i = 0; i < m; ++i) {
                data[index].Set(i,k*data[index].Get(i));
            }
        }
    }

    //умножение столбца на число+
    void columnsScalarMultiplication(T k,int index){
        if(index >= m){
            isFailed = 1;
            std::cout << "Incorrect index for columns scalar multiplication";
            return;
        }else{
            this->Transposition();
            this->stringsScalarMultiplication(k,index);
            this->Transposition();
        }
    }

    //сложение двух строк,первый индекс - той,к которой прибавляем строку со вторым индексом+
    void additionoftwoStrings(int first,int second){
        if(first >= m || second >=m){
            isFailed = 1;
            std::cout << "Incorrect indexes for addition of two strings";
            return;
        }else{
            for (int i = 0; i < m; ++i) {
                data[first].Set(i,data[first].Get(i) + data[second].Get(i));
            }
        }
    }

    //сложение двух столбцов,первый индекс - того,к которому прибавляем столбец со вторым индексом+
    void additionoftwoColumns(int first,int second){
        if(first >= m || second >= m){
            isFailed = 1;
            std::cout << "Incorrect indexes for addition of two columns";
            return;
        }else{
            this->Transposition();
            this->additionoftwoStrings(first,second);
            this->Transposition();
        }
    }

    //умножение всей матрицы на число+
    void matrixMultiplicationByNumber(T k){
        for (int i = 0; i < m; ++i) {
            stringsScalarMultiplication(k,i);
        }
    }

    //сумма двух матриц+
    SquareMatrix<T>* Sum(SquareMatrix<T>& first,SquareMatrix<T>& second){
        if(first.getM() != second.getM()){
            isFailed = 1;
            std::cout << "Can't sum";
            return nullptr;
        }else{

            T** tmp = (T**)malloc(sizeof(T*)*m);
            for (int i = 0; i < m; ++i) {
                tmp[i] = (T*)malloc(sizeof(T)*m);
            }

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < m; ++j) {
                    tmp[i][j] = first.Get(i,j) + second.Get(i,j);
                }
            }
            SquareMatrix<T>* result = new SquareMatrix<T>(m,tmp);
            for (int i = 0; i < m; ++i) {
                delete[] tmp[i];
            }
            delete[] tmp;
            return result;
        }
    }

    //умножение двух матриц+
    SquareMatrix<T>* Multiply(SquareMatrix<T>& first,SquareMatrix<T>& second){
        if(first.getM() != second.getM()){
            isFailed = 1;
            std::cout << "Can't multiply";
            return nullptr;
        }else{
            int M = first.getM();
            int N = second.getM();
            int Q = first.getM();
            T** tmp = (T**)malloc(sizeof(T*)*M);
            for (int i = 0; i < M; ++i) {
                tmp[i] = (T*)malloc(sizeof(T)*N);
            }
            for (int i = 0; i < M; ++i) {
                for (int j = 0; j < N; ++j) {
                    tmp[i][j] = 0;
                    for (int k = 0; k < Q; ++k) {
                        tmp[i][j]+=first.Get(i,k)*second.Get(k,j);
                    }
                }
            }
            SquareMatrix<T>* result = new SquareMatrix<T>(M,tmp);
            for (int i = 0; i < M; ++i) {
                delete[] tmp[i];
            }
            delete[] tmp;
            return result;
        }
    }

    T MatrixNorm(){
        T max = 0;
        for (int i = 0; i < m; ++i) {
            max+=data[0].Get(i);
        }
        T tmp;
        for (int i = 0; i < m; ++i) {
            tmp = 0;
            for (int j = 0; j < m; ++j) {
                tmp+=data[i].Get(j);
            }
            if(tmp >= max)
                max = tmp;
        }
        return max;
    }

    //печать+
    void Print(){
        if(this != nullptr && isFailed == 0){
            std::cout<<std::endl;
            for (int i = 0; i < m; ++i) {
                data[i].Print();
                std::cout << std::endl;
            }
            std::cout<<std::endl;
        }else
            std::cout << "\nCan't print, invalid input" << std::endl;
    }
};


#endif //UNIT_SQUAREMATRIX_H
