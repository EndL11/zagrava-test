#include <vector>
#include <iostream>

//додані бібліотеки
#include <fstream>
#include <string>
/*
	В коді є закоментовані ділянки, вони використовувались для виведення різної інформації, 
	так як по завданню необхідно виводити лише відповіді на запити, я закоментував їх, але
	можливо в майбутньому це знадобиться або ж просто для виведення різної інформації:
	
		- виведення тегів
		- виведення атрибутів
		- виведення зчитаних даних
		- виведення запитів

	Для цього реалізовано перевантаження операторів виведення. 

	Якщо вони зайві, їх завжди можна видалити

	Сподіваюсь все правильно :)
*/
using namespace std;

class Tag;

class Attribute {
public:
	string name;
	string value;
	Tag &tag = *(Tag*)0;

	Attribute() {
		name = "name...";
		value = "value...";
	}
	Attribute(string n) {
		name = n;
		value = "value...";
	}
	friend ostream& operator<<(ostream &os, const Attribute &attr) {
		os << "\tName: " << attr.name << " = " << attr.value << endl;
		return os;
	}
};

class Tag {	
public:
	string name;
	vector<Attribute> attrs;
	string parent;
	bool is_closed;

	Tag() {
		name = "tag";
		parent = name;
		is_closed = false;
	}
	Tag(string n) {
		name = n;
		parent = name;
		is_closed = false;
	}
	void Closed() {
		this->is_closed = !this->is_closed;
	}
	void SetParent(string str) {
		parent = str;
	}
	~Tag() {};
	void Show() {
		cout << "******Tag******" << endl;
		cout << "Name: " << name << endl;
		cout << "Parent tag: " << parent << endl;
		cout << "Attributes:\n";
		if (attrs.size() > 0)
		{
			for (auto k : attrs) {
				cout << k << endl;
			}
		}
		cout << "******Tag******" << endl << endl;
	}
};

class Request {
public:
	string attribute;
	string response = "";
	vector<string> tags;
	Request(string req) {
		string at = "";
		string t = "";
		bool recording = false;
		for (int i = 0; i < req.size(); i++) {
			if (req[i] != '.' && req[i] != '~') {
				t += req[i];
			}
			else {
				tags.push_back(t);
				if (req[i] == '.') {
					t = "";
				}
			}
			if (recording) {
				if (req[i] != ' ')
					at += req[i];
			}
			if (req[i] == '~') {
				recording = true;
			}

		}
		//cout << "Attribute is " << at << endl;
		attribute = at;
	}
	   
	void GetResponse(vector<Tag> &tag) {
		for (int i = 0; i < tag.size(); i++) {
			for (int j = 0; j < tag[i].attrs.size(); j++) {
				if (tags[tags.size() - 1] == tag[i].name && tag[i].attrs[j].name == attribute) {
					response = tag[i].attrs[j].value;
				}
			}
		}
		PrintResponse();
	}

	void PrintResponse() {
		if (response != "")
			cout << response << endl;
		else
			cout << "Not found!" << endl;
	}

	friend ostream& operator <<(ostream &os, const Request &req) {
		os << "Attribute: " << req.attribute << endl;
		os << "Tags: ";
		for (auto v : req.tags) {
			os << "\t" << v << endl;
		}
		return os;
	}
};

//	виведення переданого рядкового вектору
void PrintVector(vector<string> &v) {
	for (auto s : v) {
		cout << s << endl;
	}
}

int main() {
	/* Enter your code here. Read input from file input.txt. Print output to STDOUT */
	setlocale(LC_ALL, "ru");

	//	відкриття файлу
	ifstream input_file("input.txt");
	
	//	збереження значень кількості тегів і кількості запитів
	//	але я зрозумів що це вказує на кількість зчитування рядків до переносу, коли вже зробив ось цим способом
	//	можна вважати ці дані зайвими, але по завданню вони є, тому я залишу це тут...
	int amount_of_tags, amount_of_requests;
	int created_tags = 0, created_requests = 0;

	vector<Tag> tag_vec;
	vector<Attribute> attrs;
	vector<Request> reqs;

	string s;
	vector<string> attributes;
	vector<string> attributes_value;

	if (!input_file.is_open())
	{
		cout << "Файл не открыт\n\n";
	}
	else
	{
		int i = 0;
		for (input_file >> s; !input_file.eof(); input_file >> s, i++) {
			if (i == 0) {
				amount_of_tags = stoi(s);
			}
			else if (i == 1) {
				amount_of_requests = stoi(s);
			}

			//	додавання атрибуту
			if (
				s.find("=") == std::string::npos
				&& !s.find_first_not_of("\"", 0)
				&& !s.find_first_not_of('<', 0)
				&& s.find("~") == std::string::npos
				&& i != 0
				&& i != 1
				) {
				Attribute attr(s);
				attrs.push_back(attr);
				attributes.push_back(s);
			}

			//	додавання значення атрибуту
			if (s.find("\"") != std::string::npos) {
				string attr = "";
				for (int j = 0; j < s.size(); j++) {
					if (s[j] != '>' && s[j] != '\"' && s[j] != ' ' && s[j] != '/') {
						attr += s[j];
					}
				}
				attributes_value.push_back(attr);
				attrs[attributes_value.size()-1].value = attr;
				if (tag_vec.size() > 0) {
					for (int l = tag_vec.size() - 1; l >= 0; l--) {
						if (tag_vec[l].is_closed == false) {
							tag_vec[l].attrs.push_back(attrs[attributes_value.size() - 1]);
							break;
						}
					}
				}
			}

			//	додавання запиту
			if (s.find("~") != std::string::npos) {
				Request req(s);
				reqs.push_back(req);
				created_requests++;
			}

			//	додавання тега
			if (s.find_first_not_of('<', 0)) {
				string attr = "";
				for (int j = 0; j < s.size(); j++) {
					if (s[j] != '<' && s[j] != '>' && s[j] != '/' && s[j] != ' ') {
						attr += s[j];
					}
				}
				created_tags++;

				//	якщо тег одразу закритий
				if (s.find("/>") != std::string::npos && s.find("<") != std::string::npos) {
					Tag tag(attr);
					tag.Closed();
					tag_vec.push_back(tag);
					continue;
				}

				if (tag_vec.size() < 1) {
					Tag tag(attr);
					tag_vec.push_back(tag);
				}
				else {

					//	індикатор вказує що це повторний(закриваючий) тег
					bool indic = false;
					for (int t = 0; t < tag_vec.size(); t++) {
						if (tag_vec[t].name == attr) {
							tag_vec[t].Closed();
							indic = true;
							break;
						}
					}
					if (indic) {
						continue;
					}
					else {
						for (int i = tag_vec.size() - 1; i >= 0; i--) {
							if (tag_vec[i].is_closed == false) {
								Tag tag(attr);
								tag.SetParent(tag_vec[i].name);
								tag_vec.push_back(tag);
							}
						}
						
					}

				}
			}
		}

		if (input_file.is_open()) {
			// додавання останнього зчитаного запиту
			if (s.find("~") != std::string::npos) {
				Request req(s);
				reqs.push_back(req);
				created_requests++;
			}
		}

		//	закриття файлу
		input_file.close();

		//cout << endl;
		//cout << "Количество тегов: " << amount_of_tags << endl;
		//cout << "Количество запросов: " << amount_of_requests << endl;
		//cout << "\nTags:" << endl;
		//PrintVector(tags);

		//cout << "\nAttributes:" << endl;
		//PrintVector(attributes);

		//cout << "\nAttributes values:" << endl;
		//PrintVector(attributes_value);

		//cout << "\nRequests:" << endl;
		//PrintVector(requests);

		//cout << endl << endl;

		//for (auto t : tag_vec) {
		//	t.Show();
		//	cout << t.is_closed << endl;
		//}

		//for (auto a : attrs) {
		//	cout << a << endl;
		//}

		//	отримання відповідей на запити
		for (auto r : reqs) {
			r.GetResponse(tag_vec);
			//cout << r << endl;
		}
	}	
	system("pause");
	return 0;
}