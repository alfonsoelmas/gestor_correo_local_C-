//===========================================
//Módulo Correo
//===========================================


#ifndef mCorreo
#define mCorreo

#include <string>
#include <fstream>
using namespace std;

//-------------------------------------------
//Declaracion de Tipos y Constantes
const int TAM_CHAR1=21;	
const int TAM_CHAR2=39;
typedef time_t tFecha;
struct tCorreo
{
	string Emisor;
	string Destinatario;
	string Asunto;
	string Cuerpo;
	tFecha Fecha;
	string CorreoId;
};

//-------------------------------------------
//Declaración de Prototipos de funciones

//-------------------------------------------
//Recibe un identificador de emisor y devuelve un correo con todos sus datos rellenos
void Correo_Nuevo(tCorreo &Correo, string Emisor);

//-------------------------------------------
// Recibe un identificador de emisor y el correo original que se va a contestar,
//y devuelve un correo con todos sus datos rellenos
void Correo_Contestacion(const tCorreo &CorreoOriginal, tCorreo &Correo, string Emisor);

//-------------------------------------------
//Devuelve un string con el contenido completo del correo para poderlo mostrar por consola.
string Correo_Cuerpo(const tCorreo &Correo);

//-------------------------------------------
//Devuelve un string que contiene la información que se mostrará en la bandeja de entrada/salida:
//emisor, asunto y fecha sin hora.
string Correo_ObtenerCabecera(const tCorreo &Correo);

//-------------------------------------------
//Dado un flujo de archivo de entrada (ya abierto), lee los datos que corresponden a un correo y lo
//devuelve. Devuelve false cuando el correo cargado no es válido.
bool Correo_Cargar(tCorreo &Correo, ifstream &Archivo);

//-------------------------------------------
//Dado un flujo de archivo de salida (ya abierto), escribe en el flujo los datos que
//corresponden a un correo.
void Correo_Guardar(const tCorreo &Correo, ofstream &Archivo);


//Operadores Para LstCorreo(ordenar por asunto y fecha):
bool operator  <(tCorreo opIzq, tCorreo opDer);
bool operator  >(tCorreo opIzq, tCorreo opDer);
bool operator <=(tCorreo opIzq, tCorreo opDer);
bool operator >=(tCorreo opIzq, tCorreo opDer);

#endif