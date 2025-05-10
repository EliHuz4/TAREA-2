// CODIGO SPOTIFIND

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Libreria para ocupar booleanos
#include <string.h> // Libreria para comparar char
#include <ctype.h> // Para revisar si se ingresa un digito o un caracter
#include "TDAS/list.h"
#include "TDAS/map.h"
#include "TDAS/list.c"
#include "TDAS/map.c"

typedef struct{ // Estructura para cada cancion
    size_t id; // ID de la cancion
    char *Artista; // Artista de la cancion
    char *AlbumName; // Album al que pertenece la cancion
    char *SongName; // Nombre de la cancion
    float Tempo; // Tempo de la cancion
    char *TrackGenre; // Genero de la cancion
}TipoCancion;


int is_equal_str(void* key1, void* key2) { // Funcion para comparar dos string y saber si son iguales
    return strcmp((char*)key1, (char*)key2) == 0;
}

char *duplicarCadena(char *str) { // Funcion que crea una copia nueva de una cadena de caracteres
    char *copia = malloc(strlen(str) + 1);
    if (copia != NULL) {
        strcpy(copia, str);
    }
    return copia;
}

char *aMayus(char *Palabra){ // Funcion que convierte todos los caracteres en Mayusculas
    for(int i = 0; Palabra[i] != '\0'; i++){
        Palabra[i] = toupper(Palabra[i]);
    }
    return Palabra;
}

char *aMinus(char *Palabra){ // Funcion que convierte todos los caracteres en Minusculas
    for(int i = 0; Palabra[i] != '\0'; i++){
        Palabra[i] = tolower(Palabra[i]);
    }
    return Palabra;
}

char *normalizarClave(char *strOriginal) { // Limpia y estandariza una cadena para usarla como clave en un mapa
    if (strOriginal == NULL) return NULL;

    char *copia = duplicarCadena(strOriginal);
    if (!copia) return NULL;

    // Elimina salto de línea
    copia[strcspn(copia, "\r\n")] = '\0';

    // Elimina espacios finales
    int len = strlen(copia);
    while (len > 0 && isspace(copia[len - 1])) {
        copia[--len] = '\0';
    }
    aMinus(copia);

    return copia;
}

// Funcion que llena un mapa donde cada clave es un artista y el valor es una lista de sus canciones
void PoblarMapaArtistas(Map *MapaArtistas, List *ListaCanciones) {
    TipoCancion *Cancion = list_first(ListaCanciones);
    int contador = 0;

    while (Cancion != NULL) {
        if (Cancion->Artista == NULL) {
            Cancion = list_next(ListaCanciones);
            continue;
        }

        // Limpia y normaliza el nombre del artista
        char *claveLimpia = normalizarClave(Cancion->Artista);
        if (claveLimpia == NULL) {
            printf(">> ERROR AL DUPLICAR EL NOMBRE DEL ARTISTA <<\n");
            Cancion = list_next(ListaCanciones);
            continue;
        }

        // Asegura que no tenga saltos de línea
        claveLimpia[strcspn(claveLimpia, "\r\n")] = '\0';

        // Busca si el artista ya está en el mapa
        MapPair *par = map_search(MapaArtistas, claveLimpia);
        List *ListaArtista;

        if (par == NULL) { // Si el artista no esta, se crea una nueva lista para ese artista
            ListaArtista = list_create();
            map_insert(MapaArtistas, claveLimpia, ListaArtista);
            printf("\rARTISTAS PROCESADOS: %d", contador++);
        } else { // Si el artista está en la lista, se usa la lista existente
            ListaArtista = (List *)par->value;
            free(claveLimpia);
        }

        // Se agrega la canción a la lista correspondiente del artista
        list_pushBack(ListaArtista, Cancion);
        Cancion = list_next(ListaCanciones);
    }

    printf("\n\n>> SE CARGARON %d ARTISTAS <<\n\n", contador);
}

// Funcion que llena un mapa donde cada clave es un género musical y el valor es una lista con canciones de ese género
void PoblarMapaGenre(Map *MapaGenre, List *ListaCanciones) {
    TipoCancion *Cancion = list_first(ListaCanciones);
    int contador = 0;
    
    while (Cancion != NULL) {
        if (Cancion->TrackGenre == NULL) {
            Cancion = list_next(ListaCanciones);
            continue;
        }

        // Se limpia y estandariza el nombre del género
        char *claveLimpia = normalizarClave(Cancion->TrackGenre);
        if (claveLimpia == NULL) {
            printf(">> ERROR AL DUPLICAR EL NOMBRE DEL GENERO <<\n");
            Cancion = list_next(ListaCanciones);
            continue;
        }

        // Eliminar salto de línea
        claveLimpia[strcspn(claveLimpia, "\r\n")] = '\0';

        // Eliminar espacios finales
        int len = strlen(claveLimpia);
        while (len > 0 && isspace(claveLimpia[len - 1])) {
            claveLimpia[--len] = '\0';
        }

        // Pasar a minúsculas
        for (int i = 0; claveLimpia[i]; i++) {
            claveLimpia[i] = tolower(claveLimpia[i]);
        }

        // Buscar el MapPair correspondiente a esta clave
        MapPair *par = map_search(MapaGenre, claveLimpia);
        List *ListaGenero;

        if (par == NULL) {
            // Si no existe, crear una nueva lista y insertar en el mapa
            ListaGenero = list_create();
            map_insert(MapaGenre, claveLimpia, ListaGenero);
            printf("\rGENEROS PROCESADOS: %d", ++contador);
        } else {
            // Si ya existe la clave, liberar la copia y usar la lista existente
            ListaGenero = (List *)par->value;
            free(claveLimpia);
        }
        
        // Añadir la canción a la lista del género
        list_pushBack(ListaGenero, Cancion);

        Cancion = list_next(ListaCanciones);
    }

    printf("\n\n>> SE CARGARON %d GENEROS <<\n\n", contador);
}


// Funcion para leer el archivo a partir de una ruta establecida
void leerArchivo(char *location, List *ListaCanciones){

    printf("\nIntentando abrir archivo en ruta: %s ...\n\n", location);

    FILE *csv = fopen(location, "r"); // Se trata de abrir el archivo
    if(csv == NULL){ // Si no se pudo abrir se muestra un error y se vuelve al menu principal
        perror("\n\nERROR AL ABRIR EL ARCHIVO\n\n");
        return;
    }
    printf("EL ARCHIVO FUE ABIERTO CON EXITO!\n\n"); // Si se pudo abrir se muestra un mensaje
    char linea[1000]; // Char de linea entera (fila)
    int fila = 0; // Contador de filas
    size_t Cont = 0; // Contador de canciones

    while (fgets(linea, sizeof(linea), csv)) { // Bucle que se repetira siempre que la linea exista

        if(fila == 0){
            fila++;
            continue;
        }

        char *LineaCpy = duplicarCadena(linea); // Se duplcia la linea y se guarda en otra variable
        if(LineaCpy == NULL) { // Si no se duplicó correctamente muestra un error
            printf("\n\n >> ERROR: NO SE PUDO DUPLICAR LA LINEA DE TEXTO <<\n\n");
            continue;
        }

        TipoCancion *Cancion = malloc(sizeof(TipoCancion)); // Se crea la variable puntero a "TipoCancion"
        if(Cancion == NULL){ // Si hubo un error de asignacion de memoria muestra un error
            printf("\n\n >> ERROR: NO SE PUDO CREAR LA CANCION <<");
            free(LineaCpy); // Se libera la memoria de la linea duplicada
            continue;
        }

        // Asignacion de variables
        Cancion->id = Cont;
        Cancion->Artista = NULL;
        Cancion->AlbumName = NULL;
        Cancion->SongName = NULL;
        Cancion->TrackGenre = NULL;
        Cont++; // Suma 1 al contador

        // Division de columnas mediante comas
        char *token = strtok(LineaCpy, ",");
        int columna = 0;
        
        while (token != NULL)
        {
            // Extraccion de los campos que nos interezan
            if (columna == 2)
            {
                Cancion->Artista = duplicarCadena(token);
            }
            else if (columna == 3)
            {
                Cancion->AlbumName = duplicarCadena(token);
            }
            else if (columna == 4)
            {
                Cancion->SongName = duplicarCadena(token);
            }
            else if (columna == 18)
            {
                Cancion->Tempo = atof(token);
            }
            else if (columna == 20)
            {
                Cancion->TrackGenre = duplicarCadena(token);
            }
            token = strtok(NULL, ",");
            columna++;
        }
        // Muestra el proceso de las canciones 
        printf("\rCANCIONES PROCESADAS: %zu", Cont);
        list_pushBack(ListaCanciones, Cancion);
        free(LineaCpy); // Libera la linea duplicada
        fila++;
    }
    fclose(csv); // cierra el archivo
    printf("\n\n >> SE CARGARON %d CANCIONES <<\n\n", Cont);
}

// Funcion para liberar la memoria de cada dato de la cancion
void FreeListaCanciones(List *lista) {
    TipoCancion *cancion = list_first(lista); // Se crea una variable Tipo Cancion y se le asigna al primero de la lista de canciones
    while (cancion != NULL) { // Bucle que se repetira mientras que exista una cancion
        if(cancion)
        {
            free(cancion->AlbumName);
            free(cancion->Artista);
            free(cancion->SongName);
            free(cancion->TrackGenre);
            free(cancion);
        }
        cancion = list_next(lista); // Pasa a la siguiente cancion en la lista
    }
    free(lista);
}

char *SubMenu1(){ // Funcion para crear un sub menu dentro de la terminal
    char *location = malloc(10001); // Crea un string
    printf("\n- RUTA PREDETERMINADA: ARCHIVO\\song_dataset_.csv -\n"); // Indica la ruta por defecto del archivo
    printf("\nIngrese la Ruta de el Archivo -> ");
    scanf("%10000s", location); // Escanea la linea de la terminal
    getchar();
    return location;
}

char *SubMenuGenre(){ // Funcion para crear un sub menu para ingresar el genero musical a buscar
    char temp[51];  // Se crea un string
    printf("\nINGRESE EL GENERO MUSICAL QUE DESEA BUSCAR ->");
    scanf("%s", temp); // Lee el genero musical
    getchar();
    char *genero = duplicarCadena(temp); // Duplica la cadena ingresada
    return genero;
}

char *SubMenuArtist(){ // Funcion para crear un sub menu para ingresar el artista a buscar
    char temp[51];  // Se crea un string
    printf("\nINGRESE EL ARTISTA QUE DESEA BUSCAR ->");
    if (fgets(temp, sizeof(temp), stdin) != NULL) { // Se lee la linea de texto ingresada por el usuario
        // Elimina el carácter de nueva línea al final (opcional)
        size_t len = strlen(temp);
        if (len > 0 && temp[len - 1] == '\n') {
            temp[len - 1] = '\0';
        }
    }
    char *Artista = duplicarCadena(temp); // Se duplica la linea de texto
    return Artista;
}

void BuscarGenero(Map *GenreMap) { // Funcion para buscar por genero musical
    char *temp = SubMenuGenre(); // Se crea un string temporal a partir de el sub menu que se despliega
    char *Clave = normalizarClave(temp); // Se normaliza el string para facilitar la busqueda de el genero musical
    free(temp); // Se libera el string de el sub menu

    // Buscar el MapPair correspondiente a la clave del género
    MapPair *par = map_search(GenreMap, Clave);
    
    if (par != NULL) {
        // Muestra la lista de canciones del genero desde el valor del par
        List *Lista = (List *)par->value;
        size_t cont = 0;
        TipoCancion *Cancion = list_first(Lista);
        printf("\n______________________________________________________________________________________________________________\n");
        printf("___________________________________ LISTA DE CANCIONES DEL GENERO: %s ___________________________________\n", aMayus(Clave));
        printf("______________________________________________________________________________________________________________\n");
        printf("| ID            | ARTISTA                | ALBUM                      | CANCION                    | TEMPO     | GENERO    |\n");
        printf("--------------------------------------------------------------------------------------------------------------\n");
        
        while (Cancion != NULL) {
            printf("| %-13zu | %-22s | %-26s | %-26s | %-9.2f | %-10s |\n",
                    // Si no se encuentra uno de los datos o el campo esta vacio, se reemplaza por N/A
                   Cancion->id,
                   Cancion->Artista ? Cancion->Artista : "N/A",
                   Cancion->AlbumName ? Cancion->AlbumName : "N/A",
                   Cancion->SongName ? Cancion->SongName : "N/A",
                   Cancion->Tempo,
                   Cancion->TrackGenre ? Cancion->TrackGenre : "N/A");
            cont++;
            Cancion = list_next(Lista);
        }
        
        printf("______________________________________________________________________________________________________________\n");
        printf("\n\n SE ENCONTRARON %zu RESULTADOS PARA EL GENERO ""%s""", cont, Clave);
        } else { // En caso de no encontrar al genero musical muestra un mensaje
            printf("\n>> NO SE ENCONTRO EL GENERO MUSICAL INGRESADO: %s <<\n", Clave);
        }

    free(Clave); // Se libera la clave
}

void BuscarPorArtista(Map *ArtistMap) {// Funcion para buscar por artista
    char *temp = SubMenuArtist(); // Se crea un string temporal a partir de el sub menu que se despliega
    char *Clave = normalizarClave(temp); // Se normaliza el string para facilitar la busqueda del artista
    free(temp);

    // Buscar el MapPair correspondiente a la clave del género
    MapPair *par = map_search(ArtistMap, Clave);
    
    if (par != NULL) {
        // Se muestra la lista de canciones del artista desde el valor del par
        List *Lista = (List *)par->value;
        size_t cont = 0;
        TipoCancion *Cancion = list_first(Lista);
        printf("\n______________________________________________________________________________________________________________\n");
        printf("___________________________________ LISTA DE CANCIONES DEL ARTISTA: %s ___________________________________\n", aMayus(Clave));
        printf("______________________________________________________________________________________________________________\n");
        printf("| ID            | ARTISTA                | ALBUM                      | CANCION                    | TEMPO     | GENERO    |\n");
        printf("--------------------------------------------------------------------------------------------------------------\n");
        
        while (Cancion != NULL) {
            printf("| %-13zu | %-22s | %-26s | %-26s | %-9.2f | %-10s |\n",
                    // Si no se encuentra un dato o el campo esta vacio se reemplaza con N/A
                   Cancion->id,
                   Cancion->Artista ? Cancion->Artista : "N/A",
                   Cancion->AlbumName ? Cancion->AlbumName : "N/A",
                   Cancion->SongName ? Cancion->SongName : "N/A",
                   Cancion->Tempo,
                   Cancion->TrackGenre ? Cancion->TrackGenre : "N/A");
            cont++;
            Cancion = list_next(Lista);
        }
        
        printf("______________________________________________________________________________________________________________\n");
        printf("\n\n SE ENCONTRARON %zu RESULTADOS PARA EL ARTISTA ""%s""", cont, Clave);
        } else { // En caso de no encontrar al artista se muestra un mensaje
            printf("\n>> NO SE ENCONTRO EL ARTISTA INGRESADO: %s <<\n", Clave);
        }

    free(Clave); // Se libera la clave
}


void liberarMapaDeListas(Map *mapa) { // Funcion para liberar la memoria de los mapas 
    MapPair *par = map_first(mapa);
    while (par != NULL) {
        List *lista = (List *)par->value;
        // No se libera cada cancion porque ya se liberó en FreeListaCanciones
        free(lista);  // Se libera la lista
        free(par->key); // Se libera la clave de cada par
        par = map_next(mapa);
    }
    free(mapa);
}

void CargarCanciones(List *lista) // Funcion principal para cargar las canciones al programa
{
    char *locacion = SubMenu1();
    leerArchivo(locacion, lista);
    free(locacion);
}

void Menu(){ // Funcion que muestra el menu de opciones que puede escoger el usuario
    printf("\n\n==================\n");
    printf("    SPOTIFIND\n");
    printf("==================\n");
    printf("1) Cargar Canciones\n");
    printf("2) Buscar por Genero\n");
    printf("3) Buscar por Artista\n");
    printf("4) Buscar por Tempo\n");
    printf("5) Salir\n");
    printf("Seleccione una opcion -> ");
}

int SubMenuTempo() { // Muestra un sub menu de opciones que puede escoger el usuario en base al tempo de las canciones
    int opcion;
    printf("\nCATEGORIA DE TEMPO:\n\n");
    printf("1) LENTAS     (< 80 BPM)\n");
    printf("2) MODERADAS  (80 - 120 BPM)\n");
    printf("3) RAPIDAS    (> 120 BPM)\n");
    printf("\nSeleccione una opcion -> ");
    scanf("%d", &opcion);
    getchar();
    return opcion;
}

void BuscarPorTempo(List *ListaCanciones) { // Funcion para buscar canciones mediante el tempo de las mismas
    int opcion = SubMenuTempo(); // Se despliega el sub menu

    float minTempo = 0; // Se crea una variable para fijar el tempo minimo que deben cumplir las canciones para ser mostradas
    float maxTempo = 0; // Se crea una variable para fijar el tempo maximo que deben cumplir las canciones para ser mostradas
    char *categoria = ""; // Se crea un string para mostrar que categoria se esta mostrando

    if (opcion == 1) { 
        maxTempo = 80;
        categoria = "LENTAS (< 80 BPM)";
    }
    else if (opcion == 2) {
        minTempo = 80;
        maxTempo = 120;
        categoria = "MODERADAS (80 - 120 BPM)";
    }
    else if (opcion == 3) {
        minTempo = 120;
        maxTempo = 1000; // Limite excesivamente alto
        categoria = "RAPIDAS (> 120 BPM)";
    }
    else { // Si el usuario ingresa una opcion fuera del menu muestra un mensaje
        printf("\n>> OPCION NO VALIDA <<\n");
        return;
    }

    TipoCancion *Cancion = list_first(ListaCanciones);
    bool encontrada = false; // Flag para saber si hay alguna cancion que se haya mostrado
    size_t Cont = 0;
    printf("\n______________________________________________________________________________________________________________\n");
    printf("_______________________________ CANCIONES %s _______________________________\n", categoria);
    printf("______________________________________________________________________________________________________________\n");
    printf("| ID            | ARTISTA                | ALBUM                      | CANCION                    | TEMPO     | GENERO    |\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");

    while (Cancion != NULL) {
        if ((Cancion->Tempo >= minTempo) && (Cancion->Tempo < maxTempo)) {
            Cont++;
            encontrada = true;
            printf("| %-13zu | %-22s | %-26s | %-26s | %-9.2f | %-10s |\n",
                    // En caso de que un dato no se encuentra o el campo este vacio se reemplaza por N/A
                   Cancion->id,
                   Cancion->Artista ? Cancion->Artista : "N/A",
                   Cancion->AlbumName ? Cancion->AlbumName : "N/A",
                   Cancion->SongName ? Cancion->SongName : "N/A",
                   Cancion->Tempo,
                   Cancion->TrackGenre ? Cancion->TrackGenre : "N/A");
        }
        Cancion = list_next(ListaCanciones);
    }

    if (!encontrada) { // Si la flag nunca cambio significa que ninguna cancion se mostró, por lo tanto se muestra un mensaje
        printf("\n>> NO SE ENCONTRARON CANCIONES EN ESTA CATEGORIA <<\n");
    }
    else{
        printf("SE ENCONTRARON %zu RESULTADOS PARA EL TEMPO %s\n", Cont, categoria);
    }

    printf("______________________________________________________________________________________________________________\n");
}

int main(){
    
    int seleccion; // Seleccion de menu del usuario
    List *ListSong = list_create(); // Se crea la lista de canciones
    Map *ArtistMap = map_create(is_equal_str); // Se crea el mapa de artistas
    Map *GenreMap = map_create(is_equal_str); // Se crea el mapa de generos musicales
    
    while (true)
    {
        Menu(); // Se muestra el menu
        char temp[2]; // Se crea un string temporal para guardar la seleccion del usuario
        scanf("%1s", temp);
        getchar();

        if(isdigit(temp[0])) // si el usuario ingreso un numero se transforma a int
        {
            seleccion = atoi(temp);
        }
        else{ // Si el usuario ingreso una letra se muestra un mensaje 
            printf("\n\nSELECCIONE UNA OPCION VALIDA\n");
            continue;
        }

        if(seleccion == 1) // Si la seleccion fue 1
        {
            if(ListSong->size == 0)
            {
                CargarCanciones(ListSong); // Se cargan las canciones en la lista
                PoblarMapaArtistas(ArtistMap, ListSong); // Se cargan los artistas en un mapa
                PoblarMapaGenre(GenreMap, ListSong); // Se cargan los generos musicales en un mapa
            }
            else{ // Si el usuario ya cargo las canciones se muestra un mensaje
                printf("\n>> YA SE CARGARON TODAS LAS CANCIONES DISPONIBLES <<");
            }
        }
        else if(seleccion == 2) // Si la seleccion fue 2
        {
            if(ListSong->size == 0) // Si la lista esta vacia se muestra un mensaje
            {
                printf("\n>> NO HAY GENEROS MUSICALES DISPONIBLES <<\n");
            }
            else{ // Si hay al menos 1 elemento se llama a la funcion "BuscarGenero"
                BuscarGenero(GenreMap);
            }
        }
        else if(seleccion == 3) // Si la seleccion fue 3
        {
            if(ListSong->size == 0) // Si la lista esta vacia se muestra un mensaje
            {
                printf("\n>> NO HAY ARTISTAS DISPONIBLES <<\n");
            }
            else{ // Si hay al menos 1 elemento se llama a la funcion "BuscarPorArtista"
                BuscarPorArtista(ArtistMap);
            }
        }
        else if(seleccion == 4) // Si la seleccion fue 4
        {
            if (ListSong->size == 0) { // Si la lista esta vacia se muestra un mensaje
                printf("\n>> NO HAY CANCIONES CARGADAS, VUELVA MAS TARDE <<\n");
            } else { // Si hay al menos 1 elemento se llama a la funcion "BuscarPorTempo"
                BuscarPorTempo(ListSong);
            }
        }
        else if(seleccion == 5){ // Si la opcion fue 5 
            printf("\nVuelve Pronto!!\n\n"); // Se muestra un mensaje de despedida
            break; // Se rompe el bucle
        }
        else if(seleccion > 5 || seleccion < 1){ // Si la opcion fue un numero mayor a 5 o menor a 1 muestra un mensaje
            printf("\n\nSELECCIONE UNA OPCION VALIDA\n");
        }
        printf("\n");
    }
    // Se libera memoria
    FreeListaCanciones(ListSong);
    liberarMapaDeListas(ArtistMap);
    liberarMapaDeListas(GenreMap);
    return 0; // Finaliza el programa
}
