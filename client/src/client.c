#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char *ip;
	char *puerto;
	char *valor;

	t_log *logger;
	t_config *config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");
	// Loggeamos el valor de config
	log_info(logger, "Log de la ip,el puerto y el valor");
	
	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	handshake(conexion);
	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);
	
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log *iniciar_logger(void)
{
	t_log *nuevo_logger;
	if ((nuevo_logger = log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO)) == NULL)
	{
		printf("error");
		exit(1);
	}
	log_info(nuevo_logger, "hola me llamo es brian");
	return nuevo_logger;
}

t_config *iniciar_config(void)
{
	t_config *nuevo_config = config_create("./cliente.config");
	if ((nuevo_config == NULL))
	{
		printf("error");
		exit(1);
	}
	return nuevo_config;
}

void leer_consola(t_log *logger)
{
	char *linea;
	linea = readline(">");
	while (strncmp(linea, "", 3))
	{
		log_info(logger, linea);
		free(linea);
		linea = readline(">");
	}
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char *leido;
	t_paquete *paquete = crear_paquete();
	leido = readline(">");
	// Leemos y esta vez agregamos las lineas al paquete
	while(strncmp(leido, "", 3)){
		agregar_a_paquete(paquete, leido, sizeof(leido));
		free(leido);
		leido = readline(">");
	}
	enviar_paquete(paquete,conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log *logger, t_config *config)
{
	if (logger != NULL)
		log_destroy(logger);

	if (config != NULL)
		config_destroy(config);

	//printf("%s\n", "llegue");
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config)
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}

void handshake (int socket_conexion)
{
	size_t bytes;

	int32_t handshake = 1;
	int32_t result;

	bytes = send(socket_conexion, &handshake, sizeof(int32_t), 0);
	bytes = recv(socket_conexion, &result, sizeof(int32_t), MSG_WAITALL);

	if (result == 0) {
   	  printf("yipeeee");// Handshake OK
	} else {
	  printf("nope");	      		// Handshake ERROR
	}
}