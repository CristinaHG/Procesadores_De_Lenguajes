%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAXLONBUFFER 200
char buffer[MAXLONBUFFER];
%}

letter	[a-zA-Z]
alphanum [a-zA-Z0-9_]
digit	[0-9]
space	[ \t]
endline [\n]
symbol_char 	[^\']
symbol_cad		[^\"]

%option noyywrap

%%

"main"	{ return 	MAIN;}
"["	{ return 	BEGIN_LIST;}
"]"	{ return 	END_LIST;}
"="	{ yylval.atrib = ASSIGN; return ASSIGN;}	
"return"	{ return 	RETURN;}
"("	{ return 	PL;}
")"	{ return 	PR;}
"{"	{ return 	BEGIN_P;}
"}"	{ return 	END;}
","	{ return 	COMMA;}
";"	{ return 	SEMICOLON;}
"input"	{ return 	INPUT;}
"output"	{ return 	OUTPUT;}
"START_VAR" { return START_VAR_DEC; }
"END_VAR" { return END_VAR_DEC; }

"if"	{ linea_control = linea_actual; return 	IF;}
"else"	{ return 	ELSE;}
"while"	{ linea_control = linea_actual; return 	WHILE;}
"do"	{ return 	DO ;}
"until"	{ linea_control = linea_actual; return 	UNTIL;}

"int"	{ yylval.tipo = entero; return 	TYPE;}
"float"	{ yylval.tipo = real; return 	TYPE;}
"char"	{ yylval.tipo = caracter; return 	TYPE;}
"bool"	{ yylval.tipo = booleano; return 	TYPE;}
"list of"	{ yylval.tipo = lista; return 	LIST;}

"<<"	{yylval.atrib = 0; yylval.tipo = lista;  return OP_LIST;}
">>"	{yylval.atrib = 1;  yylval.tipo = lista; return OP_LIST;}
"$"	{return OP_LIST_START_CURSOR;}

"~"	{ yylval.atrib = 0; yylval.tipo = entero; return 	OP_UNIT;}
"!"	{ yylval.atrib = 1; yylval.tipo = booleano; return 	OP_UNIT;}
"#"	{ yylval.atrib = 2; yylval.tipo = lista; return 	OP_UNIT;}
"?"	{ yylval.atrib = 3; yylval.tipo = lista; return	OP_UNIT;}


"-"	{ yylval.atrib = 0; yylval.tipo = entero; return OP_BIN;}
"+"	{ yylval.atrib = 1; yylval.tipo = entero; return OP_BIN;}
"*"	{ yylval.atrib = 2; yylval.tipo = entero; return OP_BIN;}
"/"	{ yylval.atrib = 3; yylval.tipo = entero; return OP_BIN;}

"&"	{ yylval.atrib = 4; yylval.tipo = entero; return OP_BIN;}
"|"	{ yylval.atrib = 5; yylval.tipo = entero; return OP_BIN;}
"^"	{ yylval.atrib = 6; yylval.tipo = entero; return OP_BIN;}

">"	{ yylval.atrib = 7; yylval.tipo = booleano; return OP_BIN;}
"<"	{ yylval.atrib = 8; yylval.tipo = booleano; return OP_BIN;}
">="	{ yylval.atrib = 9; yylval.tipo = booleano; return 	OP_BIN;}
"<="	{ yylval.atrib = 10; yylval.tipo = booleano; return OP_BIN;}
"=="	{ yylval.atrib = 11; yylval.tipo = booleano; return OP_BIN;}
"!="	{ yylval.atrib = 12; yylval.tipo = booleano; return OP_BIN;}
"||"	{ yylval.atrib = 13; yylval.tipo = booleano; return	OP_BIN;}
"&&"	{ yylval.atrib = 14; yylval.tipo = booleano; return	OP_BIN;}

"%"	{ yylval.atrib = 15; yylval.tipo = lista; return 	OP_BIN;}
"--" { yylval.atrib = 16; yylval.tipo = lista; return OP_BIN;}
"**"	{ yylval.atrib = 17; yylval.tipo = lista; return 	OP_BIN;}

"++"	{ yylval.atrib = 19; yylval.tipo = lista; return 	OP_BIN;}
"@"	{ yylval.atrib = 18; yylval.tipo = lista; return 	OP_BIN;}


"true"	{ yylval.lexema = strdup (yytext); yylval.atrib = 1; yylval.tipo = booleano; return 	BOOL;}
"false"	{ yylval.lexema = strdup (yytext); yylval.atrib = 0; yylval.tipo = booleano; return 	BOOL;}
"-"?{digit}+	{ yylval.lexema = strdup (yytext); yylval.atrib = INT; yylval.tipo = entero; return  INT;}
"-"?{digit}+\.{digit}+	{ yylval.lexema = strdup (yytext); yylval.atrib = FLOAT; yylval.tipo = real; return  FLOAT;}
"\'"{symbol_char}"\'"	{ yylval.lexema = strdup (yytext); yylval.atrib = CHAR; yylval.tipo = caracter; return  CHAR;}
"\""{symbol_cad}+"\""	{ yylval.lexema = strdup (yytext); yylval.atrib = CAD; return CAD;}

{letter}{alphanum}* { yylval.lexema = strdup (yytext); return  ID;}
{space}+ {}
{endline} {linea_actual++;}
. {snprintf(buffer, MAXLONBUFFER, "el(los) caracter(es) '%s' no forma(n) ningun token conocido",yytext); yyerror(buffer);}

%%
