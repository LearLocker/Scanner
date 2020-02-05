#pragma once
#ifndef Semant
#define Semant

#include <string.h>
#include <iostream>
#include "Def.h"

#define EMPTY -1
#define max(a,b) a<b? b : a

//static const char dType[][20] = { "unknow",  "int", "short int", "long int", "long long int", "block" };

//enum DataType {	TYPE_UNKNOWN, TYPE_INTEGER, TYPE_SHORT,	TYPE_LONG, TYPE_LLONG, TYPE_BLOCK };

static const char dType[][20] = { "unknow",  "int", "long long int", "block" };

enum DataType {	TYPE_UNKNOWN, TYPE_INTEGER, TYPE_LLONG, TYPE_BLOCK };


union DataValue
{
	int DataAsInt;
	long long DataAsLong;
};

class Node	// элемент семантической таблицы
{
public:
	Node *Up,       // родитель
		*Left,      // левый потомок
		*Right;  	// правый потомок

	TypeLex	id;						// идентификатор объекта 
	DataType dataType;				// тип значения
	bool FlagReturn;				// имеет ли оп return
	DataValue  dataValue;			// ссылка на значение константы или NULL 

	// ФУНКЦИИ ОБРАБОТКИ БИНАРНОГО ДЕРЕВА
	Node(Node *u, DataType dataT, TypeLex lex) {
		Up = u;
		memcpy(id, lex, max(strlen(id), strlen(lex)));
		//id = lex;
		dataType = dataT;
	}
	~Node() {}
};

class TSemant
{
public: TScanner *sc;
		 //Type *_type;
		 static Node *Cur;				// текущий элемент дерева
		 static Node *Root;
public:
	TSemant(TScanner *s);
	~TSemant() {}

	// ФУНКЦИИ ОБРАБОТКИ БИНАРНОГО ДЕРЕВА

	void SetLeft(DataType dataT, TypeLex lex);		// задать левого потомка
	void SetRight();		// задать правого потомка
	TypeLex* FindUp();		//
	Node *FindUpNode();		// найти родительский узел
	Node *Find(TypeLex lex);		// найти узел по заданной лексеме
	void ToUp();		// подняться к родительскому узлу

	// СЕМАНТИЧЕСКИЕ ПОДПРОГРАММЫ

	void SetVariable(DataType dataT, TypeLex lex);// занесение переменной a в таблицу с типом t
	void SetFunct(DataType dataT, TypeLex lex);// занесение функции a в таблицу с типом t
	void SetBlock();// занесение блока таблицу
	DataType SemGetType(TypeLex a);	// найти в таблице переменную с именем a
									// и вернуть ссылку на соответствующий элемент дерева
	DataType SemGetFunct(TypeLex a);	//	найти в таблице функцию с именем a
									//	и вернуть ссылку на соответствующий элемент дерева
	bool checkReturn();
	void assignReturn();
	bool checkCompability(DataType toType, DataType fromType);
	DataType Cast(DataType toType, DataType fromType);
	Node* DupControl(TypeLex a);		// проверка идентификатора а на повторное описание внутри блока
	void SemantPrintError(char  * err, char * a, int numstr);
	void print_tree(const Node* root);
	void print_subtree(const Node* node);

};

#endif // !Semant
