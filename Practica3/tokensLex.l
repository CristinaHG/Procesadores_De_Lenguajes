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

"||"	{ return 	OP_OR;}
"&&"	{ return 	OP_AND;}
"=="	{ return 	OP_EQ;}
"!="	{ return 	OP_EQ;}
">"	{ return 	OP_REL;}
"<"	{ return 	OP_REL;}
">="	{ return 	OP_REL;}
"<="	{ return 	OP_REL;}
"-"	{ return 	OP_ADD;}
"+"	{ return 	OP_ADD;}
"*"	{ return 	OP_MUL;}
"/"	{ return 	OP_MUL;}
"%"	{ return 	OP_MUL;}
"--" { return OP_UNARY;}
"++"	{ return 	OP_UNARY;}

"inicio"	{ return 	MAIN;}
"entero"	{ return 	TYPE;}
"real"	{ return 	TYPE;}
"caracter"	{ return 	TYPE;}
"bool"	{ return 	TYPE;}
"arreglo de"	{ return 	ARRAY;}
"["	{ return 	BEGIN_LIST;}
"]"	{ return 	END_LIST;}
"="	{ return 	ASSIGN;}
"!"	{ return 	OP_UNARY;}
"("	{ return 	PL;}
")"	{ return 	PR;}
"{"	{ return 	BEGIN_P;}
"}"	{ return 	END_P;}
","	{ return 	COMMA;}
";"	{ return 	SEMICOLON;}
":"	{ return 	TWOPOINTS;}
"selecciona" {return SWITCH;}
"caso" {return CASE;}
"romper" {return BREAK;}
"si"	{ return 	IF;}
"sino"	{ return 	ELSE;}
"mientras"	{ return 	WHILE;}
"entrada"	{ return 	INPUT;}
"salida"	{ return 	OUTPUT;}
"verdadero"	{ return 	BOOL;}
"falso"	{ return 	BOOL;}
"devuelve"	{ return 	RETURN;}
"VARINI" { return START_VAR_DEC; }
"VARFIN" { return END_VAR_DEC; }
{digit}+	{ return  INT;}
{digit}+\.{digit}+	{ return  FLOAT;}
"\'"{symbol_char}"\'"	{ return  CHAR;}
"\""{symbol_cad}+"\""	{ return CAD;}
{letter}{alphanum}* { return  ID;}
{space}+ {}
{endline} {linea_actual++;}
. {snprintf(buffer, MAXLONBUFFER, "el(los) caracter(es) '%s' no forma(n) ningun token conocido",yytext);
yyerror(buffer);
}

%%
