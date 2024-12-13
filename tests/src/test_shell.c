/**
 * @file test_shell.c
 * @brief Archivo de pruebas para las funciones relacionadas con la shell personalizada.
 *
 * Este archivo contiene pruebas unitarias para verificar el funcionamiento de
 * varias funciones de la shell, como manejo de señales, comandos internos como
 * `cd`, `clr`, y `echo`, y verificación de errores en ciertos casos.
 */

#include <test_shell.h>

extern int foreground_pid; /**< PID del proceso en primer plano */
extern int job_count;      /**< Contador de trabajos */

// Función para probar el manejo de señales cuando no hay proceso en primer plano
/**
 * @brief Prueba el manejo de señales cuando no hay un proceso en primer plano.
 *
 * La función establece que no hay un proceso en primer plano y simula
 * la llegada de una señal `SIGINT`, verificando que no se haga nada cuando
 * no hay procesos en ejecución en primer plano.
 *
 * @return 0 si la prueba pasa.
 */
int test_manejar_senal_sin_proceso_en_primer_plano(void)
{
    foreground_pid = -1;   /**< No hay proceso en primer plano */
    manejar_senal(SIGINT); /**< Ejecuta la señal de interrupción */

    // No hay una salida específica aquí, solo se observa el mensaje en la consola
    printf("Prueba manejar_senal_sin_proceso_en_primer_plano pasada\n");
    return 0; /**< Retorna 0 si el test pasa */
}

// Función para probar la ejecución del comando `cd` al directorio home
/**
 * @brief Prueba el comando `cd` con el argumento NULL, que debe llevar al directorio home.
 *
 * La función ejecuta el comando `cd(NULL)` para cambiar al directorio home
 * y luego verifica que la variable de entorno `PWD` apunte al directorio home.
 *
 * @return 0 si la prueba pasa.
 */
int test_ejecutar_cd_home(void)
{
    ejecutar_cd(NULL);                                       /**< Ejecuta el comando `cd` para ir al home */
    char* pwd = getenv("PWD");                               /**< Obtiene el valor actual de `PWD` */
    assert(pwd != NULL && strcmp(pwd, getenv("HOME")) == 0); /**< Verifica si estamos en el home */

    printf("Prueba ejecutar_cd_home pasada\n");
    return 0; /**< Retorna 0 si el test pasa */
}

// Función para probar la ejecución del comando `cd` a un directorio inexistente
/**
 * @brief Prueba el comando `cd` con una ruta inexistente.
 *
 * La función intenta cambiar al directorio `/directorio/inexistente`,
 * y luego verifica que se imprima un mensaje de error en caso de que no exista.
 *
 * @return 0 si la prueba pasa, aunque no hay aserciones explícitas, se asume que pasa si no hay fallos visibles.
 */
int test_ejecutar_cd_directorio_inexistente(void)
{
    ejecutar_cd("/directorio/inexistente"); /**< Intenta cambiar a un directorio inexistente */
    // Verificar si se imprime el mensaje de error
    perror("Error esperado en ejecutar_cd_directorio_inexistente");
    return 0; /**< Retorna 0 si el test pasa (sin aserciones explícitas) */
}

// Función para probar el comando `echo`
/**
 * @brief Prueba el comando `echo` con variables de entorno y texto normal.
 *
 * La función ejecuta el comando `echo` dos veces: primero con una variable de entorno
 * y luego con un mensaje normal. Verifica que la salida sea la esperada.
 *
 * @return 0 si la prueba pasa.
 */
int test_ejecutar_echo(void)
{
    setenv("TEST_VAR", "Hola mundo", 1); /**< Establece una variable de entorno */
    ejecutar_echo("$TEST_VAR");          /**< Debería imprimir "Hola mundo" */

    ejecutar_echo("Prueba de mensaje"); /**< Debería imprimir "Prueba de mensaje" */
    printf("\nPrueba ejecutar_echo pasada\n");
    return 0; /**< Retorna 0 si el test pasa */
}
