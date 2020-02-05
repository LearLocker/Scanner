#pragma once
#ifndef __DIAGRAM
#define __DIAGRAM
#include "Scanner.h"
#include "Semant.h"

class TDiagram
{
private: TScanner *sc;
		 TSemant *sm;
public:
	TDiagram(TScanner *s, TSemant *m) { sc = s; sm = m; }
	~TDiagram(){}
	void S(); // главная программа
	void D(); // описание переменных
	void F(); // описание функций
	void Z(); // список переменных
	DataType V(); // выражение
	void Q(); // состовной оператор
	void K(); // операторы и описания
	void O(); // оператор
	DataType B(); // вызов функции
	void R(); // Return
	void P(); // присваивание
	void W(); // Do{}while()
	DataType A(); // слагаемое
	DataType H(); // множитель
	DataType E(); // элементарное выражение

	void PrintTree();
	DataType resolveType(TypeLex l);
};

#endif // !__DIAGRAM
