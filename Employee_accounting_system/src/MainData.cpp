//
// Created by User on 01.12.2025.
//
#include "../headers/MainData.h"


void MainData::write_worker(shared_ptr<I_Worker> worker, int code, const string& div, bool add_if_null) {//check = true
    shared_ptr<Data> div_ptr = find_division(div);
    if(div_ptr == nullptr)throw std::logic_error("Division is not find");
    worker->set_division(div_ptr);
    workers[code] = worker;
    div_ptr->write_worker(worker, code);
}

void MainData::add_division(const string& name, const string& where) {
    if(find_division(name) != nullptr)throw std::logic_error("division with this name already be");
    auto leader = make_shared<Leader>(NULL_LEADER);
    shared_ptr<Data> data = make_shared<Data>(name);
    leader->set_division(data);
    if (where == this->name) {
        workers[key_div] = leader;
        key_div--;
        return;
    }
    find_division(where)->add_div(leader);
}

shared_ptr<Data> MainData::find_division(const string& name) {
    for (const auto& it : workers) {
        shared_ptr<I_Worker> worker = it.second;

        if (worker->get_status() == NULL_LEADER) {
            shared_ptr<Data> div = worker->get_division();
            if (div->get_name() == name)return div;
            shared_ptr<Data> found = div->find_division(name);
            if (found)return found;
        }
    }
    return nullptr;
}

shared_ptr<Data> MainData::find_worker_division(int code) {
        auto worker_it = workers.find(code);
        if (worker_it == workers.end()) return nullptr;
        auto worker = worker_it->second;
        auto div = worker->get_division();
        if (div)return div;

        for (const auto& it : workers) {
            worker = it.second;
            if (worker->get_status() == NULL_LEADER) {
                auto division = worker->get_division();
                auto result = division->find_worker_division(code);
                if (result) return result;
            }
        }
    return nullptr;
}


worker_info MainData::erase_worker(size_t code, bool flag_delete) {//flag = true
    auto it = workers.find(code);
    if (it == workers.end())throw std::logic_error("not find");
    shared_ptr<Data> div = find_worker_division(code);
    div->erase_worker(code);
    worker_info info(move(it->second), div, it->first);
    workers.erase(it);
    if (flag_delete)info.reset();
    return info;
}

