#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <esp_now.h>
#include <WiFi.h>


//SETUP for the screen : 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



//SETUP for the buttons :
const int ROW_NUM = 3; 
const int COLUMN_NUM = 3; 

char keys[ROW_NUM][COLUMN_NUM] = {
  {'U','L','D'},  // U = UP, L = LEFT, D = DOWN 
  {'R','s','S'},  // R = RIGHT, s = SELECT, S = START  
  {'A','B','_'}   // A = A, B = B, _ = not connected
};

byte pin_rows[ROW_NUM] = {20, 10, 0}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {7, 8, 9}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );



//Importation of the sprites :

int wizard_w = 32;
int wizard_h = 41;
static const unsigned char PROGMEM wizard[] = {
  0x00, 0x07, 0x00, 0x00, 
  0x60, 0x18, 0x80, 0x00, 
  0x90, 0x06, 0x40, 0x00, 
  0x90, 0x02, 0x20, 0x00, 
  0xf0, 0x02, 0x10, 0x00, 
  0x60, 0x02, 0x08, 0x00, 
  0x60, 0x04, 0x08, 0x00, 
  0x60, 0x02, 0x06, 0x00, 
  0x60, 0x06, 0x01, 0x00, 
  0x60, 0x3c, 0x03, 0xc0, 
  0x60, 0xc3, 0xfc, 0x70, 
  0x60, 0x3e, 0x07, 0xc0, 
  0x60, 0x04, 0x54, 0x00, 
  0x60, 0x1c, 0x04, 0x00, 
  0x70, 0x32, 0x32, 0x0e, 
  0x70, 0x62, 0x03, 0x91, 
  0x38, 0xce, 0x02, 0xe1, 
  0x4d, 0x8a, 0x03, 0x39, 
  0x4f, 0x33, 0x02, 0x8e, 
  0x3e, 0x05, 0x06, 0x88, 
  0x3c, 0x88, 0x84, 0x48, 
  0x34, 0x8c, 0x4e, 0x68, 
  0x35, 0x34, 0x4d, 0x10, 
  0x34, 0x54, 0x54, 0xa0, 
  0x36, 0x92, 0x24, 0x60, 
  0x37, 0xa2, 0x04, 0x40, 
  0x33, 0x22, 0x04, 0x00, 
  0x32, 0x21, 0x24, 0x00, 
  0x30, 0x21, 0x22, 0x00, 
  0x30, 0x31, 0x26, 0x00, 
  0x30, 0x29, 0x26, 0x00, 
  0x30, 0x2d, 0x21, 0x00, 
  0x30, 0x24, 0x91, 0x00, 
  0x10, 0x24, 0x91, 0x00, 
  0x00, 0x24, 0x48, 0x80, 
  0x00, 0x22, 0x44, 0x40, 
  0x00, 0x43, 0x04, 0x40, 
  0x00, 0xc0, 0xf5, 0x40, 
  0x01, 0x00, 0x1f, 0x80, 
  0x00, 0xf7, 0xe0, 0x00, 
  0x00, 0x3c, 0x00, 0x00, 
  };

int wizard_walk_1_w = 21;
int wizard_walk_1_h = 41;
static const unsigned char PROGMEM wizard_walk_1[] = {
  0x03, 0x80, 0x00, 
  0x0c, 0x60, 0x00, 
  0x03, 0xb0, 0x00, 
  0x01, 0x90, 0x00, 
  0x01, 0xf8, 0x00, 
  0x01, 0x64, 0x00, 
  0x02, 0x64, 0x00, 
  0x01, 0x63, 0x00, 
  0x03, 0x60, 0x80, 
  0x1e, 0x61, 0xe0, 
  0x61, 0xfe, 0x38, 
  0x1f, 0x63, 0xe0, 
  0x02, 0x6a, 0x00, 
  0x0e, 0x62, 0x00, 
  0x19, 0xf9, 0x00, 
  0x11, 0xe1, 0x00, 
  0x17, 0xc1, 0x00, 
  0x1f, 0x21, 0x00, 
  0x1f, 0xa1, 0x00, 
  0x17, 0xc3, 0x00, 
  0x37, 0xc2, 0x70, 
  0x26, 0xe7, 0x88, 
  0x2a, 0xe7, 0x08, 
  0x2a, 0xeb, 0xc8, 
  0x33, 0xd2, 0x70, 
  0x13, 0xc2, 0x40, 
  0x11, 0xc2, 0x40, 
  0x10, 0xd3, 0x40, 
  0x10, 0xd1, 0x80, 
  0x18, 0xd3, 0x00, 
  0x14, 0xd3, 0x00, 
  0x16, 0xd0, 0x80, 
  0x12, 0xc8, 0x80, 
  0x12, 0xc8, 0x80, 
  0x12, 0x24, 0x40, 
  0x11, 0x22, 0x20, 
  0x21, 0x82, 0x20, 
  0x60, 0x7a, 0xa0, 
  0x80, 0x0f, 0xc0, 
  0x7b, 0xf0, 0x00, 
  0x1e, 0x00, 0x00
  };

int wizard_walk_2_w = 21;
int wizard_walk_2_h = 41;
static const unsigned char PROGMEM wizard_walk_2[] = {
  0x03, 0x80, 0x00, 
  0x0c, 0x40, 0x00, 
  0x03, 0x20, 0x00, 
  0x01, 0x10, 0x00, 
  0x01, 0x08, 0x00, 
  0x01, 0x04, 0x30, 
  0x02, 0x04, 0x48, 
  0x01, 0x03, 0x48, 
  0x03, 0x00, 0xf8, 
  0x1e, 0x01, 0xf0, 
  0x61, 0xfe, 0x38, 
  0x1f, 0x03, 0xf0, 
  0x02, 0x2a, 0x30, 
  0x02, 0x02, 0x30, 
  0x01, 0x19, 0x30, 
  0x01, 0x01, 0xf0, 
  0x01, 0x01, 0x70, 
  0x02, 0xc1, 0xb0, 
  0x02, 0x61, 0x70, 
  0x06, 0x33, 0x70, 
  0x05, 0x9a, 0xe0, 
  0x0a, 0xaf, 0x90, 
  0x1a, 0x67, 0x90, 
  0x13, 0x0b, 0xe0, 
  0x11, 0x8b, 0xe0, 
  0x11, 0x8b, 0x60, 
  0x11, 0x67, 0x60, 
  0x10, 0x93, 0x60, 
  0x10, 0x9b, 0x60, 
  0x18, 0x9f, 0x60, 
  0x14, 0x97, 0x60, 
  0x16, 0x92, 0xe0, 
  0x12, 0x48, 0xe0, 
  0x12, 0x48, 0xe0, 
  0x12, 0x24, 0x60, 
  0x11, 0x22, 0x60, 
  0x21, 0x82, 0x60, 
  0x60, 0x7a, 0xc0, 
  0x80, 0x0f, 0xc0, 
  0x7b, 0xf0, 0x00, 
  0x1e, 0x00, 0x00
  };

int wizard_walk_3_w = 21;
int wizard_walk_3_h = 41;
static const unsigned char PROGMEM wizard_walk_3[] = {
  0x03, 0x80, 0x00, 
  0x0c, 0x40, 0x00, 
  0x03, 0x20, 0x00, 
  0x01, 0x10, 0x00, 
  0x01, 0x08, 0x00, 
  0x01, 0x04, 0x00, 
  0x02, 0x04, 0x00, 
  0x01, 0x03, 0x00, 
  0x03, 0x18, 0x80, 
  0x1e, 0x25, 0xe0, 
  0x61, 0xfe, 0x38, 
  0x1f, 0x3f, 0xe0, 
  0x02, 0x3a, 0x00, 
  0x02, 0x1a, 0x00, 
  0x01, 0x19, 0x70, 
  0x01, 0x19, 0x88, 
  0x07, 0x19, 0x08, 
  0x05, 0x19, 0xc8, 
  0x19, 0x99, 0x70, 
  0x12, 0x9b, 0x40, 
  0x34, 0x5a, 0x40, 
  0x26, 0x36, 0x40, 
  0x2a, 0x26, 0x80, 
  0x2a, 0x6b, 0x00, 
  0x33, 0xd3, 0x00, 
  0x13, 0xc2, 0x00, 
  0x11, 0xe2, 0x00, 
  0x10, 0xf2, 0x00, 
  0x10, 0xb1, 0x00, 
  0x1a, 0xb3, 0x00, 
  0x14, 0xb3, 0x00, 
  0x17, 0xb0, 0x80, 
  0x17, 0xf8, 0x80, 
  0x13, 0x78, 0x80, 
  0x13, 0x34, 0x40, 
  0x11, 0x32, 0x20, 
  0x21, 0xb2, 0x20, 
  0x60, 0x7a, 0xa0, 
  0x80, 0x3f, 0xc0, 
  0x7b, 0xf0, 0x00, 
  0x1e, 0x20, 0x00
  };

int portal_right_w = 17;
int portal_right_h = 50;
static const unsigned char PROGMEM portal_right[] = {
  0xe0, 0x00, 0x00, 
  0x18, 0x00, 0x00, 
  0x06, 0x00, 0x00, 
  0x11, 0x80, 0x00, 
  0x10, 0x70, 0x00, 
  0x20, 0x48, 0x00, 
  0xc3, 0x84, 0x00, 
  0x04, 0x04, 0x00, 
  0xf8, 0x02, 0x00, 
  0x20, 0x02, 0x00, 
  0x10, 0x02, 0x00, 
  0x10, 0x02, 0x00, 
  0x08, 0x03, 0x00, 
  0x08, 0x05, 0x00, 
  0x08, 0x19, 0x00, 
  0x08, 0x61, 0x00, 
  0x0f, 0x82, 0x00, 
  0x06, 0x02, 0x00, 
  0xc4, 0x02, 0x00, 
  0x24, 0x02, 0x00, 
  0x24, 0x02, 0x00, 
  0x14, 0x02, 0x00, 
  0x04, 0x02, 0x00, 
  0x04, 0x02, 0x00, 
  0x04, 0x07, 0x00, 
  0x07, 0xf9, 0x00, 
  0x14, 0x01, 0x00, 
  0x24, 0x01, 0x00, 
  0xc4, 0x01, 0x00, 
  0x04, 0x01, 0x00, 
  0x04, 0x07, 0x00, 
  0x26, 0x79, 0x00, 
  0xc3, 0x81, 0x00, 
  0x02, 0x02, 0x00, 
  0x1a, 0x02, 0x00, 
  0x02, 0x02, 0x00, 
  0x06, 0x02, 0x00, 
  0x06, 0x02, 0x00, 
  0x1a, 0x02, 0x00, 
  0x02, 0x02, 0x00, 
  0xfe, 0x01, 0x00, 
  0x01, 0xf6, 0x80, 
  0x18, 0x0c, 0x80, 
  0xe3, 0xf0, 0x80, 
  0x00, 0x00, 0x80, 
  0x00, 0x00, 0x80, 
  0x00, 0x00, 0x80, 
  0xc0, 0x01, 0x00, 
  0x38, 0x06, 0x00, 
  0x07, 0xf8, 0x00,
  };

int portal_left_w = 23;
int portal_left_h = 50;
static const unsigned char PROGMEM portal_left[] = {
  0x00, 0x00, 0x0e, 
  0x00, 0x00, 0x10, 
  0x00, 0x00, 0x60, 
  0x00, 0x00, 0x80, 
  0x00, 0x03, 0x20, 
  0x00, 0x04, 0x20, 
  0x00, 0x04, 0x10, 
  0x00, 0x08, 0x0e, 
  0x00, 0x08, 0x70, 
  0x00, 0x10, 0x80, 
  0x00, 0x28, 0x80, 
  0x00, 0x28, 0x80, 
  0x00, 0x45, 0x00, 
  0x00, 0x83, 0x00, 
  0x00, 0x81, 0x00, 
  0x00, 0x81, 0x08, 
  0x00, 0x81, 0x10, 
  0x00, 0x81, 0x26, 
  0x00, 0x81, 0x08, 
  0x00, 0x81, 0x00, 
  0x00, 0x81, 0x00, 
  0x00, 0x82, 0x06, 
  0x00, 0x82, 0x08, 
  0x00, 0x7e, 0x00, 
  0x00, 0x42, 0x20, 
  0x00, 0x41, 0x10, 
  0x00, 0x41, 0x08, 
  0x00, 0x41, 0x00, 
  0x00, 0x41, 0x20, 
  0x00, 0x41, 0x20, 
  0x00, 0x41, 0x10, 
  0x00, 0x5e, 0x08, 
  0x00, 0xe2, 0x00, 
  0x00, 0x82, 0x00, 
  0x00, 0x82, 0x00, 
  0x00, 0x82, 0x00, 
  0x01, 0x02, 0x00, 
  0x0f, 0x02, 0x00, 
  0x30, 0xfc, 0x00, 
  0x40, 0x03, 0xfe, 
  0x80, 0x00, 0x00, 
  0xff, 0x00, 0x00, 
  0x80, 0xff, 0xc0, 
  0x80, 0x00, 0x3e, 
  0x78, 0x00, 0x00, 
  0x07, 0xf0, 0x00, 
  0x00, 0x0f, 0x00, 
  0x00, 0x00, 0xfe, 
  0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 
  };


int fireball_w = 22;
int fireball_h = 11;
static const unsigned char PROGMEM fireball[] = {
  0x00, 0x0f, 0x80, 
  0x00, 0x38, 0x70, 
  0x03, 0xc0, 0x08, 
  0x02, 0x01, 0xf4, 
  0x01, 0xc3, 0xfc, 
  0x3f, 0xc7, 0xfc, 
  0xff, 0xcf, 0xfc, 
  0x01, 0x87, 0xfc, 
  0x0e, 0x01, 0xfc, 
  0x1f, 0xfc, 0x38, 
  0x00, 0x03, 0xe0 
  };

int fireball_reversed_w = 22;
int fireball_reversed_h = 11;
static const unsigned char PROGMEM fireball_reversed[] = {
  0x07, 0xc0, 0x00, 
  0x38, 0x70, 0x00, 
  0x40, 0x0f, 0x00, 
  0xbe, 0x01, 0x00, 
  0xff, 0x0e, 0x00, 
  0xff, 0x8f, 0xf0, 
  0xff, 0xcf, 0xfc, 
  0xff, 0x86, 0x00, 
  0xfe, 0x01, 0xc0, 
  0x70, 0xff, 0xe0, 
  0x1f, 0x00, 0x00, 
  };

int shield_w = 5;
int shield_h = 17;
static const unsigned char PROGMEM shield[] = {
  0x88, 0xd8, 
  0xa8, 0x88, 
  0x88, 0x88, 
  0x88, 0x88, 
  0x88, 0x88, 
  0x88, 0x88, 
  0x88, 0x88, 
  0x50, 0x50, 
  0x20
  };

int heal_w = 10;
int heal_h = 10;
static const unsigned char PROGMEM heal[] = {
  0x1e, 0x00, 
  0x12, 0x00, 
  0x12, 0x00, 
  0xf3, 0xc0, 
  0x80, 0x40, 
  0x80, 0x40, 
  0xf3, 0xc0, 
  0x12, 0x00, 
  0x12, 0x00, 
  0x1e, 0x00
  };

int stun_bubulle_h = 4;
int stun_bubulle_w = 4;
static const unsigned char PROGMEM stun_bubulle[] = {
 0x60, 0x90, 
 0x90, 0x60
  };

int empty_heart_h = 7;
int empty_heart_w = 7;
static const unsigned char PROGMEM empty_heart[] = {
  0x6c, 0x92, 
  0x82, 0x82, 
  0x44, 0x28, 
  0x10, 
  };

int full_heart_h = 7;
int full_heart_w = 7;
static const unsigned char PROGMEM full_heart[] = {
  0x6c, 0xfe, 
  0xfe, 0xfe, 
  0x7c, 0x38, 
  0x10, 
  };


//definition of all the possible messages :
const char stunned_message[36] = {'Y','O','U',' ','A','R','E',' ','S','T','U','N','N','E','D',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
const char shield_message[36] = {'Y','O','U',' ','A','L','R','E','A','D','Y',' ','H','A','V','E',' ','A', 'S','H','I','E','L','D',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
const char looking_message[36] = {'L','O','O','K','I','N','G',' ','F','O','R',' ','A',' ',' ',' ',' ',' ','P','O','R','T','A','L',' ','.','.','.',' ',' ',' ',' ',' ',' ',' ',' '};
const char found_message[36] = {'A',' ','P','O','R','T','A','L',' ','H','A','S',' ','B','E','E','N',' ','F','O','U','N','D',' ','!',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
const char death_message[36] = {' ',' ',' ',' ','Y','O','U',' ','A','R','E',' ','D','E','A','D',' ',' ',' ',' ',' ','(','P','R','E','S','S',' ','R','E','S','E','T',' ',')',' '};
const char stop_message[36] = {'T','H','E',' ','P','O','R','T','A','L',' ','S','T','O','P','P','E','D','E','M','M','I','T','T','I','N','G',' ','.','.','.',' ',' ',' ',' ',' '};
const char win_message[36] = {' ',' ',' ',' ','Y','O','U',' ','W','O','N',' ','!',' ',' ',' ',' ',' ',' ',' ',' ',' ','(','P','R','E','S','S',' ','R','E','S','E','T',')',' '};

//creation of the usefull variables : 
int spell_cursor = 0;
bool have_a_shield = false;
bool is_stunned = false;
int remained_life = 3;
int stunned_duration = -1;
int already_shield_message_duration = -1;
bool is_alive = true;
bool have_won = false;

//creation of the variables used to make the sprites mouve druign animations :
int fireball_frame = -1;
int fireball_reversed_frame = -1;
int shield_frame = -1;
int heal_frame = -1;
int stun_frame = -1;
int stun_reversed_frame = -1;

int memo_time = millis();

// SETUP for wifi communication (have to replace one line by an other depending on which gmaeboy is which) :
//uint8_t broadcastAddress[] = {0x58, 0xCF, 0x79, 0x07, 0x34, 0xC4};// vers carte PAul
uint8_t broadcastAddress[] = {0x58, 0xCF, 0x79, 0x06, 0x30, 0x9C}; // vers carte Théo
//uint8_t broadcastAddress[] = {0x58, 0xCF, 0x79, 0x06, 0x2C, 0x34}; // vers carte WHoooui

// Structure example to send data
typedef struct struct_message { char trigger;} struct_message;
struct_message myData;
char incoming_trigger[1];

//SETUP of the routine that execute when recieving data from the other player
void OnDataRecv(const uint8_t * mac, const uint8_t *incoming_data, int len) 
{
  memcpy(&incoming_trigger, incoming_data, sizeof(incoming_trigger));
  switch (incoming_trigger[0])
  {
    case 'F' : fireball_reversed_frame = 0; break; // receving a fireball 
    case 'S' : stun_reversed_frame = 0; break; //receiving a stun
    case 'D': you_won(); break;
  }
}



void setup() {

  //SETUP for the screen
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    while(true){} // Don't proceed, loop forever
  }
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text



  //SETUP for wifi connection :
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;


  display.clearDisplay();
  bool looking_for_game = true;
  int walk_frame = 0;
  int portal_arrive_frame = -1;
  int magician_step = 0;
  while(looking_for_game)
  {
    if(millis() - memo_time >= 75)
    {
      memo_time = millis();
      magician_step++;

      if(magician_step >= 4)
      {
        magician_step = 0;
        display.fillRect(52, 5, 36, 41, 0);
        walk_frame = (walk_frame + 1) % 4;
        switch(walk_frame)
        {
          case 0: display.drawBitmap(52, 5, wizard, wizard_w, wizard_h, 1); break;
          case 1: display.drawBitmap(63, 5, wizard_walk_1, wizard_walk_1_w, wizard_walk_1_h, 1); break;
          case 2: display.drawBitmap(59, 5, wizard_walk_2, wizard_walk_2_w, wizard_walk_2_h, 1); break;
          case 3: display.drawBitmap(55, 5, wizard_walk_3,wizard_walk_3_w, wizard_walk_3_h, 1); break;
        }
        draw_message(looking_message);
      }
      if(portal_arrive_frame != -1)
      {
        portal_arrive_frame++;
        display.fillRect(88, 0, 40, 50, 0);
        display.drawBitmap(128-portal_arrive_frame, 0, portal_left, portal_left_w, portal_left_h, 1);
        display.drawBitmap(151-portal_arrive_frame, 0, portal_right, portal_right_w, portal_right_h, 1);
        draw_message(found_message);
      }
      if(portal_arrive_frame >= 40)
      {
        looking_for_game = false;
      }
      display.display();
    }
    
    //test if there is a connection : 
    if (esp_now_add_peer(&peerInfo) == ESP_OK)
    {
      portal_arrive_frame = 0;
    }

  }
  draw_next_frame();
}

void loop() {
  if(is_alive && !(have_won))
  {
    //routines for when then player push a button :
    char key = keypad.getKey();
    
    if (key)
    {
      switch(key)
      {
        case 'A': cast_spell(spell_cursor); break;
        case 'U': spell_cursor--; if (spell_cursor < 0) {spell_cursor = 3;} break;
        case 'D': spell_cursor++; if (spell_cursor > 3) {spell_cursor = 0;} break;
      }
    }

    //stop all animations that come to there end and ativate the corresponding effect:
    if(fireball_frame >= 53) 
    { 
      fireball_frame = -1;
      //send the information that a fireball passed the portal
      myData.trigger = 'F';
      esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    }

    if(fireball_reversed_frame == 20 && have_a_shield) { fireball_reversed_frame = -1; have_a_shield = false;}
    if(fireball_reversed_frame >= 31) { fireball_reversed_frame = -1; check_death(--remained_life); }
    if(shield_frame >= 34) { shield_frame = -1; have_a_shield = true;}
    if(heal_frame >= 50) { heal_frame = -1; if(remained_life < 3){remained_life++;}}
    if(stun_frame >= 37) 
    { 
      stun_frame = -1;
      //send the information that a stun passed the portal
      myData.trigger = 'S';
      esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    }
    if(stun_reversed_frame == 20) { if(have_a_shield) {stun_reversed_frame = -1;}}
    if(stun_reversed_frame >= 29) { stun_reversed_frame = -1; stunned_duration = 0; is_stunned = true;}
    if(stunned_duration >= 50) { stunned_duration = -1; is_stunned = false;}
    if(already_shield_message_duration >= 15) { already_shield_message_duration = -1;}

    if(millis() - memo_time >= 100)
    {
      memo_time = millis();
      //update the position of all required animation :
      if(fireball_frame != -1) {fireball_frame += 2;}
      if(fireball_reversed_frame != -1) {fireball_reversed_frame += 2;}
      if(shield_frame != -1) {shield_frame += 2;}
      if(heal_frame != -1) {heal_frame += 2;}
      if(stun_frame != -1) {stun_frame += 1;}
      if(stun_reversed_frame != -1) {stun_reversed_frame += 1;}
      if(stunned_duration != -1) {stunned_duration += 1;}
      if(already_shield_message_duration != -1) {already_shield_message_duration += 1;}
      draw_next_frame();
    }
  }
}

void cast_spell(int choose)
{
  if(!(is_stunned))
  {
    switch(choose)
    {
      case 0 : //casting a fireball
        if(fireball_frame == -1){fireball_frame = 0;}
        break;

      case 1 : //creating a shield
        if(!(have_a_shield) && shield_frame == -1){shield_frame = 0;}
        else if (shield_frame == -1) {already_shield_message_duration = 0;}
        break;

      case 2 : //healing
        if(heal_frame == -1){heal_frame = 0;}
        break;

      case 3 : //stun
        if(stun_frame == -1) {stun_frame = 0;}
        break;
    }
  }
}

void display_spell_menu()
{
  display.fillRect(0, 0, 43, 45, 0);
  
  display.drawRect(0, 0, 43, 45, 1);

  display.drawRect(2, 2 + 10 * spell_cursor, 39, 11, 1);

  display.setCursor(4,4);
  display.write('F');
  display.write('I');
  display.write('R');
  display.write('E');

  display.setCursor(4,14);
  display.write('S');
  display.write('H');
  display.write('I');
  display.write('E');
  display.write('L');
  display.write('D');

  display.setCursor(4,24);
  display.write('H');
  display.write('E');
  display.write('A');
  display.write('L');

  display.setCursor(4,34);
  display.write('S');
  display.write('T');
  display.write('U');
  display.write('N');

} 

void draw_next_frame(){
  
  display.clearDisplay();
  
  display_spell_menu();

  for (int i = 1; i <= 3; i++)
  {
    if(i <= remained_life) { display.drawBitmap(44, 19 - 9 * (i-1), full_heart, full_heart_w, full_heart_h, 1);}
    else { display.drawBitmap(44, 19 - 9 * (i-1), empty_heart, empty_heart_w, empty_heart_h, 1);}
  }


  display.drawBitmap(88, 0, portal_left, portal_left_w, portal_left_h, 1);

  if(fireball_frame != -1) {display.drawBitmap(58 + fireball_frame, 19, fireball, fireball_w, fireball_h, 1);}
  if(fireball_reversed_frame != -1) {display.drawBitmap(111 - fireball_reversed_frame, 19, fireball_reversed, fireball_reversed_w, fireball_reversed_h, 1);}
  if(shield_frame != -1) 
  {
    display.fillRect(87, -17 + shield_frame, shield_w, shield_h, 0);
    display.drawBitmap(87 , -17 + shield_frame, shield, shield_w, shield_h, 1);
  }
  if(stun_frame != -1) 
  {
    display.drawBitmap(74 + stun_frame + stun_frame % 5, 19, stun_bubulle, stun_bubulle_w, stun_bubulle_h, 1);
    display.drawBitmap(80 + stun_frame, 19 + stun_frame % 5, stun_bubulle, stun_bubulle_w, stun_bubulle_h, 1);
    display.drawBitmap(74 + stun_frame , 25 - stun_frame % 5, stun_bubulle, stun_bubulle_w, stun_bubulle_h, 1);
    display.drawBitmap(80 + stun_frame - stun_frame % 5, 25, stun_bubulle, stun_bubulle_w, stun_bubulle_h, 1);
  }
  if(stun_reversed_frame != -1) 
  {
    display.drawBitmap(111 - stun_reversed_frame , 19 + stun_reversed_frame % 5, stun_bubulle, stun_bubulle_w, stun_bubulle_h, 1);
    display.drawBitmap(117 - stun_reversed_frame - stun_reversed_frame % 5, 19 , stun_bubulle, stun_bubulle_w, stun_bubulle_h, 1);
    display.drawBitmap(111 - stun_reversed_frame + stun_reversed_frame % 5, 25 , stun_bubulle, stun_bubulle_w, stun_bubulle_h, 1);
    display.drawBitmap(117 - stun_reversed_frame , 25 - stun_reversed_frame % 5, stun_bubulle, stun_bubulle_w, stun_bubulle_h, 1);
  }
  
  display.fillRect(52, 5, wizard_w-4, wizard_h, 0);
  display.drawBitmap(52, 5, wizard, wizard_w, wizard_h, 1);

  if(heal_frame != -1) 
  {
    display.drawBitmap(76, 2, heal, heal_w, heal_h, 1);
    if(heal_frame <= 25){ display.fillRect(76 + heal_frame / 5, 2 + heal_frame / 5, heal_w - ((heal_frame / 5) * 2), heal_h - ((heal_frame / 5) * 2), 0);}
    if(heal_frame > 25){ display.fillRect(76 + 4 - (heal_frame - 25) / 5, 2 + 4 - (heal_frame - 25) / 5, 2 + (((heal_frame - 25) / 5) * 2), 2 + (((heal_frame - 25) / 5) * 2), 0);}
  }

  display.fillRect(111, 0, portal_right_w, portal_right_h, 0);
  display.drawBitmap(111, 0, portal_right, portal_right_w, portal_right_h, 1);
  
  if(have_a_shield)
  {
    display.fillRect(87, 17, shield_w, shield_h, 0);
    display.drawBitmap(87, 17, shield, shield_w, shield_h, 1);
  }

  if(already_shield_message_duration != -1){draw_message(shield_message);}

  if(is_stunned){draw_message(stunned_message);}

  display.display();
}

void draw_message(const char which_message[]){
  
  display.fillRect(2, 45, 123, 20, 0);
  display.drawRect(2, 45, 123, 20, 1);
  display.setCursor(4,47);
  
  for(int i = 0; i < 18; i++){display.write(which_message[i]);}
  display.setCursor(4,57);
  for(int i = 0; i < 18; i++){display.write(which_message[i+18]);}
}

void check_death(int HP)
{
  if(HP <= 0)
  {
    //send the information that the wizard is dead :
    myData.trigger = 'D';
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    is_alive = false;
    display.clearDisplay();
    draw_message(death_message);
    display.display();
    while(true){}
  }
}

void you_won()
{
  have_won = true;
  display.fillRect(93, 17, 33, 11, 0);
  display.drawRect(93, 17, 33, 11, 1);
  display.setCursor(95,19);
  display.write('B');
  display.write('O');
  display.write('N');
  display.write('K');
  display.write('!');
  display.display();
  delay(750);
  display.fillRect(93, 17, 33, 11, 0);
  display.drawBitmap(88, 0, portal_left, portal_left_w, portal_left_h, 1);
  display.drawBitmap(111, 0, portal_right, portal_right_w, portal_right_h, 1);
  display.display();
  delay(500);
  draw_message(stop_message);
  display.display();
  delay(2000);
  display.clearDisplay();
  draw_message(win_message);
  display.display();
  while(true){}
}