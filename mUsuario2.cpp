//===========================================
//Módulo Usuario
//===========================================

#include "mUsuario.h"
#include <iostream>
#include <conio.h>
//-------------------------------------------
//Dado un flujo de archivo (ya abierto), se carga un usuario de fichero.
bool Usuario_Cargar(tUsuario &Usuario, ifstream &Archivo)
{
	bool carga=false;
	string compruebo;
	if(Archivo.is_open() && (Archivo >> compruebo) && compruebo!="XXX")
	{
		Usuario.Nombre=compruebo;
		Archivo >> Usuario.Contrasenia;
		Archivo >> Usuario.Recibidos.Cnt;
		for(int i=0; i<Usuario.Recibidos.Cnt; i++)
		{
			Archivo >> Usuario.Recibidos.Registros[i].CorreoId;
			Archivo >> Usuario.Recibidos.Registros[i].Leido;
		}

		Archivo >> Usuario.Enviados.Cnt;
		for(int i=0; i<Usuario.Enviados.Cnt; i++)
		{
			Archivo >> Usuario.Enviados.Registros[i].CorreoId;
			Archivo >> Usuario.Enviados.Registros[i].Leido;
		}
		carga=true;
	}
	return carga;
}

//-------------------------------------------
//Dado un flujo de archivo (ya abierto), se guarda un usuario en fichero.
void Usuario_Guardar(const tUsuario &Usuario, ofstream &Archivo)
{
	Archivo << Usuario.Nombre			<< endl;
	Archivo << Usuario.Contrasenia		<< endl;
	Archivo << Usuario.Recibidos.Cnt	<< endl;
	for(int i=0; i<Usuario.Recibidos.Cnt; i++)
	{
		Archivo << Usuario.Recibidos.Registros[i].CorreoId << " ";
		Archivo << Usuario.Recibidos.Registros[i].Leido;
	}
	Archivo << Usuario.Enviados.Cnt	<< endl;
	for(int i=0; i<Usuario.Enviados.Cnt; i++)
	{
		Archivo << Usuario.Enviados.Registros[i].CorreoId << " ";
		Archivo << Usuario.Enviados.Registros[i].Leido;
	}
	Archivo << endl;
}

//-------------------------------------------
//Recibe un identificador de usuario y una contraseña y los asigna al usuario.
void Usuario_Inicializar(tUsuario &Usuario, string Id, string Contrasenia)
{
	Usuario.Contrasenia=Contrasenia;
	Usuario.Nombre=Id;
	Usuario.Enviados.Cnt=0;
	Usuario.Recibidos.Cnt=0;
}

//-------------------------------------------
//Recibe una contraseña y un usuario y devuelve si la contraseña es correcta o no.
bool Usuario_ValidarContrasenia(const tUsuario &Usuario, string Contrasenia)
{
	bool correcta=false;
	if(Usuario.Contrasenia==Contrasenia) correcta=true;
	return correcta;
}

bool operator >	(tUsuario usuarioIzq, tUsuario usuarioDer)	{ return (usuarioIzq.Nombre > usuarioDer.Nombre); }
bool operator >=(tUsuario usuarioIzq, tUsuario usuarioDer)	{ return (usuarioIzq.Nombre > usuarioDer.Nombre); }
bool operator <=(tUsuario usuarioIzq, tUsuario usuarioDer)	{ return (usuarioIzq.Nombre > usuarioDer.Nombre); }
bool operator <	(tUsuario usuarioIzq, tUsuario usuarioDer)	{ return (usuarioIzq.Nombre > usuarioDer.Nombre); }