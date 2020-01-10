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
		//string nam = "";
		//for (int i = 0; i < n.size()-1; i++) {
		//	if (n[i] != ' ') {
		//		nam += n[i];
		//	}
		//}
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
		cout << "Attribute is " << at << endl;
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
			cout << "Response is " << response << endl;
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
		cout << "Файл не открыт\n\n";
	}
	else
	{
		cout << "Все ОК! Файл открыт!\n\n";

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
				Request req(s);
				reqs.push_back(req);
				requests.push_back(s);
				created_requests++;
			}

			if (s.find_first_not_of('<', 0)) {
				string attr = "";
				for (int j = 0; j < s.size(); j++) {
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

		if (s.find("~") != std::string::npos) {
			Request req(s);
			reqs.push_back(req);
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
			//cout << t.is_closed << endl;
		}

		//for (auto a : attrs) {
		//	cout << a << endl;
		//}

		for (auto r : reqs) {
			r.GetResponse(tag_vec);
			cout << r << endl;
		}

	}

	
	input_file.close();
	system("pause");
	return 0;
}