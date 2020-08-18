//===========================================
//Módulo ListaCorreos
//===========================================

#include "mListaCorreos.h"
#include "ASLib.h"
using namespace std;
//-------------------------------------------
//Inicializa la lista.
void LstCorreo_Inicializar(tListaCorreos &Correos)
{
	Correos.Correo = new tCorreo [MAXCORREOS];
	Correos.TAM_MAX = MAXCORREOS;
	Correos.Cnt=0;
}

//-------------------------------------------
//Implementa la carga de la lista de correos desde el fichero de correos de nombre
//<NombreDominio>-correos.txt.
bool LstCorreo_Cargar(tListaCorreos &Correos, string Dominio)
{	
	ifstream leo;
	int num;
	bool comp=false;
	LstCorreo_Inicializar(Correos);
	leo.open(Dominio+NOMBRE_TXT);

	if(leo.is_open())
	{
		leo >> num;
		if(Correos.TAM_MAX<=num)
		{
			int tamNuevo = (num/10)*10+10;
			LstCorreo_aumentar(Correos, tamNuevo);
		}
		for(int i=0; i<num; i++) {Correo_Cargar(Correos.Correo[Correos.Cnt], leo); Correos.Cnt++;}
		comp=!comp;
	}
	leo.close();
	return comp;
}

//-------------------------------------------
//Implementa el guardado de la lista de correos en el fichero de correos de
//nombre <NombreDominio>- correos.txt . Guarda en fichero la lista de correos.
//El nombre del fichero será como en el subprograma anterior.
void LstCorreo_Guardar(const tListaCorreos &Correos, string Dominio)
{
	ofstream escribo;

	escribo.open(Dominio+NOMBRE_TXT);
	escribo << Correos.Cnt << endl;
	for(int i=0; i<Correos.Cnt; i++) Correo_Guardar(Correos.Correo[i],escribo);
	escribo.close();
}

//-------------------------------------------
//Dado un correo, si hay espacio en la lista, lo coloca en la posición de la lista que
//le corresponda de acuerdo con su identificador y devuelve true. Si no lo ha
//podido colocar devuelve false.
bool LstCorreo_Insertar(tListaCorreos &Correos, const tCorreo &Correo)
{
	bool comp=false;

	if(Correos.Cnt>=Correos.TAM_MAX)
	{
		int nuevoTam = ((Correos.TAM_MAX*3)/2)+1;
		LstCorreo_aumentar(Correos,nuevoTam);
	}	
	int Pos=0;
	if(!LstCorreo_Buscar(Correos, Correo.CorreoId, Pos))	//Compruebo si ya existe(Si existe devuelvo false y no inserto)
	{
		for( int i=Correos.Cnt; i>Pos; i--)
		Correos.Correo[i]=Correos.Correo[i-1];
		Correos.Correo[Pos]=Correo;
		Correos.Cnt++;
		comp=true;
	}
	return comp;
}

//-------------------------------------------
//Dado un identificador de correo y la lista, devuelve, si dicho identificador existe
//en la lista, su posición y el valor true, y si no existe en la lista, la posición que le
//correspondería y el valor false.
bool LstCorreo_Buscar(const tListaCorreos &Correos, string id, int &Pos)
{
	//Hago una busqueda binaria con el id del correo
	
	int		ini=0, fin=Correos.Cnt-1;
	bool	encontrado=false;

	while(!encontrado && ini<=fin)
	{
		int mitad=(ini+fin)/2;
		if(id==Correos.Correo[mitad].CorreoId)
		{
			encontrado=true;
			Pos=mitad;
		}
		else if(id<Correos.Correo[mitad].CorreoId)
		{
			fin=mitad-1;
			Pos=fin+1;
		}
		else
		{
			ini=mitad+1;
			Pos=ini;
		}
	}
	return encontrado;
}

//-------------------------------------------
//Dada una lista de correos la devuelve ordenada por asunto y fecha. Como es una clave de ordenación doble,
//habrá que redefinir el operador de comparación en el módulo que corresponda.
void LstCorreo_Ordenar_AF(tListaCorreos &Correos)
{
	/*METODO DE ORDENACIÓN POR INSERCIÓN*/
	for(int i=0; i<Correos.Cnt; i++)
	{
		int pos=i;
		tCorreo aux;

		while(pos>0 && Correos.Correo[pos-1]<Correos.Correo[pos])
		{
			//INTERCAMBIO
			aux=Correos.Correo[pos-1];
			Correos.Correo[pos-1]=Correos.Correo[pos];
			Correos.Correo[pos]=aux;
			pos--;
		}
	}
}

void LstCorreo_Ordenar_ID(tListaCorreos &Correos)
{
	/*METODO DE ORDENACIÓN POR INSERCIÓN*/
	for(int i=0; i<Correos.Cnt; i++)
	{
		int pos=i;
		string aux;

		while(pos>0 && Correos.Correo[pos-1].CorreoId>Correos.Correo[pos].CorreoId)
		{
			//INTERCAMBIO
			aux=Correos.Correo[pos-1].CorreoId;
			Correos.Correo[pos-1].CorreoId=Correos.Correo[pos].CorreoId;
			Correos.Correo[pos].CorreoId=aux;
			pos--;
		}
	}
}

void LstCorreo_aumentar (tListaCorreos &Correos, const int nuevoTam)
{
	tListaCorreos Aux;

	Aux.Correo = new tCorreo[nuevoTam];


	for(int i=0; i<Correos.TAM_MAX; i++) Aux.Correo[i] = Correos.Correo[i];
	Correos.TAM_MAX= nuevoTam;
	delete []Correos.Correo;

	Correos.Correo=Aux.Correo;
}