/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion.
	(c) Granados Duran Evelin Geraldine 321315153

	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.

******************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TAMTOKEN 1000
#define MAX_ELEMENTOS 70000
#define MAX_SUGERIDAS 100


//Funciones publicas del proyecto
/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/

// funciones del diccionario 

// permitir solo letras y numeros 
int esCaracterPermitido(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'));
}
//eliminar signos de puntuacion y cambiarlos por un espacio
void eliminarPuntuacion(char* cadena)
{
	int longitud = strlen(cadena);
	for (int i = 0; i < longitud; i++)
	{
		if (!esCaracterPermitido(cadena[i]))
		{
			cadena[i] = ' ';
		}
	}
}
//convertimos todas las cadenas en minuscula
void convertirMinusculas(char* cadena)
{
	int longitud = strlen(cadena);
	for (int i = 0; i < longitud; i++)
	{
		cadena[i] = tolower(cadena[i]);
	}
}

void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int* iNumElementos)
{
	FILE* fpDicc;
	fopen_s(&fpDicc, szNombre, "r");
	if (fpDicc != NULL)
	{
		printf("\nSe pudo abrir el archivo.\n");
		char palabra[TAMTOKEN];

		while (fscanf(fpDicc, "%s", palabra) != EOF)
		{
			eliminarPuntuacion(palabra);
			convertirMinusculas(palabra);

			int i;
			int encontrado = 0;

			for (i = 0; i < *iNumElementos; i++)
			{
				if (strcmp(szPalabras[i], palabra) == 0)
				{
					iEstadisticas[i]++;
					encontrado = 1;
					break;
				}
			}

			if (!encontrado)
			{
				strcpy(szPalabras[*iNumElementos], palabra);
				iEstadisticas[*iNumElementos] = 1;
				(*iNumElementos)++;
			}
		}
		fclose(fpDicc);

		// Ordenar el diccionario alfabéticamente utilizando el método de la burbuja
		int i, j;
		for (i = 0; i < *iNumElementos - 1; i++)
		{
			for (j = 0; j < *iNumElementos - i - 1; j++)
			{
				if (strcmp(szPalabras[j], szPalabras[j + 1]) > 0)
				{
					char temp[TAMTOKEN];
					strcpy(temp, szPalabras[j]);
					strcpy(szPalabras[j], szPalabras[j + 1]);
					strcpy(szPalabras[j + 1], temp);

					int tempFreq = iEstadisticas[j];
					iEstadisticas[j] = iEstadisticas[j + 1];
					iEstadisticas[j + 1] = tempFreq;
				}
			}
		}
	}
	else
	{
		printf("\nNo se pudo abrir el archivo.\n");
	}
}



/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
Intercambio(char* palabra, int indice, int* iNumSugeridas, char szPalabrasSugeridas[][TAMTOKEN]) {
	int len = strlen(palabra);
	if (indice < len - 1) {
		char temp = palabra[indice];
		palabra[indice] = palabra[indice + 1];
		palabra[indice + 1] = temp;
		strcpy(szPalabrasSugeridas[*iNumSugeridas], palabra);
		(*iNumSugeridas)++;
		temp = palabra[indice];
		palabra[indice] = palabra[indice + 1];
		palabra[indice + 1] = temp;
	}
}
void Reemplazo(char* palabra, int indice, char letra, int* iNumSugeridas, char szPalabrasSugeridas[][TAMTOKEN]) {
	palabra[indice] = letra;
	strcpy(szPalabrasSugeridas[*iNumSugeridas], palabra);
	(*iNumSugeridas)++;
}

void Insercion(char* palabra, int indice, char letra, int* iNumSugeridas, char szPalabrasSugeridas[][TAMTOKEN]) {
	int len = strlen(palabra);
	for (int i = len; i >= indice; --i) {
		palabra[i + 1] = palabra[i];
	}
	palabra[indice] = letra;
	strcpy(szPalabrasSugeridas[*iNumSugeridas], palabra);
	(*iNumSugeridas)++;
}
void ClonaPalabras(char* szPalabraLeida, char szPalabrasSugeridas[][TAMTOKEN], int* iNumSugeridas) {
	*iNumSugeridas = 0;
	int len = strlen(szPalabraLeida);

	// Lógica para clonar palabras con intercambio, reemplazo e inserción
	for (int i = 0; i < len; ++i) {
		// Intercambio de letras
		Intercambio(szPalabraLeida, i, iNumSugeridas, szPalabrasSugeridas);

		// Reemplazo de letra por otras letras del alfabeto
		for (char letra = 'a'; letra <= 'z'; ++letra) {
			Reemplazo(szPalabraLeida, i, letra, iNumSugeridas, szPalabrasSugeridas);
		}

		// Inserción de letras del alfabeto
		for (char letra = 'a'; letra <= 'z'; ++letra) {
			Insercion(szPalabraLeida, i, letra, iNumSugeridas, szPalabrasSugeridas);
		}
	}
}

/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
#define TAMTOKEN 50
#define MAX_CANDIDATAS 100

void intercambiar(char* str1, char* str2) {
	char temp[TAMTOKEN];
	strcpy(temp, str1);
	strcpy(str1, str2);
	strcpy(str2, temp);
}

void ListaCandidatas(
	char szPalabrasSugeridas[][TAMTOKEN],
	int iNumSugeridas,
	char szPalabras[][TAMTOKEN],
	int iEstadisticas[],
	int iNumElementos,
	char szListaFinal[][TAMTOKEN],
	int iPeso[],
	int& iNumLista) {

	// Copiar palabras candidatas y sus pesos
	for (int i = 0; i < iNumSugeridas; ++i) {
		strcpy(szListaFinal[i], szPalabrasSugeridas[i]);
		iPeso[i] = iEstadisticas[i];
	}

	// Ordenar las palabras candidatas por peso usando el algoritmo de ordenamiento de burbuja
	for (int i = 0; i < iNumSugeridas - 1; ++i) {
		for (int j = 0; j < iNumSugeridas - i - 1; ++j) {
			if (iPeso[j] < iPeso[j + 1]) {
				// Intercambiar palabras
				intercambiar(szListaFinal[j], szListaFinal[j + 1]);
				// Intercambiar pesos
				int temp = iPeso[j];
				iPeso[j] = iPeso[j + 1];
				iPeso[j + 1] = temp;
			}
		}
	}

	iNumLista = iNumSugeridas; // Actualizar el número de elementos en la lista final
}

int main() {
	char szPalabras[MAX_ELEMENTOS][TAMTOKEN];
	int iEstadisticas[MAX_ELEMENTOS];
	int iNumElementos = 0;
	char nombreArchivo[] = fpDicc

		Diccionario(nombreArchivo, szPalabras, iEstadisticas, &iNumElementos);


	// Imprime el diccionario ordenado
	printf("Diccionario ordenado alfabéticamente:\n");
	for (int i = 0; i < iNumElementos; i++) {
		printf("%s - Frecuencia: %d\n", szPalabras[i], iEstadisticas[i]);
	}

	