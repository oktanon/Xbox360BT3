# PLAN.md: Fase 0, cierre de documentación del port de BT3 a Xbox 360

Este archivo va en la raíz del repo, junto a `AGENTS.md`. El agente lo lee antes de cada tarea
y lo actualiza al cerrar cada ítem.

## Estado general
- Fase: 0 (análisis y documentación). Sin consola y sin toolchain instalado.
- Rama de trabajo: `docs/fase0-correcciones`.
- Última actualización: (completar al cerrar cada sesión).

## Hechos verificados
No reverificar salvo que se pida. Cada hecho tiene su evidencia.

- El grep de `#ifdef PORT` / `#ifndef PORT` en `src/` e `include/` da 193 líneas, con 191 bloques
  reales de PORT. La diferencia son las guardas `PORT_VU0_A_H` y `PORT_VU0_B_H`. Los bloques
  `REF_VU0_*` y `EFT_Z_IMPL` no son PORT. Evidencia: `docs/port/evidence/grep-ifdef.txt`.
- `include/battle/eft_z.h:365` y `:408` son `#ifdef PORT`.
- La Sección 1 de `xbox360-todo.md` tiene 369 filas. Clasificación: 175 llamadas reales, 27
  definiciones, 70 stub/implementación en `port/src/`, 26 accesos a registros, 26 NOP VU1, 17
  declaraciones, 15 `#define`, 12 ensamblador inline, 1 comentario. Evidencia:
  `docs/port/evidence/classified_calls.tsv` y `classify_calls.py`.
- Las 175 llamadas tienen sintaxis verificada. Ninguna está dentro de `#if 0`, `#ifdef` o un
  comentario. Una tiene continuación con `\` (`src/sys/mcflow_a.c:103`, macro `MCF_POLL`).
- `src/menu/menu_g_b.c`: en el commit `102f5c0`, `case 39` y `case 40` tienen
  `gProgress->mode = 38;` sin `#ifdef`. El commit `18c32e3` agrega `#ifdef PORT` (líneas 57 y 76)
  que pone `mode = 4` solo si `Port_NetSession()`. Veredicto: Revertir.
- `docs/systems/menu_overlay.md:273-275` describe la misma lógica de modos 38, 39 y 40.

## Decisiones pendientes (las toma Estani, no el agente)
- [ ] Toolchain: libxenon (recomendado; falta confirmarlo por escrito).
- [ ] División de `xbox360-todo.md`: Opción 1, por subsistema (regla 16 de AGENTS.md).
- [ ] Netplay: Revertir para `menu_g_b.c:57` y `:76`; Desactivar para el resto de bloques de netplay.
- [ ] Definición de `#ifdef PORT` en la columna de mecanismo (la propone el agente en la sesión 1; aprobarla).

## Verificaciones pendientes
- [ ] `IVec4_Copy`, `IVec4_Swap` y `Vu0_InitAxisRegs`: confirmar las líneas citadas en
      `src/port/vu0_a.c` (1255, 1271, 1477) con `git grep -n`.
- [ ] `sceCdSearchFile`: se usa en `src/sys/file.c:194` y no tiene definición en `port/`.
      Si no está en ningún lado, el build de PC no enlaza. Marcar `TODO(verificar-enlace)`.
- [ ] `VIF1_STAT` y `GIF_STAT`: decidir si son emulación (`port/src/plat_mem.c:10`) o esperas de
      hardware. Aplicar una sola categoría a todas las filas.
- [ ] `D2_CHCR`: definir qué significa `#ifdef PORT` en la columna de mecanismo y aplicarlo a todas las filas.
- [ ] Tabla cruzada categoría × mecanismo, en TSV. Explicar las 70 filas de categoría "stub/impl en port/src"
      frente a las 107 de mecanismo "stub de enlace".
- [ ] `MCF_POLL`: `git grep -n MCF_POLL`. Si no se usa, marcar la fila 245 como `macro_no_usada`.
- [ ] `src/battle/hud_a.c:439`: `git show 92b6e74 -- src/battle/hud_a.c`. Confirmar que el `#ifdef PORT`
      es el único que agrega `Port_ClashSquare`.
- [ ] Revisar los dos TSV completos, no solo las primeras líneas.

## Correcciones de contenido
- [ ] `xbox360-todo.md` en "Riesgos técnicos": nota sobre las direcciones PS2 en los `extern` de
      `menu_g_b.c` (`0x356090`, `0x348710`, `0x351508`). Son dependencias de código, no de hardware.
- [ ] Quitar "Dragon Smash" y "sticks ausentes en teclado". Ninguna de las dos aparece en el código.
- [ ] `shaders.h`: se genera con `port/tools/undefined.py` (líneas 121 a 150) desde `port/src/gs/shaders/`.
      No es un archivo fuente.
- [ ] Unificar en la taxonomía la categoría de netplay (Revertir / Desactivar).
- [ ] Reemplazar los 11 veredictos "Revisar caso por caso" por veredictos de la taxonomía, con evidencia.
- [ ] Quitar `TODO(verificar-abi)` de `hud_a.c:439`. No aplica a ese bloque.

## Documentos
- [ ] `xbox360-todo.md`: links relativos con ancla de línea; numeración de encabezados; tabla de
      conciliación 201 → 191 → 193; dividir en hub y tres archivos por subsistema (1-2, 3-4, 5-7);
      conteos del hub iguales a la salida del script.
- [ ] `xbox360-status.md`: links relativos; `TODO(decidir-capa)` en menciones a Direct3D, XInput,
      XAudio2, XContentCreate, XPhysicalAlloc, CreateThread, CRITICAL_SECTION y HLSL; reemplazar
      "auditoría exhaustiva" por la descripción real del método; criterios de salida de la fase 0
      con evidencia; enlace a la sección de riesgos.
- [ ] `endian.md`: links relativos; verificar cada cita de archivo y línea; completar `SWAP64`;
      mover `GfxPostSpritePkt`, `EftAdXyzf` y `EftPrim` a "Formatos de paquetes internos (fuera de la
      regla de disco)"; tabla de máscaras de bitfields con `TODO(verificar)` donde falte confirmación;
      enlace a `endian-schema.md` marcado como pendiente; sección "Verificación de layout" con
      `static_assert` para `TexEntry` (0x40), `SaveData` (0x4000) y `McFlowReplayFile` (0x1AC00).
- [ ] `AGENTS.md`: confirmar que tiene criterios de salida de la fase 0, toolchain, ABI, caché y
      ensamblador en contexto técnico, reglas 10 a 16, taxonomía de veredictos, forma de trabajar y
      mapa de documentos. Si falta algo, reportarlo sin agregarlo.

## Pendientes de fase 1 (no empezar todavía)
- [ ] Instalar libxenon en WSL (en casa).
- [ ] Crear `docs/port/xbox360-build.md` con los comandos reales de build, empaquetado y subida.
- [ ] Crear `docs/port/endian-schema.md` para `TexFile`/`TexEntry`, `ObjMdlFile` y `StgRelocHdr`.

## Riesgos abiertos
- Esperas activas sobre `VIF1_STAT`, `GIF_STAT` y `D2_CHCR` en `src/sys/dma.c`. `TODO(hw-wait)`.
- Buffers `UNCACHED` en `src/sys/movie.c:66`. `TODO(decidir-capa)`.
- Convención de paso y retorno de structs en PowerPC de 32 bits. `TODO(verificar-abi)`.
- Código MIPS en cadenas `asm` de `src/sys/vu0_a_c.c`. Fuera del build de 360.

## Reglas de trabajo para el agente
1. Leer `AGENTS.md` y este `PLAN.md` antes de cada tarea.
2. Hacer los cambios de a uno, con alcance explícito: modificar solo la sección indicada y no tocar el resto.
3. Antes de editar, dar un resumen del cambio y esperar confirmación.
4. Después de cada cambio, revisar el diff con `git diff`.
5. Al cerrar cada ítem, marcarlo aquí y registrar la sesión.

## Registro de sesiones
- (Completar al cerrar cada sesión: fecha, modelo, qué cambió, `TODO` abiertos y archivos tocados.)
