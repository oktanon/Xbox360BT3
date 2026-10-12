/*
 * port/src/xbox360/xbr_stubs_sce.c
 *
 * Stubs no-op de los simbolos de Sony (sce*, libpad2, libcdvd, libkernl)
 * que src/main.c y otros .c del juego referencian. En Xbox 360 no hay
 * IOP, ni CD/DVD, ni sistema de archivos de PS2: el port de PC los resolvia
 * con stubs similares en port/src/plat_*.c. Aqui los replicamos contra el
 * XDK para que el linker los encuentre al compilar el port de 360.
 *
 * Cada stub lleva:
 *   - // XBOX360: motivo del stub
 *   - TODO(xbox360) con la fase y el subsistema donde se hara la
 *     implementacion real (regla 7 de AGENTS.md)
 *   - Xbr_Log en la primera llamada, para que el log muestre que el stub
 *     entro, sin inundar en bucles
 *
 * Los valores de retorno copian los del port de PC (port/src/plat_*.c):
 * son los mismos que ya estaban validados para mantener la logica del juego
 * en su sitio hasta que llegue la implementacion real.
 */
#include "xbox360/xbr_platform.h"

#include <xtl.h>

/* --------------------------------------------------------------------------
 * Libkernl: hilos, prioridad y FlushCache
 * -------------------------------------------------------------------------- */

/* XBOX360: PS2 GetThreadId devolvia el id del hilo actual del EE. En Xbox 360
 * solo hay un hilo "de juego" en este port; XAPILIB crea el hilo principal y
 * expone GetCurrentThread/GetCurrentFiber en la RTL del XDK, pero no las
 * necesitamos en la subtarea 1.3. Devolvemos 1, igual que plat_sys.c. */
int GetThreadId(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("TODO(xbox360): GetThreadId stub (fase 2+, subsistema 2 input)\n");
        logged = 1;
    }
    return 1;
}

/* XBOX360: el planificador de PS2 tenia 128 niveles. En Xbox 360 las
 * prioridades de hilo son las del XDK (0 = normal, etc.). Por ahora
 * aceptamos la llamada y devolvemos 0, como el port de PC. */
int ChangeThreadPriority(int tid, int prio) {
    (void)tid;
    (void)prio;
    return 0;
}

/* XBOX360: FlushCache de PS2 era DCache/ICache. La coherencia de cache en
 * Xbox 360 entre CPU y GPU la gestiona la propia libreria (D3D hace flush
 * antes de cada Present/Draw). Por ahora es no-op. */
void FlushCache(int mode) {
    (void)mode;
}

/* --------------------------------------------------------------------------
 * IOP: reinicio, modulos y filesystem (no existen en Xbox 360)
 * -------------------------------------------------------------------------- */

/* XBOX360: no hay IOP que reiniciar. Stub para que src/main.c::Sys_RebootIop
 * compile y enlaze. Impl real: nada. */
int sceFsReset(void) {
    static int logged;
    if (!logged) {
        Xbr_Log("TODO(xbox360): sceFsReset stub (fase 2+, subsistema 1 archivos)\n");
        logged = 1;
    }
    return 0;
}

/* XBOX360: no hay CD/DVD. La carga de datos del juego (ISO, paquetes, etc.)
 * vendra del sistema de archivos de Xbox 360 (XContent o game:\). Stub. */
int sceCdInit(int mode) {
    (void)mode;
    return 1;
}

/* XBOX360: companion de sceCdInit, no hay soporte fisico de CD/DVD. Stub. */
int sceCdMmode(int media) {
    (void)media;
    return 1;
}

/* XBOX360: SIF (SubCPU Interface) era el bus entre EE e IOP. En Xbox 360 no
 * existe ese modelo. Stub. */
void sceSifInitRpc(unsigned int mode) {
    (void)mode;
}

/* XBOX360: src/main.c::Sys_RebootIop espera a que termine el reboot del IOP
 * (while(!sceSifRebootIop(...))). Devolvemos 1 (exito) en la primera llamada
 * para no colgar. */
int sceSifRebootIop(char *img) {
    (void)img;
    static int logged;
    if (!logged) {
        Xbr_Log("TODO(xbox360): sceSifRebootIop stub (fase 2+, subsistema 1)\n");
        logged = 1;
    }
    return 1;
}

/* XBOX360: companero de sceSifRebootIop. */
int sceSifSyncIop(void) {
    return 1;
}

/* XBOX360: src/main.c::Sys_LoadIopModules carga 13 modulos IRX (SIO2MAN,
 * MCMAN, MCSERV, ...). En Xbox 360 no hay modulos. Devolvemos 0 (exito) para
 * que el lazo while(!sceSifLoadModule(...)) no se cuelgue. */
int sceSifLoadModule(char *name, int argLen, char *args) {
    (void)name;
    (void)argLen;
    (void)args;
    return 0;
}

/* --------------------------------------------------------------------------
 * sceOpen/Lseek/Read/Close: abiertos por src/main.c::Overlay_Load
 * -------------------------------------------------------------------------- */

/* XBOX360: no hay un equivalente directo de sceOpen. En fase 2 el subsistema
 * 1 (archivos/memoria) abrira ficheros del sistema de archivos de Xbox 360
 * (game:\ o hard disk) segun la politica de instalacion que se decida. Por
 * ahora devolvemos -1 para que el bucle do/while de Overlay_Load no se
 * cuelgue en caso de que la fase 1.4 llegue a invocarlo (no deberia: el
 * bucle esta despues de Gfx_Init()). */
int sceOpen(char *name, int flags) {
    (void)name;
    (void)flags;
    return -1;
}

int sceLseek(int fd, int offset, int whence) {
    (void)fd;
    (void)offset;
    (void)whence;
    return 0;
}

int sceRead(int fd, void *buf, int size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

int sceClose(int fd) {
    (void)fd;
    return 0;
}

/* --------------------------------------------------------------------------
 * sceCd*: deteccion del disco y busqueda de ficheros
 * -------------------------------------------------------------------------- */

/* XBOX360: el port de PC devolvia 0x14 (SCECdNODISC + 0x10 o similar) para
 * que el menu se saltase la identificacion del disco. Mismo valor aqui. */
int sceCdGetDiskTypeSafe(void) {
    return 0x14;
}

/* XBOX360: idem: 10 = "no media" para que el juego siga. */
int sceCdStatus(void) {
    return 10;
}

/* XBOX360: src/sys/file.c::Disc_Identify espera 1 si encuentra el fichero
 * pedido en la tabla. Devolvemos 0 (no encontrado) como hace el port de PC. */
int sceCdSearchFile(void *file, const char *name) {
    (void)file;
    (void)name;
    return 0;
}

/* --------------------------------------------------------------------------
 * Generador aleatorio del port de PC (port/src/plat_sys.c)
 * -------------------------------------------------------------------------- */

/* XBOX360: func_002BB390 era el origen de la semilla de rand() en PS2 (un
 * reloj o contador). En Xbox 360 la sembramos con un valor fijo para que
 * cada ejecucion sea repetible (igual que el port de PC sin BT3_SEED).
 * El PC permitia override por env (BT3_SEED=...); aqui no hay getenv() en
 * la RTL del XDK. La forma de override vendra por un fichero de
 * configuracion en game:\ (fase 2+, subsistema 1 archivos) o por
 * configuracion del XEX. */
long func_002BB390(void) {
    return 0x12345678L;
}

/* XBOX360: el rand/srand del juego se reemplazo en el port de PC por
 * Port_Srand/Port_Rand (mismo algoritmo: newlib 64-bit). Misma
 * implementacion en 360; no es PowerPC-specific. */
unsigned long long gPortRandNext = 1;

void Port_Srand(unsigned int seed) {
    gPortRandNext = seed;
}

int Port_Rand(void) {
    gPortRandNext = gPortRandNext * 6364136223846793005ull + 1;
    return (int)((gPortRandNext >> 32) & 0x7FFFFFFF);
}
