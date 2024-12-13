/**
 * @file pipes.c
 * @brief Implementación de las funciones de manejo de pipes y control del monitor en la shell personalizada.
 */

#include <pipes.h>

/**
 * @brief Inicializa los pipes para comunicación entre la shell y el monitor.
 *
 * Crea dos pipes: uno para enviar datos al monitor y otro para recibir datos.
 * En caso de error al crear los pipes, muestra un mensaje de error y termina el programa.
 */
void iniciar_pipes()
{
    if (pipe(pipe_to_monitor) == -1 || pipe(pipe_from_monitor) == -1)
    {
        perror("Error al crear pipes");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Inicia el proceso de monitorización.
 *
 * Crea un proceso hijo para ejecutar el monitor. Redirige la entrada estándar (stdin) del monitor
 * al pipe de entrada y la salida estándar (stdout) al pipe de salida.
 * Si la creación del proceso falla, muestra un mensaje de error.
 */
void start_monitor()
{
    if ((monitor_pid = fork()) == 0)
    {
        // Código del proceso hijo (monitor)
        close(pipe_to_monitor[1]);   ///< Cerrar el extremo de escritura del pipe de entrada
        close(pipe_from_monitor[0]); ///< Cerrar el extremo de lectura del pipe de salida

        dup2(pipe_to_monitor[0], STDIN_FILENO);    ///< Redirigir stdin al extremo de lectura del pipe de entrada
        dup2(pipe_from_monitor[1], STDOUT_FILENO); ///< Redirigir stdout al extremo de escritura del pipe de salida

        execl("../tp2SO/metrics", "metrics", NULL); ///< Ejecuta el programa de monitorización
        perror("Error al ejecutar el monitor");
        exit(EXIT_FAILURE); ///< Termina el proceso hijo en caso de error
    }
    else if (monitor_pid > 0)
    {
        close(pipe_to_monitor[0]); ///< Cerrar extremos innecesarios en el proceso padre
        close(pipe_from_monitor[1]);
        printf("Monitor iniciado con PID %d\n", monitor_pid);
    }
    else
    {
        perror("Error al crear el proceso de monitoreo");
    }
}

/**
 * @brief Detiene el proceso de monitorización.
 *
 * Envía una señal SIGKILL al proceso del monitor para forzar su terminación y
 * espera a que el proceso hijo termine. Si el monitor no está en ejecución,
 * informa al usuario.
 */
void stop_monitor()
{
    if (monitor_pid > 0)
    {
        kill(monitor_pid, SIGKILL);    ///< Enviar señal SIGKILL al monitor
        waitpid(monitor_pid, NULL, 0); ///< Esperar a que el monitor termine
        monitor_pid = -1;
        printf("Monitor detenido.\n");
    }
    else
    {
        printf("Monitor no está en ejecución.\n");
    }
}

/**
 * @brief Informa sobre el estado del monitor.
 *
 * Imprime el estado actual del monitor. Indica si el monitor está en ejecución y su PID,
 * o muestra un mensaje si el monitor no está en ejecución.
 */
void status_monitor()
{
    if (monitor_pid > 0)
    {
        printf("Monitor en ejecución con PID %d.\n", monitor_pid);
    }
    else
    {
        printf("Monitor no está en ejecución.\n");
    }
}
