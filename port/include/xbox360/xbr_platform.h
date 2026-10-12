/*
 * port/include/xbox360/xbr_platform.h
 *
 * API publica de la capa de plataforma del port a Xbox 360 (PowerPC Xenon,
 * big-endian, 32 bits). Vivir en port/include/xbox360/ cumple la regla 1 de
 * AGENTS.md: el codigo especifico de 360 no se mezcla con el del port de PC.
 *
 * El prefijo Xbr_ (Xbox port) evita choques con los XbrUtils del XDK u
 * otros simbolos que el XDK pueda exportar.
 */
#ifndef XBOX360_XBR_PLATFORM_H
#define XBOX360_XBR_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Inicializa la capa de plataforma. Llamada por el entry point (main) antes
 * de saltar a la logica del juego. Imprime una linea de presentacion y deja
 * los servicios basicos (Xbr_Log, stubs sce*) listos para usar.
 *
 * En fases posteriores anadira: reserva de heap, hilo principal, pool de
 * hilos, sockets para la red, montaje del sistema de archivos.
 */
void Xbr_Init(void);

/* Log con formato estilo printf. El mensaje resultante se envia a
 * OutputDebugStringA del XDK, visible en xbWatson/PIX/xenia-canary en
 * depuracion. La cadena de formato debe terminar con '\n'.
 *
 * El buffer interno es de 1024 bytes (corte en overflow sin truncar el
 * programa; no se pretende tener logs extensos en Xbox 360).
 */
void Xbr_Log(const char *fmt, ...);

/* Cierre de la capa de plataforma. Por ahora no-op: el XAPILIB se encarga
 * del shutdown del runtime al volver de main(). En fases posteriores
 * liberara recursos propios (heap, sockets, hilos).
 */
void Xbr_Shutdown(void);

#ifdef __cplusplus
}
#endif

#endif
