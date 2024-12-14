#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>       // Para FILE, printf, NULL
#include <stdlib.h>      // Para malloc, free, NULL
#include <string.h>      // Para strcmp, strrchr
#include <dirent.h>      // Para DIR, struct dirent, opendir, readdir
#include <sys/stat.h>    // Para struct stat, stat
#include <limits.h>      // Para PATH_MAX
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096 // Define PATH_MAX si no está definido
#endif

/**
 * @brief Busca archivos con una extensión específica dentro de un directorio y sus subdirectorios.
 *
 * @param path Ruta del directorio a explorar.
 * @param extension Extensión de los archivos a buscar (por ejemplo, "config" o "json").
 */
void search_directory(const char *path, const char *extension);

#endif // FILESYSTEM_H
