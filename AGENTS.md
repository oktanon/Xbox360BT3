# AGENTS.md: Port de BT3 (PS2) a Xbox 360 RGH

## Fase actual
Estamos en la **fase 0**: sin consola disponible y sin toolchain de Xbox 360 instalado.
Las tareas permitidas son solo de análisis, documentación y preparación:
- Leer y documentar código y estructuras existentes.
- Inventariar dependencias de PS2 y de PC.
- Escribir tests en el host que no requieran hardware.
- Preparar documentos en `docs/port/`.
Criterios de salida de la fase 0 (todos deben cumplirse):
- Toolchain elegido: XDK (Microsoft Xbox 360 SDK), con nombre y versión fijados en la
  sección ## Toolchain fijado. La versión exacta del SDK queda como pendiente de la
  fase 1 (ver `PLAN.md`, "Pendientes de fase 1"). Elegirlo no implica instalarlo ni
  compilar con él durante la fase 0.
- Política de endianness aprobada en docs/port/endian.md, sin TODO(decidir) abiertos.
- Ningún veredicto “Revisar caso por caso” abierto en el inventario.
- Inventario de llamadas y de bloques `#ifdef` con conteos reproducibles y verificados por muestreo.
- Ninguna ruta absoluta en `docs/`.
- No se modificó `src/` ni `port/src/` durante la fase 0.

No está permitido en esta fase: compilar para Xbox 360, generar XEX, subir archivos a la
consola, ni modificar el port de PC para ejecutarlo en 360, instalar el XDK de Xbox 360
(MSVC 2010 + Xbox 360 SDK), descargar cualquier SDK de Xbox 360, o ejecutar cualquier
compilador cruzado para PowerPC, incluso para “verificar” la instalación.

## Objetivo
Portar Dragon Ball Z: Budokai Tenkaichi 3 (PS2, SLUS-21678) a Xbox 360 (PowerPC Xenon,
32 bits, big-endian). El código del juego está en `src/` e `include/` (decompilación).
La capa de plataforma de PC está en `port/` y debe reemplazarse por una de 360.

## Contexto técnico
- PS2: MIPS R5900, little-endian, 32 bits. Xbox 360: PowerPC, big-endian, 32 bits.
- Este repo ya es un port a PC (x86-64, Vulkan). Los cambios de PC están dentro de `#ifdef PORT`.
- El build de PC es el oráculo de comportamiento correcto.
- La aritmética del juego se reproduce exactamente como en PS2. Mantener ese comportamiento
  al usar FPU/VMX de PowerPC (redondeo, enteros con signo, NaN).
- Los datos del juego NO están en el repo y NO deben commitearse: ISO, datos extraídos,
  tablas con valores, o XEX con datos incluidos. El uso es personal, con disco propio.
- Toolchain: XDK (Microsoft Xbox 360 SDK, MSVC 2010). Capa de plataforma: Direct3D 9
  (renderer, shaders `vs_3_0`/`ps_3_0`), XInput (mandos), XAudio2 (sonido), XContent
  (saves), más las APIs de Windows que el XDK expone (`CreateThread`,
  `CRITICAL_SECTION`, `CreateEvent`, etc.). Las APIs de libxenon (drivers bare-metal
  de free60) no se usan en este port. Si en la documentación aparece una referencia
  cruzada a libxenon, se reemplaza por su equivalente del XDK o se marca
  `TODO(decidir-capa)`.
- ABI: la convención de paso y retorno de structs en PowerPC 32 bits no se asume. Se verifica con el toolchain antes de cambiar cualquier bloque `#ifdef` que dependa de eso. Hasta entonces, la afirmación lleva `TODO(verificar-abi)`. En la fase 0, TODO(verificar-abi) se verifica contra la documentación de la ABI del toolchain elegido, sin compilar. Si esa documentación no basta, el TODO se traslada a docs/port/xbox360-todo.md con la etiqueta diferido-fase-1. Los TODO diferidos no bloquean el cierre de la fase 0.
- Caché: los buffers que el port de PC dejó sin caché (`UNCACHED`, p. ej. `movie.c:66`)
  no se consideran resueltos. Requieren decisión de coherencia CPU/GPU.
- Ensamblador: el código MIPS en cadenas `asm` (p. ej. `src/sys/vu0_a_c.c`) no compila
  en PowerPC. Debe quedar fuera del build de 360 y documentado.

## Toolchain fijado
- Nombre: XDK (Microsoft Xbox 360 SDK)
- Versión o commit: <pendiente de fijar; ver "Pendientes de fase 1" en `PLAN.md`>
- Estado: no instalado. La instalación es tarea de la fase 1.
- Decidido en la sesión 5 de `PLAN.md`. Justificación: libxenon (free60) no tiene
  aceleración 2D/3D en la GPU Xenos (verificado en free60.org); el homebrew 3D de
  Xbox 360, incluido este port, usa el XDK para tener Direct3D 9, XInput, XAudio2 y
  XContent.

## Archivos protegidos
- `configure.py`, `install.py` y el build de PC son la referencia del build existente.
  No modificarlos sin consultar antes.
- `src/` e `include/` no se modifican salvo lo indicado en las reglas de abajo.
- `port/src/` y `src/` no se modifican durante la fase 0.

## Reglas de trabajo
1. Código específico de 360 va en `port/src/` o bajo `#ifdef XBOX360`. En `src/` solo se
   tocan lo mínimo necesario (endianness, portabilidad), marcando cada cambio con
   `// XBOX360:` y explicando el motivo.
2. Endianness: todo dato leído del disco (little-endian) pasa por la función de conversión
   acordada en `docs/port/endian.md`. Prohibido hacer cast directo de buffers a structs con
   campos multibyte.
3. Desde la fase 1, un subsistema por vez, en este orden: archivos/memoria, input, renderer, sonido, memory card, películas, lógica restante. En la fase 0 esta regla no aplica.
4. Antes de marcar un ítem como terminado: debe compilar, y debe coincidir con el build de PC
   en el caso de prueba definido (hash de frame o log de estado).
5. Actualizar `docs/port/xbox360-status.md` al terminar cada ítem. Estados válidos: pendiente,
   en curso, compila, verificado en hardware.
6. No inventar direcciones, offsets ni valores de tablas. Si falta un dato, marcar
   `TODO(needs-disc-check)` y preguntar.
7. Funciones no implementadas para 360 llevan `TODO(xbox360)` y deben fallar con un log claro.
8. No inventar comandos de build, empaquetado ni subida. Los comandos reales se definirán en
   `docs/port/xbox360-build.md` (pendiente de la fase 1, con el XDK ya fijado). Si ese archivo
   no existe, no se ejecuta ningún comando de build de 360.
9. No usar el compilador x86 del host para el build de 360.
10. Las esperas activas sobre registros de hardware (`*VIF1_STAT`, `*GIF_STAT`,
    `*D2_CHCR`) no se dan por resueltas. Cada una se marca `TODO(hw-wait)` con su
    condición de salida en Xbox 360.
11. Las correcciones de bugs que el port de PC introdujo (p. ej. `menu_c_c.c:72`) se
    mantienen en 360, salvo decisión explícita registrada en
    `docs/port/xbox360-todo.md`.
12. Las referencias a archivos en documentos son relativas al repo. No se usan
    `file:///` ni rutas de la máquina del usuario.
13. Antes de citar un archivo, función o línea, se verifica que existe (ls, grep).
    Las rutas `src/port/` y `port/src/` se citan como existen, no por suposición.
14. Cada número de un documento (conteos de bloques, llamadas, filas) incluye el
    comando que lo produce. Si no se puede reproducir, no se escribe.
15. Los veredictos usan solo las categorías de la sección "Taxonomía de veredictos".
    "Revisar caso por caso" no es un veredicto: cada bloque así marcado se resuelve
    antes de cerrar la fase 0.
16. Los documentos de `docs/port/` no se recortan para ajustarse a un tamaño. Si
    superan unas 500 líneas, se dividen por subsistema, sin perder contenido.

## Taxonomía de veredictos
- **Reutilizar versión PORT (idéntica)**: el bloque PC se mantiene tal cual en 360.
- **Reutilizar versión PORT (con adaptación)**: el bloque PC se mantiene con cambios
  de plataforma específicos.
- **Revertir a versión PS2 original**: se restaura el comportamiento de PS2 que el port
  de PC había cambiado.
- **Backend propio para 360**: el bloque depende de hardware o API que hay que
  implementar en `port/src/`.
- **Desactivar**: se excluye código de PC sin reemplazo (netplay, ImGui, debug de PC).
- **Opcional**: mods, texturas externas, logs de depuración.
- **Pendiente**: `TODO(...)` con el motivo. Nunca se usa como veredicto final.

## Inventario de pendientes
Mantener `docs/port/xbox360-todo.md` con:
- `grep -rn "#ifdef PORT" src include`
- revisión de `port/src/` buscando stubs y dependencias de PC
- llamadas a hardware PS2 (GS, DMA, SIF, VU0/VU1, pad, memcard, cdvd, sce*)

## Forma de trabajar
- Antes de editar, proponer un plan corto y esperar confirmación.
- Cambios pequeños, una tarea por sesión, con diff revisable.
- Si algo del código no está claro, preguntar antes de asumir.
- Cada hallazgo se documenta con archivo, línea y motivo.
- Los documentos de inventario se leen completos antes de editarlos. No se resumen de memoria.
- Cada tarea termina con un informe: archivos cambiados, comandos ejecutados con su salida
  y `TODO` que quedaron abiertos.

## Mapa de documentos
- `docs/port/xbox360-status.md`: estado de subsistemas y criterios de salida.
- `docs/port/xbox360-todo.md`: inventario de llamadas PS2, bloques `#ifdef`, stubs de `port/src/`.
- `docs/port/endian.md`: política de endianness y accesos a datos de disco.
- `docs/port/endian-schema.md`: esquema de campos por struct (pendiente).
- `docs/port/xbox360-build.md`: comandos reales de build, empaquetado y subida (pendiente, fase 1).
- `port/evidences/`: tests del host que no requieren hardware. Son la evidencia que respalda los conteos y veredictos de docs/port/.
