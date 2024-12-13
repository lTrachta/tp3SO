/**
 * @file shell.c
 * @brief Implementa las funcionalidades principales de la shell, como comandos internos, manejo de señales, ejecución
 * de comandos externos, pipes y redirección.
 */

#include <shell.h>

int foreground_pid = 0; /**< PID del proceso en primer plano */
int job_count = 0;      /**< Contador de trabajos en segundo plano */

/**
 * @brief Maneja las señales SIGINT, SIGTSTP y SIGQUIT.
 *
 * Si hay un proceso en primer plano, redirige la señal a dicho proceso. Si no hay ningún proceso en primer plano,
 * muestra un mensaje indicando que no hay proceso para recibir la señal.
 *
 * @param sig Señal recibida (por ejemplo, SIGINT, SIGTSTP o SIGQUIT).
 */
void manejar_senal(int sig)
{
    if (foreground_pid > 0)
    {
        kill(foreground_pid, sig); /**< Redirige la señal al proceso en primer plano */
        printf("\n");
    }
    else
    {
        switch (sig)
        {
        case SIGINT:
            printf("\n");
            printf("No hay proceso en primer plano para detener (SIGINT)\n");
            break;
        case SIGTSTP:
            printf("\n");
            printf("No hay proceso en primer plano para detener (SIGTSTP)\n");
            break;
        case SIGQUIT:
            printf("\n");
            printf("No hay proceso en primer plano para detener (SIGQUIT)\n");
            break;
        default:
            break;
        }
    }
}

/**
 * @brief Inicializa el manejador de señales para SIGINT, SIGTSTP y SIGQUIT.
 *
 * Configura el manejador de señales usando la función `manejar_senal` para que se ejecute cuando
 * se reciban señales específicas.
 */
void iniciar_manejador_senales()
{
    struct sigaction sa;
    sa.sa_handler = manejar_senal; /**< Asocia el manejador de señal */
    sa.sa_flags = SA_RESTART;      /**< Reinicia llamadas interrumpidas por señales */
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

/**
 * @brief Cambia el directorio de trabajo actual.
 *
 * Cambia el directorio al especificado. Si el directorio es `~`, cambia al directorio del usuario.
 * Si el directorio es `-`, cambia al último directorio visitado (OLDPWD).
 *
 * @param directorio Directorio al que se desea cambiar.
 */
void ejecutar_cd(char* directorio)
{
    char* oldpwd = getenv("PWD"); // Obtiene el directorio actual
    char cwd[HOST_NAME_MAX];      // Buffer para el nuevo directorio

    if (directorio == NULL || strcmp(directorio, "~") == 0)
    {
        // Si no hay argumento o es "~", cambia al home
        directorio = getenv("HOME");
    }
    else if (strcmp(directorio, "-") == 0)
    {
        // Si es "-", cambia al último directorio (OLDPWD)
        directorio = getenv("OLDPWD");
        if (directorio == NULL)
        {
            printf("OLDPWD no está definido\n");
            return;
        }
        printf("Cambiando a: %s\n", directorio); // Imprime el directorio al que va
    }

    printf("Intentando cambiar al directorio: %s\n", directorio); // Debugging

    if (chdir(directorio) == 0)
    {
        if (oldpwd != NULL)
        {
            setenv("OLDPWD", oldpwd, 1);
        }

        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            setenv("PWD", cwd, 1);
            printf("Nuevo directorio: %s\n", cwd); // Debugging
        }
        else
        {
            perror("Error obteniendo el directorio actual");
        }
    }
    else
    {
        perror("Error al cambiar de directorio");
    }
}

/**
 * @brief Limpia la pantalla de la terminal.
 *
 * Utiliza secuencias de escape ANSI para mover el cursor a la esquina superior izquierda (\033[H)
 * y limpiar la pantalla (\033[J).
 */
void ejecutar_clr(void)
{
    printf("\033[H\033[J"); /**< Comando ANSI para limpiar la pantalla */
}

/**
 * @brief Imprime un mensaje en la terminal o muestra el valor de una variable de entorno.
 *
 * Si el mensaje comienza con `$`, se interpreta como una variable de entorno y muestra su valor.
 * De lo contrario, imprime el mensaje directamente.
 *
 * @param mensaje Mensaje o nombre de variable de entorno a mostrar.
 */
void ejecutar_echo(char* mensaje)
{
    if (mensaje[0] == '$')
    {
        char* var = getenv(mensaje + 1); /**< Salta el primer carácter `$` */
        if (var)
        {
            printf("%s\n", var); /**< Muestra el valor de la variable de entorno */
        }
        else
        {
            printf("Variable de entorno no encontrada\n");
        }
    }
    else
    {
        printf("%s", mensaje); /**< Imprime el mensaje si no es una variable de entorno */
    }
}

/**
 * @brief Sale de la shell.
 */
void ejecutar_quit(void)
{
    printf("Saliendo de la shell...\n");
    exit(0); /**< Sale de la shell */
}

/**
 * @brief Lee y ejecuta un comando, detectando redirección, pipes y ejecución en segundo plano.
 *
 * La función verifica si el comando contiene redirección, pipes o se ejecutará en segundo plano.
 * Ejecuta el comando como un comando interno o como un programa externo según corresponda.
 *
 * @param comando Comando a ejecutar.
 */
void leer_y_ejecutar_comando(char* comando)
{
    if (strchr(comando, '<') || strchr(comando, '>'))
    {
        ejecutar_con_redireccion(comando);
        return;
    }

    if (strchr(comando, '|') != NULL)
    {
        ejecutar_comando_con_pipes(comando); /**< Manejo de comandos con pipes */
    }
    else
    {
        char* args[HOST_NAME_MAX]; /**< Array para almacenar argumentos */
        int i = 0;
        int background = 0;

        size_t len = strlen(comando);
        if (len > 0 && comando[len - 1] == '&')
        {
            background = 1;
            comando[len - 1] = '\0'; /**< Remueve '&' del comando */
        }

        char* token = strtok(comando, " ");
        while (token != NULL && i < HOST_NAME_MAX)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if ((strcmp(args[0], "clear") == 0) && background == 0)
        {
            ejecutar_clr();
        }
        else if (strcmp(args[0], "exit") == 0 && background == 0)
        {
            ejecutar_quit();
        }
        else if (strcmp(args[0], "cd") == 0 && background == 0)
        {
            printf("Argumento para cd: %s\n", args[1]);
            if (args[1] == NULL || strlen(args[1]) == 0)
            {
                // Si no hay argumento o está vacío, cambiar al directorio HOME
                ejecutar_cd(NULL); // Usamos NULL para que la función cd use el directorio HOME
            }
            else
            {
                ejecutar_cd(args[1]); // Usamos el directorio que el usuario pasa
            }
        }
        else if (strcmp(args[0], "echo") == 0 && background == 0)
        {
            for (int j = 1; j < i; j++)
            {
                ejecutar_echo(args[j]);
                if (j < i - 1)
                {
                    printf(" ");
                }
            }
            printf("\n");
        }
        else
        {
            pid_t pid = fork(); /**< Crear proceso hijo */
            if (pid == 0)
            {
                if (execvp(args[0], args) == -1)
                {
                    perror("No es ningun comando cd, echo, exit o clear");
                }
                exit(EXIT_FAILURE);
            }
            else if (pid > 0)
            {
                if (background)
                {
                    job_count++;
                    printf("[%d] %d\n", job_count, pid);
                }
                else
                {
                    foreground_pid = pid;
                    int status;
                    while (waitpid(pid, &status, WUNTRACED) > 0)
                    {
                        if (WIFSTOPPED(status))
                        {
                            foreground_pid = -1;
                            break;
                        }
                    }
                }
            }
            else
            {
                perror("Error al crear el proceso");
            }
        }
    }
}

/**
 * @brief Ejecuta un comando con pipes.
 *
 * Divide el comando en subcomandos separados por pipes y los ejecuta conectando la salida
 * de un subcomando con la entrada del siguiente.
 *
 * @param comando Comando que contiene pipes.
 */
void ejecutar_comando_con_pipes(char* comando)
{
    char* subcomandos[HOST_NAME_MAX];
    int num_subcomandos = 0;

    char* token = strtok(comando, "|");
    while (token != NULL && num_subcomandos < HOST_NAME_MAX)
    {
        subcomandos[num_subcomandos++] = token;
        token = strtok(NULL, "|");
    }

    int pipefd[2];
    int fd_in = 0;

    for (int i = 0; i < num_subcomandos; i++)
    {
        pipe(pipefd);
        pid_t pid = fork();

        if (pid == 0)
        {
            dup2(fd_in, 0);
            if (i < num_subcomandos - 1)
            {
                dup2(pipefd[1], 1);
            }
            close(pipefd[0]);
            leer_y_ejecutar_comando(subcomandos[i]);
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            wait(NULL);
            close(pipefd[1]);
            fd_in = pipefd[0];
        }
        else
        {
            perror("Error en la creación del proceso para los pipes");
        }
    }
}

/**
 * @brief Ejecuta un comando con redirección de entrada y salida.
 *
 * Esta función procesa un comando que puede incluir redirección de entrada
 * (`<`) o de salida (`>`), y ejecuta el comando con los archivos especificados
 * para la entrada y/o salida. La redirección se realiza mediante los descriptores
 * de archivo `STDIN_FILENO` y `STDOUT_FILENO`. Si se encuentran errores al
 * abrir los archivos de entrada o salida, se reportan mediante `perror` y el
 * proceso hijo termina con un error.
 *
 * @param comando El comando a ejecutar con redirección. El comando debe ser
 *                una cadena que contenga el comando seguido de posibles
 *                operadores de redirección (`<` y `>`), y los archivos
 *                correspondientes.
 */
void ejecutar_con_redireccion(char* comando)
{
    char* args[HOST_NAME_MAX];
    char* input_file = NULL;
    char* output_file = NULL;
    int i = 0;

    char* token = strtok(comando, " ");
    while (token != NULL && i < HOST_NAME_MAX)
    {
        if (strcmp(token, "<") == 0)
        {
            // Captura el archivo de entrada
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                input_file = token;
            }
        }
        else if (strcmp(token, ">") == 0)
        {
            // Captura el archivo de salida
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                output_file = token;
            }
        }
        else
        {
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // Terminar la lista de argumentos

    pid_t pid = fork();
    if (pid == 0)
    {
        // Proceso hijo: configurar redirección si se requiere
        if (input_file)
        {
            int in_fd = open(input_file, O_RDONLY);
            if (in_fd == -1)
            {
                perror("Error al abrir archivo de entrada");
                exit(EXIT_FAILURE);
            }
            dup2(in_fd, STDIN_FILENO); // Redirige stdin
            close(in_fd);
        }
        if (output_file)
        {
            int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd == -1)
            {
                perror("Error al abrir archivo de salida");
                exit(EXIT_FAILURE);
            }
            dup2(out_fd, STDOUT_FILENO); // Redirige stdout
            close(out_fd);
        }

        // Ejecutar el comando con redirección aplicada
        if (execvp(args[0], args) == -1)
        {
            perror("Error al ejecutar el comando");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid > 0)
    {
        // Proceso padre: esperar al hijo
        int status;
        waitpid(pid, &status, 0);
    }
    else
    {
        perror("Error al crear el proceso");
    }
}
