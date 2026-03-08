using std::pair;
/**
 * @brief Структура узла хеш-таблицы
 * @tparam Key Тип ключа
 * @tparam Value Тип значения
 */
template<typename Key, typename Value>
struct Node {
    std::pair<const Key, Value> element;
    char if_used;//-1 удалено, 0 пусто, 1 занято
    /**
    * @brief Конструктор узла
    */
    Node(){if_used = 0;}
};

template<typename Key, typename Value, typename FuncHash=std::hash<Key> >
    class Table;
/**
 * @brief Итератор для хеш-таблицы
 * @tparam Key Тип ключа
 * @tparam Value Тип значения
 * @tparam is_const Флаг константности итератора
 */
template<typename Key, typename Value, bool is_const>
class TableIterator {
private:
    typedef std::conditional_t<is_const, const Node<Key, Value>, Node<Key, Value>>* ptr;
    ptr current;
    ptr end; // указатель на память за последним элементом
    TableIterator():current(nullptr), end(nullptr) {}
    /**
    * @brief Конструктор итератора
    * @param start Начальная позиция
    * @param end Конечная позиция
    */
    TableIterator(ptr start, ptr end):current(start), end(end) {
        while (current < this->end && current->if_used != 1) {
            ++current;
        }
    };
    /**
    * @brief Возвращает указатель на текущий узел
    * @return Указатель на узел
    */
    ptr get_node() const {
        return current;
    }

public:
    friend class Table<Key, Value>;
    friend class TableIterator<Key, Value, !is_const>;//Другой итератор является дружественным
    using iterator_category = std::forward_iterator_tag;
    using iterator_concept = std::forward_iterator_tag;

    typedef ptrdiff_t difference_type;
    typedef pair<const Key, Value> value_type;

    typedef std::conditional_t<is_const, const value_type, value_type>* pointer;
    typedef std::conditional_t<is_const, const value_type, value_type>& reference;
    typedef std::forward_iterator_tag iterator_tag;

    /**
    * @brief Конструктор преобразования из итератора другого типа константности
    * @tparam other_const Константность другого итератора
    * @param other Другой итератор
    * @requires (is_const >= other_const) Преобразование только в константный итератор
    */
    template<bool other_const>
    TableIterator(const TableIterator<Key, Value, other_const>& other) noexcept
    requires (is_const >= other_const) {
        current = other.current;
        end = other.end;
    }

    TableIterator(const TableIterator&) = default;
    TableIterator& operator=(const TableIterator&) = default;

    /**
     * @brief Оператор присваивания из итератора другого типа константности
     * @tparam other_const Константность другого итератора
     * @param other Другой итератор
     * @return Ссылка на текущий итератор
     * @requires (is_const >= other_const) Присваивание только в константный итератор
     */
    template<bool other_const>
    TableIterator& operator=(const TableIterator<Key, Value, other_const>& other) noexcept
    requires (is_const >= other_const){
        current = other.current;
        end = other.end;
        return *this;
    }
    /**
     * @brief Оператор сравнения итераторов
     * @tparam other_const Константность другого итератора
     * @param other Другой итератор
     * @return true если итераторы равны, false иначе
     */
    template<bool other_const>
    bool operator==(const TableIterator<Key, Value, other_const>& other) const noexcept {
        return current == other.current;
    }
    /**
     * @brief Оператор неравенства итераторов
     * @tparam other_const Константность другого итератора
     * @param other Другой итератор
     * @return true если итераторы не равны, false иначе
     */
    template<bool other_const>
    bool operator!=(const TableIterator<Key, Value, other_const>& other) const noexcept {
        return current != other.current;
    }
    /**
     * @brief Префиксный инкремент итератора
     * @return Ссылка на итератор
     */
    TableIterator& operator++() {
        if (current >= end)return *this;
        ++current;
        while (current < end && current->if_used != 1) {
            ++current;
        }
        return *this;
    }
    /**
     * @brief Постфиксный инкремент итератора
     * @return Итератор до увеличения
     */
    TableIterator operator++(int) {
        TableIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    /**
     * @brief Оператор разыменования
     * @return Ссылка на элемент
     */
    reference operator*() {
        return current->element;
    }
    /**
     * @brief Константный оператор разыменования
     * @return Константная ссылка на элемент
     */
    reference operator*() const {
        return current->element;
    }
    /**
     * @brief Оператор доступа к члену
     * @return Указатель на элемент
     */
    pointer operator->() {
        return &current->element;
    }
    /**
     * @brief Константный оператор доступа к члену
     * @return Константный указатель на элемент
     */
    pointer operator->() const{
        return &current->element;
    }
};
