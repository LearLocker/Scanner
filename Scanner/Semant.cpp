#include "pch.h"
#include <string.h>
#include <iostream>
#include <iomanip>
#include "Def.h" 
#include "Scanner.h" 
#include "Semant.h"
#include <assert.h>
#include <list>

#define max(a,b) a<b? b : a

using namespace std;

char* __Char(string text) {
	char *Serr = new char[text.size() + 1];
	strcpy(Serr, text.c_str());
	return Serr;
}
TSemant::TSemant(TScanner *s) { sc = s; }

Node* TSemant::Root = new Node(NULL, TYPE_UNKNOWN, (char*)"");

Node* TSemant::Cur = Root;

// ФУНКЦИИ ОБРАБОТКИ БИНАРНОГО ДЕРЕВА

void TSemant::SetLeft(DataType dataT, TypeLex lex)
{
	Cur->Left = new Node(Cur, dataT, lex);
	Cur = Cur->Left;
}

void TSemant::SetRight()
{
	Cur->Right = new Node(Cur, TYPE_UNKNOWN, (char *)"");
	Cur = Cur->Right;
}

TypeLex* TSemant::FindUp()
{
	Node *node = FindUpNode();
	TypeLex* a = (TypeLex*)node->id;
	return a;
}

Node* TSemant::FindUpNode()
{
	Node* node = Cur;
	while (node->Right == NULL || node->dataType == TYPE_UNKNOWN || node->dataType == TYPE_BLOCK) {
		node = node->Up;
	}
	return node;
}

Node* TSemant::Find(TypeLex lex)
{
	Node *node = Cur;
	do {
		if (node->id != NULL && memcmp(lex, node->id, max(strlen(node->id), strlen(lex))) == 0)
			return node;
		node = node->Up;
	} while (node != NULL);
	return NULL;
}

void TSemant::ToUp()
{
	while (Cur->dataType != TYPE_UNKNOWN)
	{
		Cur = Cur->Up;
	}
	Cur = Cur->Up;
}


// СЕМАНТИЧЕСКИЕ ПОДПРОГРАММЫ

void TSemant::SetVariable(DataType dataT, TypeLex lex)
{
	Node *node = DupControl(lex);
	if (node != NULL)
		SemantPrintError(__Char("Повторное описание переменной "), lex, sc->GetCurrentLine());
	SetLeft(dataT, lex);
}

void TSemant::SetFunct(DataType dataT, TypeLex lex)
{
	Node *node = DupControl(lex);
	if (node != NULL)
		SemantPrintError(__Char("Повторное описание функции "), lex, sc->GetCurrentLine());
	SetLeft(dataT, lex);
	SetRight();
}

void TSemant::SetBlock()
{
	SetLeft(TYPE_BLOCK, (char*)"");
	SetRight();
}

DataType TSemant::SemGetType(TypeLex a)
{
	Node *node = Find(a);
	if (node == NULL)
		SemantPrintError(__Char("Отсутствует описание идентификатора "), a, sc->GetCurrentLine());
	if (node->Right != NULL) {
		SemantPrintError(__Char("Следующий идентификатор не является переменной "), a, sc->GetCurrentLine());
	}
	return node->dataType;
}

DataType TSemant::SemGetFunct(TypeLex a)
{
	Node *node = Find(a);
	if (node == NULL)
		SemantPrintError(__Char("Отсутствует описание идентификатора "), a, sc->GetCurrentLine());
	if (node->Right == NULL) {
		SemantPrintError(__Char("Следующий идентификатор не является функцеий "), a, sc->GetCurrentLine());
	}
	return node->dataType;
}

void TSemant::assignReturn() {
	FindUpNode()->FlagReturn = true;
}


bool TSemant::checkReturn() {
	return FindUpNode()->FlagReturn;
}

bool TSemant::checkCompability(DataType toType, DataType fromType)
{
	DataType type = Cast(toType, fromType);
	return type != TYPE_UNKNOWN;
}

DataType TSemant::Cast(DataType toType, DataType fromType)
{
	if (toType == fromType) {
		return toType;
	}
	if (toType == TYPE_SHORT && fromType == TYPE_INTEGER || fromType == TYPE_SHORT && toType == TYPE_INTEGER) {
		return TYPE_INTEGER;
	}
	if (toType == TYPE_LONG && (fromType == TYPE_INTEGER || fromType == TYPE_SHORT)|| fromType == TYPE_LONG && (toType == TYPE_INTEGER || toType == TYPE_SHORT)) {
		return TYPE_LONG;
	}
	if (toType == TYPE_LLONG && (fromType == TYPE_INTEGER || fromType == TYPE_SHORT || fromType == TYPE_LONG) || fromType == TYPE_LLONG && (toType == TYPE_LONG || toType == TYPE_INTEGER || toType == TYPE_SHORT)) {
		return TYPE_LLONG;
	}

	return TYPE_UNKNOWN;
}

Node* TSemant::DupControl(TypeLex a)
{
	Node *node = Cur;
	while (memcmp(node->id, "", max(strlen(node->id), strlen(""))) != 0)
	{
		cout << node->id << endl;
		if (memcmp(a, node->id, max(strlen(node->id), strlen(a))) == 0)
			return node;
		node = node->Up;
	}
	return NULL;
}

void TSemant::SemantPrintError(char  * err, char * a, int numstr) {
	// выдать сообщение об ошибке 
	if (a[0] == '\0')
		cout << "Ошибка : " << err << " " << a << endl;
	else
		cout << "Ошибка : " << err << ". Неверный символ " << a << " в строке " << numstr << endl;
	exit(0);
}

struct node_print_state_t {
	node_print_state_t* child_state;
	bool printing_last_child;
};

node_print_state_t* _root_state = NULL;

void TSemant::print_tree(const Node* root) {
	assert(_root_state == NULL);
	try {
		if (root != NULL) {
			print_subtree(root);
		}
	}
	catch (...) {
		// если что-то пошло не так, принудительно reset-им состояние
		_root_state = NULL;
		throw;
	}
}

void TSemant::print_subtree(const Node* node) {
	node_print_state_t* parent_state;
	if (_root_state != NULL) {
		printf(" ");
		node_print_state_t* s = _root_state;
		while (s->child_state != NULL) {
			printf(s->printing_last_child ? "  " : "| ");
			s = s->child_state;
		}
		parent_state = s;
		printf(parent_state->printing_last_child ? "L" : "+");
	}
	else {
		parent_state = NULL;
	}
	printf(">%s\n", node->id);

	if ((node->Left != NULL) || (node->Right != NULL)) { // если есть дети
		node_print_state_t s;
		if (parent_state != NULL) {
			parent_state->child_state = &s;
		}
		else {
			_root_state = &s;
		}
		s.child_state = NULL;

		// печатаем детей
		if (node->Left != NULL) {
			s.printing_last_child = (node->Right == NULL);
			print_subtree(node->Left);
		}
		if (node->Right != NULL) {
			s.printing_last_child = true;
			print_subtree(node->Right);
		}

		if (parent_state != NULL) {
			parent_state->child_state = NULL;
		}
		else {
			_root_state = NULL;
		}
	}
}