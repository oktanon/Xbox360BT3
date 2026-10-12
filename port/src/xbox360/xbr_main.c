/*
 * port/src/xbox360/xbr_main.c
 *
 * Entry point del port a Xbox 360 (PowerPC Xenon, big-endian, 32 bits).
 *
 * Historia:
 *   - 1.2 (fase 1): main() vacio, smoke test "hello XEX".
 *   - 1.3 (fase 1): main() llama a Xbr_Init() y deja la capa de plataforma
 *                    lista. En 1.4 anadiremos la llamada a Game_Main()
 *                    del juego (src/main.c) y los stubs de los subsistemas
 *                    que preceden a Gfx_Init().
 *
 * El entry point del XEX se registra en el Linker (propiedades del proyecto);
 * por defecto es main(). El startup del XDK (xapilib.lib) llama a main() una
 * vez inicializadas las estructuras del runtime (heap, threads, etc.).
 */
#include "xbox360/xbr_platform.h"

#include <xtl.h>

void __cdecl main(void) {
    Xbr_Init();
    Xbr_Log("Xbr_main: hello Xbox 360\n");

    /* En 1.4 sustituimos este bucle por Game_Main(). Mientras tanto, si
     * el XEX se ejecuta en un emulador o devkit, vemos "Xbr_Init" y
     * "Xbr_main: hello Xbox 360" en la salida de OutputDebugStringA. */
    for (;;) {
        __asm { nop }
    }
}
