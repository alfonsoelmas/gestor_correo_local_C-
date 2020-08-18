#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
//===========================================
//Módulo ListaUsuarios
//===========================================

#include "mListaUsuarios.h"
#include <iostream>
#include <conio.h>	
#include "ASLib.h"
using namespace std;
//-------------------------------------------
//Inicializa la lista.
void LstUsuario_Inicializar(tListaUsuarios &Usuarios)
{
	for(int i=0; i<MAXUSUARIOS; i++) Usuarios.Usuario[i] = new tUsuario;
	Usuarios.Cnt=0;
}
//-------------------------------------------
//Implementa la carga de la lista de usuarios desde el fichero de usuarios de
//nombre <NombreDominio>-usuarios.txt 
bool LstUsuario_Cargar(tListaUsuarios &Usuarios,  string Dominio)
{
	bool		comp	=false;
	string		sArchivo=Dominio+"_usuarios.txt";
	ifstream	Archivo;
	
	LstUsuario_Inicializar(Usuarios);		//Inicializacion

	Archivo.open(sArchivo);
	if(Archivo.is_open())
	{
		while(Usuario_Cargar(*Usuarios.Usuario[Usuarios.Cnt], Archivo)) Usuarios.Cnt++;
		comp=true;
	}
	Archivo.close();
	return comp;
}

//-------------------------------------------
//Implementa el guardado de la lista de usuarios en el fichero de usuarios de
//nombre <NombreDominio>-usuarios.txt
void LstUsuario_Guardar(const tListaUsuarios Usuarios, string Dominio)
{
	string		sArchivo=Dominio + TXT_USUARIO;
	ofstream	Archivo;
	string		dato;

	Archivo.open(sArchivo);
	for(int i=0; i<Usuarios.Cnt; i++) Usuario_Guardar(*Usuarios.Usuario[i],Archivo);
	Archivo << "XXX";
	Archivo.close();
}

//-------------------------------------------
//Añade un usuario en la posición de la lista que le corresponde, si hay sitio para
//ello. Además devuelve un booleano indicando si la operación tuvo éxito o no.

bool LstUsuario_Aniadir(tListaUsuarios &Usuarios, const tUsuario &Usuario)
{
	bool comp=false;
	if(Usuarios.Cnt < MAXUSUARIOS)
	{
		int Pos=0;
		if(!LstUsuario_BuscarUsuario(Usuarios, Usuario.Nombre, Pos))	//Compruebo si ya existe(Si existe devuelvo false y no inserto)
		{
			for(int i=Usuarios.Cnt; i>Pos; i--)	*Usuarios.Usuario[i]=*Usuarios.Usuario[i-1];

			*(Usuarios.Usuario[Pos]) = Usuario;

			Usuarios.Cnt++;
			comp=true;
		}
	}
	return comp;
}


//-------------------------------------------
//Dado un identificador de usuario y la lista, devuelve, si dicho
//identificador existe en la lista, su posición y el valor true, y si no existe en la
//lista, la posición que le correspondería y el valor false.
bool LstUsuario_BuscarUsuario(const tListaUsuarios &Usuarios, string id, int &Posicion)
{
	int		ini=0, fin=Usuarios.Cnt-1;
	bool	encontrado=false;  

	while(!encontrado && ini<=fin)
	{
		int mitad=(ini+fin)/2;						//Se supone ordenado. Busqueda dicotomica.
		if(id==Usuarios.Usuario[mitad]->Nombre)
		{
			encontrado=true;
			Posicion=mitad;
		}
		else if(id<Usuarios.Usuario[mitad]->Nombre)
		{
			fin=mitad-1;
			Posicion=fin+1;
		}
		else
		{
			ini=mitad+1;
			Posicion=ini;
		}
	}
	
	return encontrado;
}

//-------------------------------------------------------------------------
//Ordenar lista usuarios por id:
void LstUsuarios_Ordenar_ID(tListaUsuarios &Usuarios)
{
	/*METODO DE ORDENACIÓN POR INSERCIÓN*/
	for(int i=0; i<Usuarios.Cnt; i++)
	{
		int pos=i;
		string aux;

		while(pos>0 && Usuarios.Usuario[pos-1]->Nombre>Usuarios.Usuario[pos]->Nombre)
		{
			//INTERCAMBIO
			aux=Usuarios.Usuario[pos-1]->Nombre;
			Usuarios.Usuario[pos-1]->Nombre=Usuarios.Usuario[pos]->Nombre;
			Usuarios.Usuario[pos]->Nombre=aux;
			pos--;
		}
	}
}