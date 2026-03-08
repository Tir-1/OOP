//
// Created by User on 17.11.2025.
//

#include "Leader.h"
#include "Data.h"
string Leader::get_post() const {
    return post;
}
double Leader::get_salary() const {
    return salary;
}
status Leader::get_status() const {
    return type;
}

string Leader::get_name() const {
    return username;
}
shared_ptr<Data> Leader::get_division() const {
    return division;
}
string Leader::get_education() const {return education;}
year Leader::get_year() const {return birth_year;}



void Leader::set_post(const string& new_post) {
    post = new_post;
}
void Leader::set_salary(double new_salary) {
    salary = new_salary;
}
void Leader::set_division(shared_ptr<Data> new_division) {
    division = new_division;
}

void Leader::show(std::ostream& o) const {
    o << username << endl;
    o << string_status(type) << endl;
    o << "Руководит в подразделении: " << division->get_name() << endl;
    o << "Должность " + post << endl;
    o << "Зарплата " << salary << endl;
    o << "Год рождения " << birth_year << endl;
    o << "Образование " +education << endl;
}

