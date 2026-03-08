//
// Created by User on 17.12.2025.
//
#include "View.h"
#include "using.h"
#include "Worker.h"
#include "Leader.h"
#include "input.h"
#include <iostream>




void View::run() {
    output << "Введите название предприятия: ";

    string company_name;
    getline(cin, company_name);
    controller = std::make_unique<Controller>(company_name);
    bool running = true;

    while (running) {
        output << "\nПредприятие " + controller->get_name_company() << endl;
        output << "1. Добавить нового сотрудника" << endl;
        output << "2. Добавить новое подразделение" << endl;
        output << "3. Удалить сотрудника по коду" << endl;
        output << "4. Найти сотрудника по коду" << endl;
        output << "5. Вывести сотрудников подразделения" << endl;
        output << "6. Сохранить состояние в файл" << endl;
        output << "7. Загрузить состояние из файла" << endl;
        output << "8. Повысить сотрудника до лидера" << endl;
        output << "9. Вывести подчинённых" << endl;
        output << "10. Вывести подчинённых с помощью параллельной функции" << endl;
        output << "11. Таймирование" << endl;
        output << "0. Выйти из программы" << endl;

        int choice;
        question(choice, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
        try {
            switch (choice) {
                case 1:
                    add_worker_menu();
                    break;
                case 2:
                    add_division_menu();
                    break;
                case 3:
                    remove_worker_menu();
                    break;
                case 4:
                    find_worker_menu();
                    break;
                case 5:
                    show_division_workers_menu();
                    break;
                case 6:
                    save_menu();
                    break;
                case 7:
                    load_menu();
                    break;
                case 8:
                    promotion_menu();
                    break;
                case 9:
                    find_subordinates_menu();
                    break;
                case 10:
                    find_subordinates_parallel_menu();
                    break;
                case 11:
                    time_menu();
                    break;
                case 0:
                    running = false;
                    break;
            }
        }
        catch (const std::exception& e) {cout << e.what() << endl;}
    }
}

void View::add_worker_menu() {
    output << "Введите код сотрудника (целое число): ";
    int code;
    Input(code);
    output << "Введите имя сотрудника: ";
    string name;
    Input_str(name);
    output << "Введите должность: ";
    string post;
    Input_str(post);
    output << "Введите зарплату: ";
    double salary;
    {
        double temp;
        cin >> temp;
        if (!cin.good()) {
            clear();
            output << "Неверный формат зарплаты!" << endl;
            return;
        }
        salary = temp;
        clear();
    }
    output << "Введите образование: ";
    string education;
    Input_str(education);
    output << "Введите год рождения: ";
    int year_int;
    Input(year_int);
    year birth_year{year_int};
    output << "Выберите тип сотрудника:" << endl;
    output << "1. Обычный работник (WORKER)" << endl;
    output << "2. Руководитель (LEADER)" << endl;
    output << "введите 1 или 2" << endl;

    int type_choice;
    question(type_choice, {1, 2});
    status worker_type = (type_choice == 1) ? WORKER : LEADER;
    output << "Введите название подразделения, где работает сотрудник: ";
    string division_name;
    Input_str(division_name);
    try {
        controller->add_worker(code, name, post, salary, education, birth_year, worker_type, division_name);
        output << "Сотрудник успешно добавлен!" << endl;
    } catch (const std::exception& e) {
        output << "Ошибка при добавлении сотрудника: " << e.what() << endl;
    }
}

void View::add_division_menu() {
    output << "Введите название нового подразделения: ";
    string div_name;
    Input_str(div_name);

    output << "Введите название родительского подразделения\n";
    string parent_name;
    Input_str(parent_name);

    try {
        controller->add_division(div_name, parent_name);
        output << "Подразделение успешно добавлено!" << endl;
    } catch (const std::exception& e) {
        output << "Ошибка при добавлении подразделения: " << e.what() << endl;
    }
}


void View::remove_worker_menu() {
    output << "Введите код сотрудника для удаления: ";
    int code;
    Input(code);
    try {
        controller->erase_worker(code);
    } catch (const std::exception& e) {
        output << "Ошибка при удалении сотрудника: " << e.what() << endl;
    }
}

void View::find_worker_menu(){
    output << "Введите код сотрудника для поиска: ";
    int code;
    Input(code);
    try {
        I_Worker* worker = controller->find_worker(code);
        if (worker) {
            output << *worker;
        } else {
            output << "Сотрудник не найден." << endl;
        }
    } catch (const std::exception& e) {
        output << "Ошибка при поиске сотрудника: " << e.what() << endl;
    }
}
void View::promotion_menu() {
    output << "Введите код сотрудника, которого вы хотите повысить" << endl;
    int code;
    Input(code);

    try {
        controller->promotion(code);
    }
    catch (const std::exception& e) {
        output << "Ошибка " << e.what() << endl;
    }
}

void View::show_division_workers_menu() {
    output << "Введите название подразделения: ";
    string division_name;
    Input_str(division_name);
    try {
        auto division = controller->find_division(division_name);
        if (!division) {
            output << "Подразделение '" << division_name << "' не найдено." << endl;
            return;
        }

        output << *division;
    } catch (const std::exception& e) {
        output << "Ошибка: " << e.what() << endl;
    }
}

void View::find_subordinates_menu() {
    size_t code;
    output << "Введите код лидера, подчинённых которого вы хотите найти";
    Input(code);
    controller->find_subordinates(code, output);
}

void View::find_subordinates_parallel_menu() {
    size_t code;
    output << "Введите код лидера, подчинённых которого вы хотите найти";
    Input(code);
    controller->find_subordinates_parallel(code, output);
}

void View::time_menu() {
    size_t code;
    output << "Введите код лидера, подчинённых которого вы хотите найти";
    Input(code);
    controller->write_time(code, output);
}


void View::save_menu() {
   output << "Введите имя файла для сохранения: ";
    string filename;
    Input_str(filename);
    try {
        controller->save(filename);
        cout << "Состояние успешно сохранено в файл '" << filename << "'" << endl;
    } catch (const std::exception& e) {
        cout << "Ошибка при сохранении: " << e.what() << endl;
    }
}


void View::load_menu() {
    output << "Введите имя файла для загрузки: ";
    string filename;
    Input_str(filename);

    output << "Текущие данные будут потеряны" << endl;
    output << "Продолжить? 1 - Да, 0 - Нет  ";

    int choice;
    question(choice, {0, 1});

    if (choice == 0) {
        output << "Загрузка отменена" << endl;
        return;
    }
    try {
        controller->load(filename);
        output << "Загружено предприятие: " << controller->get_name_company() << endl;
    } catch (const std::exception& e) {
        output << "Ошибка при загрузке: " << e.what() << endl;
    }
}

