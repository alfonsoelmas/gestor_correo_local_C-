//===========================================
//Módulo ListaRegistros
//===========================================


#ifndef mListaRegistros
#define mListaRegistros

#include <string>
#include <fstream>
using namespace std;
//-------------------------------------------
//Tipos y Constantes
const int MAXREGISTROS=10;
const int CENTINELA_POS=-1;
struct tRegistro { string CorreoId; bool Leido; };
struct tListaRegistros	
{ 
	tRegistro *Registros; 
	int Cnt; 
	int TAM_MAX;
};

//-------------------------------------------
//Prototipos de funciones

//-------------------------------------------
//Inicializa la lista.
void LstReg_Inicializar(tListaRegistros &ListaReg);

//-------------------------------------------
//Dado un flujo de archivo de entrada (ya abierto), lee los datos que corresponden a
//una lista de registros y la devuelve.
void LstReg_Cargar(tListaRegistros &ListaReg, ifstream &Archivo);

//-------------------------------------------
//Dado un flujo de archivo de salida (ya abierto), guarda los datos de
//la lista de registro.
void LstReg_Guardar(const tListaRegistros ListaReg, ofstream &Archivo);

//-------------------------------------------
//Dado un registro lo inserta al final de la lista. Si la lista está llena devuelve
//false, en otro caso devuelve true. Este subprograma se ejecutará cuando un
//usuario envíe un correo ya que se insertará el registro correspondiente en la
//lista de registros que representa su bandeja de salida, y también en las listas de
//registros que representan las bandejas de entrada de cada uno de los
//destinatarios del correo.
bool LstReg_Insertar(tListaRegistros &ListaReg, tRegistro Registro);

//-------------------------------------------
//Dado un identificador de correo, busca el registro correspondiente y si lo encuentra lo
//elimina de la lista (¡sin dejar huecos!). Si no lo encuentra, devuelve false, en
//otro caso devuelve true. Este subprograma representa la acción de un usuario
//del borrado de un determinado correo de una de sus bandejas. OJO: esta
//operación sólo supone que el registro es borrado de la lista de registros, pero el
//correo seguirá existiendo en la lista de correos.
bool LstReg_Borrar(tListaRegistros &ListaReg, string Id);

//-------------------------------------------
//Dado un identificador de correo, busca el correspondiente registro y pone el indicador de
//leído a true. La operación devuelve un booleano indicando si se encontró o no el registro.
bool LstReg_CorreoLeido(tListaRegistros &ListaReg, string Id);

//-------------------------------------------
//Dado un identificador de correo y la lista, devuelve, sidicho identificador existe en la
//lista, su posición, y si no existe devuelve ­‐1.
int LstReg_Buscar(const tListaRegistros &ListaReg, string Id);

//-------------------------------------------
//Aumenta el tamaño de la LstReg
//nuevaCapacidad  =  (viejaCapacidad  *  3)  /  2  +  1
void LstReg_aumentar (tListaRegistros &ListaReg, const int nuevoTam);
#endif