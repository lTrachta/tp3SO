/**
 * @file test_shell.h
 * @brief Declaraciones de funciones de prueba para la funcionalidad de la shell personalizada.
 */

#ifndef TEST_SHELL_H
#define TEST_SHELL_H

#include <assert.h>
#include <shell.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Prueba el manejo de señales cuando no hay un proceso en primer plano.
 *
 * Verifica que la shell maneje correctamente las señales cuando no hay un proceso en primer plano
 * para recibirlas, y que se imprima el mensaje adecuado.
 *
 * @return 0 si la prueba pasa, un valor distinto de 0 si falla.
 */
int test_manejar_senal_sin_proceso_en_primer_plano(void);

/**
 * @brief Prueba el cambio de directorio a la carpeta del usuario (`$HOME`) con `cd`.
 *
 * Verifica que el comando `cd` sin argumentos cambie el directorio al home del usuario.
 *
 * @return 0 si la prueba pasa, un valor distinto de 0 si falla.
 */
int test_ejecutar_cd_home(void);

/**
 * @brief Prueba el intento de cambio a un directorio inexistente con `cd`.
 *
 * Verifica que el comando `cd` a un directorio no existente muestre un mensaje de error.
 *
 * @return 0 si la prueba pasa, un valor distinto de 0 si falla.
 */
int test_ejecutar_cd_directorio_inexistente(void);

/**
 * @brief Prueba la función `echo` para imprimir un mensaje en la terminal.
 *
 * Verifica que el comando `echo` muestre el mensaje dado o el valor de una variable de entorno.
 *
 * @return 0 si la prueba pasa, un valor distinto de 0 si falla.
 */
int test_ejecutar_echo(void);

#endif // TEST_SHELL_H
