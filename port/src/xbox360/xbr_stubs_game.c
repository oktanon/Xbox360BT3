/*
 * port/src/xbox360/xbr_stubs_game.c
 *
 * Stubs de las funciones del juego que src/main.c referencia y que el
 * decompilador del proyecto aun no tiene portadas. Cada stub lleva
 * Xbr_Log para que el log del XEX muestre por donde pasa Game_Main, y
 * // XBOX360: con la fase y el subsistema en que se hara la impl real.
 *
 * El orden sigue la secuencia de Game_Main en src/main.c (Sys_RebootIop,
 * Sys_LoadIopModules, ChangeThreadPriority, Heap_Init, Gfx_Init, Snd_Init,
 * File_Init, Movie_Init, Common_*, Progress_Init, Save_Init, Job_Init,
 * Vu0_Init, Dma_InitBuffers, Dbg_Init, Pad_Init, McCard_Init, Font_Init,
 * FontIcon_Init, Fade_Init, PadWatch_Init, y el bucle Overlay_Load +
 * Progress_Main + Battle_Main).
 *
 * Los stubs son no-op (devuelven 0, NULL o void) salvo Gfx_Init, que es el
 * destino de la fase 1 y anota una linea de log destacada.
 */
#include "xbox360/xbr_platform.h"

#include <xtl.h>

/* OverlayEntry (definido en include/main.h): src/main.c lo usa para cargar
 * DBZP.BIN en el bucle final. Mientras Game_Main no llegue al bucle
 * (queda cortado por el return de los stubs de Init), basta con una tabla
 * minima. */
typedef struct OverlayEntry {
    char *label;
    char *path;
} OverlayEntry;

/* Tabla minima con una entrada que apunte a una cadena literal: si el bucle
 * del juego se llega a ejecutar, sceOpen("...") devolvera -1 (no existe
 * el fichero en game:\) y el lazo do/while quedara colgado a proposito.
 * Es un comportamiento aceptable para la fase 1 (criterio: log hasta
 * Gfx_Init, no ejecucion completa). */
OverlayEntry gOverlayTbl[1] = {
    { (char *)"PROGRESS\n", (char *)"cdrom0:\\BIN\\DBZP.BIN;1" },
};

/* Buffer donde Overlay_Load deposita el codigo del overlay. Reserva 1 MB
 * (suficiente para el menu DBZP.BIN de PS2, que pesa ~0x89EB1C bytes). En
 * Xbox 360 el contenido vendra del sistema de archivos local; aqui solo
 * necesitamos una direccion valida para que la escritura no casque. */
unsigned char MainMenu_StartBallLoops[0x100000];

/* --------------------------------------------------------------------------
 * Secuencia de init de Game_Main (antes de Gfx_Init)
 * -------------------------------------------------------------------------- */

/* XBOX360: Sys_InitIopHeap se llama desde Sys_LoadIopModules (src/main.c:127)
 * tras cargar los IRX. En Xbox 360 no hay IOP, asi que es no-op. */
void Sys_InitIopHeap(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Sys_InitIopHeap stub (subsistema 1 archivos)\n");
        logged = 1;
    }
}

/* XBOX360: Heap_Init arma los HeapBlock del juego (ver src/sys/heap.c). La
 * implementacion real esta en port/src/xbox360/xbr_heap.c (subtarea 2.2
 * de la fase 2): reserva 32 MB, inicializa un bloque libre unico y
 * define los simbolos gHeapStart[0..1] / gHeapEnd[0..1] / gHeapSize[0..1]
 * que el juego referencia como `extern` desde src/sys/heap.c. */

/* XBOX360: este es el TARGET del criterio de salida de la fase 1
 * ("XEX que arranca y llega con log hasta Gfx_Init"). El log se imprime
 * con un marcador "=== GFX_INIT ===" para que sea facil de buscar. La
 * implementacion real vendra en fase 3 (subsistema 3 renderer, Direct3D 9). */
void Gfx_Init(void) {
    Xbr_Log("=== GFX_INIT reached ===\n");
    Xbr_Log("Xbr_Log: Gfx_Init stub (subsistema 3 renderer)\n");
}

/* --------------------------------------------------------------------------
 * Resto del init de Game_Main (despues de Gfx_Init). Las marcamos en el log
 * para que se vea el recorrido completo, pero no son necesarias para el
 * criterio de la fase 1. Quedaran implementadas en las fases 2-7.
 * -------------------------------------------------------------------------- */

void Snd_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Snd_Init stub (subsistema 4 sonido)\n");
        logged = 1;
    }
}

void File_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: File_Init stub (subsistema 1 archivos)\n");
        logged = 1;
    }
}

void Movie_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Movie_Init stub (subsistema 6 peliculas)\n");
        logged = 1;
    }
}

void Common_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Common_Init stub (subsistema 7 logica)\n");
        logged = 1;
    }
}

void Common_LoadBoot(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Common_LoadBoot stub (subsistema 7)\n");
        logged = 1;
    }
}

void Common_Reload(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Common_Reload stub (subsistema 7)\n");
        logged = 1;
    }
}

void Progress_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Progress_Init stub (subsistema 7)\n");
        logged = 1;
    }
}

void Save_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Save_Init stub (subsistema 5 memory card)\n");
        logged = 1;
    }
}

void Job_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Job_Init stub (subsistema 7)\n");
        logged = 1;
    }
}

void Vu0_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Vu0_Init stub (subsistema 7; VU0 no existe en Xbox 360)\n");
        logged = 1;
    }
}

void Dma_InitBuffers(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Dma_InitBuffers stub (subsistema 7; DMA directo del PS2 no aplica)\n");
        logged = 1;
    }
}

void Dbg_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Dbg_Init stub (subsistema 7)\n");
        logged = 1;
    }
}

void Pad_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Pad_Init stub (subsistema 2 input)\n");
        logged = 1;
    }
}

void McCard_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: McCard_Init stub (subsistema 5 memory card)\n");
        logged = 1;
    }
}

void Font_Init(int sel) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Font_Init(%d) stub (subsistema 1 archivos)\n", sel);
        logged = 1;
    }
}

void FontIcon_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: FontIcon_Init stub (subsistema 1)\n");
        logged = 1;
    }
}

void Fade_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: Fade_Init stub (subsistema 7)\n");
        logged = 1;
    }
}

void PadWatch_Init(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("Xbr_Log: PadWatch_Init stub (subsistema 7)\n");
        logged = 1;
    }
}

/* --------------------------------------------------------------------------
 * Bucle final de Game_Main: Overlay_Load(0); Progress_Main(0); Battle_Main(0);
 * Para fase 1 no es necesario que estas funciones hagan nada util: el bucle
 * se corta a proposito (sceOpen -> -1 cuelga el do/while). Pero los
 * simbolos deben existir para que el link no falle.
 * -------------------------------------------------------------------------- */

void Progress_Main(int arg) {
    static int logged;
    (void)arg;
    if (!logged) {
        Xbr_Log("Xbr_Log: Progress_Main stub (loop final; subsistema 7)\n");
        logged = 1;
    }
}

void Battle_Main(int arg) {
    static int logged;
    (void)arg;
    if (!logged) {
        Xbr_Log("Xbr_Log: Battle_Main stub (loop final; subsistema 7)\n");
        logged = 1;
    }
}

/* --------------------------------------------------------------------------
 * Auxiliares: src/main.c define View_ApplyScissor que llama a Dma_AddScissor
 * (declarada en include/sys/dma.h). En Xbox 360 no hay DMA en el sentido
 * PS2; los scissor se pasan a D3D directamente (subsistema 3). Stub aqui
 * solo para que el link no falle: View_ApplyScissor no se invoca en el
 * camino a Gfx_Init en fase 1.
 * -------------------------------------------------------------------------- */

void Dma_AddScissor(int x0, int x1, int y0, int y1) {
    static int logged;
    (void)x0;
    (void)x1;
    (void)y0;
    (void)y1;
    if (!logged) {
        Xbr_Log("Xbr_Log: Dma_AddScissor stub (subsistema 7; scissor lo gestiona D3D en fase 3)\n");
        logged = 1;
    }
}
