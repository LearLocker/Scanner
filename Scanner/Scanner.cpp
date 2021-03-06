#include "pch.h"
#include <string.h>
#include <stdio.h>
#include "Def.h" 
#include "Scanner.h"
#include <iostream>
using namespace std;

char* _char(string text) {
	char *Serr = new char[text.size() + 1];
	strcpy(Serr, text.c_str());
	return Serr;
}

TScanner::TScanner(char * FileName) {
	GetData(FileName);
	PutUK(0, 0);
}

char sign[] = {'[', ']', '\'', '\"', '_'};
void TScanner::PutUK(int i, int j) { uk = i; curline = j; }	// запомнить указатель 

void TScanner::ResetCLine() { curline = 0; }	// запомнить указатель 

int	TScanner::GetUK() {return uk;}	//восстановить указатель

int	TScanner::GetCurrentLine() { return curline; }	//восстановить указатель строки

void TScanner::PrintError (char  * err, char * a)	
{
	// выдать сообщение об ошибке 
	if (a[0] == '\0')
		cout << "Ошибка : " << err << " " << a << endl;
	else
		cout << "Ошибка : " << err << ". Неверный символ " << a << " в строке " << curline << endl;
	exit(0);
}


int	TScanner::Scanner(TypeLex l) {

	int i;	// текущая длина лексемы
	for (i = 0; i < MAX_LEX; i++)
		l[i] = 0;	//очистили поле лексемы
	i = 0;	// лексема заполняется с позиции i

start:	// все игнорируемые элементы: 
	while ((t[uk] == ' ') || (t[uk] == '\n') || (t[uk] == '\t')) 
	{
		if (t[uk] == '\n')
		{
			curline++;
			simbol_in_line = uk - simbol_in_line;
		}
		uk++;
	}

	// пропуск незначащих элементов 
	if ((t[uk] == '/') && (t[uk + 1] == '/'))
	{ // начался комментарий, надо пропустить текст до ’\n’ 
		uk = uk + 2;
		while ((t[uk] != '\n') && (t[uk] != '#'))	uk++;
		goto start;
	}
	if (t[uk] == '\0') { l[0] = '#'; return TEnd; }

	
	if (t[uk] == '0')
	{
		l[i++] = t[uk++];
		// числовая шестнадцатиричная константа
		if (t[uk] == 'x' || t[uk] == 'X')
		{
			l[i++] = t[uk++];
			while (t[uk] <= '9' && t[uk] >= '0' || ( t[uk] <= 'f' && t[uk] >= 'a' || t[uk] <= 'F' && t[uk] >= 'A'))
			{
				if (i < MAX_LEX - 1) 
					l[i++] = t[uk++]; 
				else uk++;
			}

			return TConsIntHex;
		}
		return TConsIntDec;
	}

	else 
		if ((t[uk] <= '9') && (t[uk] >= '0'))
		{
			l[i++] = t[uk++];
			// числовая константа
			while ((t[uk] <= '9') && (t[uk] >= '0'))
				if (i < MAX_LEX - 1) l[i++] = t[uk++]; else uk++;
			return TConsIntDec;
		}

	else
		if ((t[uk] >= 'a') && (t[uk] <= 'z') || (t[uk] >= 'A') && (t[uk] <= 'Z'))
		{
			int lenght = 1;
			//	начинается идентификатор 
poisk:      l[i++] = t[uk++];
			while ((t[uk] <= '9') && (t[uk] >= '0') || (t[uk] >= 'a')
				&& (t[uk] <= 'z') || (t[uk] >= 'A') && (t[uk] <= 'Z')
				|| t[uk]=='[' || t[uk]==']' || t[uk]=='\'' || t[uk]=='\""' || t[uk] == '_')
				if (i < MAX_LEX - 1)
				{
					l[i++] = t[uk++];
					lenght++;
				}
				else uk++;
			// длинный идентификатор обрезали


			// проверка на ключевое слово: 
			for (int j = 0; j < MAX_KEYW; j++)
				if (strcmp(l, Keyword[j]) == 0)
					return IndexKeyword[j];


			if (lenght > 1)
				if (strstr("long int", l) != NULL && strcmp(l, "long int") < 0 || strstr("long long int", l) != NULL && strcmp(l, "long long int") < 0 || strstr("short int", l) != NULL && strcmp(l, "short int") < 0)
					goto poisk;
			return TIdent;
		}


	else
		if (t[uk] == ',')
		{
			l[i++] = t[uk++]; return TComma;
		}
	else
		if (t[uk] == ';')
		{
			l[i++] = t[uk++]; return TSemicolon;
		}
	else
		if (t[uk] == '(')
		{
			l[i++] = t[uk++]; return TOBracket;
		}
	else
		if (t[uk] == ')')
		{
			l[i++] = t[uk++]; return TCBracket;
		}
	else
		if (t[uk] == '{')
		{
			l[i++] = t[uk++]; return TOBrace;
		}
	else
		if (t[uk] == '}')
		{
			l[i++] = t[uk++]; return TCBrace;
		}
	else
		if (t[uk] == '+')
		{
			l[i++] = t[uk++]; return TPlus;
		}
	else
		if (t[uk] == '-')
		{
			l[i++] = t[uk++]; return TMinus;
		}
	else
	    if (t[uk] == '/')
		{
			l[i++] = t[uk++]; return TDiv;
		}
	else
		if (t[uk] == '%')
		{
			l[i++] = t[uk++]; return TMod;
		}
	else
		if (t[uk] == '*')
		{
			l[i++] = t[uk++]; return TMult;
		}
	else
		if (t[uk] == '<')
		{
			l[i++] = t[uk++];
			if (t[uk] == '=') { l[i++] = t[uk++]; return TEQLess; }
			return TLess;
		}
	else
		if (t[uk] == '>')
		{
			l[i++] = t[uk++];
			if (t[uk] == '=') { l[i++] = t[uk++]; return TEQMore; }
	     	else return TMore;
		}
	else
		if (t[uk] == '!')
		{
			l[i++] = t[uk++];
			if (t[uk] == '=') { l[i++] = t[uk++]; return  TNEQ; }
			else {
					string strerr = "Неверный символ";
					char *err = new char[strerr.size() + 1];
					strcpy(err, strerr.c_str());
					PrintError(err, l); // ошибка
					delete err;
	    			return TErr;
				}
		}
	else
		if (t[uk] == '=')
		{
			l[i++] = t[uk++];
			if (t[uk] == '=') { l[i++] = t[uk++]; return TEQ; }
			else return TSave;
		}
	else {
		string strerr = "Неверный символ";
		char *err = new char[strerr.size() + 1];
		strcpy(err, strerr.c_str());
		PrintError(err, l); // ошибка
		delete err;
		uk++;
		return TErr;
	}

} // конец Scaner


void TScanner::GetData(char * FileName) 
{
		// ввод файла FileName, который содержит текст исходного модуля 
		char aa;
		cout << FileName << endl;
		FILE * in = fopen(FileName, "r");

		string strFile = "Отсутствует входной файл";
		string sprob = "";
		char *probel = new char[strFile.size() + 1];
		char *InpFile = new char[strFile.size() + 1];
		strcpy(InpFile, strFile.c_str());
		strcpy(probel, sprob.c_str());

		if (in == NULL) { PrintError(InpFile, probel); exit(1); }

		int i = 0;
		while (!feof(in))
		{
			fscanf(in, "%c", &aa);
			if (!feof(in)) t[i++] = aa; 
			if (i >= MAX_TEXT - 1)
			{
				string SizeModule = "Слишком большой размер исходного модуля";
				char *Mod = new char[SizeModule.size() + 1];
				strcpy(Mod, SizeModule.c_str());
				PrintError(Mod, probel);
				delete Mod; break;
			}
		}
		t[i] = '\0';	// приписываем знак ’\0’ в конец текста
		delete InpFile;
		fclose(in);
} // конец GetData()