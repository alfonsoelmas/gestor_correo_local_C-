#include "ASLib.h"
#include "GestorCorreo.h"		//Para pintar la hora permanentemente en EsperaEvento.
using namespace std;

// =============================================================================
// Funciones
// =============================================================================
	void GetCaja (int Fila, int Columna, int Largo, int Alto, CHAR_INFO Buffer[80*50]);
	void PutCaja (int Fila, int Columna, int Largo, int Alto, CHAR_INFO Buffer[80*50]);

	//==============================================================================
	// Funcion Pita
	//==============================================================================
	void Pita(int Veces)
	{
	   int	i;
	   for (i=0; i<Veces ; i++) cout << "\a";	// printf ("%c", 7)
	}
	//==============================================================================
	// Funcion Locate Fila Columna
	//==============================================================================
	void Locate(int Fila, int Columna)  
	{  
		COORD Locate = {Columna-1, Fila-1};  
		//Locate.X = Columna-1;  
		//Locate.Y = Fila-1;  
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Locate);  
	}
	//==============================================================================
	//	Funciones de Relleno
	//==============================================================================
	string LlenaDer(string &Dato, int TotLon, char Pad)
	{
		int	i;
		int	Len = Dato.length();

		if (Len >= TotLon) return Dato;
		for (i = 0; i < TotLon - Len; i++) {Dato += Pad;}
		return Dato;
	}
	//==============================================================================
	string LlenaIzq(string &Dato, int TotLon, char Pad)
	{
		string	Aux="";
		int	i;
		int	Len = Dato.length();
		if (Len >= TotLon) return Dato;
 		Aux += Pad;
		for (i = 1; i < TotLon - Len; i++) Aux += Pad;
		Aux += Dato;
		Dato = Aux;
		return Dato;
	}
	//==============================================================================
	string LlenaCen(string &Dato, int TotLon, char Pad)
	{
		int	LonAux;
		int	Len = Dato.length();
		if (Len >= TotLon) return Dato;
		LonAux = (Len + TotLon) / 2;
	
		return LlenaIzq (LlenaDer (Dato, LonAux, Pad), TotLon, Pad);
	}
	//==============================================================================
	// Funcion Limpia blancos
	//==============================================================================
	string Limpia(string &Dato)
	{
		if (Dato.length())
		{
			while (Dato[0]==' ') Dato.erase(0, 1);
			while (Dato[Dato.length()-1]==' ') Dato.erase(Dato.length()-1, 1);
		}
		return Dato;
	}
	//==============================================================================
	// Funcion Borra Pantalla
	//==============================================================================
	void CLS(int Color)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color*16);
		system("cls");
		Locate(1, 1);
		Cursor(NOCURSOR);
	}
	//==============================================================================
	// Funcion Color Tinta Papel
	//==============================================================================
	void Color(int Tinta, int Papel) 
	{ 
		int Color;
		Color = Tinta + Papel * 16;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color); 
	} 
	//==============================================================================
	// Funcion Cursor
	//==============================================================================
	void Cursor (int TipoCursor)
	{
		//TipoCursor = NOCURSOR		= 0	Cursor invisible
		//TipoCursor = SOLIDCURSOR	= 1	Cursor grande
		//TipoCursor = NORMALCURSOR	= 2	Cursor normal
		//TipoCursor = ?			= x	Cursor fino

		CONSOLE_CURSOR_INFO MyCursor = {1, FALSE};	//Por defecto invisible

		switch (TipoCursor)
		{
			case NOCURSOR:		MyCursor.dwSize=1;		MyCursor.bVisible = FALSE;	break;	
			case SOLIDCURSOR:	MyCursor.dwSize=100;	MyCursor.bVisible = TRUE;	break;	
			case NORMALCURSOR:	MyCursor.dwSize=16;		MyCursor.bVisible = TRUE;	break;
			default:			MyCursor.dwSize=8;		MyCursor.bVisible = TRUE;	break;
		}
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &MyCursor);
	}
	//==============================================================================
	// Funcion EsperaEvento
	//==============================================================================
	void EsperaEvento(int &Tecla, rRaton &XRaton)
	{
		HANDLE			hStdin, Entrada, Salida;
		DWORD			result;
		INPUT_RECORD	inBuf;
		int				TeclaExt = 0;
		Tecla = 0;
		static COORD	MyPos;					//Coordenadas definitivas. Así se preserva su valor cuando se mueve la rueda;

		/* Obtiene los manejadores estándar */
		Entrada = GetForegroundWindow();
		hStdin = GetStdHandle (STD_INPUT_HANDLE);
		result = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
		SetConsoleMode(hStdin, result);

	INI:
		FlushConsoleInputBuffer(hStdin);			// lee los eventos del ratón o teclado
		result=0;
		while (result==0)
		{
			PeekConsoleInput(hStdin, &inBuf, 1, &result);
			PintaTxt(1,59,AMARILLO,NEGRO,MostrarFecha(time(0)));	//PINTA LA FECHA Y HORA
		}
		//Hay algo... Lo lee
		ReadConsoleInputA (hStdin, &inBuf, 1, &result);

		if(inBuf.EventType == KEY_EVENT)    	//KEY_EVENT=1, MOUSE_EVENT=2, 16 cambia ventana
		{
			if( inBuf.Event.KeyEvent.bKeyDown )
				{
					Tecla		= inBuf.Event.KeyEvent.uChar.AsciiChar;
					TeclaExt	= inBuf.Event.KeyEvent.wVirtualKeyCode;
					if (Tecla < 0) Tecla = 256 + Tecla;
					if (Tecla == 0 && TeclaExt != 0) { Tecla = 1000+TeclaExt; }
				}
		}

		if ( inBuf.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)				//Solo lo leo aquí para que si muevo la rueda no distorsione las coordenadas
		{
			MyPos.Y = inBuf.Event.MouseEvent.dwMousePosition.Y + 1;
			MyPos.X = inBuf.Event.MouseEvent.dwMousePosition.X + 1;	
		}
		switch (inBuf.Event.MouseEvent.dwButtonState)
		{
   			case 1:				XRaton.IBoton = 1; break;
   			case 2:				XRaton.DBoton = 1; break;
			case 3:				XRaton.TBoton = 1; break;
   			case 4:				XRaton.CBoton = 1; break;
			case 7864320:		XRaton.Rueda  = 1; break;	//Rueda hacia arriba	HIWORD(inBuf.Event.MouseEvent.dwButtonState)= 120
			case 4287102976:	XRaton.Rueda  =-1; break;	//Rueda hacia abajo		HIWORD(inBuf.Event.MouseEvent.dwButtonState)= 65416
			default: break;
		}

		if ( inBuf.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)	XRaton.DClick = 1;		// Reporta si se dio un doble click
		
		XRaton.Fila		= MyPos.Y;
		XRaton.Columna	= MyPos.X;

		if (Tecla == 0 && inBuf.EventType == KEY_EVENT) goto INI;
		if (Tecla == 0) Tecla = -1;

		FlushConsoleInputBuffer(hStdin);
		Salida = GetForegroundWindow();
		if (Entrada != Salida)							// Si el Handle de entrada no es el mismo...
		{ Tecla=-1; XRaton.IBoton=0; XRaton.DBoton=0; XRaton.TBoton=0; XRaton.CBoton=0; XRaton.Rueda=0; XRaton.Fila=0; XRaton.Columna=0; }
	}
	//========================================================================
	// Raton en Caja
	//========================================================================
	int	RatonInCaja (int Fila, int Columna, int Largo, int Alto, rRaton Raton)
	{
		int Salida=0;

		if ((Raton.Fila		>= Fila)			&&
			(Raton.Columna	>= Columna)			&&
			(Raton.Fila		<= Fila+Alto-1 )	&&
			(Raton.Columna	<= Columna+Largo-1 )) Salida=1;
		
		return Salida;
	}
	//========================================================================
	// Funcion Espera milisegundos
	//========================================================================
	void EsperaMiliSec(double iSeconds)
	{
		 int iSeg1, iSeg2;
		 iSeg1 = (int)clock();			//CLOCKS_PER_SEC;
		 iSeg2 = iSeg1;
		 while((iSeg2-iSeg1) < iSeconds)
			  iSeg2 = (int)clock();		//CLOCKS_PER_SEC;
	}
	// =============================================================================
	// Delay en milisegundos
	// =============================================================================
	void Delay(double MiliSegundos)
	{
		clock_t StartTime	= clock();
		clock_t EndTime		= StartTime + (clock_t)MiliSegundos;
		while( clock() < EndTime );
	}
	//========================================================================
	// Funcion Char Pinta
	//========================================================================
	void PintaChr (int Fila, int Columna, int Tinta, int Papel, char Car)
	{
		Cursor(NOCURSOR);
		Locate (Fila, Columna);
		Color (Tinta, Papel);
		cout << Car;
	}
	//========================================================================
	// Funcion PintaTxt y BorraTxt
	//========================================================================
	void PintaTxt (int Fila, int Columna, int Tinta, int Papel, string Txt)
	{
		Cursor	(NOCURSOR);
		Locate	(Fila, Columna);
		Color	(Tinta, Papel);
		cout << Txt;
	}
	//========================================================================
	void BorraTxt (int Fila, int Columna, int Papel, int Lon)
	{
		string Tmp;
		Tmp.assign(Lon, ' ');
		Cursor	(NOCURSOR);
		Locate	(Fila, Columna);
		Color	(Papel, Papel);
		cout << Tmp ;
	}
		//========================================================================
	// Espera
	//========================================================================
	void Espera()
	{
		rRaton	MyRaton;
		int		Tecla = -1;
		int		Pulsado = 0;
		CHAR_INFO Buffer[80*50];			//Buffer de 3 lineas
		string	Mensaje;

		GetCaja (23, 1, 80, 3, Buffer);	//Guarda lo que hay
		Mensaje =  "Ü";
		PintaTxt(23, 1, BLANCOB, NEGRO, LlenaCen(Mensaje, 79, 'Ü'));
		Mensaje = "Para continuar pulsa una tecla o haz click con el ratàn.";
		PintaTxt(24, 1, ROJO, BLANCOB, LlenaCen(Mensaje, 79, ' '));
  		Mensaje= "ß";
		PintaTxt(25, 1, BLANCOB, NEGRO, LlenaCen(Mensaje, 79, 'ß'));

		while (Pulsado==0 && Tecla == -1)
		{
			MyRaton.IBoton = 0; MyRaton.DBoton = 0;
			MyRaton.CBoton = 0; MyRaton.TBoton = 0;
			EsperaEvento(Tecla, MyRaton);
			Pulsado = MyRaton.IBoton+MyRaton.DBoton+MyRaton.CBoton+MyRaton.DBoton;
		}
	   PutCaja (23, 1, 80, 3, Buffer);	//Pone lo que habia
	}
	//========================================================================
	// Funcion Pinta Caja
	//========================================================================
	void PintaCaja (rCaja Caja, string Titulo)
	{
		int		ESI, ESD, EII, EID, HS, HI, VI, VD;
		int		i, j, PapelEsp, SaltaLineas;
		string	Relleno;
		char	Pad;

		Cursor(NOCURSOR);
		PapelEsp=Caja.Papel;

		if (Caja.Marco <0)
		{
			i = Caja.Marco*-1;
			ESI=i; ESD=i; EII=i; EID=i; HS=i; HI=i; VI=i; VD=i;
		}
		else
			switch (Caja.Marco)
			{
				case 1:		ESI=218; ESD=191; EII=192; EID=217; HS=196; HI=196; VI=179; VD=179; break;	//Linea simple
				case 2:		ESI=201; ESD=187; EII=200; EID=188; HS=205; HI=205; VI=186; VD=186; break;	//Linea doble
				case 3:		ESI=219; ESD=219; EII=219; EID=219; HS=223; HI=220; VI=219; VD=219; break;	//Linea ancha
				case 4:		ESI=220; ESD=220; EII=223; EID=223; HS=220; HI=223; VI=219; VD=219; break;	//Linea Ancha corta
				case 5:		ESI=219; ESD=219; EII=223; EID=223; HS=223; HI=223; VI=219; VD=219; break;	//Linea Ancha corta-abajo
				case 6:		ESI=220; ESD=220; EII=219; EID=219; HS=220; HI=220; VI=219; VD=219; break;	//Linea Ancha corta-arriba
				case 7:		ESI= 43; ESD= 43; EII= 43; EID= 43; HS= 45; HI= 45; VI=221; VD=221; break;	//Linea texto
				case 8:		ESI=254; ESD=254; EII=254; EID=254; HS=254; HI=254; VI=254; VD=254; break;	//Linea puntos
				case 9:		ESI=250; ESD=250; EII=250; EID=250; HS=250; HI=250; VI=250; VD=250; break;	//Linea puntos
				default:	ESI= 32; ESD= 32; EII= 32; EID= 32; HS= 32; HI= 32; VI= 32; VD= 32; break;	//Sin Marco
			}

		if (Caja.Marco==4 || Caja.Marco==6) PapelEsp=Caja.Fondo;
		else								PapelEsp=Caja.Papel;

		Color(Caja.Tinta, PapelEsp);

		//Si no hay Titulo Pinta la Caja, en caso Contrario, el Titulo
		SaltaLineas=0;
	
		if (Titulo.length() > 0)		// Hay Titulo
		{
			Pad=char(220);	//'_'; //220 solido abajo
			Relleno.insert(0, Caja.Largo, Pad);
			PintaTxt(Caja.Fila,	  Caja.Columna, Caja.Tinta, Caja.Fondo, Relleno);
			PintaTxt(Caja.Fila+1, Caja.Columna, Caja.Papel, Caja.Tinta, LlenaCen(Titulo, Caja.Largo, ' '));
		
			Pad=char(223);	//'¯'; //223 solido arriba
			Relleno="";
			Relleno.insert(0, Caja.Largo, Pad);
			if (Caja.Marco > 2 && Caja.Marco < 7)		// si la linea es ancha...
			{
				Pad=char(219);	//'¦'; //219 solido todo
				Relleno.replace(0, 1, 1, Pad);
				Relleno.replace(Caja.Largo-1, 1, 1, Pad);
			}
			PintaTxt(Caja.Fila+2, Caja.Columna, Caja.Tinta, Caja.Papel, Relleno);
			SaltaLineas = 2;
		}
		else
		{
			Locate(Caja.Fila, Caja.Columna);
			cout  << char(ESI);
			for (i = 1; i < Caja.Largo-1; i++) cout  << char(HS);
 			cout  << char(ESD);
		}

		Color(Caja.Tinta, Caja.Papel);
	
		for (j=1+SaltaLineas; j<Caja.Alto-1; j++)
		{
			Locate(Caja.Fila+j, Caja.Columna);
			cout  << char(VI);
			for (i = 1; i < Caja.Largo-1; i++) cout  << char(32);
			cout  << char(VD);
		}

		if (Caja.Marco==4 || Caja.Marco==5) PapelEsp=Caja.Fondo;
		else								PapelEsp=Caja.Papel;
		Locate(Caja.Fila+j, Caja.Columna);
		Color(Caja.Tinta, PapelEsp);
	
		cout  << char(EII);
		for (i = 1; i < Caja.Largo-1; i++) cout  << char(HI);
		cout  << char(EID);
	}
	// ======================================================================
	// MENU VERTICAL
	// ======================================================================
	int MenuUpDown(rMenu MyMenuUpDown, int Opcion, string Menu[])
	{
		rCaja	MyCaja;
		rRaton	MyRaton;					//Para EsperaEvento
		int		Largo;						//, Alto;
		int		MaxOpciones, OpcionRaton;
		int		Fila, Columna;
		int		i;
		int		SaltoWhile;
		int		Tecla = -1;					//Para EsperaEvento
		int		Direccion;
		string	TopeS;
		string	TopeI;
		string	TopeC;
		double	Tiempo = 0.5;			
		static	rRaton LastRaton;

		Largo = 0;
		MaxOpciones	= -1;
		for (i=0; i<MAX_OP_MH; i++)
		{
			if (Menu[i].length() > (unsigned)Largo)	Largo		= Menu[i].size();				//Busco la Opcion mas larga para rellenar las demas a ese tamaño
			if (Menu[i].length() > 0)				MaxOpciones = MaxOpciones+1; else break;	//Hasta que hay una posicion vacía
		}
	
		if (Opcion < 0)				Opcion = 0;
		if (Opcion > MaxOpciones)	Opcion = MaxOpciones;

		TopeS.assign(Largo, 'Ü');
		TopeI.assign(Largo, 'ß');
		TopeC.assign(Largo, 'Û');

		MyCaja.Fila		= MyMenuUpDown.Fila;
		MyCaja.Columna	= MyMenuUpDown.Columna;
		MyCaja.Largo	= Largo;
		MyCaja.Alto		= (MaxOpciones+1) * 2 + 1;
		MyCaja.Tinta	= MyMenuUpDown.PapelNormal; //.TintaNormal;
		MyCaja.Papel	= MyMenuUpDown.PapelNormal;
		MyCaja.Marco	= 4;
		MyCaja.Fondo	= MyMenuUpDown.Fondo;
		PintaCaja (MyCaja,"");

		// Pinta todas las opciones
		//=========================
		Cursor( NOCURSOR );
		Fila = MyMenuUpDown.Fila+1;
		Columna = MyMenuUpDown.Columna;
		for (i = 0; i < MaxOpciones+1; i++)
		{
			PintaTxt(Fila, Columna, MyMenuUpDown.TintaNormal, MyMenuUpDown.PapelNormal, Menu[i].append(Largo-Menu[i].size(), ' '));
			Fila = Fila + 2;
		}

		Fila = Opcion * 2 + MyMenuUpDown.Fila + 1;
		// Pinta la opcion actual como seleccionada
		//========================================
		PintaTxt(Fila-1, Columna, MyMenuUpDown.PapelOpcion, MyMenuUpDown.Fondo*(Opcion==0) + MyMenuUpDown.PapelNormal*(Opcion!=0), TopeS);
		PintaTxt(Fila,   Columna, MyMenuUpDown.TintaOpcion, MyMenuUpDown.PapelOpcion, Menu[Opcion]);
		PintaTxt(Fila+1, Columna, MyMenuUpDown.PapelOpcion, MyMenuUpDown.Fondo*(Opcion==MaxOpciones) + MyMenuUpDown.PapelNormal*(Opcion!=MaxOpciones), TopeI);
	
		//Bucle hasta INTRO
		while (Tecla != INTRO)
		{
	EVENTO:
			MyRaton.Fila	= 0;	MyRaton.Columna	= 0;	MyRaton.IBoton	= 0;
			MyRaton.DBoton	= 0;	MyRaton.CBoton	= 0;	MyRaton.TBoton	= 0;
			MyRaton.DClick	= 0;
			Tecla=0;
			EsperaEvento(Tecla, MyRaton);
		
			//Miramos si ha habido TECLA o RATON
			//------------------------------------
			if (Tecla==-1)	//No hay TECLA
			{
				SaltoWhile=0;
 				// Si el Evento esta dentro de la caja...
				if (RatonInCaja (MyCaja.Fila, MyCaja.Columna, MyCaja.Largo, MyCaja.Alto, MyRaton))
				{
					// Si el estado del raton ha cambiado
					if ( MyRaton.Fila != LastRaton.Fila || MyRaton.IBoton != LastRaton.IBoton || MyRaton.Columna != LastRaton.Columna )
					{
						LastRaton = MyRaton;
						OpcionRaton = (MyRaton.Fila - MyCaja.Fila + 1) / 2 - 1 ;

						if (OpcionRaton > MaxOpciones)	OpcionRaton = MaxOpciones;
						if (OpcionRaton < 0)			OpcionRaton = 0;
						if (OpcionRaton < Opcion)		Tecla = CURUP;
						else if (OpcionRaton > Opcion)	Tecla = CURDOWN;
					}
					else goto EVENTO;	//No hay TECLA ni cambios en RATON
				}
				else goto EVENTO;		//No hay TECLA y RATON no esta dentro de la caja
			}
			else
			{
				SaltoWhile = 1;		//Ha habido TECLA. Salto el bucle y proceso
			}

			if (MyRaton.IBoton == 1) Tecla = INTRO;

  			switch (Tecla)
			{
				case CURUP		:	Direccion  =-1; break;		//Flecha arriba
				case CURDOWN	:	Direccion  = 1; break;		//Flecha abajo
				case INTRO		:	goto SALIDA;
				case ESC		:	Opcion     =-1; goto SALIDA;
				default			:	goto EVENTO;				//Direccion  = 1; break;		//Flecha abajo
			}

			if (SaltoWhile==1) goto SALTOWHILE;	// Ha habido TECLA

			while (OpcionRaton != Opcion )		// Bucle para sincronizar opciones
			{
	SALTOWHILE:
		   		if (Tecla != INTRO)	//Si no ha validado la opcion
				{	//Pinta el anterior
					//*********************************************************************
					if (Opcion==0 && Direccion==1)
						PintaTxt(Fila-Direccion, Columna, MyMenuUpDown.PapelNormal, MyMenuUpDown.Fondo, TopeS);
					else
						if (Opcion==MaxOpciones && Direccion==-1)
							PintaTxt(Fila-Direccion, Columna, MyMenuUpDown.PapelNormal, MyMenuUpDown.Fondo, TopeI);
						else
							PintaTxt(Fila-Direccion, Columna, MyMenuUpDown.PapelNormal, MyMenuUpDown.PapelNormal, TopeC);

					EsperaMiliSec(Tiempo);
					//*********************************************************************

					if (Opcion==MaxOpciones && Direccion==1)
						PintaTxt(MyMenuUpDown.Fila, Columna, MyMenuUpDown.PapelOpcion, MyMenuUpDown.Fondo, TopeS);
					else
						if (Opcion==0 && Direccion==-1)
							PintaTxt((MaxOpciones*2)+MyMenuUpDown.Fila+2, Columna, MyMenuUpDown.PapelOpcion, MyMenuUpDown.Fondo, TopeI);
						else
							PintaTxt(Fila+Direccion, Columna, MyMenuUpDown.PapelOpcion, MyMenuUpDown.PapelNormal, TopeC);

					EsperaMiliSec(Tiempo);

					//Pinta la Opcion anterior en color normal
					//=====================================================================
					PintaTxt(Fila, Columna, MyMenuUpDown.TintaNormal, MyMenuUpDown.PapelNormal, Menu[Opcion]);
					EsperaMiliSec(Tiempo);

					//Cambia de Opcion y Fila
					//=====================================================================
					Opcion = Opcion + Direccion;
					Fila = Fila + 2*Direccion;
					if (Opcion > MaxOpciones)	{Opcion=0;			 Fila=MyMenuUpDown.Fila+1;}
					if (Opcion < 0)				{Opcion=MaxOpciones; Fila=(MaxOpciones*2)+MyMenuUpDown.Fila+1;}

					//Pinta la Opcion actual en color seleccionado
					//=====================================================================
					PintaTxt(Fila, Columna, MyMenuUpDown.TintaOpcion, MyMenuUpDown.PapelOpcion, Menu[Opcion]);
					EsperaMiliSec(Tiempo);

					//Pinta el tope superior de la opcion actual en brillante
					//*********************************************************************
					if (Opcion==0 && Direccion==1)
						PintaTxt((MaxOpciones*2)+MyMenuUpDown.Fila+2, Columna, MyMenuUpDown.PapelNormal, MyMenuUpDown.Fondo, TopeI);
					else
						if (Opcion==MaxOpciones && Direccion==-1)
							PintaTxt(MyMenuUpDown.Fila, Columna, MyMenuUpDown.PapelNormal, MyMenuUpDown.Fondo, TopeS);
						else
							if (Direccion==1)	PintaTxt(Fila-Direccion, Columna, MyMenuUpDown.PapelOpcion, MyMenuUpDown.PapelNormal, TopeS);
							else		     	PintaTxt(Fila-Direccion, Columna, MyMenuUpDown.PapelOpcion, MyMenuUpDown.PapelNormal, TopeI);

					//EsperaMiliSec(Tiempo);
					Sleep((DWORD)Tiempo);

					//Pinta el tope inferior de la opcion actual en brillante
					//*********************************************************************
					if (Opcion==MaxOpciones && Direccion==1)
						PintaTxt(Fila+Direccion, Columna, MyMenuUpDown.PapelOpcion, MyMenuUpDown.Fondo, TopeI);
					else
   	      				if (Opcion==0 && Direccion==-1)
      	      				PintaTxt(Fila+Direccion, Columna, MyMenuUpDown.PapelOpcion, MyMenuUpDown.Fondo, TopeS);
						else
							if (Direccion==1)	PintaTxt(Fila+Direccion, Columna, MyMenuUpDown.PapelOpcion, MyMenuUpDown.PapelNormal, TopeI);
							else				PintaTxt(Fila+Direccion, Columna, MyMenuUpDown.PapelOpcion, MyMenuUpDown.PapelNormal, TopeS);

					EsperaMiliSec(Tiempo);
				}// FIN Tecla != INTRO
				if (SaltoWhile==1 && Tecla!=INTRO) goto EVENTO;
			}	// FIN bucle While de sincronizacion opciones
		}	//FIN Tecla != INTRO

	SALIDA:
		return(Opcion);
	}	//---FIN FUNCION--
	// ======================================================================
	// MENU HORIZONTAL
	// ======================================================================
	int MenuFila(rMenu MyMenuFila, int Opcion, string Menu[])
	{
		rCaja			MyCaja;
		int				Largo = 0;			//Largo del Menu
		char			TopeS[80];
		char			TopeI[80];
		int				Direccion;			//Derecha o Izquierda
		int				MaxOpciones, OpcionRaton;
		int				Fila, Columna;
		int				i;
		int				SaltoWhile;
		int				Tecla = -1;			//Para EsperaEvento
		rRaton  		MyRaton;			//Para EsperaEvento
		static rRaton	LastRaton;

		//--------------------------------------------------------------------
		MaxOpciones	= -1;
		for (i=0; i<12; i++)
			if (Menu[i].length() > 0)	MaxOpciones=MaxOpciones+1;
			else	break;

		for (i=0; i<=MaxOpciones; i++)
   			Largo = Largo + Menu[i].length();

		if (Opcion < 0)				Opcion = 0;
		if (Opcion > MaxOpciones)	Opcion = MaxOpciones;

		MyCaja.Fila		= MyMenuFila.Fila;
		MyCaja.Columna	= MyMenuFila.Columna;
		MyCaja.Largo	= Largo;
		MyCaja.Alto		= 3;
		MyCaja.Tinta	= MyMenuFila.PapelNormal; //.TintaNormal;
		MyCaja.Papel	= MyMenuFila.PapelNormal;
		MyCaja.Marco	= 4;
		MyCaja.Fondo	= MyMenuFila.Fondo;
		PintaCaja (MyCaja,"");

		// Pinta todas las opciones
		//=========================
		Cursor( NOCURSOR );
		Fila	= MyMenuFila.Fila+1;
		Columna	= MyMenuFila.Columna;
		for (i = 0; i < MaxOpciones+1; i++)
		{
			PintaTxt(Fila, Columna, MyMenuFila.TintaNormal, MyMenuFila.PapelNormal, Menu[i]);
			Columna = Columna + Menu[i].length();
		}

		// Pinta la opcion actual como seleccionada
		//=========================================
		Fila    = MyMenuFila.Fila + 1;
		Columna = MyMenuFila.Columna;
		for (i=0; i<=Opcion-1; i++) { Columna = Columna + Menu[i].length(); }
		i = Menu[Opcion].length();
		memset(TopeS, 'Ü', i); TopeS[i]=0;
		memset(TopeI, 'ß', i); TopeI[i]=0;

		PintaTxt(Fila-1, Columna, MyMenuFila.PapelOpcion, MyMenuFila.Fondo, TopeS);
		PintaTxt(Fila,   Columna, MyMenuFila.TintaOpcion, MyMenuFila.PapelOpcion, Menu[Opcion]);
		PintaTxt(Fila+1, Columna, MyMenuFila.PapelOpcion, MyMenuFila.Fondo, TopeI);

		//Bucle hasta INTRO
		while (Tecla != INTRO)
		{
	EVENTO:
			MyRaton.Fila	= 0;	MyRaton.Columna	= 0;	MyRaton.IBoton	= 0;
			MyRaton.DBoton	= 0;	MyRaton.CBoton	= 0;	MyRaton.TBoton	= 0;
			MyRaton.DClick	= 0;	Tecla=0;
			EsperaEvento(Tecla, MyRaton);

			//Sale de EsperaEvento... Miramos si ha habido TECLA o RATON
			//----------------------------------------------------------
			if (Tecla==-1)	//No hay TECLA
			{
				SaltoWhile = 0;		//Debo entrar en bucle para sincronizar opciones
				// Si el Evento esta dentro de la caja...
				if (RatonInCaja (MyCaja.Fila, MyCaja.Columna, MyCaja.Largo, MyCaja.Alto, MyRaton))
				{
					// Si el estado del raton ha cambiado
					if ( MyRaton.Columna != LastRaton.Columna || MyRaton.Fila != LastRaton.Fila ||MyRaton.IBoton != LastRaton.IBoton )
					{
						LastRaton = MyRaton;
						Columna = MyMenuFila.Columna;
						for (i=0; i<=MaxOpciones; i++)
						{
							Columna = Columna + Menu[i].length();
							if (MyRaton.Columna < Columna)
							{
								OpcionRaton = i;
								break;
							}
						}

						if (OpcionRaton > MaxOpciones)	OpcionRaton = MaxOpciones;
						if (OpcionRaton < 0)			OpcionRaton = 0;
						if (OpcionRaton < Opcion) Tecla = CURLEFT;
				   else	if (OpcionRaton > Opcion) Tecla = CURRIGHT;
					}
					else	goto EVENTO;	//No hay TECLA ni cambios en RATON
				}
				else	goto EVENTO;		//No hay TECLA y RATON no esta dentro de la caja
			}
			else	// Ha habido Tecla -----------------------------------------------
			{
				SaltoWhile = 1;		//Ha habido TECLA. Salto el bucle y proceso
			}

			if (MyRaton.IBoton == 1) Tecla = INTRO;

			switch (Tecla)
			{
				//case CURDOWN	:	Direccion  = 1; break;		//Flecha abajo
				case CURLEFT	:	Direccion  =-1; break;		//Flecha izquierda
				case INTRO		:	goto SALIDA;
				case ESC		:	Opcion	  =-1; goto SALIDA;
				case CURRIGHT	:	Direccion  = 1; break;		//Flecha abajo
				default			:	goto EVENTO;
			}

			if (SaltoWhile==1) goto SALTOWHILE;	// Ha habido TECLA, la proceso

			while (OpcionRaton != Opcion )		// Bucle para sincronizar opciones
			{
	SALTOWHILE:
				if (Tecla != INTRO)	//Si no ha validado la opcion
				{
					//Pinta el anterior
					//*********************************************************************
					Fila    = MyMenuFila.Fila + 1;
					Columna = MyMenuFila.Columna;
					for (i=0; i<=Opcion-1; i++) { Columna = Columna + Menu[i].length(); }
					i = Menu[Opcion].length();
					memset(TopeS, 'Ü', i); TopeS[i]=0;
					memset(TopeI, 'ß', i); TopeI[i]=0;

					PintaTxt(Fila-1, Columna, MyMenuFila.PapelNormal, MyMenuFila.Fondo, TopeS);
					PintaTxt(Fila,   Columna, MyMenuFila.TintaNormal, MyMenuFila.PapelNormal, Menu[Opcion]);
					PintaTxt(Fila+1, Columna, MyMenuFila.PapelNormal, MyMenuFila.Fondo, TopeI);

					//Cambia de Opcion y Fila
					//=====================================================================
					Opcion = Opcion + Direccion;
					if (Opcion > MaxOpciones)	{Opcion = 0;			Fila = MyMenuFila.Fila+1;}
					if (Opcion < 0)				{Opcion = MaxOpciones;	Fila = (MaxOpciones*2)+MyMenuFila.Fila+1;}
					Fila    = MyMenuFila.Fila + 1;
					Columna = MyMenuFila.Columna;
					for (i=0; i<=Opcion-1; i++) { Columna = Columna + Menu[i].length(); }
					i = Menu[Opcion].length();
					memset(TopeS, 'Ü', i); TopeS[i]=0;
					memset(TopeI, 'ß', i); TopeI[i]=0;

					PintaTxt(Fila-1, Columna, MyMenuFila.PapelOpcion, MyMenuFila.Fondo, TopeS);
					PintaTxt(Fila,   Columna, MyMenuFila.TintaOpcion, MyMenuFila.PapelOpcion, Menu[Opcion]);
					PintaTxt(Fila+1, Columna, MyMenuFila.PapelOpcion, MyMenuFila.Fondo, TopeI);

				}  // FIN Tecla != INTRO
				if (SaltoWhile==1 && Tecla!=INTRO) goto EVENTO; 
			}	// FIN bucle While de sincronizacion opciones
		}	//FIN Tecla != INTRO
	SALIDA:
		return(Opcion);
	}	//---FIN FUNCION--

	// =============================================================================
	// Boton
	// =============================================================================
	int PintaBoton(int NumBoton, rBoton MyBoton1, rBoton MyBoton2)
	{
		int		LonTxt, Salida, Boton, BotonDefecto, i, j;
		int  	Inicial1, Inicial2;
		rBoton	MyBoton;
		rRaton	MyRaton;
		char	Inicial[2];

		Boton = 0;
		MyRaton.Fila	= 0;
		MyRaton.Columna	= 0;
		MyRaton.IBoton	= 0;
		MyRaton.DBoton	= 0;
		MyRaton.CBoton	= 0;
		MyRaton.TBoton	= 0;
		MyRaton.DClick	= 0;

		Inicial1 = toupper(MyBoton1.Texto[0]);
		Inicial2 = toupper(MyBoton2.Texto[0]);

		for (j=0; j<NumBoton; j++)
		{
			if (j==0) MyBoton=MyBoton1; else MyBoton=MyBoton2;
			LonTxt = strlen(MyBoton.Texto);
			for (i=0; i<MyBoton.Largo; i++)
			{
				PintaTxt (MyBoton.Fila,		MyBoton.Columna+i, MyBoton.Tinta, MyBoton.Papel, "Ü");
				PintaTxt (MyBoton.Fila+1,	MyBoton.Columna+i, MyBoton.Tinta, MyBoton.Papel, "Û");
				PintaTxt (MyBoton.Fila+2,	MyBoton.Columna+i, MyBoton.Tinta, MyBoton.Papel, "ß");
			}

			//Pinta la Sombra
			//==========================================================================
			PintaTxt (MyBoton.Fila+1, MyBoton.Columna+i, MyBoton.Sombra, MyBoton.Papel, "Û");
			PintaTxt (MyBoton.Fila+2, MyBoton.Columna+i, MyBoton.Sombra, MyBoton.Papel, "Û");
			for (i=0; i<MyBoton.Largo-1; i++)
			PintaTxt (MyBoton.Fila+2, MyBoton.Columna+1+i, MyBoton.Tinta, MyBoton.Sombra, "ß");
			//==========================================================================

			PintaTxt (MyBoton.Fila+1, MyBoton.Columna+(MyBoton.Largo-LonTxt)/2, MyBoton.Papel, MyBoton.Tinta, MyBoton.Texto);
		}

		BotonDefecto = 1;
		memset(Inicial, Inicial1, 1); Inicial[1]=0;
		PintaTxt (MyBoton1.Fila+1, MyBoton1.Columna+(MyBoton1.Largo-LonTxt)/2, BLANCOB, MyBoton1.Tinta, Inicial);

		if (NumBoton==2)
		{
			memset(Inicial, Inicial2, 1); Inicial[1]=0;
			PintaTxt (MyBoton2.Fila+1, MyBoton2.Columna+(MyBoton2.Largo-LonTxt)/2, NEGRO, MyBoton2.Tinta, Inicial);
		}

		//================================================================================
		while (Boton != Inicial1 && Boton != Inicial2)
		{
			MyRaton.Fila	= 0; MyRaton.Columna	= 0; MyRaton.IBoton = 0;
			MyRaton.DBoton	= 0; MyRaton.CBoton	= 0; MyRaton.TBoton = 0;
			MyRaton.DClick	= 0;
			EsperaEvento(Boton, MyRaton);

			//Miramos si es teccla de control
			//===============================
			switch (Boton)
			{
				case	INTRO:
					if (BotonDefecto==1)	Boton = Inicial1;
					else					Boton = Inicial2;				
					break;
				case	TAB:				Boton = CURLEFT;
				case	CURLEFT:
					if (NumBoton==2)		BotonDefecto = 3-BotonDefecto;
					Boton = 0;
					break;
				case	CURRIGHT:
					if (NumBoton==2)		BotonDefecto = 3-BotonDefecto;
					Boton = 0;
					break;
			}

			if (NumBoton==2)
			{
				if (RatonInCaja(MyBoton1.Fila, MyBoton1.Columna, MyBoton1.Largo, 3, MyRaton)) BotonDefecto = 1;
				if (RatonInCaja(MyBoton2.Fila, MyBoton2.Columna, MyBoton2.Largo, 3, MyRaton)) BotonDefecto = 2;
				if (BotonDefecto==1)
				{
					memset(Inicial, Inicial2, 1); Inicial[1]=0;
					PintaTxt (MyBoton2.Fila+1, MyBoton2.Columna+(MyBoton2.Largo-LonTxt)/2, NEGRO,   MyBoton2.Tinta, Inicial);
					memset(Inicial, Inicial1, 1); Inicial[1]=0;
					PintaTxt (MyBoton1.Fila+1, MyBoton1.Columna+(MyBoton1.Largo-LonTxt)/2, BLANCOB, MyBoton1.Tinta, Inicial);
				}
				else
      			{
					memset(Inicial, Inicial1, 1); Inicial[1]=0;
					PintaTxt (MyBoton1.Fila+1, MyBoton1.Columna+(MyBoton1.Largo-LonTxt)/2, NEGRO,   MyBoton1.Tinta, Inicial);
					memset(Inicial, Inicial2, 1); Inicial[1]=0;
					PintaTxt (MyBoton2.Fila+1, MyBoton2.Columna+(MyBoton2.Largo-LonTxt)/2, BLANCOB, MyBoton2.Tinta, Inicial);
				}
			}
			Boton = toupper(Boton);
		
			if (MyRaton.IBoton == 1)	//Si ha hecho Click Izquierdo
			{
				// Si está en el Boton1
				if 	(RatonInCaja(MyBoton1.Fila, MyBoton1.Columna, MyBoton1.Largo, 3, MyRaton))
					Boton = Inicial1;
				else if (RatonInCaja(MyBoton2.Fila, MyBoton2.Columna, MyBoton2.Largo, 3, MyRaton))
					Boton = Inicial2;
			}
		}

		if 	(Boton==Inicial1)	{Salida=1; MyBoton=MyBoton1;}
		else					{Salida=2, MyBoton=MyBoton2;}

		PintaTxt (MyBoton.Fila,   MyBoton.Columna, MyBoton.Tinta, MyBoton.Papel, " ");
		PintaTxt (MyBoton.Fila+1, MyBoton.Columna, MyBoton.Tinta, MyBoton.Papel, " ");
		PintaTxt (MyBoton.Fila+2, MyBoton.Columna, MyBoton.Tinta, MyBoton.Papel, " ");

		for (i=0; i<MyBoton.Largo; i++)
		{
			PintaTxt (MyBoton.Fila,   MyBoton.Columna+1+i, MyBoton.Tinta, MyBoton.Papel, "Ü");
			PintaTxt (MyBoton.Fila+1, MyBoton.Columna+1+i, MyBoton.Tinta, MyBoton.Papel, "Û");
			PintaTxt (MyBoton.Fila+2, MyBoton.Columna+1+i, MyBoton.Tinta, MyBoton.Papel, "ß");
		}
		PintaTxt (MyBoton.Fila+1, MyBoton.Columna+1+(MyBoton.Largo-LonTxt)/2, BLANCOB, MyBoton.Tinta, MyBoton.Texto);
		EsperaMiliSec(500);
		Pita(1);
		
		for (j=0; j<NumBoton; j++)
		{
			if (j==0) MyBoton=MyBoton1; else MyBoton=MyBoton2;
			LonTxt = strlen(MyBoton.Texto);
			for (i=0; i<MyBoton.Largo; i++)
			{
				PintaTxt (MyBoton.Fila,		MyBoton.Columna+i, MyBoton.Tinta, MyBoton.Papel, "Ü");
				PintaTxt (MyBoton.Fila+1,	MyBoton.Columna+i, MyBoton.Tinta, MyBoton.Papel, "Û");
				PintaTxt (MyBoton.Fila+2,	MyBoton.Columna+i, MyBoton.Tinta, MyBoton.Papel, "ß");
			}
			//Pinta la Sombra
			//==========================================================================
			PintaTxt (MyBoton.Fila,   MyBoton.Columna+i, MyBoton.Sombra, MyBoton.Papel, " ");
			PintaTxt (MyBoton.Fila+1, MyBoton.Columna+i, MyBoton.Sombra, MyBoton.Papel, "Û");
			PintaTxt (MyBoton.Fila+2, MyBoton.Columna+i, MyBoton.Sombra, MyBoton.Papel, "Û");
			for (i=0; i<MyBoton.Largo-1; i++)
			PintaTxt (MyBoton.Fila+2, MyBoton.Columna+1+i, MyBoton.Tinta, MyBoton.Sombra, "ß");
			//==========================================================================

			PintaTxt (MyBoton.Fila+1, MyBoton.Columna+(MyBoton.Largo-LonTxt)/2, MyBoton.Papel, MyBoton.Tinta, MyBoton.Texto);
		}
		BotonDefecto = 1;
		memset(Inicial, Inicial1, 1); Inicial[1]=0;
		PintaTxt (MyBoton1.Fila+1, MyBoton1.Columna+(MyBoton1.Largo-LonTxt)/2, BLANCOB, MyBoton1.Tinta, Inicial);

		if (NumBoton==2)
		{
			memset(Inicial, Inicial2, 1); Inicial[1]=0;
			PintaTxt (MyBoton2.Fila+1, MyBoton2.Columna+(MyBoton2.Largo-LonTxt)/2, NEGRO, MyBoton2.Tinta, Inicial);
		}

		EsperaMiliSec(500);

		return (Salida);
	}
	// =============================================================================
	// Mensaje
	// =============================================================================
	int	PintaMensaje(int Tipo, string Texto[])
	{
		rCaja		MyCaja;
		rBoton		MyBoton, MyBoton2;
		int			Tinta, Papel, Boton, i;
		string		Icono[4];
		CHAR_INFO	MyBuff[80*50];				//Para preservar lo que habia
		string		Titulo;						//Para el Titulo de la Ventana

		switch (Tipo)
		{
			case 1:	Tinta=VERDEB;	Papel=VERDE;	Titulo="Informaci¢n:";	Boton=1; Icono[0]="  Ü  "; Icono[1]=" ÜÜ  "; Icono[2]="  Û  "; Icono[3]=" ßßß "; break;
			case 2:	Tinta=CYANB;	Papel=CYAN;		Titulo="Cuidado!";		Boton=2; Icono[0]=" ÜÜÜ "; Icono[1]=" ß Û "; Icono[2]="  Ûß "; Icono[3]="  Ü  "; break;
			case 3:	Tinta=LILA;		Papel=MORADO;	Titulo="! Atenci¢n !";	Boton=2; Icono[0]="  Ü  "; Icono[1]="  Û  "; Icono[2]="  ß  "; Icono[3]="  ß  "; break;
			default:Tinta=ROJO;		Papel=GRANATE;	Titulo="!! Alto !!";	Boton=2; Icono[0]=" Ü Ü "; Icono[1]=" Ü Û "; Icono[2]=" Û ß "; Icono[3]=" ß ß "; break;
		}
		MyCaja.Fila 	= 5;
		MyCaja.Columna	= 24;
		MyCaja.Largo 	= 32;
		MyCaja.Alto 	= 15;
		MyCaja.Tinta 	= Tinta;
		MyCaja.Papel 	= Papel;
		MyCaja.Marco	= 0;
		MyCaja.Fondo 	= CYANB;

		GetCaja(MyCaja.Fila, MyCaja.Columna, MyCaja.Largo, MyCaja.Alto, MyBuff);
		
		PintaCaja(MyCaja, Titulo);

		for (i=0; i<4; i++) PintaTxt(MyCaja.Fila+10+i, MyCaja.Columna+1, Papel, Tinta, Icono[i]);
		for (i=0; i<5; i++) PintaTxt(MyCaja.Fila+ 4+i, MyCaja.Columna+1, BLANCOB, Papel, LlenaCen(Texto[i], 30, ' '));
	
		MyBoton.Sombra	= NEGRO;
		MyBoton.Fila	= 16;
		MyBoton.Columna	= 25+7;
		MyBoton.Largo	= 10;
		MyBoton.Tinta	= Tinta;
		MyBoton.Papel	= Papel;
		strcpy_s (MyBoton.Texto, "Aceptar");
		if (Boton==1)
		{
			MyBoton.Columna		= 26+7;
			MyBoton.Largo		= 20;
     		strcpy_s (MyBoton2.Texto, "¦");
		}
		else
		{
			MyBoton2.Sombra		= NEGRO;
			MyBoton2.Fila		= 16;
			MyBoton2.Columna	= 37+7;
			MyBoton2.Largo		= 10;
			MyBoton2.Tinta		= Tinta;
			MyBoton2.Papel		= Papel;
			strcpy_s (MyBoton2.Texto, "Cancelar");
		}

		Boton = PintaBoton(Boton, MyBoton, MyBoton2);
		PutCaja(MyCaja.Fila, MyCaja.Columna, MyCaja.Largo, MyCaja.Alto, MyBuff);
		return(Boton);
	}

	//=================================================================================================
	// BoxGet
	//=================================================================================================
	void BoxGet(rAtrBox &MyBox)
	{
		HANDLE		hStdout			= GetStdHandle(STD_OUTPUT_HANDLE);
		SMALL_RECT	srctReadRect	= {MyBox.Col-1, MyBox.Fil-1, MyBox.Col+MyBox.Lrg-1, MyBox.Fil+MyBox.Alt-1};	//.Left, .Top, .Right, .Bottom
		COORD		coordBufSize	= {MyBox.Lrg, MyBox.Alt};													// Tamaño (Fila, Columna) de chiBuffer .Y .X
		COORD		coordBufCoord	= {0, 0};	
		ReadConsoleOutput( hStdout, MyBox.Buf, coordBufSize, coordBufCoord, &srctReadRect);
	}
	//=================================================================================================
	// BoxPut
	//=================================================================================================
	void BoxPut(rAtrBox MyBox)
	{
		HANDLE		hStdout			= GetStdHandle(STD_OUTPUT_HANDLE);
		SMALL_RECT	srctWriteRect	= {MyBox.Col-1, MyBox.Fil-1, MyBox.Col+MyBox.Lrg-1, MyBox.Fil+MyBox.Alt-1};	//.Left, .Top, .Right, .Bottom
		COORD		coordBufSize	= {MyBox.Lrg, MyBox.Alt};													// Tamaño (Fila, Columna) de chiBuffer
		COORD		coordBufCoord	= {0, 0};																	// Tamaño (Largo, Alto) de chiBuffer
		WriteConsoleOutput( hStdout, MyBox.Buf, coordBufSize, coordBufCoord, &srctWriteRect);
	}
	//=================================================================================================
	// BoxDel. Borra un Box con tinta/papel desde fila/columna hasta largo/alto
	//=================================================================================================
	void BoxDel(rAtrBox MyBox)
	{
		CHAR_INFO Buf[80*50];							//Buffer Temporal para preservar el original
		for (int i=0; i<MyBox.Lrg*MyBox.Alt; i++)
		{
			Buf[i].Char.AsciiChar = ' ';
			Buf[i].Attributes     = MyBox.Tnt + MyBox.Ppl*16;
		}
		HANDLE		hStdout			= GetStdHandle(STD_OUTPUT_HANDLE);
		SMALL_RECT	srctWriteRect	= {MyBox.Col-1, MyBox.Fil-1, MyBox.Col+MyBox.Lrg-1, MyBox.Fil+MyBox.Alt-1};	//.Left, .Top, .Right, .Bottom
		COORD		coordBufSize	= {MyBox.Lrg, MyBox.Alt};													// Tamaño (Fila, Columna) de chiBuffer
		COORD		coordBufCoord	= {0, 0};																	// Tamaño (Largo, Alto) de chiBuffer
		WriteConsoleOutput( hStdout, Buf, coordBufSize, coordBufCoord, &srctWriteRect);
	}
	//=================================================================================================
	// BoxCpy. Copia un Box en otro
	//=================================================================================================
	void BoxCpy(rAtrBox BoxOrig, rAtrBox &BoxDest)
	{
		BoxDest.Fil = BoxOrig.Fil;
		BoxDest.Col = BoxOrig.Col;
		BoxDest.Lrg = BoxOrig.Lrg;
		BoxDest.Alt = BoxOrig.Alt;
		BoxDest.Tnt = BoxOrig.Tnt;
		BoxDest.Ppl = BoxOrig.Ppl;
		for (int i=0; i<BoxOrig.Lrg*BoxOrig.Alt; i++)
		{
			BoxDest.Buf[i].Char.AsciiChar = BoxOrig.Buf[i].Char.AsciiChar;
			BoxDest.Buf[i].Attributes     = BoxOrig.Buf[i].Attributes;
		}
	}
	//=================================================================================================
	// BoxMov. Mueve un Box desde fila/columna - largo/altoa una nueva posicion.
	//=================================================================================================
	void BoxMov(rAtrBox MyBox, rCoor Origen, int Modo)
	{
		rAtrBox		BoxAux;
		rCoor		Destino;
	
		Destino.Fil = MyBox.Fil; Destino.Col = MyBox.Col;		//Preservo la Coordenadas originales de MyBox
		BoxCpy(MyBox, BoxAux);									//Copio MyBox en Auxiliar para igualar atributos
		BoxAux.Fil = Origen.Fil;	BoxAux.Col = Origen.Col;	//Situo el Origen en BoxAux
		MyBox.Fil  = Origen.Fil;	MyBox.Col  = Origen.Col;	//Situo el Origen en MyBox
	
		do
		{
			BoxGet(BoxAux);			//Leo lo que hay en Origen
			BoxPut(MyBox);			//Pinto MyBox en el Origen
			Sleep(200);				//Espero un poco...
			BoxPut(BoxAux);			//Recupero lo que había

			if (BoxAux.Fil<Destino.Fil) { BoxAux.Fil++;  MyBox.Fil++;  }	//Aumento Fil de MyBox y BoxAux
			if (BoxAux.Col<Destino.Col)										//Aumento Col de MyBox y BoxAux
			{
				BoxAux.Col+=3; MyBox.Col+=3;
				if (BoxAux.Col>Destino.Col) { BoxAux.Col = Destino.Col; MyBox.Col = Destino.Col; }
			}
		}
		while (MyBox.Fil < Destino.Fil || MyBox.Col < Destino.Col);
	
		BoxPut(MyBox);
	}
	//==============================================================================
	// Funcion GetCaja y PutCaja (Redireccionan. Por mantener compatibilidad con MyBorlandLib)
	//==============================================================================
	void GetCaja (int Fila, int Columna, int Largo, int Alto, CHAR_INFO Buffer[80*50])
	{
		rAtrBox MyBox;
		MyBox.Fil=Fila;
		MyBox.Col=Columna;
		MyBox.Lrg=Largo;
		MyBox.Alt=Alto;
		MyBox.Tnt=NEGRO;
		MyBox.Ppl=NEGRO;
		BoxGet(MyBox);
		for (int i=0; i<MyBox.Lrg*MyBox.Alt; i++)
		{Buffer[i].Char.AsciiChar= MyBox.Buf[i].Char.AsciiChar; Buffer[i].Attributes = MyBox.Buf[i].Attributes;}
	}
	//==============================================================================
	void PutCaja (int Fila, int Columna, int Largo, int Alto, CHAR_INFO Buffer[80*50])
	{
		rAtrBox MyBox;
		MyBox.Fil=Fila;
		MyBox.Col=Columna;
		MyBox.Lrg=Largo;
		MyBox.Alt=Alto;

		for (int i=0; i<MyBox.Lrg*MyBox.Alt; i++)
		{ MyBox.Buf[i].Char.AsciiChar = Buffer[i].Char.AsciiChar; MyBox.Buf[i].Attributes = Buffer[i].Attributes; }
		BoxPut(MyBox);
	}
	
	// =============================================================================
	// Funcion GetTxt
	// =============================================================================
	int GetTxt(rAtrDat &Dato, rAtrPnt Pnt, rAtrCpt Cpt, rCpyPst CpyPst)
	{
		rRaton	MyRaton;
		unsigned int	Col = Pnt.Col;					//Columna para pintar el Dato
		unsigned int	CursorPos;						//Posicion del cursor
		int				Tecla = 0;
		int				Salida = 0;						//Validacion de Salida
		int				Valida = true;
		int				ModoCursor = NORMALCURSOR;
		bool			Bucle;							//Fin de captura
		bool			AutoSkip = (Cpt.Vld && V_AUTOSKIP == V_AUTOSKIP);
		bool			Insert = true;
		size_t			Found;
		string			DatoTmp = Dato.Dat;				//Trabajamos con DatoTmp
		string			DatoPass = Dato.Dat;
		string			Mascara = "";

		if (Dato.Lon < 1) {Pita(5); return -1;}

		Mascara.assign(Dato.Lon, char(250));		//
	
		if (Cpt.Msk.length() > 1) { Mascara = "[" + Mascara + "]"; Col++; }	//Cpt.Msk
		BorraTxt(Pnt.Fil, Pnt.Col, Cpt.Ppl, Mascara.length());			//Borra lo que hubiera con el color del papel del atributo.
		PintaTxt(Pnt.Fil, Pnt.Col, Cpt.Tnt, Cpt.Ppl, Mascara);			//Pinta la mascara.
		PintaTxt(Pnt.Fil, Col, Cpt.Tnx, Cpt.Ppl, Dato.Dat);				//Pinta el dato por defecto
		CursorPos = Col + Dato.Dat.length();
		do						//Mientras el dato sea inválido
		{
			Bucle = true;
			while (Bucle)		//Mientras no se valide o autoskip
			{	
				Cursor(ModoCursor);
				Locate(Pnt.Fil, CursorPos);
			
				MyRaton.IBoton = 0; MyRaton.DBoton = 0; MyRaton.CBoton = 0; MyRaton.TBoton = 0;
				MyRaton.Fila   = 0; MyRaton.Columna= 0; MyRaton.DClick = 0;
				Tecla=-1;
				while (Tecla == -1)
				{
					EsperaEvento(Tecla, MyRaton);		//Si el evento es RATON, espera hasta TECLADO
					if		(MyRaton.IBoton) Tecla=INTRO;
					else if (MyRaton.DBoton) Tecla=ESC;
				}

				switch (Tecla)
				{
				case SHIFT:		break;
				case CTRL:		break;
				case ALT:		break;
				case PAUSA:		break;
				case MAYUS:		break;
				case BLQNUM:	break;
				case BLQDSPL:	break;
				case 1222:		break;	//acento
				case 1186:		break;

				case INTRO:		if (Cpt.Vld && V_INTRO == V_INTRO)		{ Salida = V_INTRO;		Bucle = false; }	break;	//Si está permitida... fin bucle y sale
				case CURDOWN:	if (Cpt.Vld && V_CURDOWN == V_CURDOWN)	{ Salida = V_CURDOWN;	Bucle = false; }	break;	//Si está permitida... fin bucle y sale
				case TAB:		if (Cpt.Vld && V_TAB == V_TAB)			{ Salida = V_TAB;		Bucle = false; }	break;	//Si está permitida... fin bucle y sale
				case AVPAG:		if (Cpt.Vld && V_AVPAG == V_AVPAG)		{ Salida = V_AVPAG;		Bucle = false; }	break;	//Si está permitida... fin bucle y sale
				case ESC: 		if (Cpt.Vld && V_ESCAPE == V_ESCAPE)	{ Salida = V_ESCAPE;	Bucle = false; }	break;	//Si está permitida... fin bucle y sale
				case CURUP:		if (Cpt.Vld && V_CURUP == V_CURUP)		{ Salida = V_CURUP;		Bucle = false; }	break;	//Si está permitida... fin bucle y sale
				case C_TAB:		if (Cpt.Vld && V_CTRLTAB == V_CTRLTAB)	{ Salida = V_CTRLTAB;	Bucle = false; }	break;	//Si está permitida... fin bucle y sale
				case REPAG:		if (Cpt.Vld && V_REPAG == V_REPAG)		{ Salida = V_REPAG;		Bucle = false; }	break;	//Si está permitida... fin bucle y sale
				case CURRIGHT:	if (CursorPos-Col < DatoTmp.length()) CursorPos++;	break; //Cursor Right
				case CURLEFT:	if (CursorPos-Col > 0) CursorPos--;					break;
				case INICIO:	CursorPos = Col;									break;
				case FIN:		CursorPos = Col + DatoTmp.length();					break;
				case CINCO:															break;	//Se podria situar en el espacio siguiente ciclico
				case INSERT:	Insert = !Insert; ModoCursor = 3 - ModoCursor;		break;	//Balancea entre SOLIDCURSOR y NORMALCURSOR
				case BACKSPACE:	if (CursorPos-Col > 0)					
								{
									if (CpyPst.Permitido) DatoPass = DatoPass.substr(0, CursorPos-Col-1) + DatoPass.substr(CursorPos-Col);
									DatoTmp = DatoTmp.substr(0, CursorPos-Col-1) + DatoTmp.substr(CursorPos-Col);
									CursorPos--;
									PintaTxt(Pnt.Fil, Pnt.Col, Cpt.Tnt, Cpt.Ppl, Mascara);
									if (CpyPst.Permitido) PintaTxt(Pnt.Fil, Col, Cpt.Tnx, Cpt.Ppl, DatoPass);
									else PintaTxt(Pnt.Fil, Col, Cpt.Tnx, Cpt.Ppl, DatoTmp);

								}	break;	//Back Space
				case SUPR:		if (CursorPos-Col < DatoTmp.length())
								{
									if (CpyPst.Permitido) DatoPass = DatoPass.substr(0, CursorPos-Col) + DatoPass.substr(CursorPos-Col+1);
									DatoTmp = DatoTmp.substr(0, CursorPos-Col) + DatoTmp.substr(CursorPos-Col+1);
									PintaTxt(Pnt.Fil, Pnt.Col, Cpt.Tnt, Cpt.Ppl, Mascara);
									if (CpyPst.Permitido) PintaTxt(Pnt.Fil, Col, Cpt.Tnx, Cpt.Ppl, DatoPass);
									else PintaTxt(Pnt.Fil, Col, Cpt.Tnx, Cpt.Ppl, DatoTmp);
								}	break;	//Suprimir
				default:
					Found = Cpt.Cnj.find(char(Tecla));
					if (Found==string::npos) Pita(1);				//Control de si esta en el conjunto permitido;
					else
					{
						if (Insert)
						{
							if (DatoTmp.length() == Mascara.length()-2) Pita(1);
							else
							{
								DatoTmp.insert(CursorPos-Col, 1, char(Tecla));
								if (CpyPst.Permitido) DatoPass.insert(CursorPos-Col, 1, char(42));
							}
						}
						else
						{
							if (CursorPos-Col < DatoTmp.length()) DatoTmp = DatoTmp.substr(0, CursorPos-Col) + char(Tecla) + DatoTmp.substr(CursorPos-Col + 1);
							else if (DatoTmp.length() == Mascara.length()-2) Pita(1);
							else
							{
								DatoTmp.insert(CursorPos-Col, 1, char(Tecla));
								if (CpyPst.Permitido) DatoPass.insert(CursorPos-Col, 1, char(42));
							}
						}
						if (CursorPos-Col < DatoTmp.length()) CursorPos++;
						PintaTxt(Pnt.Fil, Pnt.Col, Cpt.Tnt, Cpt.Ppl, Mascara);
						if (CpyPst.Permitido) PintaTxt(Pnt.Fil, Col, Cpt.Tnx, Cpt.Ppl, DatoPass);
						else PintaTxt(Pnt.Fil, Col, Cpt.Tnx, Cpt.Ppl, DatoTmp);
					}
													
					break;								//Mete el caracter en el DatoTmp
														//Actualiza Mascara
														//Actualiza Cursorpos
				}		//Fin del switch
			
			}			//Fin de while(Bucle)

			BorraTxt(Pnt.Fil, Pnt.Col, Pnt.Ppl, Mascara.length());
			if (CpyPst.Permitido) PintaTxt(Pnt.Fil, Col, Pnt.Tnt, Pnt.Ppl, DatoPass);
			else PintaTxt(Pnt.Fil, Col, Pnt.Tnt, Pnt.Ppl, DatoTmp);
			
		}while(!AutoSkip);		//Fin del do
			
		// Sale de la captura
		//==============================================================================
		Dato.Dat = DatoTmp;		//¿Rellenamos con el caracter por el lado segun AtrDat?
		return Salida;
	}

	//======================================================================
	// CapsLock - NumLock - Insert - Scroll - Control - Shift - on/off
	//======================================================================
	bool BlqMayON()  { return (GetKeyState(VK_CAPITAL) & 0x0001) == 0x0001; }
	bool BlqNumON()  { return (GetKeyState(VK_NUMLOCK) & 0x0001) == 0x0001; }
	bool InsertON()  { return (GetKeyState(VK_INSERT ) & 0x0001) == 0x0001; }
	bool ScrollON()  { return (GetKeyState(VK_SCROLL ) & 0x0001) == 0x0001; }
	bool ControlON() { return (GetKeyState(VK_CONTROL) & 0x0001) == 0x0001; }
	bool ShiftON()   { return (GetKeyState(VK_SHIFT  ) & 0x8000) == 0x8000; }

	// =============================================================================
	// Quick Sort
	//==============================================================================
	void QuickSort(int Array[], int Left, int Right)
	{
		int NewLeft		= Left;							//Left  = indice mínimo = 0
		int NewRight	= Right;						//Right = indice maximo
		int Pivote		= Array[(Left + Right) / 2];
  
		// particion
		while (NewLeft <= NewRight)
		{
			while (Array[NewLeft]	< Pivote) NewLeft++;
			while (Array[NewRight]	> Pivote) NewRight--;
			if (NewLeft <= NewRight)
			{
				swap (Array[NewLeft], Array[NewRight]);
				NewLeft++;
				NewRight--;
			}
		}
  
		// recursion
		if (Left < NewRight)	QuickSort(Array, Left, NewRight);
		if (NewLeft < Right)	QuickSort(Array, NewLeft, Right);
	}

/*=============================================================================
|     Rutina: MatSel.cpp                                                      |
|       Tipo: int - Retorna Salida, el valor de validacion                    |
|    Llamada: MatSel(Matriz, rCajaFondo, Elemento)                            |
|                                                                             | 
|Entrada --> Matriz[]    Matriz a seleccionar                                 | 
|            CajaFonfo.Fila    Fila de la ventana                             | 
|            CajaFondo.Columna Columna de la ventana                          | 
|            CajaFondo.Largo   Nº de Columnas de ventana de seleccion         | 
|            CajaFondo.Alto    Nº de Filas de la ventana de seleccion         | 
|            CajaFondo.Tinta                                                  |
|            CajaFondo.Papel                                                  |
|            CajaFondo.Fondo                                                  |
|            CajaFondo.Marco                                                  | 
|            Elemento         Nº de elementos en Matriz                       |
|Salida  --> Elemento   Elemento elegido                                      | 
|            Salida     Tecla de validacion 0=[Intro] 1=[Esc]                 | 
|                                                                             | 
|       Nota: Se controla el desbordamiento de pantalla                       | 
=============================================================================*/
int MatSel (string Tabla[], rCaja CajaFondo, int &Elemento, int ES)
{
	#define	VALCAMPO	1
	#define NOVALCAMPO	0

	int		Puntero;
	int		Veces;
	int		Fil;
	int		Tinta;
	int		Papel;
	int		Salida		= -1;										//Para que se embucle al iniciar hasta VALCAMPO=1, NOVALCAMPO=0
	int		NumElem		= Elemento;									//En Elemento retorno el elegido, así que trabajo con otra var
	int		Tecla		= -1;
	int		InicioFin	= 0;										//0--> va a INICIO, 1--> va a FIN
	int		BotonActivo = 6;
	int		BotonAnterior=0;
	
	string	Aux;
	rRaton	MyRaton;
	
	string sBandeja;
	
	if (ES==-1)	sBandeja="Salida ";
	else		sBandeja="Entrada";
	Elemento = 1;

	//Controles de desbordamiento de pantalla y elementos de entrada
	//if (NumElem < 1)								{ Elemento = 0; return -2; }				// Si no hay elementos...
	if (CajaFondo.Largo < 17)						CajaFondo.Largo		= 17;					// Tiene que haber un Minimo de Largo
	if (CajaFondo.Largo > 74)						CajaFondo.Largo		= 74;					// Tiene que haber un Maximo de Largo
	if (CajaFondo.Alto  < 5 )						CajaFondo.Alto		= 5;					// Tiene que haber un Minimo de Alto
	if (CajaFondo.Alto  > 44)						CajaFondo.Alto		= 44;					// Tiene que haber un Maximo de Alto (pant. de 50)
	if (CajaFondo.Fila + CajaFondo.Alto > 44)		CajaFondo.Fila		= 44 - CajaFondo.Alto;	// Si la Fila y el alto desborda... corrijo la Fila
	if (CajaFondo.Columna + CajaFondo.Largo > 75)	CajaFondo.Columna	= 75 - CajaFondo.Largo;	// Si la Columna y el Largo desborda... corrijo la Columna
	
	int Min				= CajaFondo.Alto / 2;
	int Max				= CajaFondo.Alto / 2 + 1;
	int LenElem			= Tabla[Elemento].length();   				// Longit£d de los elementos
	
	rAtrBox	MyBox;													// Para restaurar lo que habia
	MyBox.Fil			= CajaFondo.Fila-1;
	MyBox.Col			= CajaFondo.Columna;
	MyBox.Lrg			= CajaFondo.Largo + 6;						// Para que quepan los botones y la ventana interior
	MyBox.Alt			= CajaFondo.Alto + 6;
	MyBox.Tnt			= CajaFondo.Tinta;
	MyBox.Ppl			= CajaFondo.Papel;
	
	rCaja	MyCaja;
	MyCaja.Marco		= 0;
	MyCaja.Fila			= CajaFondo.Fila;
	MyCaja.Columna		= CajaFondo.Columna;
	MyCaja.Largo		= CajaFondo.Largo + 6;
	MyCaja.Alto			= CajaFondo.Alto + 6;
	MyCaja.Tinta		= CajaFondo.Papel;
	MyCaja.Papel		= CajaFondo.Tinta;
	MyCaja.Fondo		= CajaFondo.Fondo;

	rCaja	BotonSube;
	BotonSube.Marco		= 1;
	BotonSube.Fila		= CajaFondo.Fila + 1;
	BotonSube.Columna	= CajaFondo.Columna + CajaFondo.Largo + 2;
	BotonSube.Largo		= 3;
	BotonSube.Alto		= Min;
	BotonSube.Tinta		= CajaFondo.Tinta;
	BotonSube.Papel		= CajaFondo.Fondo;
	BotonSube.Fondo		= CajaFondo.Tinta;
	
	rCaja	BotonBaja;
	BotonBaja.Marco		= 1;
	BotonBaja.Alto		= CajaFondo.Alto - Min - 1;
	BotonBaja.Largo		= 3;
	BotonBaja.Fila		= CajaFondo.Fila + CajaFondo.Alto - BotonBaja.Alto + 1;
	BotonBaja.Columna	= CajaFondo.Columna + CajaFondo.Largo + 2;
	BotonBaja.Tinta		= CajaFondo.Tinta;
	BotonBaja.Papel		= CajaFondo.Fondo;
	BotonBaja.Fondo		= CajaFondo.Tinta;
	
	rCaja	BotonESC;	//Finalizar Sesión
	BotonESC.Marco		= 1;
	BotonESC.Fila		= CajaFondo.Fila + CajaFondo.Alto + 2;
	BotonESC.Columna	= CajaFondo.Columna + 1;
	BotonESC.Largo		= 11;	//"Finalizar"
	BotonESC.Alto		= 3;
	BotonESC.Tinta		= CajaFondo.Tinta;
	BotonESC.Papel		= CajaFondo.Fondo;
	BotonESC.Fondo		= CajaFondo.Tinta;
	
	rCaja	BotonBandeja;	//"Salida "-"Entrada"
	BotonBandeja.Marco	= 1;
	BotonBandeja.Fila	= CajaFondo.Fila + CajaFondo.Alto + 2;
	BotonBandeja.Largo	= 9;
	BotonBandeja.Alto	= 3;
	BotonBandeja.Columna= CajaFondo.Columna + BotonESC.Largo + 2 + 8;	//Bandeja:
	BotonBandeja.Tinta	= CajaFondo.Tinta;
	BotonBandeja.Papel	= CajaFondo.Fondo;
	BotonBandeja.Fondo	= CajaFondo.Tinta;

	rCaja	BotonOrdenar;	//"Ordenar"
	BotonOrdenar.Marco	= 1;
	BotonOrdenar.Fila	= CajaFondo.Fila + CajaFondo.Alto + 2;
	BotonOrdenar.Largo	= 9;
	BotonOrdenar.Alto	= 3;
	BotonOrdenar.Columna= CajaFondo.Columna + BotonBandeja.Columna + BotonBandeja.Largo-1;
	BotonOrdenar.Tinta	= CajaFondo.Tinta;
	BotonOrdenar.Papel	= CajaFondo.Fondo;
	BotonOrdenar.Fondo	= CajaFondo.Tinta;

	rCaja	BotonEnviar;
	BotonEnviar.Marco	= 1;
	BotonEnviar.Fila	= CajaFondo.Fila + CajaFondo.Alto + 2;
	BotonEnviar.Largo	= 7;
	BotonEnviar.Alto	= 3;
	BotonEnviar.Columna	= CajaFondo.Columna + BotonOrdenar.Columna + 19;	//Correo:
	BotonEnviar.Tinta	= CajaFondo.Tinta;
	BotonEnviar.Papel	= CajaFondo.Fondo;
	BotonEnviar.Fondo	= CajaFondo.Tinta;

	rCaja	BotonBorrar;
	BotonBorrar.Marco	= 1;
	BotonBorrar.Fila	= CajaFondo.Fila + CajaFondo.Alto + 2;
	BotonBorrar.Largo	= 10;
	BotonBorrar.Alto	= 3;
	BotonBorrar.Columna	= CajaFondo.Columna + BotonEnviar.Columna + BotonEnviar.Largo-1;
	BotonBorrar.Tinta	= CajaFondo.Tinta;
	BotonBorrar.Papel	= CajaFondo.Fondo;
	BotonBorrar.Fondo	= CajaFondo.Tinta;

	rCaja	BotonOK;		//Leer
	BotonOK.Marco		= 1;
	BotonOK.Fila		= CajaFondo.Fila + CajaFondo.Alto + 2;
	BotonOK.Largo		= 6;
	BotonOK.Alto		= 3;
	BotonOK.Columna		= CajaFondo.Columna + CajaFondo.Largo - BotonOK.Largo + 5;
	BotonOK.Tinta		= CajaFondo.Tinta;
	BotonOK.Papel		= CajaFondo.Fondo;
	BotonOK.Fondo		= CajaFondo.Tinta;

	rCaja	BtnActivo;	//Al inicio es el botonOK
	BtnActivo.Marco		= 1;
	BtnActivo.Fila		= CajaFondo.Fila + CajaFondo.Alto + 2;
	BtnActivo.Largo		= 13;
	BtnActivo.Alto		= 3;
	BtnActivo.Columna	= CajaFondo.Columna + CajaFondo.Largo - BotonOK.Largo + 5;
	BtnActivo.Tinta		= AMARILLO;
	BtnActivo.Papel		= AZULB;
	BtnActivo.Fondo		= CajaFondo.Tinta;

	BoxGet(MyBox);												//Antes de Pintar, Lee lo que habia

	PintaCaja(MyCaja,"");
	PintaCaja(BotonSube, "");
	PintaCaja(BotonBaja, "");
	
	PintaCaja(BotonESC, "");
	PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3, CajaFondo.Columna + BotonESC.Largo + 2, BLANCOB, VERDE, "Bandeja:");
	PintaCaja(BotonBandeja, "");
	PintaCaja(BotonOrdenar, "");
	PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3, CajaFondo.Columna + BotonESC.Largo + BotonBandeja.Largo + BotonOrdenar.Largo + 16, BLANCOB, VERDE, "Correo:");
	PintaCaja(BotonEnviar, "");
	PintaCaja(BotonBorrar, "");
	PintaCaja(BotonOK, "");
	
	PintaChr(CajaFondo.Fila+2,					CajaFondo.Columna+CajaFondo.Largo+3,	CajaFondo.Tinta, CajaFondo.Fondo, char(30));
	PintaChr(CajaFondo.Fila+Min+1,				CajaFondo.Columna+CajaFondo.Largo+2,	CajaFondo.Papel, CajaFondo.Tinta, char(17));
	PintaChr(CajaFondo.Fila+Min+1,				CajaFondo.Columna+CajaFondo.Largo+3,	CajaFondo.Papel, CajaFondo.Tinta, char(205));
	PintaChr(CajaFondo.Fila+CajaFondo.Alto-1,	CajaFondo.Columna+CajaFondo.Largo+3,	CajaFondo.Tinta, CajaFondo.Fondo, char(31));
	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	CajaFondo.Columna+2,					CajaFondo.Papel, CajaFondo.Fondo, "Finalizar");
	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	CajaFondo.Columna+2,					CajaFondo.Tinta, CajaFondo.Fondo, "F");
	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonBandeja.Columna+1,					CajaFondo.Papel, CajaFondo.Fondo, sBandeja);
	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonBandeja.Columna+1,					CajaFondo.Tinta, CajaFondo.Fondo, sBandeja.substr(0, 1));
	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonOrdenar.Columna+1,					CajaFondo.Papel, CajaFondo.Fondo, "Ordenar");
	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonOrdenar.Columna+1,					CajaFondo.Tinta, CajaFondo.Fondo, "O");
	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonEnviar.Columna+1,					CajaFondo.Papel, CajaFondo.Fondo, "Nuevo");
	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonEnviar.Columna+1,					CajaFondo.Tinta, CajaFondo.Fondo, "N");
	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonBorrar.Columna+1,					CajaFondo.Papel, CajaFondo.Fondo, "Eliminar");
	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonBorrar.Columna+1,					CajaFondo.Tinta, CajaFondo.Fondo, "E");
	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonOK.Columna+1,						CajaFondo.Papel, CajaFondo.Fondo, "Leer");
	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonOK.Columna+1,						CajaFondo.Tinta, CajaFondo.Fondo, "L");

	rAtrBox	MyBoxDel;
	MyBoxDel.Tnt=CajaFondo.Papel;
	
	if (Elemento > Max)	{ Puntero = Elemento-Min;	Fil = CajaFondo.Fila + 1; }					
	else				{ Puntero = 1;				Fil = CajaFondo.Fila + Max - Elemento + 1; }
	
	if (ES==-1)	PintaTxt(CajaFondo.Fila-1, 3, NEGRO, CYANB, " L De:                          Asunto:                       Fecha:"); //Bandeja Salida
	else		PintaTxt(CajaFondo.Fila-1, 3, NEGRO, CYANB, " L Para:                        Asunto:                       Fecha:"); //Bandeja Entrada
	

//------------------------------------------------------------------------------
	do 
	{
		//Determinaci¢n del n£mero de Filas a pintar
		//------------------------------------------
		if (Puntero == 1 && Fil == CajaFondo.Fila+1 )	Veces = CajaFondo.Alto;
		else if (Puntero == 1)							Veces = CajaFondo.Alto - Fil + CajaFondo.Fila + 1;
		else											Veces = NumElem - Puntero + 1;
		if (Veces > CajaFondo.Alto)						Veces = CajaFondo.Alto;
		if (Veces > NumElem)							Veces = NumElem;

		//Si se debe borrar una Fila superior...
		//--------------------------------------
		if (Fil > CajaFondo.Fila + 1)
		{
			MyBoxDel.Fil = CajaFondo.Fila + 1;			//MyBoxDel.Fil = FilBox+1;
			MyBoxDel.Col = CajaFondo.Columna + 1;		//MyBoxDel.Col = ColBox+1;
			MyBoxDel.Lrg = CajaFondo.Largo;
			MyBoxDel.Alt = Fil - CajaFondo.Fila - 1;
			MyBoxDel.Ppl = CajaFondo.Papel;
			BoxDel(MyBoxDel);
		}

		for (int i=0; i<=Veces-1; i++)
		{
			if (Fil + i == CajaFondo.Fila + Max)	{ Tinta = CajaFondo.Papel; Papel = CajaFondo.Fondo; }
			else									{ Tinta = CajaFondo.Fondo; Papel = CajaFondo.Papel ; }
			PintaTxt( Fil + i , CajaFondo.Columna + 1, Tinta, Papel, Tabla[Puntero + i].substr(0, CajaFondo.Largo) );
		}

		//Si se deben borrar Filas inferiores...
		//--------------------------------------
		if (Fil - CajaFondo.Fila + Veces < CajaFondo.Alto + 1)
		{
			MyBoxDel.Fil = Fil + Veces;
			MyBoxDel.Col = CajaFondo.Columna + 1;
			MyBoxDel.Lrg = CajaFondo.Largo;
			MyBoxDel.Alt = CajaFondo.Alto - Fil + CajaFondo.Fila - Veces + 1;
			MyBoxDel.Ppl = CajaFondo.Papel;
			BoxDel(MyBoxDel);
		}
		//=========================================================================================
		
		if (Fil > CajaFondo.Fila + 1)	Elemento = Puntero + Min + CajaFondo.Fila - Fil + 1;
		else							Elemento = Puntero + Min;
		
		MyRaton.Fila	= 0;	MyRaton.Columna	= 0;	MyRaton.IBoton	= 0;
		MyRaton.DBoton	= 0;	MyRaton.CBoton	= 0;	MyRaton.TBoton	= 0;
		MyRaton.DClick	= 0;	MyRaton.Rueda	= 0;	
		Tecla=0;

		//Pinta El Boton Activo
		BotonOK.Tinta=BtnActivo.Tinta;		BotonOK.Papel=BtnActivo.Papel;
		BotonESC.Tinta=BtnActivo.Tinta;		BotonESC.Papel=BtnActivo.Papel;
		BotonBandeja.Tinta=BtnActivo.Tinta;	BotonBandeja.Papel=BtnActivo.Papel;
		BotonOrdenar.Tinta=BtnActivo.Tinta;	BotonOrdenar.Papel=BtnActivo.Papel;
		BotonEnviar.Tinta=BtnActivo.Tinta;	BotonEnviar.Papel=BtnActivo.Papel;
		BotonBorrar.Tinta=BtnActivo.Tinta;	BotonBorrar.Papel=BtnActivo.Papel;
	
		switch (BotonActivo)
		{
		case 6:	PintaCaja(BotonOK, "");		PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonOK.Columna+1,		AMARILLO,	AZULB, "Leer"); 
											PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonOK.Columna+1,		ROJO,		AZULB, "L"); break;
		case 1:	PintaCaja(BotonESC, "");	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonESC.Columna+1,		AMARILLO,	AZULB, "Finalizar"); 
											PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonESC.Columna+1,		ROJO,		AZULB, "F"); break;
		case 2: PintaCaja(BotonBandeja, "");PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonBandeja.Columna+1, AMARILLO,	AZULB, sBandeja);
											PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonBandeja.Columna+1, ROJO,		AZULB, sBandeja.substr(0, 1)); break;
		case 3: PintaCaja(BotonOrdenar, "");PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonOrdenar.Columna+1, AMARILLO,	AZULB, "Ordenar");
											PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonOrdenar.Columna+1, ROJO,		AZULB, "O"); break;
		case 4: PintaCaja(BotonEnviar, "");	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonEnviar.Columna+1,	AMARILLO,	AZULB, "Nuevo");
											PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonEnviar.Columna+1,	ROJO,		AZULB, "N"); break;
		case 5: PintaCaja(BotonBorrar, "");	PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonBorrar.Columna+1,	AMARILLO,	AZULB, "Eliminar");
											PintaTxt(CajaFondo.Fila+CajaFondo.Alto+3,	BotonBorrar.Columna+1,	ROJO,		AZULB, "E"); break;
		}

		EsperaEvento(Tecla, MyRaton);

		//Miramos si ha habido TECLA o RATON
		//------------------------------------
		if (Tecla==-1)	//No hay TECLA, es RATON
		{
			if ( RatonInCaja(MyCaja.Fila, MyCaja.Columna, MyCaja.Largo, MyCaja.Alto, MyRaton) )
			{
				Tecla = (MyRaton.Rueda == 1)*CURUP + (MyRaton.Rueda ==-1)*CURDOWN;
				//if (MyRaton.Rueda  == 1) Tecla = CURUP;  	//Rueda UP
				//if (MyRaton.Rueda  ==-1) Tecla = CURDOWN;	//Rueda DOWN
				if (MyRaton.DClick == 1)
				{
					InicioFin = 1-InicioFin;
					if(InicioFin)	Tecla = FIN*(Elemento!=NumElem) + INICIO*(Elemento==NumElem);	//InicioFin=1
					else			Tecla = INICIO*(Elemento!=1)	+ FIN*(Elemento==1); ;			//InicioFin=0
				}
				
				if (RatonInCaja(BotonSube.Fila, BotonSube.Columna, BotonSube.Largo, BotonSube.Alto, MyRaton))
				{
					if (MyRaton.IBoton == 1)	Tecla = CURUP;
					if (MyRaton.DBoton == 1)	Tecla = REPAG;
				}
				if (RatonInCaja (BotonBaja.Fila, BotonBaja.Columna, BotonBaja.Largo, BotonBaja.Alto, MyRaton))
				{
					if (MyRaton.IBoton == 1)	Tecla = CURDOWN;
					if (MyRaton.DBoton == 1)	Tecla = AVPAG;
				}

				if (RatonInCaja(BotonESC.Fila, BotonESC.Columna, BotonESC.Largo, BotonESC.Alto, MyRaton))
					{if (MyRaton.IBoton==1) {BotonActivo=1; Tecla=INTRO;}}
				if (RatonInCaja(BotonBandeja.Fila, BotonBandeja.Columna, BotonBandeja.Largo, BotonBandeja.Alto, MyRaton))
					{if (MyRaton.IBoton==1) { BotonActivo=2; Tecla=INTRO;}}
				if (RatonInCaja(BotonOrdenar.Fila, BotonOrdenar.Columna, BotonOrdenar.Largo, BotonOrdenar.Alto, MyRaton))
					{if (MyRaton.IBoton==1) { BotonActivo=3; Tecla=INTRO;}}
				if (RatonInCaja(BotonEnviar.Fila, BotonEnviar.Columna, BotonEnviar.Largo, BotonEnviar.Alto, MyRaton))
					{if (MyRaton.IBoton==1) { BotonActivo=4; Tecla=INTRO;}}
				if (RatonInCaja(BotonBorrar.Fila, BotonBorrar.Columna, BotonBorrar.Largo, BotonBorrar.Alto, MyRaton))
					{if (MyRaton.IBoton==1) { BotonActivo=5; Tecla=INTRO;}}
				if (RatonInCaja(BotonOK.Fila, BotonOK.Columna, BotonOK.Largo, BotonOK.Alto, MyRaton))
					{if (MyRaton.IBoton==1) { BotonActivo=6; Tecla=INTRO;}}
			}
		}

		string Mnsj[5];
		switch (Tecla)
		{
		case -1: break;
		case 70: case 102:	Salida=1;	break;							// 'F', 'f' Finalizar Sesion
		case 66: case 98:	Salida=2;	break;							// 'B', 'b' Bandeja de Entrada
		case 79: case 111:	Salida=3;	break;							// 'O'. 'o' Ordenar Bandeja
		case 78: case 110:	Salida=4;	break;							// 'N', 'n' Correo Nuevo
		case 69: case 101:	Salida=5;	break;							// 'E', 'e' Eliminar
		case 76: case 108:	Salida=6;	break;							// 'L', 'l' Leer Correo
		case INTRO:		if (NumElem==0 && BotonActivo>4)							// Si no hay correos
						{
							Mnsj[0]="No hay correos en la bandeja.";
							Mnsj[1]="Acciones posibles:";
							Mnsj[2]="  -> [Correo Nuevo]      ";
							Mnsj[3]="  -> [Cambiar de Bandeja]";
							Mnsj[4]="  -> [Finalizar Sesion]  ";
							PintaMensaje(1, Mnsj);
						}
						else
						{
							Salida = BotonActivo;
						}				
						break;				// [INTRO]
		case ESC:		Salida=1; break;	// [ESC] sale como si hubiera pulsado boton finalizar sesión
		case CURRIGHT:	BotonAnterior=BotonActivo; BotonActivo++; if (BotonActivo>6) BotonActivo=1; break;
		case CURLEFT:	BotonAnterior=BotonActivo; BotonActivo--; if (BotonActivo<1) BotonActivo=6; break;
		case CURDOWN:	if (Fil - CajaFondo.Fila + NumElem - Max - 1 != 0)
						{
							if (Fil == CajaFondo.Fila + 1)			Puntero = Puntero + 1;
							if (Fil >  CajaFondo.Fila + 1)			Fil		= Fil - 1;
							if (NumElem < Max)						Puntero = 1;
							else if (Puntero > NumElem - Max + 1)	Puntero = NumElem - Max + 1;
						}
						break;
		case CURUP:		Puntero = Puntero - 1;
						if (Puntero < 1)
						{
							Puntero = 1;
							Fil		= Fil + 1;
							if (Fil-CajaFondo.Fila > Max) Fil = Max + CajaFondo.Fila;
						}
						break;
		case FIN:		Puntero = NumElem - Min;
						if (Puntero < 1)	Puntero	= 1;
						if (NumElem < Max)	Fil		= 1 + CajaFondo.Fila + Max - NumElem;
						else				Fil		= 1 + CajaFondo.Fila;
						break;
		case INICIO:	Puntero	= 1;
						Fil		= CajaFondo.Fila + Max;
						break;
		case AVPAG:		for (int i=1; i<=Min-1*(Min==0); i++)
						{
							Fil = Fil - 1;
							if (Fil - CajaFondo.Fila + NumElem < Max + 1)	Fil = Fil + 1;
							if (Fil == CajaFondo.Fila) 
							{
								Fil		= CajaFondo.Fila + 1;
								Puntero = Puntero + 1;
								if (Puntero > NumElem - Min) Puntero = NumElem - Min;
							}
							if (Puntero < 1) Puntero = 1;
						}
						break;
		case REPAG:		for (int i=1; i<=Min-1*(Min==0); i++)
						{
							Puntero = Puntero - 1;
							if (Puntero < 1)
							{
								Puntero = 1;
								Fil		= Fil + 1;
								if (Fil - CajaFondo.Fila > Max) Fil = Max + CajaFondo.Fila;
							}
						}
						break;
		default:		break;
		}						// de switch
		//Pinta El Boton Anterior
		BotonOK.Tinta=CajaFondo.Tinta;		BotonOK.Papel=CajaFondo.Fondo;
		BotonESC.Tinta=CajaFondo.Tinta;		BotonESC.Papel=CajaFondo.Fondo;	
		BotonBandeja.Tinta=CajaFondo.Tinta;	BotonBandeja.Papel=CajaFondo.Fondo;
		BotonOrdenar.Tinta=CajaFondo.Tinta;	BotonOrdenar.Papel=CajaFondo.Fondo;
		BotonEnviar.Tinta=CajaFondo.Tinta;	BotonEnviar.Papel=CajaFondo.Fondo;	
		BotonBorrar.Tinta=CajaFondo.Tinta;	BotonBorrar.Papel=CajaFondo.Fondo;
	
		switch (BotonAnterior)
		{
		case 6:	PintaCaja(BotonOK, "");		PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3,	BotonOK.Columna+1,		CajaFondo.Papel, CajaFondo.Fondo, "Leer"); 
											PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3,	BotonOK.Columna+1,		CajaFondo.Tinta, CajaFondo.Fondo, "L"); break;
		case 1:	PintaCaja(BotonESC, "");	PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3,	BotonESC.Columna+1,		CajaFondo.Papel, CajaFondo.Fondo, "Finalizar"); 
											PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3,	BotonESC.Columna+1,		CajaFondo.Tinta, CajaFondo.Fondo, "F"); break;
		case 2: PintaCaja(BotonBandeja, "");PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3,	BotonBandeja.Columna+1, CajaFondo.Papel, CajaFondo.Fondo, sBandeja);
											PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3,	BotonBandeja.Columna+1, CajaFondo.Tinta, CajaFondo.Fondo, sBandeja.substr(0, 1)); break;
		case 3: PintaCaja(BotonOrdenar, "");PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3,	BotonOrdenar.Columna+1, CajaFondo.Papel, CajaFondo.Fondo, "Ordenar");
											PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3,	BotonOrdenar.Columna+1, CajaFondo.Tinta, CajaFondo.Fondo, "O"); break;
		case 4: PintaCaja(BotonEnviar, "");	PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3,	BotonEnviar.Columna+1,	CajaFondo.Papel, CajaFondo.Fondo, "Nuevo");
											PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3,	BotonEnviar.Columna+1,	CajaFondo.Tinta, CajaFondo.Fondo, "N"); break;
		case 5: PintaCaja(BotonBorrar, "");	PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3,	BotonBorrar.Columna + 1,CajaFondo.Papel, CajaFondo.Fondo, "Eliminar");
											PintaTxt(CajaFondo.Fila + CajaFondo.Alto + 3,	BotonBorrar.Columna + 1,CajaFondo.Tinta, CajaFondo.Fondo, "E"); break;
		}
	}while (Salida==-1); 		// de do
//-----------------------------------------------------------------------------
	BoxPut(MyBox);				//Pinto lo que habia
	PintaTxt(CajaFondo.Fila-1, 3, NEGRO, CYANB, "                                                                  ");
	return Salida;				//Hasta luego Lukas
}

//======================================================================
// SortStr. Ordena un string con 'registros' de longitud fija.
//======================================================================
	int SortStr (string &Cadena, int LongBloque)
	{
		string	Dato1, Dato2;
		int		Puntero1, Puntero2, Puntero3, Puntero4, Puntero5, Puntero6, Sw;
		int		LongCadena = Cadena.length();

		if (LongBloque > LongCadena)		return 1;
		if (LongBloque * LongCadena < 1)	return 2;

		int Registros = int(LongCadena / LongBloque + .5);
		if (Registros < 1)					return 3;

		Puntero1 = int(log(Registros) / log(2));	//Puntero1 = int(log(Registros) / log(2) + .5);
		Puntero2 = int(pow(2, Puntero1) / 2);

		while (Puntero2)
		{
			Puntero3 = Registros - Puntero2;
			Puntero4 = 1;
			while (Puntero4 <= Puntero3)
			{
				Puntero5 = Puntero4;
				do
				{
					Puntero6 = Puntero5 + Puntero2;
					Dato1 = Cadena.substr((Puntero5 - 1) * LongBloque, LongBloque);
					Dato2 = Cadena.substr((Puntero6 - 1) * LongBloque, LongBloque);
					Sw = 1;
					if ( Dato1 > Dato2 )
					{
						Cadena.replace((Puntero5 - 1) * LongBloque, LongBloque, Dato2);
						Cadena.replace((Puntero6 - 1) * LongBloque, LongBloque, Dato1);
						Puntero5 = Puntero5 - Puntero2;
						if (Puntero5 > 0) Sw = 0;
					}
				}while (Sw == 0);
				Puntero4 = Puntero4 + 1;
			}	//wend
			Puntero2 = int(Puntero2 / 2);
		}	//wend
		return 0;
	}

//========================================================================================
// SortMat. Ordena una Matriz de string con 'registros' de longitud fija, por posiciones.
//========================================================================================
	int SortMat (string MatStr[])
	{
		string	Dato1, Dato2;
		int		Puntero1, Puntero2, Puntero3, Puntero4, Puntero5, Puntero6, Sw;
		int		LongCadena = MatStr[1].length();			//Tamaño de cada registro. Han de ser iguales de tamaño todos.
		int		Desde=30;
		int		Hasta=60;
		int		LongBloque = Hasta-Desde;

		if (LongBloque > LongCadena)		return 1;		//Control de substring
		if (LongBloque * LongCadena < 1)	return 2;		//Control de tamaños

		int Registros=0;
		while (MatStr[Registros+1].length()>0) Registros++;	//Cuenta los registros que tienen algo
		if (Registros < 1)					return 3;		//Control de cantidad

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
		return 0;
	}