//===========================================
//Módulo Usuario
//===========================================


#ifndef mUsuario
#define mUsuario

#include "mListaRegistros.h"

using namespace std;
//-------------------------------------------
//Tipos y Constantes

struct tUsuario
{
	string			Nombre;
	string			Contrasenia;
	tListaRegistros Recibidos;
	tListaRegistros Enviados;
};

//-------------------------------------------
//Prototipos de funciones

//-------------------------------------------
//Dado un flujo de archivo (ya abierto), se carga un usuario de fichero.
bool Usuario_Cargar(tUsuario &Usuario, ifstream &Archivo);

//-------------------------------------------
//Dado un flujo de archivo (ya abierto), se guarda un usuario en fichero.
void Usuario_Guardar(const tUsuario &Usuario, ofstream &Archivo);

//-------------------------------------------
//Recibe un identificador de usuario y una contraseña y los asigna al usuario.
void Usuario_Inicializar(tUsuario &Usuario, string Id, string Contrasenia);

//===========================================
//Recibe una contraseña y un usuario y devuelve si la contraseña es correcta o no.
bool Usuario_ValidarContrasenia(const tUsuario &Usuario, string Contrasenia);
//Operadores: 
bool operator>(	tUsuario usuarioIzq, tUsuario usuarioDer);
bool operator>=(tUsuario usuarioIzq, tUsuario usuarioDer);
bool operator<=(tUsuario usuarioIzq, tUsuario usuarioDer);
bool operator<(	tUsuario usuarioIzq, tUsuario usuarioDer);
#endif

