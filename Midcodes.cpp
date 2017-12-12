#include "vector"
#include "string"
#include "iostream"
#include "fstream"
#include "map"

#include "midcodes.h"
#include "symboltable.h"

using namespace std;

SymbolItem find(string name, string nkey, SymbolTable *ntable, map<string, SymbolTable> &tables)
{
	if (nkey != "#OverAll")
	{
		if (ntable->isexists(name))
		{
			SymbolItem item = ntable->find(name);
			return item;
		}
		map<string, SymbolTable>::iterator iter = tables.find("#OverAll");
		SymbolTable &ttable = iter->second;
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

void Midcodes::toMips(string filename, map<string, SymbolTable> &tables)
{
	vector<string> mpcode;
	mpcode.insert(mpcode.end(), ".data");
	mpcode.insert(mpcode.end(), ".word  0x1000");

	map<string, SymbolTable>::iterator iter1 = tables.find("#StringConst");
	map<string, SymbolItem>::iterator iter = iter1->second.symlist.begin();

	for (; iter != iter1->second.symlist.end(); iter++)
	{
		string tmp = iter->second.getname() + ": .asciiz " + iter->second.getstr();
		mpcode.insert(mpcode.end(), tmp);
	}

	int stringend = mpcode.size();

	iter1 = tables.find("#OverAll");
	SymbolTable* ntable = &(iter1->second);
	string nkey = "#OverAll";

	mpcode.insert(mpcode.end(), "");
	mpcode.insert(mpcode.end(), ".text");
	mpcode.insert(mpcode.end(), "move $fp,$sp");
	mpcode.insert(mpcode.end(), "li $s1," + to_string(ntable->getsize()));
	mpcode.insert(mpcode.end(), "sub $sp,$sp,$s1");
	mpcode.insert(mpcode.end(), "j main");

	for (int i = 0; i < clist.size(); i++)
	{
		vector<string> line= clist[i];
		if (line[1] == ":" || line[0][0] == '.')
		{
			string tmp;
			for (int j = 0; j < line.size(); j++)
			{
				tmp = tmp + line[j] + " ";
			}
			mpcode.insert(mpcode.end(), tmp);
		}
		else if (line[0] == "goto")
		{
			string tmp;
			tmp = "j " + line[1];
		}
		else if (line[0] == "=")
		{
			if (line[1] == "ret")
			{
				string tmp;
				SymbolItem item2 = find(line[2], nkey, ntable, tables);
				tmp = "sw " + string("$v0,") + to_string(item2.getoffset()*-1) + "($fp)";
				mpcode.insert(mpcode.end(), tmp);
				continue;
			}

			SymbolItem item1 = find(line[1], nkey, ntable, tables);
			SymbolItem item2 = find(line[2], nkey, ntable, tables);

			string tmp1 = "lw " + string("$s1,") + to_string(item1.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp1);
			string tmp4 = "sw " + string("$s1,") + to_string(item2.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp4);
		}
		else if (line[0] == "[]=")
		{
			SymbolItem item1 = find(line[1], nkey, ntable, tables);
			SymbolItem item2 = find(line[2], nkey, ntable, tables);
			SymbolItem item3 = find(line[3], nkey, ntable, tables);

			string tmp1 = "lw " + string("$s3,") + to_string(item3.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp1);
			string tmp2 = "lw " + string("$s2,") + to_string(item2.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp2);
			string tmp3 = "sll $s2,$s2,2";
			mpcode.insert(mpcode.end(), tmp3);
			string tmp4 = "sub $s2,$fp,$s2";
			mpcode.insert(mpcode.end(), tmp4);
			string tmp5 = "sw $s3," + to_string(item1.getoffset()*-1)+"($s2)";
			mpcode.insert(mpcode.end(), tmp5);
		}
		else if (line[0] == "[]")
		{
			SymbolItem item1 = find(line[1], nkey, ntable, tables);
			SymbolItem item2 = find(line[2], nkey, ntable, tables);
			SymbolItem item3 = find(line[3], nkey, ntable, tables);

			string tmp1 = "li " + string("$s1,") + to_string(item1.getoffset());
			mpcode.insert(mpcode.end(), tmp1);
			string tmp2 = "lw " + string("$s2,") + to_string(item2.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp2);			
			mpcode.insert(mpcode.end(), "sll $s2,$s2,2");
			string tmp3 = "add $s1,$s2,$s1";
			mpcode.insert(mpcode.end(), tmp3);
			string tmp4 = "sub $s1,$fp,$s1";
			mpcode.insert(mpcode.end(), tmp4);
			string tmp5 = "lw $s3,($s1)";
			mpcode.insert(mpcode.end(), tmp5);
			string tmp6 = "sw $s3," + to_string(item3.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp6);
		}
		else if (line[0] == "+" || line[0] == "-" || line[0] == "/" || line[0] == "*")
		{
			SymbolItem item1 = find(line[1], nkey, ntable, tables);
			SymbolItem item2 = find(line[2], nkey, ntable, tables);
			SymbolItem item3 = find(line[3], nkey, ntable, tables);

			string tmp1 = "lw " + string("$s1,") + to_string(item1.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp1);
			string tmp2 = "lw " + string("$s2,") + to_string(item2.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp2);

			string tmp;
			switch (line[0][0])
			{
			case '+':
				tmp.assign("add $s2,$s1,$s2");
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("sw "+ string("$s2,") + to_string(item3.getoffset()*-1) + "($fp)");
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
			SymbolItem item1 = find(line[1], nkey, ntable, tables);
			SymbolItem item2 = find(line[3], nkey, ntable, tables);

			string tmp1 = "lw " + string("$s1,") + to_string(item1.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp1);
			string tmp2 = "lw " + string("$s2,") + to_string(item2.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp2);

			if (line[2] == "==")
			{
				string tmp = "beq $s1,$s2," + line[4];
				mpcode.insert(mpcode.end(), tmp);
			}
			else if (line[2] == ">=")
			{
				string tmp1 = "sub $s1,$s1,$s2";
				mpcode.insert(mpcode.end(), tmp1);
				string tmp2 = "bgez $s1," + line[4];
				mpcode.insert(mpcode.end(), tmp2);
			}
			else if (line[2] == "<=")
			{
				string tmp = "sub $s1,$s1,$s1";
				mpcode.insert(mpcode.end(), tmp);
				string tmp1 = "blez $s1," + line[4];
				mpcode.insert(mpcode.end(), tmp1);
			}
			else if (line[2] == ">")
			{
				string tmp = "sub $s1,$s1,$s2";
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("bgtz $s1," + line[4]);
				mpcode.insert(mpcode.end(), tmp);

			}
			else if (line[2] == "<")
			{
				string tmp = "sub $s1,$s1,$s2";
				mpcode.insert(mpcode.end(), tmp);
				tmp.assign("bltz $s1," + line[4]);
				mpcode.insert(mpcode.end(), tmp);
			}
			else
			{
				string tmp = "bne $s1,$s2," + line[4];
				mpcode.insert(mpcode.end(), tmp);
			}
		}
		else if (line[0] == "func")
		{
			iter1 = tables.find(line[2]);
			ntable = &(iter1->second);
			nkey.assign(line[2]);
			
			if(line[2] != "main")
				mpcode.insert(mpcode.end(), "sw $ra,0($fp)");
			int num = atoi(line[3].c_str());
			for (int j = 0; j < num; j++)
			{
				line = clist[++i];
				SymbolItem item = find(line[2], nkey, ntable, tables);
				string tmp = "lw " + string("$s1,") + to_string(12+4*j) + "($fp)";
				mpcode.insert(mpcode.end(), tmp);
				mpcode.insert(mpcode.end(), "sw $s1," + to_string(item.getoffset()*-1) + "($fp)");
			}
		}
		else if (line[0] == "ret") 
		{
			SymbolItem item = find(line[1], nkey, ntable, tables);
			string tmp = "lw " + string("$v0,") + to_string(item.getoffset()*-1) + "($fp)";

			mpcode.insert(mpcode.end(), "li $ra,0($fp)");
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
				SymbolItem item = find(line[1], nkey, ntable, tables);
				string tmp = "lw " + string("$s1,") + to_string(item.getoffset()) + "($fp)";
				mpcode.insert(mpcode.end(), tmp);
				mpcode.insert(mpcode.end(), "sw $s1,"+to_string(12+4*j)+"($sp)");
			}
			mpcode.insert(mpcode.end(), "move $fp,$sp");
			mpcode.insert(mpcode.end(), "li $s1," + ntable->getsize());
			mpcode.insert(mpcode.end(), "sub $sp,$sp,$s1");
			mpcode.insert(mpcode.end(), "jal " + nkey);
		}
		else if (line[0] == "li")
		{
			SymbolItem item1 = find(line[1], nkey, ntable, tables);
			mpcode.insert(mpcode.end(), "li $s1,"+line[2]);
			string tmp = "sw " + string("$s1,") + to_string(item1.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);
		}
		else if (line[0] == "scan")
		{
			SymbolItem item1 = find(line[1], nkey, ntable, tables);

			mpcode.insert(mpcode.end(), "li $v0,5");
			mpcode.insert(mpcode.end(), "syscall");
			string tmp = "sw " + string("$v0,") + to_string(item1.getoffset()*-1) + "($fp)";
			mpcode.insert(mpcode.end(), tmp);
		}
		else if (line[0] == "print")
		{
			SymbolItem item1 = find(line[1], nkey, ntable, tables);
			if (item1.gettype() != NOTYPE)
			{
				if (item1.gettype() == INT_TYPE)
				{
					string tmp1 = "lw " + string("$a0,") + to_string(item1.getoffset()*-1) + "($fp)";
					mpcode.insert(mpcode.end(), tmp1);
					mpcode.insert(mpcode.end(), "li $v0,1");
					mpcode.insert(mpcode.end(), "syscall");
				}
				else
				{
					string tmp1 = "li " + string("$a0,") + to_string(item1.getoffset()*-1);
					mpcode.insert(mpcode.end(), tmp1);
					mpcode.insert(mpcode.end(), "add $a0,$fp,$a0");
					mpcode.insert(mpcode.end(), "li $v0,4");
					mpcode.insert(mpcode.end(), "syscall");
				}
				continue;
			}
			mpcode.insert(mpcode.end(), "la $a0,"+line[1]);
			mpcode.insert(mpcode.end(), "li $v0,4");
			mpcode.insert(mpcode.end(), "syscall");
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
