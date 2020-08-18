//===========================================
//M�dulo Gestor
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
//Inicializa el gestor e intenta arrancarlo cargando la informaci�n del dominio que se le pasa como
//par�metro. Para ello inicializar� y cargar� la lista de usuarios y de correos de
//dicho dominio. Si tiene �xito en todas las operaciones devuelve true y si alguna
//falla devuelve false.
bool GestorArrancar(tGestor &Gestor, string Dominio);

//-------------------------------------------
//Esta operaci�n apaga el gestor y guarda para ello las listas de usuarios y de correos del dominio que se
//encontrase activo en ese momento.
void GestorApagar(const tGestor &Gestor);

//-------------------------------------------
//Lee los datos de usuario necesarios para crear una cuenta (id y contrase�a) y si el id de usuario no existe y hay
//espacio en la lista de usuarios, crea la cuenta del usuario y registra al nuevo
//usuario como usuario activo en el gestor. Devuelve un booleano indicando si la
//operaci�n tuvo �xito.
bool GestorCrearCuenta(tGestor &Gestor);

//-------------------------------------------
//Lee los datos de usuario necesarios para validar la cuenta (id y contrase�a) y si el usuario existe y la
//contrase�a coincide, registra al usuario como usuario activo. Devuelve un
//booleano indicando si la operaci�n tuvo �xito.
bool GestorIniciarSesion(tGestor &Gestor);

//-------------------------------------------
//Solicita el correo que el usuario quiere leer (ser� el n�mero con el que el correo
//es mostrado por pantalla en la bandeja correspondiente), valida que existe y si
//es as�, lo marca como correo le�do. A continuaci�n, busca el correo en la lista de
//correos y si lo encuentra lo muestra en la pantalla de lectura del correo
//void GestorLeerCorreo(tGestor& Gestor, tListaRegistros &ListaReg);
void GestorLeerCorreo(tGestor& Gestor, string CorreoId, int iBandeja);

//-------------------------------------------
//Este subprograma implementa el env�o del correo en nuestra aplicaci�n. Para ello el
//correo recibido como par�metro es insertado en la lista de correo. Si ha habido
//�xito, entonces se inserta el registro correspondiente en la lista de registros
//enviados del emisor y se intenta insertar igualmente un registro de dicho correo
//en la lista de registros recibidos del destinatario del correo. Si el destinatario no
//existe o si su bandeja de entrada est� llena, entonces se mostrar� un mensaje de error.
void GestorEnviarCorreo(tGestor &Gestor, tCorreo &Correo);

//-------------------------------------------
//Este subprograma implementa el borrado del correo de una bandeja de un
//determinado usuario (OJO: el correo no se borra de la lista de correos). Para
//ello, solicita el correo que el usuario quiere borrar (ser� el n�mero con el que el
//correo es mostrado por pantalla en la bandeja correspondiente), valida que
//existe y si es as�, procede a borrarlo.
//void GestorBorrarCorreo(tGestor &Gestor, tListaRegistros &ListaReg);
void GestorBorrarCorreo(tGestor& Gestor, string CorreoId, int iBandeja);

//-------------------------------------------
//Este subprograma implementa la lectura r�pida de correos sin leer. El resultado
//es que muestra en una pantalla todos los correos sin leer ordenados por asunto
//(ignorando todos los �Re: �) y por fecha. Al finalizar su ejecuci�n los correos sin
//leer quedar�n marcados como le�dos.
void GestorLecturaRapida(string MatSort[]);

#endif
