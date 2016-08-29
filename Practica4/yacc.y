%{
/*********************************************************
**
** Fichero: PRUEBA.Y
** Función: Pruebas de YACC para practicas de PL
**
********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/**
***
	La siguiente declaracion permite que ’yyerror’ se pueda invocar desde el
	fuente de lex
**/
void yyerror( const char * msg ) ;
/**
	La siguiente variable se usará para conocer el numero de la línea
	que se esta leyendo en cada momento. También es posible usar la variable
	’yylineno’ que también nos muestra la línea actual. Para ello es necesario
	invocar a flex con la opción ’-l’ (compatibilidad con lex).
**/

int linea_actual = 1 ;
int linea_control;

typedef enum
{
	marca,
	marca_condicional,
	funcion,
	variable,
	parametro_formal
} tipoEntrada;

typedef enum
{
	entero,
	real,
	caracter,
	booleano,	
	lista,
	desconocido,
	no_asignado
} dtipo;

typedef struct
{
	tipoEntrada entrada;
	char *nombre;
	dtipo tipoDato;
	dtipo tipoDatoLista;
	unsigned int parametros;
} entradaTS;

#define MAX_TS 500
unsigned int TOPE = -1, topeF; /* Tope de la pila */
unsigned int subProg = 0, decVar = 0; /* Indicador de comienzo de bloque de un subprog */
unsigned int func = 0, posParam = 0;
char idFuncion[100];
entradaTS TS[MAX_TS]; /* Pila de la tabla de símbolos */
dtipo tipoTMP, tipoListaTMP;


typedef struct
{
	int atrib; /* Atributo del símbolo (si tiene) */
	char *lexema; /* Nombre del lexema */
	dtipo tipo; /* Tipo del símbolo */
	dtipo tipoLista; /* Si tipo es lista, tipo de la lista */
} atributos;

#define YYSTYPE atributos /* A partir de ahora, cada símbolo tiene */
													/* una estructura de tipo atributos */

/* Lista de funciones y procedimientos para manejo de la TS */

void TS_InsertaMARCA()
{
	TOPE++;
	if(subProg == 1)
	{
		TS[TOPE].entrada = marca;
		TS[TOPE].nombre = "MARCA_INICIAL";
		int topeTMP = TOPE;
		while(TS[topeTMP].entrada != funcion && topeTMP > 0)
		{
			if(TS[topeTMP].entrada == parametro_formal)
			{	
				TOPE++;
				TS[TOPE].entrada = variable;
				TS[TOPE].nombre = TS[topeTMP].nombre;
				TS[TOPE].tipoDato = TS[topeTMP].tipoDato;
				if(TS[TOPE].tipoDato == lista)
					TS[TOPE].tipoDatoLista = TS[topeTMP].tipoDatoLista;
			}
			topeTMP--;
		}
	}
	else
	{
		TS[TOPE].entrada = marca_condicional;
		TS[TOPE].nombre = "MARCA_CONDICIONAL";
	}
}

void TS_VaciarENTRADAS()
{	
	while(TS[TOPE].entrada != marca && TS[TOPE].entrada != marca_condicional)
	{
		TOPE--;
	}
	TOPE--;
}

int existeVar(atributos a)
{	
	int topeTMP = TOPE ;
	while(topeTMP >= 0)
	{	
		//printf("%s %s\n", TS[topeTMP].nombre, a.lexema);
		if(TS[topeTMP].entrada == variable && strcmp(TS[topeTMP].nombre, a.lexema) == 0)
		{
			return 1;
		}
		topeTMP--;
	}
	return 0;
}

int existeID(atributos a)
{	
	int topeTMP = TOPE ;
	while(TS[topeTMP].entrada != marca && TS[topeTMP].entrada != marca_condicional && topeTMP >= 0)
	{	
		if(strcmp(TS[topeTMP].nombre, a.lexema) == 0)
		{
			return 1;
		}
		topeTMP--;
	}
	return 0;
}

void TS_InsertaIDENT(atributos a)
{
	//printf("%d\n", a.tipo);
	if(existeID(a))
		fprintf(stderr,"[Linea %d]: %s: existe.\n",linea_actual,a.lexema);
	else
	{
		TOPE++;	
		TS[TOPE].entrada = variable;
		TS[TOPE].nombre = a.lexema;
		TS[TOPE].tipoDato = tipoTMP;
		if(tipoTMP == lista)
			TS[TOPE].tipoDatoLista = tipoListaTMP;
	}
	//printf("ID: %s TIPO: %d\n", a.lexema, tipoTMP);
}

void TS_InsertaSUBPROG(atributos t, atributos a)
{
	TOPE++;
	TS[TOPE].entrada = funcion;
	TS[TOPE].nombre = a.lexema;
	TS[TOPE].parametros = 0;
	TS[TOPE].tipoDato = t.tipo;
}

void TS_InsertaPARAMF(atributos a)
{
		TOPE++;	
		TS[TOPE].entrada = parametro_formal;
		TS[TOPE].nombre = a.lexema;
		TS[TOPE].tipoDato = a.tipo;
		if(a.tipo == lista)
			TS[TOPE].tipoDatoLista = a.tipoLista;

		int topeTMP = TOPE;

		while(TS[topeTMP].entrada != funcion && topeTMP >= 0)
		{	
			topeTMP--;
		}
		TS[topeTMP].parametros++;
}

void comprobarExprLogica(atributos a)
{
	if(a.tipo != booleano)
    fprintf(stderr,"[Linea %d]: no hay expresion tipo logica.\n",linea_control);
}

void comprobarExprLista(atributos a)
{
	if(a.tipo != lista)
    fprintf(stderr,"[Linea %d]: el argumento no es de tipo lista.\n",linea_actual);
}

unsigned int asignaTipo(atributos a)
{
	int topeTMP = TOPE ;
	int existe = 0;
	unsigned int tipo = desconocido;
	//printf("tipo=%d",a.tipo);	
	if(existeVar(a))
	{
	while( existe == 0 && topeTMP>=0)
	{	
		if(!strcmp(TS[topeTMP].nombre,a.lexema) )
		{
			existe = 1;
			tipo = TS[topeTMP].tipoDato;
		}
		topeTMP--;
	}
	//printf("%s %d %d\n", a.lexema, a.tipo, tipo);
	}
	else
		fprintf(stderr,"[Linea %d]: %s: no definida.\n",linea_actual,a.lexema);	
	return tipo;
}

unsigned int asignaTipoLista(atributos a)
{
	int topeTMP = TOPE ;
	int existe = 0;
	unsigned int tipo = desconocido;
	if(existeVar(a))
	{
	while( existe == 0 && topeTMP>=0)
	{	
		if(!strcmp(TS[topeTMP].nombre,a.lexema) )
		{
			existe = 1;
			tipo = TS[topeTMP].tipoDatoLista;
		}
		topeTMP--;
	}
	}
	return tipo;
}

unsigned int comprobarTipoASSIGN(atributos a, atributos op, atributos b)
{
	//printf("[Linea %d] %s %d | %s %d\n", linea_actual, a.lexema, asignaTipo(a), b.lexema, b.tipo);
	unsigned int tipo = desconocido;
	int error = 0;	
	if(existeVar(a) && b.tipo != desconocido)
	{
		tipo = asignaTipo(a);
		if(tipo != b.tipo)
		{
			if((tipo == real || tipo == entero) && (b.tipo == real || b.tipo == entero))
			{
				error = 0;
			}
			else
				error = 1;
		}		
		else if(tipo == lista)
		{
			unsigned int tipoLista = asignaTipoLista(a);
			if(tipoLista != b.tipoLista)
				error = 1;
		}
	}
	
	if(error && b.tipo != desconocido)
		fprintf(stderr,"[Linea %d]: tipos incompatibles al asignar.\n",linea_actual);

	return tipo;
}

unsigned int real_entero2(dtipo t1, dtipo t2)
{
	if((t1 == real || t1 == entero) && (t2 == real || t2 == entero))
		return 1;
	return 0;
}

unsigned int real_entero(dtipo t)
{
	if(t == real || t == entero)
		return 1;
	return 0;
}

unsigned int comprobarTipoBIN(atributos a, atributos op, atributos b)
{
	//printf("[Linea %d] %s %d | %s %d\n", linea_actual, a.lexema, asignaTipo(a), b.lexema, b.tipo);
	unsigned int tipo = op.tipo;
	int error = 1;	

	switch(op.atrib)
	{
		case 0: // -
		case 3: // /		
			if(a.tipo == lista && real_entero(b.tipo))
				error = 0; 
			else
				error = !real_entero2(a.tipo, b.tipo);
		break;

		case 1: // +
		case 2: // *
			if(a.tipo == lista && real_entero(b.tipo))
			{				
				error = 0;
				tipo = lista;
			}
			else if(real_entero(a.tipo) && b.tipo == lista)
			{
				error = 0;
				tipo = entero;
			}
			else
				error = !real_entero2(a.tipo, b.tipo);			
		break;

		case 4: // &
		case 5: // |
		case 6: // ^
		case 7: // >
		case 8: // <
		case 9: // >=
		case 10: // <=
				error = !real_entero2(a.tipo, b.tipo);
		break;
		
		case 13: // ||
		case 14: // &&
			if(a.tipo == booleano && b.tipo == booleano)
				error = 0;
		break;

		case 15: // %
		case 16: // --
		case 18: // @
		
			if(a.tipo == lista && b.tipo == entero)
				error = 0;		
		break;	

		case 19: // ++
			if(a.tipo == lista && (a.tipoLista == b.tipo || real_entero2(a.tipoLista, b.tipo)))
				error = 0;
		break;

		case 17: // **
			if(a.tipo == lista && b.tipo == lista)
				error = 0;
		break;

		case 11: // ==
		case 12: // !=
		default:		
			if(a.tipo != b.tipo)
				{
					error = !real_entero2(a.tipo, b.tipo);
				}	
			else
				error = 0;
	}
	if(error && a.tipo != desconocido && b.tipo != desconocido)
		fprintf(stderr,"[Linea %d]: tipos incompatibles al operar.\n",linea_actual);

	return tipo;
}

unsigned int comprobarTipoUNIT(atributos op, atributos a)
{
	//printf("[Linea %d] %s %d | %s %d\n", linea_actual, a.lexema, a.tipo, op.lexema, op.tipo);
	unsigned int tipo = 0;
	int error = 1;	

	switch(op.atrib)
	{
		case 0: // ~a
			if(a.tipo == real || a.tipo == entero)
			{
				tipo = a.tipo;
				error = 0;
			}		
		break;

		case 1: // !a
			if(a.tipo == booleano)
			{			
				tipo = booleano;
				error = 0;
			}
		break;
		
		case 2: // #l
			if(a.tipo == lista)
			{
				tipo = entero;
				error = 0;
			}
		break;

		case 3: // ?l
			if(a.tipo == lista)
			{
				tipo = a.tipoLista;
				error = 0;
			}
		break;
	}
	
	if(error && a.tipo != desconocido)
		fprintf(stderr,"[Linea %d]: tipo incompatible en la operacion sobre.\n ",linea_actual);

	return tipo;
}

unsigned int existeFuncionID(char *id)
{	
	int topeTMP = TOPE, existe=0;
	
	while(topeTMP >= 0 && existe == 0)
	{	
		if(TS[topeTMP].entrada == funcion && !strcmp(id, TS[topeTMP].nombre) )
			existe=1;
		else
			topeTMP--;
	}
	if(existe)
		topeF = topeTMP;
	return existe;
}

void existeFuncion(atributos a)
{	
	if(!existeFuncionID(a.lexema))
		fprintf(stderr,"[Linea %d]: %s: no existe o fuera de ambito.\n",linea_actual,a.lexema);
}

unsigned int asignaTipoFuncion(char *id)
{
	unsigned int tipo = desconocido;
	//printf("%s, %d\n", TS[topeF].nombre, TS[topeF].tipoDato);
	if(existeFuncionID(id))
		tipo = TS[topeF].tipoDato;
	return tipo;
}

void verificaNumPar(unsigned int num)
{
	int topeTMP = topeF;
	if(existeFuncionID(idFuncion))
	{
		if(TS[topeTMP].parametros != num)
		{
			fprintf(stderr,"[Linea %d]: %s: numero de  parametros incorrecto.\n",linea_actual,TS[topeTMP].nombre);
		}
	}
}

void verificaParam(atributos a,unsigned int pos)
{
	int topeTMP = topeF;

	if(existeFuncionID(idFuncion))
	{
		if(pos <= TS[topeTMP].parametros)
		{
			if(TS[topeTMP+pos].tipoDato == real && a.tipo == entero)
			{
				a.tipo = real;
			}
			//printf("param: %d: ,tipo = %d. lexema:%s, tipo= %d \n",pos,TP[topeTMP+pos].tipoDato,a.lexema,a.tipo);
			if(TS[topeTMP].parametros==0)
			{
				fprintf(stderr,"[Linea %d]: %s: no tiene parametros.\n",linea_actual,TS[topeTMP].nombre);
			}
			else if(TS[topeTMP+pos].tipoDato != a.tipo)
			{
					fprintf(stderr,"[Linea %d]: tipo del parametro %d incompatible.\n",linea_actual,pos);
			}
		}
	}
}
/* Fin de funciones y procedimientos para manejo de la TS */
%}

/**
Para uso de mensajes de error sintáctico con BISON.
La siguiente declaración provoca que ’bison’, ante un error sintáctico,
visualice mensajes de error con indicación de los tokens que se esperaban
en el lugar en el que produjo el error (SÓLO FUNCIONA CON BISON=2.1).
Para Bison2.1 es mediante
 YYERROR_VERBOSE
En caso de usar mensajes de error mediante ’mes’ y ’mes2’ (ver apéndice D)
nada de lo anterior debe tenerse en cuenta.

**/
%error-verbose
/**
A continuación declaramos los nombres simbólicos de los tokens.
byacc se encarga de asociar a cada uno un código
**/
%token MAIN
%token IF
%token ELSE
%token DO
%token UNTIL

%left OP_BIN
%right OP_LIST
%left OP_LIST_START_CURSOR
%right OP_UNIT

%token TYPE
%token BEGIN_LIST
%token END_LIST
%token ASSIGN
%token PL
%token PR
%token BEGIN_P
%token END
%token COMMA
%token SEMICOLON
%token INPUT
%token OUTPUT
%token START_VAR_DEC
%token END_VAR_DEC
%token LIST
%token ID
%token RETURN
%token WHILE
%token BOOL
%token INT
%token FLOAT
%token CHAR
%token CAD


%%
/**
Sección de producciones que definen la gramática.
**/

/**
 * Práctica 3: Definición del lenguaje: BBAAD
 * Grupo 7
 * Francisco Javier Moreno Vega
 * Alberto Quesada Aranda
 * 18/12/2014
 */

/**
 * Definiciones básicas.
 */

program : header_program block;

block : start_block { TS_InsertaMARCA(); } local_var_dec sub_progs sentences end_block { TS_VaciarENTRADAS(); };

sub_progs : sub_progs sub_prog | ;

sub_prog : header_subprogram {subProg = 1;} block {subProg = 0;} ;

local_var_dec : START_VAR_DEC { decVar = 1; } local_var { decVar = 0; } END_VAR_DEC | ;

header_program : type MAIN PL PR;

start_block : BEGIN_P;

end_block : END;

local_var : local_var var_body | var_body;

var_body : type {tipoTMP = $1.tipo; tipoListaTMP = $1.tipoLista;} list_id SEMICOLON | error;

list_id : list_id COMMA ID {if(decVar)TS_InsertaIDENT($3);}| 
			ID {if(decVar)TS_InsertaIDENT($1);}| 
			error;

header_subprogram : type ID PL {TS_InsertaSUBPROG($1, $2);} parameters PR | type ID PL PR {TS_InsertaSUBPROG($1, $2);};

parameters : parameters COMMA type ID {TS_InsertaPARAMF($4);} | type ID {TS_InsertaPARAMF($2);} | error;

sentences : sentences sentence | sentence;

sentence : block |
sentence_assign |
sentence_if_then_else |
sentence_while |
sentence_input |
sentence_output |
sentence_return |
sentence_do_until |
sentence_list_forward_back |
sentence_list_start_cursor |
error;

sentence_assign : ID ASSIGN expr SEMICOLON {$1.tipo = asignaTipo($1); $1.tipoLista = asignaTipoLista($1); strcpy($1.lexema,$1.lexema); $$.tipo = comprobarTipoASSIGN($1,$2,$3);};

sentence_if_then_else : IF PL expr PR sentence {comprobarExprLogica($3);}
| IF PL expr PR sentence ELSE sentence {comprobarExprLogica($3);};

sentence_while : WHILE PL expr PR sentence {comprobarExprLogica($3);};

sentence_input : INPUT CAD COMMA list_id SEMICOLON | INPUT list_id SEMICOLON;

sentence_output : OUTPUT list_expr_cad SEMICOLON;

sentence_return : RETURN expr SEMICOLON;

sentence_do_until : DO sentence UNTIL PL expr PR SEMICOLON {comprobarExprLogica($5);};

sentence_list_forward_back : expr OP_LIST SEMICOLON {comprobarExprLista($1);};

sentence_list_start_cursor : OP_LIST_START_CURSOR expr SEMICOLON {comprobarExprLista($2);};

expr : PL expr PR {$$ = $2;} |
ID {$$.tipo = asignaTipo($1); $$.tipoLista = asignaTipoLista($1); strcpy($$.lexema,$1.lexema);} |
const {$$.tipo = $1.tipo; if($$.tipo == lista)$$.tipoLista = $1.tipoLista; }|
function_call |
OP_UNIT expr { $$.tipo = comprobarTipoUNIT($1, $2);}|
expr OP_BIN expr {$$.tipo = comprobarTipoBIN($1, $2, $3);}|
error;

function_call : ID PL {strcpy(idFuncion,$1.lexema); func=1; existeFuncion($1); } list_expr PR {func=0; verificaNumPar(posParam); posParam=0;$$.tipo = asignaTipoFuncion(idFuncion);} | 
							ID PL {strcpy(idFuncion,$1.lexema); func=1; existeFuncion($1);} PR {func=0; verificaNumPar(posParam); posParam=0;$$.tipo = asignaTipoFuncion(idFuncion);};

list_expr : list_expr COMMA expr {if(func){	posParam++; verificaParam($3, posParam);}} | 
						expr {if(func){ posParam++; verificaParam($1,posParam);}} ;

type : TYPE{$$.tipo = $1.tipo;} | LIST TYPE {$$.tipo = lista; $$.tipoLista = $2.tipo;};

const : INT |
FLOAT |
CHAR |
BOOL |
const_list {$$.tipo = lista; $$.tipoLista = $1.tipoLista;} ;

const_list : list_of_int {$$.tipoLista = entero;}|
list_of_float {$$.tipoLista = real;}|
list_of_char {$$.tipoLista = caracter;}|
list_of_bool {$$.tipoLista = booleano;};

list_of_int : BEGIN_LIST list_int END_LIST;
list_int : list_int COMMA INT | INT;

list_of_float : BEGIN_LIST list_float END_LIST;
list_float : list_float COMMA FLOAT | FLOAT;

list_of_char : BEGIN_LIST list_char END_LIST;
list_char : list_char COMMA CHAR | CHAR;

list_of_bool : BEGIN_LIST list_bool END_LIST;
list_bool : list_bool COMMA BOOL | BOOL;

list_expr_cad : list_expr_cad COMMA expr_cad | expr_cad;

expr_cad : expr | CAD;
%%
/** aqui incluimos el fichero generado por el ’lex’
*** que implementa la función ’yylex’
**/
#ifdef DOSWINDOWS
#include "../generated/lexyy.c"
#else
#include "../generated/lex.yy.c"
#endif
/**
Variable de entorno que indica la plataforma
se debe implementar la función yyerror. En este caso
simplemente escribimos el mensaje de error en pantalla
**/

void yyerror( const char *msg )
{
	fprintf(stderr,"[Linea %d]: %s\n", linea_actual, msg) ;
}



