//-------------------------------------------
//===========================================
//M�dulo ListaCorreos
//===========================================


#ifndef mListaCorreos
#define mListaCorreos

#include "mCorreo.h"
using namespace std;
//-------------------------------------------
//Tipos y constantes

const int MAXCORREOS=10;
const string NOMBRE_TXT="_correos.txt";

struct tListaCorreos
{
	tCorreo *Correo;
	int Cnt;
	int TAM_MAX;
};

//-------------------------------------------
//Prptotipos de funciones

//-------------------------------------------
//Inicializa la lista.
void LstCorreo_Inicializar(tListaCorreos &Correos);

//-------------------------------------------
//Implementa la carga de la lista de correos desde el fichero de correos de nombre
//<NombreDominio>-correos.txt.
bool LstCorreo_Cargar(tListaCorreos &Correos, string Dominio);

//-------------------------------------------
//Implementa el guardado de la lista de correos en el fichero de correos de
//nombre <NombreDominio>- correos.txt . Guarda en fichero la lista de correos.
//El nombre del fichero ser� como en el subprograma anterior.
void LstCorreo_Guardar(const tListaCorreos &Correos, string Dominio);

//-------------------------------------------
//Dado un correo, si hay espacio en la lista, lo coloca en la posici�n de la lista que
//le corresponda de acuerdo con su identificador y devuelve true. Si no lo ha
//podido colocar devuelve false.
bool LstCorreo_Insertar(tListaCorreos &Correos, const tCorreo &Correo);

//-------------------------------------------
//Dado un identificador de correo y la lista, devuelve, si dicho identificador existe
//en la lista, su posici�n y el valor true, y si no existe en la lista, la posici�n que le
//corresponder�a y el valor false.
bool LstCorreo_Buscar(const tListaCorreos &Correos, string id, int &Pos);

//-------------------------------------------
//Dada una lista de correos la devuelve ordenada por asunto y fecha. Como es una clave de ordenaci�n doble,
//habr� que redefinir el operador de comparaci�n en el m�dulo que corresponda.
void LstCorreo_Ordenar_AF(tListaCorreos &Correos);

//La lista de correos estar� siempre ordenada por ID para trabajar con ella.
void LstCorreo_Ordenar_ID(tListaCorreos &Correos);

//-------------------------------------------
//Aumenta el tama�o de la LstReg
//nuevaCapacidad  =  (viejaCapacidad  *  3)  /  2  +  1
void LstCorreo_aumentar (tListaCorreos &Correos, const int nuevoTam);

#endif