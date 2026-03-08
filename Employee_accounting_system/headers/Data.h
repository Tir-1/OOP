//
// Created by User on 05.11.2025.
//

#ifndef INC_3_DATA_H
#define INC_3_DATA_H
#include "using.h"
#include "Table.h"
#include "I_Worker.h"

class Controller;

struct worker_info {
    shared_ptr<I_Worker> worker;
    shared_ptr<Data> div;
    size_t code;
    void reset() {
        worker.reset();
        div.reset();
        code = 0;
    }
};
class Data: public std::enable_shared_from_this<Data> {
protected:
   Table<int, shared_ptr<I_Worker>> workers;
    int key_div;
    string name;
public:
    Data(const string& name):name(name), workers(), key_div(-1){};

    string get_name() const;

    void write_worker(shared_ptr<I_Worker> worker, size_t code);
    I_Worker* find_worker(size_t code);
    void add_div(shared_ptr<I_Worker> leader);
    shared_ptr<Data> find_division(const string& name);
    shared_ptr<Data> find_worker_division_recur(int code, bool& found);
    shared_ptr<Data> find_worker_division(int code);
    void erase_worker(size_t code);
    void print_subordinates(std::ostream& o, bool first);
    friend std::ostream& operator <<(std::ostream& o, const Data& d);
    friend class Controller;
    friend string write_div_parallel(shared_ptr<Data> div);
    friend void find_all_div(shared_ptr<Data> div, std::vector<shared_ptr<Data>>& all_divisions);
    void delete_data();
};


#endif //INC_3_DATA_H