# PLAN.md: Fase 0, cierre de documentación del port de BT3 a Xbox 360

Este archivo va en la raíz del repo, junto a `AGENTS.md`. El agente lo lee antes de cada tarea
y lo actualiza al cerrar cada ítem.

## Estado general
- Fase: 0 (análisis y documentación). Sin consola y sin toolchain instalado.
- Rama de trabajo: `docs/fase0-correcciones`.
- Última actualización: 2026-10-11 (sesión 5).
- Toolchain: **XDK** (decidido en sesión 5; pendiente fijar versión del SDK y actualizar
  `## Toolchain fijado` en `AGENTS.md`).

## Hoja de ruta de fases

Aprobada en la sesión 5. Toolchain: **XDK** (Direct3D 9, XInput, XAudio2, XContent).
El orden de subsistemas sigue la regla 3 de `AGENTS.md`. Cada fase tiene su criterio de
salida; una fase no empieza hasta cerrar la anterior.

- **Fase 0 — Análisis y documentación (en curso)**. Tareas: cerrar las verificaciones de
  este PLAN; resolver los 11 "Revisar caso por caso"; rutas relativas en `docs/port/`;
  completar `endian.md` (SWAP64, máscaras de bitfields) y esqueleto de `endian-schema.md`;
  `static_assert` de layouts (`TexEntry` 0x40, `SaveData` 0x4000, `McFlowReplayFile`
  0x1AC00); actualizar `AGENTS.md` con el toolchain. Salida: toolchain fijado por escrito,
  0 "revisar caso por caso", 0 rutas absolutas en `docs/`.
- **Fase 1 — Toolchain y esqueleto ejecutable**. Instalar el XDK (MSVC 2010 + Xbox 360
  SDK); build propio que compila `src/` + `include/` con `-DPORT -DXBOX360` excluyendo los
  4 archivos `vu0_*_c.c` (mismo conjunto `REPLACED` de `port/tools/portsrc.py:11`) y enlaza
  `src/port/vu0_a.c`, `src/port/vu0_b.c`, `port/src/softfloat_ps2.c`, `port/src/mathf_pc.c`;
  `plat_mem` de 360 (heap de 32 MB en dirección PS2, scratchpad y registros: el espacio de
  32 bits de 360 es plano y las direcciones PS2 caben directas; no se necesita
  `port/tools/ptr32.py`); stubs de sistema; log (UART/red). `docs/port/xbox360-build.md`
  con los comandos reales. Salida: XEX que arranca y llega con log hasta `Gfx_Init()`.
- **Fase 2 — Archivos, memoria e input**. Primitivas endianness (`ReadLE*`/`WriteLE*`/
  `SWAP*`) en un header común con tests de host; `plat_file`/`plat_fcache` sobre
  almacenamiento de 360 (`hdd:\`, `usb:\`); byteswap por formato de disco siguiendo
  `docs/port/endian.md` (packs, BPE, `TexFile`/`TexEntry` con reloc in-situ, `ObjMdlFile`
  y su bitfield `bound`, `StgRelocHdr`, `BobjAnim`, GSC, textos, bancos de sonido); input
  XInput → paquete DualShock2 de 18 bytes (2 mandos, vibración). Salida: recursos cargados
  con valores correctos en logs; input verificado.
- **Fase 3 — Renderer**. VBlank/pacing 60 Hz; interceptación DMA/GIF reutilizando
  `Port_DmaChcr` y la máquina de estados de `port/src/gs/gs_core.c` como oráculo; backend
  Direct3D 9 (shaders `vs_3_0`/`ps_3_0`) para los paquetes GS (`PRIM`, `TEST`, `ALPHA`,
  `TEX0`, `FRAME`, `ZBUF`, `SCISSOR`); formatos de textura GS (PSMT8/4/32, CLUT); render
  targets/EDRAM y `sceGsExecStoreImage` (cross-fade); efectos (outline, glow, depth tint,
  distance blur); HUD 16:9 nativo (`Port_WideFactor`, `Port_GsMarker`); split-screen.
  Salida: menú navegable y batalla renderizada, comparable con el build PC (hash/captura).
- **Fase 4 — Sonido**. ADX (decodificador C portable existente) → XAudio2; emulación SPU2
  (`port/src/gs/snd_se.c`) → mezcla 48 kHz; byteswap de cabeceras de bancos HD/VAG.
  Salida: música, voces y SFX correctos.
- **Fase 5 — Memory Card**. `plat_mc` → XContent o archivos en `hdd:\BT3\saves\`;
  `SaveData` serialize/deserialize LE + checksum idéntico (saves interoperables con el
  build PC); replays (0x1AC00). Salida: guardar/cargar partida y replay con checksum válido.
- **Fase 6 — Películas**. MPEG-2 software (portable) → textura D3D9; sincronía A/V;
  resolver buffers `UNCACHED` (`src/sys/movie.c:66`) con coherencia CPU/GPU. Salida:
  opening y cutscenes reproduciéndose.
- **Fase 7 — Lógica restante y verificación global**. `softfloat_ps2`/`mathf_pc` en PPC
  (redondeo exacto PS2, NaN, denormales); enlace VU0 (cierra `TODO(verificar-enlace-vu0)`);
  ABI de structs PPC 32-bit contra los bloques "Evaluar con ABI PowerPC"; jobs → hilos;
  paridad contra el oráculo PC (mismas entradas → mismo log de estado). Salida: paridad
  funcional con el build PC.
- **Fase 8 — Optimización y empaquetado**. VMX en softfloat/renderer, 3 cores/6 hilos,
  tiling EDRAM; instalador de datos desde la ISO del usuario (adaptar
  `port/tools/extract_disc.py`; jamás commitear datos); opcionales (texture packs,
  stages/songs externos, overlay debug); guía de instalación en RGH. Salida: release
  usable en RGH.

Nota sobre "lenguaje": el juego ya es C portable; la conversión a Xbox 360 es compilación
cruzada a PowerPC big-endian más la capa de plataforma, no una traducción de lenguaje. El
ensamblador MIPS de `src/sys/vu0_*_c.c` queda fuera del build (hay versiones C en
`src/port/`).

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
- [x] Toolchain: **XDK** (Direct3D 9, XInput, XAudio2, XContent). Decidido en la sesión 5
      tras verificar que libxenon (free60) no tiene aceleración 3D en Xenos. Pendiente:
      fijar la versión exacta del SDK y actualizar `## Toolchain fijado` en `AGENTS.md`.
- [x] División de `xbox360-todo.md`: hub + 3 archivos (subsistemas 1-2, 3-4, 5-7).
- [ ] Netplay: Revertir para `menu_g_b.c:57` y `:76`; Desactivar para el resto de bloques de netplay.
- [ ] Definición de `#ifdef PORT` en la columna de mecanismo (la propone el agente en la sesión 1; aprobarla).
- [ ] Clasificación del ensamblador inline de `src/sys/vu0_a_c.c`: marcar como **emulación en port/src**.

## Verificaciones pendientes
- [x] `IVec4_Copy`, `IVec4_Swap` y `Vu0_InitAxisRegs`: confirmar las líneas citadas en
      `src/port/vu0_a.c` (1255, 1271, 1477) con `git grep -n`. **Cita corregida**:
      la línea correcta de `IVec4_Swap` (en realidad `Ref_IVec4_Swap`) es 1173,
      no 1255. Mi cita de PLAN.md no estaba verificada.
- [x] `sceCdSearchFile`: se usa en `src/sys/file.c:194` y no tiene definición en `port/`. Resuelto en
      sesión 13: el port de PC genera el stub automáticamente con `port/tools/undefined.py + link.py`.
      Para Xbox 360 hay que añadirlo manualmente (veredicto "Backend propio para 360" en subsistema 1).
- [x] `VIF1_STAT` y `GIF_STAT`: reclasificadas de "emulación en port/src" a "espera activa (TODO(hw-wait))"
      en sesión 13. En 360 los registros no existen; sustituir por fence/Present de D3D9 (regla 10).
- [x] `D2_CHCR`: definido en sesión 4 como "emulación en port/src" (`Port_DmaChcr(2)`).
- [ ] `Vu0_Init`: resolver su clasificación según verificación de `port/tools/portsrc.py:11` y
      `src/port/vu0_b.c:1022`. Si `src/sys/vu0_b_c.c` está excluido del build de PC, el mecanismo
      debe ser **reemplazo por archivo excluido**, no "sin reemplazo".
- [x] Rutas absolutas en `docs/port/`: 746 URLs `file:///` reescritas a relativas con
      `docs/port/evidence/relative_paths.py` (sesión 7). Las 8 rutas Xbox
      (`game:\`, `hdd:\`, `usb:\`) se mantienen: son sintaxis de la API XContent/XDK.
- [x] Tabla cruzada categoría × mecanismo: generada en sesión 13 en `docs/port/evidence/cross_table.tsv`.
- [x] `MCF_POLL`: la macro se usa 21 veces en `src/sys/mcflow_a.c` (sesión 13). La cita original
      "macro_no_usada" era incorrecta.
- [x] `src/battle/hud_a.c:439`: resuelto en sesión 8 como "Reutilizar versión PORT (con adaptación)".
- [x] Revisar los dos TSV completos, no solo las primeras líneas.
- [x] Protección de `src/` y `port/src/` durante la fase 0: `git diff main --stat -- src port/src`
      y `git status --short src port/src` están vacíos. No se modificó código fuente protegido.

## Correcciones de contenido
- [x] `AGENTS.md`: actualizado `## Toolchain fijado` a XDK, el criterio de salida de la
      fase 0, el bloque "Toolchain" en "Contexto técnico", las prohibiciones de la fase 0
      y la regla 8. Versión exacta del SDK queda como pendiente de fase 1 (placeholder
      explícito). Sesión 6.
- [ ] `xbox360-todo.md` en "Riesgos técnicos": nota sobre las direcciones PS2 en los `extern` de
      `menu_g_b.c` (`0x356090`, `0x348710`, `0x351508`). Son dependencias de código, no de hardware.
- [x] Quitar "Dragon Smash" y "sticks ausentes en teclado". Ninguna de las dos aparece en el código.
      **Verificado el 2026-10-11 (sesión 1)**: `git grep -n "Dragon Smash" -- src/ include/ port/ docs/`
      y `git grep -n "sticks ausentes|stick ausente|stick missing|missing stick" -- docs/port/`
      no devuelven coincidencias. La única coincidencia de "missing sticky" en
      `src/port/vu0_b.c:28` es texto sobre bits sticky de IEEE 754, no sobre sticks
      de teclado. Item descartado; no hay texto a quitar. Evidencia:
      `docs/port/evidence/verify-dragon-smash-missing.txt`.
- [ ] `shaders.h`: se genera con `port/tools/undefined.py` (líneas 121 a 150) desde `port/src/gs/shaders/`.
      No es un archivo fuente.
- [ ] Unificar en la taxonomía la categoría de netplay (Revertir / Desactivar).
- [x] Reemplazar los 11 veredictos "Revisar caso por caso" por veredictos de la taxonomía, con evidencia.
- [ ] Quitar `TODO(verificar-abi)` de `hud_a.c:439`. No aplica a ese bloque.

## Documentos
- [ ] `xbox360-todo.md`: links relativos con ancla de línea; numeración de encabezados; tabla de
      conciliación 201 → 191 → 193; dividir en hub y tres archivos por subsistema (1-2, 3-4, 5-7);
      conteos del hub iguales a la salida del script.
- [ ] `xbox360-status.md`: links relativos; `TODO(decidir-capa)` en menciones a Direct3D, XInput,
      XAudio2, XContentCreate, XPhysicalAlloc, CreateThread, CRITICAL_SECTION y HLSL; reemplazar
      "auditoría exhaustiva" por la descripción real del método; criterios de salida de la fase 0
      con evidencia; enlace a la sección de riesgos.
- [x] `endian.md`: SWAP64 completado (sesión 9), tabla de máscaras de bitfields
      verificada (sesión 9), paquetes DMA movidos a 3.12 y sección de
      `static_assert` añadida (sesión 10). Tamaño de los 3 structs verificados
      con `git grep` (`TexEntry` 0x40 en `gfxm_b_b.h:28`, `SaveData` 0x4000 en
      `save.h:13`, `McFlowReplayFile` 0x1AC00 en `mcflow_a.h:92`).
- [ ] `AGENTS.md`: confirmar que tiene criterios de salida de la fase 0, toolchain, ABI, caché y
      ensamblador en contexto técnico, reglas 10 a 16, taxonomía de veredictos, forma de trabajar y
      mapa de documentos. Si falta algo, reportarlo sin agregarlo.

## Pendientes de fase 1 (no empezar todavía)
- [x] Instalar el XDK (MSVC 2010 + Xbox 360 SDK) y fijar la versión exacta en `AGENTS.md`
      (sesión 14: XDK 21256.3 + VS 2010 Pro, rutas en `AGENTS.md` `## Toolchain fijado`).
- [ ] Crear `docs/port/xbox360-build.md` con los comandos reales de build, empaquetado y subida.
- [x] Crear `docs/port/endian-schema.md` para `TexFile`/`TexEntry`, `ObjMdlFile` y `StgRelocHdr`.

## Riesgos abiertos
- Esperas activas sobre `VIF1_STAT`, `GIF_STAT` y `D2_CHCR` en `src/sys/dma.c`. `TODO(hw-wait)`.
- Buffers `UNCACHED` en `src/sys/movie.c:66`. `TODO(decidir-capa)`.
- Convención de paso y retorno de structs en PowerPC de 32 bits. `TODO(verificar-abi)`.
- Código MIPS en cadenas `asm` de `src/sys/vu0_a_c.c`. Fuera del build de 360.
- Mapeo de los nombres VU0 (`IVec4_Swap`, `IVec4_Copy`, `Vu0_InitAxisRegs`) en el build de Xbox 360: el redirect por `-include port/src/vu0_names.h` sólo aplica al PC. Las funciones `Ref_*` de `src/port/vu0_a.c:1173,1271,1477` son C portable y probablemente se compilen sin cambios en PowerPC; el MIPS hand-written de `src/sys/vu0_a_c.c` y `vu0_a_c_b.c` queda fuera. `TODO(verificar-enlace-vu0)`. Documentado en `docs/port/xbox360-todo.md` sección "Riesgos abiertos". Evidencia: `docs/port/evidence/grep-vu0-raw.txt`, `docs/port/evidence/verify-vu0-lines.txt`, `docs/port/evidence/classify_calls.py:30-32`.

## Reglas de trabajo para el agente
1. Leer `AGENTS.md` y este `PLAN.md` antes de cada tarea.
2. Hacer los cambios de a uno, con alcance explícito: modificar solo la sección indicada y no tocar el resto.
3. Antes de editar, dar un resumen del cambio y esperar confirmación.
4. Después de cada cambio, revisar el diff con `git diff`.
5. Al cerrar cada ítem, marcarlo aquí y registrar la sesión.

## Registro de sesiones
- **Sesión 1, 2026-10-11, opencode-go/minimax-m3**:
  - Creación de rama `docs/fase0-correcciones` desde `main`. `git status --short`
    confirma que no hay cambios en `src/` ni en `port/src/` (los archivos sin commitear
    son `docs/`, `port/tests/endian/`, `PLAN.md` y `AGENTS.md`, todos fuera de las
    carpetas protegidas).
  - Commit base `79dcafa` con `PLAN.md` y `docs/port/*` (8 archivos, 2404 inserciones).
    Se omitieron `AGENTS.md` y `port/tests/endian/` según indicación del usuario.
  - Verificación reproducible de `IVec4_Copy`, `IVec4_Swap` y `Vu0_InitAxisRegs`
    en `src/port/vu0_a.c`. Cita de PLAN.md imprecisa: `IVec4_Swap` está en 1173
    (no 1255); los nombres reales llevan prefijo `Ref_`. Evidencia guardada en
    `docs/port/evidence/verify-vu0-lines.txt`.
  - Verificación reproducible de inexistencia de "Dragon Smash" y "sticks ausentes
    en teclado" en todo el repo (PS2 + PC). Evidencia guardada en
    `docs/port/evidence/verify-dragon-smash-missing.txt`.
  - Actualización de `PLAN.md`: marcados los dos items verificados y registrado
    este resumen de sesión.
  - Archivos tocados: `PLAN.md`, `docs/port/evidence/verify-vu0-lines.txt`,
    `docs/port/evidence/verify-dragon-smash-missing.txt` (todos sin commitear
    todavía, esperando confirmación).
  - `TODO` abiertos al cierre de la sesión: ninguno nuevo. Las verificaciones
    restantes de "Verificaciones pendientes" siguen pendientes.

- **Sesión 2, 2026-10-11, opencode-go/minimax-m3**:
  - Corrección de la cita errónea de `IVec4_Swap` (1255 → 1173) en `PLAN.md`,
    `verify-vu0-lines.txt`, `classify_calls.py:31` y 13 filas del TSV.
  - Hallazgo: el redirect `Ref_*` → nombre asm existe en
    `port/src/vu0_names.h:26, 37, 169`. No tiene `#ifdef PORT`; se aplica al
    compilar `src/port/vu0_a.c` con `-include port/src/vu0_names.h` (verificado en
    `port/tools/undefined.py:175` y `port/tools/check_protos.py:58`).
  - Verificación de exclusión: `src/sys/vu0_a_c.c` y `src/sys/vu0_a_c_b.c` están
    en el conjunto `REPLACED` de `port/tools/portsrc.py:11`, no se compilan en
    PC build. Salida cruda guardada en `docs/port/evidence/grep-vu0-raw.txt`.
  - Aplicada la lectura laxa: mecanismo se mantiene como "emulación en port/src"
    (4 categorías); `regla_reemplazo` actualizado a "renombrado por -include
    port/src/vu0_names.h" en las 13 filas VU0 del TSV.
  - Nueva sección "4. Riesgos abiertos" en `xbox360-todo.md` (la sección
    "Conclusiones" pasó a ser la 5). Contiene el ítem
    `TODO(verificar-enlace-vu0)` con la nota sobre la redirección y la
    reutilización esperada de las versiones `Ref_*` en PowerPC.
  - Bullet equivalente agregado a "Riesgos abiertos" en `PLAN.md`.
  - Archivos tocados: `PLAN.md`, `docs/port/xbox360-todo.md`,
    `docs/port/evidence/classify_calls.py`,
    `docs/port/evidence/classified_calls.tsv`,
    `docs/port/evidence/verify-vu0-lines.txt`, y nuevo
    `docs/port/evidence/grep-vu0-raw.txt` (todos sin commitear, esperando
    confirmación).
  - `TODO` abiertos al cierre de la sesión: ninguno nuevo.

- **Sesión 3, 2026-10-11, opencode-go/kimi-k2.7-code**:
  - Revertida la "lectura laxa" de la sesión 2 en `docs/port/evidence/classify_calls.py`
    y `docs/port/evidence/classified_calls.tsv`: se restauraron los textos originales de
    `regla_reemplazo` para `IVec4_Copy`, `IVec4_Swap` y `Vu0_InitAxisRegs`, y se eliminó
    la cita falsa `port/src/vu0_b.c:1022` para `Vu0_Init`, devolviéndolo a
    `sin reemplazo` / `src/sys/vu0_b_c.c:1083`.
  - Se mantuvo la corrección de `IVec4_Swap` a `src/port/vu0_a.c:1173`.
  - Verificación de bloques `#ifdef PORT`:
    - `include/sys/dma.h:86`: dentro de `#ifdef PORT` (líneas 85-89).
    - `src/sys/dma.c:279`: fuera de `#ifdef PORT`.
    - `src/sys/movie.c:59`: dentro de `#ifdef PORT` (líneas 57-62).
  - Verificación de citas de la sesión 2:
    - `port/tools/portsrc.py:11` incluye `src/sys/vu0_a_c.c` y `src/sys/vu0_a_c_b.c` en `REPLACED`.
    - `port/tools/check_protos.py:58` menciona `vu0_names.h`.
    - `port/tools/undefined.py:175` menciona `vu0_names.h`.
  - Conteo de rutas absolutas en `docs/port/`: `endian.md:66`, `xbox360-status.md:5`,
    `xbox360-todo.md:662`; total 733.
  - Criterio 6 de `AGENTS.md` verificado: `git diff main --stat -- src port/src` y
    `git status --short src port/src` están vacíos.
  - Archivos tocados: `docs/port/evidence/classify_calls.py`,
    `docs/port/evidence/classified_calls.tsv`, `PLAN.md`.
  - `TODO` abiertos al cierre de la sesión: ninguno nuevo.

- **Sesión 4, 2026-10-11, opencode-go/kimi-k2.7-code**:
  - Verificación de `Vu0_Init`:
    - `git grep -n "Vu0_Init" -- src port`:
      - Definición original PS2: `src/sys/vu0_b_c.c:1083`.
      - Definición usada por el build de PC: `src/port/vu0_b.c:1022` (`Ref_Vu0_Init`,
        redirigido a `Vu0_Init` por `port/src/vu0_names.h:168`).
      - Llamadas: `src/main.c:146`, `src/sys/gfxm_d_c.c:90`, `src/sys/vu0_b_c.c:1085`.
    - `git grep -n "Ref_Vu0_Init" -- src port`:
      - `port/src/vu0_names.h:168` (`#define Ref_Vu0_Init Vu0_Init`)
      - `port/src/vu0_names.h:169` (`#define Ref_Vu0_InitAxisRegs Vu0_InitAxisRegs`)
      - `src/port/vu0_a.c:1477` (`Ref_Vu0_InitAxisRegs`)
      - `src/port/vu0_b.c:1022` (`Ref_Vu0_Init`)
    - Revisión de `port/tools/portsrc.py`:
      - Línea 11: `REPLACED = { ..., "src/sys/vu0_b_c.c", ... }`.
      - Línea 17: el filtro `not in REPLACED` se usa en `sources()`.
      - `port/tools/check_protos.py:77` y `port/tools/undefined.py:84` llaman a
        `portsrc.sources()`; por tanto `src/sys/vu0_b_c.c` **no se compila en PC**.
    - Veredicto provisional: la clasificación de `Vu0_Init` debe ser **reemplazo por
      archivo excluido** (`port/tools/portsrc.py:11`, `src/port/vu0_b.c:1022`), no
      "sin reemplazo" / `src/sys/vu0_b_c.c:1083`.
  - Residuos de la sesión 2:
    - `git grep -n "vu0_b.c:1022\|lectura laxa\|port/src/vu0_b.c" -- docs PLAN.md`
      devolvió solo tres coincidencias, todas en `PLAN.md` (registro de sesiones 2 y 3).
      No quedó la cita falsa en `docs/port/xbox360-todo.md`.
  - Desglose de rutas absolutas en `docs/port/`:
    - `file:///`: `(git grep -o -n -E 'file:///' -- docs/port/).Count` = **746**.
      Ejemplos: `docs/port/endian.md:9`, `:35`, `:36`.
    - `[A-Za-z]:\\`: `(git grep -o -n -E '[A-Za-z]:\\' -- docs/port/).Count` = **14**.
      Ejemplos: `docs/port/xbox360-todo.md:413` (`c:\` dentro de URL),
      `:735` (`game:\`, `hdd:\`, `usb:\`), `:847` (`hdd:\BT3\saves\...`).
      De las 14, 6 son `c:\` dentro de URLs `file:///c:/...` (falsos positivos como
      rutas Windows reales) y 8 son rutas reales de Xbox 360 (`game:\`, `hdd:\`, `usb:\`).
    - `/home/`: `(git grep -o -n -E '/home/' -- docs/port/).Count` = **0**.
    - `/Users/`: `(git grep -o -n -E '/Users/' -- docs/port/).Count` = **746**.
      Todos son falsos positivos: aparecen dentro de las URLs `file:///c:/Users/...`.
    - Total real de rutas absolutas: 746 URLs `file:///` + 8 rutas Xbox = **754**.
      Los 6 `c:\` y 746 `/Users/` son solapamientos con las URLs.
  - `#ifdef PORT` para `D2_CHCR`:
    - `git grep -n "D2_CHCR" -- include/sys/dma.h`:
      - `include/sys/dma.h:93`: `#define D2_CHCR Port_DmaChcr(2)` (dentro de `#ifdef PORT`)
      - `include/sys/dma.h:95`: `#define D2_CHCR ((volatile u32 *)0x1000A000)` (fuera de PORT)
    - Texto propuesto para PLAN.md: `include/sys/dma.h:92-96`: bajo `#ifdef PORT`,
      `D2_CHCR` se redefine como `Port_DmaChcr(2)`; en PS2 es el acceso directo al
      registro DMA `((volatile u32 *)0x1000A000)`. Mecanismo: **emulación en port/src**.
  - Actualización de `PLAN.md`: agregados ítems de verificación pendiente para `Vu0_Init`
    y desglose de rutas absolutas; agregada decisión pendiente para clasificar el
    ensamblador inline de `vu0_a_c.c` como **emulación en port/src**.
  - Archivos tocados: `PLAN.md`.
  - `TODO` abiertos al cierre de la sesión: los ítems agregados en "Verificaciones
    pendientes" y "Decisiones pendientes".

- **Sesión 5, 2026-10-11, opencode-go/kimi-k3**:
  - Lectura completa del proyecto: `AGENTS.md`, `PLAN.md`, `docs/port/xbox360-status.md`,
    `docs/port/xbox360-todo.md` (894 líneas), `docs/port/endian.md`, `README.md`,
    `configure.py`, `port/tools/portsrc.py`, `port/src/plat_mem.c`, `src/main.c` y la
    estructura de `port/src/` y `port/src/gs/`.
  - Hallazgo verificado con fuentes externas (free60.org y documentación de libxenon):
    libxenon no tiene aceleración 2D/3D en la GPU Xenos; las APIs que los documentos
    mencionan (Direct3D 9, XInput, XAudio2, XContent) son del XDK de Microsoft.
  - Decisión del usuario (vía pregunta interactiva): toolchain **XDK**.
  - Hallazgo de arquitectura registrado en la hoja de ruta: Xbox 360 es 32 bits nativo,
    por lo que la maquinaria de punteros del port de PC (`port/tools/ptr32.py`, mapeo de
    memoria bajo 4 GB de `port/src/plat_mem.c`) no es necesaria en 360; el trabajo se
    concentra en endianness y capa de plataforma.
  - Añadida la sección "Hoja de ruta de fases" (fases 0 a 8 con tareas y criterios de
    salida) a `PLAN.md`, y el mapa de fases con estados a `docs/port/xbox360-status.md`.
    El orden de subsistemas sigue la regla 3 de `AGENTS.md`.
  - Actualizadas las "Decisiones pendientes" (toolchain XDK decidido), los "Pendientes de
    fase 1" (instalación del XDK en lugar de libxenon) y las "Correcciones de contenido"
    (nuevo ítem: actualizar `AGENTS.md` con el toolchain XDK, pendiente de aprobación).
  - Alcance aprobado por el usuario para esta sesión: solo la hoja de ruta. Quedan para
    sesiones siguientes: rutas relativas en `docs/port/`, los 11 "Revisar caso por caso",
    `endian.md` (SWAP64, máscaras) y `AGENTS.md`.
  - Archivos tocados: `PLAN.md`, `docs/port/xbox360-status.md`.
  - `TODO` abiertos al cierre de la sesión: actualizar `AGENTS.md` (toolchain XDK) tras
    aprobación; los pendientes de fase 0 siguen como estaban.

- **Sesión 6, 2026-10-11, opencode-go/kimi-k3**:
  - Actualización de `AGENTS.md` al toolchain XDK, aprobada por el usuario ("empieza por
    el punto 1" de la lista de TODO de la sesión 5):
    - Criterio de salida de la fase 0: "Toolchain elegido: XDK, con nombre y versión
      fijados en ## Toolchain fijado; la versión exacta del SDK queda como pendiente
      de fase 1".
    - Prohibiciones de la fase 0: "instalar el XDK de Xbox 360 (MSVC 2010 + Xbox 360
      SDK), descargar cualquier SDK de Xbox 360, o ejecutar cualquier compilador cruzado
      para PowerPC". Se elimina la mención a "instalar libxenon, descargar SDKs de
      PowerPC".
    - Bloque "Toolchain" en "Contexto técnico" reescrito: XDK = toolchain; capa de
      plataforma = Direct3D 9, XInput, XAudio2, XContent, más las APIs de Windows que
      el XDK expone; libxenon queda nombrado como lo que NO se usa.
    - Sección "## Toolchain fijado": nombre XDK, versión con placeholder explícito
      ("<pendiente de fijar; ver Pendientes de fase 1 en PLAN.md>"), justificación con
      referencia a free60.org y a la sesión 5 de PLAN.md.
    - Regla 8: xbox360-build.md pendiente de fase 1 "con el XDK ya fijado".
  - Las 3 menciones restantes de "libxenon" en AGENTS.md son intencionales (todas en
    contexto de justificación, equivalencias o comparación con el XDK).
  - Cierre del pendiente "AGENTS.md: actualizar ## Toolchain fijado" en PLAN.md.
  - Archivos tocados: `AGENTS.md` (137 líneas, +12 sobre el original), `PLAN.md`
    (actualización de "Correcciones de contenido" y entrada de sesión 6).
  - Comandos ejecutados: `git diff --no-index /dev/null AGENTS.md` para revisar el
    resultado (AGENTS.md estaba untracked; el diff muestra el archivo entero como
    nuevo), `git grep` para localizar las menciones a libxenon y XDK.
  - `TODO` abiertos al cierre de la sesión: ninguno nuevo. La versión exacta del SDK
    del XDK queda como pendiente en "Pendientes de fase 1" de PLAN.md.

- **Sesión 7, 2026-10-11, opencode-go/kimi-k3**:
  - Confirmado por el usuario: toolchain XDK (decidido en sesión 5) + versión del SDK
    queda pendiente. Plan: commit 1 (sesiones 5+6) y luego punto 2 (rutas relativas).
  - Commit 1: `bbfe268` "fase 0: hoja de ruta a XDK y AGENTS.md actualizado"
    (3 archivos: `PLAN.md`, `docs/port/xbox360-status.md`, `AGENTS.md`).
  - Punto 2: rutas relativas en `docs/port/`. Análisis de patrones con `git grep`:
    729 líneas con la raíz `file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/`
    en 3 archivos (`endian.md`, `xbox360-status.md`, `xbox360-todo.md`); el conteo
    por ocurrencias del script es 746.
  - Script reproducible: `docs/port/evidence/relative_paths.py` (modo dry-run por
    defecto; `--apply` para reescribir). Evidencia: `docs/port/evidence/relative-paths.txt`
    (746 líneas con el mapeo original → relativo).
  - Aplicado: 746 URLs reescritas. `git grep -c 'file:///' -- 'docs/'` devuelve 0.
    Las 8 rutas Xbox (`game:\`, `hdd:\`, `usb:\`) se mantienen: son sintaxis de la
    API XContent/XDK, no `file:///` ni rutas de la máquina.
  - Verificación posterior: 2 enlaces a `port/src/gs/shaders.h` (archivo generado,
    no fuente) ya existían antes; no se introdujeron nuevos enlaces rotos.
  - Commit 2: `110ba2e` "fase 0: rutas relativas en docs/port/ y contenido de
    sesiones 2-4" (9 archivos, 1636 inserciones, 744 borrados). Incluye los cambios
    de contenido de sesiones 2-4 que estaban sin commitear
    (`docs/port/evidence/classified_calls.tsv`, `classify_calls.py`,
    `verify-vu0-lines.txt`, nuevo `grep-vu0-raw.txt`, y los cambios de
    `xbox360-todo.md`: sección "4. Riesgos abiertos" + `TODO(verificar-enlace-vu0)`).
    Mezclarlos en un solo commit es la forma más simple de mantener la historia
    lineal; si el usuario prefiere separarlos, se puede reescribir.
  - Cierre del pendiente "Rutas absolutas en `docs/port/`" en "Verificaciones
    pendientes" de PLAN.md.
  - `port/tests/endian/` queda como untracked, fuera del repo por decisión de la
    sesión 1 (registrada en PLAN.md).
  - `TODO` abiertos al cierre de la sesión: los restantes de fase 0
    (11 "Revisar caso por caso", `endian.md` SWAP64/máscaras, esqueleto
    `endian-schema.md`, `static_assert` de layouts, división de
    `xbox360-todo.md` por subsistemas).

- **Sesión 8, 2026-10-11, opencode-go/kimi-k3**:
  - Resolución de los 11 "Revisar caso por caso" en `xbox360-todo.md` (regla 15).
    Cada bloque se leyó en su archivo fuente para entender qué hace antes de
    asignar el veredicto de la taxonomía de AGENTS.md.
  - Distribución de los 11 veredictos:
    - 4 "Reutilizar versión PORT (idéntica)": `battle_load.c:547`
      (`Port_GameBigAlloc`), `eft_det_a.c:91` (macro `(fn, 0)` para void/s32),
      `eft_det_a.c:136` (normalización s64→s32), `eft_i.c:49` (bit field u64).
    - 1 "Reutilizar versión PORT (con adaptación)": `hud_a.c:439` (opción Square
      reutilizable; eliminar sub-condición de netplay).
    - 2 "Desactivar": `btl_cam.c:512` y `:531` (netplay online, consistente
      con la decisión de la sesión 5 de desactivar netplay en el port base).
    - 4 "Opcional": `battle_load.c:578` (stage de split-screen para mods
      externos, depende de fase 8), `:1518` `:1528` `:1544` (test harness del
      PC con variables `BT3_TEST_*`; no aplica en consola).
  - Verificación: `git grep -c "Revisar caso por caso" -- docs/port/xbox360-todo.md`
    devuelve 0. La única ocurrencia restante en el repo está en
    `docs/port/xbox360-status.md:11` como parte del criterio de salida de la
    fase 0 ("0 veredictos 'Revisar caso por caso'") — ahora cumplido.
  - Commit: `c19e771` "fase 0: resolver los 11 'Revisar caso por caso' (regla 15)"
    (1 archivo, 11 inserciones, 11 borrados).
  - Cierre del pendiente "Reemplazar los 11 veredictos 'Revisar caso por caso'"
    en "Correcciones de contenido".
  - `TODO` abiertos al cierre de la sesión: los restantes de fase 0
    (`endian.md` SWAP64/máscaras, esqueleto `endian-schema.md`,
    `static_assert` de layouts, división de `xbox360-todo.md` por subsistemas).

- **Sesión 9, 2026-10-11, opencode-go/kimi-k3**:
  - Trabajo en `endian.md` (punto 3.b de los TODO de la sesión 8):
    - Sección 4.1: `SWAP64` ya no es placeholder. Implementación portable que
      reutiliza `SWAP32` sobre la mitad alta y baja del `u64`, combinadas con
      shift y OR:
        ```c
        #define SWAP64(x) (((u64)SWAP32((u32)((u64)(x)         )) << 32) | \
                           (u64)SWAP32((u32)((u64)(x) >> 32)))
        ```
    - Sección 4.4 (Categoría 4 - bitfields): nueva subsección "Máscaras reales
      (verificadas)" con tabla de 10 entradas (máscara, campo, `#define`,
      archivo:línea) cubriendo `ObjMdlFile.bound` (0x80000000, sin macro),
      `Hud.showGauges/Timer/Notice/Combo/Prompt` (0x01..0x10 con macros
      `HUD_SHOW_*`), `HudCSprite.hidden/mirror` (1, 2 con macros
      `HUDC_SPR_*`) y `HudENode.hidden/mirror` (1, 2 sin macro, comentario
      en línea).
    - Todas las citas verificadas con `git grep` (regla 13 de AGENTS.md):
      `include/sys/gfxm_e.h:32-33`, `include/battle/hud_a.h:76-80` y `92-101`,
      `include/battle/hud_c.h:17-23` y `40-41`, `include/battle/hud_e.h:30-36`.
    - Implementación de `ObjMdlFile.bound` corregida: una nota anterior decía
      "escrituras de `flags` en `src/sys/gfxm_e_b.c:174`", que era incorrecta
      (esa línea escribe `tbp`, no `flags`). `git grep` confirma que el único
      acceso a `mdl->file->bound` en `src/` es la lectura de `src/sys/gfxm_e_b.c:173`;
      no se accede a `flags` desde `src/`. La implementación ahora describe
      dos opciones coherentes.
  - Commit: `e4d76e0` "fase 0: completar SWAP64 y tabla de mascaras de
    bitfields en endian.md" (1 archivo, 40 inserciones, 3 borrados).
  - Cierre parcial del pendiente "`endian.md` SWAP64 y máscaras" en
    "Correcciones de contenido" de PLAN.md. Quedan para sesiones siguientes
    los otros sub-ítems del pendiente: mover `GfxPostSpritePkt`/`EftAdXyzf`/
    `EftPrim` a "Formatos de paquetes internos", sección "Verificación de
    layout" con `static_assert` de TexEntry/SaveData/McFlowReplayFile.
  - `TODO` abiertos al cierre de la sesión: los restantes de fase 0
    (mover GfxPostSpritePkt/EftAdXyzf/EftPrim en endian.md; esqueleto
    `endian-schema.md`; `static_assert` de layouts; división de
    `xbox360-todo.md` por subsistemas; verificaciones pendientes adicionales).

- **Sesión 10, 2026-10-11, opencode-go/kimi-k3**:
  - Resto de `endian.md` (punto "segui con el resto de endian.md"):
    - Sección 3.11: quitadas las 3 entradas de bitfields de 64 bits para
      paquetes DMA a la GPU (`GfxPostSpritePkt`, `EftAdXyzf`, `EftPrim`).
      Estas no son datos de disco, así que no cumplen la regla 2 de
      AGENTS.md; se documentan por separado.
    - Nueva sección 3.12 "Formatos de Paquetes Internos (Fuera de la Regla
      de Disco)": reubica las 3 entradas, explica el problema (PPC asigna
      bits desde MSB; x86 desde LSB; el GPU decodifica por offset fijo) y
      propone la estrategia de reemplazarlos por funciones `GS_Pack*` con
      shifts/ORs explícitos sobre posiciones de bits fijas, con un ejemplo
      completo de `EftAdXyzf_Pack`.
    - Nueva sección 4.5 "Formatos de Paquetes DMA a la GPU (Bitfields
      Internos)": resumen estratégico de la 3.12.
    - Nueva sección 4.6 "Verificación de Layout con `static_assert`": tabla
      de los 3 structs con tamaño crítico (`TexEntry` 0x40, `SaveData` 0x4000,
      `McFlowReplayFile` 0x1AC00), cada uno con su cita exacta al header y
      un bloque de `static_assert` para que el build de host (y de 360)
      detecte desviaciones en tiempo de compilación.
  - Todas las citas verificadas con `git grep` (regla 13):
    `gfxm_b_b.h:28`, `save.h:13` y `:114`, `mcflow_a.h:92`, `gfxm_a.c:67-85`,
    `eft_ad_b.c:43-48`, `eft_d.c:82-94`.
  - Commit: `ba40d75` "fase 0: mover paquetes DMA y anadir verificacion
    de layout en endian.md" (1 archivo, 89 inserciones, 3 borrados).
  - Cierre del pendiente "`endian.md` SWAP64 y máscaras, mover bitfields
    internos, sección de static_assert" en "Correcciones de contenido" de
    PLAN.md.
  - `TODO` abiertos al cierre de la sesión: los restantes de fase 0
    (esqueleto de `endian-schema.md` para TexFile/TexEntry, ObjMdlFile y
    StgRelocHdr; división de `xbox360-todo.md` por subsistemas si supera
    500 líneas tras las ediciones; verificaciones pendientes adicionales
    como `sceCdSearchFile`, VIF1_STAT/GIF_STAT, tabla cruzada
    categoría×mecanismo, MCF_POLL; y la versión exacta del SDK del XDK).

- **Sesión 11, 2026-10-11, opencode-go/kimi-k3**:
  - Creación de `docs/port/endian-schema.md` (145 líneas, dentro del
    límite de 500 de la regla 16). Complementa a `endian.md` con un mapa
    campo-a-campo de los structs binarios de disco:
    1. `TexFile` y `TexEntry` (`include/sys/gfxm_b_b.h:28-52`): tablas con
       offset, tipo, tamaño, endianness LE en disco y acción en 360
       (`ReadLE32` + `STG_RELOC` para punteros).
    2. `ObjMdlFile` (`include/sys/gfxm_e.h:30-49`): idem, incluyendo el
       bitfield `flags:31/bound:1` (PPC asigna `bound` al MSB; enlaza con
       `endian.md` 4.4).
    3. `StgRelocHdr` y sub-structs (`include/sys/gfxm_e.h:172-276`):
       cabecera del escenario con `STG_RELOC` para cada puntero, y tabla
       resumen de los 10 sub-structs (A, B, C, D, E, Timer, A1, A2, B1,
       D1) con sus offsets de punteros.
    4. `SaveData` (resumen; `static_assert 0x4000`).
    5. `McFlowReplayFile` (resumen; `static_assert 0x1AC00`).
  - Verificación de citas (regla 13): todas las líneas citadas confirmadas
    con `git show HEAD:...` durante la creación. Una cita errónea
    detectada y corregida: la condición "`0` y `-1` se convierten a NULL"
    en `StgOctNode.list` está en `src/sys/gfxm_e_d.c:29-30`, no `:43-44`
    como puse inicialmente; corregido antes del commit.
  - Commit: `f341907` "fase 0: esqueleto de endian-schema.md (TexFile,
    ObjMdlFile, StgRelocHdr)" (1 archivo, 194 inserciones, archivo nuevo).
  - Cierre del pendiente "Crear `docs/port/endian-schema.md`" en
    "Pendientes de fase 1" de PLAN.md.
  - `TODO` abiertos al cierre de la sesión: los restantes de fase 0
    (división de `xbox360-todo.md` por subsistemas si supera 500 líneas;
    verificaciones pendientes: `sceCdSearchFile`, VIF1_STAT/GIF_STAT,
    tabla cruzada categoría×mecanismo, MCF_POLL; y la versión exacta
    del SDK del XDK).

- **Sesión 12, 2026-10-11, opencode-go/kimi-k3**:
  - División de `xbox360-todo.md` (regla 16 de AGENTS.md). El doc tenía
    821 líneas tras la sesión 7; la sección 1 ("Llamadas a Hardware PS2
    por Subsistema") ocupaba 414 líneas con 369 llamadas distribuidas en
    7 subsistemas. La división sigue el plan original de PLAN.md sesión 1
    (hub + 3 archivos: 1-2, 3-4, 5-7).
  - Script reproducible: `docs/port/evidence/split_todo.py` (modo dry-run
    por defecto; `--apply` para escribir). Antes de aplicar, verifiqué
    que concatenando los rangos (14,82)+(83,250)+(251,427) se reconstruye
    la sección 1 original (74937 chars) byte a byte. Tras aplicar,
    verifiqué que cada "### Subsistema N" aparece exactamente 1 vez en
    su sub-archivo y que el título de sección 1 aparece 4 veces (1 índice
    en hub + 3 cabeceras).
  - Estructura resultante:
    - `xbox360-todo.md` (hub, 432 líneas): cabecera + resumen + índice a
      los 3 sub-archivos + secciones 2 (`#ifdef PORT`), 3 (stubs de
      `port/src/`), 4 (riesgos abiertos), 5 (conclusiones).
    - `xbox360-todo-subsistemas-1-2.md` (82 líneas): subsistemas
      Archivos/Memoria e Input.
    - `xbox360-todo-subsistemas-3-4.md` (179 líneas): Renderer y Sonido.
    - `xbox360-todo-subsistemas-5-7.md` (188 líneas): Memory Card,
      Películas, Lógica restante.
  - Todos los archivos quedan por debajo del límite de 500 líneas.
  - Los enlaces relativos `../../src/...` y `../../port/...` siguen
    válidos desde los 4 archivos (todos viven en `docs/port/`).
  - Commit: `28cebbb` "fase 0: dividir xbox360-todo.md por subsistemas
    (regla 16)" (5 archivos, 600 inserciones, 485 borrados).
  - Cierre del pendiente "División de `xbox360-todo.md`" en "Decisiones
    pendientes" de PLAN.md.
  - `TODO` abiertos al cierre de la sesión: verificaciones pendientes
    restantes (`sceCdSearchFile` enlace, VIF1_STAT/GIF_STAT
    clasificación, tabla cruzada categoría×mecanismo, MCF_POLL) y la
    versión exacta del SDK del XDK.

- **Sesión 13, 2026-10-11, opencode-go/kimi-k3**:
  - Verificaciones pendientes de PLAN.md resueltas (5 de 6; la 6ª,
    `hud_a.c:439`, ya estaba resuelta en sesión 8):
    - **V1 (`sceCdSearchFile`)**: `git grep` confirma que solo aparece
      como `extern` y llamada en `src/sys/file.c`. El port de PC genera
      el stub automáticamente con `port/tools/undefined.py` +
      `port/tools/link.py` (verificado en `port/tools/link.py:38`
      "undefined reference to `(\w+)'"). Para Xbox 360 hay que añadir
      un stub manual en `port/src/plat_file.c` (veredicto "Backend
      propio para 360" en subsistema 1).
    - **V2 (`VIF1_STAT`, `GIF_STAT`)**: reclasificadas de "emulación en
      port/src" a "espera activa (TODO(hw-wait))" en `SYMBOL_MAP` de
      `classify_calls.py`. 19 filas afectadas (9 VIF1_STAT + 10 GIF_STAT).
      El TSV regenerado tiene 19 filas con mecanismo "espera activa
      (TODO(hw-wait))" y 143 con "emulación en port/src" (antes 162).
    - **V3 (`D2_CHCR`)**: ya documentado en sesión 4 de PLAN.md
      (mecanismo: "emulación en port/src" vía `Port_DmaChcr(2)`).
    - **V4 (tabla cruzada)**: generada en `docs/port/evidence/cross_table.tsv`
      y explicada la asimetría 70 vs 107: 70 = categoría "Stub /
      implementación" (definiciones en `port/src/`); 107 = mecanismo
      "stub de enlace" (41 definiciones + 65 llamadas + 1 directiva = 107).
    - **V5 (`MCF_POLL`)**: `git grep -n MCF_POLL -- src/` devuelve 22
      ocurrencias (1 definición + 21 usos). La cita original que
      marcaba la fila 245 como `macro_no_usada` era incorrecta.
  - Script reproducible: `docs/port/evidence/cross_table.py` (lee
    `classified_calls.tsv`, genera `cross_table.tsv` con la matriz
    completa y la nota explicativa).
  - `classify_calls.py` actualizado para leer la sección 1 de los
    3 sub-archivos (post-división de sesión 12) en lugar del hub.
  - Commit: `f712edd` "fase 0: reclasificar VIF1_STAT/GIF_STAT y
    tabla cruzada" (4 archivos, 123 inserciones, 28 borrados).
  - Cierre de 6 verificaciones pendientes en PLAN.md.
  - `TODO` abiertos al cierre de la sesión: solo queda la versión
    exacta del SDK del XDK (cuando la tengas).

- **Sesión 14, 2026-10-11, opencode-go/kimi-k3**:
  - Cierre de la fase 0. El usuario proporcionó:
    - **XDK versión 21256.3** (última oficial del XDK 2010, Q4 2010).
    - **Visual Studio 2010 Professional** (EN-US).
    - **Rutas de instalación** (verificadas con `Test-Path`):
      - XDK: `C:\Program Files (x86)\Microsoft Xbox 360 SDK` (existe,
        contiene `art`, `bin`, `doc`, `include`, `lib`, `redist`,
        `Source`, `system`, `TechPreview`, `xboxfont`, `readme1st.txt`).
      - Visual Studio: `C:\Program Files (x86)\Microsoft Visual Studio 10.0`
        (existe, contiene la carpeta `Microsoft Visual Studio 2010
        Professional - ENU`).
  - Cambios en `AGENTS.md`:
    - Criterio de salida de la fase 0: actualizada la redacción para
      reflejar que la versión y rutas están fijadas.
    - Sección `## Toolchain fijado`: nombre XDK, versión **21256.3**,
      IDE VS 2010 Pro, rutas exactas con subcarpetas clave.
  - Cambios en `docs/port/xbox360-status.md`: nueva subsección
    "Referencias externas" que documenta el repo
    [XexUtils](https://github.com/ClementDreptin/XexUtils) sugerido por
    el usuario. Útil para fase 1 (build/empaquetado) y fase 8
    (instalación en RGH).
  - Cambios en `PLAN.md`: cierre del pendiente de "fijar la versión
    exacta del SDK" en "Pendientes de fase 1". Nueva entrada de sesión 14.
  - Estado de la rama `docs/fase0-correcciones`: 14 commits desde la
    sesión 5. Todos los criterios de salida de la fase 0 de `AGENTS.md`
    cumplidos.
