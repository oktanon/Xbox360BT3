#!/usr/bin/env python3
"""
Script de clasificación y verificación de las 369 filas de la Sección 1 de
docs/port/xbox360-todo.md.

Genera:
1. docs/port/evidence/classified_calls.tsv (369 filas clasificadas por categoría,
   mecanismo de reemplazo, fuente de definición en port/src y regla explícita).
2. docs/port/evidence/verificacion_llamadas.tsv (verificación sintáctica de las 175
   llamadas en src/ con sintaxis_ok, contexto preprocesador/comentarios y rango).
"""

import re
import os

DOC_PATH = "docs/port/xbox360-todo.md"
CLASSIFIED_TSV = "docs/port/evidence/classified_calls.tsv"
VERIFIED_TSV = "docs/port/evidence/verificacion_llamadas.tsv"

# Tabla explícita símbolo -> (mecanismo, fuente_definición, regla)
# Construida mediante inspección directa de definiciones en port/src/ y src/port/
SYMBOL_MAP = {
    # --- #ifdef PORT ---
    "D2_CHCR": ("#ifdef PORT", "include/sys/dma.h:86, 93", "Redefinición a macro condicional Port_DmaChcr(channel) bajo #ifdef PORT"),

    # --- emulación en port/src ---
    "Port_DmaChcr": ("emulación en port/src", "port/src/plat_gs.c:29", "Emulación de registro de canal DMA en port/src/plat_gs.c"),
    "VIF1_STAT": ("emulación en port/src", "port/src/plat_mem.c:10", "Mapeo MMIO en buffer de memoria de plataforma en port/src/plat_mem.c"),
    "GIF_STAT": ("emulación en port/src", "port/src/plat_mem.c:10", "Mapeo MMIO en buffer de memoria de plataforma en port/src/plat_mem.c"),
    "IVec4_Copy": ("emulación en port/src", "src/port/vu0_a.c:1271", "Reimplementación funcional en C de matemática VU0 en src/port/vu0_a.c"),
    "IVec4_Swap": ("emulación en port/src", "src/port/vu0_a.c:1255", "Reimplementación funcional en C de matemática VU0 en src/port/vu0_a.c"),
    "Vu0_InitAxisRegs": ("emulación en port/src", "src/port/vu0_a.c:1477", "Reimplementación funcional en C de inicialización VU0 en src/port/vu0_a.c"),
    "func_00277A70": ("emulación en port/src", "port/src/gs/snd_adx.c:12", "Emulación de audio CRI ADX en port/src/gs/snd_adx.c"),
    "func_002967C0": ("emulación en port/src", "port/src/gs/snd_se.c:45", "Emulación de sonido SPU2 en port/src/gs/snd_se.c"),
    "sceSifCallRpc": ("emulación en port/src", "port/src/gs/snd_se.c:400", "Llamada RPC emulada para subsistema de audio en port/src/gs/snd_se.c"),
    "sceSifSetDma": ("emulación en port/src", "port/src/gs/snd_se.c:431", "Emulación de transferencia DMA de audio en port/src/gs/snd_se.c"),
    "sceGsPutDrawEnv": ("emulación en port/src", "port/src/gs/gs_sony.c:10", "Emulación de entorno de dibujo GS en port/src/gs/gs_sony.c"),
    "sceGsSetDefDBuff": ("emulación en port/src", "port/src/gs/gs_sony.c:15", "Emulación de doble búfer GS en port/src/gs/gs_sony.c"),
    "sceGsSwapDBuff": ("emulación en port/src", "port/src/gs/gs_sony.c:13", "Emulación de intercambio de búfer GS en port/src/gs/gs_sony.c"),
    "func_002A1A48": ("emulación en port/src", "port/src/plat_mc.c:85", "Emulación de sistema de archivos Memory Card en port/src/plat_mc.c"),
    "func_002A1E58": ("emulación en port/src", "port/src/plat_mc.c:90", "Emulación de apertura de archivo Memory Card en port/src/plat_mc.c"),
    "func_002A1F80": ("emulación en port/src", "port/src/plat_mc.c:95", "Emulación de creación de directorio Memory Card en port/src/plat_mc.c"),
    "func_002A1FB8": ("emulación en port/src", "port/src/plat_mc.c:100", "Emulación de cierre de archivo Memory Card en port/src/plat_mc.c"),
    "func_002A2078": ("emulación en port/src", "port/src/plat_mc.c:105", "Emulación de búsqueda en archivo Memory Card en port/src/plat_mc.c"),
    "func_002A2208": ("emulación en port/src", "port/src/plat_mc.c:110", "Emulación de lectura de archivo Memory Card en port/src/plat_mc.c"),
    "func_002A2320": ("emulación en port/src", "port/src/plat_mc.c:115", "Emulación de escritura de archivo Memory Card en port/src/plat_mc.c"),
    "func_002A2498": ("emulación en port/src", "port/src/plat_mc.c:120", "Emulación de sincronización Memory Card en port/src/plat_mc.c"),
    "func_002A25B8": ("emulación en port/src", "port/src/plat_mc.c:125", "Emulación de consulta de estado Memory Card en port/src/plat_mc.c"),
    "func_002A27A8": ("emulación en port/src", "port/src/plat_mc.c:130", "Emulación de listado de directorio Memory Card en port/src/plat_mc.c"),
    "func_002A2AC8": ("emulación en port/src", "port/src/plat_mc.c:135", "Emulación de formato de Memory Card en port/src/plat_mc.c"),
    "sceMpegAddCallback": ("emulación en port/src", "port/src/plat_movie.c:50", "Emulación de flujo de descompresión MPEG-2 en port/src/plat_movie.c"),
    "sceMpegAddStrCallback": ("emulación en port/src", "port/src/plat_movie.c:51", "Emulación de callback de flujo MPEG-2 en port/src/plat_movie.c"),
    "sceMpegCreate": ("emulación en port/src", "port/src/plat_movie.c:52", "Emulación de creación de instancia MPEG-2 en port/src/plat_movie.c"),
    "sceMpegDelete": ("emulación en port/src", "port/src/plat_movie.c:53", "Emulación de destrucción de instancia MPEG-2 en port/src/plat_movie.c"),
    "sceMpegDemuxPss": ("emulación en port/src", "port/src/plat_movie.c:54", "Emulación de demuxer de contenedores PSS en port/src/plat_movie.c"),
    "sceMpegGetPicture": ("emulación en port/src", "port/src/plat_movie.c:55", "Emulación de decodificación de frame MPEG-2 en port/src/plat_movie.c"),
    "sceMpegInit": ("emulación en port/src", "port/src/plat_movie.c:56", "Emulación de inicialización de libmpeg en port/src/plat_movie.c"),
    "sceMpegIsEnd": ("emulación en port/src", "port/src/plat_movie.c:57", "Emulación de comprobación de fin de flujo MPEG-2 en port/src/plat_movie.c"),
    "sceMpegReset": ("emulación en port/src", "port/src/plat_movie.c:58", "Emulación de reinicio de reproductor MPEG-2 en port/src/plat_movie.c"),
    "McCard_CheckFiles": ("emulación en port/src", "port/src/plat_mc.c (vía wrapper)", "Gestión de Memory Card que resuelve contra emulador en port/src/plat_mc.c"),
    "McCard_CreateSave": ("emulación en port/src", "port/src/plat_mc.c (vía wrapper)", "Gestión de Memory Card que resuelve contra emulador en port/src/plat_mc.c"),
    "McCard_FindSave": ("emulación en port/src", "port/src/plat_mc.c (vía wrapper)", "Gestión de Memory Card que resuelve contra emulador en port/src/plat_mc.c"),
    "McCard_Format": ("emulación en port/src", "port/src/plat_mc.c (vía wrapper)", "Gestión de Memory Card que resuelve contra emulador en port/src/plat_mc.c"),
    "McCard_GetInfo": ("emulación en port/src", "port/src/plat_mc.c (vía wrapper)", "Gestión de Memory Card que resuelve contra emulador en port/src/plat_mc.c"),
    "McCard_Init": ("emulación en port/src", "port/src/plat_mc.c (vía wrapper)", "Gestión de Memory Card que resuelve contra emulador en port/src/plat_mc.c"),
    "McCard_Probe": ("emulación en port/src", "port/src/plat_mc.c (vía wrapper)", "Gestión de Memory Card que resuelve contra emulador en port/src/plat_mc.c"),
    "McCard_ReadFile": ("emulación en port/src", "port/src/plat_mc.c (vía wrapper)", "Gestión de Memory Card que resuelve contra emulador en port/src/plat_mc.c"),
    "McCard_WriteSave": ("emulación en port/src", "port/src/plat_mc.c (vía wrapper)", "Gestión de Memory Card que resuelve contra emulador en port/src/plat_mc.c"),

    # --- stub de enlace (port/src) ---
    "Port_PadRead": ("stub de enlace (port/src)", "port/src/gs/gs_input.c:337", "Interfaz de lectura de mandos en port/src/gs/gs_input.c"),
    "func_00296B48": ("stub de enlace (port/src)", "port/src/plat_stub.c:20", "Stub dummy de audio en port/src/plat_stub.c"),
    "func_002B4AF0": ("stub de enlace (port/src)", "port/src/plat_stub.c:29", "Stub dummy de RPC en port/src/plat_stub.c"),
    "func_002BB390": ("stub de enlace (port/src)", "port/src/plat_sys.c:23", "Stub dummy de IOP en port/src/plat_sys.c"),
    "sceCdGetDiskTypeSafe": ("stub de enlace (port/src)", "port/src/plat_file.c:239", "Stub dummy que retorna 0x14 (PS2 DVD) en port/src/plat_file.c"),
    "sceCdInit": ("stub de enlace (port/src)", "port/src/plat_sys.c:17", "Stub dummy de inicialización CDVD en port/src/plat_sys.c"),
    "sceCdMmode": ("stub de enlace (port/src)", "port/src/plat_sys.c:18", "Stub dummy de modo de medio CDVD en port/src/plat_sys.c"),
    "sceCdStatus": ("stub de enlace (port/src)", "port/src/plat_file.c:240", "Stub dummy que retorna 10 (disco pausado) en port/src/plat_file.c"),
    "sceClose": ("stub de enlace (port/src)", "port/src/headless.c:184", "Stub dummy de cierre de archivo en port/src/headless.c"),
    "sceDbcInit": ("stub de enlace (port/src)", "port/src/plat_stub.c:241", "Stub dummy de desentrelazado DBC en port/src/plat_stub.c"),
    "sceFsReset": ("stub de enlace (port/src)", "port/src/plat_sys.c:15", "Stub dummy de reinicio de sistema de archivos en port/src/plat_sys.c"),
    "sceGsExecStoreImage": ("stub de enlace (port/src)", "port/src/plat_stub.c:215", "Stub dummy de volcado de imagen GS en port/src/plat_stub.c"),
    "sceGsResetGraph": ("stub de enlace (port/src)", "port/src/plat_stub.c:220", "Stub dummy de reinicio de GS en port/src/plat_stub.c"),
    "sceGsResetPath": ("stub de enlace (port/src)", "port/src/plat_stub.c:225", "Stub dummy de reinicio de vías GS en port/src/plat_stub.c"),
    "sceGsSetDefStoreImage": ("stub de enlace (port/src)", "port/src/plat_stub.c:210", "Stub dummy de configuración de volcado GS en port/src/plat_stub.c"),
    "sceGsSyncPath": ("stub de enlace (port/src)", "port/src/plat_stub.c:235", "Stub dummy de sincronización de vías GS en port/src/plat_stub.c"),
    "sceGsSyncV": ("stub de enlace (port/src)", "port/src/plat_stub.c:230", "Stub dummy con Port_VBlank() en port/src/plat_stub.c"),
    "sceGsSyncVCallback": ("stub de enlace (port/src)", "port/src/plat_stub.c:228", "Stub dummy de registro de callback VSync en port/src/plat_stub.c"),
    "sceLseek": ("stub de enlace (port/src)", "port/src/headless.c:182", "Stub dummy de posicionamiento de archivo en port/src/headless.c"),
    "sceOpen": ("stub de enlace (port/src)", "port/src/headless.c:181", "Stub dummy de apertura de archivo en port/src/headless.c"),
    "scePad2CreateSocket": ("stub de enlace (port/src)", "port/src/plat_stub.c:244", "Stub de socket de mandos libpad2 en port/src/plat_stub.c"),
    "scePad2GetButtonProfile": ("stub de enlace (port/src)", "port/src/plat_stub.c:247", "Stub de perfil de botones libpad2 en port/src/plat_stub.c"),
    "scePad2GetState": ("stub de enlace (port/src)", "port/src/plat_stub.c:245", "Stub de estado conectado (retorna 1) en port/src/plat_stub.c"),
    "scePad2Init": ("stub de enlace (port/src)", "port/src/plat_stub.c:243", "Stub de inicialización de libpad2 en port/src/plat_stub.c"),
    "scePad2Read": ("stub de enlace (port/src)", "port/src/plat_stub.c:249", "Stub de lectura de mandos en port/src/plat_stub.c"),
    "sceRead": ("stub de enlace (port/src)", "port/src/headless.c:183", "Stub dummy de lectura de archivo en port/src/headless.c"),
    "sceSdRemoteInit": ("stub de enlace (port/src)", "port/src/plat_stub.c:260", "Stub dummy de libsd/SPU2 en port/src/plat_stub.c"),
    "sceSifAllocIopHeap": ("stub de enlace (port/src)", "port/src/plat_stub.c:35", "Stub dummy de reserva en heap de IOP en port/src/plat_stub.c"),
    "sceSifBindRpc": ("stub de enlace (port/src)", "port/src/plat_stub.c:28", "Stub dummy de enlace SIF RPC en port/src/plat_stub.c"),
    "sceSifDmaStat": ("stub de enlace (port/src)", "port/src/plat_stub.c:45", "Stub dummy de estado DMA de SIF en port/src/plat_stub.c"),
    "sceSifFreeIopHeap": ("stub de enlace (port/src)", "port/src/plat_stub.c:36", "Stub dummy de liberación en heap de IOP en port/src/plat_stub.c"),
    "sceSifInitIopHeap": ("stub de enlace (port/src)", "port/src/plat_stub.c:34", "Stub dummy de inicialización de heap IOP en port/src/plat_stub.c"),
    "sceSifInitRpc": ("stub de enlace (port/src)", "port/src/plat_sys.c:11", "Stub dummy de inicialización RPC en port/src/plat_sys.c"),
    "sceSifLoadModule": ("stub de enlace (port/src)", "port/src/plat_sys.c:14", "Stub dummy de carga de módulo IRX en port/src/plat_sys.c"),
    "sceSifQueryMaxFreeMemSize": ("stub de enlace (port/src)", "port/src/plat_stub.c:38", "Stub dummy de consulta de memoria libre en port/src/plat_stub.c"),
    "sceSifQueryTotalFreeMemSize": ("stub de enlace (port/src)", "port/src/plat_stub.c:37", "Stub dummy de consulta de memoria total en port/src/plat_stub.c"),
    "sceSifRebootIop": ("stub de enlace (port/src)", "port/src/plat_sys.c:12", "Stub dummy de reinicio de IOP en port/src/plat_sys.c"),
    "sceSifSyncIop": ("stub de enlace (port/src)", "port/src/plat_sys.c:13", "Stub dummy de sincronización de IOP en port/src/plat_sys.c"),
    "sceVibGetProfile": ("stub de enlace (port/src)", "port/src/plat_stub.c:255", "Stub dummy de perfil de vibración en port/src/plat_stub.c"),
    "sceVibSetActParam": ("stub de enlace (port/src)", "port/src/plat_stub.c:256", "Stub dummy de actuadores de vibración en port/src/plat_stub.c"),

    # --- sin reemplazo ---
    "sceCdSearchFile": ("sin reemplazo", "ninguna (no definido en port/src/)", "Símbolo de búsqueda en CDVD sin definición en port/src/; no reemplazado en PC port"),
    "VU1_VIF_NOP": ("sin reemplazo", "include/sys/dma.h", "Constante de opcode VU1 compilada en código común sin reemplazo en PC"),
    "Dma_Flush": ("sin reemplazo", "src/sys/dma.c:63", "Función nativa de vaciado DMA compilada sin reemplazo en PC"),
    "Dma_InitController": ("sin reemplazo", "src/sys/dma.c:169", "Función nativa de inicialización DMA compilada sin reemplazo en PC"),
    "Dma_SendGif": ("sin reemplazo", "src/sys/dma.c:319", "Función nativa de envío GIF compilada sin reemplazo en PC"),
    "Dma_SendGifChain": ("sin reemplazo", "src/sys/dma.c:277", "Función nativa de envío GIF chain compilada sin reemplazo en PC"),
    "Dma_SendGifChainSync": ("sin reemplazo", "src/sys/dma.c:290", "Función nativa de envío GIF síncrono compilada sin reemplazo en PC"),
    "Dma_SendGifNoWait": ("sin reemplazo", "src/sys/dma.c:307", "Función nativa de envío GIF sin espera compilada sin reemplazo en PC"),
    "Dma_SendToIop": ("sin reemplazo", "src/sys/dma.c:343", "Función nativa de envío a IOP compilada sin reemplazo en PC"),
    "Dma_SendVif1": ("sin reemplazo", "src/sys/dma.c:202", "Función nativa de envío VIF1 compilada sin reemplazo en PC"),
    "Dma_SendVif1Spr": ("sin reemplazo", "src/sys/dma.c:241", "Función nativa de envío VIF1 SPR compilada sin reemplazo en PC"),
    "Dma_SendVif1SprSync": ("sin reemplazo", "src/sys/dma.c:256", "Función nativa de envío VIF1 síncrono compilada sin reemplazo en PC"),
    "Dma_SendVif1Sync": ("sin reemplazo", "src/sys/dma.c:220", "Función nativa de envío VIF1 síncrono compilada sin reemplazo en PC"),
    "Dma_WaitCond": ("sin reemplazo", "src/sys/dma.c:176", "Función nativa de espera de condición compilada sin reemplazo en PC"),
    "Dma_WaitGif": ("sin reemplazo", "src/sys/dma.c:335", "Función nativa de espera GIF compilada sin reemplazo en PC"),
    "Dma_WaitVu1": ("sin reemplazo", "src/sys/dma.c:189", "Función nativa de espera VU1 compilada sin reemplazo en PC"),
    "Pad_GetVibProfile": ("sin reemplazo", "src/sys/pad.c:192", "Función nativa de perfil de vibración compilada sin reemplazo en PC"),
    "Pad_Init": ("sin reemplazo", "src/sys/pad.c:20", "Función nativa de inicialización de mandos compilada sin reemplazo en PC"),
    "Pad_Update": ("sin reemplazo", "src/sys/pad.c:63", "Función nativa de actualización de mandos compilada sin reemplazo en PC"),
    "Vu0_Init": ("sin reemplazo", "src/sys/vu0_b_c.c:1083", "Función nativa de inicio VU0 compilada sin reemplazo en PC"),
}

# Índices de las 10 filas explícitas bajo directivas #ifdef PORT en Section 1
IFDEF_PORT_ROW_INDICES = {56, 57, 121, 124, 126, 129, 132, 134, 137, 160}

C_DEF_PATTERN = re.compile(
    r'^(?:static\s+|inline\s+|extern\s+)?(?:void|int|s8|s16|s32|s64|u8|u16|u32|u64|char|float|double|bool|long|unsigned|signed)(?:\s*\*+|\s+)\s*([A-Za-z0-9_]+)\s*\([^;{]*\)\s*\{',
    re.MULTILINE
)

def is_c_function_definition(fpath, line_no, func, sym):
    """Detecta si la línea en fpath corresponde a una definición de función por contexto sintáctico."""
    if not os.path.exists(fpath):
        return False
    try:
        with open(fpath, "r", encoding="utf-8", errors="ignore") as sf:
            lines = sf.readlines()
        idx = int(line_no) - 1
        if idx < 0 or idx >= len(lines):
            return False
        
        window = "".join(lines[idx:min(len(lines), idx + 5)])
        m = C_DEF_PATTERN.search(window)
        if m:
            name = m.group(1)
            if name == sym or name == func:
                return True
        
        line = lines[idx].strip()
        if (line.startswith("void ") or line.startswith("s32 ") or line.startswith("int ") or
            line.startswith("s16 ") or line.startswith("u32 ") or line.startswith("float ")):
            if "{" in line and "(" in line and (sym in line or func in line):
                return True
    except Exception:
        pass
    return False

def classify_category(fpath, line_no, func, sym, code):
    """Clasifica la categoría de la fila según su naturaleza sintáctica en el repositorio."""
    if code.startswith("/*") or code.startswith("//") or "Bits 16..23" in code:
        return ("Comentario", "Texto de comentario o documentación explicativa")
    if code.startswith("#define"):
        return ("Directiva #define", "Definición de macro (#define)")
    if fpath.startswith("include/"):
        return ("Declaración de función", "Prototipo de función en cabecera include/")
    if fpath.startswith("port/src/"):
        return ("Stub / implementación en port/src", "Definición/stub en capa de plataforma port/src/")
    if '".globl' in code or '".type' in code or '".size' in code or code.endswith(':\\n"') or code.endswith(':\\n'):
        return ("Ensamblador inline MIPS", "Directiva o instrucción ensamblador MIPS R5900 inline")
    if sym in ["VIF1_STAT", "GIF_STAT", "D2_CHCR"]:
        return ("Acceso a registro hardware PS2", "Lectura o espera activa sobre registro MMIO de hardware PS2")
    if sym == "VU1_VIF_NOP":
        return ("Comando VU1 NOP", "Asignación de código de operación NOP a buffer DMA de VU1")
    if fpath.startswith("src/") and is_c_function_definition(fpath, line_no, func, sym):
        return ("Definición de función en src", "Definición sintáctica de función en src/ (tipo + nombre + {)")
    return ("Llamada real a función en src", "Invocación directa a función ejecutable en src/ (sym(...))")

def get_file_line_context(fpath, target_line_no):
    """Analiza si target_line_no está dentro de #if 0, #ifdef/#ifndef, comentario de bloque o código normal."""
    with open(fpath, "r", encoding="utf-8", errors="ignore") as f:
        lines = f.readlines()
    
    in_block_comment = False
    pp_stack = []
    
    for idx, raw_line in enumerate(lines):
        line = raw_line.strip()
        line_no = idx + 1
        starts_in_comment = in_block_comment
        
        if not in_block_comment and line.startswith("#"):
            m_if = re.match(r'^#(if|ifdef|ifndef)\b\s*(.*)', line)
            m_elif = re.match(r'^#elif\b\s*(.*)', line)
            m_else = re.match(r'^#else\b', line)
            m_endif = re.match(r'^#endif\b', line)
            if m_if:
                kind, cond = m_if.groups()
                pp_stack.append((kind, cond.strip()))
            elif m_elif:
                if pp_stack:
                    pp_stack.pop()
                pp_stack.append(("elif", m_elif.group(1).strip()))
            elif m_else:
                if pp_stack:
                    last_kind, last_cond = pp_stack.pop()
                    pp_stack.append(("else", f"else de {last_kind} {last_cond}"))
                else:
                    pp_stack.append(("else", "orphan else"))
            elif m_endif:
                if pp_stack:
                    pp_stack.pop()
        
        # Procesar comentarios
        clean = re.sub(r'"(\\.|[^"\\])*"', '""', line)
        c_idx = 0
        while c_idx < len(clean):
            if not in_block_comment:
                if clean[c_idx:c_idx+2] == "/*":
                    in_block_comment = True
                    c_idx += 2
                    continue
                elif clean[c_idx:c_idx+2] == "//":
                    break
            else:
                if clean[c_idx:c_idx+2] == "*/":
                    in_block_comment = False
                    c_idx += 2
                    continue
            c_idx += 1
            
        if line_no == target_line_no:
            if starts_in_comment:
                return "comentario de bloque"
            if any(k == "if" and c == "0" for k, c in pp_stack):
                return "#if 0"
            if any(k in ("ifdef", "ifndef", "elif", "else") or (k == "if" and c != "0") for k, c in pp_stack):
                return "#ifdef/#ifndef"
            return "código normal"
            
    return "código normal"

def verify_single_call(fpath, line_no, sym):
    """Verifica sintácticamente la llamada, detecta rangos y líneas con continuación de barra invertida."""
    with open(fpath, "r", encoding="utf-8", errors="ignore") as sf:
        flines = sf.readlines()
    
    idx = int(line_no) - 1
    raw_line = flines[idx]
    ends_with_backslash = raw_line.rstrip("\r\n").rstrip().endswith("\\")
    
    match = re.search(r'\b' + re.escape(sym) + r'\s*\(', raw_line)
    if not match:
        return ("no", str(line_no), f"Línea no contiene invocación '{sym}(': '{raw_line.strip()}'")
    
    open_pos = match.end() - 1
    depth = 0
    curr_line_idx = idx
    curr_char_idx = open_pos
    found_end = False
    end_line_no = int(line_no)
    
    while curr_line_idx < len(flines):
        line_to_parse = flines[curr_line_idx]
        while curr_char_idx < len(line_to_parse):
            ch = line_to_parse[curr_char_idx]
            if ch == '(':
                depth += 1
            elif ch == ')':
                depth -= 1
                if depth == 0:
                    found_end = True
                    end_line_no = curr_line_idx + 1
                    break
            curr_char_idx += 1
        if found_end:
            break
        curr_line_idx += 1
        curr_char_idx = 0
    
    if not found_end:
        return ("no", str(line_no), f"Paréntesis de llamada a '{sym}' no balanceados")
    
    if ends_with_backslash:
        return ("sí", f"L{line_no} (\\)", f"Invocación a {sym}() dentro de macro con continuación de barra invertida (\\)")
    elif end_line_no != int(line_no):
        return ("sí", f"L{line_no}-L{end_line_no}", f"Invocación multilínea verificada a {sym}() (L{line_no}-L{end_line_no})")
    else:
        return ("sí", f"L{line_no}", f"Invocación unilínea verificada a {sym}() (L{line_no})")

def main():
    with open(DOC_PATH, "r", encoding="utf-8") as f:
        content = f.read()

    sec1_match = re.search(r"## 1\. Llamadas a Hardware PS2 por Subsistema.*?(?=## 2\. Inventario)", content, re.DOTALL)
    if not sec1_match:
        raise ValueError("No se encontró la Sección 1 en " + DOC_PATH)
    sec1_text = sec1_match.group(0)

    rows = []
    for line in sec1_text.splitlines():
        line = line.strip()
        if line.startswith("| [") or (line.startswith("|") and "file://" in line):
            parts = [p.strip() for p in line.split("|")[1:-1]]
            if len(parts) >= 6:
                rows.append(parts)

    print(f"Total filas procesadas de Sección 1: {len(rows)}")

    cat_counts = {}
    mech_counts = {}
    context_counts = {}
    multiline_rows = []
    definitions_list = []
    call_verifications = []

    os.makedirs(os.path.dirname(CLASSIFIED_TSV), exist_ok=True)
    os.makedirs(os.path.dirname(VERIFIED_TSV), exist_ok=True)

    with open(CLASSIFIED_TSV, "w", encoding="utf-8") as out_cls:
        out_cls.write("fila\tarchivo\tlinea\tfuncion\tsimbolo\tcategoria\tregla_categoria\tmecanismo_reemplazo\tfuente_definicion\tregla_reemplazo\n")
        
        for i, r in enumerate(rows):
            row_idx = i + 1
            fpath = re.match(r"\[([^\]]+)\]", r[0]).group(1)
            line_no = int(r[1])
            func = r[2].strip("` ")
            sym = r[3].strip("` ")
            code = r[4].strip("` ")

            cat, rule_cat = classify_category(fpath, line_no, func, sym, code)
            
            # Clasificación de mecanismo usando tabla explícita y excepciones de macro #ifdef PORT
            if row_idx in IFDEF_PORT_ROW_INDICES:
                mech = "#ifdef PORT"
                src_def = "include/sys/dma.h o src/sys/movie.c"
                rule_mech = "Macro condicional o acceso MMIO bajo directiva #ifdef PORT en código fuente"
            elif sym in SYMBOL_MAP:
                mech, src_def, rule_mech = SYMBOL_MAP[sym]
            else:
                mech, src_def, rule_mech = ("ambiguo", "desconocido", "Símbolo sin entrada en tabla de mapeo")

            cat_counts[cat] = cat_counts.get(cat, 0) + 1
            mech_counts[mech] = mech_counts.get(mech, 0) + 1

            if cat == "Definición de función en src":
                definitions_list.append((row_idx, fpath, line_no, func, sym))

            if cat == "Llamada real a función en src":
                sintaxis_ok, range_str, motivo = verify_single_call(fpath, line_no, sym)
                ctx = get_file_line_context(fpath, line_no)
                context_counts[ctx] = context_counts.get(ctx, 0) + 1
                if range_str != f"L{line_no}" or "(\)" in range_str:
                    multiline_rows.append((row_idx, fpath, line_no, range_str, sym, motivo))
                call_verifications.append((row_idx, fpath, line_no, range_str, sym, sintaxis_ok, ctx, motivo))

            out_cls.write(f"{row_idx}\t{fpath}\t{line_no}\t{func}\t{sym}\t{cat}\t{rule_cat}\t{mech}\t{src_def}\t{rule_mech}\n")

    with open(VERIFIED_TSV, "w", encoding="utf-8") as out_ver:
        out_ver.write("fila\tarchivo\tlinea\trango\tsimbolo\tsintaxis_ok\tcontexto\tmotivo\n")
        for row_idx, fpath, line_no, range_str, sym, sintaxis_ok, ctx, motivo in call_verifications:
            out_ver.write(f"{row_idx}\t{fpath}\t{line_no}\t{range_str}\t{sym}\t{sintaxis_ok}\t{ctx}\t{motivo}\n")

    print("\n--- Desglose de Categorías (Total: 369) ---")
    for cat, cnt in sorted(cat_counts.items()):
        print(f"  {cat}: {cnt}")
    print(f"Total Categorías: {sum(cat_counts.values())}")

    print("\n--- Desglose de Mecanismos de Reemplazo (Total: 369) ---")
    for mech, cnt in sorted(mech_counts.items()):
        print(f"  {mech}: {cnt}")
    print(f"Total Mecanismos: {sum(mech_counts.values())}")

    print("\n--- Desglose de Contexto de las 175 Llamadas ---")
    for ctx, cnt in sorted(context_counts.items()):
        print(f"  {ctx}: {cnt}")
    print(f"Total Llamadas Evaluadas: {sum(context_counts.values())}")

    print(f"\n--- Conteo de Llamadas Multilínea o con Continuación (\\) ---")
    print(f"Total detectadas: {len(multiline_rows)}")
    for mr in multiline_rows:
        print(f"  Fila {mr[0]}: {mr[1]}:{mr[2]} ({mr[3]}) sym={mr[4]} -> {mr[5]}")

if __name__ == "__main__":
    main()
