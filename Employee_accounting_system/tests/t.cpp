//
// Created by User on 23.12.2025.
//
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "../headers/Controller.h"
#include "../headers/Worker.h"
#include "../headers/Leader.h"
#include "../headers/Data.h"
#include "../headers/Table.h"
#include <sstream>
#include <fstream>
#include <chrono>
using namespace std::chrono;

using Catch::Matchers::ContainsSubstring;

TEST_CASE("Тесты для таблицы") {
    Table<int, string> table;
    SECTION("Insert, find") {
        table.insert(1, "A");
        table.insert(2, "B");

        auto it1 = table.find(1);
        REQUIRE(it1 != table.end());
        REQUIRE(it1->second == "A");

        auto it2 = table.find(2);
        REQUIRE(it2 != table.end());
        REQUIRE(it2->second == "B");

        auto it3 = table.find(3);
        REQUIRE(it3 == table.end());
    }

    SECTION("Operator[]") {
        table[1] = "1";
        table[2] = "2";

        REQUIRE(table[1] == "1");
        REQUIRE(table[2] == "2");
    }

    SECTION("Erase") {
        table.insert(1, "First");
        table.insert(2, "Second");
        REQUIRE(table.get_size() == 2);
        REQUIRE(table.erase(1) == 1);
        REQUIRE(table.get_size() == 1);
        auto it = table.find(1);
        REQUIRE(it == table.end());
        it = table.find(2);
        REQUIRE(it != table.end());
    }

    SECTION("Clear") {
        table.insert(1, "First");
        table.insert(2, "Second");
        table.insert(3, "Third");
        REQUIRE(table.get_size() == 3);
        table.clear();
        REQUIRE(table.get_size() == 0);
        REQUIRE(table.find(1) == table.end());
    }

    SECTION("Iterator operations") {
        table.insert(1, "A");
        table.insert(2, "B");
        table.insert(3, "C");
        int count = 0;
        for (const auto& it : table) {
            count++;
            switch (count) {
                case 1:
                    REQUIRE(it.second == "A");
                    break;
                case 2:
                    REQUIRE(it.second == "B");
                    break;
                case 3:
                    REQUIRE(it.second == "C");
                    break;
            }
        }
        REQUIRE(count == 3);
    }
    SECTION("allocate") {
        Table<int, std::string> table;

        for (int i = 0; i < 1000; i++) {
            table.insert(i, "Value" + std::to_string(i));
        }
        REQUIRE(table.get_size() == 1000);
        for (int i = 0; i < 1000; i++) {
            auto it = table.find(i);
            REQUIRE(it != table.end());
            REQUIRE(it->second == "Value" + std::to_string(i));
        }
        for (int i = 0; i < 500; i++) {
            table.erase(i);
        }
        REQUIRE(table.get_size() == 500);
        for (int i = 1000; i < 1500; i++) {
            table.insert(i, "Value" + std::to_string(i));
        }
        REQUIRE(table.get_size() == 1000);
    }
}

TEST_CASE("Worker") {


    Worker worker("Name", "Developer", 50000.0, "gh", year{1976});

    SECTION("Getters return correct values") {
        REQUIRE(worker.get_name() == "Name");
        REQUIRE(worker.get_post() == "Developer");
        REQUIRE(worker.get_salary() == 50000.0);
        REQUIRE(worker.get_education() == "gh");
        REQUIRE(static_cast<int>(worker.get_year()) == 1976);
        REQUIRE(worker.get_status() == WORKER);
    }

    SECTION("Setters") {
        worker.set_post("Senior");
        worker.set_salary(60000.0);
        REQUIRE(worker.get_post() == "Senior");
        REQUIRE(worker.get_salary() == 60000.0);
    }

    SECTION("Output stream operator") {
        std::stringstream s;
        s << worker;

        REQUIRE_THAT(s.str(), ContainsSubstring("Name"));
        REQUIRE_THAT(s.str(), ContainsSubstring("Developer"));
        REQUIRE_THAT(s.str(), ContainsSubstring("Служащий подразделения"));
    }
}

TEST_CASE("Leader class") {


    Leader leader("Name", "Manager", 34.6, "gh", year{1976});

    SECTION("Getters") {
        REQUIRE(leader.get_name() == "Name");
        REQUIRE(leader.get_post() == "Manager");
        REQUIRE(leader.get_salary() == 34.6);
        REQUIRE(leader.get_education() == "gh");
        REQUIRE(static_cast<int>(leader.get_year()) == 1976);
        REQUIRE(leader.get_status() == LEADER);
    }

    SECTION("Set division") {
        auto division = std::make_shared<Data>("IT");
        leader.set_division(division);

        REQUIRE(leader.get_division() == division);
        REQUIRE(leader.get_division()->get_name() == "IT");
    }
    SECTION("Setters") {
        leader.set_post("Senior");
        leader.set_salary(60000.0);
        REQUIRE(leader.get_salary() == 60000.0);
        REQUIRE(leader.get_post() == "Senior");
    }

    SECTION("Copy constructor") {
        auto worker = std::make_shared<Worker>("Name", "Developer", 89.8,
                                             "gh", year{1976});
        Leader promoted_leader(worker);

        REQUIRE(promoted_leader.get_name() == "Name");
        REQUIRE(promoted_leader.get_post() == "Developer");
        REQUIRE(promoted_leader.get_salary() == 89.8);
        REQUIRE(promoted_leader.get_education() == "gh");
        REQUIRE(static_cast<int>(promoted_leader.get_year()) == 1976);
        REQUIRE(promoted_leader.get_status() == LEADER);
    }

    SECTION("NULL_LEADER constructor") {
        Leader null_leader(NULL_LEADER);
        REQUIRE(null_leader.get_status() == NULL_LEADER);
        REQUIRE(null_leader.get_name() == "NULL LEADER");
    }
}

TEST_CASE("Data") {
    auto division = std::make_shared<Data>("IT");

    SECTION("Basic getters") {
        REQUIRE(division->get_name() == "IT");
    }

    SECTION("Worker management") {
        auto worker1 = std::make_shared<Worker>("Name", "Dev", 50000, "gh",
                                              std::chrono::year{1976});
        auto worker2 = std::make_shared<Worker>("Name2", "QA", 45000, "gh",
                                              std::chrono::year{1976});

        division->write_worker(worker1, 1);
        division->write_worker(worker2, 2);

        SECTION("Find worker") {
            auto found = division->find_worker(1);
            REQUIRE(found != nullptr);
            REQUIRE(found->get_name() == "Name");
            REQUIRE(division->find_worker(999) == nullptr);
        }

        SECTION("Erase worker") {
            division->erase_worker(1);
            REQUIRE(division->find_worker(1) == nullptr);
            REQUIRE(division->find_worker(2) != nullptr);
        }
    }

    SECTION("Division hierarchy") {
        auto parent = std::make_shared<Data>("Parent");
        auto child = std::make_shared<Data>("Child");

        auto null_leader = std::make_shared<Leader>(NULL_LEADER);
        null_leader->set_division(child);
        parent->add_div(null_leader);

        SECTION("Find division in hierarchy") {
            auto found = parent->find_division("Child");
            REQUIRE(found != nullptr);
            REQUIRE(found->get_name() == "Child");

            found = parent->find_division("Parent");
            REQUIRE(found != nullptr);
            REQUIRE(found->get_name() == "Parent");

            found = parent->find_division("NТТТТ");
            REQUIRE(found == nullptr);
        }
    }

    SECTION("Worker division search") {
        auto div1 = std::make_shared<Data>("div1");
        auto div2 = std::make_shared<Data>("div2");

        auto null_leader = std::make_shared<Leader>(NULL_LEADER);
        null_leader->set_division(div2);
        div1->add_div(null_leader);

        auto worker = std::make_shared<Worker>("n", "n", 1000, "gh",
                                             std::chrono::year{1976});
        div2->write_worker(worker, 123);

        auto found_division = div1->find_worker_division(123);
        REQUIRE(found_division != nullptr);
        REQUIRE(found_division->get_name() == "div2");

        REQUIRE(div1->find_worker_division(999) == nullptr);
    }
}

TEST_CASE("MainData") {
    MainData company("Test Company");

    SECTION("get name") {
        REQUIRE(company.get_name() == "Test Company");
    }

    SECTION("Division management") {
        company.add_division("IT", "Test Company");

        auto found = company.find_division("IT");
        REQUIRE(found != nullptr);
        REQUIRE(found->get_name() == "IT");

        company.add_division("QA", "IT");
        auto qa = company.find_division("QA");
        REQUIRE(qa != nullptr);
        REQUIRE(qa->get_name() == "QA");

        SECTION("error of add") {
            REQUIRE_THROWS_AS(company.add_division("IT", "Test Company"),std::logic_error);}
    }

    SECTION("Worker operations") {
        company.add_division("IT", "Test Company");

        auto worker = std::make_shared<Worker>("Name", "Dev", 89, "gh",
                                             std::chrono::year{1976});

        company.write_worker(worker, 1, "IT", false);

        SECTION("Find worker") {
            auto found = company.find_worker(1);
            REQUIRE(found != nullptr);
            REQUIRE(found->get_name() == "Name");
            REQUIRE(company.find_worker(999) == nullptr);
        }

        SECTION("Find worker division") {
            auto division = company.find_worker_division(1);
            REQUIRE(division != nullptr);
            REQUIRE(division->get_name() == "IT");
        }

        SECTION("Erase worker") {
            auto info = company.erase_worker(1, false);
            REQUIRE(info.worker != nullptr);
            REQUIRE(info.worker->get_name() == "Name");
            REQUIRE(info.div != nullptr);
            REQUIRE(info.code == 1);

            REQUIRE(company.find_worker(1) == nullptr);
        }

        SECTION("Erase non-existent worker") {
            REQUIRE_THROWS_AS(company.erase_worker(999), std::logic_error);
        }
    }

    SECTION("Reset operation") {
        company.add_division("IT", "Test Company");
        auto worker = std::make_shared<Worker>("Name", "Dev", 89, "gh",
                                             std::chrono::year{1976});
        company.write_worker(worker, 1, "IT", false);

        REQUIRE(company.find_division("IT") != nullptr);
        REQUIRE(company.find_worker(1) != nullptr);
        company.reset();
        REQUIRE(company.find_division("IT") == nullptr);
        REQUIRE(company.find_worker(1) == nullptr);
    }
}

TEST_CASE("Controller") {
    Controller controller("Test Company");

    SECTION("Company name") {
        REQUIRE(controller.get_name_company() == "Test Company");
    }

    SECTION("Add division") {
        controller.add_division("IT", "Test Company");

        auto division = controller.find_division("IT");
        REQUIRE(division != nullptr);
        REQUIRE(division->get_name() == "IT");

        controller.add_division("QA", "IT");
        auto qa = controller.find_division("QA");
        REQUIRE(qa != nullptr);
        REQUIRE(qa->get_name() == "QA");
    }

    SECTION("Add worker") {
        controller.add_division("IT", "Test Company");



        SECTION("Add regular worker") {
            controller.add_worker(12, "Name", "n", 67.8,
                                "gh", year{1976}, WORKER, "IT");

            auto worker = controller.find_worker(12);
            REQUIRE(worker != nullptr);
            REQUIRE(worker->get_name() == "Name");
            REQUIRE(worker->get_post() == "n");
            REQUIRE(worker->get_status() == WORKER);
        }

        SECTION("Add leader") {
            controller.add_worker(13, "Lead", "Manager", 89.7,
                                "gh", year{1976}, LEADER, "IT");

            auto leader = controller.find_worker(13);
            REQUIRE(leader != nullptr);
            REQUIRE(leader->get_name() == "Lead");
            REQUIRE(leader->get_post() == "Manager");
            REQUIRE(leader->get_status() == LEADER);
        }

        SECTION("Add duplicate worker code") {
            controller.add_worker(12, "k", "Dev", 50000, "gh",
                                year{1976}, WORKER, "IT");
            REQUIRE_THROWS_AS(
                controller.add_worker(12, "k", "QA", 45000, "gh",
                                    year{1976}, WORKER, "IT"),
                std::logic_error
            );
        }

        SECTION("Add worker to non-existent division") {
            REQUIRE_THROWS_AS(
                controller.add_worker(103, "Error", "Dev", 34, "gh",
                                    year{1976}, WORKER, "gffg"),
                std::logic_error
            );
        }

        SECTION("Add NULL_LEADER throws") {
            REQUIRE_THROWS_AS(
                controller.add_worker(104, "Null", "N", 789, "N",
                                    year{1976}, NULL_LEADER, "IT"),
                std::logic_error
            );
        }
    }

    SECTION("Promotion") {
        controller.add_division("IT", "Test Company");

        controller.add_worker(1, "Name", "Developer", 50000.0,
                            "gh", year{1976}, WORKER, "IT");

        SECTION("Promote worker to leader") {
            controller.promotion(1);
            auto promoted = controller.find_worker(1);
            REQUIRE(promoted != nullptr);
            REQUIRE(promoted->get_status() == LEADER);
            REQUIRE(promoted->get_name() == "Name");
        }

        SECTION("Promote non-existent worker throws") {
            REQUIRE_THROWS_AS(controller.promotion(3), std::logic_error);
        }
    }

    SECTION("Subordinate operations") {
        controller.add_division("IT", "Test Company");
        controller.add_division("Dev", "IT");
        controller.add_division("QA", "IT");




        controller.add_worker(1, "A", "Manager", 8, "gh",
                            year{1976}, LEADER, "IT");
        controller.add_worker(2, "B", "Developer", 8, "gh",
                            year{1976}, WORKER, "Dev");
        controller.add_worker(3, "C", "Developer", 8, "gh",
                            year{1976}, WORKER, "QA");
        controller.add_worker(4, "D", "Developer", 8, "gh",
                            year{1976}, WORKER, "Dev");

        SECTION("Find subordinates of leader") {
            std::stringstream s;
            controller.find_subordinates(1, s);

            REQUIRE_THAT(s.str(), ContainsSubstring("B"));
            REQUIRE_THAT(s.str(), ContainsSubstring("C"));
            REQUIRE_THAT(s.str(), ContainsSubstring("D"));
        }

        SECTION("Find subordinates of non-leader") {
            std::stringstream s;
            REQUIRE_THROWS_AS(controller.find_subordinates(2, s),
                            std::logic_error);
        }

        SECTION("Find subordinates of non-existent worker") {
            std::stringstream s;
            REQUIRE_THROWS_AS(controller.find_subordinates(999, s),
                            std::logic_error);
        }

        SECTION("Parallel subordinates function") {
            std::stringstream s;
            controller.find_subordinates_parallel(1, s);

            std::string output = s.str();
            REQUIRE_THAT(output, ContainsSubstring("B") ||
                               ContainsSubstring("C") ||
                               ContainsSubstring("D"));
        }
    }
}

TEST_CASE("Controller save/load") {
    const std::string test_filename = "test_company.json";
    SECTION("Save and load operations") {
        {
            Controller controller("Test Company");
            controller.add_division("IT", "Test Company");
            controller.add_division("HR", "Test Company");



            controller.add_worker(1, "Name1", "Developer", 8,
                                "gh", year{1976}, WORKER, "IT");
            controller.add_worker(2, "Name2", "Manager", 8,
                                "gh", year{1976}, LEADER, "HR");

            controller.save(test_filename);
        }

        {
            Controller controller("Temp");
            controller.load(test_filename);

            REQUIRE(controller.get_name_company() == "Test Company");

            auto worker1 = controller.find_worker(1);
            REQUIRE(worker1 != nullptr);
            REQUIRE(worker1->get_name() == "Name1");
            REQUIRE(worker1->get_post() == "Developer");
            REQUIRE(worker1->get_status() == WORKER);

            auto worker2 = controller.find_worker(2);
            REQUIRE(worker2 != nullptr);
            REQUIRE(worker2->get_name() == "Name2");
            REQUIRE(worker2->get_post() == "Manager");
            REQUIRE(worker2->get_status() == LEADER);

            auto it = controller.find_division("IT");
            REQUIRE(it != nullptr);
            auto hr = controller.find_division("HR");
            REQUIRE(hr != nullptr);
        }


        std::remove(test_filename.c_str());
    }

    SECTION("Load non-existent file") {
        Controller controller("Test");
        REQUIRE_THROWS_AS(controller.load("sdfsdfdsdsf"),
                        std::runtime_error);
    }

    SECTION("Load invalid JSON") {
        const std::string file_er = "invalid.json";
        {
            std::ofstream file(file_er);
            file << "sdfsdfsdfsddsf";
        }
        Controller controller("Test");
        REQUIRE_THROWS_AS(controller.load(file_er), std::runtime_error);
        std::remove(file_er.c_str());
    }
}
