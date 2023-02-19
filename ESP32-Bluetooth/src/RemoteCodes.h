// Power: 8E7807F
// Mute: 8E700FF
// Decrease Volume: 8E730CF
// Inrease Volume: 8E7906F
// Line1: 8E750AF
// Line2: 8E7A857
// OPT: 8E7B04F
// COX: 8E76897
// Bluetooth Enable: 8E7708F
// Start/Stop: 8E728D7
// Previous: 8E7609F
// Next: 8E710EF

typedef struct RemoteCodes_t {
    unsigned long btnTogglePower;
    unsigned long btnToggleMute;
    unsigned long btnDecreaseVolume;
    unsigned long btnIncreaseVolume;
    unsigned long btnEnableLine1;
    unsigned long btnEnableLine2;
    unsigned long btnEnableOPT;
    unsigned long btnEnableCOX;
    unsigned long btnEnableBluetooth;
    unsigned long btnTogglePlay;
    unsigned long btnPrevious;
    unsigned long btnNext;
} RemoteCodes_t;

const RemoteCodes_t RemoteCodes = {
    .btnTogglePower = 0x8E7807F,
    .btnToggleMute = 0x8E700FF,
    .btnDecreaseVolume = 0x8E730CF,
    .btnIncreaseVolume = 0x8E7906F,
    .btnEnableLine1 = 0x8E750AF,
    .btnEnableLine2 = 0x8E7A857,
    .btnEnableOPT = 0x8E7B04F,
    .btnEnableCOX = 0x8E76897,
    .btnEnableBluetooth = 0x8E7708F,
    .btnTogglePlay = 0x8E728D7,
    .btnPrevious = 0x8E7609F,
    .btnNext = 0x8E710EF,
};