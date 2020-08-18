//===========================================
//Módulo Gestor
//===========================================
#include <conio.h>
#include <iostream>
#include "mGestor.h"
#include "mListaUsuarios.h"
#include "GestorCorreo.h"
#include "ASLib.h"
using namespace std;

//-------------------------------------------
//Inicializa el gestor e intenta arrancarlo cargando la información del dominio que se le pasa como
//parámetro. Para ello inicializará y cargará la lista de usuarios y de correos de
//dicho dominio. Si tiene éxito en todas las operaciones devuelve true y si alguna
//falla devuelve false.
bool GestorArrancar(tGestor &Gestor, string Dominio)
{
	Gestor.Dominio=Dominio;
	Gestor.UsuarioActivo=0;				// No hay Usuario Activo todavia.
	//Cargamos la Lista de Usuarios
	//------------------------------
	bool bUsuarios=false;
	bUsuarios=LstUsuario_Cargar(Gestor.LstUsuarios, Dominio);

	//Cargamos la Lista de Correos
	//------------------------------
	bool bCorreos=false;
	bCorreos=LstCorreo_Cargar(Gestor.LstCorreos, Dominio);
	
	return (bUsuarios && bCorreos);
}

//-------------------------------------------
//Esta operación apaga el gestor y guarda para ello las listas de usuarios y de correos del dominio que se
//encontrase activo en ese momento.
void GestorApagar(const tGestor &Gestor)
{
	LstCorreo_Guardar(Gestor.LstCorreos, DOMINIO);
	LstUsuario_Guardar(Gestor.LstUsuarios, DOMINIO);
	delete []Gestor.LstCorreos.Correo;
	for(int i=0; i<Gestor.LstUsuarios.Cnt; i++)
	{
		delete []Gestor.LstUsuarios.Usuario[i]->Recibidos.Registros;
		delete []Gestor.LstUsuarios.Usuario[i]->Enviados.Registros;
	}
}

//-------------------------------------------
//Lee los datos de usuario necesarios para crear una cuenta (id y contraseña) y si el id de usuario no existe y hay
//espacio en la lista de usuarios, crea la cuenta del usuario y registra al nuevo
//usuario como usuario activo en el gestor. Devuelve un booleano indicando si la
//operación tuvo éxito.
bool GestorCrearCuenta(tGestor &Gestor)
{
	string C_Digitos	= "0123456789";
	string C_Numeros	= "-.";
	string C_Puntuacion = " #$%&@'*+-.,;:=<>?![](){}/|ªº^_\"¿¡";
	string C_Mayusculas = "ABCDEFGHIJKLMNOPQRSTUVWXYZÇÑ";
	string C_Minusculas = "abcdefghijklmnopqrstuvwxyzçñ";
	string C_MayusAcent = "ÁÉÍÓÚÂÊÎÔÛÀÈÌÒÙÄËÏÖÜ";
	string C_MinusAcent = "áéíóúâêîôûàèìòùäëïöüÿ";

	bool CuentaCreada=true;
	
	rAtrCpt Cpt;
	Cpt.Msk = "[¸]";
	Cpt.Tnt = BLANCOB;
	Cpt.Ppl = NEGRO;
	Cpt.Tnx = CYANB;
	Cpt.Vld = V_INTRO + V_TAB + V_CURDOWN + V_AVPAG + V_CTRLINTRO + V_ESCAPE + V_CTRLTAB + V_CURUP + V_REPAG;
	Cpt.Cnj = C_Minusculas+C_Mayusculas+C_Puntuacion+C_Digitos+C_MinusAcent;
	
	int F=2, C=16, L=50, A=12, T=AZUL, P=CYANB;
	rAtrDat Usuario;
	rAtrDat Password;

	CLS(NEGRO);

	rCaja InicioCaja;
	InicioCaja.Fila=F;
	InicioCaja.Columna=C;
	InicioCaja.Largo=L;
	InicioCaja.Alto=A;
	InicioCaja.Tinta=T;
	InicioCaja.Papel=P;
	InicioCaja.Marco=0;
	InicioCaja.Fondo=NEGRO;
	string Titulo="REGISTRO de USUARIO";
	PintaCaja(InicioCaja, Titulo);

	int Fil=F+2, Col=C+8, Tnt=T, Ppl=P;

	string n_="\xA4";
	PintaTxt(Fil+2, Col, Tnt, Ppl, "   Dominio: "+Gestor.Dominio);
	PintaTxt(Fil+4, Col, Tnt, Ppl, "   Usuario:");
	PintaTxt(Fil+6, Col+7, Tnt, Ppl, "Contrase"+n_+"a:");
	PintaTxt(Fil+7, Col, Tnt, Ppl, "Repita Contrase"+n_+"a:");

	rAtrPnt Pnt;
	rCpyPst CpyPst;

	Usuario.Sde=DERECHA;
	Usuario.Car=' ';		//Rellena por la derecha a blancos (Usuario.Sde=DERECHA;)
	Usuario.Lon=17;

	Pnt.Col=Col+11;
	Pnt.Fil=Fil+4;
	Pnt.Ppl=CYANB;
	Pnt.Tnt=AZUL;

	Cpt.Cnj="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_.";
	Cpt.Ppl=T;
	Cpt.Tnt=P;
	Cpt.Vld = V_AUTOSKIP + V_INTRO + V_TAB + V_ESCAPE + V_CTRLTAB;
	CpyPst.Permitido=false;					//false--> se ven los caracteres, true--> se ocultan con asteriscos

	int		ValidoCaptura;
	bool	UsuarioInvalido=true;
	while(UsuarioInvalido)
	{
		Usuario.Dat="";	
		ValidoCaptura=GetTxt(Usuario, Pnt, Cpt, CpyPst);		//Captura el Usuario
		
		if (ValidoCaptura==V_INTRO)
		{
			if(Usuario.Dat=="")						//Si el usuario está en blanco...
			{
				string Mnsj[5];
				Mnsj[0]="El campo 'Usuario'";
				Mnsj[1]="no puede estar en blanco.";
				PintaMensaje(1, Mnsj);
			}
			else									//Verificamos si el Usuario ya existe...
			{
				string id=Usuario.Dat+"@"+DOMINIO;
				int Posicion;
				UsuarioInvalido=false;
				if (LstUsuario_BuscarUsuario(Gestor.LstUsuarios, id, Posicion))	//Si existe...
				{
					string Mnsj[5];
					Mnsj[0]="Lo sentimos.";
					Mnsj[1]="El Nombre de Usuario ya existe.";
					Mnsj[2]="Elija otro Nombre de Usuario";
					PintaMensaje(1, Mnsj);
					UsuarioInvalido=true;
				}
			}
		}
		else
		{
			UsuarioInvalido=false;	//Ha dado ESCAPE al capturar usuario. Salgo del bucle y controlo abajo...
			CuentaCreada=false;
		}
	}

	if (ValidoCaptura==V_INTRO && CuentaCreada==true)		//Ha salido con INTRO. Verifico mas cosas
	{
		Password.Sde=DERECHA;
		Password.Car=' ';
		Password.Lon=10;
		CpyPst.Permitido=true;					//false--> se ven los caracteres, true--> se ocultan con asteriscos

		bool PasswordsDiferentes=true;
		do
		{
			Pnt.Fil=Fil+6;
			Cpt.Cnj="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_.@#%&$/()=?¿'!¡*+-,;:-çÇ[]^";
			Pnt.Col=Col+11+7;
			Password.Dat="";
			
			do
			{
				GetTxt(Password, Pnt, Cpt, CpyPst);		//Captura la Password
				Limpia(Password.Dat);
				if (Password.Dat=="") Pita(1);
			}while (Password.Dat=="");

			string Pass=Password.Dat;
			Pnt.Fil++;
			Password.Dat="";
			GetTxt(Password, Pnt, Cpt, CpyPst);		//ReCaptura la Password
			if (Pass==Password.Dat) PasswordsDiferentes=false;
			else
			{
				string Mnsj[5];
				Mnsj[0]="Las contrse"+n_+"as no coinciden.";
				PintaMensaje(1, Mnsj);
				PintaTxt(Fil+6, Col+18,	Tnt, Ppl, "            ");
				PintaTxt(Fil+7, Col+18,	Tnt, Ppl, "            ");
			}
		}while (PasswordsDiferentes);

		//Ya tenemos Usuario y Password. Vamos a darlo de alta
		tUsuario UsuarioNuevo;
		Usuario_Inicializar(UsuarioNuevo,Usuario.Dat+"@"+DOMINIO,Password.Dat);

		string Mnsj[5];
		if (LstUsuario_Aniadir(Gestor.LstUsuarios, UsuarioNuevo))
		{
			int Posicion=0;
			LstUsuario_BuscarUsuario(Gestor.LstUsuarios, UsuarioNuevo.Nombre, Posicion);
			Gestor.UsuarioActivo=Posicion;
			
			Mnsj[0]="Enhorabuena !";
			Mnsj[1]="Su registro se ha realizado";
			Mnsj[2]="con exito.";
			Mnsj[4]="Bienvenido a "+DOMINIO;
			//Aqui podriamos enviar un correo de Bienvenida al nuevo Usuario...
		}
		else
		{
			Mnsj[0]="Lo sentimos.";
			Mnsj[2]="No caben mas usuarios.";
			Mnsj[4]="Disculpen las molestias.";
			CuentaCreada=false;
		}
		PintaMensaje(1, Mnsj);
	}

	return CuentaCreada;
}

//-------------------------------------------
//Lee los datos de usuario necesarios para validar la cuenta (id y contraseña) y si el usuario existe y la
//contraseña coincide, registra al usuario como usuario activo. Devuelve un
//booleano indicando si la operación tuvo éxito.
bool GestorIniciarSesion(tGestor &Gestor)
{
	string C_Digitos = "0123456789";
	string C_Numeros = "-.";
	string C_Puntuacion = " #$%&@'*+-.,;:=<>?![](){}/|ªº^_\"¿¡";
	string C_Mayusculas = "ABCDEFGHIJKLMNOPQRSTUVWXYZÇÑ";
	string C_Minusculas = "abcdefghijklmnopqrstuvwxyzçñ";
	string C_MayusAcent = "ÁÉÍÓÚÂÊÎÔÛÀÈÌÒÙÄËÏÖÜ";
	string C_MinusAcent = "áéíóúâêîôûàèìòùäëïöüÿ";
	
	rAtrCpt Cpt;
	Cpt.Msk = "[¸]";
	Cpt.Tnt = BLANCOB;
	Cpt.Ppl = NEGRO;
	Cpt.Tnx = CYANB;
	Cpt.Vld = V_INTRO + V_TAB + V_CURDOWN + V_AVPAG + V_CTRLINTRO + V_ESCAPE + V_CTRLTAB + V_CURUP + V_REPAG;
	Cpt.Cnj = C_Minusculas+C_Mayusculas+C_Puntuacion+C_Digitos+C_MinusAcent;
	
	int F=2, C=16, L=50, A=12, T=AZUL, P=CYANB;
	rAtrDat Usuario;
	rAtrDat Password;

	CLS(NEGRO);

	bool Identificado=false;
	int Salida;
	do
	{
		rCaja InicioCaja;
		InicioCaja.Fila=F;
		InicioCaja.Columna=C;
		InicioCaja.Largo=L;
		InicioCaja.Alto=A;
		InicioCaja.Tinta=T;
		InicioCaja.Papel=P;
		InicioCaja.Marco=0;
		InicioCaja.Fondo=NEGRO;
		string Titulo="INICIO de SESION";
		PintaCaja(InicioCaja, Titulo);

		int Fil=F+2, Col=C+8, Tnt=T, Ppl=P;
		PintaTxt(Fil+2, Col, Tnt, Ppl, "   Dominio: "+Gestor.Dominio);
		PintaTxt(Fil+4, Col, Tnt, Ppl, "   Usuario:");
		string _n="\xA4";PintaTxt(Fil+6, Col, Tnt, Ppl, "Contrase"+_n+"a:");

		rAtrPnt Pnt;
		rCpyPst CpyPst;

		Usuario.Car=' ';
		Usuario.Dat="";
		Usuario.Lon=17;
		Usuario.Sde=DERECHA;

		Pnt.Col=Col+11;
		Pnt.Fil=Fil+4;
		Pnt.Ppl=CYANB;
		Pnt.Tnt=AZUL;

		Cpt.Cnj="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_.";
		Cpt.Ppl=T;
		Cpt.Tnt=P;
		Cpt.Vld = V_AUTOSKIP + V_INTRO + V_TAB + V_ESCAPE + V_CTRLTAB;
		CpyPst.Permitido=false;

		Salida=GetTxt(Usuario, Pnt, Cpt, CpyPst);	//Captura el Usuario
			
		if (Salida==V_INTRO)						//Si pulsa[INTRO] pide password
		{
			Identificado=true;						//Se identifica
			Password.Car=' ';
			Password.Dat="";
			Password.Lon=10;
			Password.Sde=DERECHA;

			CpyPst.Permitido=true;					//Esto no vale pa'ná (pendiente de implementar)
			Pnt.Fil=Fil+6;
			Cpt.Cnj="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_.@#%&$/()=?¿'!¡*+-,;:-çÇ[]^";
	
			GetTxt(Password, Pnt, Cpt, CpyPst);		//Captura la Password
		}
		else Identificado=true;				//Si pulsa[ESC] cuando pide nombre quiere salir...
	}while (!Identificado);

	bool UserPassword=false;	// a false por si no pulso [INTRO] en Usuario...
	if (Salida==V_INTRO)		// Si pulso[INTRO] en Usuario, se pidio password y miro su validacion...
	{
		string id=Usuario.Dat+"@"+DOMINIO;
		int Posicion;
		bool Encontrado=LstUsuario_BuscarUsuario(Gestor.LstUsuarios, id, Posicion);
		bool Validado;
		if (Encontrado)	//Si lo encuentra...
		{
			//Verifica la password...
			Validado=Usuario_ValidarContrasenia(*Gestor.LstUsuarios.Usuario[Posicion], Password.Dat);
		}
	
		if (!Encontrado||!Validado)	UserPassword=false;				// Si no lo encuentra o no coincide la password...
		else { Gestor.UsuarioActivo=Posicion; UserPassword=true; }	// Usuario Autentificado.
	}

	return UserPassword;
}

//-------------------------------------------
//Solicita el correo que el usuario quiere leer (será el número con el que el correo
//es mostrado por pantalla en la bandeja correspondiente), valida que existe y si
//es así, lo marca como correo leído. A continuación, busca el correo en la lista de
//correos y si lo encuentra lo muestra en la pantalla de lectura del correo
//void GestorLeerCorreo(tGestor &Gestor, tListaRegistros &ListaReg)
void GestorLeerCorreo(tGestor &Gestor, string CorreoId, int iBandeja)
{
	int Pos=0;
	LstCorreo_Buscar(Gestor.LstCorreos, CorreoId, Pos);		//Si no lo encuentra--->Mensaje de error en ficheros/Datos

	PintaTxt(5,  3, AZUL, CYANB, "    De: "); PintaTxt(5, 11, AZULB, CYANB, Gestor.LstCorreos.Correo[Pos].Emisor);
	PintaTxt(6,  3, AZUL, CYANB, "  Para: "); PintaTxt(6, 11, AZULB, CYANB, Gestor.LstCorreos.Correo[Pos].Destinatario);
	PintaTxt(7,  3, AZUL, CYANB, "Asunto: "); PintaTxt(7, 11, AZULB, CYANB, Gestor.LstCorreos.Correo[Pos].Asunto);
	PintaTxt(5, 50, AZUL, CYANB, "Fecha: ");  PintaTxt(5, 57, AZULB, CYANB, MostrarFecha(Gestor.LstCorreos.Correo[Pos].Fecha)); 
	
	string Mensaje=Gestor.LstCorreos.Correo[Pos].Cuerpo;
	int Lon=Mensaje.length();
	int Fil=9, Col=3;
	
	//
	//Hay que controlar mensajes superlargos --> RE:'s de RE:'s ...
	//
	
	Cursor(SOLIDCURSOR);
	for(int i=0; i<Lon; i++)
	{
		Locate(Fil, Col);
		cout << Mensaje[i];
		Delay(50.0);
		
		if (Mensaje[i]==10)	{ Fil++; Col=3; }
		else				{ Col++; if (Col>78) { Fil++; Col=3; } }
	
		if (Fil==22) { Fil=9; Col=3; }
		
	}
	Cursor(NOCURSOR);

	//Una vez Leído, lo pongo a LEIDO...
	LstReg_CorreoLeido(Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Recibidos, CorreoId);

	//Preguntamos si lo quiere contestar o volver a la bandeja
	string	Menu[MAX_OP_MH];
	Menu[0] = " Volver a la Bandeja ";
	if (iBandeja==-1) Menu[1] = " Contestar "; //Si la bandeja es de ENTRADA

	rMenu	MyMenuUpDown;
	MyMenuUpDown.Fila			= 23;
	MyMenuUpDown.Columna		= 2;
	MyMenuUpDown.TintaNormal	= CYANB;
	MyMenuUpDown.PapelNormal	= NEGRO;
	MyMenuUpDown.Fondo			= NEGRO;
	MyMenuUpDown.TintaOpcion	= AMARILLO;
	MyMenuUpDown.PapelOpcion	= AZULB;
	int Opcion =0;
	Opcion=MenuFila(MyMenuUpDown, Opcion, Menu);
	PintaTxt(23, 1, NEGRO, NEGRO, "                                   ");
	PintaTxt(24, 1, NEGRO, NEGRO, "                                   ");
	PintaTxt(25, 1, NEGRO, NEGRO, "                                   ");

	//Hay que controlar si se quiere contestar al correo
	//Opcion=0 --> Volver a Bandeja. Opcion=1 --> Contestar Correo
	if (Opcion==1)		//Si lo quiere constestar...
	{
		tCorreo CorreoContestar;
		Correo_Contestacion(Gestor.LstCorreos.Correo[Pos], CorreoContestar, Gestor.LstCorreos.Correo[Pos].Emisor);
		GestorEnviarCorreo(Gestor, CorreoContestar);
	}
}

//-------------------------------------------
//Este subprograma implementa el envío del correo en nuestra aplicación. Para ello el
//correo recibido como parámetro es insertado en la lista de correo. Si ha habido
//éxito, entonces se inserta el registro correspondiente en la lista de registros
//enviados del emisor y se intenta insertar igualmente un registro de dicho correo
//en la lista de registros recibidos del destinatario del correo. Si el destinatario no
//existe o si su bandeja de entrada está llena, entonces se mostrará un mensaje de error.
void GestorEnviarCorreo(tGestor &Gestor, tCorreo &CorreoNuevo)
{
	string		Emisor=Limpia(Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Nombre);
	tRegistro	RegistroNuevo;
	string		IdDestinatario=Limpia(CorreoNuevo.Destinatario);
	int			PosicionDestinatario;
	
	//Capturo los datos del correo si es nuevo
	if (CorreoNuevo.Asunto=="") Correo_Nuevo(CorreoNuevo, Emisor);
	
	//Busco la Posicion que ocupa el destinatario dentro del Gestor
	IdDestinatario=CorreoNuevo.Destinatario;
	if (!LstUsuario_BuscarUsuario(Gestor.LstUsuarios, IdDestinatario, PosicionDestinatario))
	{
		string Mnsj[5];
		Mnsj[0]="Lo sentimos. El destinatario:";
		Mnsj[2]=IdDestinatario;
		Mnsj[4]="No existe. Se cancela el envio.";
		PintaMensaje(1, Mnsj);
	}
	else
	{
		string Mnsj[5];
		Mnsj[0]="Atenci\xA2n:";
		Mnsj[1]="Se dispone a Enviar el Correo.";
		Mnsj[3]="Pulse [Aceptar]  para Enviar.";
		Mnsj[4]="Pulse [Cancelar] para Volver.";
	
		int Opcion=PintaMensaje(3, Mnsj);			//=1 Boton1[Aceptar]->Enviar, =2 Boton2[Cancelar]->Regresar
		if (Opcion==1)								//Lo que se tenga que hacer para Enviar el Mensaje.
		{
			//Inserto en la lista de Correos
			LstCorreo_Insertar(Gestor.LstCorreos, CorreoNuevo);
	
			//Ordeno la lista de Correos
			LstCorreo_Ordenar_ID(Gestor.LstCorreos);			
	
			//Inserto en la lista de registros enviados del emisor
			RegistroNuevo.Leido=false;
			RegistroNuevo.CorreoId=CorreoNuevo.CorreoId;
			if(LstReg_Insertar(Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Enviados, RegistroNuevo))
				//Inserto en la lista de registros recibidos del destinatario
				LstReg_Insertar(Gestor.LstUsuarios.Usuario[PosicionDestinatario]->Recibidos, RegistroNuevo);
		}
	}
}

//-------------------------------------------
//Este subprograma implementa el borrado del correo de una bandeja de un
//determinado usuario (OJO: el correo no se borra de la lista de correos). Para
//ello, solicita el correo que el usuario quiere borrar (será el número con el que el
//correo es mostrado por pantalla en la bandeja correspondiente), valida que
//existe y si es así, procede a borrarlo.
//void GestorBorrarCorreo(tGestor &Gestor, tListaRegistros &ListaReg)
void GestorBorrarCorreo(tGestor &Gestor, string CorreoId, int iBandeja)
{
	string Mnsj[5];
	Mnsj[0]="Atenci\xA2n:";
	Mnsj[1]="Se dispone a borrar el Correo.";
	Mnsj[3]="Pulse [Aceptar]  para Borrar.";
	Mnsj[4]="Pulse [Cancelar] para Volver.";
	int Opcion=PintaMensaje(3, Mnsj);		//=1 Boton1[Aceptar]->Borrar, =2 Boton2[Cancelar]->No Borrar

	if (Opcion==1)		//Lo que se tenga que hacer para Borrar el Mensaje.
	{
		if (iBandeja==-1)	//Bandeja de Entrada
		{
			LstReg_Borrar(Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Recibidos, CorreoId);
		}
		else				//Bandeja de Salida
		{
			LstReg_Borrar(Gestor.LstUsuarios.Usuario[Gestor.UsuarioActivo]->Enviados, CorreoId);
		}
	}
}

//-------------------------------------------
//Este subprograma implementa la lectura rápida de correos sin leer. El resultado
//es que muestra en una pantalla todos los correos sin leer ordenados por asunto
//(ignorando todos los “Re: ”) y por fecha. Al finalizar su ejecución los correos sin
//leer quedarán marcados como leídos.
void GestorLecturaRapida(string MatStr[])
{
	string	Dato1, Dato2;
	int		Puntero1, Puntero2, Puntero3, Puntero4, Puntero5, Puntero6, Sw;
	int		LongCadena = MatStr[1].length();			//Tamaño de cada registro. Han de ser iguales de tamaño todos.
	int		Desde=30;
	int		Hasta=60;
	int		LongBloque = Hasta-Desde;

	int Continuar=0;
	if (LongBloque > LongCadena)		Continuar=1;		//Control de substring
	if (LongBloque * LongCadena < 1)	Continuar=2;		//Control de tamaños

	int Registros=0;
	while (MatStr[Registros+1].length()>0) Registros++;	//Cuenta los registros que tienen algo
	if (Registros < 1)					Continuar=3;		//Control de cantidad

	if (Continuar==0)
	{
		//-------------------------------------------
		Puntero1 = int(log(Registros) / log(2));	//Puntero1 = int(log(Registros) / log(2) + .5);
		Puntero2 = int(pow(2, Puntero1) / 2);

		while (Puntero2)
		{
			Puntero3 = Registros-Puntero2;
			Puntero4 = 1;
			while (Puntero4 <= Puntero3)
			{
				Puntero5 = Puntero4;
				do
				{
					Puntero6 = Puntero5 + Puntero2;
					Dato1 = MatStr[Puntero5].substr(32, 39);
					Dato2 = MatStr[Puntero6].substr(32, 39);
					//Quitamos los 'Re: '
					/*==================================
						Duda: quitamos solo los re: de la respuesta que se acaba de hacer? Porque si yo hago una respuesta a una respuesta tendre
						Re: Re: y yo personalmente no lo he quitado porque considero que el segundo Re ya formaria parte del asunto y tampoco lo veo
						muy especificado en la P4...
					====================================*/
					while (Dato1.substr(0, 4)=="Re: ") { Dato1=Dato1.substr(4); Dato1.insert(25, "    "); }
					while (Dato2.substr(0, 4)=="Re: ") { Dato2=Dato2.substr(4); Dato2.insert(25, "    "); }
					Sw = 1;
					if (Dato1>Dato2)
					{
						string Temp=MatStr[Puntero5];
						MatStr[Puntero5]=MatStr[Puntero6];
						MatStr[Puntero6]=Temp;
						Puntero5 = Puntero5 - Puntero2;
						if (Puntero5 > 0) Sw = 0;
					}
				}while (Sw == 0);
				Puntero4 = Puntero4 + 1;
			}	//wend
			Puntero2 = int(Puntero2 / 2);
		}	//wend
	}
}

