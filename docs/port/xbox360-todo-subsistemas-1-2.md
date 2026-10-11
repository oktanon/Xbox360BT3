# Llamadas a Hardware PS2: Subsistemas 1-2 (Archivos/Memoria e Input)

Parte de [xbox360-todo.md](./xbox360-todo.md) (hub). Ver el indice de
subsistemas en la cabecera del hub. Esta seccion cubre los
**subsistemas 1 y 2** de la regla 3 de AGENTS.md (orden de
implementacion en fase 1: archivos/memoria, input, renderer, sonido,
memory card, peliculas, logica restante).

---

## 1. Llamadas a Hardware PS2 por Subsistema (Regla 3 de AGENTS.md)

## 1. Llamadas a Hardware PS2 por Subsistema (Regla 3 de AGENTS.md)

### Subsistema 1: 1. Archivos / Memoria / CDVD (sceCd*, sceOpen, sceRead...)

| Archivo | Línea | Función | Llamada / Símbolo | Código en Línea | ¿Reemplazado en PC dentro de `#ifdef PORT`? |
| :--- | :---: | :--- | :--- | :--- | :--- |
| [port/src/headless.c](../../port/src/headless.c#L181) | 181 | `sceOpen` | `sceOpen` | `int sceOpen(char *name, int flags) { (void)name; (void)fl...` | Implementación/stub en PC port (`port/src/headless.c`) |
| [port/src/headless.c](../../port/src/headless.c#L182) | 182 | `sceLseek` | `sceLseek` | `int sceLseek(int fd, int offset, int whence) { (void)fd; ...` | Implementación/stub en PC port (`port/src/headless.c`) |
| [port/src/headless.c](../../port/src/headless.c#L183) | 183 | `sceRead` | `sceRead` | `int sceRead(int fd, void *buf, int size) { (void)fd; (voi...` | Implementación/stub en PC port (`port/src/headless.c`) |
| [port/src/headless.c](../../port/src/headless.c#L184) | 184 | `sceClose` | `sceClose` | `int sceClose(int fd) { (void)fd; return 0; }` | Implementación/stub en PC port (`port/src/headless.c`) |
| [port/src/plat_file.c](../../port/src/plat_file.c#L239) | 239 | `sceCdGetDiskTypeSafe` | `sceCdGetDiskTypeSafe` | `int sceCdGetDiskTypeSafe(void) { return 0x14; }` | Implementación/stub en PC port (`port/src/plat_file.c`) |
| [port/src/plat_file.c](../../port/src/plat_file.c#L240) | 240 | `sceCdStatus` | `sceCdStatus` | `int sceCdStatus(void) { return 10; }` | Implementación/stub en PC port (`port/src/plat_file.c`) |
| [port/src/plat_sys.c](../../port/src/plat_sys.c#L15) | 15 | `sceFsReset` | `sceFsReset` | `int sceFsReset(void) { return 0; }` | Implementación/stub en PC port (`port/src/plat_sys.c`) |
| [port/src/plat_sys.c](../../port/src/plat_sys.c#L17) | 17 | `sceCdInit` | `sceCdInit` | `int sceCdInit(int mode) { (void)mode; return 1; }` | Implementación/stub en PC port (`port/src/plat_sys.c`) |
| [port/src/plat_sys.c](../../port/src/plat_sys.c#L18) | 18 | `sceCdMmode` | `sceCdMmode` | `int sceCdMmode(int media) { (void)media; return 1; }` | Implementación/stub en PC port (`port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L72) | 72 | `Overlay_Load` | `sceOpen` | `fd = sceOpen(gOverlayTbl[idx].path, SCE_RDONLY);` | No dentro de `#ifdef PORT` (stub en `port/src/headless.c`) |
| [src/main.c](../../src/main.c#L74) | 74 | `Overlay_Load` | `sceLseek` | `size = sceLseek(fd, 0, SCE_SEEK_END);` | No dentro de `#ifdef PORT` (stub en `port/src/headless.c`) |
| [src/main.c](../../src/main.c#L75) | 75 | `Overlay_Load` | `sceLseek` | `sceLseek(fd, 0, SCE_SEEK_SET);` | No dentro de `#ifdef PORT` (stub en `port/src/headless.c`) |
| [src/main.c](../../src/main.c#L77) | 77 | `Overlay_Load` | `sceRead` | `while (sceRead(fd, MainMenu_StartBallLoops, size) != size) {` | No dentro de `#ifdef PORT` (stub en `port/src/headless.c`) |
| [src/main.c](../../src/main.c#L80) | 80 | `Overlay_Load` | `sceClose` | `sceClose(fd);` | No dentro de `#ifdef PORT` (stub en `port/src/headless.c`) |
| [src/main.c](../../src/main.c#L87) | 87 | `Sys_RebootIop` | `sceCdInit` | `sceCdInit(SCECdINIT);` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L88) | 88 | `Sys_RebootIop` | `sceCdMmode` | `sceCdMmode(SCECdDVD);` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L94) | 94 | `Sys_RebootIop` | `sceCdInit` | `sceCdInit(SCECdINIT);` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L95) | 95 | `Sys_RebootIop` | `sceCdMmode` | `sceCdMmode(SCECdDVD);` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L96) | 96 | `Sys_RebootIop` | `sceFsReset` | `sceFsReset();` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/sys/file.c](../../src/sys/file.c#L64) | 64 | `Vsync_InstallHandler` | `sceGsSyncVCallback` | `sceGsSyncVCallback(Vsync_Handler);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/file.c](../../src/sys/file.c#L181) | 181 | `Disc_Identify` | `sceCdGetDiskTypeSafe` | `switch (sceCdGetDiskTypeSafe()) {` | No dentro de `#ifdef PORT` (stub en `port/src/plat_file.c`) |
| [src/sys/file.c](../../src/sys/file.c#L194) | 194 | `Disc_Identify` | `sceCdSearchFile` | `if (sceCdSearchFile(file, gDiscExeNameTbl[i]) == 1) {` | No dentro de `#ifdef PORT` (stub en `port/src/plat_file.c`) |
| [src/sys/file.c](../../src/sys/file.c#L207) | 207 | `Disc_GetDriveState` | `sceCdStatus` | `switch (sceCdStatus()) {` | No dentro de `#ifdef PORT` (stub en `port/src/plat_file.c`) |
| [src/sys/file.c](../../src/sys/file.c#L232) | 232 | `Disc_GetDriveState` | `sceCdGetDiskTypeSafe` | `if (sceCdGetDiskTypeSafe() == 0) {` | No dentro de `#ifdef PORT` (stub en `port/src/plat_file.c`) |


### Subsistema 2: 2. Input (scePad2*, sceDbc*, sceVib*, Pad_*)

| Archivo | Línea | Función | Llamada / Símbolo | Código en Línea | ¿Reemplazado en PC dentro de `#ifdef PORT`? |
| :--- | :---: | :--- | :--- | :--- | :--- |
| [include/sys/pad.h](../../include/sys/pad.h#L10) | 10 | `(file scope)` | `Pad_Update` | `Bits 16..23 are derived from the analog sticks by Pad_Upd...` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/pad.h](../../include/sys/pad.h#L71) | 71 | `(file scope)` | `scePad2GetState` | `#define PAD_STATE_STABLE 1 /* scePad2GetState: controller...` | No dentro de `#ifdef PORT` (stub/driver en `port/src/plat_stub.c` y `port/src/gs/gs_input.c`) |
| [include/sys/pad.h](../../include/sys/pad.h#L133) | 133 | `(file scope)` | `Pad_Init` | `void Pad_Init(void);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/pad.h](../../include/sys/pad.h#L138) | 138 | `(file scope)` | `Pad_Update` | `void Pad_Update(void);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/pad.h](../../include/sys/pad.h#L144) | 144 | `(file scope)` | `Pad_GetVibProfile` | `s32 Pad_GetVibProfile(s32 pad);` | No dentro de `#ifdef PORT` (código común) |
| [port/src/gs/gs_input.c](../../port/src/gs/gs_input.c#L337) | 337 | `Port_PadRead` | `Port_PadRead` | `int Port_PadRead(int socket, unsigned char *data) {` | Implementación/stub en PC port (`port/src/gs/gs_input.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L241) | 241 | `sceDbcInit` | `sceDbcInit` | `int sceDbcInit() { return 1; }` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L242) | 242 | `scePad2Init` | `scePad2Init` | `int scePad2Init() { return 1; }` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L244) | 244 | `scePad2CreateSocket` | `scePad2CreateSocket` | `int scePad2CreateSocket() { return sPadSockets++; }` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L245) | 245 | `scePad2GetState` | `scePad2GetState` | `int scePad2GetState() { return 1; }            /* connect...` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L247) | 247 | `scePad2GetButtonProfile` | `scePad2GetButtonProfile` | `int scePad2GetButtonProfile(int socket, unsigned char *pr...` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L275) | 275 | `scePad2Read` | `scePad2Read` | `int scePad2Read(int socket, unsigned char *data) {` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L317) | 317 | `(file scope)` | `Port_PadRead` | `} else if (Port_PadRead(socket, data)) {` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L333) | 333 | `sceVibGetProfile` | `sceVibGetProfile` | `int sceVibGetProfile() { return 0; }` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L334) | 334 | `sceVibSetActParam` | `sceVibSetActParam` | `int sceVibSetActParam() { return 0; }` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [src/main.c](../../src/main.c#L149) | 149 | `Game_Main` | `Pad_Init` | `Pad_Init();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/late_a.c](../../src/sys/late_a.c#L857) | 857 | `Shen_Run` | `Pad_Update` | `Pad_Update();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/movie.c](../../src/sys/movie.c#L289) | 289 | `Movie_Run` | `Pad_Update` | `Pad_Update();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/pad.c](../../src/sys/pad.c#L20) | 20 | `Pad_Init` | `Pad_Init` | `void Pad_Init(void) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/pad.c](../../src/sys/pad.c#L24) | 24 | `Pad_Init` | `sceDbcInit` | `sceDbcInit();` | No dentro de `#ifdef PORT` (stub/driver en `port/src/plat_stub.c` y `port/src/gs/gs_input.c`) |
| [src/sys/pad.c](../../src/sys/pad.c#L25) | 25 | `Pad_Init` | `scePad2Init` | `scePad2Init(0);` | No dentro de `#ifdef PORT` (stub/driver en `port/src/plat_stub.c` y `port/src/gs/gs_input.c`) |
| [src/sys/pad.c](../../src/sys/pad.c#L32) | 32 | `Pad_Init` | `scePad2CreateSocket` | `gPad[0].socket = scePad2CreateSocket(&param, &gPad[0]);` | No dentro de `#ifdef PORT` (stub/driver en `port/src/plat_stub.c` y `port/src/gs/gs_input.c`) |
| [src/sys/pad.c](../../src/sys/pad.c#L38) | 38 | `Pad_Init` | `scePad2CreateSocket` | `gPad[1].socket = scePad2CreateSocket(&param, &gPad[1]);` | No dentro de `#ifdef PORT` (stub/driver en `port/src/plat_stub.c` y `port/src/gs/gs_input.c`) |
| [src/sys/pad.c](../../src/sys/pad.c#L63) | 63 | `Pad_Update` | `Pad_Update` | `void Pad_Update(void) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/pad.c](../../src/sys/pad.c#L72) | 72 | `Pad_Update` | `scePad2GetState` | `pad->state = scePad2GetState(pad->socket);` | No dentro de `#ifdef PORT` (stub/driver en `port/src/plat_stub.c` y `port/src/gs/gs_input.c`) |
| [src/sys/pad.c](../../src/sys/pad.c#L77) | 77 | `Pad_Update` | `Pad_GetVibProfile` | `Pad_GetVibProfile(i);` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/pad.c](../../src/sys/pad.c#L80) | 80 | `Pad_Update` | `scePad2GetButtonProfile` | `pad->profileSize = scePad2GetButtonProfile(pad->socket, p...` | No dentro de `#ifdef PORT` (stub/driver en `port/src/plat_stub.c` y `port/src/gs/gs_input.c`) |
| [src/sys/pad.c](../../src/sys/pad.c#L86) | 86 | `Pad_Update` | `scePad2Read` | `pad->dataSize = scePad2Read(pad->socket, pad->data);` | No dentro de `#ifdef PORT` (stub/driver en `port/src/plat_stub.c` y `port/src/gs/gs_input.c`) |
| [src/sys/pad.c](../../src/sys/pad.c#L192) | 192 | `Pad_GetVibProfile` | `Pad_GetVibProfile` | `s32 Pad_GetVibProfile(s32 pad) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/pad.c](../../src/sys/pad.c#L195) | 195 | `Pad_GetVibProfile` | `sceVibGetProfile` | `return sceVibGetProfile(p->socket, p->vibProfile);` | No dentro de `#ifdef PORT` (stub/driver en `port/src/plat_stub.c` y `port/src/gs/gs_input.c`) |
| [src/sys/pad.c](../../src/sys/pad.c#L212) | 212 | `Pad_SendActuator` | `sceVibSetActParam` | `sceVibSetActParam(p->socket, 1, p->vibProfile, 2, &p->vib...` | No dentro de `#ifdef PORT` (stub/driver en `port/src/plat_stub.c` y `port/src/gs/gs_input.c`) |


