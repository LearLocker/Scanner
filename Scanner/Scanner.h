#pragma once
#ifndef __Scanner
#define __Scanner
#include "Def.h"

class TScanner {
private: 
	TypeMod t;   // исходный текст
	int uk;     // указатель текущей позиции в исходном тексте
	int curline = 0;
	int simbol_in_line = 0;
public:
	void PutUK(int i, int j);
	int GetUK();
	int GetCurrentLine();
	void ResetCLine();
	void PrintError(char *, char *);
	int Scanner(TypeLex l);
	void GetData(char *);
	TScanner(char *);
	~TScanner() {}
};
#endif // !__Scanner
