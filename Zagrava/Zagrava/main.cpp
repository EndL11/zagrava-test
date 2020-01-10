#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include <fstream>
#include <string>

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
	void AddAttr(Attribute &attr) {
		this->attrs.push_back(attr);
	}
	void Show() {
		cout << "Name: " << name << endl;
		cout << "Parent tag: " << parent << endl;
		cout << "Attributes:\n";
		if (attrs.size() > 0)
		{
			for (auto k : attrs) {
				cout << k << endl;
			}
		}

	}

};



class Request {
	
};


void PrintVector(vector<string> &v) {
	for (auto s : v) {
		cout << s << endl;
	}
}

int main() {
	/* Enter your code here. Read input from file input.txt. Print output to STDOUT */
	setlocale(LC_ALL, "ru");
	ifstream input_file("input.txt");

	int amount_of_tags, amount_of_requests;
	int created_tags = 0, created_requests = 0;

	vector<Tag> tag_vec;
	vector<Attribute> attrs;
	vector<Request> reqs;

	string s;
	vector<string> tags;
	vector<string> attributes;
	vector<string> attributes_value;
	vector<string> requests;

	if (!input_file.is_open())
	{
		cout << "���� �� ������\n\n";
	}
	else
	{
		cout << "��� ��! ���� ������!\n\n";

		int i = 0;
		for (input_file >> s; !input_file.eof(); input_file >> s, i++) {
			if (i == 0) {
				amount_of_tags = stoi(s);
			}
			else if (i == 1) {
				amount_of_requests = stoi(s);
			}
			cout << s << endl;
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
			if (s.find("\"") != std::string::npos) {
				string attr = "";
				for (int j = 0; j < s.size(); j++) {
					if (s[j] != '>' && s[j] != '\"' && s[j] != ' ') {
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

			if (s.find("~") != std::string::npos) {
				requests.push_back(s);
				created_requests++;
			}

			if (s.find_first_not_of('<', 0)) {
				string attr = "";
				for (int j = 0; j <= s.size(); j++) {
					if (s[j] != '<' && s[j] != '>' && s[j] != '/' && s[j] != ' ') {
						attr += s[j];
					}
				}
				tags.push_back(attr);
				created_tags++;
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
					Tag tag(attr);
					tag.SetParent(tag_vec[tag_vec.size() - 1].name);
					tag_vec.push_back(tag);
				}
			}

		}

		if (s.find_first_not_of('<', 0)) {
			string attr = "";
			for (int j = 0; j <= s.size(); j++) {
				if (s[j] != '<' && s[j] != '>' && s[j] != '/' && s[j] != ' ') {
					attr += s[j];
				}
			}
			tags.push_back(attr);
			created_tags++;
		}

		if (
			s.find("=") == std::string::npos
			&& !s.find_first_not_of("\"", 0)
			&& !s.find_first_not_of('<', 0)
			&& s.find("~") == std::string::npos
			&& i != 0
			&& i != 1
			) {
			attributes.push_back(s);
		}
		if (s.find("\"") != std::string::npos) {
			string attr = "";
			for (int j = 0; j < s.size(); j++) {
				if (s[j] != '>' && s[j] != '\"' && s[j] != ' ') {
					attr += s[j];
				}
			}
			attributes_value.push_back(attr);
		}

		if (s.find("~") != std::string::npos) {
			requests.push_back(s);
			created_requests++;
		}

		input_file.clear();
		input_file.seekg(0, ios::beg);

		cout << endl;
		cout << amount_of_tags << endl;
		cout << amount_of_requests << endl;
		cout << "\nTags:" << endl;
		PrintVector(tags);

		cout << "\nAttributes:" << endl;
		PrintVector(attributes);

		cout << "\nAttributes values:" << endl;
		PrintVector(attributes_value);

		cout << "\nRequests:" << endl;
		PrintVector(requests);

		cout << endl << endl;

		for (auto t : tag_vec) {
			t.Show();
			cout << t.is_closed << endl;
		}

		for (auto a : attrs) {
			cout << a.name << " = " << a.value << endl;
		}

	}

	
	input_file.close();
	system("pause");
	return 0;
}