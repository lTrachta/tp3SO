/**
 * @file json.h
 * @brief Declaraciones de funciones para manejar configuraciones JSON.
 */

#ifndef JSON_H
#define JSON_H

#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Agregado para strcmp

/**
 * @brief Carga la configuración desde un archivo JSON.
 * @return Un puntero a un objeto cJSON con la configuración cargada.
 */
cJSON* cargar_configuracion(void);

/**
 * @brief Guarda la configuración en un archivo JSON.
 * @return Un entero indicando éxito (0) o error (distinto de 0).
 */
int guardar_configuracion(cJSON* configuracion);

/**
 * @brief Ejecuta un comando de configuración específico.
 */
void ejecutar_comando_config(char** args);

/**
 * @brief Modifica una clave específica en la configuración.
 */
void modificar_configuracion(const char* clave, const char* valor);

#endif // JSON_H
