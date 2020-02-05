#pragma once
#ifndef __def
#define __def
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT 10000   // максимально возможная длина текста
#define MAX_LEX    100   // максимально возможная длина лексемы
#define MAX_KEYW     8   // число ключевых слов

typedef char TypeLex[MAX_LEX];
typedef char TypeMod[MAX_TEXT];

//
#define TDo 1
#define TWhile 2
#define TInt 3
#define TSInt 4
#define TLInt 5
#define TLLInt 6
#define TReturn 7
#define TMain 8

#define TIdent 10
#define TConsIntDec 20
#define TConsIntHex 21

#define TDot  30
#define TComma  31
#define TSemicolon  32
#define TOBracket 33
#define TCBracket 34
#define TOBrace 35
#define TCBrace 36

#define TPlus 40
#define TMinus 41
#define TMult 42
#define TDiv 43
#define TMod 44
#define TSave 45
#define TLess 46
#define TMore 47
#define TEQLess 48
#define TEQMore 49
#define TEQ 50
#define TNEQ 51

#define TErr 200
#define TEnd 100

static TypeLex Keyword[MAX_KEYW] = { "do", "while", "int", "short int",
							  "long int", "long long int", "return","main"};

static int	IndexKeyword[MAX_KEYW] = { TDo,	TWhile,	TInt, TSInt,
							   TLInt,TLLInt,TReturn,TMain};

#endif // !__def


