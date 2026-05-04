//
// Created by User on 17.12.2025.
//

#ifndef LAB3_VIEW_H
#define LAB3_VIEW_H
#include "Controller.h"
#include "input.h"
#include <memory>
/**
 * @brief Класс для взаимодействия с пользователем
 * Реализует консольный интерфейс
 */
class View {
private:
    unique_ptr<Controller> controller;
    std::ostream& output;

    /**
     * @brief Меню добавления работника
     */
    void add_worker_menu();
    /**
     * @brief Меню добавления подразделения
     */
    void add_division_menu();
    /**
     * @brief Меню удаления работника
     */
    void remove_worker_menu();
    /**
     * @brief Меню поиска работника
     */
    void find_worker_menu();
    /**
     * @brief Меню повышения работника
     */
    void promotion_menu();
    /**
     * @brief Меню отображения работников подразделения
     */
    void show_division_workers_menu();
    /**
     * @brief Меню сохранения данных
     */
    void save_menu();
    /**
     * @brief Меню загрузки данных
     */
    void load_menu();
    /**
     * @brief Меню поиска подчиненных
     */
    void find_subordinates_menu();
    /**
     * @brief Меню поиска подчиненных многопоточный функция
     */
    void find_subordinates_parallel_menu();
    /**
     * @brief Меню измерения времени выполнения параллельной и обычной функции
     */
    void time_menu();


public:
    /**
     * @brief Конструктор View
     * @param o Поток вывода
     */
    explicit View(std::ostream& o):output(o){};
    /**
     * @brief Запускает главный цикл приложения
     */
    void run();
    View(): output(std::cout){};
};
#endif //LAB3_VIEW_H