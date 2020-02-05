// Scanner.cpp : ���� ���� �������� ������� "main". ����� ���������� � ������������� ���������� ���������.
//


#include "pch.h"
#include<locale.h> 
#include <iostream>
#include <string.h> 
#include "Def.h" 
#include "Scanner.h"
#include "Diagram.h"
#include "Semant.h"

using namespace std;

char* Char(string text) {
	char *Serr = new char[text.size() + 1];
	strcpy(Serr, text.c_str());
	return Serr;
}

int main()
{
	setlocale(LC_ALL, "RUS");
	TScanner *sc, *sc2;
	int type; 
	TypeLex l;

	sc = new TScanner(Char("input.txt"));// ���� �� ��������� 
	do {
		type = sc->Scanner(l);
		cout << l << " - ��� " << type << endl;
	} while (type != TEnd);
	
	sc2 = new TScanner(Char("input2.txt"));
	TSemant *sm = new TSemant(sc2);
	TDiagram *dg = new TDiagram(sc2, sm); 

	dg->S();
		type = sc2->Scanner(l);
	if (type == TEnd)
		cout << "�������������� ������ �� ����������." << endl;
	else	
		sc2->PrintError(Char("������ ����� � ����� ���������."), Char(""));
	dg->PrintTree();

	return 0;
}