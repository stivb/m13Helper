const byte  KEY_ONE =-117;       
const byte  KEY_TWO =66;         
const byte  KEY_THREE =62;       
const byte  KEY_FOUR =23;        
const byte  KEY_FIVE =20;
const byte  NO_LK =0;
const byte  CAPS_LK =4;
const byte  NUM_LK =2;
const byte  SCROLL_LK =1;
const byte  ALL_LK=7;


const byte  STOPPED =10;
const byte  PLAYING =11;
const byte  RECORDING =12;
const byte  OVERDUBBING =13;
const byte  PLAYING_ONCE =14;
const byte  UNDO =15;

bool VIRTUAL_MODE=false;

struct Logic
{
    byte currState;
    byte currMode;
    byte currKey;
    byte nextState;
    byte effectInc;
};




Logic const logics[30] =
{
    { STOPPED,NO_LK,KEY_ONE,RECORDING,0},
    { STOPPED,NO_LK,KEY_TWO,PLAYING,0},
    { STOPPED,NO_LK,KEY_THREE,STOPPED,0},
    { PLAYING,NO_LK,KEY_ONE,OVERDUBBING,0},
    { PLAYING,NO_LK,KEY_TWO,STOPPED,0},
    { PLAYING,NO_LK,KEY_THREE,UNDO,0},
    { RECORDING,NO_LK,KEY_ONE,OVERDUBBING,0},
    { RECORDING,NO_LK,KEY_TWO,PLAYING,0},
    { RECORDING,NO_LK,KEY_THREE,UNDO,0},
    { OVERDUBBING,NO_LK,KEY_ONE,PLAYING,0},
    { OVERDUBBING,NO_LK,KEY_TWO,PLAYING,0},
    { OVERDUBBING,NO_LK,KEY_THREE,UNDO,0},
    { PLAYING_ONCE,NO_LK,KEY_ONE,RECORDING,0},
    { PLAYING_ONCE,NO_LK,KEY_TWO,PLAYING,0},
    { PLAYING_ONCE,NO_LK,KEY_THREE,STOPPED,0},
    
    { STOPPED,CAPS_LK,KEY_ONE,RECORDING,0},
    { STOPPED,CAPS_LK,KEY_TWO,PLAYING,0},
    { STOPPED,CAPS_LK,KEY_THREE,STOPPED,0},
    { PLAYING,CAPS_LK,KEY_ONE,OVERDUBBING,0},
    { PLAYING,CAPS_LK,KEY_TWO,STOPPED,0},
    { PLAYING,CAPS_LK,KEY_THREE,UNDO,0},
    { RECORDING,CAPS_LK,KEY_ONE,PLAYING,1},
    { RECORDING,CAPS_LK,KEY_TWO,PLAYING_ONCE,0},
    { RECORDING,CAPS_LK,KEY_THREE,UNDO,0},
    { OVERDUBBING,CAPS_LK,KEY_ONE,PLAYING,1},
    { OVERDUBBING,CAPS_LK,KEY_TWO,PLAYING_ONCE,0},
    { OVERDUBBING,CAPS_LK,KEY_THREE,UNDO,0},
    { PLAYING_ONCE,CAPS_LK,KEY_ONE,OVERDUBBING,0},
    { PLAYING_ONCE,CAPS_LK,KEY_TWO,PLAYING,0},
    { PLAYING_ONCE,CAPS_LK,KEY_THREE,UNDO,0}

};





//works well with the old keyboard
//just needs green in 2, white in 11 and black in the middle row and 
//red in the short row on the end

/*
 * M13 Footswitch MIDI CC# Looper Function
Record/Overdub 50 0 to 63 = Overdub 64 to 127 = Record
Play/Stop 28 0 to 63 = Stop 64 to 127 = Play
Half Speed 36 Toggles between Half Speed & Normal Speed
Reverse 85 Toggles Reverse On or Off
Undo/Redo 82 Activates Undo/Redo
Play Once 80 Activates Play Once function
Pre/Post 84 0 to 63 = Pre 64 to 127 = Post
Looper Control 86 0 to 63 = Exit 64 to 127 = Enter
 */

bool debug_mode=false;
byte fx[12]={0x0D,0x0C,0x0B,0x10,0x0F,0x0E,0x13,0x12,0x11,0x16,0x15,0x14};



byte scenes[4][12]={
                   {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B},
                   {0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17},
                   {0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23},
                   {0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F}
                   };





byte floorkeys[5] = {KEY_ONE,KEY_TWO,KEY_THREE,KEY_FOUR,KEY_FIVE};


int state = STOPPED;
int folder;
int row;
byte channel =0;
int defDelay = 10;
bool lightOn=false;
char tbs[16];
int folderSelected=0;
int sceneSelected=0;
int rowSelected=0;
bool isPlaying = false;
bool isRecording = false;
bool isPre=false;
int totalPosition;

unsigned long loopStartedAt;
unsigned long loopEndedAt;
unsigned long loopDuration;


const int DataPin = 5;
const int IRQpin =  6;
PS2KeyAdvanced keyboard;

const int CLK = 7;
const int DIO = 8;
