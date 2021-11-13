#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>


using namespace std;  // структура котороя помещается в список , вроде работает

struct Freq // таблица частотности
{
	char s;
	int n;
	double high = 0;
	double low = 0;
};

string FileToString(ifstream& file) // читаем текст с файла
{
	string text;
	if (file.is_open())
	{
		stringstream ss;
		ss << file.rdbuf();
		text = ss.str();
		file.close();
	}
	else
		cout << "Your file are failed!" << endl;

	return text;
}
Freq* CreateTableOfFrequency(string text) // заполняем таблицу частотности
{
	char symbols[255];  // заносим все символы ASCII в массив
	for (int i = 0; i < 255; i++)
		symbols[i] = i;

	Freq* temp = new Freq[255]; // создаем таблицу частотности через (структуру и массив)
	int k = 0;

	for (int i = 0; i < 255; i++) // бежим по входному тексту и считываем частотности каждого символа, записывая в (временную) таблицу частотности
	{
		for (int j = 0; j < text.size(); j++)
			if (symbols[i] == text[j])
				k++;
		temp[i].s = symbols[i];
		temp[i].n = k;
		k = 0;
	}


	for (int i = 0; i < 255; i++) // смотрим какие символы БЫЛИ в исходном тексте
		if (temp[i].n != 0)
			k++;

	int size = k;
	Freq* table = new Freq[256]; // создаем финальную таблицу частотности
	k = 0;

	for (int i = 0; i < 255; i++) // заносим в финальную таблицу данные
		if (temp[i].n != 0)
		{
			table[k].n = temp[i].n;
			table[k].s = temp[i].s;
			k++;
		}
	table[k].n = 0;


	int tmp;
	char tmps;
	int j;
	for (int i = 1; i < size; i++) // сортируем финальную таблицу по убыванию (прямая вставка)
	{
		tmp = table[i].n;
		tmps = table[i].s;
		for (j = i - 1; j >= 0 && table[j].n < tmp; j--)
		{
			table[j + 1].n = table[j].n;
			table[j + 1].s = table[j].s;
		}
		table[j + 1].n = tmp;
		table[j + 1].s = tmps;
	}

	double templow = 0;
	double temphigh = 0;

	for (int i = 0; i < size; i++)
	{
		templow = temphigh;
		temphigh += ((double)table[i].n / text.size());

		table[i].low = templow;
		table[i].high = temphigh;
	}
	


	

	delete[]temp;
	return table;
}
double EncodeInterval(Freq* table, string text)
{
	int i = 0;
	int k = 0;
	double low = 0;
	double high = 0;

	while (table[i].s != text[k])
		i++;

	
	double lowtemp = table[i].low;
	double hightemp = table[i].high;

	k++;

	for (; k < text.size(); k++)
	{
		i = 0;
		while (table[i].s != text[k])
			i++;

		low = lowtemp + (hightemp - lowtemp) * table[i].low;
		high = lowtemp + (hightemp - lowtemp) * table[i].high;

		lowtemp = low;
		hightemp = high;

		cout << "[" << low << " ; " << high << ")" << endl; // проверка на интервалы
	}
	return low;
}
void PrintInformaton(Freq* table) // вывод таблицы на экран (символ, кол-во символов, код символа)
{
	cout << "********* ТАБЛИЦА ЧАСТОТНОСТИ С ИНТЕРВАЛАМИ *********" << endl << endl;
	cout << "CHAR   FREQ         INTERVAL" << endl << endl;
	int i = 0;
	while (table[i].n != 0)
	{
		cout << table[i].s << "   =   " << table[i].n << "   =   " << "[" << table[i].low << " ; " << table[i].high << ")";
		cout << endl;
		i++;
	}
	cout << "************************************************" << endl;
}
string TableOfFrequencyToText(Freq* table)
{
	int i = 0;
	string text;
	int k = 0;
	int n = 0;
	while (table[i].n != 0)
	{
		text += table[i].s;
		n += table[i].n;
		text += '[';
		text += to_string(table[i].low);
		text += ';';
		text += to_string(table[i].high);
		text += ')';
		i++;
		k++;
	}
	text += "count=";
	text += to_string(n);
	text += "size=";
	text += to_string(k);
	return text;
}
void TextToFile(ofstream& file, string text)
{
	file << text;
}
void ENCODE(ifstream& input, ofstream& out, ofstream& outtable)
{
	string text = FileToString(input);
	cout << "ENCODING TEXT: " << text << endl;
	Freq* table = CreateTableOfFrequency(text);
	PrintInformaton(table);
	double k = EncodeInterval(table, text);
	string tabletxt = TableOfFrequencyToText(table);
	string outputencode = to_string(k);
	cout << "RESULT_OF_ENCODUNG: " << outputencode << endl;
	cout << "TABLE_FOR_DECODING: " << tabletxt << endl;
	TextToFile(outtable, tabletxt);
	TextToFile(out, outputencode);
}


int ReturnCountOfSymbols(string text) // ицем в сжатом файле пометку "size=" чтобы извлечь из файла количество символов в итоговом файле
{
	int i = 0;
	int k;
	for (i = text.find("size=", i++); i != string::npos; i = text.find("size=", i + 1))
		k = i;
	k += 5;

	string tmp;
	for (; k < size(text); k++)
		tmp += text[k];
	int result = stoi(tmp);
	return result;
}
int ReturnSizeOfENCODINGtxt(string text)
{
	int i = 0;
	int k;
	int j;

	for (i = text.find("size=", i++); i != string::npos; i = text.find("size=", i + 1))
		k = i;

	i = 0;

	for (i = text.find("count=", i++); i != string::npos; i = text.find("count=", i + 1))
		j = i;
	j += 6;

	string tmp;
	for (; j < k; j++)
		tmp += text[j];
	int result = stoi(tmp);
	return result;
}
Freq* TableFromFile(string text, int k)
{
	Freq* table = new Freq[k + 1];
	int i = 0;
	string tmp;

	for (int j = 0; j < k; j++)
	{
		table[j].s = text[i];
		table[j].n = -1;
		i += 2;

		while (text[i] != ';')
		{
			tmp += text[i];
			i++;
		}
		table[j].low = stod(tmp);
		tmp.clear();
		i++;
		while (text[i] != ')')
		{
			tmp += text[i];
			i++;
		}
		table[j].high = stod(tmp);
		tmp.clear();
		i++;
	}
	table[k].n = 0;
	return table;
}
double LowFromTxt(string text)
{
	return stod(text);
}
string TableToDECODE(Freq* table, double ll, int n)
{
	double code = ll;
	int j = 0;
	string result;

	for (int i = 0; i < n; i++)
	{
		while ((code < table[j].low) || (code >= table[j].high))
			j++;

		result += table[j].s;
		code = (code - table[j].low) / (table[j].high - table[j].low);
		j = 0;
	}
	return result;
}
void DECODE(ifstream& input, ifstream& inputtable, ofstream& out)
{
	string table = FileToString(inputtable);
	string encode = FileToString(input);
	Freq* TABLE = TableFromFile(table, ReturnCountOfSymbols(table));
	string result = TableToDECODE(TABLE, LowFromTxt(encode), ReturnSizeOfENCODINGtxt(table));
	TextToFile(out, result);
	cout << "DECODING TEXT: " << encode << endl;
	cout << "TABLE_OF_DECODING: " << table << endl;
	PrintInformaton(TABLE);
	cout << "RESULT: " << result << endl;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	////////// ENCODE //////////////
	ifstream infile("InputENCODE.txt");
	ofstream outfile("OutputENCODE.txt");
	ofstream outfilecode("OutputTableENCODE.txt");

	ENCODE(infile, outfile, outfilecode);
	


	///////////DECODE/////////////////
	ifstream infiled("InputDECODE.txt");
	ifstream infiletable("InputTableDECODE.txt");
	ofstream outfiled("OutFileDECODE.txt");

	DECODE(infiled, infiletable, outfiled);
	

	infile.close();
	outfile.close();
	outfile.close();
	infiled.close();
	infiletable.close();
	outfiled.close();
	return 0;

}