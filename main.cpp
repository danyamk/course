#include <iostream>
#include "stdlib.h"
#include <map>
#include <set>
#include <vector>
#include <iomanip>
using namespace std;


class Date {
public:
    Date (int year, int month, int day){
        if (month > 12 || month <= 0){
            throw out_of_range("Month value is invalid: " + to_string(month));
        } else {
            if (day > 31 || day <= 0){
                throw out_of_range ("Day value is invalid: " + to_string(day));
            } else {
                year_now = year;
                month_now = month;
                day_now = day;
            }
        }

    }
    Date (){}
  int GetYear() const{
        return year_now;
  }
  int GetMonth() const{
        return month_now;
  }
  int GetDay() const{
        return day_now;
  }

private:
    int year_now;
    int month_now;
    int day_now;
};


bool operator< (const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() != rhs.GetYear()){
        return lhs.GetYear() < rhs.GetYear();
    } else if (lhs.GetMonth() != rhs.GetMonth()){
        return lhs.GetMonth() < rhs.GetMonth();
    } else {
        return lhs.GetDay() < rhs.GetDay();
    }
}


bool operator== (const Date& lhs, const Date& rhs){
    return (lhs.GetDay() == rhs.GetDay()) && (lhs.GetMonth() == rhs.GetMonth()) && (lhs.GetYear() == rhs.GetYear());
}


istream& operator>> (istream& stream, Date& date){
    int a,b,c;
    stream >> a;
    stream.ignore(1);
    stream >> b;
    stream.ignore(1);
    stream >> c;
    date = {a, b, c};
    return stream;
}


ostream& operator << (ostream& stream, const Date& date){
    stream << setfill('0')<< setw(4)<<date.GetYear();
    stream << '-';
    stream << setw(2) <<setfill('0') <<date.GetMonth();
    stream << '-';
    stream << setw(2) << setfill('0')<< date.GetDay();
    return stream;
}


Date ParseDate (const string& k) {
    stringstream stream(k);
    int a, b, c;
    string c_string;
    bool check_sym= true;
    Date date;
    stream >> a;
    if (stream.peek() != '-') {
        check_sym = false;
    }
    stream.ignore(1);

    stream >> b;
    if (stream.peek() != '-') {
        check_sym = false;
    }
    stream.ignore(1);

    stream >> c_string;
    string check;
    if (c_string.size() == 0){
        check_sym = false;
    } else {
        for (int i = 0; i < c_string.size(); i++) {
            if (isdigit(c_string[i]) || c_string[i] == '+' || c_string[i] == '-') {
                check += c_string[i];
            } else {
                check_sym = false;
            }
        }
    }
    c = atoi(check.c_str());
    if (!check_sym){
        throw invalid_argument("Wrong date format: " + k);
    } else {
        date = {a, b, c};
        return date;
    }
};


class Database {
public:
  void AddEvent(const Date& date, const string& event){
      data_base[date].insert(event);
  }

  bool DeleteEvent(const Date& date, const string& event){
      for (auto& s : data_base) {
          if (s.first == date) {
              for (auto& d : s.second) {
                  if (d == event){
                      s.second.erase(event);
                      return true;
                  }
              }
          }
      }
      return false;
  }


  int DeleteDate(const Date& date) {
      int value;
      for (auto& s : data_base){
          if (s.first == date){
              value = data_base[date].size();
              data_base.erase(date);
              return value;
          }
      }
      return 0;
  }


  void Find(const Date& date) const{
      for (const auto& s : data_base){
          if (s.first == date){
              for (auto& k : s.second){
                  cout << k << endl;
              }
          }
      }
  }


  void Print() const{
      for (const auto& s : data_base){
          for(const auto k : s.second){
              cout << s.first;
              cout << " " << k << endl;
          }
      }
  }

private:
  map<Date,set<string>> data_base;
};


int main() {
    Database db;
    Date date;
    string command,event, commands, date_string;
    while (getline(cin, command)) {
        try {
            stringstream input(command);
            commands.clear();
            input >> commands;
            if (commands == "Add") {
                try{
                    input >> date_string;
                    date = ParseDate(date_string);
                    input >> event;
                    db.AddEvent(date, event);
                } catch (exception &k){
                    cout << k.what() << endl;
                    }
            } else {
                if (commands == "Del") {
                    try {
                        input >> date_string;
                        date = ParseDate(date_string);
                        if (!input.eof()) {
                            input >> event;
                            if (db.DeleteEvent(date, event)) {
                                cout << "Deleted successfully" << endl;
                            } else {
                                cout << "Event not found" << endl;
                            }
                        } else {
                            int b = db.DeleteDate((date));
                            if (b >= 0) {
                                cout << "Deleted " << b << " events" << endl;
                            } else {
                                cout << "Event not found" << endl;
                            }
                        }
                    } catch (exception &k){
                        cout << k.what() << endl;
                    }
                } else {
                    if (commands == "Find") {
                        try {
                            input >> date_string;
                            date = ParseDate(date_string);
                            db.Find(date);
                        } catch (exception& k){
                            cout << k.what() << endl;
                        }
                    } else {
                        if (commands == "Print") {
                            db.Print();
                        } else {
                            if (commands != ""){
                                try{
                                    throw invalid_argument ("Unknown command: "+ commands);
                                } catch (exception &k){
                                    cout << k.what() << endl;
                                }
                            }
                        }
                    }
                }
            }
        } catch (exception& k){}
    }
}