/*
 * port/include/xbox360/xbr_endian.h
 *
 * Helpers de endianness para el port a Xbox 360 (PowerPC Xenon, big-endian,
 * 32 bits). Los datos en disco del juego (PS2) son little-endian; aqui se
 * convierten a tipos nativos del port (que en PPC son big-endian, pero el
 * tipo abstracto es siempre little-endian en disco).
 *
 * Politica (regla 2 de AGENTS.md + docs/port/endian.md): prohibido hacer
 * cast directo de buffers a structs con campos multibyte; todo pasa por
 * estas primitivas. Las firmas son SIMETRICAS a las de
 * `src/sys/mem_read.c` (Mem_LoadWord, Mem_ReadS32, etc.) para que el
 * port se pueda ir reescribiendo de forma 1-a-1.
 *
 * Esta cabecera es la fuente de verdad para las conversiones; los tests
 * en `port/tests/endian/` (Python, sesion 0) y `xbr_endian.c` (C, sesion
 * 14, subtarea 2.2) verifican el mismo contrato desde dos lados.
 */
#ifndef XBOX360_XBR_ENDIAN_H
#define XBOX360_XBR_ENDIAN_H

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Primitivas de carga tipada desde buffer LE (ReadLE* en docs/port/endian.md)
 * Equivalentes a Mem_LoadWord / Mem_ReadS32 / Mem_ReadF32 del decomp del juego.
 * -------------------------------------------------------------------------- */

static __inline uint16_t Xbr_LoadLE16(const void *buf) {
    const uint8_t *p = (const uint8_t *)buf;
    return (uint16_t)(p[0] | ((uint16_t)p[1] << 8));
}

static __inline int16_t Xbr_LoadLES16(const void *buf) {
    return (int16_t)Xbr_LoadLE16(buf);
}

static __inline uint32_t Xbr_LoadLE32(const void *buf) {
    const uint8_t *p = (const uint8_t *)buf;
    return (uint32_t)p[0]
         | ((uint32_t)p[1] << 8)
         | ((uint32_t)p[2] << 16)
         | ((uint32_t)p[3] << 24);
}

static __inline int32_t Xbr_LoadLES32(const void *buf) {
    return (int32_t)Xbr_LoadLE32(buf);
}

static __inline uint64_t Xbr_LoadLE64(const void *buf) {
    return (uint64_t)Xbr_LoadLE32(buf)
         | ((uint64_t)Xbr_LoadLE32((const uint8_t *)buf + 4) << 32);
}

static __inline int64_t Xbr_LoadLES64(const void *buf) {
    return (int64_t)Xbr_LoadLE64(buf);
}

/* Float IEEE 754 de 32 bits. La conversion pasa por u32 para no depender
 * del orden natural del host (PPC BE, x86 LE, ARM configurable): los
 * bits se re-interpretan tal cual, asi "load little-endian" + "reinterpret
 * como float" es independiente del endianness del CPU. */
static __inline float Xbr_LoadLEF32(const void *buf) {
    union { uint32_t u; float f; } cvt;
    cvt.u = Xbr_LoadLE32(buf);
    return cvt.f;
}

/* Si el caller quiere los 32 bits exactos sin reinterpretar (ej. para
 * compararlos con un valor raw en una cabecera), usa este. */
static __inline uint32_t Xbr_LoadLE32Raw(const void *buf) {
    return Xbr_LoadLE32(buf);
}

/* --------------------------------------------------------------------------
 * Primitivas de escritura tipada en buffer LE (WriteLE* en docs/port/endian.md)
 * -------------------------------------------------------------------------- */

static __inline void Xbr_StoreLE16(void *buf, uint16_t v) {
    uint8_t *p = (uint8_t *)buf;
    p[0] = (uint8_t)(v & 0xFFu);
    p[1] = (uint8_t)((v >> 8) & 0xFFu);
}

static __inline void Xbr_StoreLE32(void *buf, uint32_t v) {
    uint8_t *p = (uint8_t *)buf;
    p[0] = (uint8_t)(v & 0xFFu);
    p[1] = (uint8_t)((v >> 8) & 0xFFu);
    p[2] = (uint8_t)((v >> 16) & 0xFFu);
    p[3] = (uint8_t)((v >> 24) & 0xFFu);
}

static __inline void Xbr_StoreLE64(void *buf, uint64_t v) {
    Xbr_StoreLE32(buf, (uint32_t)v);
    Xbr_StoreLE32((uint8_t *)buf + 4, (uint32_t)(v >> 32));
}

static __inline void Xbr_StoreLEF32(void *buf, float v) {
    union { uint32_t u; float f; } cvt;
    cvt.f = v;
    Xbr_StoreLE32(buf, cvt.u);
}

/* --------------------------------------------------------------------------
 * Swap de un valor (no de buffer): util para los in-situ relocations de
 * texturas, modelos y registros GS (categoria 2 de docs/port/endian.md).
 * Swap16/32/64 son inline; SwapBuffer* estan en xbr_endian.c.
 * -------------------------------------------------------------------------- */

static __inline uint16_t Xbr_Swap16(uint16_t v) {
    return (uint16_t)((v << 8) | (v >> 8));
}

static __inline uint32_t Xbr_Swap32(uint32_t v) {
    return ((v & 0xFF000000u) >> 24)
         | ((v & 0x00FF0000u) >> 8)
         | ((v & 0x0000FF00u) << 8)
         | ((v & 0x000000FFu) << 24);
}

static __inline uint64_t Xbr_Swap64(uint64_t v) {
    return ((uint64_t)Xbr_Swap32((uint32_t)v) << 32)
         | (uint64_t)Xbr_Swap32((uint32_t)(v >> 32));
}

/* Byteswap in-situ de N elementos. No inline porque el bucle con size_t
 * generaria mas codigo del deseable en cada punto de uso. */
void Xbr_SwapBuffer16(void *buf, size_t count);
void Xbr_SwapBuffer32(void *buf, size_t count);
void Xbr_SwapBuffer64(void *buf, size_t count);

/* Auto-test: corre en Xbr_Init; si las primitivas funcionan, loguea OK;
 * si alguna falla, loguea FAIL con el valor inesperado. Asi el log de
 * xenia confirma que la conversion endian es correcta. */
void Xbr_EndianSelfTest(void);

#ifdef __cplusplus
}
#endif

#endif
