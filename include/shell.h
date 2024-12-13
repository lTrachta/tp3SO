/**
 * @file shell.h
 * @brief Declaraciones de funciones principales para el funcionamiento de la shell.
 */

#ifndef SHELL_H
#define SHELL_H

#include <fcntl.h>  // Para las funciones de archivos como open()
#include <signal.h> // Para el manejo de señales
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> // Para wait()
#include <unistd.h>

/**
 * @brief Definición del tamaño máximo del nombre de host.
 */
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 1024
#endif

/** @brief PID del proceso en primer plano actual. */
extern int foreground_pid;

/** @brief Número de procesos en segundo plano (trabajos) en ejecución. */
extern int job_count;

/**
 * @brief Maneja una señal específica recibida por la shell.
 */
void manejar_senal(int sig);

/**
 * @brief Inicializa el manejador de señales para la shell.
 */
void iniciar_manejador_senales(void);

/**
 * @brief Cambia el directorio actual.
 */
void ejecutar_cd(char* directorio);

/**
 * @brief Limpia la pantalla de la terminal.
 */
void ejecutar_clr(void);

/**
 * @brief Muestra un mensaje en la consola.
 */
void ejecutar_echo(char* mensaje);

/**
 * @brief Cierra la shell.
 */
void ejecutar_quit(void);

/**
 * @brief Lee un comando de la entrada y lo ejecuta.
 */
void leer_y_ejecutar_comando(char* comando);

/**
 * @brief Ejecuta un comando que incluye pipes.
 */
void ejecutar_comando_con_pipes(char* comando);

/**
 * @brief Ejecuta un comando con redirección de entrada o salida.
 */
void ejecutar_con_redireccion(char* comando);

#endif // SHELL_H
