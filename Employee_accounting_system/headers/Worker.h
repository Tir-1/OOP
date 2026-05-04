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
    Worker(string username,string post, double salary, string education, const year& birth_year):
    username(std::move(username)), post(std::move(post)), salary(salary), education(std::move(education)), birth_year(birth_year){type = WORKER;}

    [[nodiscard]] string get_post() const override;
    [[nodiscard]] double get_salary() const override;
    [[nodiscard]] string get_name() const override;
    [[nodiscard]] string get_education() const override;
    [[nodiscard]] year get_year() const override;

    void set_post(const string& new_post) override;
    void set_salary(double new_salary) override;


    [[nodiscard]] status get_status() const override;
    [[nodiscard]] shared_ptr<Data> get_division() const override {return nullptr;}
    void set_division(shared_ptr<Data> new_div) override {};
};
#endif //INC_3_EMPLOYEE_H