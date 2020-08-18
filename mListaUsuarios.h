//===========================================
//M�dulo ListaUsuarios
//===========================================


#ifndef mListaUsuarios
#define mListaUsuarios

#include "mUsuario.h"
using namespace std;

//-------------------------------------------
//Tipos y Constantes

const int MAXUSUARIOS=50;
const string TXT_USUARIO="_usuarios.txt";

struct tListaUsuarios
{
	tUsuario *Usuario[MAXUSUARIOS];
	int Cnt;
};

//-------------------------------------------
//Prototipos de funciones

//-------------------------------------------
//Inicializa la lista.
void LstUsuario_Inicializar(tListaUsuarios &Usuarios);

//-------------------------------------------
//Implementa la carga de la lista de usuarios desde el fichero de usuarios de
//nombre <NombreDominio>-usuarios.txt 
bool LstUsuario_Cargar(tListaUsuarios &Usuarios, string Dominio);

//-------------------------------------------
//Implementa el guardado de la lista de usuarios en el fichero de usuarios de
//nombre <NombreDominio>-usuarios.txt
void LstUsuario_Guardar(const tListaUsuarios Usuarios, string Dominio);

//-------------------------------------------
//A�ade un usuario en la posici�n de la lista que le corresponde, si hay sitio para
//ello. Adem�s devuelve un booleano indicando si la operaci�n tuvo �xito o no.
bool LstUsuario_Aniadir(tListaUsuarios &Usuarios, const tUsuario &Usuario);

//-------------------------------------------
//Dado un identificador de usuario y la lista, devuelve, si dicho
//identificador existe en la lista, su posici�n y el valor true, y si no existe en la
//lista, la posici�n que le corresponder�a y el valor false.
bool LstUsuario_BuscarUsuario(const tListaUsuarios &Usuarios, string id, int &Posicion);

//Ordena listaUsuarios por ID
void LstUsuarios_Ordenar_ID(tListaUsuarios &Usuarios);

#endif