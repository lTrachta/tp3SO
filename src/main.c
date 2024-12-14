/**
 * @file main.c
 * @brief Programa principal de la shell personalizada con soporte para configuración JSON, manejo de comandos internos
 * y ejecución de archivo de comandos.
 */

#include <cjson/cJSON.h> ///< Biblioteca cJSON para trabajar con JSON
#include <json.h>        ///< Incluye funciones para manipulación de configuración JSON
#include <pipes.h>       ///< Incluye funciones para manejo de pipes
#include <prompt.h>      ///< Incluye la función mostrar_prompt()
#include <shell.h>       ///< Incluye funciones para comandos internos
#include <filesystem.h>  ///< Incluye funciones para la exploración del filesystem
#include <stdio.h>       ///< Para funciones estándar como printf
#include <stdlib.h>      ///< Para funciones de gestión de memoria
#include <string.h>      ///< Para funciones de manipulación de cadenas
#include <sys/types.h>   ///< Para tipos de datos específicos de sistema como pid_t
#include <sys/wait.h>    ///< Para control de procesos y funciones wait
#include <unistd.h>      ///< Para llamadas al sistema relacionadas con el sistema operativo (como sleep)

#define HOST_NAME_MAX		64

/// Definición de constante TRUE para bucle infinito
#define TRUE 1

/// Variable global para controlar el estado del monitor
int monitor_en_ejecucion = 0;

/**
 * @brief Función principal de la shell personalizada.
 *
 * Esta función inicializa las configuraciones, verifica la existencia de un archivo
 * de comandos y maneja el ciclo principal de la shell, procesando comandos del usuario o del archivo.
 *
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Arreglo de cadenas con los argumentos de línea de comandos.
 * @return EXIT_SUCCESS en caso de éxito, o EXIT_FAILURE en caso de error.
 */
int main(int argc, char* argv[])
{
    iniciar_pipes();             ///< Inicializa el manejo de pipes
    iniciar_manejador_senales(); ///< Inicia el manejador de señales

    // Cargar configuraciones desde config.json
    cJSON* configuracion = cargar_configuracion();
    if (configuracion == NULL)
    {
        printf("Error al cargar la configuración.\n");
    }
    else
    {
        printf("Configuración cargada correctamente.\n");
    }

    /// Manejo de archivo de comandos
    FILE* file = NULL;
    if (argc > 1)
    {
        file = fopen(argv[1], "r");
        if (!file)
        {
            perror("Error al abrir el archivo");
            return EXIT_FAILURE;
        }
    }

    /// Bucle principal de la shell personalizada
    while (TRUE)
    {
        char comando[HOST_NAME_MAX];

        /// Manejo de comandos internos para el monitor
        if (strcmp(comando, "start_monitor") == 0)
        {
            start_monitor();
        }
        else if (strcmp(comando, "stop_monitor") == 0)
        {
            stop_monitor();
        }
        else if (strcmp(comando, "status_monitor") == 0)
        {
            status_monitor();
        }

        if (file)
        {
            // Leer comandos del archivo de comandos
            if (fgets(comando, sizeof(comando), file) != NULL)
            {
                printf("Ejecutando comando en archivo: %s\n", comando);
                comando[strcspn(comando, "\n")] = 0; ///< Eliminar el salto de línea
                leer_y_ejecutar_comando(comando);    ///< Ejecutar el comando leído
            }
            else
            {
                // Verificar fin de archivo o error de lectura
                if (feof(file))
                {
                    printf("Fin del archivo alcanzado.\n");
                }
                else
                {
                    perror("Error al leer del archivo");
                }
                break;
            }
        }
        else
        {
            mostrar_prompt(); ///< Mostrar el prompt de la shell
            if (fgets(comando, sizeof(comando), stdin) != NULL)
            {
                comando[strcspn(comando, "\n")] = 0; ///< Eliminar el salto de línea
                printf("Ejecutando comando: %s\n", comando);

                // Verificar si el comando comienza con "search"
                if (strncmp(comando, "search", 6) == 0)
                {
                    char* cmd = strtok(comando, " "); // "search" ya está en cmd
                    char* path = strtok(NULL, " ");
                    char* type = strtok(NULL, " ");

                    if (path && type)
                    {
                        if (strcmp(type, "config") == 0)
                        {
                            search_directory(path, "config");
                        }
                        else if (strcmp(type, "json") == 0)
                        {
                            search_directory(path, "json");
                        }
                        else
                        {
                            printf("Tipo no soportado. Use 'config' o 'json'.\n");
                        }
                    }
                    else
                    {
                        printf("Uso incorrecto del comando search. Ejemplo: search <directorio> <config|json>\n");
                    }
                }
                else if (strncmp(comando, "config", 6) == 0)
                {
                    // Si es "config", dividir en key y value (sin modificar el comando original)
                    char* cmd = strtok(comando, " "); // "config" ya está en cmd
                    char* key = strtok(NULL, " ");
                    char* value = strtok(NULL, " ");

                    // Verificar si key y value existen
                    if (key && value)
                    {
                        // Llamar a la función modificar_configuracion para actualizar el archivo config.json
                        modificar_configuracion(key, value);
                        printf("Configuración actualizada: %s = %s\n", key, value);
                    }
                    else
                    {
                        printf("Uso incorrecto del comando config. Ejemplo: config <métrica> <valor>\n");
                    }
                }
                else
                {
                    // Si no es "config", ejecutar el comando normalmente sin dividirlo
                    printf("Comando recibido: %s\n", comando);
                    leer_y_ejecutar_comando(comando);
                }
            }
            else
            {
                perror("Error al leer el comando");
                break;
            }
        }
    }

    /// Liberar memoria y cerrar el archivo de configuración
    cJSON_Delete(configuracion);
    return EXIT_SUCCESS;
}
