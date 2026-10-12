/*
 * port/include/xbox360/xbr_input.h
 *
 * API de input (subsistema 2) del port a Xbox 360. Reemplaza la
 * libpad2/libdbc/libvib de PS2 por XInput. Las firmas de las
 * funciones sce* son las mismas que aparecen como `extern` en
 * src/sys/pad.c (fase 1), asi cuando en fases futuras se compile
 * src/sys/pad.c los enlaces casan sin mas.
 *
 * Regla 1 de AGENTS.md: este codigo vive en port/src/xbox360/ y
 * src/sys/pad.h no se modifica.
 */
#ifndef XBOX360_XBR_INPUT_H
#define XBOX360_XBR_INPUT_H

#include <xtl.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Inicializa la capa XInput. Llamada por Xbr_Init en el flujo de
 * arranque (subsistema 2 input). Tambien corre un auto-test que
 * valida que XInputGetState responde con ERROR_SUCCESS o
 * ERROR_DEVICE_NOT_CONNECTED (segun haya o no pad) y loguea
 * "input self-test OK" o el motivo del FAIL. */
void Xbr_InputInit(void);

/* libdbc / libpad2 / libvib (PS2): prototipos con la misma firma
 * que el decomp del juego en src/sys/pad.c. */
int  sceDbcInit(void);
int  scePad2Init(int mode);
int  scePad2CreateSocket(void *param, void *work);
int  scePad2GetState(int socket);
int  scePad2GetButtonProfile(int socket, void *profile);
int  scePad2Read(int socket, unsigned char *data);
int  sceVibGetProfile(int socket, unsigned char *profile);
int  sceVibSetActParam(int socket, int profileSize, unsigned char *profile,
                        int dataSize, void *data);

#ifdef __cplusplus
}
#endif

#endif
