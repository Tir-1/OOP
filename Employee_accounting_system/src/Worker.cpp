//
// Created by User on 17.11.2025.
//
#include "Worker.h"
string Worker::get_post() const {
    return post;
}
double Worker::get_salary() const {
    return salary;
}
status Worker::get_status() const {
    return type;
}
string Worker::get_name() const {
    return username;
}
string Worker::get_education() const {return education;}
year Worker::get_year() const {return birth_year;}


void Worker::set_post(const string& new_post) {
    post = new_post;
}
void Worker::set_salary(double new_salary) {
    salary = new_salary;
}

void Worker::show(std::ostream& o) const {
    o << username << endl;
    o << string_status(type) << endl;
    o << post << endl;
    o << salary << endl;
    o << birth_year << endl;
    o << education << endl;
}
