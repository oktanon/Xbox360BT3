# Esquema de Endianness por Struct (`docs/port/endian-schema.md`)

Catálogo de los **formatos binarios en disco** del juego (todos little-endian, según [docs/port/endian.md](../../docs/port/endian.md)) que requieren byteswap al cargarse en PowerPC Xenon. Por cada struct se documenta: offset, nombre, tipo, tamaño y acción de port a Xbox 360.

Este documento **complementa** `endian.md` (política general y primitivas) y **`xbox360-todo.md` sección 3** (inventario exhaustivo por subsistema). El objetivo aquí es tener, para los structs más críticos, un mapa campo-a-campo con la acción concreta de byteswap, de modo que la fase 2 (fase de archivos) tenga una referencia única.

## Convenciones

- **Endianness en disco**: siempre little-endian (todos los datos del juego son LE).
- **Endianness en memoria en 360**: big-endian nativo (PowerPC Xenon). Los enteros multibyte leídos de disco deben pasarse por `ReadLE*` / `SWAP*` (definidos en `endian.md` sección 4.2).
- **Bitfields**: en GCC PowerPC los bitfields se asignan desde el MSB. Ver `endian.md` sección 4.4 (máscaras verificadas) y 4.5 (paquetes DMA).
- **Acción "in-place relocation"**: algunos structs tienen offsets relativos que se sobreescriben **in-situ** con punteros absolutos de PPC tras la carga. Ver `endian.md` sección 4.3 Categoría 2.

## Structs cubiertos

1. [`TexFile` y `TexEntry`](#1-texfile-y-texentry) — texturas.
2. [`ObjMdlFile`](#2-objmdlfile) — modelos 3D.
3. [`StgRelocHdr` y sub-structs](#3-stgrelochdr-y-sub-structs) — escenario (octree + tablas).
4. [`SaveData`](#4-savedata) — partida guardada (resumen, ver `endian.md` 3.9 para detalle).
5. [`McFlowReplayFile`](#5-mcflowreplayfile) — replay de batalla (resumen).

Los structs 4 y 5 son R/W (lectura y escritura); los structs 1-3 son R/W in-situ (ver `endian.md` 3.3, 3.4, 3.5).

---

## 1. TexFile y TexEntry

Header: [include/sys/gfxm_b_b.h](../../include/sys/gfxm_b_b.h). Cargador: `Tex_ByteSwapFile` + `Res_RelocateOffsets` en [src/sys/gfxm_b_b.c](../../src/sys/gfxm_b_b.c).

### 1.1. `TexFile` (cabecera, 0x10 bytes)

| Offset | Nombre | Tipo | Tamaño | Endianness en disco | Acción en 360 |
| :---: | :--- | :--- | :---: | :---: | :--- |
| 0x00 | `count` | `u32` | 4 | LE | `ReadLE32(p + 0x00)` |
| 0x04 | `entOfs` | `u32` | 4 | LE | `ReadLE32(p + 0x04)`: offset de la tabla de entradas, en **palabras** desde el inicio del archivo |
| 0x08 | `unk08` | `u8[8]` | 8 | — | Sin swap (bytes sueltos) |
| 0x10 | `ent` | `TexEntry *` | 4 | — | **Res_RelocateOffsets** sobreescribe in-situ con `(TexEntry *)((u8 *)base + entOfs * 4)` (no se swapea; es una dirección de memoria ya relocalizada) |

### 1.2. `TexEntry` (entrada, 0x40 bytes)

| Offset | Nombre | Tipo | Tamaño | Endianness en disco | Acción en 360 |
| :---: | :--- | :--- | :---: | :---: | :--- |
| 0x00 | `pixOfs` | `u32` | 4 | LE | `ReadLE32`; **relocalizado in-situ** a `(void *)(base + (pixOfs << 2))` |
| 0x04 | `clutOfs` | `u32` | 4 | LE | `ReadLE32`; **relocalizado in-situ** a `(void *)(base + (clutOfs << 2))` |
| 0x08 | `pixSize` | `s32` | 4 | LE | `ReadLE32` |
| 0x0C | `clutSize` | `s32` | 4 | LE | `ReadLE32` |
| 0x10 | `tbpStep` | `s32` | 4 | LE | `ReadLE32` |
| 0x14 | `cbpStep` | `s32` | 4 | LE | `ReadLE32` |
| 0x18 | `pixBlt` | `u32` | 4 | LE | `ReadLE32` (parte alta de `BITBLTBUF`) |
| 0x1C | `clutBlt` | `u32` | 4 | LE | `ReadLE32` |
| 0x20 | `tbpOfs` | `s32` | 4 | LE | `ReadLE32` |
| 0x24 | `cbpOfs` | `s32` | 4 | LE | `ReadLE32` |
| 0x28 | `unk28` | `u8[8]` | 8 | — | Sin swap |
| 0x30 | `tex0` | `u64` | 8 | LE | `ReadLE64` (registro TEX0 del GS) |
| 0x38 | `pix` | `void *` | 4 | — | Res_RelocateOffsets (sobreescribe con `base + pixOfs*4`) |
| 0x3C | `clut` | `void *` | 4 | — | Res_RelocateOffsets (sobreescite con `base + clutOfs*4`) |

**Notas**:
- `Tex_ByteSwapFile` recorre el array de `count` entradas tras la carga y aplica `SWAP32` a todos los campos multibyte. Una vez swapeado, `Res_RelocateOffsets` opera en big-endian nativo de PPC: los punteros resultantes son direcciones absolutas correctas sin mezcla de endianness.
- `tex0` (TEX0) se interpreta como un registro de 64 bits fijo, no como un bitfield: en PS2/PC se construye con shifts/ORs y se almacena tal cual; en 360 también, pero la lectura desde disco requiere `ReadLE64`.

---

## 2. ObjMdlFile

Header: [include/sys/gfxm_e.h](../../include/sys/gfxm_e.h). Cargador: `Mdl_ByteSwapFile` + `BtlObjMdl_InitAlphaSlots` en [src/sys/gfxm_e_b.c](../../src/sys/gfxm_e_b.c).

### 2.1. `ObjMdlFile` (cabecera, 0x70 bytes)

| Offset | Nombre | Tipo | Tamaño | Endianness en disco | Acción en 360 |
| :---: | :--- | :--- | :---: | :---: | :--- |
| 0x00 | `unk00` | `u8[0xC]` | 12 | — | Sin swap |
| 0x0C | `flags` + `bound` | bitfield `u32 : 31` + `u32 : 1` | 4 | LE | Swap de la palabra completa (`ReadLE32`); `bound` queda en bit 31 (MSB) por orden PowerPC — ver `endian.md` 4.4 |
| 0x10 | `unk10` | `u8[0x18]` | 24 | — | Sin swap |
| 0x28 | `alphaRow` | `s32` | 4 | LE | `ReadLE32` |
| 0x2C | `alpha` | `u8[16]` | 16 | — | Sin swap; **modificado in-situ** en `BtlObjMdl_InitAlphaSlots` (`src/sys/gfxm_e_b.c:155-157`) |
| 0x3C | `unk3C` | `u8[0x12]` | 18 | — | Sin swap |
| 0x4E | `texFirst` | `u8` | 1 | — | Sin swap |
| 0x4F | `texCount` | `u8` | 1 | — | Sin swap |
| 0x50 | `unk50` | `u8[4]` | 4 | — | Sin swap |
| 0x54 | `texNoAlpha` | `u8` | 1 | — | Sin swap |
| 0x55 | `unk55` | `u8[3]` | 3 | — | Sin swap |
| 0x58 | `tbp` | `s32` | 4 | LE | `ReadLE32`; **modificado in-situ** en `src/sys/gfxm_e_b.c:174-179` con `0x3480` |
| 0x5C | `cbp` | `s32` | 4 | LE | `ReadLE32`; modificado in-situ con `0x3C00` |
| 0x60 | `cbp2` | `s32` | 4 | LE | `ReadLE32`; modificado in-situ con `0x3D00` |
| 0x64 | `tbp2` | `s32` | 4 | LE | `ReadLE32`; modificado in-situ con `0x3D40` |
| 0x68 | `unk68` | `s32` | 4 | LE | `ReadLE32` |
| 0x6C | `meshOfs` | `s32` | 4 | LE | `ReadLE32`: offset en **bytes** de la lista de mallas (vs `TexFile.entOfs` que es en palabras) |

**Notas**:
- El bitfield `flags : 31 / bound : 1` en PPC queda con `bound` en el MSB (bit 31). El único acceso en `src/` es `mdl->file->bound` en [src/sys/gfxm_e_b.c:173](../../src/sys/gfxm_e_b.c#L173). Ver `endian.md` 4.4 para la estrategia de port (aceptar la posición MSB o refactorizar el bitfield a `u32 bound : 1` separado).
- Los campos `tbp/cbp/cbp2/tbp2` se sobrescriben con valores fijos (`0x3480`, `0x3C00`, `0x3D00`, `0x3D40`); son escrituras a un campo ya swapeado, así que en PPC se escriben directamente como `s32` big-endian-nativo.
- `BtlObjMdl_InitAlphaSlots` modifica las entradas `alpha[16]` in-situ (`src/sys/gfxm_e_b.c:155-157`): asigna un slot de paleta CLUT a cada material. No requiere swap (bytes sueltos).

---

## 3. StgRelocHdr y sub-structs

Header: [include/sys/gfxm_e.h](../../include/sys/gfxm_e.h). Cargador: `BtlStage_Relocate` y `StgOctree_Relocate` en [src/sys/gfxm_e_d.c](../../src/sys/gfxm_e_d.c).

`StgRelocHdr` es la **cabecera del escenario**. Tras cargar el archivo desde disco, `BtlStage_Relocate` recorre la cabecera y todas las sub-tablas, convirtiendo cada offset relativo (LE 32 bits) en un puntero absoluto PPC (`base + offset`). El macro clave es `STG_RELOC` en `src/sys/gfxm_e_d.c:14`:

```c
#define STG_RELOC(type, field) ((field) = (type)(base + (s32)(field)))
```

### 3.1. `StgRelocHdr` (cabecera, 0x60 bytes)

| Offset | Nombre | Tipo | Tamaño | Endianness en disco | Acción en 360 |
| :---: | :--- | :--- | :---: | :---: | :--- |
| 0x00 | `unk00` | `u8[0x10]` | 16 | — | Sin swap |
| 0x10 | `timerCount` | `u32` | 4 | LE | `ReadLE32` |
| 0x14 | `timers` | `StgRelocTimer *` | 4 | LE | `ReadLE32` + `STG_RELOC` (offset → ptr) |
| 0x18 | `aCount` | `u32` | 4 | LE | `ReadLE32` |
| 0x1C | `a` | `StgRelocA *` | 4 | LE | `ReadLE32` + `STG_RELOC` |
| 0x20 | `bCount` | `u32` | 4 | LE | `ReadLE32` |
| 0x24 | `b` | `StgRelocB *` | 4 | LE | `ReadLE32` + `STG_RELOC` |
| 0x28 | `unk28` | `s32` | 4 | LE | `ReadLE32` |
| 0x2C | `unk2C` | `void *` | 4 | LE | `ReadLE32` + `STG_RELOC` |
| 0x30 | `cCount` | `u32` | 4 | LE | `ReadLE32` |
| 0x34 | `c` | `StgRelocC *` | 4 | LE | `ReadLE32` + `STG_RELOC` |
| 0x38 | `dCount` | `u32` | 4 | LE | `ReadLE32` |
| 0x3C | `d` | `StgRelocD *` | 4 | LE | `ReadLE32` + `STG_RELOC` |
| 0x40 | `unk40` | `void *` | 4 | LE | `ReadLE32` + `STG_RELOC` |
| 0x44 | `eCount` | `u32` | 4 | LE | `ReadLE32` |
| 0x48 | `e` | `StgRelocE *` | 4 | LE | `ReadLE32` + `STG_RELOC` |
| 0x4C | `unk4C` | `s32` | 4 | LE | `ReadLE32` |
| 0x50 | `tree` | `StgOctNode *` | 4 | LE | `ReadLE32` + `STG_RELOC` |
| 0x54 | `texOfs` | `s32` | 4 | LE | `ReadLE32`: offset en **palabras** desde `base` |
| 0x58 | `tex` | `TexFile *` | 4 | — | `Res_RelocateOffsets` sobreescribe in-situ con `(TexFile *)((u8 *)base + texOfs * 4)` |
| 0x5C | `base` | `s32 *` | 4 | — | Argumento de `BtlStage_Relocate`, no de disco |

**Notas**:
- El `STG_RELOC` macro reemplaza cada campo puntero por `(type)(base + (s32)(field))` **después** del swap: la palabra se lee con `ReadLE32` (LE → BE), queda como `s32` nativo en PPC, y luego se interpreta como offset para producir la dirección absoluta.
- `tex` se calcula aparte porque necesita alineación y el factor de palabras: `texOfs * 4`.

### 3.2. `StgOctNode` (nodo del octree, 0x40 bytes)

`StgOctree_Relocate` (en `src/sys/gfxm_e_d.c:17-45`) aplica `STG_RELOC` recursivamente a los 8 hijos y a `list`.

| Offset | Nombre | Tipo | Tamaño | Endianness en disco | Acción en 360 |
| :---: | :--- | :--- | :---: | :---: | :--- |
| 0x00 | `unk00` | `s32 *` | 4 | LE | `ReadLE32` + `STG_RELOC` cuando no es 0 |
| 0x04 | `child[2][2][2]` | `StgOctNode *[8]` | 32 | LE | `ReadLE32` por hijo + `STG_RELOC` recursivo |
| 0x24 | `unk24` | `u8[0x14]` | 20 | — | Sin swap |
| 0x38 | `count` | `s32` | 4 | LE | `ReadLE32` |
| 0x3C | `list` | `s32 **` | 4 | LE | `ReadLE32` + `STG_RELOC`; `0` y `-1` se convierten a `NULL` (`src/sys/gfxm_e_d.c:29-30`) |

### 3.3. Sub-structs (`StgRelocTimer`, `StgRelocA`, `B`, `C`, `D`, `E`)

Cada sub-struct contiene un puntero (o varios) que se relocaliza con `STG_RELOC`. Detalle completo en [include/sys/gfxm_e.h](../../include/sys/gfxm_e.h#L181-L251). Patrones:

| Sub-struct | Tamaño | Campos puntero a swapear | Notas |
| :--- | :---: | :--- | :--- |
| `StgRelocTimer` | 0x10 | `ptr` (0x04) | Cada timer tiene un `unk00` LE y un `ptr` a relocalizar |
| `StgRelocA2` | 0x10 | `ptr8`, `ptrC` (0x08, 0x0C) | Dos punteros por entrada |
| `StgRelocA1` | 0x10 | `items` (0x04), `opt` (0x08) | `items` es array de `StgRelocA2` |
| `StgRelocA` | 0x08 | `items` (0x04) | Cabecera de la tabla A |
| `StgRelocB1` | 0x20 | `list` (0x04), `opt` (0x08) | `list` es `void**` |
| `StgRelocB` | 0x08 | `items` (0x04) | Cabecera de la tabla B |
| `StgRelocC` | 0x50 | `ptr44` (0x44) | Único puntero |
| `StgRelocD1` | 0x10 | `ptr8` (0x08) | Único puntero |
| `StgRelocD` | 0x40 | `ptr04`, `ptr0C`, `ptr10`, `items` (0x04, 0x0C, 0x10, 0x18) | Cuatro punteros |
| `StgRelocE` | 0x50 | `ptr0C[2]`, `ptr14[2]`, `ptr1C[2]`, `list` (0x0C, 0x14, 0x1C, 0x28) | Tres arrays de 2 y `list` |

Todos los offsets LE 32 bits: `ReadLE32` + `STG_RELOC` cuando es puntero; `ReadLE32` cuando es `count` o `s32` opaco. Los campos `u8[N]` y `u16` no requieren swap (los `u16` se manejan con `ReadLE16` si son críticos; ver `endian.md` 3.7).

---

## 4. SaveData

Header: [include/sys/save.h](../../include/sys/save.h). Tamaño: **0x4000 bytes** (`SAVE_SIZE` en `save.h:13`). Detalle completo en `endian.md` 3.9.

`SaveData` es R/W bidireccional con la memory card. No admite un pase de byteswap in-place porque su layout interno se construye campo a campo y muchos sub-arrays usan tipos de 16 bits (`u16 flags`, `u16 val[3]`, `s16 level`, `u16 item[8]`). Estrategia: funciones explícitas `Save_Deserialize(SaveData *dst, const u8 *srcRaw)` y `Save_Serialize(u8 *dstRaw, const SaveData *src)` usando `ReadLE*`/`WriteLE*` para cada campo (ver `endian.md` 4.3 Categoría 3).

`static_assert(sizeof(SaveData) == 0x4000)` en el build (ver `endian.md` 4.6).

---

## 5. McFlowReplayFile

Header: [include/sys/mcflow_a.h](../../include/sys/mcflow_a.h). Tamaño: **0x1AC00 bytes** (`mcflow_a.h:92`). Mismo patrón que `SaveData`: R/W bidireccional con la memory card, con `McCard_Checksum` sobre los 0x1AC00 bytes tras la serialización.

Sub-structs clave:
- `McFlowHeader` (al inicio, antes de 0x38): 2× `u32 sum[2]`, 2× `u32 hdrSum[2]`, `s32 chara[2][5]`, `u32 count`. Todos LE 32 bits; los `sum` y `hdrSum` se calculan en la serialización (no se swapean al cargar: se sobrescriben con el checksum recalculado).
- `McFlowReplay` (a partir de 0x38, 0x1ABA8 bytes): contiene `McFlowReplayHeader`, `McFlowInput` (por frame), etc. Estructura detallada en [src/sys/mcflow_a.c](../../src/sys/mcflow_a.c).

`static_assert(sizeof(McFlowReplayFile) == 0x1AC00)` en el build (ver `endian.md` 4.6).

---

## Pendiente de flesh-out por sub-fase

Esta primera versión del documento cubre los structs enumerados en `PLAN.md` para la fase 2. Cuando se identifiquen más structs críticos (p. ej. `ObjMdlMesh` con su VIF stream, `BobjAnim`, `FlashTag`, `LipSyncKey`), se añadirán secciones siguiendo el mismo formato. La regla 16 de AGENTS.md (no recortar documentos) sugiere que este archivo puede subdividirse por subsistema si supera ~500 líneas.
