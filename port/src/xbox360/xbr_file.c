/*
 * port/src/xbox360/xbr_file.c
 *
 * Implementacion real de sceOpen / sceLseek / sceRead / sceClose para el
 * port a Xbox 360 (PowerPC Xenon, big-endian, 32 bits). En la subtarea 1.3
 * estos eran stubs no-op; la subtarea 2.1 los reemplaza por codigo real
 * que abre ficheros del sistema de archivos de Xbox 360 (game:\data\)
 * usando fopen / fread / fseek / fclose de la RTL del XDK.
 *
 * Politica de instalacion (regla 1 de AGENTS.md: el codigo especifico de
 * 360 vive en port/src/xbox360/):
 *   - Los paths PS2 del juego son "cdrom0:\\BIN\\DBZP.BIN;1" o parecidos.
 *   - Aqui se traducen a "game:\\data\\BIN\\DBZP.BIN" (strip "cdrom0:\\"
 *     por delante, strip ";<ver>" por detras, prepend "game:\\data\\").
 *   - En xenia-canary, el dispositivo "game:" se mapea a una carpeta
 *     del storage root (ver docs/port/xbox360-build.md §9).
 *   - En una consola RGH, los datos se depositan manualmente o via el
 *     installer de Aurora/FSD en game:\data\.
 *
 * Restricciones de 2.1:
 *   - Solo lectura (SCE_RDONLY). Escritura ira en fases 5/8 (saves,
 *     overlays, etc.).
 *   - Maximo 16 ficheros abiertos simultaneamente (PS2 usaba 16).
 *   - Los datos leidos son little-endian (formato PS2); la conversion
 *     endian se hara en la subtarea 2.2.
 */
#include "xbox360/xbr_platform.h"

#include <xtl.h>
#include <stdio.h>
#include <string.h>

/* PS2 whence y flags (los define src/main.c, los repetimos aqui para que
 * xbr_file.c no dependa de include/main.h). */
#define XBR_SCE_RDONLY    1
#define XBR_SCE_SEEK_SET  0
#define XBR_SCE_SEEK_CUR  1
#define XBR_SCE_SEEK_END  2

#define XBR_FD_MAX 16

/* Tabla de ficheros abiertos. El descriptor que ve el juego es el
 * indice + 1 (estilo PS2: 0 = invalido). */
static struct {
    int used;
    FILE *fp;
} sFds[XBR_FD_MAX];

/* --------------------------------------------------------------------------
 * Path translation
 * -------------------------------------------------------------------------- */

/* Traduce un path PS2 a un path Xbox 360. Reglas:
 *   - "cdrom0:\\PATH;N" o "cdrom0:/PATH;N" -> "game:\\data\\PATH"
 *   - Si el path no empieza por "cdrom0:", se devuelve tal cual (por si
 *     en futuras fases hay paths "host:" o similares).
 *   - El sufijo ";<num>" (version PS2) se ignora: fopen de xapilib no
 *     lo entiende y ademas los datos vienen sin el.
 * Devuelve 0 si ok, -1 si el resultado no cabe en out_size. */
static int xbr_translate_path(const char *ps2_path, char *out, size_t out_size) {
    const char *p = ps2_path;
    size_t i = 0;
    const char *prefix;
    size_t prefix_len;

    if (out_size == 0) {
        return -1;
    }

    /* Detectar prefijo "cdrom0:\" o "cdrom0:/". */
    if (strncmp(p, "cdrom0:\\", 8) == 0 || strncmp(p, "cdrom0:/", 8) == 0) {
        prefix = "game:\\data\\";
        prefix_len = 11;
        p += 8;
    } else {
        prefix = "";
        prefix_len = 0;
    }

    if (prefix_len + 1 > out_size) {
        return -1;
    }
    memcpy(out, prefix, prefix_len);
    i = prefix_len;

    while (*p != '\0' && i + 1 < out_size) {
        if (*p == ';') {
            /* Sufijo de version PS2 (ej. ";1"). Lo descartamos. */
            break;
        }
        out[i++] = *p;
        p++;
    }
    out[i] = '\0';
    return 0;
}

/* --------------------------------------------------------------------------
 * Tabla de descriptores
 * -------------------------------------------------------------------------- */

static int xbr_fd_alloc(FILE *fp) {
    int i;
    for (i = 0; i < XBR_FD_MAX; i++) {
        if (!sFds[i].used) {
            sFds[i].used = 1;
            sFds[i].fp = fp;
            return i + 1;
        }
    }
    return -1;
}

static FILE *xbr_fd_get(int fd) {
    int idx = fd - 1;
    if (idx < 0 || idx >= XBR_FD_MAX || !sFds[idx].used) {
        return NULL;
    }
    return sFds[idx].fp;
}

static void xbr_fd_free(int fd) {
    int idx = fd - 1;
    if (idx >= 0 && idx < XBR_FD_MAX) {
        sFds[idx].used = 0;
        sFds[idx].fp = NULL;
    }
}

/* --------------------------------------------------------------------------
 * API publica (mismos prototipos que src/main.c declara como extern)
 * -------------------------------------------------------------------------- */

int sceOpen(char *name, int flags) {
    char path[256];
    char mode[4];
    FILE *fp;
    int fd;
    static int logged;

    (void)flags;
    if (name == NULL) {
        return -1;
    }
    if (xbr_translate_path(name, path, sizeof(path)) != 0) {
        return -1;
    }

    /* 2.1: solo lectura. Cuando la fase 5/8 anada escritura se traducen
     * mas bits de flags (SCE_WRONLY, SCE_CREAT, SCE_TRUNC, ...). */
    if ((flags & 0xF) == XBR_SCE_RDONLY) {
        strcpy(mode, "rb");
    } else {
        strcpy(mode, "rb");
    }

    fp = fopen(path, mode);
    if (fp == NULL) {
        if (!logged) {
            Xbr_Log("Xbr_Log: sceOpen('%s' -> '%s') no existe\n", name, path);
            logged = 1;
        }
        return -1;
    }
    fd = xbr_fd_alloc(fp);
    if (fd < 0) {
        fclose(fp);
        return -1;
    }
    if (!logged) {
        Xbr_Log("Xbr_Log: sceOpen('%s' -> '%s') fd=%d\n", name, path, fd);
        logged = 1;
    }
    return fd;
}

int sceLseek(int fd, int offset, int whence) {
    FILE *fp = xbr_fd_get(fd);
    int origin;
    long result;

    if (fp == NULL) {
        return -1;
    }
    switch (whence) {
    case XBR_SCE_SEEK_SET: origin = SEEK_SET; break;
    case XBR_SCE_SEEK_CUR: origin = SEEK_CUR; break;
    case XBR_SCE_SEEK_END: origin = SEEK_END; break;
    default: return -1;
    }
    result = fseek(fp, (long)offset, origin);
    if (result != 0) {
        return -1;
    }
    return (int)ftell(fp);
}

int sceRead(int fd, void *buf, int size) {
    FILE *fp = xbr_fd_get(fd);
    size_t got;

    if (fp == NULL || buf == NULL || size < 0) {
        return 0;
    }
    got = fread(buf, 1, (size_t)size, fp);
    return (int)got;
}

int sceClose(int fd) {
    FILE *fp = xbr_fd_get(fd);
    if (fp == NULL) {
        return -1;
    }
    fclose(fp);
    xbr_fd_free(fd);
    return 0;
}
