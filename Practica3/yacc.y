%{

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void yyerror( const char * msg ) ;

int linea_actual = 1 ;
%}

%error-verbose

%token MAIN
%token IF
%token ELSE
%token SWITCH
%token CASE
%token BREAK
%left OP_OR
%left OP_AND
%left OP_EQ
%left OP_REL
%left OP_ADD
%left OP_MUL
%right OP_UNARY
%token TYPE
%token BEGIN_LIST
%token END_LIST
%token ASSIGN
%token PL
%token PR
%token BEGIN_P
%token END_P
%token COMMA
%token SEMICOLON
%token TWOPOINTS
%token INPUT
%token OUTPUT
%token START_VAR_DEC
%token END_VAR_DEC
%token ARRAY
%token ID
%token BOOL
%token RETURN
%token WHILE
%token INT
%token FLOAT
%token CHAR
%token CAD

%%

program : header_program block;

block : start_block local_var_dec sub_progs sentences end_block |
				start_block local_var_dec sub_progs end_block;

sub_progs : sub_progs sub_prog | ;

sub_prog : header_subprogram block;

local_var_dec : START_VAR_DEC local_var END_VAR_DEC | ;

header_program : type MAIN PL PR;

start_block : BEGIN_P;

end_block : END_P;

local_var : local_var var_body | var_body;

var_body : type array_id SEMICOLON | error;

array_id : ID BEGIN_LIST expr COMMA expr END_LIST | ID BEGIN_LIST expr END_LIST | array_id COMMA ID | ID | error;

header_subprogram : type ID PL parameters PR | type ID PL PR;

parameters : parameters COMMA type ID | type ID | error;

sentences : sentences sentence | sentence;

sentence : block |
sentence_assign |
sentence_if_then_else |
sentence_while |
sentence_input |
sentence_output |
sentence_return |
sentence_switch |
error;

sentence_assign : ID ASSIGN expr SEMICOLON;

sentence_if_then_else : IF PL expr PR sentence | IF PL expr PR sentence ELSE sentence;

sentence_while : WHILE PL expr PR sentence;

sentence_input : INPUT CAD COMMA array_id SEMICOLON | INPUT array_id SEMICOLON;

sentence_output : OUTPUT list_expr_string SEMICOLON;

sentence_return : RETURN expr SEMICOLON;

sentence_switch : SWITCH PL expr PR start_block switch_cases end_block;

switch_cases : CASE expr TWOPOINTS switch_block switch_cases
| CASE expr TWOPOINTS switch_block;

switch_block:	local_var_dec sentences BREAK SEMICOLON;

expr:
	PL expr PR									|
	OP_UNARY expr								|
	expr OP_MUL expr						|
	expr OP_ADD expr						|
	OP_ADD expr	%prec OP_UNARY	|
	expr OP_REL expr						|
	expr OP_EQ expr							|
	expr OP_OR expr							|
	expr OP_AND expr						|
	ID													|
	const												|
	function_call 							|
	error;

function_call : ID PL list_expr PR | ID PL PR;

list_expr : list_expr COMMA expr | expr;

type : TYPE | ARRAY TYPE;

const:
	int_constant			|
	float_constant		|
	boolean_constant	|
	char_constant			|
	array_constant		|
	string_constant;

int_constant: INT;

float_constant: FLOAT;

boolean_constant: BOOL;

char_constant: CHAR;

string_constant: CAD;

array_constant:
	BEGIN_P list_int_const END_P | BEGIN_P list_int_const SEMICOLON list_int_const END_P
	| BEGIN_P list_float_const END_P | BEGIN_P list_float_const SEMICOLON list_float_const END_P
	| BEGIN_P list_boolean_const END_P | BEGIN_P list_boolean_const SEMICOLON list_boolean_const END_P
	| BEGIN_P list_char_const END_P | BEGIN_P list_char_const SEMICOLON list_char_const END_P
	| BEGIN_P list_string_const END_P | BEGIN_P list_string_const SEMICOLON list_string_const END_P
	| BEGIN_P list_array_const END_P | BEGIN_P list_array_const SEMICOLON list_array_const END_P;

list_int_const: list_int_const COMMA int_constant | int_constant;

list_float_const: list_float_const COMMA float_constant | float_constant;

list_boolean_const: list_boolean_const COMMA boolean_constant | boolean_constant;

list_char_const: list_char_const COMMA char_constant | char_constant;

list_string_const: list_string_const COMMA string_constant | string_constant;

list_array_const: list_array_const COMMA array_constant | array_constant;

list_expr_string: list_expr_string COMMA expr_cad | expr_cad;

expr_cad: expr ;

%%

#include "./lex.yy.c"

void yyerror( const char *msg )
{
	fprintf(stderr,"[Linea %d]: %s\n", linea_actual, msg) ;
}
