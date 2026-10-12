/*
 * port/src/xbox360/xbr_endian.c
 *
 * Implementacion de las primitivas no-inline de endianness (buffers in-situ)
 * y auto-test que se ejecuta en Xbr_Init para confirmar en xenia que las
 * conversiones son correctas.
 */
#include "xbox360/xbr_endian.h"
#include "xbox360/xbr_platform.h"

void Xbr_SwapBuffer16(void *buf, size_t count) {
    uint8_t *p = (uint8_t *)buf;
    size_t i;
    for (i = 0; i < count; i++) {
        uint8_t t = p[0];
        p[0] = p[1];
        p[1] = t;
        p += 2;
    }
}

void Xbr_SwapBuffer32(void *buf, size_t count) {
    uint8_t *p = (uint8_t *)buf;
    size_t i;
    for (i = 0; i < count; i++) {
        uint8_t t0 = p[0], t1 = p[1];
        p[0] = p[3];
        p[1] = p[2];
        p[2] = t1;
        p[3] = t0;
        p += 4;
    }
}

void Xbr_SwapBuffer64(void *buf, size_t count) {
    uint8_t *p = (uint8_t *)buf;
    size_t i;
    for (i = 0; i < count; i++) {
        uint8_t t0 = p[0], t1 = p[1], t2 = p[2], t3 = p[3];
        p[0] = p[7];
        p[1] = p[6];
        p[2] = p[5];
        p[3] = p[4];
        p[4] = t3;
        p[5] = t2;
        p[6] = t1;
        p[7] = t0;
        p += 8;
    }
}

/* --------------------------------------------------------------------------
 * Auto-test
 * Verifica las primitivas con patrones representativos. El criterio
 * coincide con los tests Python de port/tests/endian/test_endian.py.
 * Si una sola conversion falla, el log mostrara el valor inesperado.
 * -------------------------------------------------------------------------- */
void Xbr_EndianSelfTest(void) {
    int ok = 1;

    /* LoadLE32 sobre 0x78 0x56 0x34 0x12 -> 0x12345678. */
    {
        const uint8_t le[4] = { 0x78, 0x56, 0x34, 0x12 };
        uint32_t v = Xbr_LoadLE32(le);
        if (v != 0x12345678u) {
            Xbr_Log("Xbr_EndianSelfTest: LoadLE32 FAIL (got 0x%08X, want 0x12345678)\n", v);
            ok = 0;
        }
    }

    /* LoadLES32 sobre 0x00 0x00 0x00 0x80 -> -2147483648. */
    {
        const uint8_t le[4] = { 0x00, 0x00, 0x00, 0x80 };
        int32_t v = Xbr_LoadLES32(le);
        if (v != -2147483648) {
            Xbr_Log("Xbr_EndianSelfTest: LoadLES32 FAIL (got %d, want -2147483648)\n", v);
            ok = 0;
        }
    }

    /* LoadLE64 sobre 8 bytes -> 0x0123456789ABCDEF. */
    {
        const uint8_t le[8] = { 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01 };
        uint64_t v = Xbr_LoadLE64(le);
        if (v != 0x0123456789ABCDEFull) {
            Xbr_Log("Xbr_EndianSelfTest: LoadLE64 FAIL (got 0x%016llX, want 0x0123456789ABCDEF)\n",
                    (unsigned long long)v);
            ok = 0;
        }
    }

    /* LoadLEF32 sobre bytes de -1.0 IEEE 754 (0xBF800000 LE = 0x00 0x00 0x80 0xBF) -> -1.0. */
    {
        const uint8_t le[4] = { 0x00, 0x00, 0x80, 0xBF };
        float v = Xbr_LoadLEF32(le);
        if (v != -1.0f) {
            Xbr_Log("Xbr_EndianSelfTest: LoadLEF32 FAIL (got %f, want -1.0)\n", (double)v);
            ok = 0;
        }
    }

    /* StoreLE32 sobre 0xDEADBEEF -> 0xEF 0xBE 0xAD 0xDE. */
    {
        uint8_t buf[4];
        Xbr_StoreLE32(buf, 0xDEADBEEFu);
        if (buf[0] != 0xEF || buf[1] != 0xBE || buf[2] != 0xAD || buf[3] != 0xDE) {
            Xbr_Log("Xbr_EndianSelfTest: StoreLE32 FAIL (got %02X %02X %02X %02X, want EF BE AD DE)\n",
                    buf[0], buf[1], buf[2], buf[3]);
            ok = 0;
        }
    }

    /* Swap32 sobre 0x12345678 -> 0x78563412. */
    {
        uint32_t v = Xbr_Swap32(0x12345678u);
        if (v != 0x78563412u) {
            Xbr_Log("Xbr_EndianSelfTest: Swap32 FAIL (got 0x%08X, want 0x78563412)\n", v);
            ok = 0;
        }
    }

    /* SwapBuffer32 sobre 2 palabras: [0x12345678, 0xDEADBEEF] LE -> BE. */
    {
        uint8_t buf[8] = { 0x78, 0x56, 0x34, 0x12, 0xEF, 0xBE, 0xAD, 0xDE };
        Xbr_SwapBuffer32(buf, 2);
        if (buf[0] != 0x12 || buf[1] != 0x34 || buf[2] != 0x56 || buf[3] != 0x78 ||
            buf[4] != 0xDE || buf[5] != 0xAD || buf[6] != 0xBE || buf[7] != 0xEF) {
            Xbr_Log("Xbr_EndianSelfTest: SwapBuffer32 FAIL\n");
            ok = 0;
        }
    }

    if (ok) {
        Xbr_Log("Xbr_Log: endian self-test OK\n");
    } else {
        Xbr_Log("Xbr_Log: endian self-test FAILED\n");
    }
}
