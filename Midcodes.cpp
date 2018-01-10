#include "vector"
#include "string"
#include "iostream"
#include "fstream"
#include "map"
#include "stdlib.h"

#include "midcodes.h"
#include "symboltable.h"

using namespace std;

//extern string to_string(int num);
Midcodes::Midcodes(map<string, SymbolTable> &t):tables(t) {}

SymbolItem Midcodes::find(string name, string nkey)
{
	map<string, SymbolTable>::iterator iter = tables.find(nkey);
	if (iter == tables.end())
	{
		return SymbolItem();
	}
	SymbolTable *ntable = &(iter->second);
	if (nkey != "#OverAll")
	{
		if (ntable->isexists(name))
		{
			SymbolItem item = ntable->find(name);
			return item;
		}
		iter = tables.find("#OverAll");
		SymbolTable ttable = iter->second;
		if (ttable.isexists(name))
		{
			SymbolItem item = ttable.find(name);
			return item;
		}
		return SymbolItem();
	}
	else
	{
		if (ntable->isexists(name))
		{
			SymbolItem item = ntable->find(name);
			return item;
		}
		return SymbolItem();
	}
}

void Midcodes::insert(vector<string> strs)
{
	vector<string> s;
	for (int i = 0; i < strs.size(); i++)
	{
		string str;
		str.assign(strs[i]);
		s.insert(s.end(), str);
	}
	clist.insert(clist.end(), s);
}

void Midcodes::insert(vector<string> strs, int index)
{
	vector<string> s;
	for (int i = 0; i < strs.size(); i++)
	{
		string str;
		str.assign(strs[i]);
		s.insert(s.end(), str);
	}
	vector<vector<string>>::iterator iter = clist.begin();
	if (index >= clist.size())
		iter = clist.end();
	else
	{
		int j = 0;
		while (j < index)
		{
			iter++;
			j++;
		}
	}
	clist.insert(iter, s);
}

void Midcodes::refill(string s, int index)
{
	clist[index].insert(clist[index].end(), s);
}

int Midcodes::size()
{
	return clist.size();
}

void Midcodes::output(string filename)
{
	ofstream fout;
	fout.open(filename);
	for (int i = 0; i < clist.size(); i++)
	{
		for (int j = 0; j < clist[i].size(); j++)
		{
			fout << clist[i][j] << " ";
		}
		fout << endl;
	}
	fout.flush();
	fout.close();
}

void Midcodes::loadword(string name, string nkey, string tar, string index)
{
	string base = "0x7fffeffc";
	map<string, SymbolTable>::iterator iter = tables.find(nkey);
	if (iter == tables.end())
	{
		cout << "error" << endl;
		return;
	}
	SymbolTable *ntable = &(iter->second);
	SymbolItem item = ntable->find(name);
	if (item.gettype() != NOTYPE)
	{
		if (index == "")
		{
			string tmp = "lw " + tar + "," + to_string(item.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);
		}
		else
		{
			mpcode.insert(mpcode.end(), "sll " + index + "," + index + ",2");
			string tmp = "li $t1," + to_string(item.getoffset());
			mpcode.insert(mpcode.end(), tmp);
			tmp = "add $t1," + index + ",$t1";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "sub $t1,$fp,$t1";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "lw " + tar + ",0($t1)";
			mpcode.insert(mpcode.end(), tmp);
		}
	}
	else
	{
		iter = tables.find("#OverAll");
		ntable = &(iter->second);
		item = ntable->find(name);
		if (index == "")
		{
			string tmp = "li $t1," + base;
			mpcode.insert(mpcode.end(), tmp);
			tmp = "lw " + tar + "," + to_string(item.getoffset()*-1) + "($t1)";
			mpcode.insert(mpcode.end(), tmp);
		}
		else
		{
			string tmp = "li $t2," + base;
			mpcode.insert(mpcode.end(), tmp);
			mpcode.insert(mpcode.end(), "sll " + index + "," + index + ",2");
			tmp = "li $t1," + to_string(item.getoffset());
			mpcode.insert(mpcode.end(), tmp);
			tmp = "add $t1," + index + ",$t1";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "sub $t1,$t2,$t1";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "lw " + tar + ",0($t1)";
			mpcode.insert(mpcode.end(), tmp);
		}
	}
}

void Midcodes::saveword(string name, string nkey, string src, string index)
{
	string base = "0x7fffeffc";
	map<string, SymbolTable>::iterator iter = tables.find(nkey);
	if (iter == tables.end())
	{
		cout << "error" << endl;
		return;
	}
	SymbolTable *ntable = &(iter->second);
	SymbolItem item = ntable->find(name);
	if (item.gettype() != NOTYPE)
	{
		if (index == "")
		{
			string tmp = "sw " + src + "," + to_string(item.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);
		}
		else
		{
			string tmp = "sll " + index + "," + index + ",2";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "sub " + index + ",$fp," + index;
			mpcode.insert(mpcode.end(), tmp);
			tmp = "sw " + src + "," + to_string(item.getoffset()*-1) + "(" + index + ")";
			mpcode.insert(mpcode.end(), tmp);
		}
	}
	else
	{
		iter = tables.find("#OverAll");
		ntable = &(iter->second);
		item = ntable->find(name);
		if (index == "")
		{
			string tmp = "li $t1," + base;
			mpcode.insert(mpcode.end(), tmp);
			tmp = "sw " + src + "," + to_string(item.getoffset()*-1) + "($t1)";
			mpcode.insert(mpcode.end(), tmp);
		}
		else
		{
			string tmp = "sll " + index + "," + index + ",2";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "li $t1," + base;
			mpcode.insert(mpcode.end(), tmp);
			tmp = "sub " + index + ",$t1," + index;
			mpcode.insert(mpcode.end(), tmp);
			tmp = "sw " + src + "," + to_string(item.getoffset()*-1) + "(" + index + ")";
			mpcode.insert(mpcode.end(), tmp);
		}
	}
}

void Midcodes::toMips(string filename, map<string, SymbolTable> &tables)
{
	string tmp, nfunc = "#OverAll";
	string nkey = "#OverAll";
	map<string, SymbolTable>::iterator giter = tables.find(nkey);
	SymbolTable* ntable = &(giter->second);
	map<string, SymbolItem>::iterator iter = giter->second.symlist.begin();

	mpcode.insert(mpcode.end(), ".data");
	mpcode.insert(mpcode.end(), ".space  " + to_string(ntable->getsize()));

	nkey = "#StringConst";
	giter = tables.find(nkey);
	iter = giter->second.symlist.begin();
	ntable = &(giter->second);

	mpcode.insert(mpcode.end(), "");
	for (; iter != giter->second.symlist.end(); iter++)
	{
		tmp = iter->second.getname() + ": .asciiz " + iter->second.getstr();
		mpcode.insert(mpcode.end(), tmp);
	}

	nkey = "#OverAll";
	giter = tables.find(nkey);
	ntable = &(giter->second);

	mpcode.insert(mpcode.end(), " ");
	mpcode.insert(mpcode.end(), ".text");
	mpcode.insert(mpcode.end(), "move $fp,$sp");
	mpcode.insert(mpcode.end(), "li $s1," + to_string(ntable->getsize()));
	mpcode.insert(mpcode.end(), "sub $sp,$sp,$s1");

	iter = giter->second.symlist.begin();
	for (; iter != giter->second.symlist.end(); iter++)
	{
		if (iter->second.getkind() != CONST_VAR)
			continue;
		SymbolItem item = iter->second;

		tmp = "li $s1," + to_string(item.getvalue());
		mpcode.insert(mpcode.end(), tmp);
		tmp = "sw $s1," + to_string(item.getoffset()*-1) + "($fp)";
		mpcode.insert(mpcode.end(), tmp);
	}

	nkey = "main";
	giter = tables.find(nkey);
	ntable = &(giter->second);
	mpcode.insert(mpcode.end(), "move $fp,$sp");
	mpcode.insert(mpcode.end(), "li $s1," + to_string(ntable->getsize()));
	mpcode.insert(mpcode.end(), "sub $sp,$sp,$s1");

	mpcode.insert(mpcode.end(), "j main");

	for (int i = 0; i < clist.size(); i++)
	{
		vector<string> line = clist[i];
		if (line[0][0] == '.'||(line.size() > 1 && line[1] == ":" ))
		{
			mpcode.insert(mpcode.end(), " ");
			tmp = "";
			for (int j = 0; j < line.size(); j++)
			{
				tmp = tmp + line[j] + " ";
			}
			mpcode.insert(mpcode.end(), tmp);

			if (line[0][0] != '.')
			{
				nfunc.assign(line[0]);
			}
		}
		else if (line[0] == "goto")
		{
			tmp = "j " + line[1];
			mpcode.insert(mpcode.end(), tmp);
		}
		else if (line[0] == "=")
		{
			if (line[1] == "ret")
			{
				saveword(line[2], nkey, "$v0");
				continue;
			}
			loadword(line[1], nkey, "$s1");
			saveword(line[2], nkey, "$s1");
		}
		else if (line[0] == "[]=")
		{
			loadword(line[3], nkey, "$s3");
			loadword(line[2], nkey, "$s2");

			saveword(line[1], nkey, "$s3", "$s2");
		}
		else if (line[0] == "[]")
		{
			loadword(line[2], nkey, "$s2");
			loadword(line[1], nkey, "$s3", "$s2");

			saveword(line[3], nkey, "$s3");
		}
		else if (line[0] == "+" || line[0] == "-" || line[0] == "/" || line[0] == "*")
		{
			if (line[1] == "0")
			{
				mpcode.insert(mpcode.end(), "move $s1,$zero");
			}
			else
			{
				loadword(line[1], nkey, "$s1");
			}
			if (line[2] == "0")
			{
				mpcode.insert(mpcode.end(), "move $s2,$zero");
			}
			else
			{
				loadword(line[2], nkey, "$s2");
			}

			switch (line[0][0])
			{
			case '+':
				tmp.assign("add $s2,$s1,$s2");
				mpcode.insert(mpcode.end(), tmp);
				saveword(line[3], nkey, "$s2");
				break;
			case '-':
				tmp.assign("sub $s2,$s1,$s2");
				mpcode.insert(mpcode.end(), tmp);
				saveword(line[3], nkey, "$s2");
				break;
			case '*':
				tmp.assign("mult $s1,$s2");
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("mflo $s2");
				mpcode.insert(mpcode.end(), tmp);
				saveword(line[3], nkey, "$s2");
				break;
			case '/':
				tmp.assign("div $s1,$s2");
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("mflo $s2");
				mpcode.insert(mpcode.end(), tmp);
				saveword(line[3], nkey, "$s2");
				break;
			}
		}
		else if (line[0] == "bnz")
		{
			if (line.size() == 3)
			{
				loadword(line[1], nkey, "$s1");
				mpcode.insert(mpcode.end(), "bne $zero,$s1," + line[2]);
				continue;
			}
			loadword(line[1], nkey, "$s1");
			loadword(line[3], nkey, "$s2");

			if (line[2] == "==")
			{
				tmp = "beq $s1,$s2," + line[4];
				mpcode.insert(mpcode.end(), tmp);
			}
			else if (line[2] == ">=")
			{
				tmp = "sub $s1,$s1,$s2";
				mpcode.insert(mpcode.end(), tmp);
				tmp = "bgez $s1," + line[4];
				mpcode.insert(mpcode.end(), tmp);
			}
			else if (line[2] == "<=")
			{
				tmp = "sub $s1,$s1,$s2";
				mpcode.insert(mpcode.end(), tmp);
				tmp = "blez $s1," + line[4];
				mpcode.insert(mpcode.end(), tmp);
			}
			else if (line[2] == ">")
			{
				tmp = "sub $s1,$s1,$s2";
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("bgtz $s1," + line[4]);
				mpcode.insert(mpcode.end(), tmp);

			}
			else if (line[2] == "<")
			{
				tmp = "sub $s1,$s1,$s2";
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("bltz $s1," + line[4]);
				mpcode.insert(mpcode.end(), tmp);
			}
			else
			{
				tmp = "bne $s1,$s2," + line[4];
				mpcode.insert(mpcode.end(), tmp);
			}
		}
		else if (line[0] == "func")
		{
			giter = tables.find(line[2]);
			ntable = &(giter->second);
			nkey.assign(line[2]);

			if (line[2] != "main")
				mpcode.insert(mpcode.end(), "sw $ra,0($fp)");
			int num = atoi(line[3].c_str());
			for (int j = 0; j < num; j++)
			{
				line = clist[++i];
				SymbolItem item = find(line[2], nkey);
				tmp = "lw " + string("$s1,") + to_string((12 + 4 * j)*-1) + "($fp)";
				mpcode.insert(mpcode.end(), tmp);
				mpcode.insert(mpcode.end(), "sw $s1," + to_string(item.getoffset()*-1) + "($fp)");
			}
			
			iter = giter->second.symlist.begin();
			for (; iter != giter->second.symlist.end(); iter++)
			{
				if (iter->second.getkind() != CONST_VAR)
					continue;
				SymbolItem item = iter->second;

				tmp = "li $s1," + to_string(item.getvalue());
				mpcode.insert(mpcode.end(), tmp);
				tmp = "sw $s1," + to_string(item.getoffset()*-1) + "($fp)";
				mpcode.insert(mpcode.end(), tmp);
			}

		}
		else if (line[0] == "ret")
		{
			if (line.size() == 1)
			{
				if (nfunc == "main")
				{
					mpcode.insert(mpcode.end(), "li $v0,10");
					mpcode.insert(mpcode.end(), "syscall");
					continue;
				}
				mpcode.insert(mpcode.end(), "lw $ra,0($fp)");
				mpcode.insert(mpcode.end(), "lw $sp,-4($fp)");
				mpcode.insert(mpcode.end(), "lw $fp,-8($fp)");
				mpcode.insert(mpcode.end(), "jr $ra");
				continue;
			}
			loadword(line[1], nkey, "$v0");
			mpcode.insert(mpcode.end(), "lw $ra,0($fp)");
			mpcode.insert(mpcode.end(), "lw $sp,-4($fp)");
			mpcode.insert(mpcode.end(), "lw $fp,-8($fp)");
			mpcode.insert(mpcode.end(), "jr $ra");
		}
		else if (line[0] == "call")
		{
			map<string, SymbolTable>::iterator iter1 = tables.find(line[1]);
			ntable = &(iter1->second);
			string fname = line[1];

			mpcode.insert(mpcode.end(), "sw $sp,-4($sp)");
			mpcode.insert(mpcode.end(), "sw $fp,-8($sp)");
			int num = atoi(line[2].c_str());
			for (int j = 0; j < num; j++)
			{
				line = clist[++i];
				loadword(line[1], nkey, "$s1");
				mpcode.insert(mpcode.end(), "sw $s1," + to_string((12 + 4 * j) * -1) + "($sp)");
			}
			mpcode.insert(mpcode.end(), "move $fp,$sp");
			mpcode.insert(mpcode.end(), "li $s1," + to_string(ntable->getsize()));
			mpcode.insert(mpcode.end(), "sub $sp,$sp,$s1");
			mpcode.insert(mpcode.end(), "jal " + fname);
		}
		else if (line[0] == "li")
		{
			mpcode.insert(mpcode.end(), "li $s1," + line[2]);
			saveword(line[1], nkey, "$s1");
		}
		else if (line[0] == "scan")
		{
			SymbolItem item1 = find(line[1], nkey);
			int flag = item1.gettype() == INT_TYPE ? 5 : 12;
			mpcode.insert(mpcode.end(), "li $v0," + to_string(flag));
			mpcode.insert(mpcode.end(), "syscall");
			saveword(line[1], nkey, "$v0");
		}
		else if (line[0] == "print")
		{
			SymbolItem item1 = find(line[1], nkey);
			if (item1.gettype() == INT_TYPE)
			{
				loadword(line[1], nkey, "$a0");
				mpcode.insert(mpcode.end(), "li $v0,1");
				mpcode.insert(mpcode.end(), "syscall");
			}
			else if (item1.gettype() == CHAR_TYPE)
			{
				loadword(line[1], nkey, "$a0");
				mpcode.insert(mpcode.end(), "li $v0,11");
				mpcode.insert(mpcode.end(), "syscall");
			}
			else
			{
				if (line[1][1] == '#')
				{
					mpcode.insert(mpcode.end(), "");
					tmp = "";
					for (int j = 0; j < line.size(); j++)
					{
						tmp = tmp + line[j] + " ";
					}
					mpcode.insert(mpcode.end(), tmp);
					mpcode.insert(mpcode.end(), "");
				}
				mpcode.insert(mpcode.end(), "la $a0," + line[1]);
				mpcode.insert(mpcode.end(), "li $v0,4");
				mpcode.insert(mpcode.end(), "syscall");
			}
		}
		else
		{
			mpcode.insert(mpcode.end(), "Error");
		}
	}

	ofstream fout;
	fout.open(filename);
	for (int i = 0; i < mpcode.size(); i++)
	{
		fout << mpcode[i] << endl;
	}
	fout.flush();
	fout.close();
}
