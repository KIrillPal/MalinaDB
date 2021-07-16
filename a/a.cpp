//#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

using namespace std;

const string
PROGRAM_NAME = "MalinaDB",
PROGRAM_VERSION = "10.0.24-MalinaDB-7 Brubuntu 16.04";
const int SHAPE = 1000;
bool islighted = false;

string command = "", mode = "";
set<string> tableset;

map<string, set<int> > tables;

struct pupil
{
	int age = 0;
	int grade = 0;
	char letter = 'A';
	string firstname = "B";
	string lastname = "b";
	double middle = 0;
	vector<int> marks;
};

int to(double a, double from, int into = SHAPE)
{
	return into * a / from;
}

bool isgirl(string s)
{
	char c = s[s.size() - 1];
	return (c == 'a' || c == 'e' || c == 'o' || c == 'u' || c == 'i');
}

vector<pupil> school;
vector<bool> isexist;

double getmid(int i)
{
	double d = 0, m = 0;
	for (int j = 0; j < school[i].marks.size(); ++j)
	{
		d += school[i].marks[j];
		//cout << "a " << school[i].marks[j] << endl;
		m++;
	}
	//cout << "res " << d / m << endl;
	return (m != 0) ? d / m : 0;
}

void sys_print(const string s)
{
	cout << s;
}

void init(int id)
{
	//
	//Ã‘Ã¨Ã±Ã²Ã¥Ã¬Ã­Ã Ã¿ Ã·Ã Ã±Ã²Ã¼
	//	
	//
	// ÃŠÃ®Ã­Ã±Ã®Ã«Ã¼Ã­Ã Ã¿ Ã·Ã Ã±Ã²Ã¼
	//
	system("color 0F");
	sys_print("KirillPal's homework loaded.\n");
	printf("\nWelcome to the %s monitor. Command end with ;\n", PROGRAM_NAME.c_str());
	printf("Your %s connection id is %d\n", PROGRAM_NAME.c_str(), id);
	printf("Server version: %s\n\n", PROGRAM_VERSION.c_str());
	printf("Copyright (c) No rights reserved, 2000, 2019, Orade, %s Corporation Ab and others.\n\n", PROGRAM_NAME.c_str());
	printf("Type 'help;' for help. Type '\\r' to clear mode. Type 'clear;' to clear the current input statement.\n\n");
}

struct attribute_socket
{
	string type;
	int type_id;
	string value_s;
	int value_i;
	int match_type = -1;
	int e = 0;
	bool active = false;
};

struct request_socket {
	string table = "all";
	string table_op1 = "empty";
	string table_op2 = "empty";
	string table_name = "table";
	string pers_name = "";
	string pers_sur = "";
	int pers_grade_min = 0;
	int pers_grade_max = 1000;
	string pers_letter = "";
	string pers_gender = "";
	int pers_mark_min = 0;
	int pers_mark_max = 1000;
	string value_s;
	int value_i;
};
struct answer_socket {
	string answer;
	string table;
	int e;

};

string simp(string s)
{
	for (int i = 0; i < s.size(); ++i)
		if (s[i] >= 65 && s[i] <= 90)
			s[i] += 32;
	return s;
}

string p_str(pupil p)
{
	string str = to_string(p.grade) + p.letter + " " + p.firstname + " " + p.lastname;
	for (int i = 0; i < p.marks.size(); ++i)
	{
		str += " " + to_string(p.marks[i]);
	}
	return str;
}

void prefix()
{
	if (mode == "")
		printf("%s[none] -> ", PROGRAM_NAME.c_str());
	else printf("%s[%s] -> ", PROGRAM_NAME.c_str(), mode.c_str());
}

void throw_exception(string res, string reason, int code)
{
	printf(" error:%d Command is denied. An Exception found in '%s'.\n%s.\n", code, res.c_str(), reason.c_str());
	mode = command = "";
	tables[""] = tables["all"];
	prefix();
	//printf("%s[%s] -> ", PROGRAM_NAME.c_str(), mode.c_str());
}

string specread()
{
	char c;
	string res;
	do
	{
		c = cin.get();
		if ((c == '\n' || c == ' ') && command == "" && res != "\\r")
			mode = command = res;
		if (c == '\n')
			prefix();
		else if (c != ' ')
			res += c;
		if (c == ';')
		{
			if (res == ";" && command == "")
			{
				throw_exception(";  ", "The syntax is broken. Empty command executing", 534);
				return "";
			}
			c = cin.get();
			if (c != ' ' && c != '\n')
			{
				while (c != ' ' && c != '\n')
					c = cin.get();
				prefix();
				throw_exception(res.substr(max((int)res.size() - 3, 0), min((int)res.size(), 3)), " The syntax is broken. Invalid end of command position", 532);
				if (c == '\n')
					prefix();
				return "";
			}
			else if (c == '\n' || c == ' ')
			{
				if (c == '\n')
					prefix();
				if (command == "")
					command = res;
			}
		}
	} while (c != ' ' && c != ';' && c != '\n');
	//cout << "specread" << command << "" << res << endl;
	if (res == "\\r")
	{
		mode = command = "";
		printf("Ok! Command cleared. Value is returned.\n");
		prefix();
		return "";
	}
	return simp(res);
}

attribute_socket get_attr()
{
	attribute_socket rs;
	string attr = specread();
	if (attr == "\n")
	{
		rs.e = 532;
		return rs;
	}
	if (attr[attr.size() - 1] == ';')
	{
		rs.active = true;
		attr = attr.substr(0, attr.size() - 1);
		if (attr == "")
			return rs;
	}
	int m = (attr + '=').find('=');
	if (m == attr.size())
		m = (attr + '>').find('>');
	else rs.match_type = 0;
	if (m == attr.size())
		m = (attr + '<').find('<');
	else if (rs.match_type == -1) rs.match_type = 1;
	if (m == attr.size())
	{
		throw_exception(attr.substr(0, 3), "The syntax is broken. Attribute must have a value", 453);
		rs.e = 453;
		return rs;
	}
	else if (rs.match_type == -1) rs.match_type = 2;
	if (attr.size() == m + 1 || attr.size() > m + 1 && attr[m + 1] != '"')
	{
		string res = attr.substr(m + 1, min(3, (int)attr.size() - m - 1)), res2;
		if (attr.size() > m + 1)
			res2 = attr.substr(m + 1, 1);
		else res2 = attr[m];
		throw_exception(res, "Symbol '\"' is excepted. Symbol '" + res2 + "' is found", 246);
		rs.e = 246;
		return rs;
	}
	if (attr.size() == m + 2 || attr.size() > m + 2 && attr[attr.size() - 1] != '"')
	{
		string res = attr.substr(max((int)attr.size() - 3, 0), min(3, (int)attr.size())), res2;
		if (attr.size() > m + 2)
			res2 = attr.substr(attr.size() - 1, 1);
		else res2 = attr[attr.size() - 1];
		throw_exception(res, "Symbol '\"' is excepted. Symbol '" + res2 + "' is found", 247);
		rs.e = 247;
		return rs;
	}
	rs.type = attr.substr(0, m);
	rs.value_s = simp(attr.substr(m + 2, attr.size() - m - 3));
	if (rs.match_type != 0 && rs.type != "grade" && rs.type != "middle")
	{
		throw_exception(attr.substr(m - 1, 3), "The operator does not match the attribute value", 520);
		rs.e = 520;
		return rs;
	}
	if (rs.type == "name")
		rs.type_id = 1;
	else if (rs.type == "surname")
		rs.type_id = 2;
	else if (rs.type == "grade")
	{
		rs.type_id = 3;
		rs.value_i = atoi(rs.value_s.c_str());
	}
	else if (rs.type == "letter")
		rs.type_id = 4;
	else if (rs.type == "in")
		rs.type_id = 5;
	else if (rs.type == "middle")
	{
		rs.type_id = 6;
		rs.value_i = atoi(rs.value_s.c_str());
	}
	else if (rs.type == "at")
		rs.type_id = 7;
	else if (rs.type == "gender")
		rs.type_id = 8;
	else if (rs.type == "first")
		rs.type_id = 9;
	else if (rs.type == "second")
		rs.type_id = 10;
	else
	{
		throw_exception(attr.substr(0, 3), "Attribute is invalid or unknown", 404);
		rs.e = 404;
		return rs;
	}
	return rs;
}

answer_socket help()
{
	answer_socket as;
	printf("help showed.\n\n");
	printf("     List of all commands:\n     Note that all text command must be first after symbol ';' and space. Command contains attributes and ends with ';'\n");
	printf("     +----------------------------------------------------------------------------------------------+\n");
	printf("     |                                         Main commands                                        |\n");
	printf("     +----------------------------------------------------------------------------------------------+\n");
	printf("     | help   | Display this help                       | help;                                     |\n");
	printf("     +----------------------------------------------------------------------------------------------+\n");
	printf("     | clear  | Clear all input                         | clear;                                    |\n");
	printf("     +--------+-----------------------------------------+-------------------------------------------+\n");
	printf("     | light  | Switch background color                 | light;                                    |\n");
	printf("     +--------+-----------------------------------------+-------------------------------------------+\n");
	printf("     |                                            Tables                                            |\n");
	printf("     +----------------------------------------------------------------------------------------------+\n");
	printf("     | table  | Create new temporal table and fill it   | table <attributes for pupil F>;           |\n");
	printf("     |        | by filter                               |                                           |\n");
	printf("     +--------+-----------------------------------------+-------------------------------------------+\n");
	printf("     | and    | Multiply tables 'first' and 'second'    | add first=\"<table 1>\" second=\"<table 2>\"  |\n");
	printf("     |        | and save result to the selected table   | <attributes for tables F>;                |\n");
	printf("     +--------+-----------------------------------------+-------------------------------------------+\n");
	printf("     | or     | Add tables 'first' and 'second'         | or first=\"<table 1>\" second=\"<table 2>\"   |\n");
	printf("     |        | and save result to the selected table   | <attributes for tables F>;                |\n");
	printf("     +--------+-----------------------------------------+-------------------------------------------+\n");
	printf("     | save   | Save the selected table to the variable | save <attributes for tables SF>;          |\n");
	printf("     +--------+-----------------------------------------+-------------------------------------------+\n");
	printf("     | show   | Show selected table on the screen       | show <attributes for tables F>;           |\n");
	printf("     +--------+-----------------------------------------+-------------------------------------------+\n");
	printf("     |                                            Pupils                                            |\n");
	printf("     +----------------------------------------------------------------------------------------------+\n");
	printf("     | add    | Add new pupil with parameters           | add name=\"<name>\" surname=\"<surname>\"     |\n");
	printf("     |        |                                         |  grade=\"<1..12>\" letter=\"<A..Z>\";         |\n");
	printf("     +--------+-----------------------------------------+-------------------------------------------+\n");
	printf("     | remove | Removes table and pupils from DB        | remove <attributes for tables SF>;        |\n");
	printf("     +--------+-----------------------------------------+-------------------------------------------+\n\n");
	printf("     Attributes tells program how to use this command.\n\n");
	printf("     Value in square brackets shows active command and selected temporal table.\n     To change selected table in command use attribute 'in'.\n");
	printf("     To throw command to execute use ';' after command or attribute.\n     You can write a lot of command in one line or one command in two or more lines.\n\n");
	printf("     Attributes are writing inline after the command word.\n     Attribute must have format like <attr_name>=\"<attr_value>\", but some attributes can use '>' and '<' operator.\n\n");
	printf("     If the attribute is using to write and save information, there is 'S' letter in fourth column.\n");
	printf("     If the attribute is using to find information, there is 'F' letter in four'th column.\n");
	printf("     +------------------------------------------------------+\n");
	printf("     |                      For pupils                      |\n");
	printf("     +------------------------------------------------------+\n");
	printf("     | name    | Pupil's name               | String   | SF |\n");
	printf("     +---------+----------------------------+----------+----+\n");
	printf("     | surname | Pupil's last name          | String   | SF |\n");
	printf("     +---------+----------------------------+----------+----+\n");
	printf("     | grade   | Pupil's grade. Can         | 1..12    | SF |\n");
	printf("     |         | use '>' or '<' instead '=' |          |    |\n");
	printf("     +---------+----------------------------+----------+----+\n");
	printf("     | letter  | Pupil's class letter       | 'A'..'Z' | SF |\n");
	printf("     +---------+----------------------------+----------+----+\n");
	printf("     | middle  | Pupil's middle mark. Can   | Float    | F  |\n");
	printf("     |         | use '>' or '<' instead '=' |          |    |\n");
	printf("     +---------+----------------------------+----------+----+\n");
	printf("     | gender  | If pupil is male           | \"Male\",  | SF |\n");
	printf("     |         | value = \"male\"             | \"Girl\"   |    |\n");
	printf("     +---------+----------------------------+----------+----+\n");
	printf("     |                      For tables                      |\n");
	printf("     +------------------------------------------------------+\n");
	printf("     | in      | Use to get pupils from     | String   | F  |\n");
	printf("     |         | this table                 |          |    |\n");
	printf("     +---------+----------------------------+----------+----+\n");
	printf("     | at      | Use to save changes at     | String   | S  |\n");
	printf("     |         | this table                 |          |    |\n");
	printf("     +---------+----------------------------+----------+----+\n");
	printf("     |                        Special                       |\n");
	printf("     +------------------------------------------------------+\n");
	printf("     | first   | Use to set first operand   | String   | F  |\n");
	printf("     |         | in the operation. Can be   |          |    |\n");
	printf("     |         | table name or attribute    |          |    |\n");
	printf("     |         | (as example to 'save')     |          |    |\n");
	printf("     +---------+----------------------------+----------+----+\n");
	printf("     | second  | Use to set second operand  | String   | F  |\n");
	printf("     |         | in the operation. Can be   |          |    |\n");
	printf("     |         | table name or attribute    |          |    |\n");
	printf("     |         | (as example to 'save')     |          |    |\n");
	printf("     +---------+----------------------------+----------+----+\n");
	mode = command = "";
	prefix();
	return as;
}

void clear()
{
	system("cls");
	init(27);
	tables[""] = tables["all"];
	mode = command = "";
	cout << "Console is cleared!\n\n";
	prefix();
}

void update_pupils()
{
	ofstream pupils_out("pupils.txt");
	ofstream tables_out("tables.txt");
	int i = 0;
	set<int> all = tables["all"];
	vector<int> change(all.size(), -1);
	for (auto j = all.begin(); j != all.end(); ++j)
	{
		change[*j] = i++;
		pupils_out << p_str(school[*j]) << endl;
	}
	for (auto j = tableset.begin(); j != tableset.end(); ++j)
	{
		string a = *j;
		all = tables[a];
		tables_out << a << " ";
		string out = "";
		int l = 0;
		for (auto k = all.begin(); k != all.end(); ++k)
		{
			if (change[*k] != -1)
			{
				++l;
				out += " " + to_string(*k);
			}
		}
		tables_out << l << out << endl;
	}
	pupils_out.close();
	tables_out.close();
}

answer_socket light()
{
	answer_socket as;
	if (islighted)
	{
		system("color 0F");
		as.answer = "Background switched to 'dark'.\nTo return 'light' theme use 'light' again.\n";
	}
	else
	{
		system("color F0");
		as.answer = "Background switched to 'light'.\nTo return 'dark' theme use 'light' again.\n";
	}
	islighted = !islighted;
	mode = command = "";
	return as;
}

answer_socket or_tables(request_socket rs)
{
	answer_socket as;
	if (rs.table_op1 == "empty")
	{
		throw_exception("last line", "The first operator is undefined", 125);
		as.e = 125;
	}
	else if (rs.table_op2 == "empty")
	{
		throw_exception("last line", "The second operator is undefined", 126);
		as.e = 126;
	}
	else
	{
		set<int> ans, op1 = tables[rs.table_op1], op2 = tables[rs.table_op2];
		ans.insert(op1.begin(), op1.end());
		ans.insert(op2.begin(), op2.end());
		tables[""] = ans;
		as.answer = "Tables '" + rs.table_op1 + "' and '" + rs.table_op2 + "' was succesfully added. New table is focused.\n";
		as.table = "";
		command = "";
		mode = rs.table_name;
	}
	return as;
}

answer_socket and_tables(request_socket rs)
{
	answer_socket as;
	if (rs.table_op1 == "empty")
	{
		throw_exception("last line", "The first operator is undefined", 125);
		as.e = 125;
	}
	else if (rs.table_op2 == "empty")
	{
		throw_exception("last line", "The second operator is undefined", 126);
		as.e = 126;
	}
	else
	{
		set<int> ans, op1 = tables[rs.table_op1], op2 = tables[rs.table_op2];
		for (auto i = op1.begin(); i != op1.end(); ++i)
			if (op2.find(*i) != op2.end())
				ans.insert(*i);
		tables[""] = ans;
		as.answer = "Tables '" + rs.table_op1 + "' and '" + rs.table_op2 + "' was succesfully multiplied. New table is focused.\n";
		as.table = "";
		command = "";
		mode = rs.table_name;
	}
	return as;
}

answer_socket save_table(request_socket rs)
{
	answer_socket as;
	if (tables[""].size() == 0)
	{
		throw_exception("last line", "The table for saving isn't selected", 526);
		as.e = 526;
	}
	else if (rs.table_name == "" || rs.table_name == "table")
	{
		throw_exception("last line", "The table name is empty or default", 527);
		as.e = 527;
	}
	else if (rs.table_name == "" || rs.table_name == "all")
	{
		throw_exception("last line", "'" + rs.table_name + "' is the system name", 535);
		as.e = 535;
	}
	else
	{
		if (rs.table == "all")
			tables[rs.table_name] = tables[""];
		else tables[rs.table_name] = tables[rs.table];
		tableset.insert(rs.table_name);
		as.answer = "Table '" + rs.table_name + "' was succesfully saved in the cache.\n";
		as.table = "";
		command = "";
		mode = rs.table_name;
		update_pupils();
	}
	return as;
}

void fop(int n)
{
	for (int i = 0; i < n; ++i)
		cout << ' ';
}

string toleft(string ms, int l)
{
	for (int i = ms.size(); i < l; ++i)
		ms += ' ';
	return ms;
}

answer_socket show_table(request_socket rs)
{
	answer_socket as;
	if (mode == "" && rs.table == "all")
	{
		throw_exception("last line", "The table for showing isn't selected", 536);
		as.e = 536;
	}
	else
	{
		const string lb = "-", ld = " ";
		int mxl[4] = { 0, 0, 0, 0 }, j = 0;
		set<int> from = tables[(rs.table == "all") ? "" : rs.table];
		vector< vector<string> > field(from.size());
		for (set<int>::iterator i = from.begin(); i != from.end(); ++i)
		{
			field[j].resize(4);
			field[j][0] = to_string(school[*i].grade) + school[*i].letter;
			mxl[0] = max(mxl[0], ((int)field[j][0].size()));
			field[j][1] = school[*i].firstname;
			mxl[1] = max(mxl[1], ((int)field[j][1].size()));
			field[j][2] = school[*i].lastname;
			mxl[2] = max(mxl[2], ((int)field[j][2].size()));
			string mark = to_string((int)trunc(school[*i].middle)) + "."
				+ to_string((int)round((school[*i].middle - trunc(school[*i].middle)) * 1e3));
			field[j][3] = mark + " -> [";
			for (int d = 0; d < school[*i].marks.size(); ++d)
				field[j][3] += " " + to_string(school[*i].marks[d]);
			field[j][3] += " ]";
			mxl[3] = max(mxl[3], ((int)field[j][3].size()));
			++j;
		}
		vector<string> ls(4);
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < mxl[i]; ++j)
				ls[i] += "-";
		if (from.size())
		{
			cout << field.size() << " people is showed on screen:\n";
			cout << "+" << lb << ls[0] << lb << "+" << lb << ls[1] << lb << "+" << lb << ls[2] << lb << "+" << lb << ls[3] << lb << "+" << endl;
			for (int i = 0; i < field.size(); ++i)
			{
				cout << "|" << ld << toleft(field[i][0], mxl[0]) << ld << "|";
				cout << ld << toleft(field[i][1], mxl[1]) << ld << "|";
				cout << ld << toleft(field[i][2], mxl[2]) << ld << "|";
				cout << ld << toleft(field[i][3], mxl[3]) << ld << "|" << endl;
			}
			cout << "+" << lb << ls[0] << lb << "+" << lb << ls[1] << lb << "+" << lb << ls[2] << lb << "+" << lb << ls[3] << lb << "+" << endl;
		}
		else
		{
			cout << "Sorry, but selected table is empty.\n\n";
		}
		command = "";
		prefix();
	}
	return as;
}


answer_socket make_table(request_socket rs)
{
	answer_socket as;
	set<int> from = tables[rs.table], to;

	for (set<int>::iterator i = from.begin(); i != from.end(); ++i)
	{
		//printf("%s\n", p_str(school[*i]).c_str());
		pupil p = school[*i];
		if ((rs.pers_name != "" && p.firstname != rs.pers_name) || (rs.pers_sur != "" && p.lastname != rs.pers_sur))
			continue;
		if (rs.pers_letter != "" && p.letter != rs.pers_letter[0])
			continue;
		if (p.grade < rs.pers_grade_min || p.grade > rs.pers_grade_max || p.middle < rs.pers_mark_min || p.middle > rs.pers_mark_max)
			continue;
		if (rs.pers_gender != "" && (isgirl(p.firstname) != (rs.pers_gender == "female")))
			continue;
		to.insert(*i);
	}
	tables[""] = to;
	if (to.size())
	{
		as.answer = "Pupils was succesfully filtered to '" + rs.table_name + "'.\n";
		as.answer += to_string(to.size()) + " pupils was added to the table.\n";
	}
	else
		as.answer = "Sorry, but there are no matching pupils in selected table.\nEmpty table is added.\n";
	as.table = "";
	command = "";
	mode = rs.table_name;

	return as;
}

answer_socket add_pupil(request_socket rs)
{
	answer_socket as;
	if (rs.pers_name == "")
	{
		throw_exception("last line", "Pupil's first name must be specified", 528);
		as.e = 528;
	}
	else if (rs.pers_sur == "")
	{
		throw_exception("last line", "Pupil's last name must be specified", 529);
		as.e = 529;
	}
	else if ((rs.pers_grade_max != rs.pers_grade_min) && !(rs.pers_grade_max == 1000 && rs.pers_grade_min == 0))
	{
		throw_exception("last line", "Pupil's grade can't be in range to set", 530);
		as.e = 530;
	}
	else if (rs.pers_grade_max == 1000 && rs.pers_grade_min == 0)
	{
		throw_exception("last line", "Pupil's grade must be specified", 532);
		as.e = 532;
	}
	else if (rs.pers_letter == "")
	{
		throw_exception("last line", "Pupil's class letter must be specified", 531);
		as.e = 531;
	}
	else
	{
		pupil p;
		p.firstname = rs.pers_name;
		p.lastname = rs.pers_sur;
		p.grade = rs.pers_grade_max;
		p.letter = rs.pers_letter[0];
		school.push_back(p);
		isexist.push_back(true);
		as.answer = " Pupil '" + p_str(p) + "' was succesfully added.\n";
		tables["all"].insert(school.size() - 1);
		tables[rs.table_name].insert(school.size() - 1);
		mode = command = "";
		tables[""] = tables["all"];
		update_pupils();
	}
	return as;
}

answer_socket remove_by_table(request_socket rs)
{
	answer_socket as;
	set<int> deleting;
	int n = tables["all"].size();
	if (rs.table == "all")
		deleting = tables[""];
	else deleting = tables[rs.table];
	for (auto i = deleting.begin(); i != deleting.end(); ++i)
		tables["all"].erase(*i);
	if (tables["all"].size() == n)
		as.answer = "This table is already deleted!\n";
	else
	{
		if (deleting.size() == n)
			as.answer += "All pupils deleted!";
		else as.answer += to_string(n - tables["all"].size()) + " pupils are deleted from DB.\n";
		if (rs.table != "all")
			as.answer += "Table '" + rs.table + "' was removed.";
		as.answer += " Deleted pupils were saved in the temporal table.\n";
		update_pupils();
	}
	if (rs.table_name != "all")
		mode = rs.table_name;
	else mode = "deleted";
	command = "";
	return as;
}

void execute(string com, request_socket rs)
{
	answer_socket as;
	if (com == "or")
		as = or_tables(rs);
	else if (com == "and")
		as = and_tables(rs);
	else if (com == "save")
		as = save_table(rs);
	else if (com == "table")
		as = make_table(rs);
	else if (com == "add")
		as = add_pupil(rs);
	else if (com == "show")
		as = show_table(rs);
	else if (com == "help")
		as = help();
	else if (com == "remove")
		as = remove_by_table(rs);
	else if (com == "light")
		as = light();
	else if (com == "clear")
		clear();
	else
	{
		throw_exception("last line", "Command is invalid or unknown", 500);
		return;
	}
	if (as.answer != "")
	{
		cout << as.answer;
		cout << endl;
		prefix();
	}
}

int main()
{
	//
	// Ã‡Ã Ã¯Ã®Ã«Ã­Ã¥Ã­Ã¨Ã¥ Ã¡Ã Ã§Ã» Ã¤Ã Ã­Ã­Ã»Ãµ
	// 
	ifstream fin;
	fin.open("pupils.txt");
	set<int> now;
	tables[""] = now;
	while (fin && !fin.eof())
	{
		char c;
		pupil p;
		string a, b;
		fin >> a;
		if (a == "")
			break;
		p.grade = atoi(a.substr(0, a.size() - 1).c_str());
		p.letter = a[a.size() - 1];
		fin >> a >> b;
		p.firstname = simp(a);
		p.lastname = simp(b);
		c = fin.get();
		while (c != 10 && c != 13 && c != -1)
		{
			int mark;
			fin >> mark;
			p.marks.push_back(mark);
			c = fin.get();
		}
		tables["all"].insert(school.size());
		school.push_back(p);
		isexist.push_back(true);
		school[school.size() - 1].middle = getmid(school.size() - 1);
		//cout << "created " << school[school.size() - 1].middle << endl;
	}
	fin.close();
	fin.open("tables.txt");
	while (fin && !fin.eof())
	{
		string name;
		int l, k;
		set<int> st;
		fin >> name >> l;
		if (name == "") break;
		tableset.insert(name);
		for (int i = 0; i < l; ++i)
		{
			fin >> k;
			st.insert(k);
		}
		tables[name] = st;
	}
	fin.close();
	tables[""] = tables["all"];
	//
	// Ã‚Ã»Ã¢Ã®Ã¤ Ã¢ ÃªÃ®Ã­Ã±Ã®Ã«Ã¼
	//
	update_pupils();
	init(50);
	prefix();
	while (true)
	{
		string st;
		while (st == "")
			st = specread();
		request_socket rs;
		if (st[st.size() - 1] == ';')
			execute(st.substr(0, st.size() - 1), rs);
		else
		{
			while (true)
			{
				attribute_socket as = get_attr();
				if (as.e != 0) break;
				if (as.type == "name")
					rs.pers_name = as.value_s;
				else if (as.type == "surname")
					rs.pers_sur = as.value_s;
				else if (as.type == "grade")
				{
					if (as.match_type == 0)
					{
						rs.pers_grade_max = as.value_i;
						rs.pers_grade_min = as.value_i;
					}
					else if (as.match_type == 1)
						rs.pers_grade_min = as.value_i;
					else
						rs.pers_grade_max = as.value_i;
				}
				else if (as.type == "middle")
				{
					if (as.match_type == 0)
					{
						rs.pers_mark_max = as.value_i;
						rs.pers_mark_min = as.value_i;
					}
					else if (as.match_type == 1)
						rs.pers_mark_min = as.value_i;
					else
						rs.pers_mark_max = as.value_i;
				}
				else if (as.type == "letter")
					rs.pers_letter = as.value_s;
				else if (as.type == "in")
					rs.table = as.value_s;
				else if (as.type == "at")
					rs.table_name = as.value_s;
				else if (as.type == "gender")
					rs.pers_gender = as.value_s;
				else if (as.type == "first")
					rs.table_op1 = as.value_s;
				else if (as.type == "second")
					rs.table_op2 = as.value_s;
				if (as.active)
				{
					execute(command, rs);
					break;
				}
			}
		}
	}
	system("pause");
}