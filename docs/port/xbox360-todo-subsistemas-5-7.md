# Llamadas a Hardware PS2: Subsistemas 5-7 (Memory Card, Peliculas, Logica restante)

Parte de [xbox360-todo.md](./xbox360-todo.md) (hub). Ver el indice de
subsistemas en la cabecera del hub. Esta seccion cubre los
**subsistemas 5, 6 y 7** de la regla 3 de AGENTS.md.

---

## 1. Llamadas a Hardware PS2 por Subsistema (Regla 3 de AGENTS.md)

### Subsistema 5: 5. Memory Card (sceMc*, libmc, McCard_*)

| Archivo | Línea | Función | Llamada / Símbolo | Código en Línea | ¿Reemplazado en PC dentro de `#ifdef PORT`? |
| :--- | :---: | :--- | :--- | :--- | :--- |
| [port/src/plat_mc.c](../../port/src/plat_mc.c#L85) | 85 | `func_002A1A48` | `func_002A1A48` | `int func_002A1A48(void) { return 0; } /* sceMcInit */` | Implementación/stub en PC port (`port/src/plat_mc.c`) |
| [port/src/plat_mc.c](../../port/src/plat_mc.c#L88) | 88 | `func_002A25B8` | `func_002A25B8` | `int func_002A25B8(int port, int slot, int *type, int *fre...` | Implementación/stub en PC port (`port/src/plat_mc.c`) |
| [port/src/plat_mc.c](../../port/src/plat_mc.c#L106) | 106 | `func_002A1E58` | `func_002A1E58` | `int func_002A1E58(int port, int slot, char *name, int mod...` | Implementación/stub en PC port (`port/src/plat_mc.c`) |
| [port/src/plat_mc.c](../../port/src/plat_mc.c#L139) | 139 | `func_002A1F80` | `func_002A1F80` | `int func_002A1F80(int port, int slot, char *name) {` | Implementación/stub en PC port (`port/src/plat_mc.c`) |
| [port/src/plat_mc.c](../../port/src/plat_mc.c#L149) | 149 | `func_002A1FB8` | `func_002A1FB8` | `int func_002A1FB8(int fd) {` | Implementación/stub en PC port (`port/src/plat_mc.c`) |
| [port/src/plat_mc.c](../../port/src/plat_mc.c#L162) | 162 | `func_002A2078` | `func_002A2078` | `int func_002A2078(int fd, int offset, int origin) {` | Implementación/stub en PC port (`port/src/plat_mc.c`) |
| [port/src/plat_mc.c](../../port/src/plat_mc.c#L174) | 174 | `func_002A2208` | `func_002A2208` | `int func_002A2208(int fd, void *buf, int size) {` | Implementación/stub en PC port (`port/src/plat_mc.c`) |
| [port/src/plat_mc.c](../../port/src/plat_mc.c#L188) | 188 | `func_002A2320` | `func_002A2320` | `int func_002A2320(int fd, void *buf, int size) {` | Implementación/stub en PC port (`port/src/plat_mc.c`) |
| [port/src/plat_mc.c](../../port/src/plat_mc.c#L204) | 204 | `func_002A27A8` | `func_002A27A8` | `int func_002A27A8(int port, int slot, char *name, int mod...` | Implementación/stub en PC port (`port/src/plat_mc.c`) |
| [port/src/plat_mc.c](../../port/src/plat_mc.c#L231) | 231 | `func_002A2AC8` | `func_002A2AC8` | `int func_002A2AC8(int port, int slot) {` | Implementación/stub en PC port (`port/src/plat_mc.c`) |
| [port/src/plat_mc.c](../../port/src/plat_mc.c#L237) | 237 | `func_002A2498` | `func_002A2498` | `int func_002A2498(int mode, int *cmd, int *result) {` | Implementación/stub en PC port (`port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L44) | 44 | `(file scope)` | `func_002A1A48` | `#define sceMcInit func_002A1A48` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L45) | 45 | `(file scope)` | `func_002A1E58` | `#define sceMcOpen func_002A1E58` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L46) | 46 | `(file scope)` | `func_002A1F80` | `#define sceMcMkdir func_002A1F80` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L47) | 47 | `(file scope)` | `func_002A1FB8` | `#define sceMcClose func_002A1FB8` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L48) | 48 | `(file scope)` | `func_002A2078` | `#define sceMcSeek func_002A2078` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L49) | 49 | `(file scope)` | `func_002A2208` | `#define sceMcRead func_002A2208` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L50) | 50 | `(file scope)` | `func_002A2320` | `#define sceMcWrite func_002A2320` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L51) | 51 | `(file scope)` | `func_002A2498` | `#define sceMcSync func_002A2498` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L52) | 52 | `(file scope)` | `func_002A25B8` | `#define sceMcGetInfo func_002A25B8` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L53) | 53 | `(file scope)` | `func_002A27A8` | `#define sceMcGetDir func_002A27A8` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L54) | 54 | `(file scope)` | `func_002A2AC8` | `#define sceMcFormat func_002A2AC8` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L84) | 84 | `McCard_Init` | `McCard_Init` | `void McCard_Init(void) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L87) | 87 | `McCard_Init` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L91) | 91 | `McCard_GetInfo` | `McCard_GetInfo` | `s32 McCard_GetInfo(s32 port) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L129) | 129 | `McCard_Probe` | `McCard_Probe` | `s32 McCard_Probe(s32 port) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L165) | 165 | `McCard_Format` | `McCard_Format` | `s32 McCard_Format(s32 port) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L204) | 204 | `McCard_CreateSave` | `McCard_CreateSave` | `s32 McCard_CreateSave(s32 port, McCardFile *file) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L385) | 385 | `McCard_WriteSave` | `McCard_WriteSave` | `s32 McCard_WriteSave(s32 port, void *data, s32 size, McCa...` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L520) | 520 | `McCard_ReadFile` | `McCard_ReadFile` | `s32 McCard_ReadFile(s32 port, void *buf, s32 size, McCard...` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L660) | 660 | `McCard_CheckFiles` | `McCard_CheckFiles` | `s32 McCard_CheckFiles(s32 port, McCardFile *file) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/battle/stgm_a_b.c](../../src/battle/stgm_a_b.c#L899) | 899 | `McCard_FindSave` | `McCard_FindSave` | `s32 McCard_FindSave(s32 port, McCardFile *file) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/main.c](../../src/main.c#L150) | 150 | `Game_Main` | `McCard_Init` | `McCard_Init();` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L103) | 103 | `(file scope)` | `McCard_GetInfo` | `if (McCard_GetInfo(0) == 0) { \` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L301) | 301 | `McFlow_UpdateSave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L316) | 316 | `McFlow_UpdateSave` | `McCard_CheckFiles` | `if (McCard_CheckFiles(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L353) | 353 | `McFlow_UpdateSave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L365) | 365 | `McFlow_UpdateSave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L399) | 399 | `McFlow_UpdateSave` | `McCard_Format` | `if (McCard_Format(0) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L410) | 410 | `McFlow_UpdateSave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L442) | 442 | `McFlow_UpdateSave` | `McCard_CreateSave` | `if (McCard_CreateSave(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L460) | 460 | `McFlow_UpdateSave` | `McCard_WriteSave` | `if (McCard_WriteSave(0, gSaveData, 0x4000, &gMcFlow->req)...` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L527) | 527 | `McFlow_UpdateSave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L615) | 615 | `McFlow_UpdateSave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L647) | 647 | `McFlow_UpdateSave` | `McCard_CreateSave` | `if (McCard_CreateSave(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L719) | 719 | `McFlow_UpdateLoad` | `McCard_CheckFiles` | `if (McCard_CheckFiles(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L761) | 761 | `McFlow_UpdateLoad` | `McCard_ReadFile` | `if (McCard_ReadFile(0, &gMcFlow->saveBuf, 0x4000, &gMcFlo...` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L912) | 912 | `McFlow_UpdateBootLoad` | `McCard_CheckFiles` | `if (McCard_CheckFiles(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L935) | 935 | `McFlow_UpdateBootLoad` | `McCard_GetInfo` | `if (McCard_GetInfo(0) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L944) | 944 | `McFlow_UpdateBootLoad` | `McCard_CheckFiles` | `if (McCard_CheckFiles(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L974) | 974 | `McFlow_UpdateBootLoad` | `McCard_GetInfo` | `if (McCard_GetInfo(0) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1008) | 1008 | `McFlow_UpdateBootLoad` | `McCard_GetInfo` | `if (McCard_GetInfo(0) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1064) | 1064 | `McFlow_UpdateBootLoad` | `McCard_GetInfo` | `if (McCard_GetInfo(0) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1072) | 1072 | `McFlow_UpdateBootLoad` | `McCard_ReadFile` | `if (McCard_ReadFile(0, &gMcFlow->saveBuf, 0x4000, &gMcFlo...` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1253) | 1253 | `McFlow_UpdateNewSave` | `McCard_CheckFiles` | `if (McCard_CheckFiles(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1283) | 1283 | `McFlow_UpdateNewSave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1316) | 1316 | `McFlow_UpdateNewSave` | `McCard_Format` | `if (McCard_Format(0) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1326) | 1326 | `McFlow_UpdateNewSave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1352) | 1352 | `McFlow_UpdateNewSave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1386) | 1386 | `McFlow_UpdateNewSave` | `McCard_CreateSave` | `if (McCard_CreateSave(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1408) | 1408 | `McFlow_UpdateNewSave` | `McCard_WriteSave` | `if (McCard_WriteSave(0, gSaveData, 0x4000, &gMcFlow->req)...` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1515) | 1515 | `McFlow_UpdateNewSave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1547) | 1547 | `McFlow_UpdateNewSave` | `McCard_CreateSave` | `if (McCard_CreateSave(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1633) | 1633 | `McFlow_UpdateReplayScan` | `McCard_CheckFiles` | `if (McCard_CheckFiles(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1654) | 1654 | `McFlow_UpdateReplayScan` | `McCard_FindSave` | `if (McCard_FindSave(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1679) | 1679 | `McFlow_UpdateReplayScan` | `McCard_ReadFile` | `if (McCard_ReadFile(0, &gMcFlow->slotHdr[gMcFlow->index],...` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1704) | 1704 | `McFlow_UpdateReplayScan` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1739) | 1739 | `McFlow_UpdateReplayScan` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1804) | 1804 | `McFlow_UpdateReplayScan` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1847) | 1847 | `McFlow_UpdateReplaySave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1892) | 1892 | `McFlow_UpdateReplaySave` | `McCard_CheckFiles` | `if (McCard_CheckFiles(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1922) | 1922 | `McFlow_UpdateReplaySave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1957) | 1957 | `McFlow_UpdateReplaySave` | `McCard_Format` | `if (McCard_Format(0) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L1968) | 1968 | `McFlow_UpdateReplaySave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2001) | 2001 | `McFlow_UpdateReplaySave` | `McCard_CreateSave` | `if (McCard_CreateSave(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2019) | 2019 | `McFlow_UpdateReplaySave` | `McCard_WriteSave` | `if (McCard_WriteSave(0, &gMcFlow->repSave.hdr, 0x1AC00, &...` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2031) | 2031 | `McFlow_UpdateReplaySave` | `McCard_FindSave` | `if (McCard_FindSave(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2087) | 2087 | `McFlow_UpdateReplaySave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2185) | 2185 | `McFlow_UpdateReplaySave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2218) | 2218 | `McFlow_UpdateReplaySave` | `McCard_CreateSave` | `if (McCard_CreateSave(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2234) | 2234 | `McFlow_UpdateReplaySave` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2279) | 2279 | `McFlow_UpdateReplayLoad` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2319) | 2319 | `McFlow_UpdateReplayLoad` | `McCard_CheckFiles` | `if (McCard_CheckFiles(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2339) | 2339 | `McFlow_UpdateReplayLoad` | `McCard_FindSave` | `if (McCard_FindSave(0, &gMcFlow->req) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2361) | 2361 | `McFlow_UpdateReplayLoad` | `McCard_ReadFile` | `if (McCard_ReadFile(0, &gMcFlow->repLoad, 0x1AC00, &gMcFl...` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2479) | 2479 | `McFlow_UpdateReplayLoad` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2524) | 2524 | `McFlow_UpdateReplayAsk` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2599) | 2599 | `McFlow_UpdateReplayAsk` | `McCard_Probe` | `McCard_Probe(0);` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |
| [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c#L2752) | 2752 | `McFlow_PollCard` | `McCard_GetInfo` | `if (McCard_GetInfo(0) == 0) {` | No dentro de `#ifdef PORT` (emulación en `port/src/plat_mc.c`) |


### Subsistema 6: 6. Películas (sceMpeg*, PSS/MPEG-2)

| Archivo | Línea | Función | Llamada / Símbolo | Código en Línea | ¿Reemplazado en PC dentro de `#ifdef PORT`? |
| :--- | :---: | :--- | :--- | :--- | :--- |
| [port/src/plat_movie.c](../../port/src/plat_movie.c#L56) | 56 | `sceMpegInit` | `sceMpegInit` | `int sceMpegInit(void) { return 0; }` | Implementación/stub en PC port (`port/src/plat_movie.c`) |
| [port/src/plat_movie.c](../../port/src/plat_movie.c#L57) | 57 | `sceMpegCreate` | `sceMpegCreate` | `int sceMpegCreate(void *mp, void *work, int size) { (void...` | Implementación/stub en PC port (`port/src/plat_movie.c`) |
| [port/src/plat_movie.c](../../port/src/plat_movie.c#L58) | 58 | `sceMpegDelete` | `sceMpegDelete` | `int sceMpegDelete(void *mp) { (void)mp; movie_close(); re...` | Implementación/stub en PC port (`port/src/plat_movie.c`) |
| [port/src/plat_movie.c](../../port/src/plat_movie.c#L59) | 59 | `sceMpegReset` | `sceMpegReset` | `int sceMpegReset(void *mp) { (void)mp; movie_close(); ret...` | Implementación/stub en PC port (`port/src/plat_movie.c`) |
| [port/src/plat_movie.c](../../port/src/plat_movie.c#L60) | 60 | `sceMpegAddCallback` | `sceMpegAddCallback` | `void *sceMpegAddCallback(void *mp, int type, void *cb, vo...` | Implementación/stub en PC port (`port/src/plat_movie.c`) |
| [port/src/plat_movie.c](../../port/src/plat_movie.c#L61) | 61 | `sceMpegAddStrCallback` | `sceMpegAddStrCallback` | `void *sceMpegAddStrCallback(void *mp, int type, int ch, v...` | Implementación/stub en PC port (`port/src/plat_movie.c`) |
| [port/src/plat_movie.c](../../port/src/plat_movie.c#L62) | 62 | `sceMpegDemuxPss` | `sceMpegDemuxPss` | `int sceMpegDemuxPss(void *mp, void *data, int size) { (vo...` | Implementación/stub en PC port (`port/src/plat_movie.c`) |
| [port/src/plat_movie.c](../../port/src/plat_movie.c#L64) | 64 | `sceMpegIsEnd` | `sceMpegIsEnd` | `int sceMpegIsEnd(void *mp) {` | Implementación/stub en PC port (`port/src/plat_movie.c`) |
| [port/src/plat_movie.c](../../port/src/plat_movie.c#L74) | 74 | `sceMpegGetPicture` | `sceMpegGetPicture` | `int sceMpegGetPicture(void *mp, void *rgb, int blocks) {` | Implementación/stub en PC port (`port/src/plat_movie.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L180) | 180 | `Movie_Init` | `sceMpegInit` | `sceMpegInit();` | No dentro de `#ifdef PORT` (decodificador en `port/src/plat_movie.c` y `port/src/plat_mpeg2.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L216) | 216 | `Movie_Open` | `sceMpegCreate` | `sceMpegCreate(&m->mpeg, m->work, m->workSize);` | No dentro de `#ifdef PORT` (decodificador en `port/src/plat_movie.c` y `port/src/plat_mpeg2.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L217) | 217 | `Movie_Open` | `sceMpegAddStrCallback` | `sceMpegAddStrCallback(&m->mpeg, SCE_MPEG_STR_M2V, 0, (sce...` | No dentro de `#ifdef PORT` (decodificador en `port/src/plat_movie.c` y `port/src/plat_mpeg2.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L218) | 218 | `Movie_Open` | `sceMpegAddCallback` | `sceMpegAddCallback(&m->mpeg, SCE_MPEG_CB_NODATA, Movie_Cb...` | No dentro de `#ifdef PORT` (decodificador en `port/src/plat_movie.c` y `port/src/plat_mpeg2.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L219) | 219 | `Movie_Open` | `sceMpegAddCallback` | `sceMpegAddCallback(&m->mpeg, SCE_MPEG_CB_BACKGROUND, Movi...` | No dentro de `#ifdef PORT` (decodificador en `port/src/plat_movie.c` y `port/src/plat_mpeg2.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L220) | 220 | `Movie_Open` | `sceMpegAddCallback` | `sceMpegAddCallback(&m->mpeg, SCE_MPEG_CB_ERROR, Movie_CbE...` | No dentro de `#ifdef PORT` (decodificador en `port/src/plat_movie.c` y `port/src/plat_mpeg2.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L233) | 233 | `Movie_Close` | `sceMpegReset` | `sceMpegReset(&m->mpeg);` | No dentro de `#ifdef PORT` (decodificador en `port/src/plat_movie.c` y `port/src/plat_mpeg2.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L234) | 234 | `Movie_Close` | `sceMpegDelete` | `sceMpegDelete(&m->mpeg);` | No dentro de `#ifdef PORT` (decodificador en `port/src/plat_movie.c` y `port/src/plat_mpeg2.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L286) | 286 | `Movie_Run` | `sceMpegIsEnd` | `while (!sceMpegIsEnd(mp)) {` | No dentro de `#ifdef PORT` (decodificador en `port/src/plat_movie.c` y `port/src/plat_mpeg2.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L301) | 301 | `Movie_Run` | `sceMpegGetPicture` | `sceMpegGetPicture(mp, m->rgb, MOVIE_MB_COUNT);` | No dentro de `#ifdef PORT` (decodificador en `port/src/plat_movie.c` y `port/src/plat_mpeg2.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L542) | 542 | `Movie_Demux` | `sceMpegDemuxPss` | `n = sceMpegDemuxPss(mp, buf->data + buf->pos, len);` | No dentro de `#ifdef PORT` (decodificador en `port/src/plat_movie.c` y `port/src/plat_mpeg2.c`) |


### Subsistema 7: 7. Lógica restante (IOP Heap, SIF RPC del sistema, VU0)

| Archivo | Línea | Función | Llamada / Símbolo | Código en Línea | ¿Reemplazado en PC dentro de `#ifdef PORT`? |
| :--- | :---: | :--- | :--- | :--- | :--- |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L20) | 20 | `sceSifInitIopHeap` | `sceSifInitIopHeap` | `int sceSifInitIopHeap() { return 0; }` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L23) | 23 | `sceSifAllocIopHeap` | `sceSifAllocIopHeap` | `void *sceSifAllocIopHeap(int size) { return Port_LowAlloc...` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L24) | 24 | `sceSifFreeIopHeap` | `sceSifFreeIopHeap` | `int sceSifFreeIopHeap(void *addr) { Port_LowFree(addr); r...` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L25) | 25 | `sceSifQueryTotalFreeMemSize` | `sceSifQueryTotalFreeMemSize` | `int sceSifQueryTotalFreeMemSize() { return 0x100000; }` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L26) | 26 | `sceSifQueryMaxFreeMemSize` | `sceSifQueryMaxFreeMemSize` | `int sceSifQueryMaxFreeMemSize() { return 0x100000; }` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L28) | 28 | `sceSifBindRpc` | `sceSifBindRpc` | `int sceSifBindRpc(void *client, int id, int mode) { (void...` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L29) | 29 | `func_002B4AF0` | `func_002B4AF0` | `int func_002B4AF0() { return 0; }      /* sceSifCheckStat...` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_sys.c](../../port/src/plat_sys.c#L11) | 11 | `sceSifInitRpc` | `sceSifInitRpc` | `void sceSifInitRpc(uint32_t mode) { (void)mode; }` | Implementación/stub en PC port (`port/src/plat_sys.c`) |
| [port/src/plat_sys.c](../../port/src/plat_sys.c#L12) | 12 | `sceSifRebootIop` | `sceSifRebootIop` | `int sceSifRebootIop(char *img) { (void)img; return 1; }` | Implementación/stub en PC port (`port/src/plat_sys.c`) |
| [port/src/plat_sys.c](../../port/src/plat_sys.c#L13) | 13 | `sceSifSyncIop` | `sceSifSyncIop` | `int sceSifSyncIop(void) { return 1; }` | Implementación/stub en PC port (`port/src/plat_sys.c`) |
| [port/src/plat_sys.c](../../port/src/plat_sys.c#L14) | 14 | `sceSifLoadModule` | `sceSifLoadModule` | `int sceSifLoadModule(char *name, int argLen, char *args) ...` | Implementación/stub en PC port (`port/src/plat_sys.c`) |
| [port/src/plat_sys.c](../../port/src/plat_sys.c#L23) | 23 | `func_002BB390` | `func_002BB390` | `long func_002BB390(void) {` | Implementación/stub en PC port (`port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L86) | 86 | `Sys_RebootIop` | `sceSifInitRpc` | `sceSifInitRpc(0);` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L89) | 89 | `Sys_RebootIop` | `sceSifRebootIop` | `while (!sceSifRebootIop("cdrom0:\\IRX\\IOPRP300.IMG;1")) {` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L91) | 91 | `Sys_RebootIop` | `sceSifSyncIop` | `while (!sceSifSyncIop()) {` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L93) | 93 | `Sys_RebootIop` | `sceSifInitRpc` | `sceSifInitRpc(0);` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L101) | 101 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\SIO2MAN.IRX;1", 0,...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L103) | 103 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\MCMAN.IRX;1", 0, N...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L105) | 105 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\MCSERV.IRX;1", 0, ...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L107) | 107 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\DBCMAN.IRX;1", 0, ...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L109) | 109 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\SIO2D.IRX;1", 0, N...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L111) | 111 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\DS2U_D.IRX;1", 0, ...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L113) | 113 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\LIBSD.IRX;1", 0, N...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L115) | 115 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\SDRDRV.IRX;1", 0, ...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L117) | 117 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\MODHSYN.IRX;1", 0,...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L119) | 119 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\MODSESQ2.IRX;1", 0...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L121) | 121 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\CDVDSTM.IRX;1", 0,...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L123) | 123 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\SOUNDS.IRX;1", 0, ...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L125) | 125 | `Sys_LoadIopModules` | `sceSifLoadModule` | `while (sceSifLoadModule("cdrom0:\\IRX\\CRI_ADXI.IRX;1", 0...` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/main.c](../../src/main.c#L146) | 146 | `Game_Main` | `Vu0_Init` | `Vu0_Init();` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_b.c` / `softfloat_ps2.c`) |
| [src/sys/iop_heap.c](../../src/sys/iop_heap.c#L11) | 11 | `Sys_InitIopHeap` | `sceSifInitIopHeap` | `sceSifInitIopHeap();` | No dentro de `#ifdef PORT` (stub en `port/src/plat_stub.c`) |
| [src/sys/iop_heap.c](../../src/sys/iop_heap.c#L16) | 16 | `IopHeap_Alloc` | `sceSifAllocIopHeap` | `return sceSifAllocIopHeap(size);` | No dentro de `#ifdef PORT` (stub en `port/src/plat_stub.c`) |
| [src/sys/iop_heap.c](../../src/sys/iop_heap.c#L21) | 21 | `IopHeap_Free` | `sceSifFreeIopHeap` | `return sceSifFreeIopHeap(addr);` | No dentro de `#ifdef PORT` (stub en `port/src/plat_stub.c`) |
| [src/sys/iop_heap.c](../../src/sys/iop_heap.c#L26) | 26 | `IopHeap_GetTotalFree` | `sceSifQueryTotalFreeMemSize` | `return sceSifQueryTotalFreeMemSize();` | No dentro de `#ifdef PORT` (stub en `port/src/plat_stub.c`) |
| [src/sys/iop_heap.c](../../src/sys/iop_heap.c#L31) | 31 | `IopHeap_GetMaxFree` | `sceSifQueryMaxFreeMemSize` | `return sceSifQueryMaxFreeMemSize();` | No dentro de `#ifdef PORT` (stub en `port/src/plat_stub.c`) |
| [src/sys/vu0_a_c.c](../../src/sys/vu0_a_c.c#L55) | 55 | `(file scope)` | `IVec4_Swap` | `".globl IVec4_Swap\n"` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_a_c.c](../../src/sys/vu0_a_c.c#L56) | 56 | `(file scope)` | `IVec4_Swap` | `".type IVec4_Swap, @function\n"` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_a_c.c](../../src/sys/vu0_a_c.c#L59) | 59 | `(file scope)` | `IVec4_Swap` | `"IVec4_Swap:\n"` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_a_c.c](../../src/sys/vu0_a_c.c#L66) | 66 | `(file scope)` | `IVec4_Swap` | `".size IVec4_Swap, . - IVec4_Swap\n"` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_a_c.c](../../src/sys/vu0_a_c.c#L148) | 148 | `(file scope)` | `IVec4_Copy` | `".globl IVec4_Copy\n"` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_a_c.c](../../src/sys/vu0_a_c.c#L149) | 149 | `(file scope)` | `IVec4_Copy` | `".type IVec4_Copy, @function\n"` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_a_c.c](../../src/sys/vu0_a_c.c#L152) | 152 | `(file scope)` | `IVec4_Copy` | `"IVec4_Copy:\n"` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_a_c.c](../../src/sys/vu0_a_c.c#L157) | 157 | `(file scope)` | `IVec4_Copy` | `".size IVec4_Copy, . - IVec4_Copy\n"` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_a_c_b.c](../../src/sys/vu0_a_c_b.c#L209) | 209 | `(file scope)` | `Vu0_InitAxisRegs` | `".globl Vu0_InitAxisRegs\n"` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_a_c_b.c](../../src/sys/vu0_a_c_b.c#L210) | 210 | `(file scope)` | `Vu0_InitAxisRegs` | `".type Vu0_InitAxisRegs, @function\n"` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_a_c_b.c](../../src/sys/vu0_a_c_b.c#L213) | 213 | `(file scope)` | `Vu0_InitAxisRegs` | `"Vu0_InitAxisRegs:\n"` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_a_c_b.c](../../src/sys/vu0_a_c_b.c#L219) | 219 | `(file scope)` | `Vu0_InitAxisRegs` | `".size Vu0_InitAxisRegs, . - Vu0_InitAxisRegs\n"` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_b_c.c](../../src/sys/vu0_b_c.c#L1083) | 1083 | `Vu0_Init` | `Vu0_Init` | `void Vu0_Init(void) {` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |
| [src/sys/vu0_b_c.c](../../src/sys/vu0_b_c.c#L1085) | 1085 | `Vu0_Init` | `Vu0_InitAxisRegs` | `Vu0_InitAxisRegs();` | No dentro de `#ifdef PORT` (implementado en C en `src/port/vu0_a.c` / `softfloat_ps2.c`) |


---
