#include <iostream>
#include "sqlite/sqlite3.h"
#include <map>
#include <list>
#include <limits>
using namespace std;
struct User {
    int plan_id;
    string planName;
    string admin_password;
    string planeName;
    string arrival_place;
    string destination_time;
    string destination_place;
    string arrival_time;
    string flight_id;
    string employId;
    string workspace;
    string employName;
    string payment;
    string name;
    string email;
    string password;
    string passportId;
    string userId;
    map<string, list<string>>crew;
    map<string, list<string>>tracks;
    void crew_management();
    void flight_Maintenanc();
    void MaintenanceTracking();
    void registerCustomer();
    void displayOptions();
    void bookTicket();
    void checkIn();
    void database_inserter2();
    
};
void User::displayOptions() {
    cout << "Welcome to the Airplane Management System" << endl;
    char choice3;
    cout << "user or admin"<<endl;
    cout<<"user 1"<<endl;
     cout<<"admin 2"<<endl;
     cout<< "enter your choice: ";
    cin>>choice3;
    if (choice3 == '1') {
        cout<< "Do you want to (1) register, (2) check in a ticket, or (3) book a ticket? (4) display the ticketer information: ";
        char choice;
        cin >> choice;
        switch (choice) {
            case '1':
                registerCustomer();
                break;
            case '2':
                checkIn();
                break;
            case '3':
                bookTicket();
                break;
            default:
                cout << "Invalid choice" << endl;
        }
    }
    else if (choice3=='2') {
        cout << "Enter admin password: ";
        cin >> admin_password;
        if (admin_password == "pass123") {
            char choose;
            cout << "crew management press 1: " << endl;
            cout << "flight management press 2: " << endl;
            cout << "maintenance tracking  press 3: " << endl;
            cin >> choose;
            switch (choose ) {
                case '1':
                    crew_management();
                    break;
                case '2':
                    flight_Maintenanc();
                    break;
                case '3':
                    MaintenanceTracking();
                    break;

                default:
                    cout << "INVALID CHOICE ";
            }
        } else {
            cout << "your  admin password is invalid" << endl;
        }
    }
}

void User::registerCustomer() {
    char choose;
    string name, email, password, passportId;

    cout <<"login to airport register"<<endl;
    cout << "logout from register"<<endl;
    cin>>choose;
    if(choose=='1') {

        cout << "Enter your name: ";
        cin >> name;
        cout << "Enter your email address: ";
        cin >> email;
        cout << "Enter your password: ";
        cin >> password;
        cout << "If you need transportation outside of Ethiopia, we will require your passport." << endl;
        cout << "Press 1 if you have a passport." << endl;
        cout << "Press 2 if you do not have a passport." << endl;
        int choice;
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice" << endl;
            return;
        }
        if (choice == 1) {
            cout << "Enter your passport id: ";
            cin >> passportId;
            cout << "Registration successful!" << endl;
        } else if (choice == 2) {
            cout << "Registration successful!" << endl;
            passportId="0000";
        } else {
            cout << "Invalid choice" << endl;
            return;
        }
        sqlite3* db;
        char* zErrmsg;
        int rc;
        string sql;
        rc = sqlite3_open("user.db", &db);
        if(rc != SQLITE_OK) {
            cout << "can't open database: " << sqlite3_errmsg(db) << endl;
            return;
        }
        // enable foreign key
        rc = sqlite3_exec(db,"PRAGMA FOREIGN_KEY = ON;" ,nullptr,nullptr, &zErrmsg);
        if (rc != SQLITE_OK) {
            cerr << "Can't foreign key: " << zErrmsg << endl;
            sqlite3_free(zErrmsg);
        }
        sql = "CREATE TABLE IF NOT EXISTS REGISTRATION_user("
              "USERID INTEGER PRIMARY KEY AUTOINCREMENT,"
              "PASSWORD CHAR(50) NOT NULL,"
              "NAME TEXT NOT NULL,"
              "EMAIL CHAR(50) UNIQUE,"
              "PASSPORTID CHAR(50) UNIQUE NULL);";
        rc = sqlite3_exec(db,sql.c_str(),NULL,nullptr,&zErrmsg);
        if (rc != SQLITE_OK){
            cout << "sql error: " << zErrmsg << endl;
            sqlite3_free(zErrmsg);
        }
        sql = "INSERT INTO REGISTRATION_user (PASSWORD, NAME, EMAIL, PASSPORTID)"
              "VALUES ('" + password + "', '" + name + "', '" + email + "', '" + passportId + "');";
        rc = sqlite3_exec(db,sql.c_str(),NULL,nullptr,&zErrmsg);
        if(rc != SQLITE_OK) {
            cout << "sql error: "<< zErrmsg << endl;
            sqlite3_free(zErrmsg);
        }
        sqlite3_close(db);
    }

    else if(choose=='2') {
        cout << "Enter Email your: " ;
        cin >> email;
        sqlite3 *db;
        char *ZErrmsg = nullptr;
        int rc = sqlite3_open("user.db", &db);

        if (rc != SQLITE_OK) {
            cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
            return;
        }

        string sql = "DELETE FROM REGISTRATION_user WHERE EMAIL = ?;";

        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);

            rc = sqlite3_step(stmt);

            if (rc != SQLITE_DONE) {
                cerr << "Deletion failed: " << sqlite3_errmsg(db) << endl;
            }

            sqlite3_finalize(stmt);
        } else {
            cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
        }

        sqlite3_close(db);
    }
}

void User::checkIn() {
    string inputEmail;
    cout << "Enter your email address: ";
    cin >> inputEmail;
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;
    bool found = false;
    rc = sqlite3_open("user.db", &db);
    if (rc) {
        cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }
    string sql = "SELECT EMAIL FROM REGISTRATION_user WHERE EMAIL = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(stmt, 1, inputEmail.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        found = true;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    if (found) {
        cout << "You are already registered in the Airplane Management System" << endl;
    } else {
        cout << "You are not registered in the Airplane Management System." << endl;
        cout << "Please register in the Airplane Management System" << endl;
    }
}
void User::bookTicket() {
    string inputEmail;
    cout << "Please enter your email address: ";
    cin >> inputEmail;
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;
    bool found = false;
    rc = sqlite3_open("user.db", &db);
    if (rc) {
        cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }
    string sql = "SELECT USERID, NAME, PASSPORTID FROM REGISTRATION_user WHERE EMAIL = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(stmt, 1, inputEmail.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        found = true;
        userId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        passportId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    if (found) {
        int choice;
        cout << "If you want to use local transportation press 1" << endl;
        cout << "If you want to use global transportation press 2" << endl;
        cin >> choice;
        if (choice == 1) {
            cout << "Enter arrival place: ";
            cin >> arrival_place;
            cout << "Enter destination place: ";
            cin>>destination_place;
            cout << "Submit payment for local transportation service: ";
            cin >> payment;
        } else if (choice == 2) {
            string inputPassportId;
            cout << "Enter arrival place: ";
            cin >> arrival_place;
            cout << "Enter destination place: ";
            cin>>destination_place;
            cout << "Enter passport number or id: ";
            cin >> inputPassportId;
            if (inputPassportId == passportId) {
                cout << "Submit payment for global transportation service: ";
                cin >> payment;
            } else {
                cout << "Invalid passport ID." << endl;
                return;
            }
        } else {
            cout << "Invalid choice" << endl;
            return;
        }
        database_inserter2();
    } else {
        cout << "You are not registered in the Airplane Management System." << endl;
        cout << "Please register in the Airplane Management System" << endl;
    }
}
void User::database_inserter2() {
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* zErrMsg = nullptr;
    int rc;
    string sql;
    rc = sqlite3_open("user.db", &db);
    if (rc) {
        cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }
    // Enable foreign key support
    rc = sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "Can't enable foreign key support: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    // Create the BOOKING table if it doesn't exist
    sql = "CREATE TABLE IF NOT EXISTS BOOKING("
          "USERID INTEGER NOT NULL,"
          "NAME TEXT NOT NULL,"
          "PAYMENT INT NOT NULL,"
          "PRIMARY KEY (USERID, NAME),"
          "FOREIGN KEY(USERID) REFERENCES REGISTRATION_user(USERID));";
    rc = sqlite3_exec(db, sql.c_str(), NULL, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    // Check if the booking already exists
    sql = "SELECT COUNT(*) FROM BOOKING WHERE USERID = ? AND NAME = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
        cout << "A booking with this USERID and NAME already exists." << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }
    sqlite3_finalize(stmt);
    // Insert the new booking
    sql = "INSERT INTO BOOKING (USERID, NAME, PAYMENT) "
          "VALUES (?, ?, ?)";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, payment.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cout << "SQL error: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Booking successfully created!" << endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void User::flight_Maintenanc(){
    char choose ;
    char cont;
    do {
        char choose1;
        cout << "\nAdmin menu:\n";
        cout << "1. Add flight" << endl;
        cout << "2. Delete flight" << endl;
        cin >> choose1;
        cin.ignore();
        if (choose1 == '1') {
            cout << "Enter flight id: ";
            getline(cin, flight_id);
            cout << "Arrival time: ";
            getline(cin, arrival_time);
            cout << "Arrival place: ";
            getline(cin, arrival_place);
            cout << "Destination time: ";
            getline(cin, destination_time);
            cout << "Destination place: ";
            getline(cin, destination_place);
            cout << "Enter plan id: ";
            cin >> plan_id;
            cin.ignore();

            sqlite3 *db;
            char *ZErrmsg = nullptr;
            int rc = sqlite3_open("user.db", &db);

            if (rc != SQLITE_OK) {
                cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
                sqlite3_close(db);
                return;
            }

            string sql = "SELECT * FROM MaintenanceTracking WHERE PLAN_ID = ?;";
            sqlite3_stmt *stmt;
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

            if (rc == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, to_string(plan_id).c_str(), -1, SQLITE_STATIC);
                rc = sqlite3_step(stmt);

                if (rc != SQLITE_ROW) {
                    cerr << "Invalid plan id. Please enter a correct plan id." << endl;
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    return;
                }

                sqlite3_finalize(stmt);
            } else {
                cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
                sqlite3_close(db);
                return;
            }


            sql = "CREATE TABLE IF NOT EXISTS FLIGHT_TABLE("
                  "FLIGHT_ID TEXT PRIMARY KEY NOT NULL, "
                  "PLAN_ID TEXT NOT NULL, "
                  "ARRIVAL_TIME TEXT NOT NULL, "
                  "ARRIVAL_PLACE TEXT NOT NULL, "
                  "DESTINATION_TIME TEXT NOT NULL, "
                  "DESTINATION_PLACE TEXT NOT NULL, "
                  "FOREIGN KEY(PLAN_ID) REFERENCES MaintenanceTracking (PLAN_ID)"
                  ");";

            rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &ZErrmsg);

            if (rc != SQLITE_OK) {
                cerr << "SQL error: " << ZErrmsg << endl;
                sqlite3_free(ZErrmsg);
                sqlite3_close(db);
                return;
            }
            string insertSql = "INSERT INTO FLIGHT_TABLE (PLAN_ID, ...) VALUES (?, ...);";
            sql = "INSERT OR REPLACE INTO FLIGHT_TABLE(FLIGHT_ID, PLAN_ID, ARRIVAL_TIME, ARRIVAL_PLACE, DESTINATION_TIME, DESTINATION_PLACE) "
                  "VALUES (?, ?, ?, ?, ?, ?);";

            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

            if (rc == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, flight_id.c_str(), -1, SQLITE_STATIC);
                // Convert plan_id to string and then to C-style string
                sqlite3_bind_text(stmt, 2, std::to_string(plan_id).c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 3, arrival_time.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 4, arrival_place.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 5, destination_time.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 6, destination_place.c_str(), -1, SQLITE_STATIC);

                rc = sqlite3_step(stmt);

                if (rc != SQLITE_DONE) {
                    cerr << "Insertion failed: " << sqlite3_errmsg(db) << endl;
                }

                sqlite3_finalize(stmt);
            } else {
                cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
            }

            sqlite3_close(db);
        } else if (choose1 == '2') {

            cout << "Enter flight id: ";
            getline(cin, flight_id);

            sqlite3 *db;
            char *ZErrmsg = nullptr;
            int rc = sqlite3_open("user.db", &db);

            if (rc != SQLITE_OK) {
                cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
                sqlite3_close(db);
                return;
            }

            string sql = "DELETE FROM FLIGHT_TABLE WHERE FLIGHT_ID = ?;";

            sqlite3_stmt *stmt;
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

            if (rc == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, flight_id.c_str(), -1, SQLITE_STATIC);

                rc = sqlite3_step(stmt);

                if (rc != SQLITE_DONE) {
                    cerr << "Deletion failed: " << sqlite3_errmsg(db) << endl;
                }

                sqlite3_finalize(stmt);
            } else {
                cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
            }

            sqlite3_close(db);
        }
    }while(choose=='y'||choose=='Y');

}
void User::crew_management(){
    char choose;
    char cont;
    do {
        char choose;
        cout << "\nAdmin menu:\n";
        cout << "1. Add employ" << endl;
        cout << "2. Delete employ" << endl;
        cin >> choose;
        cin.ignore();

        if (choose == '1') {
            cout << "Enter employID: ";
            cin >> employId;
            cin.ignore();
            cout << "Enter employ name: ";
            getline(cin, employName);
            cout << "Enter employ work space: ";
            cin >> workspace;
            crew[employId] = {employName, workspace};


            sqlite3 *db;
            int rc;
            char *zErrmsg = nullptr;
            string sql;
            rc = sqlite3_open("user.db", &db);
            if (rc != SQLITE_OK) {
                cout << "Database connection is failed." << endl;
                cout << "Error: " << sqlite3_errmsg(db) << endl;
                sqlite3_free(zErrmsg);
                return;
            }
            rc = sqlite3_exec(db, "PRAGMA foreign_keys=ON;", nullptr, nullptr, &zErrmsg);
            if (rc != SQLITE_OK) {
                cout << "Can't enable foreign key support in crew table: " << zErrmsg << endl;
                sqlite3_free(zErrmsg);
            }
            sql = "CREATE TABLE IF NOT EXISTS CREW_TABLE("
                  "EMPLOYEE_ID TEXT PRIMARY KEY NOT NULL,"
                  "EMPLOY_NAME TEXT NOT NULL,"
                  "WORK_SPACE TEXT NOT NULL);";
            rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &zErrmsg);
            if (rc != SQLITE_OK) {
                cout << "SQL error: " << zErrmsg << endl;
                sqlite3_free(zErrmsg);
            }
            for (const auto &user: crew) {
                string employId = user.first;
                auto it = user.second.begin();
                string employName = user.second.size() > 0 ? *it++ : "";
                string workspace = user.second.size() > 1 ? *it++ : "";
                sql = "INSERT INTO CREW_TABLE (EMPLOYEE_ID, EMPLOY_NAME, WORK_SPACE) "
                      "VALUES('" + employId + "', '" + employName + "', '" + workspace + "');";
                rc = sqlite3_exec(db, sql.c_str(), NULL, nullptr, &zErrmsg);
                if (rc != SQLITE_OK) {
                    cout << "SQL error: " << zErrmsg << endl;
                    sqlite3_free(zErrmsg);
                }
            }
            sqlite3_close(db);
        } else if (choose == '2') {
            cout <<"Enter employment ID";
            cin >> employId;
            sqlite3 *db;
            int rc;
            char *zErrmsg= nullptr;
            string sql;
            rc = sqlite3_open("user.db",&db);
            if(rc!=SQLITE_OK) {
                cout << "Database connection is failed: "<<endl;
                cout << "Error: " << sqlite3_errmsg(db) << endl;
                sqlite3_free(zErrmsg);
                return;
            }
            // delete employee from database from database
            sql = "DELETE FROM CREW_TABLE WHERE EMPLOYEE_ID ='" + employId + "'; ";
            rc = sqlite3_exec(db,sql.c_str(),nullptr,nullptr,&zErrmsg);
            if(rc!=SQLITE_OK){
                cout << "sql error: "<<zErrmsg << endl;
                sqlite3_free(zErrmsg);
            }
            cout << "the employee was deleted"<<endl;
            sqlite3_close(db);


        } else if (choose == '3') {
            cout << "Exit to crew management" << endl;
            exit(0);
        } else {
            cout << "Invalid choice please try again" << endl;
        }
        cout << "If you go to admin menu enter Y/n" << endl;
        cin >> cont;
    } while(cont == 'y' || cont == 'Y');

}
void User::MaintenanceTracking() {
    char choose;
    do{
        char ch1;
        cout << "\nAdmin menu:\n";
        cout << "1. Add Maintenance plan" << endl;
        cout << "2. Delete Maintenance plan" << endl;
        cout << "Enter your choice: ";
        cin >> ch1;
        cin.ignore();

        if (ch1 == '1'){


            cout << "Enter plan id: ";
            if (!(cin >> plan_id)) {
                cerr << "Invalid plan ID input. Please enter an integer." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(); // Clear input stream
            cout << "Enter plan name: ";
            getline(cin, planName);
            tracks[to_string(plan_id)] = {planName};
            sqlite3* db;
            int rc;
            char* ZErrmsg = nullptr;
            string sql;
            rc = sqlite3_open("user.db",&db);
            if(rc !=SQLITE_OK){
                cout << "Database connection is not established"<< endl;
                cout << "Error: " << sqlite3_errmsg(db) << endl;
                sqlite3_free(ZErrmsg);
                return;
            }
            rc = sqlite3_exec(db, "PRAGMA foreign_keys=ON;", nullptr, nullptr, &ZErrmsg);
            if (rc != SQLITE_OK) {
                cerr << "Can't enable foreign key support: " << ZErrmsg << endl;
                sqlite3_free(ZErrmsg);
                sqlite3_close(db);
                continue;
            }
            sql = "CREATE TABLE IF NOT EXISTS MaintenanceTracking ("
                  "PLAN_ID TEXT PRIMARY KEY NOT NULL,"
                  "PLAN_NAME TEXT NOT NULL);";
            rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &ZErrmsg);
            if (rc != SQLITE_OK) {
                cerr << "SQL error: " << ZErrmsg << endl;
                sqlite3_free(ZErrmsg);
            }
            for (const auto &track: tracks) {
                string planId = track.first;
                string planName = *track.second.begin();

                sqlite3_stmt *stmt;
                string checkSql = "SELECT COUNT(*) FROM MaintenanceTracking WHERE PLAN_ID = ?;";
                rc = sqlite3_prepare_v2(db, checkSql.c_str(), -1, &stmt, nullptr);
                if (rc != SQLITE_OK) {
                    cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    continue;
                }
                sqlite3_bind_text(stmt, 1, planId.c_str(), -1, SQLITE_STATIC);
                rc = sqlite3_step(stmt);
                int count = (rc == SQLITE_ROW) ? sqlite3_column_int(stmt, 0) : 0;
                sqlite3_finalize(stmt);
                if (count == 0) {
                    sql = "INSERT INTO MaintenanceTracking (PLAN_ID, PLAN_NAME) VALUES (?, ?);";
                } else {
                    sql = "UPDATE MaintenanceTracking SET PLAN_NAME = ? WHERE PLAN_ID = ?;";
                }
                rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                if (rc == SQLITE_OK) {
                    sqlite3_bind_text(stmt, 1, planId.c_str(), -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 2, planName.c_str(), -1, SQLITE_STATIC);
                    rc = sqlite3_step(stmt);
                }
                if (rc != SQLITE_DONE) {
                    cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
                }
                sqlite3_finalize(stmt);
            }
            sqlite3_close(db);

        }

        else if (ch1 == '2') {


            cout << "Enter plan id: ";
            if (!(cin >> plan_id)) {
                cerr << "Invalid plan ID input. Please enter an integer." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            sqlite3 *db;
            char *zErrMsg = nullptr;
            int rc = sqlite3_open("user.db", &db);
            if (rc != SQLITE_OK) {
                cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
                sqlite3_close(db);
                continue;
            }


            string sql = "DELETE FROM MaintenanceTracking WHERE PLAN_ID = ?;";
            sqlite3_stmt *stmt;

            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << endl;
                sqlite3_close(db);
                continue;
            }

            sqlite3_bind_text(stmt, 1, to_string(plan_id).c_str(), -1, SQLITE_STATIC);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                cerr << "Deletion failed: " << sqlite3_errmsg(db) << endl;
            }

            sqlite3_finalize(stmt);
            sqlite3_close(db);

        }


        else {
            cerr << "your input is invalid" << endl;
        }
        cout << "Do you want to go to the admin menu? Enter Y/n: ";
        cin >> choose;
    }while(choose =='y'||choose == 'Y');
}


int main() {

    User user;
    char choice = 'y';
    while(choice == 'y' || choice == 'Y') {
        user.displayOptions();
        cout << "Do you want to continue? y/n: ";
        cin >> choice;
        cin.ignore();
    }


    return 0;
}
