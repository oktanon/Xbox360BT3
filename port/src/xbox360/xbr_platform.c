/*
 * port/src/xbox360/xbr_platform.c
 *
 * Implementacion de la capa de plataforma del port a Xbox 360.
 * Ver xbr_platform.h para el contrato publico. Cada subsistema (archivos,
 * input, renderer, sonido, etc.) se monta encima de este modulo.
 */
#include "xbox360/xbr_platform.h"
#include "xbox360/xbr_endian.h"
#include "xbox360/xbr_input.h"

#include <xtl.h>
#include <stdarg.h>
#include <stdio.h>

void Xbr_Init(void) {
    /* El runtime del XDK (xapilib.lib) ya inicializo memoria, hilos y el
     * kernel antes de llamar a main(). Xbr_Init deja la capa lista para
     * que main() salte a Game_Main() en la subtarea 1.4. */
    Xbr_Log("Xbr_Init: port platform ready (XDK 21256.3, PowerPC Xenon, big-endian)\n");
    /* Auto-test de endianness: corre las primitivas con patrones
     * conocidos y loguea OK/FAIL. Asi el log de xenia confirma que la
     * conversion LE -> BE es correcta antes de tocar datos reales. */
    Xbr_EndianSelfTest();
    /* Inicializa la capa XInput. Auto-test integrado: sondea puerto
     * 0 y loguea si hay pad o no. */
    Xbr_InputInit();
}

void Xbr_Log(const char *fmt, ...) {
    char buf[1024];
    va_list args;
    int n;

    va_start(args, fmt);
    /* _vsnprintf es la version de la RTL del XDK; trunca en sizeof(buf)-1 y
     * cierra la cadena. La firma no devuelve -1, pero dejamos la guarda por
     * si en una version futura del runtime cambia el contrato. */
    n = _vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    if (n < 0) {
        buf[sizeof(buf) - 1] = '\0';
    }
    OutputDebugStringA(buf);
}

void Xbr_Shutdown(void) {
    /* XAPILIB libera memoria, hilos y handles al volver de main(); no hay
     * recursos propios que cerrar todavia. La forma final aparecera cuando
     * los subsistemas 1-7 (regla 3 de AGENTS.md) asignen recursos fuera del
     * heap de XAPILIB. */
}
