#include <iostream>
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

class Date {
public:
	Date(const string &date) {
		stringstream ss_date(date);
		int year, month, day;
		char separator1, separator2;

		ss_date >> year >> separator1 >> month >> separator2 >> day;

		if (ss_date.fail() || separator1 != '-' || separator2 != '-'
				|| ss_date.rdbuf()->in_avail() > 0) {
			throw invalid_argument("Wrong date format: " + date);
		} else {
			if (month < 1 || month > 12) {
				throw invalid_argument(
						"Month value is invalid: " + to_string(month));
			}

			if (day < 1 || day > 31) {
				throw invalid_argument(
						"Day value is invalid: " + to_string(day));
			}
			m_year = year;
			m_month = month;
			m_day = day;
		}
	}

	int GetYear() const {
		return m_year;
	}

	int GetMonth() const {
		return m_month;
	}

	int GetDay() const {
		return m_day;
	}

private:
	int m_year;
	int m_month;
	int m_day;
};

ostream& operator<<(ostream &stream, const Date &date) {
	stream << setfill('0') << setw(4) << date.GetYear() << '-' << setw(2)
			<< date.GetMonth() << '-' << setw(2) << date.GetDay();
	return stream;
}

bool operator<(const Date &lhs, const Date &rhs) {
	vector<int> lhs_date { lhs.GetYear(), lhs.GetMonth(), lhs.GetDay() };
	vector<int> rhs_date { rhs.GetYear(), rhs.GetMonth(), rhs.GetDay() };
	return lhs_date < rhs_date;
}

class Database {
public:
	void AddEvent(const Date &date, const string &event) {
		m_db[date].insert(event);
	}
	bool DeleteEvent(const Date &date, const string &event) {
		if (m_db.count(date) == 0) {
			return false;
		} else if (m_db[date].count(event) == 0) {
			return false;
		} else {
			m_db[date].erase(event);
			return true;
		}
	}
	int DeleteDate(const Date &date) {
		if (m_db.count(date) == 0) {
			return 0;
		} else {
			int deleted_events = m_db[date].size();
			m_db.erase(date);
			return deleted_events;
		}
	}
	set<string> Find(const Date &date) const {
		if (m_db.count(date) == 0) {
			return set<string> { };
		} else {
			return m_db.at(date);
		}
	}
	void Print() const {
		for (const auto &item : m_db) {
			for (const auto &event : item.second) {
				cout << item.first << ' ' << event << endl;
			}
		}
	}
private:
	map<Date, set<string>> m_db;
};

int main() {
	Database db;

	string command;
	while (getline(cin, command)) {
		if (!command.empty()) {
			stringstream ss_command(command);
			string operation;
			ss_command >> operation;

			if (operation == "Add") {

				string date, event;
				ss_command >> date >> event;
				try {
					Date parsed_date = Date(date);
					db.AddEvent(parsed_date, event);
				} catch (invalid_argument &ex) {
					cout << ex.what() << endl;
				}

			} else if (operation == "Del") {

				string date, event;
				ss_command >> date;
				try {
					Date parsed_date = Date(date);
					if (ss_command.rdbuf()->in_avail() > 0) {
						ss_command >> event;
						if (db.DeleteEvent(parsed_date, event)) {
							cout << "Deleted successfully" << endl;
						} else {
							cout << "Event not found" << endl;
						}
					} else {
						cout << "Deleted " << db.DeleteDate(parsed_date)
								<< " events" << endl;
					}
				} catch (invalid_argument &ex) {
					cout << ex.what() << endl;
				}

			} else if (operation == "Find") {

				string date;
				ss_command >> date;
				try {
					Date parsed_date = Date(date);
					for (const auto &event : db.Find(parsed_date)) {
						cout << event << endl;
					}
				} catch (invalid_argument &ex) {
					cout << ex.what() << endl;
				}

			} else if (operation == "Print") {
				db.Print();
			} else {
				cout << "Unknown command: " + operation << endl;
			}
		}
	}

	return 0;
}
