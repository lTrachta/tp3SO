/**
 * @file test_pipes.c
 * @brief Archivo de pruebas para funciones relacionadas con la comunicación mediante pipes.
 *
 * Este archivo contiene pruebas unitarias para verificar el correcto funcionamiento
 * de la inicialización de pipes y la interacción con el monitor usando pipes.
 */

#include <test_pipes.h>

// Variables globales necesarias para las pruebas
extern int pipe_to_monitor[2];   /**< Pipe para enviar datos al monitor */
extern int pipe_from_monitor[2]; /**< Pipe para recibir datos del monitor */
extern pid_t monitor_pid;        /**< PID del proceso del monitor */

// Simulación de las funciones relacionadas con el monitor
extern void iniciar_pipes(void);
extern void start_monitor(void);
extern void stop_monitor(void);
extern void status_monitor(void);

/**
 * @brief Función auxiliar para capturar la salida de una función que imprime a stdout.
 *
 * Esta función redirige stdout a un buffer utilizando un pipe. Ejecuta una función dada
 * y captura su salida en el buffer.
 *
 * @param buffer Buffer donde se almacenará la salida capturada.
 * @param size Tamaño del buffer.
 * @param func Función a ejecutar cuya salida será capturada.
 * @return 1 si la captura fue exitosa, -1 si ocurrió un error.
 */
static int capture_stdout(char* buffer, size_t size, void (*func)(void))
{
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0)
    {
        // Cerrar el lado de lectura del pipe en el proceso hijo
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        func(); // Ejecutar la función que se quiere probar
        close(fd[1]);
        exit(0);
    }
    else if (pid > 0)
    {
        // Cerrar el lado de escritura del pipe en el proceso padre
        close(fd[1]);
        waitpid(pid, NULL, 0); // Esperar a que termine el hijo
        ssize_t n = read(fd[0], buffer, size - 1);
        buffer[n] = '\0'; // Añadir terminador nulo al buffer
        close(fd[0]);
        return 1;
    }
    else
    {
        perror("Error al hacer fork");
        return -1;
    }
}

/**
 * @brief Prueba para verificar la correcta inicialización de los pipes de comunicación con el monitor.
 *
 * Esta función llama a `iniciar_pipes()` y luego verifica si los pipes fueron creados correctamente.
 *
 * @return 0 si la prueba pasa, 1 si falla.
 */
int test_iniciar_pipes(void)
{
    iniciar_pipes();

    // Comprobar si los pipes se crearon correctamente
    if (pipe_to_monitor[0] != -1 && pipe_to_monitor[1] != -1 && pipe_from_monitor[0] != -1 &&
        pipe_from_monitor[1] != -1)
    {
        printf("Test de iniciar_pipes: PASADO\n");
        return 0; // Retorna 0 si el test pasa
    }
    else
    {
        printf("Test de iniciar_pipes: FALLADO\n");
        return 1; // Retorna 1 si el test falla
    }
}
