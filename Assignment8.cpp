/*                  
							Global Documentation

Program Name: Memory Heirarchy Simulation Programming
Name: Harsh Mohan Sason
Lab no: 8

** Program Decription **

This is program is used for stimulating a set of memory operations. 

1. int BtoD(string):  The use of this function is to convert a given binary number to a decimal 
2. string DtoB(int):  The use of this function is to convert a given decmial number to a binary number
3. bool mhs(string):  This function is used to maintain the structure of the memory hierarchy stimulation. 
4. bool lw(string, int): This function is used to stimulate the working of the lw instruction in the memory
5. bool sw(string, int): This function is used to stimuulate the working of the sw instruction in the memory
6. void inti_data():  This function is used to initialize all the  data container for memories in the computer.
7. void display():  This function is used to display the stimulation in the given format.

*/

#include<iostream>
#include<math.h>
#include<vector>
#include<stdio.h>
#include<fstream>
#include<string>
#include<iomanip>

using namespace std;

int BtoD(string);
string DtoB(int);
bool mhs(string);
bool lw(string,int);
bool sw(string,int);
void init_data();
void print();

struct Block
{
	int hist; 
	bool v ; 
	string tag;
	string data;
};

struct Set
{
	Block block0;
	Block block1;
};

vector<string>reg;
vector<Set>cache;
vector<string>mem;

int main()
{
	string instruction;
	ifstream input;
	input.open("02-Input-object-code--correct", ios::in);
	if (input.is_open())
	{
		init_data();
		cout << "Instruction: "<<endl<<endl;
        setw(60);
		while (!input.eof())
		{
			
			input >> instruction;
			input.get();
			cout << instruction << setw(30)<< (mhs(instruction) == true ? "hit" : "miss") << endl;
			if (input.peek() == EOF)
				break;
		}
		cout << endl;
		print();
		input.close();
	}
	else
	{
		cout << "File could not be opened." << endl;
	}
	return 0;
}

string DtoB(int num)
{
	string res;
	while (num != 0)
	{
		int temp = num % 2;
		res.push_back(temp+'0');
		num /= 2;
	}
	while (res.length() < 7)
		res.push_back('0');
	for (int i = 0; i < res.length() / 2; i++)
	{
		swap(res[i], res[res.length() - 1 - i]);
	}
	return res;
}

int BtoD(string num)
{
	int len = num.length() - 1;
	int power = 0;
	int sum = 0;
	int temp;
	while (len >=0)
	{
		temp = num[len--] == '1'?pow(2, power):0;
		power++;
		sum += temp;
	}

	return sum;
}

void init_data()
{
	string zeros = "00000000000000000000000000000000";
	int num = 5; 
	Block bl; 
	Set st;
	bl.hist = 0;
	bl.data = zeros;
	bl.tag = "0000";
	bl.v = 0;
	st.block0 = st.block1 = bl;
	for (int i = 0; i < 128; i++)
	{
		string temp = DtoB(num);
		mem.push_back(zeros.substr(0, zeros.length() - temp.length())+temp);
		num++;
	}

	for (int i = 0; i < 8; i++)
	{
		reg.push_back(zeros);
		cache.push_back(st);
	}
}

bool mhs(string ins)
{
	string addr =DtoB(BtoD(ins.substr(16, ins.length() - 16))/4);
	int rt = BtoD(ins.substr(13, 3));
	if (ins.substr(0, 6) == "100011")
		return lw(addr, rt);
	else
		return sw(addr, rt);
	return 1;

}

bool lw(string addr,int rt)
{
	int index = BtoD(addr) % 8;
	string tag = addr.substr(0, 4);
	bool hit=false;
	if (cache[index].block0.tag == tag)
	{
		if (cache[index].block0.v)
		{
			hit = true;
			reg[rt] = cache[index].block0.data;
			cache[index].block0.hist = 1;
			cache[index].block1.hist = 0;
		}
	}
	else if (cache[index].block1.tag == tag)
	{

		if (cache[index].block1.v)
		{
			hit = true;
			reg[rt] = cache[index].block1.data;
			cache[index].block1.hist = 1;
			cache[index].block0.hist = 0;
		}

	}
	if (!hit)
	{
		string data = mem[BtoD(addr)];
		reg[rt] = data;
		if (!cache[index].block0.v)
		{	
			cache[index].block0.v = true;
			cache[index].block0.tag = tag;
			cache[index].block0.data = data;
			cache[index].block0.hist = 1;
			cache[index].block1.hist = 0;
		}
		else if (!cache[index].block1.v)
		{
			cache[index].block1.v = true;
			cache[index].block1.tag = tag;
			cache[index].block1.data = data;
			cache[index].block1.hist = 1;
			cache[index].block0.hist = 0;
		}
		else
		{
			if (!cache[index].block0.hist)
			{
				cache[index].block0.v = true;
				cache[index].block0.tag = tag;
				cache[index].block0.data = data;
				cache[index].block0.hist = 1;
				cache[index].block1.hist = 0;
			}
			else
			{
				cache[index].block1.v = true;
				cache[index].block1.tag = tag;
				cache[index].block1.data = data;
				cache[index].block1.hist = 1;
				cache[index].block0.hist = 0;
			}
		}
	}
	
	return hit;
}

bool sw(string addr, int rt)
{
	int index = BtoD(addr) % 8;
	string tag = addr.substr(0, 4);
	bool hit = true;
	if (cache[index].block0.tag == tag and cache[index].block0.v)
	{
		cache[index].block0.data = reg[rt];
		cache[index].block0.v = true;
		cache[index].block0.hist = 1;
		cache[index].block1.hist = 0;
	}
	else if (cache[index].block1.tag == tag and cache[index].block1.v)
	{

			cache[index].block1.data = reg[rt];
			cache[index].block1.v = true;
			cache[index].block0.hist = 0;
			cache[index].block1.hist = 1;
	}
	else
	{
		mem[BtoD(addr)] = reg[rt];
		hit = false;
	}
	return hit;
}

void print()
{
	string letterS = "s";
	int i;
	cout << "Register File"<<endl;
	for (int i = 0; i < reg.size(); i++)
	{
		cout << (letterS + to_string(i)) <<"     "<< reg[i] << endl;
	}
	cout << endl;
	cout << "Cache block0" << setw(60) << "Cache block1" << endl;
	cout << "Set#" << setw(8) << "Valid" << setw(7) << " Tag" << setw(8) << "Data" << setw(38) << "Valid" << setw(9) << "Tag" << setw(9) << "Data"  << endl;
	for (int i = 0; i < cache.size(); i++)
	{
		cout << i << setw(8) << cache[i].block0.v << setw(11) << cache[i].block0.tag << "   " << cache[i].block0.data << setw(7) << cache[i].block1.v << setw(13) << cache[i].block1.tag << "   " << cache[i].block1.data << endl;
	}
	cout << endl<<endl;
	cout << "Addr" << setw(8) << "Data" << setw(38) << "Addr" << setw(9) << "Data" << endl;
	for (i = 0; i <= 63; i++)
	{
		cout << i << (i>=10? "      ": "       ") << mem[i] << (i >= 100 ? "       " : "      ") << i + 64 << (i>=36?"      ":"       ") << mem[i + 64] << endl;
	}
}