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

class Node	// ������� ������������� �������
{
public:
	Node *Up,       // ��������
		*Left,      // ����� �������
		*Right;  	// ������ �������

	TypeLex	id;						// ������������� ������� 
	DataType dataType;				// ��� ��������
	bool FlagReturn;				// ����� �� �� return
	DataValue  dataValue;			// ������ �� �������� ��������� ��� NULL 

	// ������� ��������� ��������� ������
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
		 static Node *Cur;				// ������� ������� ������
		 static Node *Root;
public:
	TSemant(TScanner *s);
	~TSemant() {}

	// ������� ��������� ��������� ������

	void SetLeft(DataType dataT, TypeLex lex);		// ������ ������ �������
	void SetRight();		// ������ ������� �������
	TypeLex* FindUp();		//
	Node *FindUpNode();		// ����� ������������ ����
	Node *Find(TypeLex lex);		// ����� ���� �� �������� �������
	void ToUp();		// ��������� � ������������� ����

	// ������������� ������������

	void SetVariable(DataType dataT, TypeLex lex);// ��������� ���������� a � ������� � ����� t
	void SetFunct(DataType dataT, TypeLex lex);// ��������� ������� a � ������� � ����� t
	void SetBlock();// ��������� ����� �������
	DataType SemGetType(TypeLex a);	// ����� � ������� ���������� � ������ a
									// � ������� ������ �� ��������������� ������� ������
	DataType SemGetFunct(TypeLex a);	//	����� � ������� ������� � ������ a
									//	� ������� ������ �� ��������������� ������� ������
	bool checkReturn();
	void assignReturn();
	bool checkCompability(DataType toType, DataType fromType);
	DataType Cast(DataType toType, DataType fromType);
	Node* DupControl(TypeLex a);		// �������� �������������� � �� ��������� �������� ������ �����
	void SemantPrintError(char  * err, char * a, int numstr);
	void print_tree(const Node* root);
	void print_subtree(const Node* node);

};

#endif // !Semant
