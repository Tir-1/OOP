//
// Created by User on 05.11.2025.
//

#ifndef INC_3_TABLE_H
#define INC_3_TABLE_H

#include <cstdint>
#include <stdexcept>
#include "TableIterator.h"
/**
 * @brief Шаблонная хеш-таблица
 * @tparam Key Тип ключа
 * @tparam Value Тип значения
 * @tparam FuncHash Функтор хеширования
 */
template<typename Key, typename Value, typename FuncHash >
class Table {
    Node<Key, Value>* data;
    FuncHash hash;
    size_t size;
    size_t capacity;

    using iterator = TableIterator<Key, Value, false>;
    using const_iterator = TableIterator<Key, Value, true>;
    /**
    * @brief Увеличивает вместимость таблицы в 2 раза
    */
    void table_allocate() {
        size_t new_capacity = 2 * capacity;
        Node<Key, Value>* temp;
        try {
            temp = new Node<Key, Value>[new_capacity];
            for (size_t i = 0; i < capacity; ++i) {
                if (data[i].if_used == 1) {
                    const Key& k = data[i].element.first;
                    Value& v = data[i].element.second;
                    size_t h = hash(k);
                    size_t j = 0;
                    while (j < new_capacity) {
                        size_t index = (h + j) % new_capacity;
                        if (temp[index].if_used != 1) {
                           new (&temp[index].element) std::pair<const Key, Value>(k, move(v));
                            temp[index].if_used = 1;
                            break;
                        }
                        j++;
                    }
                }
            }
        }
        catch (std::bad_alloc& e) {delete [] data; delete [] temp;throw e;}
        delete[] data;
        data=temp;
        capacity = new_capacity;
    }
    /**
     * @brief Находит индекс элемента по ключу
     * @param k Ключ для поиска
     * @return Индекс элемента или capacity если не найден
     */
    size_t find_index(const Key& k) const {
        size_t first_index = hash(k) % capacity;
        size_t index = first_index;
        size_t i = 1;
        do {
            Node<Key, Value>* node = data + index;
            if(node->if_used == 0)break;
            if (node->if_used == 1 && node->element.first == k) {
                return index;
            }
            index = (first_index + i)%capacity;
            i++;
        }while (index != first_index);
        return capacity;
    }

public:
    /**
     * @brief Конструктор таблицы
     * Создает таблицу с начальной вместимостью 16.
     */
    Table():size(0), capacity(16){data = new Node<Key, Value>[capacity];}
    /**
     * @brief Деструктор таблицы
     */
    ~Table() {
        delete[] data;
    }
    /**
     * @brief Возвращает итератор на начало таблицы
     * @return Итератор на первый элемент
     */
    iterator begin() {
        return iterator(data, data + capacity);
    }
    /**
     * @brief Возвращает итератор на конец таблицы
     * @return Итератор на элемент после последнего
     */
    iterator end() {
        return iterator(data + capacity, data + capacity);
    }
    /**
     * @brief Возвращает константный итератор на начало таблицы
     * @return Константный итератор на первый элемент
     */
    const_iterator begin() const {
        return const_iterator(data, data + capacity);
    }
    /**
     * @brief Возвращает константный итератор на конец таблицы
     * @return Константный итератор на элемент после последнего
     */
    const_iterator end() const {
        return const_iterator(data + capacity, data + capacity);
    }
    /**
     * @brief Находит элемент по ключу
     * @param k Ключ для поиска
     * @return Константный итератор на найденный элемент или end() если не найден
     */
    const_iterator find(const Key& k) const {//В поиске мы идём до первого нуля, а не удалённого
        if (size == 0)return end();
        size_t index = find_index(k);
        if (index != capacity) {
            return const_iterator(data + index, data + capacity);
        }
        return end();
    }
    /**
     * @brief Находит элемент по ключу
     * @param k Ключ для поиска
     * @return Итератор на найденный элемент или end() если не найден
     */
    iterator find(const Key& k)  {
        if (size == 0)return end();
        size_t index = find_index(k);
        if (index != capacity) {
            return iterator(data + index, data + capacity);
        }
        return end();
    }
    /**
     * @brief Вставляет элемент в таблицу
     * @param k Ключ элемента
     * @param v Значение элемента
     * @return Итератор на вставленный элемент или end() если не удалось вставить
     */
    iterator insert(const Key& k, Value&& v) {
        if (size * 2 >= capacity)table_allocate();
        size_t h = hash(k);
        size_t i = 0;
        int index_del = -1;
        while (i < capacity) {
            size_t index = (h + i) % capacity;
            i++;
            if (index_del == -1 && data[index].if_used == -1)index_del = index;
            if (data[index].if_used == 0) {
                if (index_del != -1) {
                    index = index_del;
                }
                new (&data[index].element) std::pair<const Key, Value>(k, std::move(v));
                data[index].if_used = 1;
                size++;
                return iterator(data + index, data + capacity);
            }
            if (data[index].if_used == 1 && data[index].element.first == k) {return iterator(data + index, data + capacity);}
        }
        return end();
    }
    /**
     * @brief Удаляет элемент по ключу
     * @param k Ключ элемента для удаления
     * @return 1 если элемент удален, 0 если не найден
     */
    size_t erase(const Key& k) {
        size_t index = find_index(k);
        if (index != capacity) {
            data[index].if_used = -1;
            size--;
            return 1;
        }
        return 0;
    }
    /**
     * @brief Удаляет элемент по итератору
     * @param iter Итератор на элемент для удаления
     * @return 1 если элемент удален, 0 если итератор невалиден
     */
    size_t erase(iterator iter) {
        Node<Key, Value>* node = iter.get_node();
        if (node != data + capacity && node->if_used == 1) {
            node->if_used = -1;
            size--;
            return 1;
        }
        return 0;
    }
    /**
     * @brief Оператор доступа по ключу
     * @param k Ключ элемента
     * @return Ссылка на значение элемента
     * Если элемент не существует, то создает его
     */
    Value& operator[](const Key& k) {
        size_t index = find_index(k);
        if (index == capacity) {
            auto it = insert(k, Value());
            return it->second;
        }
        return data[index].element.second;

    }
    /**
     * @brief Очищает таблицу
     */
    void clear() {
        for (size_t i = 0; i < capacity; ++i) {
            data[i].if_used = 0;
        }
        size = 0;
    }
    /**
     * @brief Возвращает количество элементов в таблице
     * @return Количество элементов
     */
    [[nodiscard]] size_t get_size() const {
        return size;
    }
};



#endif //INC_3_TABLE_H
