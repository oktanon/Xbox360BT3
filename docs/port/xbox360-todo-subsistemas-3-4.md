# Llamadas a Hardware PS2: Subsistemas 3-4 (Renderer y Sonido)

Parte de [xbox360-todo.md](./xbox360-todo.md) (hub). Ver el indice de
subsistemas en la cabecera del hub. Esta seccion cubre los
**subsistemas 3 y 4** de la regla 3 de AGENTS.md.

---

## 1. Llamadas a Hardware PS2 por Subsistema (Regla 3 de AGENTS.md)

### Subsistema 3: 3. Renderer (GS, DMA, VU1, sceGs*)

| Archivo | Línea | Función | Llamada / Símbolo | Código en Línea | ¿Reemplazado en PC dentro de `#ifdef PORT`? |
| :--- | :---: | :--- | :--- | :--- | :--- |
| [include/sys/dma.h](../../include/sys/dma.h#L86) | 86 | `(file scope)` | `Port_DmaChcr` | `#define D1_CHCR Port_DmaChcr(1)` | Sí (dentro de `#ifdef PORT`) |
| [include/sys/dma.h](../../include/sys/dma.h#L93) | 93 | `(file scope)` | `Port_DmaChcr` | `#define D2_CHCR Port_DmaChcr(2)` | Sí (dentro de `#ifdef PORT`) |
| [include/sys/dma.h](../../include/sys/dma.h#L123) | 123 | `(file scope)` | `Dma_Flush` | `void Dma_Flush(void);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L133) | 133 | `(file scope)` | `Dma_InitController` | `void Dma_InitController(void);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L134) | 134 | `(file scope)` | `Dma_WaitCond` | `void Dma_WaitCond(void);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L135) | 135 | `(file scope)` | `Dma_WaitVu1` | `void Dma_WaitVu1(void);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L136) | 136 | `(file scope)` | `Dma_SendVif1` | `void Dma_SendVif1(u32 chain);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L137) | 137 | `(file scope)` | `Dma_SendVif1Sync` | `void Dma_SendVif1Sync(u32 chain);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L138) | 138 | `(file scope)` | `Dma_SendVif1Spr` | `void Dma_SendVif1Spr(u32 chain);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L139) | 139 | `(file scope)` | `Dma_SendVif1SprSync` | `void Dma_SendVif1SprSync(u32 chain);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L140) | 140 | `(file scope)` | `Dma_SendGifChain` | `void Dma_SendGifChain(u32 chain);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L141) | 141 | `(file scope)` | `Dma_SendGifChainSync` | `void Dma_SendGifChainSync(u32 chain);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L142) | 142 | `(file scope)` | `Dma_SendGifNoWait` | `void Dma_SendGifNoWait(u32 addr, u32 qwc);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L143) | 143 | `(file scope)` | `Dma_SendGif` | `void Dma_SendGif(u32 addr, u32 qwc);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L144) | 144 | `(file scope)` | `Dma_WaitGif` | `void Dma_WaitGif(void);` | No dentro de `#ifdef PORT` (código común) |
| [include/sys/dma.h](../../include/sys/dma.h#L145) | 145 | `(file scope)` | `Dma_SendToIop` | `void Dma_SendToIop(void *iopAddr, void *data, s32 size);` | No dentro de `#ifdef PORT` (código común) |
| [port/src/gs/gs_sony.c](../../port/src/gs/gs_sony.c#L13) | 13 | `(file scope)` | `sceGsSwapDBuff` | `int gGsMainFbp = -1; /* frame-buffer address of the frame...` | Implementación/stub en PC port (`port/src/gs/gs_sony.c`) |
| [port/src/gs/gs_sony.c](../../port/src/gs/gs_sony.c#L54) | 54 | `sceGsSetDefDBuff` | `sceGsSetDefDBuff` | `int sceGsSetDefDBuff(void *p, int psm, int w, int h, int ...` | Implementación/stub en PC port (`port/src/gs/gs_sony.c`) |
| [port/src/gs/gs_sony.c](../../port/src/gs/gs_sony.c#L64) | 64 | `sceGsPutDrawEnv` | `sceGsPutDrawEnv` | `int sceGsPutDrawEnv(uint64_t *giftag) {` | Implementación/stub en PC port (`port/src/gs/gs_sony.c`) |
| [port/src/gs/gs_sony.c](../../port/src/gs/gs_sony.c#L69) | 69 | `sceGsSwapDBuff` | `sceGsSwapDBuff` | `int sceGsSwapDBuff(void *p, int id) {` | Implementación/stub en PC port (`port/src/gs/gs_sony.c`) |
| [port/src/gs/gs_sony.c](../../port/src/gs/gs_sony.c#L73) | 73 | `sceGsSwapDBuff` | `sceGsPutDrawEnv` | `return sceGsPutDrawEnv((id & 1) ? db->giftag1 : db->gifta...` | Implementación/stub en PC port (`port/src/gs/gs_sony.c`) |
| [port/src/gs/snd_se.c](../../port/src/gs/snd_se.c#L431) | 431 | `sceSifSetDma` | `sceSifSetDma` | `uint32_t sceSifSetDma(SifDmaData *d, int count) {` | Implementación/stub en PC port (`port/src/gs/snd_se.c`) |
| [port/src/plat_gs.c](../../port/src/plat_gs.c#L29) | 29 | `(file scope)` | `Port_DmaChcr` | `volatile uint32_t *Port_DmaChcr(int channel) {` | Implementación/stub en PC port (`port/src/plat_gs.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L31) | 31 | `sceSifDmaStat` | `sceSifDmaStat` | `int sceSifDmaStat() { return -1; }     /* transfer finish...` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L42) | 42 | `sceGsResetGraph` | `sceGsResetGraph` | `void sceGsResetGraph() {}` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L43) | 43 | `sceGsResetPath` | `sceGsResetPath` | `void sceGsResetPath() {}` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L47) | 47 | `sceGsSetDefStoreImage` | `sceGsSetDefStoreImage` | `int sceGsSetDefStoreImage(void *si, int sbp, int sbw, int...` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L54) | 54 | `sceGsExecStoreImage` | `sceGsExecStoreImage` | `int sceGsExecStoreImage(void *si, void *dst) {` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L60) | 60 | `sceGsSyncPath` | `sceGsSyncPath` | `int sceGsSyncPath() { return 0; }` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L61) | 61 | `sceGsSyncVCallback` | `sceGsSyncVCallback` | `void *sceGsSyncVCallback(int (*handler)(int)) { void *old...` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L238) | 238 | `sceGsSyncV` | `sceGsSyncV` | `int sceGsSyncV() { Port_VBlank(); return 0; }` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [src/sys/dma.c](../../src/sys/dma.c#L63) | 63 | `Dma_Flush` | `Dma_Flush` | `void Dma_Flush(void) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L81) | 81 | `Dma_Flush` | `Dma_SendVif1` | `Dma_SendVif1(gDmaBuf[gDmaBufIdx]);` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L169) | 169 | `Dma_InitController` | `Dma_InitController` | `void Dma_InitController(void) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L176) | 176 | `Dma_WaitCond` | `Dma_WaitCond` | `void Dma_WaitCond(void) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L189) | 189 | `Dma_WaitVu1` | `Dma_WaitVu1` | `void Dma_WaitVu1(void) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L202) | 202 | `Dma_SendVif1` | `Dma_SendVif1` | `void Dma_SendVif1(u32 chain) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L204) | 204 | `Dma_SendVif1` | `Dma_WaitCond` | `Dma_WaitCond();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L207) | 207 | `Dma_SendVif1` | `VIF1_STAT` | `while (*VIF1_STAT & 0x1F000003) {` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L209) | 209 | `Dma_SendVif1` | `Dma_WaitVu1` | `Dma_WaitVu1();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L210) | 210 | `Dma_SendVif1` | `GIF_STAT` | `while (*GIF_STAT & 0xC00) {` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L220) | 220 | `Dma_SendVif1Sync` | `Dma_SendVif1Sync` | `void Dma_SendVif1Sync(u32 chain) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L221) | 221 | `Dma_SendVif1Sync` | `Dma_WaitCond` | `Dma_WaitCond();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L222) | 222 | `Dma_SendVif1Sync` | `VIF1_STAT` | `while (*VIF1_STAT & 0x1F000003) {` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L224) | 224 | `Dma_SendVif1Sync` | `Dma_WaitVu1` | `Dma_WaitVu1();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L225) | 225 | `Dma_SendVif1Sync` | `GIF_STAT` | `while (*GIF_STAT & 0xC00) {` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L232) | 232 | `Dma_SendVif1Sync` | `Dma_WaitCond` | `Dma_WaitCond();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L233) | 233 | `Dma_SendVif1Sync` | `VIF1_STAT` | `while (*VIF1_STAT & 0x1F000003) {` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L235) | 235 | `Dma_SendVif1Sync` | `Dma_WaitVu1` | `Dma_WaitVu1();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L236) | 236 | `Dma_SendVif1Sync` | `GIF_STAT` | `while (*GIF_STAT & 0xC00) {` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L241) | 241 | `Dma_SendVif1Spr` | `Dma_SendVif1Spr` | `void Dma_SendVif1Spr(u32 chain) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L242) | 242 | `Dma_SendVif1Spr` | `Dma_WaitCond` | `Dma_WaitCond();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L243) | 243 | `Dma_SendVif1Spr` | `VIF1_STAT` | `while (*VIF1_STAT & 0x1F000003) {` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L245) | 245 | `Dma_SendVif1Spr` | `Dma_WaitVu1` | `Dma_WaitVu1();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L246) | 246 | `Dma_SendVif1Spr` | `GIF_STAT` | `while (*GIF_STAT & 0xC00) {` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L256) | 256 | `Dma_SendVif1SprSync` | `Dma_SendVif1SprSync` | `void Dma_SendVif1SprSync(u32 chain) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L257) | 257 | `Dma_SendVif1SprSync` | `Dma_WaitCond` | `Dma_WaitCond();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L258) | 258 | `Dma_SendVif1SprSync` | `VIF1_STAT` | `while (*VIF1_STAT & 0x1F000003) {` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L260) | 260 | `Dma_SendVif1SprSync` | `Dma_WaitVu1` | `Dma_WaitVu1();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L261) | 261 | `Dma_SendVif1SprSync` | `GIF_STAT` | `while (*GIF_STAT & 0xC00) {` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L268) | 268 | `Dma_SendVif1SprSync` | `Dma_WaitCond` | `Dma_WaitCond();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L269) | 269 | `Dma_SendVif1SprSync` | `VIF1_STAT` | `while (*VIF1_STAT & 0x1F000003) {` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L271) | 271 | `Dma_SendVif1SprSync` | `Dma_WaitVu1` | `Dma_WaitVu1();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L272) | 272 | `Dma_SendVif1SprSync` | `GIF_STAT` | `while (*GIF_STAT & 0xC00) {` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L277) | 277 | `Dma_SendGifChain` | `Dma_SendGifChain` | `void Dma_SendGifChain(u32 chain) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L279) | 279 | `Dma_SendGifChain` | `D2_CHCR` | `while (*D2_CHCR & 0x100) {` | Sí (redefinido a `Port_DmaChcr(n)` bajo `#ifdef PORT` en `include/sys/dma.h`) |
| [src/sys/dma.c](../../src/sys/dma.c#L281) | 281 | `Dma_SendGifChain` | `GIF_STAT` | `} while (*GIF_STAT & 0xC00);` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L290) | 290 | `Dma_SendGifChainSync` | `Dma_SendGifChainSync` | `void Dma_SendGifChainSync(u32 chain) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L292) | 292 | `Dma_SendGifChainSync` | `D2_CHCR` | `while (*D2_CHCR & 0x100) {` | Sí (redefinido a `Port_DmaChcr(n)` bajo `#ifdef PORT` en `include/sys/dma.h`) |
| [src/sys/dma.c](../../src/sys/dma.c#L294) | 294 | `Dma_SendGifChainSync` | `GIF_STAT` | `} while (*GIF_STAT & 0xC00);` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L301) | 301 | `Dma_SendGifChainSync` | `D2_CHCR` | `while (*D2_CHCR & 0x100) {` | Sí (redefinido a `Port_DmaChcr(n)` bajo `#ifdef PORT` en `include/sys/dma.h`) |
| [src/sys/dma.c](../../src/sys/dma.c#L303) | 303 | `Dma_SendGifChainSync` | `GIF_STAT` | `} while (*GIF_STAT & 0xC00);` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L307) | 307 | `Dma_SendGifNoWait` | `Dma_SendGifNoWait` | `void Dma_SendGifNoWait(u32 addr, u32 qwc) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L309) | 309 | `Dma_SendGifNoWait` | `D2_CHCR` | `while (*D2_CHCR & 0x100) {` | Sí (redefinido a `Port_DmaChcr(n)` bajo `#ifdef PORT` en `include/sys/dma.h`) |
| [src/sys/dma.c](../../src/sys/dma.c#L311) | 311 | `Dma_SendGifNoWait` | `GIF_STAT` | `} while (*GIF_STAT & 0xC00);` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L319) | 319 | `Dma_SendGif` | `Dma_SendGif` | `void Dma_SendGif(u32 addr, u32 qwc) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L321) | 321 | `Dma_SendGif` | `D2_CHCR` | `while (*D2_CHCR & 0x100) {` | Sí (redefinido a `Port_DmaChcr(n)` bajo `#ifdef PORT` en `include/sys/dma.h`) |
| [src/sys/dma.c](../../src/sys/dma.c#L323) | 323 | `Dma_SendGif` | `GIF_STAT` | `} while (*GIF_STAT & 0xC00);` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L329) | 329 | `Dma_SendGif` | `D2_CHCR` | `while (*D2_CHCR & 0x100) {` | Sí (redefinido a `Port_DmaChcr(n)` bajo `#ifdef PORT` en `include/sys/dma.h`) |
| [src/sys/dma.c](../../src/sys/dma.c#L331) | 331 | `Dma_SendGif` | `GIF_STAT` | `} while (*GIF_STAT & 0xC00);` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L335) | 335 | `Dma_WaitGif` | `Dma_WaitGif` | `void Dma_WaitGif(void) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L337) | 337 | `Dma_WaitGif` | `D2_CHCR` | `while (*D2_CHCR & 0x100) {` | Sí (redefinido a `Port_DmaChcr(n)` bajo `#ifdef PORT` en `include/sys/dma.h`) |
| [src/sys/dma.c](../../src/sys/dma.c#L339) | 339 | `Dma_WaitGif` | `GIF_STAT` | `} while (*GIF_STAT & 0xC00);` | No dentro de `#ifdef PORT` directo (mapeado a memoria en `port/src/plat_mem.c:10`) |
| [src/sys/dma.c](../../src/sys/dma.c#L343) | 343 | `Dma_SendToIop` | `Dma_SendToIop` | `void Dma_SendToIop(void *iopAddr, void *data, s32 size) {` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/dma.c](../../src/sys/dma.c#L352) | 352 | `Dma_SendToIop` | `sceSifSetDma` | `id = sceSifSetDma(&dma, 1);` | No dentro de `#ifdef PORT` (emulación en `port/src/gs/snd_se.c`) |
| [src/sys/dma.c](../../src/sys/dma.c#L353) | 353 | `Dma_SendToIop` | `sceSifDmaStat` | `while (sceSifDmaStat(id) >= 0) {` | No dentro de `#ifdef PORT` (stub en `port/src/plat_stub.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L93) | 93 | `Gfx_InitDrawEnv` | `sceGsResetGraph` | `sceGsResetGraph(0, 1, 2, 0);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L94) | 94 | `Gfx_InitDrawEnv` | `sceGsSetDefDBuff` | `sceGsSetDefDBuff(&gGfx.db, 0, GFX_WIDTH, GFX_HEIGHT, 3, G...` | No dentro de `#ifdef PORT` (implementado en `port/src/gs/gs_sony.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L116) | 116 | `Gfx_SyncField` | `sceGsSyncV` | `gGfx.field = sceGsSyncV(0) == 0;` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L154) | 154 | `Gfx_Init` | `sceGsResetPath` | `sceGsResetPath();` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L155) | 155 | `Gfx_Init` | `sceGsResetGraph` | `sceGsResetGraph(0, 1, 2, 1);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L156) | 156 | `Gfx_Init` | `Dma_SendGif` | `Dma_SendGif((u32)pkt, 6);` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L157) | 157 | `Gfx_Init` | `sceGsResetGraph` | `sceGsResetGraph(0, 1, 2, 1);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L158) | 158 | `Gfx_Init` | `sceGsSetDefDBuff` | `sceGsSetDefDBuff(&db, 0, 640, GFX_HEIGHT, 2, GS_PSMZ24, 1);` | No dentro de `#ifdef PORT` (implementado en `port/src/gs/gs_sony.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L160) | 160 | `Gfx_Init` | `sceGsSyncV` | `sceGsSyncV(0);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L161) | 161 | `Gfx_Init` | `sceGsPutDrawEnv` | `sceGsPutDrawEnv(db.giftag0);` | No dentro de `#ifdef PORT` (implementado en `port/src/gs/gs_sony.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L162) | 162 | `Gfx_Init` | `sceGsSyncPath` | `sceGsSyncPath(0, 0);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L163) | 163 | `Gfx_Init` | `sceGsPutDrawEnv` | `sceGsPutDrawEnv(db.giftag1);` | No dentro de `#ifdef PORT` (implementado en `port/src/gs/gs_sony.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L164) | 164 | `Gfx_Init` | `sceGsSyncPath` | `sceGsSyncPath(0, 0);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L188) | 188 | `Gfx_EndFrame` | `sceGsSyncPath` | `sceGsSyncPath(0, 0);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L192) | 192 | `Gfx_EndFrame` | `sceGsSwapDBuff` | `sceGsSwapDBuff(&gGfx.db, gGfx.frame);` | No dentro de `#ifdef PORT` (implementado en `port/src/gs/gs_sony.c`) |
| [src/sys/gfx.c](../../src/sys/gfx.c#L194) | 194 | `Gfx_EndFrame` | `sceGsSyncPath` | `sceGsSyncPath(0, 0);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/late_a.c](../../src/sys/late_a.c#L110) | 110 | `Shen_Main` | `sceGsSyncPath` | `sceGsSyncPath(0, 0);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/late_a.c](../../src/sys/late_a.c#L864) | 864 | `Shen_Run` | `Dma_Flush` | `Dma_Flush();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/movie.c](../../src/sys/movie.c#L59) | 59 | `(file scope)` | `Port_DmaChcr` | `#define D4_CHCR Port_DmaChcr(4)` | Sí (dentro de `#ifdef PORT`) |
| [src/sys/movie.c](../../src/sys/movie.c#L282) | 282 | `Movie_Run` | `sceGsSyncV` | `sceGsSyncV(0);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L302) | 302 | `Movie_Run` | `sceGsSyncPath` | `sceGsSyncPath(0, 0);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L304) | 304 | `Movie_Run` | `sceGsSyncPath` | `sceGsSyncPath(0, 0);` | No dentro de `#ifdef PORT` (stub/emulación en `port/src/plat_stub.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L305) | 305 | `Movie_Run` | `Dma_Flush` | `Dma_Flush();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/movie.c](../../src/sys/movie.c#L319) | 319 | `Movie_Run` | `Dma_Flush` | `Dma_Flush();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/movie.c](../../src/sys/movie.c#L322) | 322 | `Movie_Run` | `Dma_Flush` | `Dma_Flush();` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/snd.c](../../src/sys/snd.c#L414) | 414 | `Snd_SendBank` | `Dma_SendToIop` | `Dma_SendToIop(bank->hdIop, bank->hd, bank->hdSize);` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/snd.c](../../src/sys/snd.c#L415) | 415 | `Snd_SendBank` | `Dma_SendToIop` | `Dma_SendToIop(bank->seqIop, bank->seq, bank->seqSize);` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/snd.c](../../src/sys/snd.c#L417) | 417 | `Snd_SendBank` | `Dma_SendToIop` | `Dma_SendToIop(gSndMgr->iopBuf, bank->body, bank->bodySize);` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/snd.c](../../src/sys/snd.c#L493) | 493 | `Snd_Update` | `Dma_SendToIop` | `Dma_SendToIop(buf, bank->body, bank->bodySize);` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L60) | 60 | `Vu1Pkt_CallProg7` | `VU1_VIF_NOP` | `p[0x3B] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L76) | 76 | `Vu1Pkt_LoadProg7` | `VU1_VIF_NOP` | `p[0x38] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L79) | 79 | `Vu1Pkt_LoadProg7` | `VU1_VIF_NOP` | `p[0x3B] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L94) | 94 | `Vu1Pkt_CallProg0` | `VU1_VIF_NOP` | `p[0x8F] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L108) | 108 | `Vu1Pkt_LoadProg0` | `VU1_VIF_NOP` | `p[0x8C] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L111) | 111 | `Vu1Pkt_LoadProg0` | `VU1_VIF_NOP` | `p[0x8F] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L126) | 126 | `Vu1Pkt_CallProg1` | `VU1_VIF_NOP` | `p[0x8F] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L140) | 140 | `Vu1Pkt_LoadProg1` | `VU1_VIF_NOP` | `p[0x8C] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L143) | 143 | `Vu1Pkt_LoadProg1` | `VU1_VIF_NOP` | `p[0x8F] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L158) | 158 | `Vu1Pkt_CallProg2` | `VU1_VIF_NOP` | `p[0x33] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L176) | 176 | `Vu1Pkt_LoadProg2` | `VU1_VIF_NOP` | `p[0x50] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L179) | 179 | `Vu1Pkt_LoadProg2` | `VU1_VIF_NOP` | `p[0x53] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L190) | 190 | `Vu1Pkt_CallProg4` | `VU1_VIF_NOP` | `p[3] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L194) | 194 | `Vu1Pkt_CallProg4` | `VU1_VIF_NOP` | `p[7] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L210) | 210 | `Vu1Pkt_LoadProg4` | `VU1_VIF_NOP` | `p[0x34] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L213) | 213 | `Vu1Pkt_LoadProg4` | `VU1_VIF_NOP` | `p[0x37] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L228) | 228 | `Vu1Pkt_CallProg5` | `VU1_VIF_NOP` | `p[0x17] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L245) | 245 | `Vu1Pkt_LoadProg5` | `VU1_VIF_NOP` | `p[0x44] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L248) | 248 | `Vu1Pkt_LoadProg5` | `VU1_VIF_NOP` | `p[0x47] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L364) | 364 | `Vu1Pkt_CallProg8` | `VU1_VIF_NOP` | `p[0x2F] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L384) | 384 | `Vu1Pkt_LoadProg8` | `VU1_VIF_NOP` | `p[0x4C] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L387) | 387 | `Vu1Pkt_LoadProg8` | `VU1_VIF_NOP` | `p[0x4F] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L398) | 398 | `Vu1Pkt_CallProg6` | `VU1_VIF_NOP` | `p[3] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L402) | 402 | `Vu1Pkt_CallProg6` | `VU1_VIF_NOP` | `p[7] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L421) | 421 | `Vu1Pkt_LoadProg6` | `VU1_VIF_NOP` | `p[0x48] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/vu1_packet.c](../../src/sys/vu1_packet.c#L424) | 424 | `Vu1Pkt_LoadProg6` | `VU1_VIF_NOP` | `p[0x4B] = VU1_VIF_NOP;` | No dentro de `#ifdef PORT` (código común) |


### Subsistema 4: 4. Sonido (sceSd*, sceSif RPC de sonido, SPU)

| Archivo | Línea | Función | Llamada / Símbolo | Código en Línea | ¿Reemplazado en PC dentro de `#ifdef PORT`? |
| :--- | :---: | :--- | :--- | :--- | :--- |
| [port/src/gs/snd_se.c](../../port/src/gs/snd_se.c#L444) | 444 | `func_002967C0` | `func_002967C0` | `int func_002967C0(int block, int cmd, int core, int mode,...` | Implementación/stub en PC port (`port/src/gs/snd_se.c`) |
| [port/src/gs/snd_se.c](../../port/src/gs/snd_se.c#L458) | 458 | `sceSifCallRpc` | `sceSifCallRpc` | `int sceSifCallRpc(void *client, int fno, int mode, void *...` | Implementación/stub en PC port (`port/src/gs/snd_se.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L34) | 34 | `sceSdRemoteInit` | `sceSdRemoteInit` | `int sceSdRemoteInit() { return 0; }` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [port/src/plat_stub.c](../../port/src/plat_stub.c#L35) | 35 | `func_00296B48` | `func_00296B48` | `int func_00296B48() { return 0; }      /* sceSdRemoteCall...` | Implementación/stub en PC port (`port/src/plat_stub.c`) |
| [src/sys/movie.c](../../src/sys/movie.c#L310) | 310 | `Movie_Run` | `func_00277A70` | `if (ADXF_GetStat(m->adxf) != ADXF_STAT_READING && func_00...` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/movie.c](../../src/sys/movie.c#L408) | 408 | `Movie_CbBackground` | `func_00277A70` | `if (ADXF_GetStat(m->adxf) != ADXF_STAT_READING && func_00...` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/snd.c](../../src/sys/snd.c#L101) | 101 | `Snd_RpcCall` | `sceSifCallRpc` | `sceSifCallRpc(&gSndRpc->client, cmd, 0, data, size, gSndR...` | No dentro de `#ifdef PORT` (emulación en `port/src/gs/snd_se.c`) |
| [src/sys/snd.c](../../src/sys/snd.c#L107) | 107 | `Snd_RpcIsBusy` | `func_002B4AF0` | `return func_002B4AF0(&gSndRpc->client);` | No dentro de `#ifdef PORT` (stub en `port/src/plat_stub.c`) |
| [src/sys/snd.c](../../src/sys/snd.c#L116) | 116 | `Snd_RpcInit` | `sceSifInitRpc` | `sceSifInitRpc(0);` | No dentro de `#ifdef PORT` (stub en `port/src/plat_sys.c`) |
| [src/sys/snd.c](../../src/sys/snd.c#L118) | 118 | `Snd_RpcInit` | `sceSifBindRpc` | `sceSifBindRpc(&gSndRpc->client, SND_RPC_ID, 0);` | No dentro de `#ifdef PORT` (código común) |
| [src/sys/snd.c](../../src/sys/snd.c#L275) | 275 | `Snd_SpuTransfer` | `func_002967C0` | `func_002967C0(1, 0x80D0, 1, 0, iopAddr, spuAddr, size);` | No dentro de `#ifdef PORT` (emulación en `port/src/gs/snd_se.c`) |
| [src/sys/snd.c](../../src/sys/snd.c#L280) | 280 | `Snd_SpuTransferWait` | `func_002967C0` | `return func_002967C0(1, 0x80F0, 1, 1);` | No dentro de `#ifdef PORT` (emulación en `port/src/gs/snd_se.c`) |
| [src/sys/snd.c](../../src/sys/snd.c#L285) | 285 | `Snd_SpuTransferCheck` | `func_002967C0` | `return func_002967C0(1, 0x80F0, 1, 0);` | No dentro de `#ifdef PORT` (emulación en `port/src/gs/snd_se.c`) |
| [src/sys/snd.c](../../src/sys/snd.c#L432) | 432 | `Snd_Init` | `sceSdRemoteInit` | `sceSdRemoteInit();` | No dentro de `#ifdef PORT` (stub en `port/src/plat_stub.c`) |
| [src/sys/snd.c](../../src/sys/snd.c#L433) | 433 | `Snd_Init` | `func_00296B48` | `func_00296B48(5);` | No dentro de `#ifdef PORT` (stub en `port/src/plat_stub.c`) |


