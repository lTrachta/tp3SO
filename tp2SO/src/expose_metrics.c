#include <expose_metrics.h>
#include <metrics.h> // Necesario para usar get_cpu_usage() y get_memory_usage()
#include <stdlib.h>  // Para funciones de utilidad general

/** Mutex para sincronización de hilos */
pthread_mutex_t lock;

/** Métrica de Prometheus para el uso de CPU */
static prom_gauge_t* cpu_usage_metric;

/** Métrica de Prometheus para el uso de memoria */
static prom_gauge_t* memory_usage_metric;

/** Métrica de Prometheus para las lecturas de sectores en disco */
prom_gauge_t* disk_io_read_metric;

/** Métrica de Prometheus para las escrituras de sectores en disco */
prom_gauge_t* disk_io_write_metric;

/** Métrica de Prometheus para los bytes recibidos en la red */
prom_gauge_t* net_rx_metric;

/** Métrica de Prometheus para los bytes transmitidos en la red */
prom_gauge_t* net_tx_metric;

/** Métrica de Prometheus para el conteo de procesos en ejecución */
prom_gauge_t* process_count_metric;

/** Métrica de Prometheus para el número de cambios de contexto */
prom_gauge_t* context_switch_metric;

/** Métrica de Prometheus para la tasa de fragmentación */
static prom_gauge_t* fragmentation_rate_metric;

/** Métrica de Prometheus para la frecuencia de uso de cada política */
static prom_gauge_t* policy_usage_metric;

/** Métrica de Prometheus para la eficiencia de cada política */
static prom_gauge_t* policy_efficiency_metric;

/**
 * @brief Actualiza la métrica de uso de CPU.
 *
 * Esta función obtiene el porcentaje de uso de CPU y actualiza la métrica
 * correspondiente en Prometheus. Se utiliza un mutex para asegurar la
 * sincronización en entornos de múltiples hilos.
 */
void update_cpu_gauge()
{
    double usage = get_cpu_usage();
    if (usage >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(cpu_usage_metric, usage, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el uso de CPU\n");
    }
}

/**
 * @brief Actualiza la métrica de uso de memoria.
 *
 * Esta función obtiene el porcentaje de uso de memoria y actualiza la métrica
 * correspondiente en Prometheus. Se utiliza un mutex para asegurar la
 * sincronización en entornos de múltiples hilos.
 */
void update_memory_gauge()
{
    double usage = get_memory_usage();
    if (usage >= 0)
    {
        pthread_mutex_lock(&lock);
        prom_gauge_set(memory_usage_metric, usage, NULL);
        pthread_mutex_unlock(&lock);
    }
    else
    {
        fprintf(stderr, "Error al obtener el uso de memoria\n");
    }
}

/**
 * @brief Función del hilo para exponer las métricas vía HTTP en el puerto 8000.
 *
 * Esta función inicializa un servidor HTTP para exponer las métricas de
 * Prometheus. Se ejecuta en un bucle infinito hasta que se interrumpe.
 *
 * @param arg Argumento no utilizado.
 * @return NULL
 */
void* expose_metrics(void* arg)
{
    (void)arg; // Argumento no utilizado

    // Aseguramos que el manejador HTTP esté adjunto al registro por defecto
    promhttp_set_active_collector_registry(NULL);

    // Iniciamos el servidor HTTP en el puerto 8000
    struct MHD_Daemon* daemon = promhttp_start_daemon(MHD_USE_SELECT_INTERNALLY, 8000, NULL, NULL);
    if (daemon == NULL)
    {
        return NULL;
    }

    // Mantenemos el servidor en ejecución
    while (1)
    {
        sleep(1);
    }

    // Nunca debería llegar aquí
    MHD_stop_daemon(daemon);
    return NULL;
}

/**
 * @brief Inicializa el mutex y las métricas de Prometheus.
 *
 * Esta función inicializa el mutex utilizado para la sincronización de hilos,
 * así como las métricas de CPU, memoria, I/O de disco, estadísticas de red,
 * conteo de procesos y cambios de contexto.
 */
void init_metrics()
{
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        fprintf(stderr, "Error al inicializar el mutex\n");
        return;
    }
    printf("Mutex inicializado correctamente.\n");

    if (prom_collector_registry_default_init() != 0)
    {
        fprintf(stderr, "Error al inicializar el registro de Prometheus\n");
        return;
    }
    printf("Registro de Prometheus inicializado correctamente.\n");

    cpu_usage_metric = prom_gauge_new("cpu_usage_percentage", "Porcentaje de uso de CPU", 0, NULL);
    memory_usage_metric = prom_gauge_new("memory_usage_percentage", "Porcentaje de uso de memoria", 0, NULL);
    disk_io_read_metric = prom_gauge_new("disk_io_read_sectors", "Lecturas de sectores en disco", 0, NULL);
    disk_io_write_metric = prom_gauge_new("disk_io_write_sectors", "Escrituras de sectores en disco", 0, NULL);
    net_rx_metric = prom_gauge_new("network_rx_bytes", "Bytes recibidos en la red", 0, NULL);
    net_tx_metric = prom_gauge_new("network_tx_bytes", "Bytes transmitidos en la red", 0, NULL);
    process_count_metric = prom_gauge_new("process_count", "Cantidad de procesos en ejecución", 0, NULL);
    context_switch_metric = prom_gauge_new("context_switches", "Número de cambios de contexto", 0, NULL);

    // Nuevas métricas
    fragmentation_rate_metric = prom_gauge_new("fragmentation_rate", "Tasa de fragmentación de memoria", 0, NULL);
    policy_usage_metric = prom_gauge_new("policy_usage", "Frecuencia de uso de políticas de asignación", 1, (const char*[]){"policy"});
    policy_efficiency_metric = prom_gauge_new("policy_efficiency", "Eficiencia de las políticas de asignación", 1, (const char*[]){"policy"});

    if (!cpu_usage_metric || !memory_usage_metric || !disk_io_read_metric || !disk_io_write_metric ||
        !net_rx_metric || !net_tx_metric || !process_count_metric || !context_switch_metric ||
        !fragmentation_rate_metric || !policy_usage_metric || !policy_efficiency_metric)
    {
        fprintf(stderr, "Error al crear una o más métricas\n");
        return;
    }

    prom_collector_registry_must_register_metric(cpu_usage_metric);
    prom_collector_registry_must_register_metric(memory_usage_metric);
    prom_collector_registry_must_register_metric(disk_io_read_metric);
    prom_collector_registry_must_register_metric(disk_io_write_metric);
    prom_collector_registry_must_register_metric(net_rx_metric);
    prom_collector_registry_must_register_metric(net_tx_metric);
    prom_collector_registry_must_register_metric(process_count_metric);
    prom_collector_registry_must_register_metric(context_switch_metric);
    prom_collector_registry_must_register_metric(fragmentation_rate_metric);
    prom_collector_registry_must_register_metric(policy_usage_metric);
    prom_collector_registry_must_register_metric(policy_efficiency_metric);

    printf("Todas las métricas se registraron correctamente.\n");
}

/**
 * @brief Destruye el mutex utilizado para la sincronización de hilos.
 *
 * Esta función libera los recursos asociados al mutex, asegurando que no haya
 * fugas de memoria ni problemas de sincronización en hilos posteriores.
 */
void destroy_mutex()
{
    pthread_mutex_destroy(&lock);
}
