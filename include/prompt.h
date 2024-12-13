/**
 * @file prompt.h
 * @brief Declaraciones de funciones para manejar el prompt de la shell.
 */

#ifndef PROMPT_H
#define PROMPT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Definición del tamaño máximo del nombre de host.
 */
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 1024
#endif

/**
 * @brief Muestra el prompt en la consola.
 */
void mostrar_prompt(void);

#endif // PROMPT_H
