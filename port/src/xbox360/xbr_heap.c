/*
 * port/src/xbox360/xbr_heap.c
 *
 * Heap del juego en Xbox 360. El juego (src/sys/heap.c) espera un par de
 * punteros globales `gHeapStart[HEAP_COUNT]`, `gHeapEnd[...]`, `gHeapSize[...]`
 * que delimitan uno o dos bloques contiguos de memoria gestionados por una
 * lista enlazada de HeapBlock. Aqui los definimos y los inicializamos en
 * Heap_Init() con un malloc de 32 MB, que es la cantidad que el juego
 * espera (heap unico de 0x1C219000 bytes en la build original, ~28 MB).
 *
 * En Xbox 360 no necesitamos mapear memoria a direcciones PS2 fijas: el
 * espacio de 32 bits del proceso es plano, asi que un malloc de 32 MB
 * nos sirve tal cual. Si el malloc falla (improbable con 512 MB de RAM
 * total), Heap_Init loguea y deja los punteros a NULL; las subsiguientes
 * llamadas a Heap_Alloc/Heap_Free devolveran NULL / seran no-op, que es
 * el mismo comportamiento que el port de PC cuando la reserva falla.
 *
 * Regla 1 de AGENTS.md: este codigo vive en port/src/xbox360/ y no
 * modifica src/ ni include/. El juego declara estos simbolos como `extern`
 * en src/sys/heap.c y los consume directamente.
 */
#include "xbox360/xbr_platform.h"

#include <xtl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* common.h arrastra types.h (s8/s16/s32/s64/u8/u16/u32/u64/f32). El
 * include_asm.h que arrastra common.h se salta entero: INCLUDE_ASM_H
 * esta definido por xbr_compat.h (ver xbr_compat.h).
 * sys/heap.h viene aparte y aporta HeapBlock + los prototipos
 * Heap_Init/Heap_Alloc/Heap_Free. */
#include "common.h"
#include "sys/heap.h"

/* 32 MB: tamaño total del heap de juego en PS2. */
#define XBR_HEAP_BYTES (32u * 1024u * 1024u)

/* Simbolos globales que src/sys/heap.c referencia como `extern`. La
 * definicion de los arrays vive aqui (no en el codigo del juego). */
HeapBlock *gHeapStart[HEAP_COUNT] = { NULL, NULL };
HeapBlock *gHeapEnd[HEAP_COUNT]   = { NULL, NULL };
int32_t   gHeapSize[HEAP_COUNT]   = { 0, 0 };

/* Otros simbolos que el juego usa (ver src/sys/heap.c, lineas 12-16):
 * gHeapUnk70 / gHeapUnk78 se escriben pero casi nunca se leen. */
int32_t gHeapUnk70[HEAP_COUNT] = { 0, 0 };
int32_t gHeapUnk78[HEAP_COUNT] = { 0, 0 };

/* gHeapDebugValue / gHeapDebugFlag: no se leen nunca, solo se escriben. */
int32_t gHeapDebugValue = 0;
uint8_t gHeapDebugFlag = 0;

/* D_3BE71C es la zona del juego que se sobreescribe con datos del
 * disco (overlay slot, etc.). En el port de PC el malloc de Port_Malloc
 * cae justo despues de este simbolo. Aqui reservamos 0x2000 bytes (8 KB)
 * para no chocar si algo lo escribe. */
uint8_t D_3BE71C[0x2000] = { 0 };

void Heap_Init(void) {
    static int logged;
    void *buf;

    if (logged) {
        return;
    }
    logged = 1;

    buf = malloc(XBR_HEAP_BYTES);
    if (buf == NULL) {
        Xbr_Log("Xbr_Log: Heap_Init FAILED (malloc %u bytes)\n", XBR_HEAP_BYTES);
        return;
    }

    /* Un unico bloque libre que cubre todo el buffer. El juego espera que
     * `used == 0` y `data == 0` para bloques libres; el trailer al final
     * del bloque (`size` repetido) lo rellena el propio juego al alloc. */
    {
        HeapBlock *block = (HeapBlock *)buf;
        block->magic    = HEAP_MAGIC;
        block->used     = 0;
        block->fromTail = 0;
        block->align    = 0x20;
        block->size     = (int32_t)XBR_HEAP_BYTES;
        block->data     = 0;
        block->dataSize = (int32_t)XBR_HEAP_BYTES - 0x20;
        block->unk1C    = 0;
        /* Trailer (repite el size al final del bloque). */
        *(int32_t *)((uint8_t *)buf + XBR_HEAP_BYTES - 4) = (int32_t)XBR_HEAP_BYTES;
    }

    gHeapStart[0] = (HeapBlock *)buf;
    gHeapEnd[0]   = (HeapBlock *)((uint8_t *)buf + XBR_HEAP_BYTES);
    gHeapSize[0]  = (int32_t)XBR_HEAP_BYTES;

    /* Heap 1 sin usar. El juego lo recorre pero la mayoria del codigo
     * (ver src/sys/heap.c) tolera punteros NULL: compara `block !=
     * gHeapStart[1]` con un bloque cualquiera, lo cual es siempre true
     * si gHeapStart[1] es NULL. */
    gHeapStart[1] = NULL;
    gHeapEnd[1]   = NULL;
    gHeapSize[1]  = 0;

    Xbr_Log("Xbr_Log: Heap_Init OK (%u MB at %p)\n", XBR_HEAP_BYTES / (1024u * 1024u), buf);
}
