/**
 * @file test_pipes.h
 * @brief Declaraciones de funciones de prueba para la funcionalidad de pipes en la shell personalizada.
 */

#ifndef TEST_PIPES_H
#define TEST_PIPES_H

#include <pipes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief Prueba la inicialización y ejecución de comandos con pipes.
 *
 * Verifica que los pipes entre comandos se creen y se gestionen correctamente,
 * y que los datos fluyan correctamente entre los procesos.
 *
 * @return 0 si la prueba pasa, un valor distinto de 0 si falla.
 */
int test_iniciar_pipes(void);

#endif // TEST_PIPES_H
