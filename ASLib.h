#ifndef ASLib
#define ASLib

//#include <atlstr.h>
//#include <ctime>
//#include <fstream>
//#include <math.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string>
//#include <time.h>

#include <conio.h>		//_getch
#include <iomanip>		//setfill, setw
#include <iostream>		//cout
#include <sstream>		//operator <<
#include <windows.h>

using namespace std;

// =============================================================================
// Teclas VIRTUAL_KEY_CODE
// =============================================================================
#define BACKSPACE	8
#define TAB			9
#define INTRO		13
#define ESC			27
#define CINCO		1012		//Tecla central del teclado numérico
#define SHIFT		1016
#define CTRL		1017
#define ALT			1018
#define PAUSA		1019
#define MAYUS		1020
#define REPAG		1033
#define AVPAG		1034
#define FIN			1035
#define INICIO		1036
#define CURLEFT  	1037
#define CURUP		1038
#define CURRIGHT	1039
#define CURDOWN  	1040
#define INSERT		1045
#define SUPR		1046
#define BLQNUM  	1144
#define BLQDSPL		1145
#define C_INTRO		10
#define C_BACKSPACE	127
#define C_TAB		1009
#define C_C			3
#define C_V			22
#define C_X			24

#define F01			1112
#define F02			1113
#define F03			1114
#define F04			1115
#define F05			1116
#define F06			1117
#define F07			1118
#define F08			1119
#define F09			1120
#define F10			1121
#define F11			1122
#define F12			1123

// =============================================================================
// Colores
// =============================================================================
#define NEGRO		0
#define AZUL		1
#define VERDE		2
#define CYAN		3
#define GRANATE		4
#define MORADO		5
#define MARRON		6
#define BLANCO		7
#define GRIS		8
#define AZULB		9
#define VERDEB		10
#define CYANB		11
#define ROJO		12
#define LILA		13
#define AMARILLO	14
#define BLANCOB		15

// =============================================================================
// Fechas
// =============================================================================
#define Lunes		1
#define Martes		2
#define Miercoles	3
#define Jueves		4
#define Viernes		5
#define Sabado		6
#define Domingo		7

#define Enero		1
#define Febrero		2
#define Marzo		3
#define Abril		4
#define Mayo		5
#define Junio		6
#define Julio		7
#define Agosto		8
#define Septiembre	9
#define Octubre		10
#define Noviembre	11
#define Diciembre	12

// =============================================================================
// Captura
// =============================================================================
#define V_INTRO			1
#define V_TAB			2
#define V_CURDOWN		4
#define V_AVPAG			8
#define V_CTRLINTRO		16
#define V_ESCAPE		32
#define V_CTRLTAB		64
#define V_CURUP			128
#define V_REPAG			256
#define V_AUTOSKIP		512
#define DERECHA			0
#define	IZQUIERDA		1
#define CENTRO			2
#define PUNTO			0
#define COMA			1

// =============================================================================
// Varios
// =============================================================================
#define MAX_OP_MH		29		//Máximo de opciones en MenuUpDoun
#define NOCURSOR		0
#define SOLIDCURSOR		1
#define NORMALCURSOR	2
#define DS_D_d_M_d_A	4	//Miercoles, 31 de Agosto de 1960
#define HH_MM_SS		1	//22:12:33

// =============================================================================
// Tipos de Datos y constantes
// =============================================================================
struct rCaja	{ int Fila; int Columna; int Largo; int Alto; int Tinta; int Papel; int Marco; int Fondo; };
struct rReloj	{ int Fila; int Columna; int Tinta; int Papel; int Visible; };
struct rMenu	{ int Fila; int Columna; int TintaNormal; int PapelNormal; int Fondo; int TintaOpcion; int PapelOpcion; };
struct rBoton	{ int Fila; int Columna; int Largo; int Tinta; int Papel; int Fondo; int Sombra; char Texto[80]; };
struct rRaton	{ int Fila; int	Columna; int IBoton; int DBoton; int CBoton; int TBoton; int DClick; int Rueda; }; //Rueda 1, 0, -1
struct rAtrDat	{ string Dat; int Lon; int Sde; char Car; };
struct rAtrPnt	{ int Fil; int Col; int Tnt; int Ppl; };
struct rAtrCpt	{ int Tnt; int Ppl; int Tnx; int Vld; string Cnj; string Msk; };					//Msk = Mascara = "[·]", "·", ""
struct rAtrBox	{ int Fil; int Col; int Lrg; int Alt; int Tnt; int Ppl; CHAR_INFO Buf[80*50]; };
struct rCpyPst	{ string Cpy; string Pst; bool Permitido; };
struct rFec		{ int Ano; int Mes; int Dia; };
struct rHor		{ int Hor; int Min; int Seg; };
struct rCoor	{ int Fil; int Col; };

// =============================================================================
// Prototipos
// =============================================================================
string	LlenaDer	(string &Dato, int TotLon, char Pad);			// Llena un string por la derecha a una longitud con un caracter
string	LlenaIzq	(string &Dato, int TotLon, char Pad);			// Llena un string por la izquierda a una longitud con un caracter
string	LlenaCen	(string &Dato, int TotLon, char Pad);			// Llena un string centrandolo a una longitud con un caracter
string	Limpia		(string &Dato);									// Limpia un string de blncos por ambos lados
void	PintaChr	(int Fila, int Columna, int Tinta, int Papel, char Car);	// Pinta un caracter en una coordenada con Tinta/Papel
void	PintaTxt	(int Fila, int Columna, int Tinta, int Papel, string Txt);	// Pinta un texto en una coordenada con Tinta/Papel
void	BorraTxt	(int Fila, int Columna, int Papel, int Lon);	// Borra un trozo de fila con Tinta/Papel

void	Pita		(int Veces);						// Funcion Pita
void	Locate		(int Fila, int Columna);			// Posiciona el Cursor en una Fila/Columna
void	CLS			(int Color);						// Borra la pantalla con un color determinado
void	Color		(int Tinta, int Papel);				// Activa un color de tinta y uno de Papel
void	Cursor		(int TipoCursor);					// Activa un tamaño de cursor
																	// TipoCursor = NOCURSOR	= 0	Cursor invisible
																	// TipoCursor = SOLIDCURSOR	= 1	Cursor grande
																	// TipoCursor = NORMALCURSOR= 2	Cursor normal
																	// TipoCursor = ??			= ?	Cursor fino
void	Delay		(double MiliSegundos);				// Delay en milisegundos
void	PintaCaja	(rCaja Caja, string Titulo);						// Pinta una caja
void	PintaMalla	(rCaja Caja, int Horizontal, int Vertical);			// Pinta una malla
void	GetCaja		(int Fila, int Columna, int Largo, int Alto, CHAR_INFO Buffer[80*50]);
void	PutCaja		(int Fila, int Columna, int Largo, int Alto, CHAR_INFO Buffer[80*50]);
void	BoxGet		(rAtrBox &MyBox);									// Lee un Box de la pantalla
void	BoxPut		(rAtrBox MyBox);									// Pinta un Box Leido
void	BoxDel		(rAtrBox MyBox);									// Borra un Box con tinta/papel desde fila/columna hasta largo/alto
void	BoxCpy		(rAtrBox BoxOrig, rAtrBox &BoxDest);				// Copia un Box en otro
void	BoxMov		(rAtrBox MyBox, rCoor Origen, int Modo);			// Mueve un Box desde Origen(fila/columna) a su posicion.
int		MenuUpDown	(rMenu MyMenuUpDown, int Opcion, string Menu[]);	// Menu Vertical
int		MenuFila	(rMenu MyMenuFila, int Opcion, string Menu[]);		// Manu Horizontal
void	Espera		();															// Muestra mensaje en filas 23-25 y Espera pulsar tecla o click raton
int		PintaBoton	(int NumBoton, rBoton MyBoton1, rBoton MyBoton2);			// Boton
int		PintaMensaje(int Tipo, string Texto[]);									// Pinta un Mensaje y espera a pulsar el boton
int		GetTxt		(rAtrDat &Dato, rAtrPnt Pnt, rAtrCpt Cpt, rCpyPst CpyPst);	// Captura un texto. Retorna la validacion
int		GetFecha	(rFec &Fecha, rAtrPnt Pnt, rAtrCpt Cpt, rCpyPst CpyPst);	// Retorna el tipo de validación INTRO, ESC...
int		GetHora		(rHor &Hora, rAtrPnt Pnt, rAtrCpt Cpt, rCpyPst CpyPst);		// Retorna el tipo de validación INTRO, ESC...
void	EsperaEvento(int &Tecla, rRaton &MyRaton);								// Funcion EsperaEvento
int		RatonInCaja	(int Fila, int Columna, int Largo, int Alto, rRaton MyRaton);	// Raton en Caja
int		MatSel		(string Tabla[], rCaja CajaFondo, int &Elemento, int ES);		//Seleccion en Array
int		SortMat		(string MatStr[]);

#endif