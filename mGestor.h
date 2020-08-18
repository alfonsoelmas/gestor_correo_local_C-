//===========================================
//Módulo Gestor
//===========================================

#ifndef mGestor
#define mGestor

#include "mListaCorreos.h"
#include "mListaUsuarios.h"

using namespace std;
//-------------------------------------------
//Tipos y Constantes
struct tGestor
{
	string			Dominio;
	tListaCorreos	LstCorreos;
	tListaUsuarios	LstUsuarios;
	int				UsuarioActivo;			// vale -1 si Sesion cerrada o no hay usuarios registrados
};
//const string	DOMINIO = "Camelot.net";
const string	DOMINIO = "ucm.net";
//const string	DOMINIO = "pepito.org";
//-------------------------------------------
//Prototipos de funciones

//-------------------------------------------
//Inicializa el gestor e intenta arrancarlo cargando la información del dominio que se le pasa como
//parámetro. Para ello inicializará y cargará la lista de usuarios y de correos de
//dicho dominio. Si tiene éxito en todas las operaciones devuelve true y si alguna
//falla devuelve false.
bool GestorArrancar(tGestor &Gestor, string Dominio);

//-------------------------------------------
//Esta operación apaga el gestor y guarda para ello las listas de usuarios y de correos del dominio que se
//encontrase activo en ese momento.
void GestorApagar(const tGestor &Gestor);

//-------------------------------------------
//Lee los datos de usuario necesarios para crear una cuenta (id y contraseña) y si el id de usuario no existe y hay
//espacio en la lista de usuarios, crea la cuenta del usuario y registra al nuevo
//usuario como usuario activo en el gestor. Devuelve un booleano indicando si la
//operación tuvo éxito.
bool GestorCrearCuenta(tGestor &Gestor);

//-------------------------------------------
//Lee los datos de usuario necesarios para validar la cuenta (id y contraseña) y si el usuario existe y la
//contraseña coincide, registra al usuario como usuario activo. Devuelve un
//booleano indicando si la operación tuvo éxito.
bool GestorIniciarSesion(tGestor &Gestor);

//-------------------------------------------
//Solicita el correo que el usuario quiere leer (será el número con el que el correo
//es mostrado por pantalla en la bandeja correspondiente), valida que existe y si
//es así, lo marca como correo leído. A continuación, busca el correo en la lista de
//correos y si lo encuentra lo muestra en la pantalla de lectura del correo
//void GestorLeerCorreo(tGestor& Gestor, tListaRegistros &ListaReg);
void GestorLeerCorreo(tGestor& Gestor, string CorreoId, int iBandeja);

//-------------------------------------------
//Este subprograma implementa el envío del correo en nuestra aplicación. Para ello el
//correo recibido como parámetro es insertado en la lista de correo. Si ha habido
//éxito, entonces se inserta el registro correspondiente en la lista de registros
//enviados del emisor y se intenta insertar igualmente un registro de dicho correo
//en la lista de registros recibidos del destinatario del correo. Si el destinatario no
//existe o si su bandeja de entrada está llena, entonces se mostrará un mensaje de error.
void GestorEnviarCorreo(tGestor &Gestor, tCorreo &Correo);

//-------------------------------------------
//Este subprograma implementa el borrado del correo de una bandeja de un
//determinado usuario (OJO: el correo no se borra de la lista de correos). Para
//ello, solicita el correo que el usuario quiere borrar (será el número con el que el
//correo es mostrado por pantalla en la bandeja correspondiente), valida que
//existe y si es así, procede a borrarlo.
//void GestorBorrarCorreo(tGestor &Gestor, tListaRegistros &ListaReg);
void GestorBorrarCorreo(tGestor& Gestor, string CorreoId, int iBandeja);

//-------------------------------------------
//Este subprograma implementa la lectura rápida de correos sin leer. El resultado
//es que muestra en una pantalla todos los correos sin leer ordenados por asunto
//(ignorando todos los “Re: ”) y por fecha. Al finalizar su ejecución los correos sin
//leer quedarán marcados como leídos.
void GestorLecturaRapida(string MatSort[]);

#endif
