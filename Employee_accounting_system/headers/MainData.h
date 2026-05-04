//
// Created by User on 01.12.2025.
//

#ifndef INC_3_MAINDATA_H
#define INC_3_MAINDATA_H
#include "Data.h"
#include "../headers/Leader.h"
class MainData: public Data {
public:
    explicit MainData(const string& name): Data(name){};
    ~MainData() {delete_data();}


    void write_worker(shared_ptr<I_Worker> worker, int code, const string& div, bool add_if_null=true);
    shared_ptr<Data> find_division(const string& name);
    void add_division(const string& name, const string& where);
    shared_ptr<Data> find_worker_division(int code);

    worker_info erase_worker(size_t code, bool flag_delete=true);

    void set_name(const string& n){name = n;}

    void reset() {
        workers.clear();
        key_div = -1;
    }
};
#endif //INC_3_MAINDATA_H