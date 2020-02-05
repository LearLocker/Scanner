#include "pch.h"
#include <string.h> 
#include <iostream>
#include <stdio.h>
#include "Def.h" 
#include "Scanner.h"
#include "Diagram.h" 
#define max(a,b) a<b? b : a

using namespace std;

char* _Char(string text) {
	char *Serr = new char[text.size() + 1];
	strcpy(Serr, text.c_str());
	return Serr;
}

void TDiagram::PrintTree()
{
	sm->print_tree(sm->Root);
}

// 
//	главная программа
//	S → SF | SD | SM | ε
//
void TDiagram::S()
{
	TypeLex l; int t, uk1, cl1;
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	sc->PutUK(uk1, cl1);
	do
	{
		uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
		t = sc->Scanner(l);
		if (t == TEnd)
			break;
		if (t == TInt || t == TSInt || t == TLInt || t == TLInt)
		{
			t = sc->Scanner(l);
			
 			if (t == TSemicolon)
				continue;

			if (t == TMain)
			{
				sc->PutUK(uk1,cl1);
				F();
			}

			if (t == TIdent)
			{
				t = sc->Scanner(l);
				if (t == TOBracket)
				{
					sc->PutUK(uk1,cl1);
					F();
				}
				else
				{
					sc->PutUK(uk1,cl1);
					D();
				}
			}
		}
		else
			sc->PrintError(_Char("ожидался ввод типа"), l);
	} while (t != TCBrace);
}

//
//	описание функций
//	F → T a() Q
//	M → T main() Q
//	T → int|short int|long int|long long int
//
void TDiagram::F()
{
	TypeLex l; int t, uk1, cl1;
	string strerr;
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);

	DataType type = resolveType(l);

	if (!(t == TInt || t == TSInt || t == TLInt || t == TLLInt))
		sc->PrintError((char*)"ожидался ввод типа функции", l);
	t = sc->Scanner(l);
	if (!(t == TIdent || t == TMain))
		sc->PrintError((char*)"ожидался идентификатор или ключевое слово main", l);

	sm->SetFunct(type, l);

	t = sc->Scanner(l);
	if (t != TOBracket)
		sc->PrintError((char*)"ожидался символ (", l);
	t = sc->Scanner(l);
	if (t != TCBracket)
		sc->PrintError((char*)"ожидался символ )", l);
	Q();

	if (!sm->checkReturn())
		sm->SemantPrintError((char*)"Отсутствует оператор return в функции", l, cl1);

	sm->ToUp();	// восстановили исходную позицию в дереве
}

//
//	описание переменных
//	D → TZ
//	T → int|short int|long int|long long int
//	список переменных
//	Z → Z, I|I
//	I → a|a=V
//
void TDiagram::D()
{
	TypeLex l; int t, uk1, cl1;
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);

	DataType type = resolveType(l);

	if (!(t == TInt || t == TSInt || t == TLInt || t == TLLInt))
		sc->PrintError((char*)"ожидался ввод типа переменной", l);

	do {
		t = sc->Scanner(l);
		if (t != TIdent)
			sc->PrintError((char*)"ожидался идентификатор", l);

		sm->SetVariable(type, l);

		uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
		t = sc->Scanner(l);
		if (t == TSave)
		{
			DataType expression_type = V();
			if (!sm->checkCompability(type, expression_type))
				sm->SemantPrintError((char*)"Ошибка приведения типа", (char*)"", cl1);
			uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
			t = sc->Scanner(l);
		}
	} while (t == TComma);
	sc->PutUK(uk1, cl1);

	t = sc->Scanner(l);
	if (t != TSemicolon)
		sc->PrintError((char*)"ожидался символ ;", l);
}

//
//	вызов функции
//	B → a();
//
DataType TDiagram::B()
{
	TypeLex l; int t, uk1, cl1;
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);
	if (t != TIdent)
		sc->PrintError(_Char("ожидался идентификатор"), l);
	DataType type = sm->SemGetFunct(l);
	t = sc->Scanner(l);
	if (t != TOBracket)
		sc->PrintError(_Char("ожидался символ ("), l);
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);
//	if (t == TCBracket) return; // нет параметров 
	if (t != TCBracket)
		sc->PrintError(_Char("ожидался знак )"), l);
	return type;
}

//
//	Return
//	R → return V;
//
void TDiagram::R()
{
	TypeLex l; int t, uk1, cl1;
	t = sc->Scanner(l); cl1 = sc->GetCurrentLine();
	if (t != TReturn)
		sc->PrintError(_Char("ожидался символ return"), l);
	DataType return_type = V();
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);
	TypeLex* func = (TypeLex*)sm->FindUp();
	//memcpy(func, sm->FindUp(), max(strlen(func), strlen(*sm->FindUp())));

	DataType func_type = sm->SemGetFunct((char*)func);
	if(!sm->checkCompability(func_type, return_type))
		sm->SemantPrintError(_Char("Ошибка приведения типа"), _Char(""), cl1);

	if (t != TSemicolon)
		sc->PrintError(_Char("ожидался символ ;"), l);
	sm->assignReturn();
}

//
//	состовной оператор
//	Q → {K}
//
void TDiagram::Q()
{
	TypeLex l; int t;
	sm->SetBlock();
	t = sc->Scanner(l);
	if (t == TSemicolon)
		return;
	if (t != TOBrace)
		sc->PrintError(_Char("ожидался символ {"), l);
	K();
	t = sc->Scanner(l);
	if (t != TCBrace)
		sc->PrintError(_Char("ожидался символ }"), l);
	sm->ToUp();
}

//
//	операторы и описания
//	K → KO|KD| ε
//
void TDiagram::K()
{
	TypeLex l; int t, uk1, cl1;
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);
	sc->PutUK(uk1, cl1);
	while (t != TCBrace)
	{
		if (t == TInt || t == TSInt || t == TLInt || t == TLLInt)
			D();
		else
			O();
		uk1 = sc->GetUK();  cl1 = sc->GetCurrentLine();
		t = sc->Scanner(l);	
		sc->PutUK(uk1, cl1);
	}
}

//
//	выражение
//	V → V>A|V<A|V>=A|V <= A|V == A| V != A|+A|-A|A
//
DataType TDiagram::V()
{
	TypeLex l; int t, uk1, cl1;
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);
	if ((t != TPlus) && (t != TMinus))
		sc->PutUK(uk1, cl1);
	DataType type = A();
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);
	while ((t <= TNEQ) && (t >= TLess)) // знаки сравнения стоят подряд
	{
		DataType new_type = A();
		type = sm->Cast(type, new_type);
		uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
		t = sc->Scanner(l);
	}
	sc->PutUK(uk1, cl1);
	return type;
}

//
//	оператор
//	O → P|Q|W|B|R|;
//
void TDiagram::O()
{
	TypeLex l; int t, uk1, cl1;
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);
	if (t == TSemicolon)
		return; // пустой оператор
	if (t == TDo)
	{
		sc->PutUK(uk1, cl1);
		W();
		return;
	}
	if (t == TReturn)
	{
		sc->PutUK(uk1, cl1); 
		R();	
		return; 
	}
	if (t == TOBrace) 
	{
		sc->PutUK(uk1, cl1);
		Q();
		return; 
	}
	// остались H и P , анализируемые по first2 
	t = sc->Scanner(l);
	sc->PutUK(uk1, cl1);
	if (t == TSave)
	{
		P();
		t = sc->Scanner(l);
		if (t != TSemicolon)
			sc->PrintError(_Char("ожидался символ ;"), l);
	}
	else
		B();
}

//
//	присваивание
//	P → a=V
//
void TDiagram::P()
{
	TypeLex l; int t, uk1, cl1;
	t = sc->Scanner(l);
	if (t != TIdent)
		sc->PrintError(_Char("ожидался идентификатор"), l);

	TypeLex ident;
	memcpy(ident, l, max(strlen(ident), strlen(l)));
	DataType var_type = sm->SemGetType(l);

	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);
	if (t != TSave)
		sc->PrintError(_Char("ожидался знак ="), l);

	DataType type = V();
	if (!sm->checkCompability(var_type, type))
		sm->SemantPrintError(_Char("Ошибка приведения типа"), _Char(""), cl1);
}

//
//	Do{}while()
//	W → do Q while(V);
//
void TDiagram::W()
{
	TypeLex l; int t, uk1, cl1;
	t = sc->Scanner(l);
	if (t != TDo) sc->PrintError(_Char("ожидался символ do"), l); 

	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);
	sc->PutUK(uk1, cl1);
	
	if (t != TOBrace) sc->PrintError(_Char("ожидался символ {"), l);  {  Q(); }
	t = sc->Scanner(l);
	if (t != TWhile) sc->PrintError(_Char("ожидался символ while"), l);

	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);
	sc->PutUK(uk1, cl1);

	if (t != TOBracket) sc->PrintError(_Char("ожидался символ ("), l); V();
	t = sc->Scanner(l);
	if (t != TSemicolon) sc->PrintError(_Char("ожидался символ ;"), l);

}

//
//	слагаемое
//	A → A+H|A-H|H
//
DataType TDiagram::A()
{
	TypeLex l; int t, uk1, cl1;
	DataType type = H();
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);
	while ((t == TPlus) || (t == TMinus))
	{
		DataType new_type = H();
		type = sm->Cast(type, new_type);
		uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
		t = sc->Scanner(l);
	}
	sc->PutUK(uk1, cl1);
	return type;
}

//
//	множитель
//	H → H*E|H/E|H%E|E
//
DataType TDiagram::H()
{
	TypeLex l; int t, uk1, cl1;
	DataType type = E();
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine(); t = sc->Scanner(l);
	while ((t == TDiv) || (t == TMult) || (t == TMod))
	{
		DataType new_type = E();
		type = sm->Cast(type, new_type);
		uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
		t = sc->Scanner(l);
	}
	sc->PutUK(uk1, cl1);
	return type;
}

//
//	 элементарное выражение
//	E → C|B(V)
//	C →c1|c2
//
DataType TDiagram::E()
{
	TypeLex l; int t, uk1, cl1;
	DataType type = TYPE_UNKNOWN;
	uk1 = sc->GetUK(); cl1 = sc->GetCurrentLine();
	t = sc->Scanner(l);
	if (t == TConsIntDec )
	{
		type = TYPE_INTEGER; return type;
	}
	if( t == TConsIntHex )
	{
		type = TYPE_INTEGER; return type;
	}
	if (t == TIdent)
	{
		type = sm->SemGetType(l); return type;
	}
	if (t == TOBracket)
	{
		type = V(); 
		t = sc->Scanner(l);
		if (t == TIdent)
		{
			type = sm->SemGetType(l);
			return type;
		}
		if (t != TCBracket)
			sc->PrintError(_Char("ожидался символ )"), l);
		return type;
	}
	t = sc->Scanner(l);   // для определения H нужно иметь first2
	sc->PutUK(uk1, cl1);       // восстанавливается uk начальное 
	if (t == TOBracket)
	{
		type = B();
	}
}

DataType TDiagram::resolveType(TypeLex l) {
	// проверка на ключевое слово: 
	for (int j = 0; j < MAX_KEYW; j++)
		if (strcmp(l, Keyword[j]) == 0)
			if (strcmp(Keyword[j],dType[TYPE_INTEGER]) == 0)
				return TYPE_INTEGER;
			else if (strcmp(Keyword[j],dType[TYPE_SHORT]) == 0)
				return TYPE_SHORT;
			else if (strcmp(Keyword[j],dType[TYPE_LONG])==0)
				return TYPE_LONG;
			else if (strcmp(Keyword[j],dType[TYPE_LLONG])==0)
				return TYPE_LLONG;
		else
			sc->PrintError(_Char("ожидался ввод типа переменной"), l);
}