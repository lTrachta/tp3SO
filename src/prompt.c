/**
 * @file prompt.c
 * @brief Muestra el prompt de la shell con el nombre de usuario, host y directorio actual.
 */

#include <prompt.h>

/**
 * @brief Muestra el prompt con el formato "usuario@host:directorio$ ".
 *
 * La función obtiene el nombre del host y el directorio de trabajo actual,
 * y luego imprime el prompt utilizando el nombre del usuario, el nombre del host
 * y el directorio de trabajo actual.
 */
void mostrar_prompt(void)
{
    char hostname[HOST_NAME_MAX];
    char cwd[HOST_NAME_MAX];

    gethostname(hostname, sizeof(hostname)); /**< Obtiene el nombre del host */
    getcwd(cwd, sizeof(cwd));                /**< Obtiene el directorio de trabajo actual */

    printf("%s@%s:%s$ ", getenv("USER"), hostname, cwd); /**< Imprime el prompt en el formato especificado */
    // getenv("USER"): Obtiene el nombre del usuario
}
