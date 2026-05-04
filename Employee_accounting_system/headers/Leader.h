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
    Leader(string username, string post, double salary, string education, const year& birth_year):username(std::move(username)), post(std::move(post)), salary(salary), birth_year(birth_year),  education(std::move(education)), type(LEADER){}
    explicit Leader(status s):username("NULL LEADER"), type(NULL_LEADER), salary(0), birth_year(0){};
    explicit Leader(const shared_ptr<I_Worker>& other) {
        username = other->get_name();
        post = other->get_post();
        salary = other->get_salary();
        education = other->get_education();
        birth_year = other->get_year();
        type = LEADER;
    }


    [[nodiscard]] string get_post() const override;
    [[nodiscard]] double get_salary() const override;
    [[nodiscard]] status get_status() const override;
    [[nodiscard]] string get_name() const override;
    [[nodiscard]] string get_education() const override;
    [[nodiscard]] year get_year() const override;

    [[nodiscard]] shared_ptr<Data> get_division() const override;

    void set_post(const string& new_post) override;
    void set_salary(double new_salary) override;
    void set_division(shared_ptr<Data> division) override;


};
#endif //INC_3_LEADER_H