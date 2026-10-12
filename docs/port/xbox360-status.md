# Estado del Port a Xbox 360

Seguimiento del estado de los subsistemas para el port de Dragon Ball Z: Budokai Tenkaichi 3 (PS2, SLUS-21678) a Xbox 360 (PowerPC Xenon, 32 bits, big-endian), según las reglas definidas en `AGENTS.md`.

Toolchain decidido (sesión 5): **XDK** (Microsoft Xbox 360 SDK: Direct3D 9, XInput, XAudio2, XContent). Detalle de tareas en `PLAN.md` (raíz del repo), sección "Hoja de ruta de fases".

Build commands documentados (subtarea 1.1 verificada): [docs/port/xbox360-build.md](./xbox360-build.md).

## Hoja de ruta de fases

| Fase | Nombre | Subsistemas (regla 3) | Estado | Criterio de salida |
| :--: | :--- | :--- | :--- | :--- |
| 0 | Análisis y documentación | — | en curso | Toolchain fijado por escrito; 0 veredictos "Revisar caso por caso"; 0 rutas absolutas en `docs/`; esqueleto de `docs/port/endian-schema.md` |
| 1 | Toolchain y esqueleto ejecutable | — | en curso | XEX que arranca y llega con log hasta `Gfx_Init()` |
| 2 | Archivos, memoria e input | 1, 2 | pendiente | Recursos cargados con valores correctos (logs); input XInput verificado |
| 3 | Renderer | 3 | pendiente | Menú navegable y batalla renderizada, comparables con el build PC (hash/captura) |
| 4 | Sonido | 4 | pendiente | Música, voces y SFX correctos (XAudio2) |
| 5 | Memory Card | 5 | pendiente | Guardar/cargar partida y replay con checksum válido |
| 6 | Películas | 6 | pendiente | Opening y cutscenes reproduciéndose con sincronía A/V |
| 7 | Lógica restante y verificación | 7 | pendiente | Paridad funcional con el build PC (mismas entradas → mismo log de estado) |
| 8 | Optimización y empaquetado | — | pendiente | Release instalable en RGH con guía de usuario |

Nota de arquitectura: Xbox 360 es 32 bits nativo, por lo que la maquinaria de punteros del port de PC (`port/tools/ptr32.py`, mapeo bajo 4 GB de `port/src/plat_mem.c`) no es necesaria; las direcciones PS2 caben directas en el espacio plano de 32 bits. El trabajo se concentra en endianness big-endian y en la capa de plataforma.

## Referencias externas

- [XexUtils](https://github.com/ClementDreptin/XexUtils) (Clement Dréptin): utilidades para el formato XEX (ejecutables de Xbox 360) — información, extracción de XEXP y otros helpers. Útil para la fase 1 (configuración del build, empaquetado) y la fase 8 (instalación en RGH). Sugerido por el usuario en la sesión 14 de `PLAN.md`.

## Estados válidos (Regla 5)
- **pendiente**: No iniciado.
- **en curso**: En desarrollo o adaptación activa.
- **compila**: Compila para la plataforma objetivo sin errores.
- **verificado en hardware**: Probado y con comportamiento verificado en hardware real Xbox 360 / RGH.

## Tabla de subsistemas (Regla 3)

Los subsistemas se abordan estrictamente en el orden establecido por la Regla 3 de `AGENTS.md`:

| Orden | Subsistema | Estado | Referencia PC (`port/src/`) | Descripción / Alcance |
| :---: | :--- | :---: | :--- | :--- |
| 1 | Archivos / Memoria | pendiente | `port/src/plat_file.c`<br>`port/src/plat_fcache.c`<br>`port/src/plat_mem.c` | Sistema de archivos CRI ADXF sobre almacenamiento de 360, gestión del heap y memoria fija de PS2 (32 MB mapeados en PowerPC Xenon), conversiones de endianness para datos leídos del disco. |
| 2 | Input | pendiente | `port/src/gs/gs_input.c` | Mapeo de mandos Xbox 360 (XInput) al buffer de pad PS2 (`libpad2` en `src/sys/pad.c`). |
| 3 | Renderer | pendiente | `port/src/plat_gs.c`<br>`port/src/gs_marker.c`<br>`port/src/gs/gs_core.c`<br>`port/src/gs/gs_draw.c`<br>`port/src/gs/gs_gpu.c`<br>`port/src/gs/gs_gl.c`<br>`port/src/gs/gs_sony.c`<br>`port/src/gs/gs_vu1.c`<br>`port/src/gs/gs_texpack.c` | Capa gráfica: emulación de GS / VIF1 / GIF, microprogramas VU1 y pipelines de renderizado adaptados a Direct3D 9 / GPU Xenos. |
| 4 | Sonido | pendiente | `port/src/gs/snd_adx.c`<br>`port/src/gs/snd_se.c`<br>`port/src/plat_sndstate.c`<br>`port/src/plat_songs.c` | Decodificación y reproducción de streams de audio CRI ADX y efectos de sonido (sampler SOUNDS.IRX reemplazado por backend de audio XAudio2). |
| 5 | Memory Card | pendiente | `port/src/plat_mc.c` | Emulación de Sony `libmc` guardando en disco duro o dispositivo de memoria de Xbox 360. |
| 6 | Películas | pendiente | `port/src/plat_movie.c`<br>`port/src/plat_mpeg2.c` | Reproducción de secuencias de video PSS/MPEG-2 (`sceMpeg*`). |
| 7 | Lógica restante | pendiente | `port/src/headless.c`<br>`port/src/plat_sys.c`<br>`port/src/plat_stub.c`<br>`port/src/softfloat_ps2.c`<br>`port/src/mathf_pc.c`<br>`port/src/plat_libm.c`<br>`port/src/plat_crash.c`<br>`port/src/plat_stages.c`<br>`port/src/plat_extras.c`<br>`port/src/plat_settings.c`<br>`port/src/plat_fastvec.c` | Aritmética de punto flotante PS2 (FPU/VMX), temporización / VBlank, stubs de syscalls del kernel PS2, inicialización y harness de pruebas. |

## Progreso de Fase 0 (Análisis y Documentación)
- [x] Inventario inicial de dependencias y llamadas a hardware PS2 ([docs/port/xbox360-todo.md](./xbox360-todo.md)).
- [x] Especificación de política de conversión de endianness ([docs/port/endian.md](./endian.md)).
- [x] Auditoría exhaustiva de los 191 bloques `#ifdef PORT` / `#ifndef PORT` en `src/` e `include/` con veredicto técnico para PowerPC Xenon / 360 ([docs/port/xbox360-todo.md#21-desglose-exhaustivo-de-bloques-ifdef-port--ifndef-port-src-e-include](./xbox360-todo.md#L504)).
- [x] Inventario exhaustivo de interfaces en `port/src/`: dependencias de Vulkan, SDL y APIs de PC con veredicto técnico para Xbox 360 ([docs/port/xbox360-todo.md#31-desglose-detallado-de-interfaces-vulkan-sdl-y-apis-de-pc](./xbox360-todo.md#L746)).
- [x] Suite de tests unitarios de conversión de endianness en el host (Python) para enteros de 16, 32 y 64 bits y float32 con casos de borde ([port/tests/endian/](../../port/tests/endian)).


