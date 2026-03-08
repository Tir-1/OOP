//
// Created by User on 17.11.2025.
//

#include "Data.h"
#include "Leader.h"
using std::move;
using std::make_unique;
string Data::get_name() const {
    return name;
}

void Data::write_worker(shared_ptr<I_Worker> worker, size_t code) {
    workers[code] = worker;
}

void Data::add_div(shared_ptr<I_Worker> leader) {
    workers[key_div] = leader;
    key_div--;
}

shared_ptr<Data> Data::find_division(const string& target_name) {
    if (this->name == target_name) {
        return shared_from_this();
    }
    for (const auto& iter : workers) {
        shared_ptr<I_Worker> worker = iter.second;
        if (worker->get_status() == NULL_LEADER) {
            shared_ptr<Leader> leader = std::dynamic_pointer_cast<Leader>(worker);
            shared_ptr<Data> div = leader->get_division();
            shared_ptr<Data> found = div->find_division(target_name);
            if (found) return found;
        }
    }
    return nullptr;
}

void Data::erase_worker(size_t code) { // используется из main. когда точно известно, что такой работник есть
    auto it = workers.find(code);
    workers.erase(it);
}

I_Worker* Data::find_worker(size_t code) {
    auto it = workers.find(code);
    if (it == workers.end()) return nullptr;
    return (it->second).get();
}



shared_ptr<Data> Data::find_worker_division_recur(int code, bool& found) {
    if (workers.find(code) != workers.end()) {
        found = true;
        return shared_from_this();
    }
    for (const auto& it : workers) {
        shared_ptr<I_Worker> worker = it.second;
        if (worker->get_status() == NULL_LEADER) {
            shared_ptr<Leader> leader = std::dynamic_pointer_cast<Leader>(worker);
            shared_ptr<Data> division = leader->get_division();
            shared_ptr<Data> result = division->find_worker_division_recur(code, found);
            if (found) {
                return result;
            }
        }
    }
    return nullptr;
}

shared_ptr<Data> Data::find_worker_division(int code) {
    bool found = false;
    return find_worker_division_recur(code, found);
}

void Data::print_subordinates(std::ostream& o, bool first) {
    for (const auto& it : workers) {
        if (it.second->get_status() == WORKER || (!first && it.second->get_status() == LEADER) ) {
            o << it.second->get_name() << endl;
        }
    }
    for (const auto& it : workers) {
        if (it.second->get_status() == NULL_LEADER) {
            it.second->get_division()->print_subordinates(o, false);
        }
    }
}

void Data::delete_data() {
    for (auto it : workers) {
        auto worker = it.second;
        if (worker->get_status() == LEADER) {
            worker->set_division(nullptr);
        }
        if (worker->get_status() == NULL_LEADER) {
            worker->get_division()->delete_data();
            worker->set_division(nullptr);
        }
    }
    workers.clear();
}

std::ostream& operator <<(std::ostream& o, const Data& d) {
    o << d.name << endl;
    for (const auto& i : d.workers) {

        if (i.second->get_status() != NULL_LEADER) {
            o << i.first << "  " << i.second->get_name() << endl;
        }
    }
    return o;
}
