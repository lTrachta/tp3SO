/**
 * @file test_json.c
 * @brief Archivo de pruebas para funciones relacionadas con la configuración JSON.
 *
 * Este archivo contiene pruebas unitarias para verificar el correcto funcionamiento
 * de la carga, modificación de métricas y modificación de intervalo en la configuración
 * JSON.
 */

#include <test_json.h>

/**
 * @brief Macro para verificar condiciones y reportar el resultado.
 *
 * Esta macro evalúa una condición y, si no se cumple, imprime un mensaje de error
 * indicando que la prueba ha fallado, junto con la línea de código donde ocurrió.
 *
 * @param condition Condición a evaluar.
 */
#define TEST_ASSERT(condition)                                                                                         \
    if (!(condition))                                                                                                  \
    {                                                                                                                  \
        printf("Error: %s failed at line %d\n", #condition, __LINE__);                                                 \
        return 1;                                                                                                      \
    }

/**
 * @brief Prueba de carga de configuraciones desde un archivo JSON.
 *
 * Esta función carga las configuraciones del archivo JSON y asegura que
 * la configuración se haya cargado correctamente.
 *
 * @return 0 si la prueba pasa, 1 si falla.
 */
int test_cargar_configuracion(void)
{
    cJSON* config = cargar_configuracion();
    TEST_ASSERT(config != NULL); // Asegura que se cargó correctamente
    printf("Configuracion cargada correctamente.\n");
    cJSON_Delete(config);
    return 0;
}

/**
 * @brief Prueba para agregar una métrica a la lista `metrics_to_collect`.
 *
 * Esta función modifica la configuración para agregar "cpu_usage" a `metrics_to_collect`
 * y luego verifica que la métrica se haya añadido correctamente.
 *
 * @return 0 si la prueba pasa, 1 si falla.
 */
int test_modificar_configuracion_agregar_metrica(void)
{
    modificar_configuracion("cpu_usage", "1");
    cJSON* config = cargar_configuracion();
    TEST_ASSERT(config != NULL);

    // Verificar que "cpu_usage" esté en metrics_to_collect
    cJSON* metrics = cJSON_GetObjectItem(config, "metrics_to_collect");
    TEST_ASSERT(metrics != NULL);

    int found = 0;
    cJSON* metric;
    cJSON_ArrayForEach(metric, metrics)
    {
        if (strcmp(metric->valuestring, "cpu_usage") == 0)
        {
            found = 1;
            break;
        }
    }
    TEST_ASSERT(found);
    cJSON_Delete(config);
    return 0;
}

/**
 * @brief Prueba para eliminar una métrica de la lista `metrics_to_collect`.
 *
 * Esta función modifica la configuración para eliminar "cpu_usage" de `metrics_to_collect`
 * y luego verifica que la métrica se haya eliminado correctamente.
 *
 * @return 0 si la prueba pasa, 1 si falla.
 */
int test_modificar_configuracion_eliminar_metrica(void)
{
    modificar_configuracion("cpu_usage", "0");
    cJSON* config = cargar_configuracion();
    TEST_ASSERT(config != NULL);

    // Verificar que "cpu_usage" ya no esté en metrics_to_collect
    cJSON* metrics = cJSON_GetObjectItem(config, "metrics_to_collect");
    TEST_ASSERT(metrics != NULL);

    int found = 0;
    cJSON* metric;
    cJSON_ArrayForEach(metric, metrics)
    {
        if (strcmp(metric->valuestring, "cpu_usage") == 0)
        {
            found = 1;
            break;
        }
    }
    TEST_ASSERT(!found);
    cJSON_Delete(config);
    return 0;
}

/**
 * @brief Prueba para modificar el intervalo de muestreo en la configuración JSON.
 *
 * Esta función modifica el valor de `sampling_interval` en la configuración JSON
 * y luego verifica que el valor haya sido actualizado correctamente.
 *
 * @return 0 si la prueba pasa, 1 si falla.
 */
int test_modificar_configuracion_intervalo(void)
{
    modificar_configuracion("sampling_interval", "20");
    cJSON* config = cargar_configuracion();
    TEST_ASSERT(config != NULL);

    int intervalo = cJSON_GetObjectItem(config, "sampling_interval")->valueint;
    TEST_ASSERT(intervalo == 20);

    cJSON_Delete(config);
    return 0;
}
