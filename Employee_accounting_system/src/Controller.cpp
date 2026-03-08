//
// Created by User on 16.12.2025.
//
#include "Controller.h"
#include <fstream>
#include <string>
#include "MainData.h"


void Controller::write_division(json* array, Data* div, Data* parent) {
    json division;
    division["name"] = div->get_name();
    division["parent"] = parent->get_name();
    json workers = json::array();
    for (const auto& it : div->workers) {
        if (it.second->get_status() != NULL_LEADER) {
            json worker;
            worker["code"] = it.first;
            worker["status"] = (it.second->get_status() == WORKER) ? "W" : "L";
            worker["name"] = it.second->get_name();
            worker["post"] = it.second->get_post();
            worker["salary"] = it.second->get_salary();
            worker["education"] = it.second->get_education();
            worker["year"] = static_cast<int>(it.second->get_year());
            workers.push_back(worker);
        }
    }
    division["workers"] = workers;
    array->push_back(division);
    for (const auto& it : div->workers) {
        if (it.second->get_status() == NULL_LEADER) {
            write_division(array, it.second->get_division().get(), div);
        }
    }
}

void Controller::save(const std::string& filename) {
    json js;
    js["company_name"] = company.get_name();
    json divisions_array = json::array();
    for (const auto& it : company.workers) {
        if (it.second->get_status() == NULL_LEADER) {
            write_division(&divisions_array, it.second->get_division().get(), &company);
        }
    }
    js["divisions"] = divisions_array;
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("не получается открыть файл");
    }
    file << js.dump(4);
    file.close();
}

void Controller::load(const std::string& filename) {
    company.reset();

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("не получается открыть файл");
    }

    try {
        json js = json::parse(file);
        if (!js.contains("company_name"))throw std::runtime_error("неправильный формат JSON");
        company.set_name(js["company_name"]);

        if (!js.contains("divisions") || !js["divisions"].is_array())throw std::runtime_error("неправильный формат JSON");

        for (const auto& division_json : js["divisions"]) {
            if (!division_json.contains("name") || !division_json.contains("parent"))throw std::runtime_error("неправильный формат JSON");
            company.add_division(division_json["name"], division_json["parent"]);

            if (division_json.contains("workers") && division_json["workers"].is_array()) {
                for (const auto& worker : division_json["workers"]) {
                    if (!worker.contains("code") || !worker.contains("name") ||
                       !worker.contains("post") || !worker.contains("salary") ||
                       !worker.contains("education") || !worker.contains("year") ||
                       !worker.contains("status")) {
                        throw std::runtime_error("Worker JSON object missing required fields");
                       }
                    status type = (worker["status"] == "W") ? WORKER : LEADER;
                   /* int code = worker["code"];
                    string name = worker["name"];
                    string post = worker["post"];
                    double salary = worker["salary"];
                    string education = worker["education"];
                    year y = static_cast<year>(worker["year"]);
                    string div_name = division_json["name"];
                    add_worker(code, name, post, salary, education, y, type, div_name);*/
                    add_worker(worker["code"], worker["name"], worker["post"], worker["salary"],
                        worker["education"], static_cast<year>(worker["year"]), type, division_json["name"]);
                }
            }
        }
    }
    catch (const json::parse_error& e) {
        throw std::runtime_error("error: " + std::string(e.what()));
    }
    file.close();
}

void Controller::promotion(size_t code) {
    I_Worker* worker = company.find_worker(code);
    if (worker == nullptr)throw std::logic_error("Worker is not found");
    if (worker->get_status() == LEADER)throw std::logic_error("It is already leader");
    worker_info info = company.erase_worker(code, false);
    shared_ptr<Leader> leader = make_shared<Leader>(Leader(info.worker));
    leader->set_division(info.div);
    company.workers[code] = leader;
    info.div->write_worker(leader, code);
    info.reset();
}

void Controller::add_worker(int code, const string &username, const string &post, double salary, const string &education, year birth_year, status type, const string &div)  {
    if (type == NULL_LEADER) throw std::logic_error("NULL LEADER = технический тип");
    if (company.find_worker(code) != nullptr) throw std::logic_error("Сотрудник с таким шифром есть");
    shared_ptr<I_Worker> worker;
    if (type == LEADER) {
        worker = make_shared<Leader>(username, post, salary, education, birth_year);
    }
    else{worker = make_shared<Worker>(username, post, salary, education, birth_year);}
    company.write_worker(worker, code, div, false);
}

void Controller::find_subordinates(int code, std::ostream &o) {
    I_Worker* worker = company.find_worker(code);
    if (worker == nullptr)throw std::logic_error("Worker is not found");
    if (worker->get_status() == WORKER || worker->get_status() == NULL_LEADER)throw std::logic_error("It is not Leader");
    worker->get_division()->print_subordinates(o, true);
}



void find_all_div(shared_ptr<Data> div, std::vector<shared_ptr<Data>>& all_divisions) {
    all_divisions.push_back(div);
    for (const auto& it : div->workers) {
        if (it.second->get_status() == NULL_LEADER) find_all_div(it.second->get_division(), all_divisions);
    }
}

string write_div_parallel(shared_ptr<Data> div) {
    string s;
    for (const auto& it : div->workers) {
        if (it.second->get_status() == WORKER) s += it.second->get_name() + "\n";
    }
    return s;
}

void Controller::find_subordinates_parallel(int code, std::ostream &o) {
    I_Worker* worker = company.find_worker(code);
    if (worker == nullptr) throw std::logic_error("Worker is not found");
    if (worker->get_status() == WORKER || worker->get_status() == NULL_LEADER)throw std::logic_error("It is not Leader");

    auto root_division = worker->get_division();
    std::vector<shared_ptr<Data>> all_divisions;
    find_all_div(root_division, all_divisions);

    std::vector<std::future<string>> futures;

    for (const auto& div : all_divisions) {
        futures.push_back(std::async(std::launch::async, write_div_parallel, div));
    }

    for (auto& future : futures) {
        o << future.get();
    }
}

void Controller::write_time(int code, std::ostream &o) {
    size_t total = 0;
    for (size_t i = 0; i < 10; i++) {
        auto start = std::chrono::steady_clock::now();
        find_subordinates(code, o);
        auto end = std::chrono::steady_clock::now();
        total += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
    size_t total2 = 0;
    for (size_t i = 0; i < 10; i++) {
        auto start = std::chrono::steady_clock::now();
        find_subordinates_parallel(code, o);
        auto end = std::chrono::steady_clock::now();
        total2 += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
    o << "TIME Обычная функция:" << total / 10 << endl;
    o << "TIME Многопоточная функция:" << total2 / 10 << endl;
}