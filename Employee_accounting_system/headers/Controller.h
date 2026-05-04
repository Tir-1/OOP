//
// Created by User on 16.12.2025.
//

#ifndef LAB3_CONTROLLER_H
#define LAB3_CONTROLLER_H
#include "using.h"
#include "MainData.h"
#include <nlohmann/json.hpp>
#include <future>
using json = nlohmann::json;

class Controller {
private:
    MainData company;
    void write_division(json* array, Data* div, Data* parent);

public:
    /**
    * @brief Конструктор контроллера
    * @param name Название компании
    */
    explicit Controller(const string& name) : company(name) {}

    /**
     * @brief Сохраняет состояние компании в файл json
     * @param filename Имя файла для сохранения
     * @throw std::runtime_error Если файл не октрывается
     */
    void save(const std::string& filename);
    /**
    * @brief Загружает состояние компании из файла
    * @param filename Имя файла для загрузки
    * @throw std::runtime_error Если файл не найден или не подходит по формату
    */
    void load(const std::string& filename);
    /**
    * @brief Повышает сотрудника до руководителя
    * @param code Код сотрудника для повышения
    * @throw std::logic_error Если сотрудник не найден или уже является руководителем
    */
    void promotion(size_t code);
    /**
     * @brief Добавляет нового сотрудника
     * @param code Код сотрудника
     * @param username ФИО сотрудника
     * @param post Должность сотрудника
     * @param salary Зарплата сотрудника
     * @param education Образование сотрудника
     * @param birth_year Год рождения сотрудника
     * @param type Тип сотрудника
     * @param div Название подразделения
     * @throw std::logic_error сотрудник с таким кодом уже существует
     */
    void add_worker(int code, const string& username, const string& post, double salary, const string& education, year birth_year, status type, const string& div);
    /**
     * @brief Находит всех подчиненных руководителя
     * @param code Код руководителя
     * @param o Поток вывода для записи результатов
     * @throw std::logic_error сотрудник не найден или не является руководителем
     */
    void find_subordinates(int code, std::ostream& o);
    /**
     * @brief Находит всех подчиненных руководителя с использованием многопоточности
     * @param code Код руководителя
     * @param o Поток вывода для записи результатов
     * @throw std::logic_error сотрудник не найден или не является руководителем
     */
    void find_subordinates_parallel(int code, std::ostream& o);
    /**
     * @brief Замеряет время выполнения поиска подчиненных обычной и многопоточной функцией
     * @param code Код руководителя
     * @param o Поток вывода для записи результатов
     */
    void write_time(int code, std::ostream& o) ;
    /**
     * @brief Добавляет новое подразделение
     * @param name Название нового подразделения
     * @param where Название родительского подразделения
     */
    void add_division(const string& name, const string& where) {company.add_division(name, where);}
    /**
     * @brief Удаляет сотрудника
     * @param code Код сотрудника для удаления
     */
    void erase_worker(int code) {company.erase_worker(code, true);}
    /**
     * @brief Находит сотрудника по коду
     * @param code Код сотрудника
     * @return Указатель на сотрудника nullptr если не найден
     */
    I_Worker* find_worker(int code) {return company.find_worker(code);}
    /**
     * @brief Находит подразделение по названию
     * @param name Название подразделения
     * @return shared_ptr на подразделение nullptr если не найдено
     */
    shared_ptr<Data> find_division(const string& name) {return company.find_division(name);}
    /**
    * @brief Возвращает название компании
    * @return Название компании
    */
    string get_name_company() const{return company.get_name();}
};
#endif //LAB3_CONTROLLER_H
