/**
 * @file json.c
 * @brief Implementación de funciones para manejar configuraciones JSON.
 */

#include <json.h>

/// Ruta del archivo JSON de configuración.
#define CONFIG_FILE_PATH "../config.json"

/**
 * @brief Carga el contenido del archivo de configuración JSON.
 *
 * @return Un puntero a un objeto cJSON con la configuración cargada,
 *         o NULL si ocurre un error al abrir o parsear el archivo.
 */
cJSON* cargar_configuracion()
{
    FILE* file = fopen(CONFIG_FILE_PATH, "r");
    if (file == NULL)
    {
        perror("No se pudo abrir config.json");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* data = malloc(length + 1);
    if (data == NULL)
    {
        perror("Error al asignar memoria para el archivo JSON");
        fclose(file);
        return NULL;
    }

    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON* configuracion = cJSON_Parse(data);
    free(data);

    if (configuracion == NULL)
    {
        fprintf(stderr, "Error al parsear config.json\n");
    }

    return configuracion;
}

/**
 * @brief Guarda el contenido de la configuración en el archivo JSON.
 *
 * @param configuracion Un puntero al objeto cJSON con la configuración a guardar.
 * @return Un entero indicando éxito (0) o error (-1).
 */
int guardar_configuracion(cJSON* configuracion)
{
    char* json_string = cJSON_Print(configuracion);
    FILE* file = fopen(CONFIG_FILE_PATH, "w");
    if (file == NULL)
    {
        perror("No se pudo abrir config.json para escritura");
        free(json_string);
        return -1;
    }

    fprintf(file, "%s", json_string);
    fclose(file);
    free(json_string);

    return 0;
}

/**
 * @brief Modifica una configuración específica en el archivo JSON.
 *
 * Esta función permite cambiar el valor de una clave específica en la configuración.
 * Puede actualizar el valor de `sampling_interval` o añadir/eliminar métricas
 * en la lista `metrics_to_collect`.
 *
 * @param clave La clave de la configuración a modificar.
 * @param valor El valor a asignar a la clave especificada.
 */
void modificar_configuracion(const char* clave, const char* valor)
{
    cJSON* configuracion = cargar_configuracion();
    if (configuracion == NULL)
    {
        return;
    }

    if (strcmp(clave, "sampling_interval") == 0)
    {
        int intervalo = atoi(valor);
        cJSON* intervalo_json = cJSON_GetObjectItem(configuracion, "sampling_interval");
        if (intervalo_json != NULL)
        {
            cJSON_SetIntValue(intervalo_json, intervalo);
        }
        else
        {
            cJSON_AddNumberToObject(configuracion, "sampling_interval", intervalo);
        }
    }
    else
    {
        // Trabajar con metrics_to_collect
        cJSON* metrics_array = cJSON_GetObjectItem(configuracion, "metrics_to_collect");
        if (!cJSON_IsArray(metrics_array))
        {
            // Crear el array si no existe
            metrics_array = cJSON_CreateArray();
            cJSON_AddItemToObject(configuracion, "metrics_to_collect", metrics_array);
        }

        int flag = atoi(valor);
        if (flag == 1)
        {
            // Agregar la métrica si no está ya presente
            cJSON* existing_metric = NULL;
            cJSON_ArrayForEach(existing_metric, metrics_array)
            {
                if (strcmp(existing_metric->valuestring, clave) == 0)
                {
                    printf("La métrica %s ya está en la lista.\n", clave);
                    guardar_configuracion(configuracion);
                    cJSON_Delete(configuracion);
                    return;
                }
            }
            cJSON_AddItemToArray(metrics_array, cJSON_CreateString(clave));
            printf("Métrica %s agregada a metrics_to_collect.\n", clave);
        }
        else if (flag == 0)
        {
            // Eliminar la métrica si está presente
            int index_to_remove = -1;
            int index = 0;
            cJSON* metric = NULL;
            cJSON_ArrayForEach(metric, metrics_array)
            {
                if (strcmp(metric->valuestring, clave) == 0)
                {
                    index_to_remove = index;
                    break;
                }
                index++;
            }
            if (index_to_remove != -1)
            {
                cJSON_DeleteItemFromArray(metrics_array, index_to_remove);
                printf("Métrica %s eliminada de metrics_to_collect.\n", clave);
            }
            else
            {
                printf("La métrica %s no se encontró en la lista.\n", clave);
            }
        }
        else
        {
            fprintf(stderr, "Valor inválido para %s. Use 1 o 0.\n", clave);
        }
    }

    if (guardar_configuracion(configuracion) == 0)
    {
        printf("Configuración actualizada correctamente.\n");
    }
    else
    {
        fprintf(stderr, "Error al guardar la configuración.\n");
    }

    cJSON_Delete(configuracion);
}

/**
 * @brief Procesa el comando "config" para modificar configuraciones.
 *
 * Esta función toma los argumentos de línea de comando para el comando "config",
 * verifica que los argumentos sean válidos y llama a `modificar_configuracion`
 * para actualizar la configuración.
 *
 * @param args Un arreglo de cadenas que contiene los argumentos del comando.
 */
void ejecutar_comando_config(char** args)
{
    if (args[1] == NULL || args[2] == NULL)
    {
        fprintf(stderr, "Uso: config <métrica> <valor>\n");
        return;
    }
    modificar_configuracion(args[1], args[2]);
}
