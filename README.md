## SPOTIFIND
---
## Descripción
Spotifind es un sistema de almacenamiento de canciones que organiza la informacion de estas mediante artista, genero musical o tempo. Permite a los usuarios visualizar las canciones cargadas al programa de manera ordenada a partir de un archivo determinado.

---
## Cómo compilar y ejecutar

Este sistema ha sido desarrollado en lenguaje C y puede ejecutarse fácilmente utilizando **Visual Studio Code** junto con una extensión para C/C++, como **C/C++ Extension Pack** de Microsoft. Para comenzar a trabajar con el sistema en tu equipo local, sigue estos pasos:
---
### Requisitos previos:

- Tener instalado [Visual Studio Code](https://code.visualstudio.com/).
- Instalar la extensión **C/C++** (Microsoft).
- Tener instalado un compilador de C (como **gcc**). Si estás en Windows, se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar el entorno [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

---
### Pasos para compilar y ejecutar:

- Descarga y descomprime el archivo .zip en una carpeta de tu elección.
- Abre el proyecto en Visual Studio Code
- Inicia Visual Studio Code.
    Selecciona **Archivo > Abrir carpeta...** y elige la carpeta donde descomprimiste el proyecto.
- Compila el código
    Abre el archivo principal (**tarea2.c**).
    Abre la terminal integrada (**Terminal > Nueva terminal**).
    En la terminal, compila el programa con el siguiente comando:
        ```
        gcc tarea2.c -o tarea2.exe
        ```
- Ejecuta el programa
    Una vez compilado, puedes ejecutar la aplicación con:
        ```
        ./tarea2.exe
        ```

---
## Funcionalidades

### Funcionando correctamente:

- Cargar canciones permite al usuario subir un archivo (escribiendo su ruta) con informacion variada de las canciones que quiere cargar, se cargaran todas las canciones al sistema y se mostrará un contador ascendiente con el valor total de canciones cargadas. Además mostrará un detalle de los artistas y generos musicales encontrados dentro de este archivo.
- Buscar por genero permitirá al usuario buscar por genero musical escribiendo el genero musical a encontrar con un maximo de 50 caracteres.
- Buscar por artista permitirá al usuario buscar canciones por artista musical, mostrando todas las canciones pertenecientes a ese artista, para buscar se escribe el nombre de el artista con un maximo de 50 caracteres.
- Buscar por tempo desplegara un menu de 3 opciones que permitira al usuario ver todas las canciones de un tempo en especifico, clasificandolas en lentas, moderadas y rapidas, ademas mostrando en que intervalos de tempo se clasifican las canciones.
- Salir finalizará el programa mostrando un mensaje.

### Problemas conocidos:

- Cuando en el menú se ingresa un caracter distinto a un numero de los disponibles el programa volverá al menú inicial dekando un mensaje de reintento.
- El programa está hecho especificamente para que funcione con el archivo predeterminado incluido, cualquier otro archivo CSV que no tenga el mismo numero de columnas ordenadas de manera especifica no funcionará en el programa.
- Cuando se intenta cargar por segunda vez un archivo el programa muestra un mensaje indicando que ya se cargaron todas las canciones disponibles.
- (EXTRA) Existen problemas dentro del codigo, no puedo compilar el programa si es que no dejo incluido en el inicio el archivo list.c y map.c

### A mejorar:

- Permitir la busqueda por nombre de la cancion.
- Permitir mostrar ciertas canciones delimitadas, ya que la terminal tiende a llenarse y esto hace que no sean legibles todas las canciones.
- Rapidez a la hora de procesar los artistas.

## Ejemplo de uso

**Paso 1: Cargar Canciones**

Se comienza cargando las canciones al programa.

```
**Opción seleccionada: 1) Cargar Canciones**

- RUTA PREDETERMINADA: ARCHIVO\song_dataset_.csv -

Ingrese la Ruta de el Archivo -> ARCHIVO\song_dataset_.csv

Intentando abrir archivo en ruta: ARCHIVO\song_dataset_.csv ...

EL ARCHIVO FUE ABIERTO CON EXITO!

CANCIONES PROCESADAS: XXXX

 >> SE CARGARON XXXX CANCIONES <<

ARTISTAS PROCESADOS: XXXX

>> SE CARGARON XXXX ARTISTAS <<

GENEROS PROCESADOS: XXXX

>> SE CARGARON XXXX GENEROS <<

**VUELVE AL MENU**
```

El programa procesa las canciones, los artistas y los generos, mostrando progresivamente cuantos lleva para finalmente dar una cantidad fija de cada especificacion.

**Paso 2: Buscar por Genero**

Tras procesar el genero el programa organiza automaticamente cada genero musical, mostrando mediante busqueda todas las canciones con ese genero.

```
**Opción seleccionada: 2) Buscar por Genero**
INGRESE EL GENERO MUSICAL QUE DESEA BUSCAR ->hip-hop

______________________________________________________________________________________________________________
___________________________________ LISTA DE CANCIONES DEL GENERO: HIP-HOP ___________________________________
______________________________________________________________________________________________________________
| ID            | ARTISTA                | ALBUM                      | CANCION                    | TEMPO     | GENERO    |
--------------------------------------------------------------------------------------------------------------
| 51000         | AP Dhillon             | Summer High                | Summer High                | 114.98    | hip-hop
   |
| 51001         | Shubh                  | No Love                    | No Love                    | 137.03    | hip-hop
   |
| 51002         | Shubh;Ikky             | Baller                     | Baller                     | 180.22    | hip-hop
   |
| 51003         | AP Dhillon;Shinda Kahlon;Gurinder Gill;Gminxr | Insane                     | Insane                     | 98.97     | hip-hop
.
.
.
SE ENCONTRARON XXX RESULTADOS PARA EL GENERO HIP-HOP

**VUELVE AL MENU**
```
La lista está ordenada mediante el ID de la cancion, de forma creciente.

**Paso 3: Buscar por Artista**

Tras procesar los artistas, el programa automaticamente organiza cada artista y sus canciones guardandolos en una lista de reproduccion con todas las canciones.
Cuando el usuario selecciona esta alternativa el programa muestra la lista de reproduccion del artista con todas sus canciones.

```
**Opción seleccionada: 3) Buscar por Artista**

INGRESE EL ARTISTA QUE DESEA BUSCAR ->bad bunny

______________________________________________________________________________________________________________
___________________________________ LISTA DE CANCIONES DEL ARTISTA: BAD BUNNY ___________________________________
______________________________________________________________________________________________________________
| ID            | ARTISTA                | ALBUM                      | CANCION                    | TEMPO     | GENERO    |
--------------------------------------------------------------------------------------------------------------
| 67359         | Bad Bunny              | Un Verano Sin Ti           | Tit├¡ Me Pregunt├│         | 106.67    | latin
     |
| 67559         | Bad Bunny              | Un Verano Sin Ti           | Efecto                     | 98.05     | latin
     |
| 67603         | Bad Bunny              | Un Verano Sin Ti           | Moscow Mule                | 99.97     | latin
.
.
.

 SE ENCONTRARON XXX RESULTADOS PARA EL ARTISTA BAD BUNNY

**VUELVE AL MENU**
```
La lista está ordenada mediante ID de la cancion, de forma creciente.


**Paso 4: Buscar por Tempo**

El usuario buscara por tempo mediante un sub menu que se desplegará, en donde al seleccionar una alternativa se mostraran todas las canciones disponibles con ese tempo en especifico.

```
**Opción seleccionada: 4) Buscar por Tempo**

CATEGORIA DE TEMPO:

1) LENTAS     (< 80 BPM)
2) MODERADAS  (80 - 120 BPM)
3) RAPIDAS    (> 120 BPM)

Seleccione una opcion -> 1

[ SI LA OPCION ES 1 ]
______________________________________________________________________________________________________________
_______________________________ CANCIONES LENTAS _______________________________
______________________________________________________________________________________________________________
| ID            | ARTISTA                | ALBUM                      | CANCION                    | TEMPO     | GENERO    |
--------------------------------------------------------------------------------------------------------------
| 113996        | Rainy Lullaby          | "#mindfulness - Soft Rain for Mindful Meditation |  Stress Relief Relaxation Music" | 0.03      | 5          |
| 113997        | Rainy Lullaby          | "#mindfulness - Soft Rain for Mindful Meditation |  Stress Relief Relaxation Music" | 0.04      | 4          |
| 114000        | Ces├íria Evora         | Miss Perfumado             | Barbincor                  | 79.20     | world-music
.
.
.
SE ENCONTRARON XXX RESULTADOS PARA EL TEMPO LENTAS (< 80 BPM)
______________________________________________________________________________________________________________

**VUELVE AL MENU**
```

Por cada opcion se mostraran todas las canciones que esten cargadas al programa con un tempo lento, moderado o rapido, dependiendo de la categoria elegida por el usuario.

**Paso 5: Salir del Programa**

El usuario cierra el programa, eliminando todas las canciones procesadas.

```
**Opción seleccionada: 5) Salir**

Vuelve Pronto!!

**TERMINA EL PROGRAMA**
```

**CODIGO CREADO POR MATIAS ELIHU SALAS VALDIVIA**
