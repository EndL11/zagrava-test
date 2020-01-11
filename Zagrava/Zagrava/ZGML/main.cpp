#include <vector>
#include <iostream>

//����� ��������
#include <fstream>
#include <string>
/*
	� ��� � ������������ ������, ���� ����������������� ��� ��������� ���� ����������, 
	��� �� �� �������� ��������� �������� ���� ������ �� ������, � ������������ ��, ���
	������� � ����������� �� ����������� ��� � ������ ��� ��������� ���� ����������:
	
		- ��������� ����
		- ��������� ��������
		- ��������� �������� �����
		- ��������� ������

	��� ����� ���������� �������������� ��������� ���������. 

	���� ���� ����, �� ������ ����� ��������

	��������� ��� ��������� :)
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
	Tag& operator=(const Tag& ref) {
		if (this == &ref) {
			return *this;
		}
		name = ref.name;
		for (int i = ref.attrs.size() - 1; i >= 0 ; i--) {
			attrs.push_back(ref.attrs[i]);
		}
		is_closed = ref.is_closed;
		parent = ref.parent;
		return *this;
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

		if (tags.size() == 1) {
			for (int i = 0; i < tag.size(); i++) {
				for (int j = 0; j < tag[i].attrs.size(); j++) {
					if (tags[tags.size() - 1] == tag[i].name && tag[i].attrs[j].name == attribute) {
						response = tag[i].attrs[j].value;
						PrintResponse();
						return;
					}
				}
			}
			PrintResponse();
			return;
		}
		vector<bool> coincidence(tags.size(), false);
		Tag temp = tag[tag.size()-1];

		for (int i = tags.size() - 1; i >= 0; i--) {
			for (int j = 0; j < tag.size(); j++) {
				if (i != 0) {
					if (tag[j].name == tags[i] && tag[j].parent == tags[i - 1]) {
						for (int w = 0; w < tag.size(); w++) {
							if (tag[w].name == tags[i - 1]) {
								temp = tag[w];
								break;
							}
						}
						coincidence[i] = true;
						break;
					}
				}
				else {
					if (tag[j].name == tags[i] && temp.parent == tags[i]) {
						for (int w = 0; w < tag.size(); w++) {
							if (tag[w].name == tags[i]) {
								temp = tag[w];
								break;
							}
						}
						coincidence[i] = true;
						break;
					}
				}
				
			}
		}
		for (int i = 0; i < coincidence.size(); i++) {
			if (coincidence[i] == false) {
				PrintResponse();
				return;
			}
		}
		for (int i = 0; i < tag.size(); i++) {
			for (int j = 0; j < tag[i].attrs.size(); j++) {
				if (tags[tags.size() - 1] == tag[i].name && tag[i].attrs[j].name == attribute) {
					response = tag[i].attrs[j].value;
					PrintResponse();
					return;
				}
			}
		}
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

//	��������� ���������� ��������� �������
void PrintVector(vector<string> &v) {
	for (auto s : v) {
		cout << s << endl;
	}
}

int main() {
	/* Enter your code here. Read input from file input.txt. Print output to STDOUT */
	setlocale(LC_ALL, "ru");

	//	�������� �����
	ifstream input_file("input.txt");
	
	//	���������� ������� ������� ���� � ������� ������
	//	��� � ������� �� �� ����� �� ������� ���������� ����� �� ��������, ���� ��� ������ ��� ��� ��������
	//	����� ������� �� ��� �������, ��� �� �������� ���� �, ���� � ������ �� ���...
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
		cout << "���� �� ������\n\n";
	}
	else
	{
		for (int i = 0; !input_file.eof(); i++) {
			input_file >> s;

			if (i == 0) {
				amount_of_tags = stoi(s);
			}
			else if (i == 1) {
				amount_of_requests = stoi(s);
			}

			//	��������� ��������
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

			//	��������� �������� ��������
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

			//	��������� ������
			if (s.find("~") != std::string::npos) {
				Request req(s);
				reqs.push_back(req);
				created_requests++;
			}

			//	��������� ����
			if (s.find_first_not_of('<', 0)) {
				string attr = "";
				for (int j = 0; j < s.size(); j++) {
					if (s[j] != '<' && s[j] != '>' && s[j] != '/' && s[j] != ' ') {
						attr += s[j];
					}
				}
				created_tags++;

				if (tag_vec.size() < 1) {
					Tag tag(attr);
					tag_vec.push_back(tag);
				}
				else {

					//	���� ��� ������ ��������
					if (s.find("/>") != std::string::npos && s.find("<") != std::string::npos) {
						Tag tag(attr);
						tag.Closed();
						for (int i = tag_vec.size() - 1; i >= 0; i--) {
							if (tag_vec[i].is_closed == false) {
								tag.SetParent(tag_vec[i].name);
								break;
							}
						}
						tag_vec.push_back(tag);
						continue;
					}

					//	��������� ����� �� �� ���������(�����������) ���
					bool indic = false;
					for (int t = tag_vec.size() - 1; t >= 0; t--) {
						if (tag_vec[t].name == attr) {
							tag_vec[t].Closed();
							indic = true;
							break;
						}
					}
					if (indic == true) {
						continue;
					}
					else {
						for (int i = tag_vec.size() - 1; i >= 0; i--) {
							if (tag_vec[i].is_closed == false) {
								Tag tag(attr);
								tag.SetParent(tag_vec[i].name);
								tag_vec.push_back(tag);
								break;
							}
						}
						
					}

				}
			}
		}

		//	�������� �����
		input_file.close();

		/*cout << endl;
		cout << "���������� �����: " << amount_of_tags << endl;
		cout << "���������� ��������: " << amount_of_requests << endl;

		cout << "\nAttributes:" << endl;
		PrintVector(attributes);

		cout << "\nAttributes values:" << endl;
		PrintVector(attributes_value);

		cout << endl << endl;

		for (auto t : tag_vec) {
			t.Show();
		}

		for (auto a : attrs) {
			cout << a << endl;
		}*/

		//	��������� �������� �� ������
		for (auto r : reqs) {
			//cout << r;
			r.GetResponse(tag_vec);
		}
	}	
	system("pause");
	return 0;
}