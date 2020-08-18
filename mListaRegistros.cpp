//===========================================
//Módulo ListaRegistros
//===========================================

#include "mListaRegistros.h"
#include "ASLib.h"
using namespace std;

//-------------------------------------------
//Inicializa la lista.
void LstReg_Inicializar(tListaRegistros &ListaReg)
{
	ListaReg.Registros = new tRegistro [MAXREGISTROS];
	ListaReg.TAM_MAX = MAXREGISTROS;
	ListaReg.Cnt=0;
}

//-------------------------------------------
//Dado un flujo de archivo de entrada (ya abierto), lee los datos que corresponden a
//una lista de registros y la devuelve.
void LstReg_Cargar(tListaRegistros &ListaReg, ifstream &Archivo)
{
	int a;
	a=ListaReg.Cnt;
	LstReg_Inicializar(ListaReg);
	ListaReg.Cnt=a;

	if(ListaReg.TAM_MAX<=ListaReg.Cnt)
	{
		int tamNuevo = (ListaReg.Cnt/10)*10+10;
		LstReg_aumentar(ListaReg, tamNuevo);
	}
	for(int i=0; i<ListaReg.Cnt; i++)
	{
		Archivo >> ListaReg.Registros[i].CorreoId;
		Archivo >> ListaReg.Registros[i].Leido;
	}
}

//-------------------------------------------
//Dado un flujo de archivo de salida (ya abierto), guarda los datos de
//la lista de registro.
void LstReg_Guardar(const tListaRegistros ListaReg, ofstream &Archivo)
{
	Archivo << ListaReg.Cnt	<< endl;
	for(int i=0; i<ListaReg.Cnt; i++)
	{
		Archivo << ListaReg.Registros[i].CorreoId;
		Archivo << " ";
		Archivo << ListaReg.Registros[i].Leido;
		Archivo << endl;
	}
}

//-------------------------------------------
//Dado un registro lo inserta al final de la lista. Si la lista está llena devuelve
//false, en otro caso devuelve true. Este subprograma se ejecutará cuando un
//usuario envíe un correo ya que se insertará el registro correspondiente en la
//lista de registros que representa su bandeja de salida, y también en las listas de
//registros que representan las bandejas de entrada de cada uno de los
//destinatarios del correo.
bool LstReg_Insertar(tListaRegistros &ListaReg, tRegistro Registro)
{
	bool comp=false;

	if(ListaReg.Cnt>=ListaReg.TAM_MAX)
	{
		int nuevoTam = ((ListaReg.TAM_MAX*3)/2)+1;
		LstReg_aumentar(ListaReg, nuevoTam);
	}

	ListaReg.Registros[ListaReg.Cnt]=Registro;
	ListaReg.Cnt++;
	comp=!comp;

	return comp;	//Ya no hace falta que sea booleano... pero tampoco molesta.
}

//-------------------------------------------
//Dado un identificador de correo, busca el registro correspondiente y si lo encuentra lo
//elimina de la lista (¡sin dejar huecos!). Si no lo encuentra, devuelve false, en
//otro caso devuelve true. Este subprograma representa la acción de un usuario
//del borrado de un determinado correo de una de sus bandejas. OJO: esta
//operación sólo supone que el registro es borrado de la lista de registros, pero el
//correo seguirá existiendo en la lista de correos.
bool LstReg_Borrar(tListaRegistros &ListaReg, string Id)
{
	bool comp=false;
	int pos;

	pos=LstReg_Buscar(ListaReg, Id);	

	if(pos!=CENTINELA_POS)
	{
		/*Como dice que está ordenada por el orden de llegada....*/
		for(int i=pos; i<ListaReg.Cnt-1; i++) ListaReg.Registros[i]=ListaReg.Registros[i+1];
		ListaReg.Cnt--;
		comp=true;
	}

	return comp;
}

//-------------------------------------------
//Dado un identificador de correo, busca el correspondiente registro y pone el indicador de
//leído a true. La operación devuelve un booleano indicando si se encontró o no el registro.
bool LstReg_CorreoLeido(tListaRegistros &ListaReg, string Id)
{
	bool comp=false;
	int pos;

	pos=LstReg_Buscar(ListaReg,Id);

	if(pos!=CENTINELA_POS)
	{
		ListaReg.Registros[pos].Leido=true;
		comp=!comp;
	}
	return comp;
}

//-------------------------------------------
//Dado un identificador de correo y la lista, devuelve, sidicho identificador existe en la
//lista, su posición, y si no existe devuelve ­‐1.
int LstReg_Buscar(const tListaRegistros &ListaReg, string Id)
{
	int pos=CENTINELA_POS, i=0;

	/*	Como los registros estan ordenados por fecha(Orden de llegada)
		la busqueda del identificador no puede ser binaria	*/

	while(pos==CENTINELA_POS && i<ListaReg.Cnt)
	{
		if(ListaReg.Registros[i].CorreoId==Id)
			pos=i;
		i++;
	}

	return pos;
}

void LstReg_aumentar (tListaRegistros &ListaReg, const int nuevoTam)
{
	tListaRegistros Aux;

	Aux.Registros = new tRegistro[nuevoTam];

	for(int i=0; i<ListaReg.TAM_MAX; i++)	Aux.Registros[i] = ListaReg.Registros[i];
	ListaReg.TAM_MAX = nuevoTam;
	delete []ListaReg.Registros;

	ListaReg.Registros=Aux.Registros;
}