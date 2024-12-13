/**
 * @file test_json.h
 * @brief Declaraciones de funciones de prueba para el manejo de configuraciones en JSON.
 */

#ifndef TEST_JSON_H
#define TEST_JSON_H

#include <json.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Prueba la carga de la configuración desde el archivo JSON.
 *
 * Verifica que el archivo `config.json` se cargue correctamente y que
 * se lean los valores de configuración esperados.
 *
 * @return 0 si la prueba pasa, un valor distinto de 0 si falla.
 */
int test_cargar_configuracion(void);

/**
 * @brief Prueba la modificación de la configuración para agregar una métrica.
 *
 * Verifica que una nueva métrica se agregue a la lista `metrics_to_collect`
 * en el archivo `config.json`.
 *
 * @return 0 si la prueba pasa, un valor distinto de 0 si falla.
 */
int test_modificar_configuracion_agregar_metrica(void);

/**
 * @brief Prueba la modificación de la configuración para eliminar una métrica.
 *
 * Verifica que una métrica existente se elimine de la lista `metrics_to_collect`
 * en el archivo `config.json`.
 *
 * @return 0 si la prueba pasa, un valor distinto de 0 si falla.
 */
int test_modificar_configuracion_eliminar_metrica(void);

/**
 * @brief Prueba la modificación de la configuración para cambiar el intervalo de muestreo.
 *
 * Verifica que el `sampling_interval` en el archivo `config.json` se modifique correctamente.
 *
 * @return 0 si la prueba pasa, un valor distinto de 0 si falla.
 */
int test_modificar_configuracion_intervalo(void);

#endif // TEST_JSON_H
