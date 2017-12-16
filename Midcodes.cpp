#include "vector"
#include "string"
#include "iostream"
#include "fstream"
#include "map"

#include "midcodes.h"
#include "symboltable.h"

using namespace std;
Midcodes::Midcodes(map<string, SymbolTable> &t):tables(t) {}

SymbolItem Midcodes::find(string name, string nkey)
{
	map<string, SymbolTable>::iterator iter = tables.find(nkey);
	if (iter == tables.end())
	{
		cout << "Not Found: " + name + " in Table:" + nkey << endl;
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
		cout << "Not Found: " + name + " in Table:" + nkey << endl;
		return SymbolItem();
	}
	else
	{
		if (ntable->isexists(name))
		{
			SymbolItem item = ntable->find(name);
			return item;
		}
		cout << "Not Found: " + name + " in Table:" + nkey << endl;
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

void Midcodes::toMips(string filename, map<string, SymbolTable> &tables)
{
	vector<string> mpcode;
	map<string, SymbolTable>::iterator giter = tables.find("#OverAll");
	SymbolTable* ntable = &(giter->second);
	string nkey = "#OverAll";
	map<string, SymbolItem>::iterator iter = giter->second.symlist.begin();

	string tmp;

	mpcode.insert(mpcode.end(), ".data");
	mpcode.insert(mpcode.end(), ".space  " + to_string(ntable->getsize() * 4));

	giter = tables.find("#StringConst");
	iter = giter->second.symlist.begin();
	ntable = &(giter->second);
	nkey = "#StringConst";

	for (; iter != giter->second.symlist.end(); iter++)
	{
		tmp = iter->second.getname() + ": .asciiz " + iter->second.getstr();
		mpcode.insert(mpcode.end(), tmp);
	}

	giter = tables.find("#OverAll");
	ntable = &(giter->second);
	nkey = "#OverAll";

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

	mpcode.insert(mpcode.end(), "j main");

	for (int i = 0; i < clist.size(); i++)
	{
		vector<string> line = clist[i];
		if (line[1] == ":" || line[0][0] == '.')
		{
			mpcode.insert(mpcode.end(), " ");
			tmp = "";
			for (int j = 0; j < line.size(); j++)
			{
				tmp = tmp + line[j] + " ";
			}
			mpcode.insert(mpcode.end(), tmp);
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
				SymbolItem item2 = find(line[2], nkey);
				tmp = "sw " + string("$v0,") + to_string(item2.getoffset()*-1) + "($fp)";
				mpcode.insert(mpcode.end(), tmp);
				continue;
			}

			SymbolItem item1 = find(line[1], nkey);
			SymbolItem item2 = find(line[2], nkey);

			string tmp1 = "lw " + string("$s1,") + to_string(item1.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp1);
			string tmp4 = "sw " + string("$s1,") + to_string(item2.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp4);
		}
		else if (line[0] == "[]=")
		{
			SymbolItem item1 = find(line[1], nkey);
			SymbolItem item2 = find(line[2], nkey);
			SymbolItem item3 = find(line[3], nkey);

			tmp = "lw " + string("$s3,") + to_string(item3.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "lw " + string("$s2,") + to_string(item2.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "sll $s2,$s2,2";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "sub $s2,$fp,$s2";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "sw $s3," + to_string(item1.getoffset()*-1) + "($s2)";
			mpcode.insert(mpcode.end(), tmp);
		}
		else if (line[0] == "[]")
		{
			SymbolItem item1 = find(line[1], nkey);
			SymbolItem item2 = find(line[2], nkey);
			SymbolItem item3 = find(line[3], nkey);

			tmp = "li " + string("$s1,") + to_string(item1.getoffset());
			mpcode.insert(mpcode.end(), tmp);
			tmp = "lw " + string("$s2,") + to_string(item2.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);
			mpcode.insert(mpcode.end(), "sll $s2,$s2,2");
			tmp = "add $s1,$s2,$s1";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "sub $s1,$fp,$s1";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "lw $s3,($s1)";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "sw $s3," + to_string(item3.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);
		}
		else if (line[0] == "+" || line[0] == "-" || line[0] == "/" || line[0] == "*")
		{
			SymbolItem item1 = find(line[1], nkey);
			SymbolItem item2 = find(line[2], nkey);
			SymbolItem item3 = find(line[3], nkey);

			tmp = "lw " + string("$s1,") + to_string(item1.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "lw " + string("$s2,") + to_string(item2.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);

			switch (line[0][0])
			{
			case '+':
				tmp.assign("add $s2,$s1,$s2");
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("sw " + string("$s2,") + to_string(item3.getoffset()*-1) + "($fp)");
				mpcode.insert(mpcode.end(), tmp);
				break;
			case '-':
				tmp.assign("sub $s2,$s1,$s2");
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("sw " + string("$s2,") + to_string(item3.getoffset()*-1) + "($fp)");
				mpcode.insert(mpcode.end(), tmp);
				break;
			case '*':
				tmp.assign("mult $s1,$s2");
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("mflo $s2");
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("sw " + string("$s2,") + to_string(item3.getoffset()*-1) + "($fp)");
				mpcode.insert(mpcode.end(), tmp);
				break;
			case '/':
				tmp.assign("div $s1,$s2");
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("mflo $s2");
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("sw " + string("$s2,") + to_string(item3.getoffset()*-1) + "($fp)");
				mpcode.insert(mpcode.end(), tmp);
				break;
			}
		}
		else if (line[0] == "bnz")
		{
			SymbolItem item1 = find(line[1], nkey);
			SymbolItem item2 = find(line[3], nkey);

			tmp = "lw " + string("$s1,") + to_string(item1.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);
			tmp = "lw " + string("$s2,") + to_string(item2.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);

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
				tmp = "sub $s1,$s1,$s1";
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
				tmp = "lw " + string("$s1,") + to_string(12 + 4 * j) + "($fp)";
				mpcode.insert(mpcode.end(), tmp);
				mpcode.insert(mpcode.end(), "sw $s1," + to_string(item.getoffset()*-1) + "($fp)");
			}
		}
		else if (line[0] == "ret")
		{
			SymbolItem item = find(line[1], nkey);
			tmp = "lw " + string("$v0,") + to_string(item.getoffset()*-1) + "($fp)";

			mpcode.insert(mpcode.end(), "lw $ra,0($fp)");
			mpcode.insert(mpcode.end(), "lw $sp,4($fp)");
			mpcode.insert(mpcode.end(), "lw $fp,8($fp)");
			mpcode.insert(mpcode.end(), "jr $ra");
		}
		else if (line[0] == "call")
		{
			map<string, SymbolTable>::iterator iter1 = tables.find(line[1]);
			ntable = &(iter1->second);
			nkey = line[1];

			mpcode.insert(mpcode.end(), "sw $sp,4($sp)");
			mpcode.insert(mpcode.end(), "sw $fp,8($sp)");
			int num = atoi(line[2].c_str());
			for (int j = 0; j < num; j++)
			{
				line = clist[++i];
				SymbolItem item = find(line[1], nkey);
				tmp = "lw " + string("$s1,") + to_string(item.getoffset()) + "($fp)";
				mpcode.insert(mpcode.end(), tmp);
				mpcode.insert(mpcode.end(), "sw $s1," + to_string(12 + 4 * j) + "($sp)");
			}
			mpcode.insert(mpcode.end(), "move $fp,$sp");
			mpcode.insert(mpcode.end(), "li $s1," + to_string(ntable->getsize()));
			mpcode.insert(mpcode.end(), "sub $sp,$sp,$s1");
			mpcode.insert(mpcode.end(), "jal " + nkey);
		}
		else if (line[0] == "li")
		{
			SymbolItem item1 = find(line[1], nkey);
			mpcode.insert(mpcode.end(), "li $s1," + line[2]);
			tmp = "sw " + string("$s1,") + to_string(item1.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);
		}
		else if (line[0] == "scan")
		{
			SymbolItem item1 = find(line[1], nkey);

			mpcode.insert(mpcode.end(), "li $v0,5");
			mpcode.insert(mpcode.end(), "syscall");
			tmp = "sw " + string("$v0,") + to_string(item1.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);
		}
		else if (line[0] == "print")
		{
			SymbolItem item1 = find(line[1], nkey);
			if (item1.gettype() == INT_TYPE)
			{
				tmp = "lw " + string("$a0,") + to_string(item1.getoffset()*-1) + "($fp)";
				mpcode.insert(mpcode.end(), tmp);
				mpcode.insert(mpcode.end(), "li $v0,1");
				mpcode.insert(mpcode.end(), "syscall");
			}
			else if (item1.gettype() == CHAR_TYPE)
			{
				tmp = "li " + string("$a0,") + to_string(item1.getoffset()*-1);
				mpcode.insert(mpcode.end(), tmp);
				mpcode.insert(mpcode.end(), "add $a0,$fp,$a0");
				mpcode.insert(mpcode.end(), "li $v0,4");
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
