#include <iostream>
#include "sqlite3.h"
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>


int AgePeople(std::string BDay) {
	std::time_t t = std::time(0);
	std::tm now;
	localtime_s(&now, &t);
	int BYear = std::stoi(BDay.substr(0, 4));
	int BMonth = std::stoi(BDay.substr(5, 2));
	int birthDays = std::stoi(BDay.substr(8, 2));
	int age = now.tm_year + 1900 - BYear;
	if (now.tm_mon + 1 < BMonth || (now.tm_mon + 1 == BMonth && now.tm_mday < birthDays)) {
		age--;
	}

	return age;
}



static int returnback(void* data, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		std::cout << azColName[i] << ": " << argv[i] << std::endl;
	}
	std::cout << std::endl;
	return 0;
}


int main() {
	sqlite3* bd;
	int op = sqlite3_open("test.db", &bd);
	if (op) {
		std::cout << "Error opening the database -- " << sqlite3_errmsg(bd) << std::endl;
		return 0;
	}
	else {
		std::cout << "Successfully.The database has been opened" << std::endl;
	}
	std::string createTable = "CREATE TABLE IF NOT EXISTS MyTable (Name TEXT, Birthday TEXT);";
	op = sqlite3_exec(bd, createTable.c_str(), NULL, 0, NULL);
    if (op != SQLITE_OK) {
        std::cout << "Error creating the table: " << sqlite3_errmsg(bd) << std::endl;
        sqlite3_close(bd);
        return 0;
    }
    else {
        std::cout << "Successfully. The table was created " << std::endl;
    }

    int choice;
    do {
        std::cout << "===============" << std::endl;
        std::cout << "1. View database" << std::endl;
        std::cout << "2. Add database" << std::endl;
        std::cout << "3. Delete database" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
        {
            std::string selectQuery = "SELECT * FROM MyTable;";
            op = sqlite3_exec(bd, selectQuery.c_str(), returnback, 0, NULL);

            if (op != SQLITE_OK) {
                std::cout << "Error when executing a data selection request: " << sqlite3_errmsg(bd) << std::endl;
                sqlite3_close(bd);
                return 0;
            }
            break;
        }

        case 2:
        {
            std::cout << "Name? ";
            std::string name;
            std::cin >> name;

            std::cout << "birthday? example - (2003.07.26): ";
            std::string birthday;
            std::cin >> birthday;
            int age = AgePeople(birthday);


            std::string insertQuery = "INSERT INTO MyTable (Name, Birthday) VALUES ('" + name + "', '" + birthday + "');";
            op = sqlite3_exec(bd, insertQuery.c_str(), NULL, 0, NULL);
            if (op != SQLITE_OK) {
                std::cout << "Error when adding: " << sqlite3_errmsg(bd) << std::endl;
                sqlite3_close(bd);
                return 0;
            }
            else {
                std::cout << "Successfully. Data has been added " << std::endl;
            }
            break;
        }
        case 3:

        {
            std::cout << "Enter the name to delete: ";
            std::string name;
            std::cin >> name;
                std::string deleteQuery = "DELETE FROM MyTable WHERE Name = '" + name + "';";
            op = sqlite3_exec(bd, deleteQuery.c_str(), NULL, 0, NULL);

            if (op != SQLITE_OK) {
                std::cout << "Error when deleting data from the table: " << sqlite3_errmsg(bd) << std::endl;
                sqlite3_close(bd);
                return 0;
            }
            else {
                std::cout << "The data has been deleted successfully" << std::endl;
            }
            break;
        }
        case 4:
            std::cout << "Exiting the program..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    } while (choice != 4);


    sqlite3_close(bd);

    return 0;
}