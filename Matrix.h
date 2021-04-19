//
// Created by Dom on 11.04.2021.
//

#ifndef UNIT_MATRIX_H
#define UNIT_MATRIX_H
#include <iostream>
#include "DynamicArray.h"
template<typename T>
class Matrix{
private:
    DynamicArray<DynamicArray<T>> data;
    int m;//количество строк
    int n;//количество столбцов

public:
    int isFailed = 0;
    Matrix();

    ~Matrix(){
        for (int i = 0; i < m; ++i) {
            data[i].Delete();
        }
    }

    //в конструкторе происходит чтение чисел из массива+
    Matrix(int m,int n,T** items)
    {
        this->m = m;
        this->n = n;
        data.Resize(m);
        for (int i = 0; i < m; ++i) {
            data[i].Resize(n);
        }

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                data[i].Set(j,items[i][j]);
            }
        }
    }

    //в конструкторе происходит ввод чисел+
    Matrix(int m,int n){
        this->m = m;
        this->n = n;
        data.Resize(m);
        for (int i = 0; i < m; ++i) {
            data[i].Resize(n);
        }
        T value;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
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
    int getN(){return n;}
    T Get(int i,int j){return data[i].Get(j);}

    //транспонирование матрицы+
    void Transposition(){
        DynamicArray<DynamicArray<T>> tmp;
        tmp.Resize(n);
        for (int i = 0; i < n; ++i) {
            tmp[i].Resize(m);
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                tmp[i].Set(j,data[j].Get(i));
            }
        }

        this->data = tmp;
        int a = m;
        int b = n;
        this->m = b;
        this->n = a;
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
        if(first >= n || second >= n){
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
            for (int i = 0; i < n; ++i) {
                data[index].Set(i,k*data[index].Get(i));
                //data[index][i] = k * data[index][i];
            }
        }
    }

    //умножение столбца на число+
    void columnsScalarMultiplication(T k,int index){
        if(index >= n){
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
            for (int i = 0; i < n; ++i) {
                data[first].Set(i,data[first].Get(i) + data[second].Get(i));
            }
        }
    }

    //сложение двух столбцов,первый индекс - того,к которому прибавляем столбец со вторым индексом+
    void additionoftwoColumns(int first,int second){
        if(first >= n || second >= n){
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
    Matrix<T>* Sum(Matrix<T>& first,Matrix<T>& second){
        if(first.getM() != second.getM() || first.getN() != second.getN()){
            isFailed = 1;
            std::cout << "Can't sum";
            return nullptr;
        }else{

            T** tmp = (T**)malloc(sizeof(T*)*m);
            for (int i = 0; i < m; ++i) {
                tmp[i] = (T*)malloc(sizeof(T)*n);
            }

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    tmp[i][j] = first.Get(i,j) + second.Get(i,j);
                }
            }
            Matrix<T>* result = new Matrix<T>(m,n,tmp);
            for (int i = 0; i < m; ++i) {
                delete[] tmp[i];
            }
            delete[] tmp;
            return result;
        }
    }

    //умножение двух матриц+
    Matrix<T>* Multiply(Matrix<T>& first,Matrix<T>& second){
        if(first.getN() != second.getM()){
            isFailed = 1;
            std::cout << "Can't multiply";
            return nullptr;
        }else{
            int M = first.getM();
            int N = second.getN();
            int Q = first.getN();
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
            Matrix<T>* result = new Matrix<T>(M,N,tmp);
            for (int i = 0; i < M; ++i) {
                delete[] tmp[i];
            }
            delete[] tmp;
            return result;
        }
    }

    T MatrixNorm(){
        if(isFailed == 0){
            T max = 0;
            for (int i = 0; i < n; ++i) {
                max+=data[0].Get(i);
            }
            T tmp;
            for (int i = 0; i < m; ++i) {
                tmp = 0;
                for (int j = 0; j < n; ++j) {
                    tmp+=data[i].Get(j);
                }
                if(tmp >= max)
                    max = tmp;
            }
            return max;
        }

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


#endif //UNIT_MATRIX_H
