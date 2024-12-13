/**
 * @file pipes.h
 * @brief Declaraciones de funciones para el manejo de pipes en el monitoreo.
 */

#ifndef PIPES_H
#define PIPES_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/** @brief Descriptor de archivo para enviar datos al monitor. */
static int pipe_to_monitor[2];

/** @brief Descriptor de archivo para recibir datos del monitor. */
static int pipe_from_monitor[2];

/** @brief PID del proceso de monitoreo. */
static pid_t monitor_pid = -1;

/**
 * @brief Inicializa los pipes para la comunicación con el monitor.
 */
void iniciar_pipes(void);

/**
 * @brief Inicia el proceso de monitoreo.
 */
void start_monitor(void);

/**
 * @brief Detiene el proceso de monitoreo.
 */
void stop_monitor(void);

/**
 * @brief Muestra el estado actual del proceso de monitoreo.
 */
void status_monitor(void);

#endif // PIPES_H
