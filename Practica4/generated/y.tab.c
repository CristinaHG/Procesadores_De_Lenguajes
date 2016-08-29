/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "yacc.y" /* yacc.c:339  */

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

#line 554 "generated/y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    MAIN = 258,
    IF = 259,
    ELSE = 260,
    DO = 261,
    UNTIL = 262,
    OP_BIN = 263,
    OP_LIST = 264,
    OP_LIST_START_CURSOR = 265,
    OP_UNIT = 266,
    TYPE = 267,
    BEGIN_LIST = 268,
    END_LIST = 269,
    ASSIGN = 270,
    PL = 271,
    PR = 272,
    BEGIN_P = 273,
    END = 274,
    COMMA = 275,
    SEMICOLON = 276,
    INPUT = 277,
    OUTPUT = 278,
    START_VAR_DEC = 279,
    END_VAR_DEC = 280,
    LIST = 281,
    ID = 282,
    RETURN = 283,
    WHILE = 284,
    BOOL = 285,
    INT = 286,
    FLOAT = 287,
    CHAR = 288,
    CAD = 289
  };
#endif
/* Tokens.  */
#define MAIN 258
#define IF 259
#define ELSE 260
#define DO 261
#define UNTIL 262
#define OP_BIN 263
#define OP_LIST 264
#define OP_LIST_START_CURSOR 265
#define OP_UNIT 266
#define TYPE 267
#define BEGIN_LIST 268
#define END_LIST 269
#define ASSIGN 270
#define PL 271
#define PR 272
#define BEGIN_P 273
#define END 274
#define COMMA 275
#define SEMICOLON 276
#define INPUT 277
#define OUTPUT 278
#define START_VAR_DEC 279
#define END_VAR_DEC 280
#define LIST 281
#define ID 282
#define RETURN 283
#define WHILE 284
#define BOOL 285
#define INT 286
#define FLOAT 287
#define CHAR 288
#define CAD 289

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 670 "generated/y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   191

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  93
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  163

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   558,   558,   560,   560,   562,   562,   564,   564,   566,
     566,   566,   566,   568,   570,   572,   574,   574,   576,   576,
     576,   578,   579,   580,   582,   582,   582,   584,   584,   584,
     586,   586,   588,   589,   590,   591,   592,   593,   594,   595,
     596,   597,   598,   600,   602,   603,   605,   607,   607,   609,
     611,   613,   615,   617,   619,   620,   621,   622,   623,   624,
     625,   627,   627,   628,   628,   630,   631,   633,   633,   635,
     636,   637,   638,   639,   641,   642,   643,   644,   646,   647,
     647,   649,   650,   650,   652,   653,   653,   655,   656,   656,
     658,   658,   660,   660
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "MAIN", "IF", "ELSE", "DO", "UNTIL",
  "OP_BIN", "OP_LIST", "OP_LIST_START_CURSOR", "OP_UNIT", "TYPE",
  "BEGIN_LIST", "END_LIST", "ASSIGN", "PL", "PR", "BEGIN_P", "END",
  "COMMA", "SEMICOLON", "INPUT", "OUTPUT", "START_VAR_DEC", "END_VAR_DEC",
  "LIST", "ID", "RETURN", "WHILE", "BOOL", "INT", "FLOAT", "CHAR", "CAD",
  "$accept", "program", "block", "$@1", "sub_progs", "sub_prog", "$@2",
  "local_var_dec", "$@3", "$@4", "header_program", "start_block",
  "end_block", "local_var", "var_body", "$@5", "list_id",
  "header_subprogram", "$@6", "parameters", "sentences", "sentence",
  "sentence_assign", "sentence_if_then_else", "sentence_while",
  "sentence_input", "sentence_output", "sentence_return",
  "sentence_do_until", "sentence_list_forward_back",
  "sentence_list_start_cursor", "expr", "function_call", "$@7", "$@8",
  "list_expr", "type", "const", "const_list", "list_of_int", "list_int",
  "list_of_float", "list_float", "list_of_char", "list_char",
  "list_of_bool", "list_bool", "list_expr_cad", "expr_cad", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289
};
# endif

#define YYPACT_NINF -62

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-62)))

#define YYTABLE_NINF -64

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       2,   -62,    10,    17,    50,    92,   -62,   -62,   -62,   -62,
     -62,    57,    78,    87,   -62,   -62,   -62,     9,    49,   -62,
      11,   -62,   -62,    48,    98,   143,   151,   151,    68,   151,
       5,    14,    93,   151,   105,   -62,   -62,   -62,   -62,   -62,
     -62,   -62,   109,   -62,   -62,   -62,   -62,   -62,   -62,   -62,
     -62,   -62,   -62,    79,   -62,   102,   -62,   -62,   -62,   -62,
     -62,   -62,   110,   -62,     7,   151,   138,   -62,   130,     8,
     -62,   -62,   -62,   -62,   -62,     6,    70,    71,    72,    23,
     -62,   -62,   128,    91,   -62,   142,    96,   -62,   151,   134,
      43,   151,    50,   -62,   -62,   -62,   151,   137,   139,   -62,
     103,    25,   144,   -62,   -62,   132,   -62,   136,   -62,   146,
     -62,   147,   -62,     7,   153,   -62,    14,   -62,    62,   151,
     152,   -62,    41,   -62,   -62,   -62,   168,   -62,   143,   151,
     -62,   -62,   -62,   -62,   113,   -62,   -62,   -62,   142,    76,
     -62,   143,   -62,    12,   181,    46,   -62,   -62,   151,   -62,
     -62,    86,   160,   143,   167,   142,   -62,     2,   -62,   -62,
     -62,   162,   -62
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    67,     0,     0,     0,     0,    68,     1,    14,     2,
       3,     0,    12,     0,     9,     6,    13,     0,     0,    20,
       0,    17,    18,    42,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,    72,    69,    70,    71,    32,
       5,     7,     0,    31,    33,    34,    35,    36,    37,    38,
      39,    40,    41,     0,    57,     0,    56,    73,    74,    75,
      76,    77,     0,    16,     0,     0,     0,    60,    55,     0,
      58,    89,    80,    83,    86,     0,     0,     0,     0,     0,
      23,    22,     0,     0,    93,    92,     0,    91,     0,    61,
       0,     0,     0,    15,     4,    30,     0,     0,     0,    11,
       0,     0,     0,    53,    78,     0,    81,     0,    84,     0,
      87,     0,    54,     0,     0,    48,     0,    49,     0,     0,
       0,    50,     0,     8,    59,    52,    24,    19,     0,     0,
      79,    82,    85,    88,     0,    21,    90,    43,    66,     0,
      64,     0,    26,     0,    44,     0,    47,    62,     0,    46,
      29,     0,     0,     0,     0,    65,    25,     0,    28,    45,
      51,     0,    27
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -62,   -62,    -3,   -62,   -62,   -62,   -62,   -62,   -62,   -62,
     -62,   -62,   -62,   -62,   170,   -62,   -61,   -62,   -62,   -62,
     -62,   -23,   -62,   -62,   -62,   -62,   -62,   -62,   -62,   -62,
     -62,   -22,   -62,   -62,   -62,   -62,     0,   -62,   -62,   -62,
     -62,   -62,   -62,   -62,   -62,   -62,   -62,   -62,    75
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,    39,    12,    18,    40,    92,    15,    17,    62,
       4,    10,    94,    20,    21,    64,    83,    41,   143,   151,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,   119,   120,   139,    22,    56,    57,    58,
      75,    59,    76,    60,    77,    61,    78,    86,    87
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       5,     9,    66,   100,    69,    70,    80,    79,    80,    85,
      19,    90,    19,   150,     1,    67,    96,     7,    55,    95,
     104,     1,     6,     1,     1,    27,   105,    28,     2,   103,
      29,    96,    81,    96,    81,     2,   -10,     2,     2,    82,
     112,    68,   128,   101,    35,    36,    37,    38,    84,    96,
      23,    96,   134,    24,    96,    25,   -60,   -60,   141,    26,
      27,     1,    28,   154,   121,    29,   118,     8,     8,   122,
      96,    30,    31,    13,   124,     2,    32,    33,    34,    35,
      36,    37,    38,   137,   106,   108,   110,    96,    97,   123,
     107,   109,   111,   147,    85,    11,   148,   138,    71,    72,
      73,    74,    14,   156,    16,   144,   157,   145,    88,    89,
      23,   114,   115,    24,    65,    25,   116,   117,   149,    26,
      27,    91,    28,   114,   127,    29,   155,     8,    93,    98,
     159,    30,    31,   114,   146,    99,    32,    33,    34,    35,
      36,    37,    38,   152,    23,   102,    89,    24,   113,    25,
      96,   -63,    67,    26,    27,   126,    28,   161,   125,    29,
     129,     8,    27,   130,    28,    30,    31,    29,   131,   140,
      32,    33,    34,    35,    36,    37,    38,   133,    68,   132,
     135,    35,    36,    37,    38,   142,   153,   158,   160,   162,
      63,   136
};

static const yytype_uint8 yycheck[] =
{
       0,     4,    25,    64,    26,    27,     1,    29,     1,    31,
       1,    33,     1,     1,    12,     1,     8,     0,    18,    42,
      14,    12,    12,    12,    12,    11,    20,    13,    26,    21,
      16,     8,    27,     8,    27,    26,    25,    26,    26,    34,
      17,    27,    17,    65,    30,    31,    32,    33,    34,     8,
       1,     8,   113,     4,     8,     6,     8,     9,    17,    10,
      11,    12,    13,    17,    21,    16,    88,    18,    18,    91,
       8,    22,    23,    16,    96,    26,    27,    28,    29,    30,
      31,    32,    33,    21,    14,    14,    14,     8,     9,    92,
      20,    20,    20,    17,   116,     3,    20,   119,    30,    31,
      32,    33,    24,    17,    17,   128,    20,   129,    15,    16,
       1,    20,    21,     4,    16,     6,    20,    21,   141,    10,
      11,    16,    13,    20,    21,    16,   148,    18,    19,    27,
     153,    22,    23,    20,    21,    25,    27,    28,    29,    30,
      31,    32,    33,   143,     1,     7,    16,     4,    20,     6,
       8,    17,     1,    10,    11,    16,    13,   157,    21,    16,
      16,    18,    11,    31,    13,    22,    23,    16,    32,    17,
      27,    28,    29,    30,    31,    32,    33,    30,    27,    33,
      27,    30,    31,    32,    33,    17,     5,    27,    21,    27,
      20,   116
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    12,    26,    36,    45,    71,    12,     0,    18,    37,
      46,     3,    38,    16,    24,    42,    17,    43,    39,     1,
      48,    49,    71,     1,     4,     6,    10,    11,    13,    16,
      22,    23,    27,    28,    29,    30,    31,    32,    33,    37,
      40,    52,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    71,    72,    73,    74,    76,
      78,    80,    44,    49,    50,    16,    56,     1,    27,    66,
      66,    30,    31,    32,    33,    75,    77,    79,    81,    66,
       1,    27,    34,    51,    34,    66,    82,    83,    15,    16,
      66,    16,    41,    19,    47,    56,     8,     9,    27,    25,
      51,    66,     7,    21,    14,    20,    14,    20,    14,    20,
      14,    20,    17,    20,    20,    21,    20,    21,    66,    68,
      69,    21,    66,    37,    66,    21,    16,    21,    17,    16,
      31,    32,    33,    30,    51,    27,    83,    21,    66,    70,
      17,    17,    17,    53,    56,    66,    21,    17,    20,    56,
       1,    54,    71,     5,    17,    66,    17,    20,    27,    56,
      21,    71,    27
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    35,    36,    38,    37,    39,    39,    41,    40,    43,
      44,    42,    42,    45,    46,    47,    48,    48,    50,    49,
      49,    51,    51,    51,    53,    52,    52,    54,    54,    54,
      55,    55,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    57,    58,    58,    59,    60,    60,    61,
      62,    63,    64,    65,    66,    66,    66,    66,    66,    66,
      66,    68,    67,    69,    67,    70,    70,    71,    71,    72,
      72,    72,    72,    72,    73,    73,    73,    73,    74,    75,
      75,    76,    77,    77,    78,    79,    79,    80,    81,    81,
      82,    82,    83,    83
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     6,     2,     0,     0,     3,     0,
       0,     5,     0,     4,     1,     1,     2,     1,     0,     4,
       1,     3,     1,     1,     0,     6,     4,     4,     2,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     5,     7,     5,     5,     3,     3,
       3,     7,     3,     3,     3,     1,     1,     1,     2,     3,
       1,     0,     5,     0,     4,     3,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     1,     3,     3,     1,
       3,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 560 "yacc.y" /* yacc.c:1646  */
    { TS_InsertaMARCA(); }
#line 1876 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 560 "yacc.y" /* yacc.c:1646  */
    { TS_VaciarENTRADAS(); }
#line 1882 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 564 "yacc.y" /* yacc.c:1646  */
    {subProg = 1;}
#line 1888 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 564 "yacc.y" /* yacc.c:1646  */
    {subProg = 0;}
#line 1894 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 566 "yacc.y" /* yacc.c:1646  */
    { decVar = 1; }
#line 1900 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 566 "yacc.y" /* yacc.c:1646  */
    { decVar = 0; }
#line 1906 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 576 "yacc.y" /* yacc.c:1646  */
    {tipoTMP = (yyvsp[0]).tipo; tipoListaTMP = (yyvsp[0]).tipoLista;}
#line 1912 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 578 "yacc.y" /* yacc.c:1646  */
    {if(decVar)TS_InsertaIDENT((yyvsp[0]));}
#line 1918 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 579 "yacc.y" /* yacc.c:1646  */
    {if(decVar)TS_InsertaIDENT((yyvsp[0]));}
#line 1924 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 582 "yacc.y" /* yacc.c:1646  */
    {TS_InsertaSUBPROG((yyvsp[-2]), (yyvsp[-1]));}
#line 1930 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 582 "yacc.y" /* yacc.c:1646  */
    {TS_InsertaSUBPROG((yyvsp[-3]), (yyvsp[-2]));}
#line 1936 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 584 "yacc.y" /* yacc.c:1646  */
    {TS_InsertaPARAMF((yyvsp[0]));}
#line 1942 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 584 "yacc.y" /* yacc.c:1646  */
    {TS_InsertaPARAMF((yyvsp[0]));}
#line 1948 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 600 "yacc.y" /* yacc.c:1646  */
    {(yyvsp[-3]).tipo = asignaTipo((yyvsp[-3])); (yyvsp[-3]).tipoLista = asignaTipoLista((yyvsp[-3])); strcpy((yyvsp[-3]).lexema,(yyvsp[-3]).lexema); (yyval).tipo = comprobarTipoASSIGN((yyvsp[-3]),(yyvsp[-2]),(yyvsp[-1]));}
#line 1954 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 602 "yacc.y" /* yacc.c:1646  */
    {comprobarExprLogica((yyvsp[-2]));}
#line 1960 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 603 "yacc.y" /* yacc.c:1646  */
    {comprobarExprLogica((yyvsp[-4]));}
#line 1966 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 605 "yacc.y" /* yacc.c:1646  */
    {comprobarExprLogica((yyvsp[-2]));}
#line 1972 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 613 "yacc.y" /* yacc.c:1646  */
    {comprobarExprLogica((yyvsp[-2]));}
#line 1978 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 615 "yacc.y" /* yacc.c:1646  */
    {comprobarExprLista((yyvsp[-2]));}
#line 1984 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 617 "yacc.y" /* yacc.c:1646  */
    {comprobarExprLista((yyvsp[-1]));}
#line 1990 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 619 "yacc.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[-1]);}
#line 1996 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 620 "yacc.y" /* yacc.c:1646  */
    {(yyval).tipo = asignaTipo((yyvsp[0])); (yyval).tipoLista = asignaTipoLista((yyvsp[0])); strcpy((yyval).lexema,(yyvsp[0]).lexema);}
#line 2002 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 621 "yacc.y" /* yacc.c:1646  */
    {(yyval).tipo = (yyvsp[0]).tipo; if((yyval).tipo == lista)(yyval).tipoLista = (yyvsp[0]).tipoLista; }
#line 2008 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 623 "yacc.y" /* yacc.c:1646  */
    { (yyval).tipo = comprobarTipoUNIT((yyvsp[-1]), (yyvsp[0]));}
#line 2014 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 624 "yacc.y" /* yacc.c:1646  */
    {(yyval).tipo = comprobarTipoBIN((yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]));}
#line 2020 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 627 "yacc.y" /* yacc.c:1646  */
    {strcpy(idFuncion,(yyvsp[-1]).lexema); func=1; existeFuncion((yyvsp[-1])); }
#line 2026 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 627 "yacc.y" /* yacc.c:1646  */
    {func=0; verificaNumPar(posParam); posParam=0;(yyval).tipo = asignaTipoFuncion(idFuncion);}
#line 2032 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 628 "yacc.y" /* yacc.c:1646  */
    {strcpy(idFuncion,(yyvsp[-1]).lexema); func=1; existeFuncion((yyvsp[-1]));}
#line 2038 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 628 "yacc.y" /* yacc.c:1646  */
    {func=0; verificaNumPar(posParam); posParam=0;(yyval).tipo = asignaTipoFuncion(idFuncion);}
#line 2044 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 630 "yacc.y" /* yacc.c:1646  */
    {if(func){	posParam++; verificaParam((yyvsp[0]), posParam);}}
#line 2050 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 631 "yacc.y" /* yacc.c:1646  */
    {if(func){ posParam++; verificaParam((yyvsp[0]),posParam);}}
#line 2056 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 633 "yacc.y" /* yacc.c:1646  */
    {(yyval).tipo = (yyvsp[0]).tipo;}
#line 2062 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 633 "yacc.y" /* yacc.c:1646  */
    {(yyval).tipo = lista; (yyval).tipoLista = (yyvsp[0]).tipo;}
#line 2068 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 639 "yacc.y" /* yacc.c:1646  */
    {(yyval).tipo = lista; (yyval).tipoLista = (yyvsp[0]).tipoLista;}
#line 2074 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 641 "yacc.y" /* yacc.c:1646  */
    {(yyval).tipoLista = entero;}
#line 2080 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 642 "yacc.y" /* yacc.c:1646  */
    {(yyval).tipoLista = real;}
#line 2086 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 643 "yacc.y" /* yacc.c:1646  */
    {(yyval).tipoLista = caracter;}
#line 2092 "generated/y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 644 "yacc.y" /* yacc.c:1646  */
    {(yyval).tipoLista = booleano;}
#line 2098 "generated/y.tab.c" /* yacc.c:1646  */
    break;


#line 2102 "generated/y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 661 "yacc.y" /* yacc.c:1906  */

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



