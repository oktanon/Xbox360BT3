/*
 * port/include/xbox360/xbr_compat.h
 *
 * Shim de compat para el build Xbox 360. Se fuerza con /FI en BT3.props, en
 * frente de cualquier cabecera del juego. Reproduce la idea de
 * port/include/port_compat.h (que se usa en el build de PC) pero adaptada a
 * 360: anula la inclusion de asm MIPS y deja los tipos del juego tal cual.
 *
 * El codigo decompilado en src/ e include/ tiene tres piezas problematicas
 * para un compilador que no sea GCC/MIPS:
 *   1. include/include_asm.h contiene __asm__(".include ...") y otros
 *      asm statements que MSVC rechaza.
 *   2. Los headers esperan que INCLUDE_ASM(INCLUDE_RODATA, RODATA_ALIGN16,
 *      LIT4_WORD, ASM_STUB_BEGIN/END) sean macros con un sentido concreto
 *      (incluir .s preensamblados, alinear .rodata, etc.). En 360 no hay
 *      asm, asi que los anulamos.
 *   3. Los tipos s8/s16/s32/u32/... de include/types.h coinciden con los de
 *      MSVC; no hay que redefinirlos.
 *
 * Regla 1 de AGENTS.md: este shim vive en port/include/xbox360/, NO modifica
 * include/ ni src/.
 */
#ifndef XBOX360_XBR_COMPAT_H
#define XBOX360_XBR_COMPAT_H

/* include_asm.h: lo declaramos ya includo para que la guarda #ifndef
 * INCLUDE_ASM_H del header lo salte entero. Asi evitamos tanto los
 * __asm__(".include ...") como los macros de asm. */
#define INCLUDE_ASM_H

/* Macros que el juego o sus headers esperan definidos. Anulamos. */
#define INCLUDE_ASM(FOLDER, NAME)
#define INCLUDE_RODATA(FOLDER, NAME)
#define RODATA_ALIGN16()
#define LIT4_WORD(NAME, HEX)
#define ASM_STUB_BEGIN() extern int asm_stub_begin_
#define ASM_STUB_END() extern int asm_stub_end_

/* GCC __attribute__((...)): muy usado en los headers del juego para
 * alinear vectores (vmx128 en PS2) o definir tipos de 128 bits en
 * GCC (mode(TI)). MSVC no los reconoce. Variadic absorbing both forms
 * (alineado y mode). Perdemos la alineacion estricta en structs del
 * juego: aceptable para la fase 1 (main.c no usa structs vectoriales);
 * las fases 3-7 (renderer) daran el salto a __declspec(align(N)) o a
 * un atributo propio de la capa de plataforma. */
#if defined(_MSC_VER) && !defined(__attribute__)
#define __attribute__(...)
#endif

#endif
