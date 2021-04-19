//
// Created by Dom on 09.04.2021.
//

#ifndef UNIT_DYNAMICARRAY_H
#define UNIT_DYNAMICARRAY_H


template <class T>
class DynamicArray{
private:
    //поля
    T* m_data;
    int m_size;//количество элементов
    int m_capacity;//количество свободных ячеек
public:
    //создание
    DynamicArray(){//конструктор по умолчанию
        m_data = NULL;
        m_size = 0;
        m_capacity = 0;
    }


    void Delete(){
        delete[] this->m_data;
    }
    DynamicArray(int size){//конструктор с числом элементов
        m_size = size;
        m_capacity = size;
        m_data = new T[size];
    }

    DynamicArray(T* items,int count){//копировать элементы
        if(items == nullptr){
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
        }
        else{
            m_size = count;			//из переданного массива
            m_capacity = count;
            m_data = new T[count];
            for (int i = 0; i < count; ++i)
            {
                m_data[i] = items[i];
            }
        }
    }

    DynamicArray(const DynamicArray<T> &dynamicArray){//копирующий конструктор
        if(&dynamicArray == nullptr){
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
        }
        else{
            m_size = dynamicArray.m_size;
            m_capacity = m_size;
            m_data = new T[m_size];
            for (int i = 0; i < dynamicArray.m_size; ++i)
            {
                m_data[i] = dynamicArray.m_data[i];
            }
        }
    }

    //операции
    void Resize(int newSize){//перераспределение памяти
        if(newSize > m_capacity){
            int new_capacity = std::max(2 * m_size, newSize);
            T* new_data = new T[new_capacity];
            for (int i = 0; i < m_size; ++i)
            {
                new_data[i] = m_data[i];
            }
            delete[] m_data;
            m_data = new_data;
            m_capacity = new_capacity;
        }
        m_size = newSize;
    }

    T Get(int index){
        if(index < 0 || index >= m_capacity)
            throw "\nGet Message : Index Out Of Range\n";
        //std::cout << "\nGet index = " << index << "\n";
        return m_data[index];
    }

    void Set(int index,T value){//задать элемент по индексу
        if(index < 0 || index > m_capacity)
            throw "\nSet Message : Index Out Of Range\n";
        //std::cout << "\nSet data = " << value << " index = "<< index <<"\n";
        m_data[index] = value;
    }

    int GetSize() const{//получить размер массива
        return m_size;
    }

    void Append(T val){//добавление элемента в конец массива
        std::cout << "\nAppend data = " << val;
        Resize(m_size + 1);//выделение доп памяти при необходимости
        m_data[m_size - 1] = val;
    }

    void Prepend(T val){//добавление элемента в начало массива
        std::cout << "\nPrepend data = " << val << "\n";
        Resize(m_size + 1);//выделение доп памяти при необходимости
        for (int i = m_size - 2; i >= 0; --i)
        {
            m_data[i + 1] = m_data[i];
        }
        m_data[0] = val;
    }

    T & operator [] (int i){//перегрузка оператора "[]" для удобства вывода
        return m_data[i];
    }

    void Print(){
        if(m_size > 0 && m_data != nullptr){
            std::cout << std::endl;
            for (int i = 0; i < m_size; ++i) {
                std::cout << m_data[i] << "\t";
            }
            std::cout << std::endl;
        }
        else{
            std::cout << "\nObject is empty!\n";
        }
    }
};

template <typename T>
std::ostream & operator << (std::ostream & out, DynamicArray<T> a){//перегрузка оператора << для вывода
    for(int i = 0; i < a.GetSize();++i){
        out << a[i] << " ";
    }
    return out;
}


#endif //UNIT_DYNAMICARRAY_H
