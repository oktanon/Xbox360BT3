# Análisis de Endianness y Lecturas Binarias de Disco (PS2 -> Xbox 360)

## 1. Introducción y Marco Normativo (AGENTS.md)

En la transición de **PlayStation 2** (MIPS R5900, 32 bits, little-endian) a **Xbox 360** (PowerPC Xenon, 32 bits, big-endian), la gestión del endianness es crítica. Todos los archivos de datos del juego (archivos dentro de particiones AFS, paquetes de recursos PACK, animaciones, modelos 3D, texturas, scripts GSC, películas FOD y partidas guardadas en Memory Card) fueron empaquetados originalmente en formato **little-endian**.

En el port previo de PC (x86-64), este problema no existía porque la arquitectura x86 es nativamente little-endian, lo que permitió mantener los casts directos de punteros del código original de PS2. En Xbox 360, desreferenciar directamente un entero de varios bytes o castear un buffer a una estructura binaria resultará en datos invertidos o punteros corruptos que causarán cuelgues inmediatos.

Conforme a la **Regla 2 de [AGENTS.md](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/AGENTS.md)**:
> *"Endianness: todo dato leído del disco (little-endian) pasa por la función de conversión acordada en `docs/port/endian.md`. Prohibido hacer cast directo de buffers a structs con campos multibyte."*

Este documento clasifica exhaustivamente todos los puntos del código donde se leen o escriben estructuras binarias de disco, identifica los patrones de acceso (`Solo lectura` vs `Lectura y Escritura (R/W)`), documenta riesgos de uniones y bitfields, y establece la estrategia y primitivas de conversión requeridas para el port de Xbox 360.

---

## 2. Tipología de Patrones Detectados

1. **Lecturas Multibyte Primitivas**: Lecturas de enteros o flotantes de 16, 32 o 64 bits directamente desde buffers de memoria leídos del disco (`*(u16 *)p`, `*(u32 *)p`, `*(f32 *)p`).
2. **Casts de Buffers a Structs**: Mapeo directo de cabeceras binarias y tablas sobre structs de C (`(TexFile *)buf`, `(ObjMdlFile *)buf`, `(BobjAnim *)buf`).
3. **Indexación de Tablas de Offset (Universal Packs)**: Macros globales y locales que leen tablas de palabras de 32 bits en cabeceras de archivos para calcular direcciones de sub-recursos (`MPACK_AT`, `PACK_AT`, `FILE_TABLE`, etc.).
4. **Relocalización y Parcheo In-Situ (In-Place Relocation - R/W)**: Subrutinas donde se leen offsets de 32 bits del archivo y se **sobreescriben inmediatamente en el propio buffer** con punteros absolutos de 32 bits de memoria, o donde se alteran paletas CLUT o tags DMA en el buffer de datos.
5. **Bitfields y Uniones en Estructuras de Disco y Registros**:
   - **Bitfields**: En GCC MIPS / x86, los bitfields se empaquetan desde el bit menos significativo (LSB). En GCC PowerPC (big-endian), se empaquetan desde el bit más significativo (MSB). Cualquier bitfield sobre datos binarios de disco queda con el orden de bits invertido.
   - **Uniones de flags con bitfields**: Patrón recurrente donde una unión contiene `u32 flags` y un `struct { u32 bit0 : 1; u32 bit1 : 1; }`. En big-endian, `flags & 1` no coincide con el primer campo de bits.
6. **Partidas Guardadas y Memory Card (Persistencia Bidireccional)**: Archivos de savegame (`SaveData`), replays (`McFlowReplayFile`) e `icon.sys`. No son solo de lectura: el motor los genera, computa checksums palabra por palabra y los graba a disco.

---

## 3. Inventario Exhaustivo de Hallazgos por Subsistema

### 3.1. Descompresión y Primitivas de Lectura de Memoria

| Archivo | Línea | Patrón / Involucrado | Motivo | Tipo de Acceso |
| :--- | :--- | :--- | :--- | :--- |
| [src/sys/bpe.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/bpe.c) | 21-24 | `*(s32 *)src` | Lee el tamaño descomprimido (`size`) y comprimido (`packSize`) de 32 bits little-endian de la cabecera BPE en el buffer de disco. | **Solo lectura** (en `src`) |
| [src/sys/mem_read.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/mem_read.c) | 11-48 | `Mem_LoadWord`, `Mem_LoadFloat`, `Mem_LoadHalf`, `Mem_ReadS32`, `Mem_ReadU32`, `Mem_ReadF32`, `Mem_ReadU16`, `Mem_ReadS16` | Funciones auxiliares de carga tipada. En PS2 realizaban cargas directas alineadas sin swap (`*(const s32 *)p`, `*(const u16 *)p`). En PPC requieren swap explícito. | **Solo lectura** |
| [src/battle/view_b_c.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/view_b_c.c) | 291-296 | `Mem_ReadS32(data + 8)`, `Mem_ReadU16(data - LIPSYNC_KEY_SIZE)` | Parseo de cabecera de datos binarios de LipSync (sincronización labial cargada de disco). | **Solo lectura** |
| [src/battle/view_b_c.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/view_b_c.c) | 323-336 | `Mem_ReadU16(gLipSync.keys + offset)` | Lectura de marcas de tiempo y estado de boca de 16 bits en el flujo binario de LipSync. | **Solo lectura** |

---

### 3.2. Indexación y Tablas de Offset de Archivos / Paquetes (Universal Packs)

El juego utiliza de forma ubicua un contenedor de paquetes donde la cabecera es un array de enteros de 32 bits little-endian con los offsets de cada recurso.

| Archivo | Línea | Patrón / Involucrado | Motivo | Tipo de Acceso |
| :--- | :--- | :--- | :--- | :--- |
| [include/menu/menu_a.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/menu/menu_a.h) | 55 | `#define MPACK_AT(pack, n)` | `((u8 *)(pack) + ((((u32 *)(pack))[n] >> 2) << 2))`: Cast de pack a `u32 *`, lectura de offset de sección de 32 bits. Utilizado en todos los menús. | **Solo lectura** |
| [include/battle/view_a.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/battle/view_a.h) | 60 | `#define PACK_AT(pack, n)` | `((void *)((u8 *)(pack) + ((((u32 *)(pack))[n] >> 2) << 2)))`: Cast de pack a `u32 *`, lectura de offset de 32 bits. Utilizado en batalla y HUD. | **Solo lectura** |
| [src/menu/menu_c_e.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/menu/menu_c_e.c) | 367 | `#define MPACK_WORD(pack, n)` | `(((s32 *)(pack))[((u32 *)(pack))[n] >> 2])`: Doble dereferencia de 32 bits; lee el offset de la sección y luego la primera palabra de 32 bits de esa sección. | **Solo lectura** |
| [src/menu/menu_e_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/menu/menu_e_b.c) | 55 | `#define TS_PACK_WORD(pack, n)` | `(*(s32 *)(((((u32 *)(pack))[n] >> 2) << 2) + (u32)(pack)))`: Dereferencia directa como `s32 *` de la primera palabra de una sección del paquete. | **Solo lectura** |
| [src/battle/btl_char_mgr.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/btl_char_mgr.c) | 240, 252, 1292-1297 | `#define FILE_TABLE(file, word)` | `((u32 *)(file) + ((u32 *)(file))[word] / 4)`: Indexa cabecera como tabla de palabras de 32 bits para ubicar tablas de personajes y parámetros comunes. | **Solo lectura** |
| [src/battle/bobj_a.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/bobj_a.c) | 2272-2277 | `BObjFile_GetEntry(BObjFile *file, s32 n)` | `u32 *buf = file->buf; if (buf[n + 1] != buf[n]) return (u8 *)buf + (buf[n] >> 2 << 2);`: Indexación de tabla de entradas de 32 bits. | **Solo lectura** |
| [src/battle/btl_obj.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/btl_obj.c) | 209 | `((u32 *)res->data[0])[1]` | Indexación de `res->data[0]` (recurso común de disco) como `u32 *` para obtener offset de luces. | **Solo lectura** |
| [src/battle/hud_a.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/hud_a.c) | 275-284 | `(HudRes *)((u8 *)file + ((file[n] >> 2) << 2))` | Indexación de `file[1..5]` como array de palabras de 32 bits para extraer sub-archivos de texturas del HUD. | **Solo lectura** (el offset) |
| [src/sys/dialog.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/dialog.c) | 54-55 | `DIALOG_TEXT`, `DIALOG_PART` | `((u16 *)((u8 *)(tbl) + ((tbl)[(i) + 1] / 4 * 4)))`: Indexación de offsets de 32 bits `tbl` y lectura de textos con caracteres `u16 *`. | **Solo lectura** |
| [src/sys/pad_watch.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/pad_watch.c) | 181 | `boot[9 + message]` | `u32 *boot = gCommonRes->boot`: Lee offset de 32 bits de mensajes de aviso del sistema desde el archivo de arranque de PS2. | **Solo lectura** |

---

### 3.3. Sistema de Texturas, DMA Chains y Relocalización In-Situ (R/W Crítico)

Este subsistema es uno de los más críticos del motor: los archivos de textura se cargan directamente en RAM y se parchean **in-situ**, reemplazando offsets por punteros de 32 bits y escribiendo tags DMA de hardware dentro del propio buffer.

| Archivo | Línea | Patrón / Involucrado | Motivo | Tipo de Acceso |
| :--- | :--- | :--- | :--- | :--- |
| [src/sys/gfxm_b_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_b_b.c) | 244-259 | `Res_RelocateOffsets(TexFile **out, u8 *base, TexFile *hdr)` | Cast directo de buffer a `TexFile *` y `TexEntry *`. Lee `hdr->entOfs`, `(*out)->count`, `e->pixOfs`, `e->clutOfs` (32 bits LE). **Sobreescribe in-situ**: `hdr->ent = (TexEntry *)(...); e->pix = ...; e->clut = ...;`. | **Lectura y Escritura (R/W)** |
| [include/sys/gfxm_b_b.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/sys/gfxm_b_b.h) | 28-53 | Structs `TexFile`, `TexEntry` | `TexFile` contiene `u32 count`, `u32 entOfs`, `TexEntry *ent`. `TexEntry` (0x40 bytes) contiene 10 campos de 32 bits (`pixOfs`, `clutOfs`, `pixSize`, etc.), un registro GS de 64 bits (`u64 tex0`), y dos punteros (`void *pix`, `void *clut`). Todos residen en el buffer de disco. | **Lectura y Escritura (R/W)** |
| [src/sys/gfxm_b_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_b_b.c) | 51-93 | `TexChain_Build(u8 *file)` | Cast de buffer a `TexChunkHdr *` (`next`, `size`, `tag`). Calcula offsets y escribe listas y tags DMA (`chain.tags = (u32 *)(file + ofs)`) directamente en el espacio no alineado al final del archivo en memoria. | **Lectura y Escritura (R/W)** |
| [src/sys/gfxm_b_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_b_b.c) | 119-170 | `TexChain_BuildPair(u8 *file, s32 index)` | Idéntico recorrido de chunks; genera tags de DMA in-situ en el buffer de textura para pares de texturas/CLUT. | **Lectura y Escritura (R/W)** |
| [src/sys/gfxm_b_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_b_b.c) | 184-190 | `TexChain_AddCbp(TexChain chain, s32 add)` | `chain.list[i].data[9] = add + chain.list[i].data[9];`: Modifica la palabra 9 (puntero de bloque GS de 32 bits) del paquete GS embebido en el buffer del archivo en memoria. | **Lectura y Escritura (R/W)** |

---

### 3.4. Modelos 3D, Shaders VIF, Costuras (Seams) y Paletas CLUT (R/W y Bitfield Crítico)

| Archivo | Línea | Patrón / Involucrado | Motivo | Tipo de Acceso |
| :--- | :--- | :--- | :--- | :--- |
| [include/sys/gfxm_e.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/sys/gfxm_e.h) | 32-33 | `ObjMdlFile`: `u32 flags : 31; u32 bound : 1;` | **Bitfield en cabecera de archivo de disco**. En PowerPC el bit `bound` queda en el bit 0 (MSB) en lugar del bit 31 (LSB). Provoca desalineación del flag. | **Lectura y Escritura (R/W)** |
| [src/sys/gfxm_e_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_e_b.c) | 173, 190 | `mdl->file->bound = 1; if (!mdl->file->bound)` | Comprueba y marca in-situ el bitfield `bound` en el propio buffer del archivo de modelo en memoria. | **Lectura y Escritura (R/W)** |
| [src/sys/gfxm_e_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_e_b.c) | 155-157 | `p[0x60 + j * 4 + 3] = slot + row;` | `BtlObjMdl_InitAlphaSlots`: Modifica directamente las entradas de la paleta CLUT dentro del buffer de textura cargado. | **Lectura y Escritura (R/W)** |
| [src/sys/gfxm_e_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_e_b.c) | 174-179 | `mdl->file->tbp = 0x3480; mdl->model->cbp = 0x3C00; ... mdl->model->alphaRow = ...` | Sobreescribe campos de 32 bits en la cabecera `ObjMdlFile` del buffer del modelo. | **Lectura y Escritura (R/W)** |
| [src/sys/gfxm_e_c.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_e_c.c) | 25-93 | `MdlTex_RebaseChain(...)` | Escanea palabras de 32 bits buscando el tag VIF `MDLTEX_VIF_TEX` (`0x6C058000`). Modifica in-situ palabras 5, 6, 9 y 10 del flujo VIF del modelo (`p[MDLTEX_TEX0A_LO]`, etc.). | **Lectura y Escritura (R/W)** |
| [include/sys/gfxm_e.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/sys/gfxm_e.h) | 51-60 | `ObjMdlMesh` | Cast directo a `ObjMdlMesh *`. Lee `s32 next`, `u16 unk04`, `u16 last`, `u16 enabled`, `u16 node`, y array de palabras VIF `u32 vif[1]`. | **Solo lectura** |
| [src/sys/gfxm_d_c.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_d_c.c) | 75-82, 120-137 | `sec = (ObjSeamSec *)((u8 *)model + model->seamOfs);` | Cast a `ObjSeamSec *`, `ObjSeamVtx *`, `ObjSeamTri *`. Lee `vtxCount`, `triCount`, `vtxOfs`, `triOfs` (s32), y de `ObjSeamTri` lee enteros de 64 bits `u64 tex0Base`, `u64 tex0Shade`. | **Solo lectura** (en disco) |
| [include/sys/gfxm_d_c.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/sys/gfxm_d_c.h) | 25-28 | `ObjSeamVtx`: `union { u128 q; s32 w[4]; } xyz;` | Unión de vértice de costura entre vector quadword `u128` y array de 4 enteros `s32 w[4]`. `ObjSeam_TransformVtx` escribe en `w[4]`. | **Lectura y Escritura** (memoria local de vértice) |

---

### 3.5. Relocalización de Escenarios (Stage Relocation / Octree) (R/W Crítico)

| Archivo | Línea | Patrón / Involucrado | Motivo | Tipo de Acceso |
| :--- | :--- | :--- | :--- | :--- |
| [src/sys/gfxm_e_d.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_e_d.c) | 14 | `#define STG_RELOC(type, field) ((field) = (type)(base + (s32)(field)))` | Macro fundamental de relocalización de escenarios. Lee un offset relativo de 32 bits little-endian y **lo sobreescribe in-situ** con el puntero absoluto en memoria. | **Lectura y Escritura (R/W)** |
| [src/sys/gfxm_e_d.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_e_d.c) | 17-45 | `StgOctree_Relocate(StgOctNode *node, s32 *base)` | Relocaliza recursivamente todos los nodos y punteros a listas y 8 hijos del octree de colisión/visibilidad del escenario. | **Lectura y Escritura (R/W)** |
| [src/sys/gfxm_e_d.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_e_d.c) | 49-146 | `BtlStage_Relocate(s32 *base)` | Relocaliza la cabecera `StgRelocHdr *gBtlStage` y todas sus tablas de sub-estructuras (`timers`, `a`, `b`, `c`, `d`, `e`, `tree`). | **Lectura y Escritura (R/W)** |

---

### 3.6. Motor de Animaciones (BObjAnim, Keyframes, Retargeting y Root Track)

| Archivo | Línea | Patrón / Involucrado | Motivo | Tipo de Acceso |
| :--- | :--- | :--- | :--- | :--- |
| [include/battle/bobj_a.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/battle/bobj_a.h) | 58-96 | `BobjAnim`, `BobjTrack`, `BobjKey`, `BobjAnimEvent` | Structs superpuestas a los archivos de animación. `BobjAnim` contiene `u16 length`, array de 16 bits `u16 track[BOBJ_NODE_MAX]`. `BobjTrack` contiene `u16 flags`, `u16 count`, y la unión `union { BobjPackedQuat rot[1]; BobjKey key[1]; } u;`. | **Solo lectura** (estructura general) |
| [src/battle/bobj_a.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/bobj_a.c) | 134-160 | `BtlObjAnim_Retarget(...)` | Retargeting de animaciones entre personajes: accede a `trk->u.key` y **modifica in-situ las coordenadas de los keyframes en el propio buffer**: `keys[i].x -= ...; keys[i].y *= scale; keys[i].x += ...;`. | **Lectura y Escritura (R/W)** |
| [src/battle/bobj_a.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/bobj_a.c) | 636-663 | `BtlObjAnim_FindHitBeginEnd(...)` | Lee eventos de animación: `attrLo`, `attrHi` (u32), `frame`, `shape` (u16). Castea offset `shape` a `BobjAnimShape *` y lee floats `s->x`, `s->y`, `s->z`, `s->r0`. | **Solo lectura** |
| [src/battle/btl_act_h_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/btl_act_h_b.c) | 269-280 | `track = (BtlActHTrack *)((u32 *)anim + anim->rootTrack)` | Parseo del track raíz de la animación. Lee `count` (u16), `flags` (u16), y empaqueta palabras de 32 bits `key[0], key[1]` o `key[4], key[5]` en un entero de 64 bits para descomprimir el cuaternión. | **Solo lectura** |
| [src/battle/btl_act_f.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/btl_act_f.c) | 3298-3309 | `track = (ActGTrack *)((u32 *)anim + anim->rootTrack)` | Idéntica rutina de parseo de cuaterniones de `rootTrack` que en `btl_act_h_b.c`. | **Solo lectura** |

---

### 3.7. Motor de Scripts (GSC Script Engine)

| Archivo | Línea | Patrón / Involucrado | Motivo | Tipo de Acceso |
| :--- | :--- | :--- | :--- | :--- |
| [include/sys/gsc.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/sys/gsc.h) | 34-45 | `GscChunk` y macro `GSC_SWAP` | Cabecera de chunk GSC: `u32 tag`, `u32 headSize`, `u32 dataSize`, `s32 arg`. En PS2 la macro `GSC_SWAP` intercambiaba manualmente los bytes de `tag` para comparar con `0x47534143` ("GSAC"). En PPC Xenon (big-endian), `GSC_SWAP` debe reevaluarse para no romper la comparación de tags de 4 caracteres. | **Solo lectura** |
| [src/sys/gsc.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gsc.c) | 132, 191 | `Gsc_FindHandler(((u16 *)task->pc)[1])` | Lectura de ID de comando de script como entero de 16 bits little-endian desde el stream de código. | **Solo lectura** |
| [src/sys/gsc.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gsc.c) | 165, 172 | `((s16 *)cmd)[1]` | Lectura de ID de acción de script como entero de 16 bits con signo little-endian. | **Solo lectura** |
| [src/sys/gsc.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gsc.c) | 560-635 | `*(u32 *)word & 0xF`, `*(u32 *)word & GSC_WORD_ARG` | Búsqueda de opciones y argumentos: lee palabras de 32 bits little-endian en el flujo de bytecode. | **Solo lectura** |
| [src/sys/gsc.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gsc.c) | 674-682 | `GscTask_PeekOperand`, `GscTask_NextOperand*` | Lectura de enteros de 32 bits y flotantes `*(f32 *)` de la sección GSDT referenciada por los operandos del script. | **Solo lectura** |

---

### 3.8. Subsistema de Audio (Bancos de Sonido y VAG)

| Archivo | Línea | Patrón / Involucrado | Motivo | Tipo de Acceso |
| :--- | :--- | :--- | :--- | :--- |
| [include/sys/snd.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/sys/snd.h) | 92-96 | `union SndRpcBuf` | Reinterpretación del buffer compartido de RPC entre `s32 w[0x62]`, `s16 h[0xC4]` y `SndQueue queue`. | **Lectura y Escritura** (memoria local) |
| [include/sys/snd.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/sys/snd.h) | 99-105 | `SndBankFile` | Cabecera del archivo de banco de sonido en disco: 5 palabras de 32 bits (`unk00`, `bodyOfs`, `hdOfs`, `seqOfs`, `endOfs`). | **Solo lectura** |
| [src/sys/snd.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/snd.c) | 359-367 | `Snd_SetupBank(SndBank *bank, u32 mask, SndBankFile *file, s32 alloc)` | Lee offsets de 32 bits `file->hdOfs`, `file->bodyOfs`, `file->seqOfs`, `file->endOfs` para calcular tamaños de secciones y punteros. | **Solo lectura** |
| [src/sys/snd.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/snd.c) | 754-770 | `SndHd_GetVagInfo`, `SndHd_GetLoopFlags` | Parseo de cabecera Sony HD: lee `head[0xC] / 4`, castea a `SndVagInfoChunk *`, lee `chunk->maxNumber` (u32) e itera sobre `SndVagParam` leyendo `sampleRate` (u16) y `loop` (u8). | **Solo lectura** |
| [port/src/gs/snd_adx.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/port/src/gs/snd_adx.c) | 79-85 | Cabecera CRI ADX | Los archivos ADX son **big-endian por especificación de CRI** (`(uint32_t)h[0] << 8 | h[1]`). En PC se reconstruían byte a byte. En PPC Xenon coinciden con el orden nativo. | **Solo lectura** |

---

### 3.9. Partidas Guardadas y Memory Card (Saves, Replays, icon.sys y Checksum) (R/W Bidireccional)

Este subsistema interactúa con almacenamiento persistente. Los datos no solo se leen, sino que se escriben al dispositivo de almacenamiento de Xbox 360 y deben mantener integridad estricta con el cálculo de sumas de verificación.

| Archivo | Línea | Patrón / Involucrado | Motivo | Tipo de Acceso |
| :--- | :--- | :--- | :--- | :--- |
| [src/battle/view_b_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/view_b_b.c) | 16-100 | `CHRTBL_FILE`, `ChrTblEntry`, `ItemTblEntry` | Cast directo de `gCommonRes->data[2]` a `ChrTblFile *`, `ChrTblEntry *`, `ItemTblEntry *`. Lee múltiples campos multibyte: `cost`, `costumes`, `baseLevel`, `flags` (u16), `stat[]` (s16), `exp[]`, `ability[]`, `flags` (s32), `targetY`, `dist` (f32). | **Solo lectura** |
| [src/sys/save.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/save.c) | 62 | `info = (ItemInfo *)((u8 *)gCommonRes->data[2] + ((ItemFile *)gCommonRes->data[2])->itemOffset / 4 * 4)` | Cast directo de cabecera y sección de ítems en el recurso común. | **Solo lectura** |
| [include/sys/save.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/sys/save.h) | 70-114 | Struct `SaveData` (0x4000 bytes) | Estructura completa de la partida guardada. Contiene enteros de 32 bits (`unlockFlags`, `money`, `screenX`, etc.), campos de 64 bits (`charaBits`, `stageBits`), arrays de `SaveSlot` (`u16 flags, val[3]`), y arrays de `SaveCustom` y `SaveRec` (`s16 level`, `u16 item[8]`). | **Lectura y Escritura (R/W)** |
| [include/sys/mcflow_a.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/sys/mcflow_a.h) | 44-48, 88-93 | Structs `McFlowHeader`, `McFlowReplayFile` (0x1AC00 bytes) | Archivo de repetición de batalla guardado en tarjeta de memoria: `u32 sum[2]`, `u32 hdrSum[2]`, `s32 chara[2][5]`, `u32 count`. | **Lectura y Escritura (R/W)** |
| [include/sys/mcflow_a.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/sys/mcflow_a.h) | 56-59 | `McFlowReq`: `union { char text[0x100]; u64 align; } title;` | Unión de alineación para copia de títulos Shift-JIS de `icon.sys` mediante palabras de 64 bits. En PPC el orden de bytes interno en cada palabra de 64 bits debe preservarse al copiar cadenas. | **Lectura y Escritura (R/W)** |
| [src/battle/stgm_a_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/stgm_a_b.c) | 934-965 | `McCard_Checksum(u32 *buf, s32 size, s32 write)` | **Cálculo de suma de verificación de Memory Card**: itera byte a byte (`size - 8`). Si `write == 1`, escribe `buf[0] = sum >> 8; buf[1] = sum & 0xFF;`. Si lee, compara `buf[0]` y `buf[1]`. `buf[0]` y `buf[1]` son palabras de 32 bits grabadas en disco. | **Lectura y Escritura (R/W)** |
| [src/battle/stgm_a_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/stgm_a_b.c) | 262-274 | Generación y escritura de `McCardIconSys` (`icon.sys`) | Rellena la estructura `gMcCardIconSys` y la escribe con `sceMcWrite`. | **Escritura** |
| [include/battle/stgm_a_b.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/battle/stgm_a_b.h) | 64-79 | Struct `McCardIconSys` (0x3C4 bytes) | Cabecera del icono de PS2 grabada en disco: contiene `u16 titleBreak`, `s32 transRate`, `s32 bgColor[4][4]`, `f32 lightDir[3][4]`, `f32 lightColor[3][4]`, `f32 ambient[4]`. | **Escritura** |

---

### 3.10. Flash / FOD Engine (Películas de Interfaz y Fuentes)

| Archivo | Línea | Patrón / Involucrado | Motivo | Tipo de Acceso |
| :--- | :--- | :--- | :--- | :--- |
| [include/sys/gfxm_b_c.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/sys/gfxm_b_c.h) | 13-18 | Struct `FlashTag`: `u8 code; u8 unk01; u16 count; u32 size;` | Cabecera de tag de archivo FOD (SWF convertido). En el archivo puede estar desalineado a nivel de byte. | **Solo lectura** |
| [src/sys/gfxm_b_c.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_b_c.c) | 39-45 | `memcpy(&tag, data + ofs, sizeof(tag));` | Copia de cabecera de tag desalineada. Al copiarse en memoria en PPC Xenon, los campos `tag.count` (16 bits) y `tag.size` (32 bits) quedan invertidos en bytes. | **Solo lectura** |
| [src/sys/gfxm_c.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_c.c) | 137-138 | `memcpy(&id, data + *ofs, 2);` | Lectura de ID de objeto de 16 bits sin swap. En PPC el valor queda invertido. | **Solo lectura** |
| [src/sys/gfxm_c.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_c.c) | 170-176 | `memcpy(&depth, data + *ofs, 2); memcpy(&id, data + *ofs, 2);` | Lectura de profundidad e ID de 16 bits sin swap para tags SWF PlaceObject2. | **Solo lectura** |
| [src/battle/view_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/view_b.c) | 243 | `text + (((u32 *)text)[n + 1] & ~3)` | Parseo de archivos de texto localizados: tabla de offsets de 32 bits seguida de strings UTF-16 / Shift-JIS de caracteres `u16 *str`. | **Solo lectura** |

---

### 3.11. Uniones y Bitfields en Estructuras y Registros de Hardware

| Archivo | Línea | Involucrado | Motivo / Riesgo en PowerPC Xenon | Tipo de Acceso |
| :--- | :--- | :--- | :--- | :--- |
| [include/sys/gfxm_e.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/sys/gfxm_e.h) | 32-33 | `ObjMdlFile`: `u32 flags : 31; u32 bound : 1;` | Bitfield directo sobre cabecera de archivo en disco. En PPC Xenon, `bound` se ubica en el bit MSB en lugar de LSB. Rompe la lectura y escritura de `mdl->file->bound`. | **Lectura y Escritura (R/W)** |
| [include/battle/hud_a.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/battle/hud_a.h) | 92-101 | `Hud`: `union { u32 flags; struct { u32 showGauges : 1; ... }; };` | Unión anónima de palabra completa con bitfield. `flags` se evalúa con máscaras `HUD_SHOW_*` (`1 << 0`), pero en PPC el bitfield empieza en el bit 31 (`1 << 31`). | **Lectura y Escritura** (memoria local) |
| [include/battle/hud_c.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/battle/hud_c.h) | 17-23 | `HudCSprite`: `union { u32 flags; struct { u32 hidden : 1; u32 mirror : 1; }; };` | Idéntica incompatibilidad: las funciones leen `flags & HUDC_SPR_HIDDEN` mientras que los setters escriben `hidden = ...`. En PPC las máscaras no coinciden con los bits. | **Lectura y Escritura** (memoria local) |
| [include/battle/hud_e.h](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/include/battle/hud_e.h) | 30-36 | `HudENode`: `union { u32 flags; struct { u32 hidden : 1; u32 mirror : 1; }; };` | Idéntica incompatibilidad entre máscara de `u32 flags` y bitfield. | **Lectura y Escritura** (memoria local) |
| [src/sys/gfxm_a.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/sys/gfxm_a.c) | 68-83 | `GfxPostSpritePkt`: `struct { u64 x:16, y:16, z:24, f:8; } xyz0;` | Bitfield de 64 bits para paquete GS de post-procesado. En PPC Xenon los campos se invierten dentro del entero de 64 bits. | **Escritura** (DMA/GPU) |
| [src/battle/eft_ad_b.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/eft_ad_b.c) | 43-48 | `EftAdXyzf`: `u64 x:16, y:16, z:24, f:8;` | Registro de vértices GS empaquetado en bitfield de 64 bits. | **Escritura** (DMA/GPU) |
| [src/battle/eft_d.c](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/src/battle/eft_d.c) | 83-94 | `EftPrim`: `u64 prim:3, iip:1, tme:1, ... pad:53;` | Registro GS PRIM mapeado mediante bitfield de 64 bits. | **Escritura** (DMA/GPU) |

---

## 4. Estrategia y Primitivas Acordadas de Conversión para Xbox 360

Para cumplir estrictamente la Regla 2 de `AGENTS.md`, se establece la siguiente arquitectura de conversión:

### 4.1. Primitivas de Intercambio de Bytes (Hardware PowerPC)

En PowerPC Xenon existen instrucciones específicas de hardware para lectura y escritura con swap de bytes en un solo ciclo: `lhbrx` (16 bits) y `lwbrx` (32 bits), así como las funciones intrínsecas del compilador.

```c
#ifdef XBOX360
    #define SWAP16(x) __builtin_bswap16((u16)(x))
    #define SWAP32(x) __builtin_bswap32((u32)(x))
    #define SWAP64(x) __builtin_bswap64((u64)(x))
#else
    #define SWAP16(x) ((((u16)(x) & 0xFF) << 8) | (((u16)(x) >> 8) & 0xFF))
    #define SWAP32(x) ((((u32)(x) & 0xFF) << 24) | (((u32)(x) & 0xFF00) << 8) | \
                       (((u32)(x) >> 8) & 0xFF00) | (((u32)(x) >> 24) & 0xFF))
    #define SWAP64(x) ...
#endif
```

### 4.2. Lectura y Escritura Segura Little-Endian (Alineada y Desalineada)

Para evitar problemas de alineación estricta y casts directos prohibidos:

```c
static inline u16 ReadLE16(const void *p) {
    const u8 *b = (const u8 *)p;
    return (u16)(b[0] | (b[1] << 8));
}

static inline u32 ReadLE32(const void *p) {
    const u8 *b = (const u8 *)p;
    return (u32)(b[0] | (b[1] << 8) | (b[2] << 16) | (b[3] << 24));
}

static inline f32 ReadLEF32(const void *p) {
    union { u32 u; f32 f; } cvt;
    cvt.u = ReadLE32(p);
    return cvt.f;
}

static inline u64 ReadLE64(const void *p) {
    const u8 *b = (const u8 *)p;
    return ((u64)ReadLE32(b)) | (((u64)ReadLE32(b + 4)) << 32);
}

static inline void WriteLE16(void *p, u16 v) {
    u8 *b = (u8 *)p;
    b[0] = (u8)(v & 0xFF);
    b[1] = (u8)((v >> 8) & 0xFF);
}

static inline void WriteLE32(void *p, u32 v) {
    u8 *b = (u8 *)p;
    b[0] = (u8)(v & 0xFF);
    b[1] = (u8)((v >> 8) & 0xFF);
    b[2] = (u8)((v >> 16) & 0xFF);
    b[3] = (u8)((v >> 24) & 0xFF);
}
```

### 4.3. Estrategia por Categoría de Acceso

1. **Categoría 1: Lecturas al Vuelo (Read-on-the-fly)**
   - Aplicable a: Universal Packs (`MPACK_AT`, `PACK_AT`), `Mem_Read*`, scripts `GSC`, `FlashTag`.
   - Implementación: Reemplazar las macros de lectura directa por llamadas a `ReadLE32(pack + n * 4)` o `SWAP32(((u32 *)pack)[n])`.
2. **Categoría 2: Archivos Relocalizados In-Situ (In-Place Relocation Pass)**
   - Aplicable a: `TexFile` / `TexEntry`, `ObjMdlFile`, `StgRelocHdr` / Octree, animaciones `BobjAnim`.
   - Implementación: Introducir un paso preliminar de byteswap al terminar la carga del archivo desde disco (`Tex_ByteSwapFile`, `Mdl_ByteSwapFile`, `Stage_ByteSwapFile`). Una vez convertidos todos los campos a big-endian nativo en memoria, las rutinas de relocalización (`Res_RelocateOffsets`, `BtlStage_Relocate`, etc.) pueden operar de forma normal, escribiendo punteros nativos de PPC sin riesgo de mezclar endianness.
3. **Categoría 3: Serialización / Deserialización de Partidas Guardadas**
   - Aplicable a: `SaveData` y `McFlowReplayFile`.
   - Implementación: No almacenar structs en memoria nativa con datos little-endian directos. Definir funciones explícitas:
     - `Save_Deserialize(SaveData *dst, const u8 *srcRaw)`: Lee el buffer de 0x4000 bytes con `ReadLE*` y puebla la struct en formato nativo.
     - `Save_Serialize(u8 *dstRaw, const SaveData *src)`: Vuelca la struct a un buffer binario little-endian con `WriteLE*` y calcula el checksum final mediante `McCard_Checksum`.
4. **Categoría 4: Refactorización de Bitfields y Uniones**
   - Aplicable a: `ObjMdlFile.bound`, `Hud`, `HudCSprite`, `HudENode`.
   - Implementación:
     - En `ObjMdlFile`: Reemplazar el bitfield por un entero explícito `u32 flags` y utilizar máscaras (`flags & 0x80000000` o la correspondiente para `bound`).
     - En `Hud`, `HudCSprite`, `HudENode`: Eliminar la unión anónima y sustituir los accesos a campos de bits por setters/getters que manipulen directamente la máscara de bits en `u32 flags`.
