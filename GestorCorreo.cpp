/*
	Cambios:
		- Incorporada la función lectura rapida.
		- Arreglado el fallo de que cuando listaUsuarios estaba llena y creaba un usuario nuevo, controlaba que no quedaban pero iniciaba sesion con el ultimo usuario creado(o de la lista).
		- Incorporaciones de la practica 5
		- Mejorada/arreglada la funcion insertar usuario.
		- Mejora en control de registro de usuarios. (Cuando le dabas a escape mientras escribias el usuario al crearlo, saltaba porque no tenia control sobre el escape)
		- (mCorreo -> Linea 429) Eliminé una ordenación que se hacía al enviar siempre un correo de todos los correos, pues no hacía falta ya que inserta de forma ordenada el correo.
		- Cambié dos cosas de la funcion MostrarFecha y MostrarSoloDia para no usar la directiva que usaba al principio (antes hacia un tm *ltm y ahora no es un puntero).
		- Controlado al escribir correo que no se pueda poner la 'X' mayuscula para que no se confunda con el centinela
*/
/// =============================================================================
// Gestor de Correo
// =============================================================================
#include "mCorreo.h"
#include "mListaCorreos.h"
#include "mUsuario.h"
#include "mListaUsuarios.h"
#include "mListaRegistros.h"
#include "mGestor.h"
#include "GestorCorreo.h"
#include "ASLib.h"

using namespace std;
//-------------------------------------------
//===========================================
// Módulo Principal
//===========================================

//===========================================
//Funciones
//===========================================
string MostrarFecha(time_t fecha)
{
    stringstream resultado;
    tm ltm;
	localtime_s(&ltm, &fecha);
    resultado << setfill ('0') << setw (2) << ltm.tm_mday << "-" << setfill ('0') << setw (2) << 1 + ltm.tm_mon << "-" << 1900 + ltm.tm_year ;
    resultado << " [" <<  setfill ('0') << setw (2) << ltm.tm_hour << ":" <<  setfill ('0') << setw (2) << ltm.tm_min << ":" <<  setfill ('0') << setw (2) << ltm.tm_sec << "]";
    return resultado.str();
}
//===========================================
string MostrarSoloDia(time_t fecha)
{
    stringstream resultado;
    tm ltm;
	localtime_s(&ltm, &fecha);
    resultado << setfill ('0') << setw (2) << ltm.tm_mday << "-" << setfill ('0') << setw (2) << 1 + ltm.tm_mon << "-" << 1900 + ltm.tm_year ;
    return resultado.str();
}

//===========================================

//===========================================
//
int Bandeja(int Bandeja, tGestor Gestor, string &CorreoSeleccionado, bool Ordenar)
{
	string Usuario=Limpia(Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Nombre);
	
	int F=1, C=2, L=78, A=20, T=AZUL, P=CYANB;
	PintaTxt(F,C,VERDEB,NEGRO,Usuario);
	time_t fecha=time(0);
	PintaTxt(F,C+57,AMARILLO,NEGRO,MostrarFecha(fecha));
	
	rCaja InicioCaja;
	InicioCaja.Fila=F+1;
	InicioCaja.Columna=C;
	InicioCaja.Largo=L;
	InicioCaja.Alto=A;
	InicioCaja.Tinta=T;
	InicioCaja.Papel=P;
	InicioCaja.Marco=0;
	InicioCaja.Fondo=NEGRO;
	
	int NumMensajes=0;
	int Indice=0;
	string Titulo;
	if (Bandeja==-1) { Titulo="BANDEJA de ENTRADA";	}
	else			 { Titulo="BANDEJA de SALIDA "; }

	PintaCaja(InicioCaja, Titulo);

	rCaja CajaFondo;
	CajaFondo.Fila=6;			//Fila de la ventana
	CajaFondo.Columna=2;		//Columna de la ventana
	CajaFondo.Alto=11;			//==> Numero de filas a mostrar en la ventana interior. ==> Alto de la ventana se recalcula
	CajaFondo.Largo=72;			//==> Largo de filas a mostrar en la ventana interior. ==> Largo de la ventana se recalcula
	CajaFondo.Tinta=VERDE;
	CajaFondo.Papel=AZUL;
	CajaFondo.Fondo=CYANB;
	CajaFondo.Marco=1;

	string *Tabla = new string [Gestor.LstCorreos.TAM_MAX+1];
	//aqui se cargan los mensajes de Entrada o Salida del Usuario Activo
	//------------------------------------------------------------------
	if (Bandeja==-1)	//Bandeja de Entrada
	{
		NumMensajes=Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Recibidos.Cnt;
		for (int i=0; i<NumMensajes; i++)
		{
			//La Tabla hay que cargarla desde el elemento 1, no el 0...
			//---------------------------------------------------------
			if (Ordenar)
			{
				if (!Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Recibidos.Registros[i].Leido)
				{
					Tabla[Indice+1]="   ";
					string Id=Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Recibidos.Registros[i].CorreoId;
					int Pos;
					LstCorreo_Buscar(Gestor.LstCorreos, Id, Pos);
					Tabla[Indice+1]+=LlenaDer(Gestor.LstCorreos.Correo[Pos].Emisor, 29, ' ');
					Tabla[Indice+1]+=LlenaDer(Gestor.LstCorreos.Correo[Pos].Asunto, 29, ' ');
					string Fecha=MostrarSoloDia(Gestor.LstCorreos.Correo[Pos].Fecha);
					Tabla[Indice+1]+=Fecha.substr(6, 4)+"-"+Fecha.substr(3, 2)+"-"+Fecha.substr(0, 2)+"  ";
					Indice++;
				}
			}
			else
			{
				if (Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Recibidos.Registros[i].Leido) Tabla[i+1]=" * ";
				else  Tabla[i+1]="   ";

				string Id=Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Recibidos.Registros[i].CorreoId;
				int Pos;
				LstCorreo_Buscar(Gestor.LstCorreos, Id, Pos);
				Tabla[i+1]+=LlenaDer(Gestor.LstCorreos.Correo[Pos].Emisor, 29, ' ');
				Tabla[i+1]+=LlenaDer(Gestor.LstCorreos.Correo[Pos].Asunto, 29, ' ');
				string Fecha=MostrarSoloDia(Gestor.LstCorreos.Correo[Pos].Fecha);
				Tabla[i+1]+=Fecha.substr(6, 4)+"-"+Fecha.substr(3, 2)+"-"+Fecha.substr(0, 2)+"  ";

			}
		}
	}
	else				// Bandeja de Salida
	{
		NumMensajes=Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Enviados.Cnt;
		for (int i=0; i<NumMensajes; i++)
		{
			//La Tabla hay que cargarla desde el elemento 1, no el 0...
			//---------------------------------------------------------
			if (Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Enviados.Registros[i].Leido) Tabla[i+1]=" * ";
			else  Tabla[i+1]="   ";

			string Id=Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Enviados.Registros[i].CorreoId;
			int Pos;
			LstCorreo_Buscar(Gestor.LstCorreos, Id, Pos);
			Tabla[i+1]+=LlenaDer(Gestor.LstCorreos.Correo[Pos].Destinatario, 29, ' ');
			Tabla[i+1]+=LlenaDer(Gestor.LstCorreos.Correo[Pos].Asunto, 29, ' ');
			string Fecha=MostrarSoloDia(Gestor.LstCorreos.Correo[Pos].Fecha);
			Tabla[i+1]+=Fecha.substr(6, 4)+"-"+Fecha.substr(3, 2)+"-"+Fecha.substr(0, 2)+"  ";
		}
	}

	//Una vez cargada la tabla de mensajes se invoca a la funcion de seleccion
	
	int Elemento;
	if (Ordenar && (Bandeja==-1))
	{
		GestorLecturaRapida (Tabla);
		Elemento=Indice;
	}
	else						Elemento=NumMensajes;									//Cantidad de elementos en la tabla.
	int Salida=MatSel (Tabla, CajaFondo, Elemento, Bandeja);
	
	//Detectamos el Correo_Id Seleccionado para trabajar sobre él.
	if (Bandeja==-1)				CorreoSeleccionado = Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Recibidos.Registros[Elemento-1].CorreoId;
	else						CorreoSeleccionado = Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Enviados.Registros [Elemento-1].CorreoId;
	delete [] Tabla;
	return Salida;												//Devuelve el mismo código de Salida de la funcion de seleccion
}

//===========================================
//
int main()
{
	int iBandeja=-1;		//Bandeja de Entrada
	bool Ordenar=false;
	int Opcion=2;			//Bandeja
	time_t fecha=time(0);

	//Arrancamos el Gestor
	//--------------------
	tGestor Gestor;
	
	if (!GestorArrancar(Gestor, DOMINIO))
	{
		string Mnsj[5];
		Mnsj[0]="No se pudo arrancar el Gestor";
		Mnsj[1]="del dominio "+DOMINIO;
		Mnsj[3]="Revise los ficheros de datos";
		Mnsj[4]="y sus permisos.";
		PintaMensaje(1, Mnsj);
		//Como no se ha arrancado, no se apaga...
		ofstream ArchivoCorreos, ArchivoUsuarios;
		ArchivoCorreos.open (DOMINIO+"_correos.txt" );
		ArchivoUsuarios.open(DOMINIO+"_usuarios.txt");
		ArchivoCorreos << 0;
		ArchivoUsuarios << "XXX";
		ArchivoCorreos.close();
		ArchivoUsuarios.close();

	}
	else
	{
		Gestor;
		bool Finalizar=false;
		do
		{
			//Se ha arrancado el Gestor. (Ha cargado datos de los ficheros)
			//Solicita Usuario y Password.
			//---------------------------
			bool InicioSesion=false;
			while (!InicioSesion)
			{
				InicioSesion=GestorIniciarSesion(Gestor);
				if (!InicioSesion)					// Si No Existe o No Coincide password...
				{
					CHAR_INFO	MyBuff[80*50];		//Para preservar lo que habia
					rCaja		MyCaja;
					MyCaja.Fila 	= 5;
					MyCaja.Columna	= 24;
					MyCaja.Largo 	= 32;
					MyCaja.Alto 	= 15;
					MyCaja.Tinta 	= BLANCOB;
					MyCaja.Papel 	= VERDE;
					MyCaja.Marco	= 0;
					MyCaja.Fondo 	= CYANB;
					string Titulo="";
					GetCaja(MyCaja.Fila, MyCaja.Columna, MyCaja.Largo, MyCaja.Alto, MyBuff);
					PintaCaja(MyCaja, Titulo);
					PintaTxt(MyCaja.Fila+1, MyCaja.Columna+1, MyCaja.Tinta, MyCaja.Papel, "    El Usuario no existe o    ");
					PintaTxt(MyCaja.Fila+2, MyCaja.Columna+1, MyCaja.Tinta, MyCaja.Papel, " Usuario/Password incorrecto. ");
			
					rMenu MyMenuUpDown;
					string Menu[4];
					Menu[0]=" Identificarse ";
					Menu[1]="  Registrarse ";
					Menu[2]="   Finalizar ";
					int Opcion=0;

					MyMenuUpDown.Fila			= MyCaja.Fila+4;
					MyMenuUpDown.Columna		= MyCaja.Columna+9;
					MyMenuUpDown.TintaNormal	= CYANB;
					MyMenuUpDown.PapelNormal	= VERDE;
					MyMenuUpDown.Fondo			= VERDE;
					MyMenuUpDown.TintaOpcion	= VERDE;
					MyMenuUpDown.PapelOpcion	= VERDEB;
					Opcion=MenuUpDown(MyMenuUpDown, Opcion, Menu);
					
					PutCaja(MyCaja.Fila, MyCaja.Columna, MyCaja.Largo, MyCaja.Alto, MyBuff);
					int Nuevo=0;
					switch(Opcion)			
					{
					case 0:					// Identificarse
							break;
					case 1:					// Registrarse
							if (GestorCrearCuenta(Gestor)) InicioSesion=true;
							else
							{
								Opcion=0;
								InicioSesion=false;
							}
							break;
					case 2:					//Finalizar
							Opcion=0;
							InicioSesion=true;
							Finalizar=true;
							break;
					}
				}	//fin de si no existe o no coincide la password
			}		//fin !InicioSesion 

			if (!Finalizar)
			{
				Opcion=2;			//Bandeja
				iBandeja=-1;			//Bandeja de Entrada

				tCorreo		CorreoNuevo;
				string		CorreoSeleccionado;
				while(Opcion!=0)
				{
					string Mnsj[5];
					switch(Opcion)
					{
					case 1: Mnsj[0]=Limpia(Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Nombre);
							Mnsj[1]="Gracias por usar "+DOMINIO;
							Mnsj[3]="Esperamos verle pronto.";
							Mnsj[4]="F_d_I U.C.M. mail";
							PintaMensaje(1, Mnsj);
							Opcion=0; break;
					case 2: Opcion=Bandeja(iBandeja, Gestor, CorreoSeleccionado, Ordenar); if (Opcion==2) iBandeja=0-iBandeja; break;
					case 3: Ordenar=!Ordenar; Opcion=2; break;												//Ordenar 
					case 4: CorreoNuevo.Asunto=""; 
							Ordenar=false; GestorEnviarCorreo(Gestor, CorreoNuevo); 					Opcion=2; break;	//Nuevo
					case 5: Ordenar=false; GestorBorrarCorreo(Gestor, CorreoSeleccionado, iBandeja);	Opcion=2; break;	//Borrar
					case 6: Ordenar=false; GestorLeerCorreo(Gestor, CorreoSeleccionado, iBandeja);		Opcion=2; break;	//Leer
					}
				}
			}
		}while (!Finalizar);
	
		GestorApagar(Gestor);		//Cerramos el Gestor (Guardamos Listas en Ficheros)
	
		CLS(CYANB);
		string Mnsj[5];
		Mnsj[0]="Gracias por usar "+DOMINIO;
		Mnsj[1]="(c) Alfonso Soria";
		Mnsj[3]="Facultad de Inform\xA0tica. U.C.M.";
		Mnsj[4]="v1.0 - Abril 2015";
		PintaMensaje(1, Mnsj);
	}
	return 0;
}
