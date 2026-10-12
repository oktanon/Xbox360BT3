/*
 * port/src/xbox360/xbr_main.c
 *
 * Entry point del port a Xbox 360 (PowerPC Xenon, big-endian, 32 bits).
 *
 * Esta fase (subtarea 1.2 de la fase 1) solo quiere validar que el toolchain
 * y el .vcxproj producen un XEX valido. En 1.3 se sustituye por Xbr_Init() y
 * lazo principal; en 1.4 por la llamada a Game_Main() del juego.
 *
 * El entry point del XEX se registra en el Linker (propiedades del proyecto):
 * por defecto es main(). El startup del XDK (xapilib.lib) llama a main() una
 * vez inicializadas las estructuras del runtime (heap, threads, etc.).
 */
#include <xtl.h>

void __cdecl main(void) {
    /* Smoke test: si el XEX llega aqui, el toolchain esta OK. */
    for (;;) {
        /* Sin log todavia; se anade Xbr_Log en 1.3. */
        __asm { nop }
    }
}
