#include <filesystem.h>

/**
 * @brief Función auxiliar para verificar si un archivo tiene una extensión específica.
 *
 * @param filename Nombre del archivo a verificar.
 * @param extension Extensión buscada (sin punto, por ejemplo, "config" o "json").
 * @return 1 si el archivo tiene la extensión, 0 en caso contrario.
 */
static int has_extension(const char *filename, const char *extension) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) {
        return 0; // No tiene extensión
    }
    return strcmp(dot + 1, extension) == 0;
}

/**
 * @brief Busca archivos con una extensión específica dentro de un directorio y subdirectorios.
 *
 * @param path Ruta del directorio a explorar.
 * @param extension Extensión de los archivos a buscar (por ejemplo, "config" o "json").
 */
void search_directory(const char *path, const char *extension) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Error al abrir el directorio");
        return;
    }

    struct dirent *entry;
    struct stat entry_info;
    char full_path[PATH_MAX];

    printf("Explorando el directorio: %s en busca de archivos '.%s'\n", path, extension);

    while ((entry = readdir(dir)) != NULL) {
        // Ignorar las entradas especiales "." y ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, PATH_MAX, "%s/%s", path, entry->d_name);
        if (stat(full_path, &entry_info) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        if (S_ISDIR(entry_info.st_mode)) {
            // Si es un subdirectorio, realizar búsqueda recursiva
            search_directory(full_path, extension);
        } else if (S_ISREG(entry_info.st_mode) && has_extension(entry->d_name, extension)) {
            // Si es un archivo regular con la extensión correcta
            printf("Archivo de configuración encontrado: %s\n", full_path);

            // Verificar acceso al archivo
            if (access(full_path, R_OK) != 0) {
                printf("No se tiene permiso para leer el archivo: %s\n", full_path);
                continue;
            }

            // Leer y mostrar contenido del archivo
            FILE *file = fopen(full_path, "r");
            if (file) {
                char line[256];
                printf("Contenido de %s:\n", full_path);
                while (fgets(line, sizeof(line), file)) {
                    printf("%s", line);
                }
                fclose(file);
                printf("\n");
            } else {
                perror("Error al abrir el archivo");
            }
        }
    }

    closedir(dir);
}
