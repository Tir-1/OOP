//
// Created by User on 05.11.2025.
//

#ifndef INC_3_EMPLOYEE_H
#define INC_3_EMPLOYEE_H
#include "I_Worker.h"

class Worker : public I_Worker {
private:
    string username;
    string post;
    double salary;
    string education;
    year birth_year;
    status type;

protected:
    void show(std::ostream& o) const override;
public:
    Worker(const string& username, const string& post, double salary, const string& education, const year& birth_year):
    username(username), post(post), salary(salary), education(education), birth_year(birth_year){type = WORKER;}

    string get_post() const override;
    double get_salary() const override;
    string get_name() const override;
    string get_education() const override;
    year get_year() const override;

    void set_post(const string& new_post) override;
    void set_salary(double new_salary) override;


    status get_status() const override;
    shared_ptr<Data> get_division() const override {return nullptr;}
    void set_division(shared_ptr<Data> new_div) override {return;};

    void promote();
};
#endif //INC_3_EMPLOYEE_H