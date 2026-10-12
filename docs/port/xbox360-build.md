# Comandos de Build, Empaquetado y Subida (Xbox 360)

Receta real de build para el port a Xbox 360 de Dragon Ball Z: Budokai
Tenkaichi 3 (PS2, SLUS-21678). Esta pagina se rellena progresivamente durante
la fase 1; cada bloque tiene la salida verificada del comando que se cita.

Criterio de salida de la fase 1 (segun `docs/port/xbox360-status.md`):
> XEX que arranca y llega con log hasta `Gfx_Init()`.

Restriccion de la regla 8 de `AGENTS.md`: hasta que esta pagina exista, no se
ejecuta ningun comando de build de 360 fuera de los aqui documentados.

---

## 1. Toolchain fijado

- Nombre: XDK (Microsoft Xbox 360 SDK)
- Version: 21256.3 (ultima oficial del XDK 2010, Q4 2010)
- IDE: Microsoft Visual Studio 2010 Professional (EN-US)
- Paths en este host (verificados en sesion 14 de `PLAN.md` y en
  `docs/port/xbox360-status.md`):
  - XDK: `C:\Program Files (x86)\Microsoft Xbox 360 SDK`
  - VS 2010: `C:\Program Files (x86)\Microsoft Visual Studio 10.0`

Componentes verificados (salida real de los comandos de la seccion 2):

| Componente | Version / tamano | Origen |
| :-- | :-- | :-- |
| `cl.exe` (cross-compiler PowerPC) | 16.00.11886.00 | `%XEDK%\bin\win32\cl.exe` (PowerPC, despues de `xdkvars.bat`) |
| `link.exe` (linker PowerPC) | 10.00.11886.00 | `%XEDK%\bin\win32\link.exe` |
| `imagexex.exe` (XEX image builder) | 2.0.21256.0 | `bin\win32\imagexex.exe` |
| `xbcp.exe` (Xbox 360 file copy) | 2.0.21256.0 | `bin\win32\xbcp.exe` |
| `fxc.exe` (shader compiler) | 5.2.3790.1830 | `bin\win32\fxc.exe` |
| `xactbld3.exe` (XACT sound bank) | 9.28.1876.0 | `bin\win32\xactbld3.exe` |
| Plataforma MSBuild "Xbox 360" | registrada | `C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\Platforms\Xbox 360\` |
| Wizards de proyecto "Xbox 360" | registrados | `C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\VCProjects\Xbox360*.vsz` |

Decidido en sesion 5 de `PLAN.md`. Justificacion: libxenon (free60) no tiene
aceleracion 2D/3D en la GPU Xenos; el homebrew 3D de Xbox 360, incluido este
port, usa el XDK para tener Direct3D 9, XInput, XAudio2 y XContent.

---

## 2. Configuracion del entorno

El XDK 21256.3 expone dos `.bat` que se llaman desde una consola cmd:

1. `vcvars32.bat` (de VS 2010) pone en el path el compilador y el linker x86
   que, en presencia del XEDK, se convierten en el cross-compiler PowerPC.
2. `xdkvars.bat` (del XDK) antepone `%XEDK%\bin\win32` al `PATH` y anade
   `%XEDK%\include\xbox`, `%XEDK%\include\xbox\sys` y `%XEDK%\include\win32`
   a `INCLUDE`, y `%XEDK%\lib\xbox` y `%XEDK%\lib\win32` a `LIB`.

Notas verificadas durante la subtarea 1.1 (fase 1):

- `XEDK` no estaba en el registro en este host. Hay que fijarlo a mano antes
  de llamar a `xdkvars.bat`; de lo contrario la expansion de `%XEDK%` deja un
  espacio final que rompe las rutas (`%XEDK%\include\xbox` ->
  `...\Xbox 360 SDK \include\xbox`).
- `xdkvars.bat` pone `%XEDK%\include\win32` ANTES que `%XEDK%\include\xbox`.
  Para el target Xbox 360 se reordena `INCLUDE` para que `xbox\sys` y
  `xbox` queden por delante de `win32`. Sin ese reorden, el compilador toma
  la version "host" de `xnamath.h` (SSE) en lugar de la version "Xbox 360"
  (VMX128) y aborta con `#error : This version of xnamath.h does not
  support Xbox 360` (reproducido en sesion 14).
- `_XBOX` lo define el propio MSBuild al elegir plataforma "Xbox 360"
  (`Microsoft.Cpp.Xbox 360.Title.props`); al compilar desde la linea de
  comandos hay que pasarlo a mano: `cl /D_XBOX ...`.
- `_XM_VMX128_INTRINSICS_` lo define `XEDK\include\xbox\xnamath.h`. No hay
  que pasarlo en `cl`; si se pasa sale un warning de redefinicion (reproducido
  en sesion 14).

Plantilla de script (la usada para la verificacion de 1.1, se generalizara
en 1.2 cuando exista el proyecto `.vcxproj`):

```bat
@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat" >nul
set "XEDK=C:\Program Files (x86)\Microsoft Xbox 360 SDK"
call "%XEDK%\bin\win32\xdkvars.bat" >nul
set "INCLUDE=%XEDK%\include\xbox;%XEDK%\include\xbox\sys;%XEDK%\include\win32;%XEDK%\include\shared;%INCLUDE%"
```

> `cmd /c "... && set INCLUDE=... && cl ..."` no funciona: la expansion de
> `%INCLUDE%` despues de `&&` la ve el shell exterior como vacia. Usar un
> `.cmd` aparte o mover el `set` a una linea anterior sin `&&`.

---

## 3. Comprobacion del toolchain (subtarea 1.1)

Smoke test reproducible: se compila y enlaza un `test.c` vacio en
`%TEMP%\opencode\xbox360-toolchain-test\`, se genera el XEX y se dumpean los
headers con `imagexex /DUMP`. Resultado real (sesion 14):

```text
$ cl /c /EHsc /W3 /O2 /Zi /nologo /D_XBOX /Tp test.c
test.c
c:\program files (x86)\microsoft xbox 360 sdk\include\xbox\xnamath.h(50) : warning C4005: '_XM_VMX128_INTRINSICS_' : macro redefinition
        command-line arguments : see previous definition of '_XM_VMX128_INTRINSICS_'
test.obj 67808 bytes
vc100.pdb 299008 bytes

$ link /OUT:test.exe /SUBSYSTEM:XBOX /LIBPATH:"%XEDK%\lib\xbox" xboxkrnl.lib xapilib.lib test.obj
Microsoft (R) Incremental Linker Version 10.00.11886.00
test.exe 102400 bytes
test.xdb 1625088 bytes

$ imagexex /IN:test.exe /OUT:test.xex
Microsoft(R) Xbox 360 Image File Builder Version 2.0.21256.0
test.xex 102400 bytes

$ imagexex /DUMP test.xex
FILE HEADER VALUES
          1 module flags
              title module
   82000000 load address
      30000 image size
   FFFFFFFF game region
              North America / Japan / China / rest of Asia / Australia/New Zealand / rest of Europe / rest of world
          0 image flags
              64KB pages
    8000605 allowed media types
              hard disk / DVD/CD / SMB filesystem / direct-from-RAM / Live-signed package
   82000000 image base address
   820103A0 entry point
   00000000 image checksum
   6ACC4E31 image timestamp
         40 number of TLS slots
   00040000 default stack size
   NOT-COMPRESSED, ENCRYPTED
```

Conclusiones de la subtarea 1.1:

- El cross-compiler `cl.exe` para PowerPC Xenon arranca y produce objetos
  `test.obj` a partir de C con `<xtl.h>`.
- El linker `link.exe` para PowerPC produce `test.exe` enlazando contra
  `xboxkrnl.lib` y `xapilib.lib`.
- `imagexex.exe` convierte el PE a XEX (`test.xex`) y lo dumpea con un
  load address `0x82000000` y entry point `0x820103A0` consistentes con
  el espacio de direcciones de Xenos.
- Los headers, las libs y los wizards de proyecto de MSVC 2010 estan en su
  sitio; la plataforma "Xbox 360" esta registrada en MSBuild.

Estado: toolchain validado. Pendiente de la subtarea 1.2: crear el proyecto
`.vcxproj` que use estas piezas para compilar `src/main.c` y los stubs de
`port/src/xbox360/`.

---

## 4. Comandos por subtarea (a rellenar)

| Subtarea | Comando | Salida esperada | Estado |
| :-- | :-- | :-- | :--: |
| 1.1 toolchain | ver seccion 3 | XEX valido de smoke test | compila |
| 1.2 proyecto | ver seccion 5 | `.xex` con `_XBOX` definido y entry point en `0x820103A8` | compila |
| 1.3 stubs | enlazado desde el `.vcxproj` (1.2) | sin errores de simbolo `sce*` no definido | pendiente |
| 1.4 main.c + bootstrap | idem 1.2 | `.xex` que contiene `main` y los stubs de `Game_Main` | pendiente |
| 1.5 verificacion final | `imagexex /DUMP` + log de ejecucion | entrada en `Gfx_Init` antes de cuelgue | pendiente |

Los comandos exactos de las subtareas 1.3 a 1.5 se anadiran aqui cuando se
ejecuten, con la salida real. Hasta entonces no se ejecuta ningun comando
de build de 360 (regla 8 de `AGENTS.md`).

---

## 5. Proyecto MSVC 2010 (subtarea 1.2)

Archivos del proyecto (todos versionados, build outputs ignorados por
`.gitignore` en `/port/build/`):

- `port/build/xbox360/BT3.props` — property sheet: fija `<XDKInstallDir>` a
  `C:\Program Files (x86)\Microsoft Xbox 360 SDK\` (evita depender de la
  clave de registro `HKLM\SOFTWARE\Microsoft\Xbox\2.0\SDK`, que no esta
  en este host), `<XEDK>` y el orden de `<IncludePath>` y `<LibraryPath>`.
- `port/build/xbox360/BT3.xbox360.vcxproj` — proyecto C++ para plataforma
  `Xbox 360` (PlatformShortName `xbox`), toolset `2010-01`,
  `ConfigurationType=Application`. Importa `BT3.props` y la plataforma
  Xbox 360 (`Microsoft.Cpp.Xbox 360.props`).
- `port/build/xbox360/BT3.sln` — solucion VS 2010 con dos configuraciones:
  `Debug|Xbox 360` y `Release|Xbox 360`.
- `port/src/xbox360/xbr_main.c` — entry point minimo. En 1.3-1.4 se
  sustituye por `Xbr_Init()` y la llamada a `Game_Main()`.

### 5.1. Comando de build

El toolset `2010-01` ya pone `$(XDKInstallDir)\bin\win32` en
`$(ExecutablePath)`, asi que MSBuild encuentra `cl.exe`, `link.exe` e
`imagexex.exe` del XDK sin necesidad de `xdkvars.bat`. Hace falta MSBuild
**32-bit** (el assembly `Microsoft.Xna.Xbox360.Build.dll` que provee las
tareas `CL`, `Link` e `ImageXex` es 32-bit):

```bat
:: cmd /c desde la raiz del repo
"C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe" ^
  "port\build\xbox360\BT3.xbox360.vcxproj" ^
  /t:Build ^
  /p:Configuration=Release ^
  /p:Platform="Xbox 360" ^
  /nologo
```

Si se quiere abrir la solucion en VS 2010 directamente, `devenv` encuentra
el toolset por su cuenta y no hace falta MSBuild manual.

### 5.2. Salida verificada (Release)

Build limpio (rebuild) con el comando de la seccion 5.1:

```text
ClCompile:
  C:\Program Files (x86)\Microsoft Xbox 360 SDK\bin\win32\CL.exe /c /Zi /nologo /W3 /WX- /O2
    /D XBOX360 /D _XBOX /D _MBCS /GF- /Gm- /EHsc /MT /GS /Gy-
    /fp:fast /fp:except- /Zc:wchar_t /Zc:forScope /GR- /openmp-
    /Fo"...\port\build\xbox360\obj\Release\\"
    /Fd"...\port\build\xbox360\obj\Release\vc100.pdb"
    /TP ..\..\src\xbox360\xbr_main.c
    /FI"C:\Program Files (x86)\Microsoft Xbox 360 SDK\include\xbox\xbox_intellisense_platform.h"
  xbr_main.c
Link:
  C:\Program Files (x86)\Microsoft Xbox 360 SDK\bin\win32\link.exe
    /OUT:"...\port\build\xbox360\bin\Release\BT3.exe"
    /PDB:"...\port\build\xbox360\bin\Release\BT3.pdb"
    /IMPLIB:"...\port\build\xbox360\bin\Release\BT3.lib"
    xinput2.lib xaudio2.lib d3d9.lib d3dx9.lib
    obj\Release\xbr_main.obj /XEX:NO
ImageXex:
  C:\Program Files (x86)\Microsoft Xbox 360 SDK\bin\win32\imagexex.exe
    /out:"...\port\build\xbox360\bin\Release\BT3.xex"
    "...\port\build\xbox360\bin\Release\BT3.exe"
  Microsoft(R) Xbox 360 Image File Builder Version 2.0.21256.0

Compilacion correcta. 0 Advertencia(s) 0 Errores. Tiempo 00:00:01.68
```

### 5.3. Verificacion del XEX

Tras el build, `imagexex /DUMP` confirma la cabecera del XEX (salida
real, sesion 14):

```text
$ imagexex /DUMP port\build\xbox360\bin\Release\BT3.xex
   82000000 load address
      30000 image size
   820103A8 entry point
   00040000 default stack size
              title module / 64KB pages
              North America / Japan / China / rest of Asia / Australia/New Zealand / rest of Europe / rest of world
              hard disk / DVD/CD / SMB filesystem / direct-from-RAM / Live-signed package
   NOT-COMPRESSED, ENCRYPTED
```

`Debug` produce un XEX valido con `image size = 0x40000` (256 KB) y mismo
load address y entry point; el resto de campos coinciden.

### 5.4. Layout de salida

Para `Configuration=Release` (idem Debug con sufijo de carpeta):

```text
port/build/xbox360/
├── bin/Release/
│   ├── BT3.exe          # PE generado por link.exe
│   ├── BT3.xex          # XEX generado por imagexex.exe (artefacto final)
│   ├── BT3.pdb          # PDB estandar de MSVC
│   ├── BT3.xdb          # simbolos de depuracion de Xbox 360
│   └── BT3.lib          # import lib (por si otro proyecto lo linkea)
└── obj/Release/
    ├── xbr_main.obj     # codigo objeto de port/src/xbox360/xbr_main.c
    ├── vc100.pdb        # PDB incremental de MSVC
    └── *.tlog           # tracking logs de MSBuild
```

Los directorios `bin\` y `obj\` estan en `.gitignore` (`/port/build/`
y `/build/` en la linea 7 y 15 de `.gitignore`).

### 5.5. Hallazgos durante 1.2

- **MSBuild 64-bit no sirve.** La tarea `ImageXex` viene de
  `Microsoft.Xna.Xbox360.Build.dll`, que es 32-bit. Hay que usar
  `C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe` (32-bit) y
  NO el de `Framework64`. El error es "Se ha intentado cargar un programa
  con un formato incorrecto."
- **El toolset Xbox 360 importa el archivo del toolset a su vez.** No
  hay que importar `Microsoft.Cpp.Xbox 360.PlatformToolset.props` a mano;
  ya lo hace `Microsoft.Cpp.Xbox 360.props` (linea 16 del .props del
  toolset). Importarlo dos veces produce `MSB4011`.
- **`<SubSystem>` no existe para Xbox 360.** El `link.xml` de la
  plataforma dice literalmente "Xbox 360: Removed SubSystem" (linea 11).
  El link.exe de PowerPC toma `/SUBSYSTEM:XBOX` por defecto; poner
  `<SubSystem>XBOX</SubSystem>` en el .vcxproj falla con
  `error : Element <SubSystem> has an invalid value of "XBOX"`.
- **`<OutputFile>` se evalua en `Microsoft.Cpp.Xbox 360.props`, antes de
  los `<PropertyGroup>` de configuracion del .vcxproj.** Si se quiere
  cambiar el sitio del `.exe` y el `.xex`, hay que fijar `<OutputFile>` y
  `<ImageXexOutput>` directamente en el PropertyGroup de la
  configuracion; `<OutDir>` solo no basta (los PDBs y la implib usan
  OutDir, pero el .exe/.xex no).
