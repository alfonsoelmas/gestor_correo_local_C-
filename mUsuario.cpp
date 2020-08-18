//===========================================
//Módulo Usuario
//===========================================

#include "mUsuario.h"
#include "ASLib.h"
using namespace std;

//#include <conio.h>
//-------------------------------------------
//Dado un flujo de archivo (ya abierto), se carga un usuario de fichero.
bool Usuario_Cargar(tUsuario &Usuario, ifstream &Archivo)
{
	bool carga=false;
	string compruebo="";
	
	if(Archivo.is_open())
	{
		Archivo >> compruebo;
		//cout << "      compruebo: " << compruebo << endl;
		if (compruebo!="XXX")
		{
			Usuario.Nombre=compruebo;
			Archivo >> Usuario.Contrasenia;			//???
			Archivo >> Usuario.Recibidos.Cnt;
			//cout << " Password: " << Usuario.Contrasenia << endl; 
			//cout << "Recibidos: " << Usuario.Recibidos.Cnt << endl;
			LstReg_Cargar(Usuario.Recibidos, Archivo);
			Archivo >> Usuario.Enviados.Cnt;
			LstReg_Cargar(Usuario.Enviados, Archivo);

			carga=true;
		}
	}
	return carga;
}

//-------------------------------------------
//Dado un flujo de archivo (ya abierto), se guarda un usuario en fichero.
void Usuario_Guardar(const tUsuario &Usuario, ofstream &Archivo)
{
	Archivo << Usuario.Nombre			<< endl;
	Archivo << Usuario.Contrasenia		<< endl;
	
	LstReg_Guardar(Usuario.Recibidos, Archivo);
	LstReg_Guardar(Usuario.Enviados, Archivo);

}

//-------------------------------------------
//Recibe un identificador de usuario y una contraseña y los asigna al usuario.
void Usuario_Inicializar(tUsuario &Usuario, string Id, string Contrasenia)
{
	Usuario.Contrasenia=Contrasenia;
	Usuario.Nombre=Id;
	
	LstReg_Inicializar(Usuario.Enviados);
	LstReg_Inicializar(Usuario.Recibidos);

	//Usuario.Enviados.Cnt=0;
	//Usuario.Recibidos.Cnt=0;
}

//-------------------------------------------
//Recibe una contraseña y un usuario y devuelve si la contraseña es correcta o no.
bool Usuario_ValidarContrasenia(const tUsuario &Usuario, string Contrasenia)
{
	bool correcta=false;
	if(Usuario.Contrasenia==Contrasenia) correcta=true;
	return correcta;
}

//------------------------------------------
bool operator >	(tUsuario usuarioIzq, tUsuario usuarioDer)	{ return (usuarioIzq.Nombre > usuarioDer.Nombre); }
bool operator >=(tUsuario usuarioIzq, tUsuario usuarioDer)	{ return (usuarioIzq.Nombre > usuarioDer.Nombre); }
bool operator <=(tUsuario usuarioIzq, tUsuario usuarioDer)	{ return (usuarioIzq.Nombre > usuarioDer.Nombre); }
bool operator <	(tUsuario usuarioIzq, tUsuario usuarioDer)	{ return (usuarioIzq.Nombre > usuarioDer.Nombre); }