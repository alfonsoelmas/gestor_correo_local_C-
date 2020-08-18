//===========================================
//MÛdulo Correo
//===========================================

#include "mCorreo.h"
#include "GestorCorreo.h"
#include "mGestor.h"
#include "ASLib.h"

//------------------------------------------
//Permite al usuario escribir y enviar un nuevo correo. Se solicitar·n los datos del nuevo correo en el siguiente orden:
//- El destinatario (sÛlo uno). - El asunto del correo (una lÌnea). - El cuerpo del correo (varias lÌneas).
//El resto de datos de un correo (identificador, emisor y fecha) se calcular·n autom·ticamente
//Si no se ha podido enviar el correo al destinatario (porque no existe o porque sus listas est·n llenas),
//se indicar· mediante un mensaje de error que el envÌo a dicho destinatario no se ha podido realizar.
//Si un correo se ha enviado con Èxito al destinatario, dicho envÌo quedar· reflejado en dos sitios:
//la bandeja de salida del emisor y la bandeja de entrada del destinatario.
//Recibe un identificador de emisor y devuelve un correo con todos sus datos rellenos
void Correo_Nuevo(tCorreo &Correo, string Emisor)
{
	PintaTxt(3, 30, CYANB, AZUL, "    CORREO NUEVO    ");
	string C_Digitos = "0123456789";
	string C_Numeros = "-.";
	string C_Puntuacion = " #$%&@'*+-.,;:=<>?![](){}/|™∫^_\"ø°";
	string C_Mayusculas = "ABCDEFGHIJKLMNOPQRSTUVWYZ«—";
	string C_Minusculas = "abcdefghijklmnopqrstuvwxyzÁÒ";
	string C_MayusAcent = "¡…Õ”⁄¬ Œ‘€¿»Ã“ŸƒÀœ÷‹";
	string C_MinusAcent = "·ÈÌÛ˙‚ÍÓÙ˚‡ËÏÚ˘‰ÎÔˆ¸ˇ";
	
	int Col=3, Fil=5;
	int Tnt=AZUL, Ppl=CYANB;
	PintaTxt(Fil,   Col,    AZUL, CYANB, "  Para: ");
	PintaTxt(Fil+1, Col,    AZUL, CYANB, "Asunto: ");
	PintaTxt(Fil,   Col+45, AZUL, CYANB, "Fecha: ");  PintaTxt(Fil, Col+51, AZULB, CYANB, MostrarFecha(time(0))); 
	
	rAtrCpt Cpt;
	Cpt.Msk = "[∏]";
	Cpt.Tnt = BLANCOB;
	Cpt.Ppl = NEGRO;
	Cpt.Tnx = CYANB;
	Cpt.Vld = V_INTRO + V_TAB + V_CURDOWN + V_AVPAG + V_CTRLINTRO + V_ESCAPE + V_CTRLTAB + V_CURUP + V_REPAG;
	Cpt.Cnj = C_Minusculas+C_Mayusculas+C_Puntuacion+C_Digitos+C_MinusAcent;
	Cpt.Ppl=Tnt;
	Cpt.Tnt=Ppl;
	Cpt.Tnx=Ppl;
	Cpt.Vld = V_AUTOSKIP + V_INTRO + V_TAB + V_ESCAPE + V_CTRLTAB;
	
	rCpyPst CpyPst;

	rAtrPnt Pnt;
	Pnt.Ppl=CYANB;
	Pnt.Tnt=AZUL;

	rAtrDat Para;
	Para.Car=' ';
	Para.Dat="";
	Para.Lon=17;
	Para.Sde=DERECHA;

	Cpt.Vld = V_AUTOSKIP + V_INTRO + V_TAB + V_ESCAPE + V_CTRLTAB;
	Cpt.Msk="[∑]";

	CpyPst.Permitido=false;		//Se ven los caracteres
		
	rAtrDat Asunto;
	Asunto.Car=' ';
	Asunto.Dat="";
	Asunto.Lon=30;
	Asunto.Sde=DERECHA;

	string TextoCuerpo;
	rAtrDat Cuerpo;
	Cuerpo.Car=' ';
	Cuerpo.Dat="";
	Cuerpo.Lon=76;
	Cuerpo.Sde=DERECHA;

	Pnt.Col=Col+8;
	Pnt.Fil=Fil;
	Cpt.Cnj=C_Mayusculas+C_Minusculas+C_Digitos+"_.";
	
	GetTxt(Para, Pnt, Cpt, CpyPst);

	Pnt.Fil=Fil+1;
	Cpt.Cnj=C_Mayusculas+C_Minusculas+C_Digitos+"_."+C_Puntuacion+C_MayusAcent+C_MinusAcent;
	GetTxt(Asunto, Pnt, Cpt, CpyPst);
	
	Pnt.Fil=Fil+3;
	Pnt.Col=2;
	for (int i=0; i<12; i++)
	{
		Pnt.Fil++;
		Cuerpo.Dat="";
		GetTxt(Cuerpo, Pnt, Cpt, CpyPst);
		TextoCuerpo+=Cuerpo.Dat+"\n";
	}
	
	Correo.Fecha		=time(0);			//Ponemos la fecha de hoy
	Correo.Asunto		=Asunto.Dat;
	Correo.Destinatario	=Para.Dat+"@"+DOMINIO;
	Correo.Emisor		=Emisor;
	Correo.Cuerpo		=TextoCuerpo;
	stringstream sFecha;
	sFecha << Correo.Fecha;
	Correo.CorreoId		=Emisor+"_"+sFecha.str();

	return;
}

//-------------------------------------------
//Recibe un identificador de emisor y el correo original que se va a contestar,
//y devuelve un correo con todos sus datos rellenos
void Correo_Contestacion(const tCorreo &CorreoOriginal, tCorreo &CorreoContestar, string Emisor)
{
	PintaTxt(3, 30, CYANB, AZUL, "  CORREO CONTESTAR    ");
	
	CorreoContestar.Emisor		 = CorreoOriginal.Destinatario;
	CorreoContestar.Destinatario = CorreoOriginal.Emisor;
	CorreoContestar.Asunto		 = "Re: "+CorreoOriginal.Asunto.substr(0, 26);
	CorreoContestar.Cuerpo		 = "";
	CorreoContestar.Fecha		 = time(0);
	stringstream sFecha;
	sFecha << CorreoContestar.Fecha;
	Limpia(Emisor);
	CorreoContestar.CorreoId	 = Emisor+"_"+sFecha.str();

	int Col=3, Fil=5;
	int Tnt=AZUL, Ppl=CYANB;
	PintaTxt(Fil,   Col,	AZUL, CYANB, "    De: "); PintaTxt(Fil,   Col+8,  AZULB, CYANB, CorreoContestar.Emisor); 
	PintaTxt(Fil+1, Col,	AZUL, CYANB, "  Para: "); PintaTxt(Fil+1, Col+8,  AZULB, CYANB, CorreoContestar.Destinatario); 
	PintaTxt(Fil+2, Col,	AZUL, CYANB, "Asunto: "); PintaTxt(Fil+2, Col+8,  AZULB, CYANB, CorreoContestar.Asunto); 
	PintaTxt(Fil,   Col+47, AZUL, CYANB, "Fecha: ");  PintaTxt(Fil,   Col+54, AZULB, CYANB, MostrarFecha(time(0))); 

	string C_Digitos	= "0123456789";
	string C_Numeros	= "-.";
	string C_Puntuacion = " #$%&@'*+-.,;:=<>?![](){}/|™∫^_\"ø°";
	string C_Mayusculas = "ABCDEFGHIJKLMNOPQRSTUVWYZ«—";
	string C_Minusculas = "abcdefghijklmnopqrstuvwxyzÁÒ";
	string C_MayusAcent = "¡…Õ”⁄¬ Œ‘€¿»Ã“ŸƒÀœ÷‹";
	string C_MinusAcent = "·ÈÌÛ˙‚ÍÓÙ˚‡ËÏÚ˘‰ÎÔˆ¸ˇ";
	
	string TextoCuerpo;
	rAtrDat Cuerpo;
	Cuerpo.Car=' ';
	Cuerpo.Dat="";
	Cuerpo.Lon=76;
	Cuerpo.Sde=DERECHA;
	
	rAtrCpt Cpt;
	Cpt.Msk = "[∏]";
	Cpt.Tnt = BLANCOB;
	Cpt.Ppl = NEGRO;
	Cpt.Tnx = CYANB;
	Cpt.Vld = V_INTRO + V_TAB + V_CURDOWN + V_AVPAG + V_CTRLINTRO + V_ESCAPE + V_CTRLTAB + V_CURUP + V_REPAG;
	Cpt.Cnj = C_Minusculas+C_Mayusculas+C_Puntuacion+C_Digitos+C_MinusAcent;
	Cpt.Ppl=Tnt;
	Cpt.Tnt=Ppl;
	Cpt.Tnx=Ppl;
	Cpt.Vld = V_AUTOSKIP + V_INTRO + V_TAB + V_ESCAPE + V_CTRLTAB;
	
	rCpyPst CpyPst;
	CpyPst.Permitido=false;		//Se ven los caracteres
	
	rAtrPnt Pnt;
	Pnt.Ppl=CYANB;
	Pnt.Tnt=AZUL;
	Pnt.Col=Col+8;
	Pnt.Fil=Fil;

	Pnt.Fil=Fil+3;
	Pnt.Col=2;
	for (int i=0; i<12; i++)
	{
		Pnt.Fil++;
		Cuerpo.Dat="";
		GetTxt(Cuerpo, Pnt, Cpt, CpyPst);
		TextoCuerpo+=Cuerpo.Dat+"\n";
	}
	
	CorreoContestar.Cuerpo = TextoCuerpo + "<cita:>\n<" + Correo_Cuerpo(CorreoOriginal) + ">\n";
	return;
}

//===========================================
//Devuelve un string con el contenido completo del correo para poderlo mostrar por consola.
string Correo_Cuerpo(const tCorreo &Correo)
{
	string contenido="";

	contenido+="De: "		+ Correo.Emisor					+ "\t";
	contenido+="\t "		+ MostrarFecha(Correo.Fecha)	+ "\n";
	contenido+="Para: "		+ Correo.Destinatario			+ "\n";
	contenido+="Asunto: "	+ Correo.Asunto					+ "\n";
	contenido+="\n""\t"		+ Correo.Cuerpo					+ "\n";
	return contenido;
}

//===========================================
//Devuelve un string que contiene la informaciÛn que se mostrar· en la bandeja de entrada/salida:
//emisor, asunto y fecha sin hora.
string Correo_ObtenerCabecera(const tCorreo &Correo)
{
	string sContenido=" ";
	int tam,rellena;

	//EMISOR(21 espacios)
	tam=Correo.Emisor.length();

	if(tam<TAM_CHAR1)
	{
		rellena=TAM_CHAR1-tam+1;
		sContenido+=Correo.Emisor;
		for(int i=0; i<rellena; i++) sContenido+=" ";
	}
	else
	{
		for(int i=0; i<TAM_CHAR1-1; i++) sContenido+=Correo.Emisor.at(i);
		sContenido+="* ";
	}

	//ASUNTO(39 espacios)
	tam=Correo.Asunto.length();

	if(tam<TAM_CHAR2)
	{
		rellena=TAM_CHAR2-tam+1;
		sContenido+=Correo.Asunto;

		for(int i=0; i<rellena; i++) sContenido+=" ";
	}
	else
	{
		for(int i=0; i<TAM_CHAR2-1; i++) sContenido+=Correo.Asunto.at(i);
		sContenido+="+ ";
	}
	//FECHA
	sContenido+=MostrarSoloDia(Correo.Fecha);

	return sContenido;
}

//===========================================
//Dado un flujo de archivo de entrada (ya abierto), lee los datos que corresponden a un correo y lo
//devuelve. Devuelve false cuando el correo cargado no es v·lido.
bool Correo_Cargar(tCorreo &Correo, ifstream &Archivo)
{
	bool carga=false;
	string compruebo;

	if(Archivo.is_open())
	{
		Archivo >> Correo.CorreoId;
		Archivo >> Correo.Fecha;
		Archivo >> Correo.Emisor;
		Archivo >> Correo.Destinatario;
		getline(Archivo, Correo.Asunto);	//getline == leo salto de linea del anterior dato
		getline(Archivo, Correo.Asunto);
		Correo.Cuerpo="";
		do
		{
			getline(Archivo,compruebo);
			if(compruebo!="X") Correo.Cuerpo+=compruebo + "\n";
		}while(compruebo!="X");
		carga=!carga;
	}
	return carga;
}

//===========================================
//Dado un flujo de archivo de salida (ya abierto), escribe en el flujo los datos que
//corresponden a un correo.
void Correo_Guardar(const tCorreo &Correo, ofstream &Archivo)
{
	Archivo << Correo.CorreoId		<< endl;
	Archivo << Correo.Fecha			<< endl;
	Archivo << Correo.Emisor		<< endl;
	Archivo << Correo.Destinatario	<< endl;
	Archivo << Correo.Asunto		<< endl;
	Archivo << Correo.Cuerpo;		//<< endl; //øøø Mete un endl de mas øøø
	Archivo << "X" << endl;
}

//===========================================
//Operadores Para LstCorreo:
bool operator < (tCorreo opIzq, tCorreo opDer)
{
	bool comp=false;

	if(opIzq.Asunto==opDer.Asunto)
	{
		if(opIzq.Fecha<opDer.Fecha)			comp=true;
		else                                comp=false;
	}
	else if(opIzq.Asunto>opDer.Asunto)		comp=false;
	else                                    comp=true;

	return comp;
}
//===========================================
bool operator > (tCorreo opIzq, tCorreo opDer)
{
	bool comp=false;

	if(opIzq.Asunto==opDer.Asunto)
	{
		if(opIzq.Fecha>opDer.Fecha)			comp=true;
		else                                comp=false;
	}
	else if(opIzq.Asunto<opDer.Asunto)		comp=false;
	else                                    comp=true;

	return comp;
}
//===========================================
bool operator <= (tCorreo opIzq, tCorreo opDer)
{
	bool comp;

	if(opIzq.Asunto==opDer.Asunto)
	{
		if(opIzq.Fecha<=opDer.Fecha)		comp=true;
		else								comp=false;
	}
	else if(opIzq.Asunto>opDer.Asunto)		comp=false;
	else                                    comp=true;

	return comp;
}
//===========================================
bool operator >= (tCorreo opIzq, tCorreo opDer)
{
	bool comp;

	if(opIzq.Asunto==opDer.Asunto)
	{
		if(opIzq.Fecha>=opDer.Fecha)		comp=true;
		else								comp=false;
	}
	else if(opIzq.Asunto<opDer.Asunto)		comp=false;
	else                                    comp=true;

	return comp;
}