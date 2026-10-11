# Estado del Port a Xbox 360

Seguimiento del estado de los subsistemas para el port de Dragon Ball Z: Budokai Tenkaichi 3 (PS2, SLUS-21678) a Xbox 360 (PowerPC Xenon, 32 bits, big-endian), según las reglas definidas en `AGENTS.md`.

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
- [x] Inventario inicial de dependencias y llamadas a hardware PS2 ([docs/port/xbox360-todo.md](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/docs/port/xbox360-todo.md)).
- [x] Especificación de política de conversión de endianness ([docs/port/endian.md](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/docs/port/endian.md)).
- [x] Auditoría exhaustiva de los 191 bloques `#ifdef PORT` / `#ifndef PORT` en `src/` e `include/` con veredicto técnico para PowerPC Xenon / 360 ([docs/port/xbox360-todo.md#21-desglose-exhaustivo-de-bloques-ifdef-port--ifndef-port-src-e-include](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/docs/port/xbox360-todo.md#L504)).
- [x] Inventario exhaustivo de interfaces en `port/src/`: dependencias de Vulkan, SDL y APIs de PC con veredicto técnico para Xbox 360 ([docs/port/xbox360-todo.md#31-desglose-detallado-de-interfaces-vulkan-sdl-y-apis-de-pc](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/docs/port/xbox360-todo.md#L746)).
- [x] Suite de tests unitarios de conversión de endianness en el host (Python) para enteros de 16, 32 y 64 bits y float32 con casos de borde ([port/tests/endian/](file:///c:/Users/Usuario/Desktop/Xbox360BT3/Xbox360BT3/port/tests/endian/)).


