//
// Created by User on 05.11.2025.
//

#ifndef INC_3_WORKER_H
#define INC_3_WORKER_H
#include "using.h"
#include <chrono>



typedef std::chrono::year year;
enum status {
    WORKER,
    LEADER,
    NULL_LEADER
};

inline string string_status(status st) {
    if (st == WORKER) {return "Служащий подразделения";}
    return "Руководитель подразделения";
}

class Data;


class I_Worker {
protected:
    virtual void show(std::ostream& o) const = 0;
public:
    [[nodiscard]] virtual string get_post() const = 0;
    [[nodiscard]] virtual double get_salary() const = 0;
    [[nodiscard]] virtual string get_name() const = 0;
    [[nodiscard]] virtual string get_education() const = 0;
    [[nodiscard]] virtual year get_year() const = 0;

    virtual void set_post(const string& new_post) = 0;
    virtual void set_salary(double new_salary) = 0;
    [[nodiscard]] virtual status get_status() const = 0;
    virtual ~I_Worker() = default;
    friend std::ostream& operator<<(std::ostream& o, const I_Worker& worker);

    [[nodiscard]] virtual shared_ptr<Data> get_division() const = 0;
    virtual void set_division(shared_ptr<Data> new_div) = 0;
};

inline std::ostream& operator <<(std::ostream& c, const I_Worker& worker) {
    worker.show(c);
    return c;
}

#endif //INC_3_WORKER_H