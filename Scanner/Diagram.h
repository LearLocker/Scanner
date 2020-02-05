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
	void S(); // ������� ���������
	void D(); // �������� ����������
	void F(); // �������� �������
	void Z(); // ������ ����������
	DataType V(); // ���������
	void Q(); // ��������� ��������
	void K(); // ��������� � ��������
	void O(); // ��������
	DataType B(); // ����� �������
	void R(); // Return
	void P(); // ������������
	void W(); // Do{}while()
	DataType A(); // ���������
	DataType H(); // ���������
	DataType E(); // ������������ ���������

	void PrintTree();
	DataType resolveType(TypeLex l);
};

#endif // !__DIAGRAM
