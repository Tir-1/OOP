//
// Created by User on 05.11.2025.
//

#ifndef INC_3_LEADER_H
#define INC_3_LEADER_H
#include "I_Worker.h"
#include "Worker.h"
#include "Data.h"


class Leader : public I_Worker {
private:
    string username;
    string post;
    double salary;
    string education;
    year birth_year;
    status type;
    shared_ptr<Data> division;
protected:
    void show(std::ostream& o) const override;
public:
    Leader(const string& username, const string& post, double salary, const string& education, const year& birth_year):username(username), post(post), salary(salary), birth_year(birth_year),  education(education), type(LEADER){}
    Leader(status s):username("NULL LEADER"), type(NULL_LEADER){};
    Leader(shared_ptr<I_Worker> other) {
        username = other->get_name();
        post = other->get_post();
        salary = other->get_salary();
        education = other->get_education();
        birth_year = other->get_year();
        type = LEADER;
    }


    string get_post() const override;
    double get_salary() const override;
    status get_status() const override;
    string get_name() const override;
    string get_education() const override;
    year get_year() const override;

    shared_ptr<Data> get_division() const override;

    void set_post(const string& new_post) override;
    void set_salary(double new_salary) override;
    void set_division(shared_ptr<Data> division) override;


};
#endif //INC_3_LEADER_H