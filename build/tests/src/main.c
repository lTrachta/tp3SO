/**
 * @file main_test.c
 * @brief Archivo principal para ejecutar todas las pruebas unitarias del proyecto.
 *
 * Este archivo contiene la función principal que ejecuta todas las pruebas de JSON,
 * Pipes, y Shell, y reporta el resultado de cada prueba.
 */

#include <stdio.h>
#include <test_json.h>
#include <test_pipes.h>
#include <test_shell.h>

/**
 * @brief Macro para verificar condiciones y reportar el resultado.
 *
 * Esta macro evalúa una condición y, si no se cumple, imprime un mensaje
 * indicando que la prueba ha fallado, junto con el nombre de la prueba.
 *
 * @param condition Condición a evaluar.
 * @param test_name Nombre de la prueba para identificarla en el reporte.
 */
#define TEST_ASSERT(condition, test_name)                                                                              \
    if (!(condition))                                                                                                  \
    {                                                                                                                  \
        printf("Test '%s' failed\n", test_name);                                                                       \
        return 1;                                                                                                      \
    }

/**
 * @brief Función principal para ejecutar todas las pruebas.
 *
 * Esta función llama a cada función de prueba para verificar distintas funcionalidades
 * del proyecto. Si alguna prueba falla, se reporta el fallo. Si todas las pruebas pasan,
 * se imprime un mensaje de éxito.
 *
 * @return 0 si todas las pruebas pasan, 1 si alguna prueba falla.
 */
int main(void)
{
    int result;

    /** Ejecutar pruebas de JSON */
    printf("->Test para cargar un archivo json:\n\n");
    result = test_cargar_configuracion();
    TEST_ASSERT(result == 0, "test_cargar_configuracion");
    printf("\n");

    printf("->Test para agregar una métrica a la lista `metrics_to_collect`:\n\n");
    result = test_modificar_configuracion_agregar_metrica();
    TEST_ASSERT(result == 0, "test_modificar_configuracion_agregar_metrica");
    printf("\n");

    printf("->Test para eliminar una métrica de la lista `metrics_to_collect`:\n\n");
    result = test_modificar_configuracion_eliminar_metrica();
    TEST_ASSERT(result == 0, "test_modificar_configuracion_eliminar_metrica");
    printf("\n");

    printf("->Test para modificar el intervalo de muestreo en la configuración JSON:\n\n");
    result = test_modificar_configuracion_intervalo();
    TEST_ASSERT(result == 0, "test_modificar_configuracion_intervalo");
    printf("\n");

    /** Ejecutar pruebas de Pipes */
    printf("->Test para verificar la correcta inicialización de los pipes de comunicación con el monitor:\n\n");
    result = test_iniciar_pipes();
    TEST_ASSERT(result == 0, "test_iniciar_pipes");
    printf("\n");

    /** Ejecutar pruebas de la Shell */
    printf("->Test para el manejo de señales cuando no hay un proceso en primer plano:\n\n");
    result = test_manejar_senal_sin_proceso_en_primer_plano();
    TEST_ASSERT(result == 0, "test_manejar_senal_sin_proceso_en_primer_plano");
    printf("\n");

    printf("->Test para el comando `cd` con el argumento NULL, que debe llevar al directorio home:\n\n");
    result = test_ejecutar_cd_home();
    TEST_ASSERT(result == 0, "test_ejecutar_cd_home");
    printf("\n");

    printf("->Test para el comando `cd` con una ruta inexistente:\n\n");
    result = test_ejecutar_cd_directorio_inexistente();
    TEST_ASSERT(result == 0, "test_ejecutar_cd_directorio_inexistente");
    printf("\n");

    printf("->Test para el comando `echo` con variables de entorno y texto normal:\n\n");
    result = test_ejecutar_echo();
    TEST_ASSERT(result == 0, "test_ejecutar_echo");
    printf("\n");

    /** Imprimir mensaje final si todas las pruebas pasaron */
    printf("All tests passed successfully!\n");
    return 0;
}
