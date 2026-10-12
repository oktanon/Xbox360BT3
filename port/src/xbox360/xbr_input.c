/*
 * port/src/xbox360/xbr_input.c
 *
 * Implementacion del subsistema 2 (input) sobre XInput del XDK.
 * Mapea los 32 bytes del buffer de libpad2 PS2 a XINPUT_GAMEPAD
 * + bLeftTrigger / bRightTrigger. Vibracion via XInputSetState
 * (wLeftMotorSpeed = small, wRightMotorSpeed = large).
 *
 * Convenciones:
 *   - socket (int devuelto por scePad2CreateSocket) = port + 1
 *     (socket 1 -> puerto 0, socket 2 -> puerto 1, etc.), para
 *     mantener una representacion tipo "handle" no cero (el
 *     protocolo de PS2 nunca devuelve socket 0).
 *   - El buffer `data` que scePad2Read rellena tiene 32 bytes: 2
 *     bytes de botones (bit=0 significa "pulsado" en libpad2, asi
 *     que se invierte con `~`), 4 bytes de sticks crudos (0x80 =
 *     centro, 0..255), 2 bytes de triggers y 24 bytes de
 *     pressure data (que Xbox 360 no tiene, asi que van a 0).
 *   - El juego lee `data[0]` y `data[1]` para construir el word
 *     `Pad.held` (ver src/sys/pad.c:87). El resto del buffer
 *     (sticks/triggers) lo lee tambien para derivar left/right.
 *
 * Auto-test: Xbr_InputInit sondea el puerto 0 con XInputGetState.
 * Si devuelve ERROR_SUCCESS o ERROR_DEVICE_NOT_CONNECTED, el test
 * es OK (el device puede o no estar conectado; el codigo los trata
 * ambos correctamente). Cualquier otro error (ERROR_NOT_LOGGED_ON,
 * etc.) hace FAIL.
 */
#include "xbox360/xbr_input.h"
#include "xbox360/xbr_platform.h"

#include <xtl.h>
#include <stdint.h>
#include <string.h>

#define XBR_PAD_MAX 4   /* XUSER_MAX_COUNT */
#define XBR_PAD_NONE 0xFF

/* Estado cacheado por puerto. */
static struct {
    int    connected;     /* 1 si el pad respondio a la ultima consulta */
    int    saw_connected; /* alguna vez se conecto (para el auto-test) */
    XINPUT_GAMEPAD state;
} sPads[XBR_PAD_MAX];

static int sXbrInputInited = 0;

/* --------------------------------------------------------------------------
 * Polling XInput por puerto
 * -------------------------------------------------------------------------- */
static int Xbr_PollXInput(int port) {
    XINPUT_STATE st;
    DWORD result;
    
    if (port < 0 || port >= XBR_PAD_MAX) {
        return 0;
    }
    ZeroMemory(&st, sizeof(st));
    result = XInputGetState((DWORD)port, &st);
    if (result == ERROR_SUCCESS) {
        sPads[port].state = st.Gamepad;
        sPads[port].connected = 1;
        sPads[port].saw_connected = 1;
        return 1;
    }
    sPads[port].connected = 0;
    return 0;
}

/* --------------------------------------------------------------------------
 * Auto-test
 * -------------------------------------------------------------------------- */
static void Xbr_InputSelfTest(void) {
    DWORD result;
    XINPUT_STATE st;
    
    /* XInputGetState con puerto 0: si devuelve SUCCESS, hay pad;
     * si devuelve DEVICE_NOT_CONNECTED, no hay pero el codigo
     * funciona; cualquier otro error (NOT_LOGGED_ON etc.) es FAIL. */
    ZeroMemory(&st, sizeof(st));
    result = XInputGetState(0, &st);
    if (result == ERROR_SUCCESS) {
        sPads[0].state = st.Gamepad;
        sPads[0].connected = 1;
        sPads[0].saw_connected = 1;
        Xbr_Log("Xbr_Log: input self-test OK (puerto 0 con pad)\n");
    } else if (result == ERROR_DEVICE_NOT_CONNECTED) {
        sPads[0].connected = 0;
        Xbr_Log("Xbr_Log: input self-test OK (puerto 0 sin pad, XInputGetState devuelve ERROR_DEVICE_NOT_CONNECTED)\n");
    } else {
        Xbr_Log("Xbr_Log: input self-test FAILED (XInputGetState = 0x%08X)\n", (unsigned)result);
    }
}

/* --------------------------------------------------------------------------
 * Public API
 * -------------------------------------------------------------------------- */
void Xbr_InputInit(void) {
    if (sXbrInputInited) {
        return;
    }
    sXbrInputInited = 1;
    Xbr_Log("Xbr_Log: sceDbcInit / scePad2Init (subsistema 2 input, XInput)\n");
    Xbr_InputSelfTest();
}

int sceDbcInit(void) {
    return 1; /* siempre OK; XInput no necesita "dbc" */
}

int scePad2Init(int mode) {
    (void)mode;
    Xbr_InputInit();
    return 1;
}

int scePad2CreateSocket(void *param, void *work) {
    /* param es Pad2SocketParam: option, port, slot, number, name[16].
     * Solo nos interesa el port; devolvemos port+1 para tener
     * handles 1-based. `work` es el area de 0x100 bytes que el
     * juego asigna (gPad[i].work); no la necesitamos. */
    int port = 0;
    if (param != NULL) {
        /* Pad2SocketParam: u32 option, s32 port, ... */
        const int *p32 = (const int *)param;
        port = p32[1]; /* port en offset 4 */
    }
    (void)work;
    if (port < 0 || port >= XBR_PAD_MAX) {
        return 0;
    }
    return port + 1;
}

int scePad2GetState(int socket) {
    int port = socket - 1;
    if (port < 0 || port >= XBR_PAD_MAX) {
        return 0;
    }
    /* Polling fresco cada vez que se pregunta; en PS2 era un
     * estado cacheado del driver, pero XInput expone el estado
     * actual y la diferencia de coste es despreciable. */
    if (Xbr_PollXInput(port)) {
        return 1; /* PAD_STATE_STABLE */
    }
    return 0;
}

int scePad2GetButtonProfile(int socket, void *profile) {
    int port = socket - 1;
    (void)port;
    /* El juego usa el profile solo para detectar "digital only"
     * (Pad_IsDigitalOnly: profile == F9 FF 00 00). Devolvemos
     * 0xFF 0xFF 0x00 0x00 -> "digital only" (todos los digitales
     * + sticks no). Es lo que el port de PC hace en su primer
     * arranque. Esto hace que el juego entre en el camino
     * "digital only" para pads que no tenemos; no rompe nada. */
    {
        unsigned char *p = (unsigned char *)profile;
        p[0] = 0xF9;
        p[1] = 0xFF;
        p[2] = 0x00;
        p[3] = 0x00;
    }
    return 4; /* bytes escritos */
}

int scePad2Read(int socket, unsigned char *data) {
    int port = socket - 1;
    
    if (data == NULL) {
        return 0;
    }
    
    if (port < 0 || port >= XBR_PAD_MAX || !Xbr_PollXInput(port)) {
        /* Sin pad: todos los botones liberados (0xFF = invertido 0)
         * y sticks centrados (0x80). El juego interpreta 0 como
         * "pulsado" y "0x80" como "centro" en este formato. */
        memset(data, 0, 32);
        data[0] = 0xFF;
        data[1] = 0xFF;
        data[2] = 0x80; /* RX center */
        data[3] = 0x80; /* RY center */
        data[4] = 0x80; /* LX center */
        data[5] = 0x80; /* LY center */
        /* Triggers y pressure: 0. */
        return 0;
    }
    
    {
        const XINPUT_GAMEPAD *gp = &sPads[port].state;
        unsigned char btn_lo = 0, btn_hi = 0;
        
        /* Botones digitales: mapeo XInput -> PS2 libpad2.
         * XInput bit 0 = down, PS2 libpad2 bit 0 = down en raw,
         * pero el juego invierte con ^0xFFFF (ver pad.c:87), asi
         * que "0 = down" en libpad2 -> "1 = down" en Pad.held.
         * Por eso aqui escribimos 0 cuando el bit XInput esta activo. */
        if (gp->wButtons & XINPUT_GAMEPAD_BACK)          btn_lo |= 0x01; /* SELECT */
        if (gp->wButtons & XINPUT_GAMEPAD_LEFT_THUMB)    btn_lo |= 0x02; /* L3 */
        if (gp->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)   btn_lo |= 0x04; /* R3 */
        if (gp->wButtons & XINPUT_GAMEPAD_START)         btn_lo |= 0x08; /* START */
        if (gp->wButtons & XINPUT_GAMEPAD_DPAD_UP)       btn_lo |= 0x10; /* UP */
        if (gp->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)    btn_lo |= 0x20; /* RIGHT */
        if (gp->wButtons & XINPUT_GAMEPAD_DPAD_DOWN)     btn_lo |= 0x40; /* DOWN */
        if (gp->wButtons & XINPUT_GAMEPAD_DPAD_LEFT)     btn_lo |= 0x80; /* LEFT */
        if (gp->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)  btn_hi |= 0x04; /* L1 */
        if (gp->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) btn_hi |= 0x08; /* R1 */
        if (gp->wButtons & XINPUT_GAMEPAD_Y)             btn_hi |= 0x10; /* TRIANGLE */
        if (gp->wButtons & XINPUT_GAMEPAD_B)             btn_hi |= 0x20; /* CIRCLE */
        if (gp->wButtons & XINPUT_GAMEPAD_A)             btn_hi |= 0x40; /* CROSS */
        if (gp->wButtons & XINPUT_GAMEPAD_X)             btn_hi |= 0x80; /* SQUARE */
        
        /* Invertimos (libpad2: 0 = down). */
        data[0] = (unsigned char)~btn_lo;
        data[1] = (unsigned char)~btn_hi;
        
        /* Sticks: XInput 16-bit signed -32768..32767 -> PS2 byte
         * 0..255 con 0x80 = centro. Usamos >> 8 sobre el valor
         * con offset 32768; clamp a 0..255. */
        data[2] = (unsigned char)(((int)gp->sThumbRX + 32768) >> 8); /* RX */
        data[3] = (unsigned char)(((int)gp->sThumbRY + 32768) >> 8); /* RY */
        data[4] = (unsigned char)(((int)gp->sThumbLX + 32768) >> 8); /* LX */
        data[5] = (unsigned char)(((int)gp->sThumbLY + 32768) >> 8); /* LY */
        
        /* Triggers: XInput bLeftTrigger / bRightTrigger son 0..255
         * ya; PS2 espera lo mismo. */
        data[6] = gp->bRightTrigger; /* R2 (PAD_DATA_R2 = 6) */
        data[7] = gp->bLeftTrigger;  /* L2 (PAD_DATA_L2 = 7) */
        
        /* Pressure: Xbox 360 no tiene sensores de presion. */
        memset(data + 8, 0, 32 - 8);
        
        return 32; /* bytes escritos */
    }
}

int sceVibGetProfile(int socket, unsigned char *profile) {
    int port = socket - 1;
    (void)port;
    if (profile == NULL) {
        return 0;
    }
    /* Perfil minimo: ambos motores disponibles. */
    profile[0] = 0xFF;
    profile[1] = 0xFF;
    return 2;
}

int sceVibSetActParam(int socket, int profileSize, unsigned char *profile,
                      int dataSize, void *data) {
    int port = socket - 1;
    unsigned char *d = (unsigned char *)data;
    XINPUT_VIBRATION vib;
    DWORD result;
    
    (void)profileSize;
    (void)profile;
    
    if (port < 0 || port >= XBR_PAD_MAX) {
        return -1;
    }
    if (d == NULL || dataSize < 2) {
        return -1;
    }
    
    /* Formato PS2 libvib: data[0] bit 0 = small on/off, data[1]
     * = large motor power (0..254). Ver src/sys/pad.c:201-212:
     *   power <<= 1; power |= p->vibSmall; -> vibData (u16)
     *   sceVibSetActParam(socket, 1, p->vibProfile, 2, &p->vibData)
     * Asi data[0] es el byte bajo y data[1] es el byte alto. */
    ZeroMemory(&vib, sizeof(vib));
    vib.wLeftMotorSpeed  = (d[0] & 0x01) ? 65535 : 0;
    vib.wRightMotorSpeed = (WORD)((unsigned)d[1] * 257u);
    if (vib.wRightMotorSpeed > 65535) {
        vib.wRightMotorSpeed = 65535;
    }
    
    if (!sPads[port].connected) {
        return 0; /* nada que hacer */
    }
    result = XInputSetState((DWORD)port, &vib);
    return (result == ERROR_SUCCESS) ? 0 : -1;
}
