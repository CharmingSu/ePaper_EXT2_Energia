/* 
 *  This example code is for PDi 1.54" BWR EPD on EXT2 board which is verified by TI Launchpad MSP-EXP432P401R(red PCB).
 *  For more information about PDi EPD and EXT2 board, please visit 
 *  http://www.pervasivedisplays.com/
 *  http://www.pervasivedisplays.com/kits/ext2_kit
 */


#define SCL_PIN 7      //EXT2 BOARD J5 pin 7
#define SDA_PIN 15     //EXT2 BOARD J5 pin 15
#define CS_PIN 19      //EXT2 BOARD J5 pin 19
#define DC_PIN 9       //EXT2 BOARD J5 pin 9
#define RESET_PIN 10   //EXT2 BOARD J5 pin 10
#define BUSY_PIN 8     //EXT2 BOARD J5 pin 8
#define PNLON_PIN 11   //EXT2 BOARD J5 pin 11
#define CSS_PIN 6      //EXT2 BOARD J5 pin 2 Slave CSB is only required of 12" operation
                       //EXT2 BOARD J5 pin 20 connected to GND
                       //EXT2 BOARD J5 pin 17 connected to GND for 4 wire SPI
                       //EXT2 BOARD J5 pin 1 connected to 3V3

extern const uint8_t blackBuffer [];
extern const uint8_t redBuffer [];

// Software SPI setup
void softwareSpi( uint8_t data ) {
  for ( int i = 0; i < 8; i++ ) {
    if ((( data >> (7 - i) ) & 0x01 ) == 1 ) digitalWrite( SDA_PIN, HIGH );
    else digitalWrite( SDA_PIN, LOW );
    digitalWrite( SCL_PIN, HIGH );
    digitalWrite( SCL_PIN, LOW );
  }
}

// Software SPI protocl setup
void sendIndexData( uint8_t index, const uint8_t *data, uint32_t len ) {
  digitalWrite( DC_PIN, LOW );      //DC Low
  digitalWrite( CS_PIN, LOW );      //CS Low
  softwareSpi( index );
  digitalWrite( CS_PIN, HIGH );     //CS High
  digitalWrite( DC_PIN, HIGH );     //DC High
  digitalWrite( CS_PIN, LOW );      //CS High
  for ( int i = 0; i < len; i++ ) softwareSpi( data[ i ] );
  digitalWrite( CS_PIN, HIGH );     //CS High
}

//setup function runs once on startup
void setup() {            
  Serial.begin( 115200 ); //begin a Serial link
  pinMode( PNLON_PIN, OUTPUT);
  pinMode( SCL_PIN, OUTPUT );
  pinMode( SDA_PIN, OUTPUT );
  pinMode( CS_PIN, OUTPUT );
  pinMode( DC_PIN, OUTPUT );
  pinMode( RESET_PIN, OUTPUT );
  digitalWrite( PNLON_PIN, HIGH );    //PANEL_ON# = 1
  pinMode( BUSY_PIN, INPUT );         //All Pins 0
  delay( 5 );                         //Delay 5ms
  digitalWrite( RESET_PIN, HIGH );    //RES# = 1
  delay( 5 );                         //Delay 5ms
  digitalWrite( RESET_PIN, LOW );
  delay( 10 );
  digitalWrite( RESET_PIN, HIGH );
  delay( 5 );
  digitalWrite( CS_PIN, HIGH );       //CS# = 1

  uint8_t data9[] = { 0x0e };         
  sendIndexData( 0x00, data9, 1 );    //Soft-reset
  delay( 5 );
  
  uint8_t data7[] = { 0x19 };   
  sendIndexData( 0xe5, data7, 1 );    //Input Temperature: 25C
  uint8_t data6[] = { 0x02 };         
  sendIndexData( 0xe0, data6, 1 );    //Active Temperature

// Send image data
  Serial.println("2");
  sendIndexData( 0x10, blackBuffer, 2888 ); //First frame
  sendIndexData( 0x13, redBuffer, 2888 );   //Second frame

  Serial.println("3");
  delay( 50 );
  uint8_t data8[] = { 0x00 };
  sendIndexData( 0x04, data8, 1 );    //Power on
  delay( 5 );
  while( digitalRead( BUSY_PIN ) != HIGH );
  sendIndexData( 0x12, data8, 1 );    //Display Refresh
  delay( 5 );
  while( digitalRead( BUSY_PIN ) != HIGH );
  sendIndexData( 0x02, data8, 1 );    //Turn off DC/DC
  while( digitalRead( BUSY_PIN ) != HIGH );
  digitalWrite( DC_PIN, LOW );
  digitalWrite( CS_PIN, LOW );
  digitalWrite( SDA_PIN, LOW );
  digitalWrite( SCL_PIN, LOW );
  digitalWrite( RESET_PIN, LOW );
  Serial.println("4");
}

void loop() {
}

const uint8_t blackBuffer [] = { 
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 1
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 2
0x80,0x00,0x00,0x00,0x00,0x00,0x01,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 3
0x80,0x00,0x00,0x00,0x00,0x00,0x02,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x19,0xe1, // 4
0x80,0x00,0x00,0x00,0x00,0x00,0x05,0x48,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x39,0xf1, // 5
0x80,0x00,0x00,0x00,0x00,0x00,0x0b,0xbd,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3b,0xf1, // 6
0x80,0x00,0x00,0x00,0x00,0x00,0x0e,0x64,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x31, // 7
0x80,0x00,0x00,0x00,0x00,0x00,0x09,0xcb,0x0d,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x71, // 8
0x80,0x00,0x00,0x00,0x00,0x00,0x1b,0x3d,0x9a,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3e,0x71, // 9
0x80,0x00,0x00,0x00,0x00,0x00,0x1e,0xd2,0x67,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x61, // 10
0x80,0x00,0x00,0x00,0x00,0x00,0x29,0x6d,0xcc,0xe8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 11
0x80,0x00,0x00,0x00,0x00,0x00,0x5b,0x9b,0x10,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1d,0xc1, // 12
0x80,0x00,0x00,0x00,0x00,0x00,0x36,0x72,0x61,0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3d,0xe1, // 13
0x80,0x00,0x00,0x00,0x00,0x00,0x69,0xaf,0xc8,0x6a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3d,0xf1, // 14
0x80,0x00,0x00,0x00,0x00,0x00,0x57,0x66,0x4a,0xf6,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x30,0x31, // 15
0x80,0x00,0x00,0x00,0x00,0x00,0xde,0xd9,0xd1,0x4c,0x00,0x00,0x00,0x00,0x3f,0xff,0x00,0x3f,0xf1, // 16
0x80,0x00,0x00,0x00,0x00,0x00,0xe9,0x27,0xaa,0x73,0x00,0x00,0x00,0x00,0x10,0x41,0x80,0x3f,0xf1, // 17
0x80,0x00,0x00,0x00,0x00,0x00,0x96,0xee,0x40,0x6e,0x00,0x00,0x00,0x00,0x10,0x41,0x00,0x1f,0xe1, // 18
0x80,0x00,0x00,0x00,0x00,0x00,0xdb,0xb9,0xb2,0xb5,0x80,0x00,0x00,0x00,0x10,0x41,0x00,0x00,0x01, // 19
0x80,0x00,0x00,0x00,0x00,0x01,0x34,0x96,0xc9,0x5a,0x80,0x00,0x00,0x00,0x10,0x40,0x00,0x1f,0xe1, // 20
0x80,0x00,0x00,0x00,0x00,0x01,0xeb,0x6d,0x20,0x33,0x00,0x00,0x00,0x00,0x1f,0xc0,0x00,0x3f,0xf1, // 21
0x80,0x00,0x00,0x00,0x00,0x01,0x94,0xff,0xd2,0x2d,0x40,0x00,0x00,0x00,0x10,0x40,0x00,0x3f,0xf1, // 22
0x80,0x00,0x00,0x00,0x00,0x03,0x2b,0x00,0xad,0x1a,0xc0,0x00,0x00,0x00,0x10,0x40,0x00,0x10,0x21, // 23
0x80,0x00,0x00,0x00,0x00,0x06,0xc0,0xe5,0x50,0xa6,0xa0,0x00,0x00,0x00,0x10,0x40,0x00,0x3f,0xf1, // 24
0x80,0x00,0x00,0x00,0x00,0x0b,0x2f,0x9e,0xd0,0x1d,0x50,0x00,0x00,0x00,0x10,0x40,0x00,0x3f,0xf1, // 25
0x80,0x00,0x00,0x00,0x00,0x7c,0x96,0x79,0xa9,0x2a,0xa0,0x00,0x00,0x00,0x3f,0xff,0x80,0x3f,0xf1, // 26
0x80,0x00,0x00,0x00,0x00,0xdb,0xb9,0xc3,0x52,0x03,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 27
0x80,0x00,0x00,0x00,0x00,0xa5,0x46,0x8e,0xf8,0x49,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0x31, // 28
0x80,0x00,0x00,0x00,0x01,0x5b,0xb9,0x75,0x94,0x12,0x94,0x00,0x00,0x00,0x00,0x00,0x01,0xfe,0x31, // 29
0x80,0x00,0x00,0x00,0x00,0xfe,0x56,0x02,0x8d,0x01,0x6a,0x00,0x00,0x00,0x00,0x00,0x01,0xff,0xb1, // 30
0x80,0x00,0x00,0x00,0x01,0xa5,0xa8,0xcd,0x30,0xa4,0xd4,0x00,0x00,0x00,0x00,0x00,0x01,0x87,0xf1, // 31
0x80,0x00,0x00,0x00,0x01,0x5a,0xd6,0x2c,0xb2,0x00,0x9a,0x00,0x00,0x00,0x00,0x00,0x01,0xf1,0xf1, // 32
0x80,0x00,0x00,0x00,0x01,0xbf,0x11,0x2b,0xdb,0x08,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0x71, // 33
0x80,0x00,0x00,0x00,0x03,0x65,0xb6,0x25,0xa5,0x20,0x6a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 34
0x80,0x00,0x00,0x00,0x01,0x7a,0xc8,0x96,0x32,0x89,0x53,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 35
0x80,0x00,0x00,0x00,0x02,0xcf,0x13,0x59,0xdc,0xd4,0x2c,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 36
0x80,0x00,0x00,0x00,0x03,0x35,0xd6,0x97,0x40,0x40,0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 37
0x80,0x00,0x00,0x00,0x06,0xee,0x80,0xac,0x2d,0x29,0x66,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 38
0x80,0x00,0x00,0x00,0x06,0xdb,0x66,0x41,0xbb,0x44,0xa8,0x40,0x00,0x00,0x00,0x00,0x01,0xf0,0x01, // 39
0x80,0x00,0x00,0x00,0x0c,0xb7,0xe3,0xcf,0x4d,0x92,0x76,0x80,0x00,0x00,0x00,0x00,0x01,0xff,0x01, // 40
0x80,0x00,0x00,0x00,0x0b,0x6d,0x90,0x06,0xe2,0x69,0x29,0x80,0x00,0x00,0x00,0x00,0x00,0x1f,0xf1, // 41
0x80,0x00,0x00,0x00,0x0e,0xdb,0x75,0xb0,0xbd,0x94,0x12,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0xf1, // 42
0x80,0x00,0x00,0x00,0x05,0xaf,0x63,0x6b,0x53,0x60,0x3a,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 43
0x80,0x00,0x00,0x00,0x0a,0x5b,0xd8,0x91,0x6a,0x59,0x95,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 44
0x80,0x00,0x00,0x00,0x0d,0xa6,0xce,0xda,0xb5,0xb2,0x8a,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 45
0x80,0x00,0x00,0x00,0x0a,0xdd,0x74,0x64,0xcc,0x94,0x18,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 46
0x80,0x00,0x00,0x00,0x0d,0x2b,0xab,0x0d,0xf6,0x6e,0x4d,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 47
0x80,0x00,0x00,0x00,0x0b,0x75,0xd4,0xab,0x3b,0x32,0x86,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 48
0x80,0x00,0x00,0x00,0x0e,0xce,0xab,0x44,0x4a,0x6d,0xb5,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 49
0x80,0x00,0x00,0x00,0x05,0xa5,0x55,0xad,0xb5,0xad,0x1a,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 50
0x80,0x00,0x00,0x00,0x0a,0x3b,0xe8,0x57,0x77,0xdb,0x4e,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 51
0x80,0x00,0x00,0x00,0x05,0xce,0x80,0xe4,0x8e,0x5a,0xe6,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 52
0x80,0x00,0x00,0x00,0x0e,0xb5,0x58,0x2b,0x6d,0xa7,0x53,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 53
0x80,0x00,0x00,0x00,0x09,0x6a,0xa0,0x15,0xd2,0xfa,0xab,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 54
0x80,0x00,0x00,0x00,0x07,0x5d,0x80,0x39,0xb5,0xb6,0xd8,0xe8,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 55
0x80,0x00,0x00,0x00,0x04,0xb3,0x50,0x06,0x6b,0x99,0x76,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 56
0x80,0x00,0x00,0x00,0x05,0x6e,0x64,0x0d,0x35,0x6e,0xdb,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 57
0x80,0x00,0x00,0x00,0x05,0x95,0x90,0x12,0xdc,0xf7,0x2d,0xb0,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 58
0x80,0x00,0x00,0x00,0x06,0x6a,0xe8,0x03,0xab,0x1a,0xb6,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 59
0x80,0x00,0x00,0x00,0x05,0xb5,0x35,0x09,0x56,0xcf,0xc9,0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 60
0x80,0x00,0x00,0x00,0x04,0x8b,0x4a,0x02,0xb9,0xb2,0xf7,0xfa,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 61
0x80,0x00,0x00,0x00,0x03,0x7e,0xf4,0x10,0xcb,0xdd,0x3f,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 62
0x80,0x00,0x00,0x00,0x01,0xd3,0xac,0x09,0x4c,0x7e,0xd2,0x74,0x40,0x00,0x00,0x00,0x00,0x00,0x01, // 63
0x80,0x00,0x00,0x00,0x02,0x2d,0x50,0x82,0x32,0xab,0xdd,0xbb,0x40,0x00,0x00,0x00,0x00,0x00,0x01, // 64
0x80,0x00,0x00,0x00,0x02,0x5a,0xb2,0x04,0x5b,0xd5,0xba,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 65
0x80,0x00,0x00,0x00,0x01,0xa5,0xa4,0x00,0x85,0x5e,0x77,0xb6,0x10,0x00,0x00,0x00,0x00,0x00,0x01, // 66
0x80,0x00,0x00,0x00,0x01,0x3f,0x72,0x40,0x0c,0x33,0xea,0x59,0x40,0x00,0x00,0x00,0x00,0x00,0x01, // 67
0x80,0x00,0x00,0x00,0x00,0xd2,0xcd,0x00,0x93,0xf4,0x95,0xe6,0x80,0x00,0x00,0x00,0x00,0x00,0x01, // 68
0x80,0x00,0x00,0x00,0x00,0xad,0xb8,0x40,0x07,0x4f,0x7e,0xbd,0x80,0x00,0x00,0x00,0x00,0x00,0x01, // 69
0x80,0x00,0x00,0x00,0x00,0x57,0x51,0x00,0x0e,0x80,0x03,0x5a,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 70
0x80,0x00,0x00,0x00,0x00,0x6a,0xaa,0x00,0x05,0xc0,0x00,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 71
0x80,0x00,0x00,0x00,0x00,0x55,0x24,0x04,0x1f,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 72
0x80,0x00,0x00,0x00,0x00,0x2e,0xdb,0x10,0x0b,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 73
0x80,0x00,0x00,0x00,0x00,0x19,0xb4,0x80,0x96,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 74
0x80,0x00,0x00,0x00,0x00,0x13,0x2b,0xc0,0x2d,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 75
0x80,0x00,0x00,0x00,0x00,0x0e,0xd9,0x0b,0x5b,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 76
0x80,0x00,0x00,0x00,0x00,0x05,0xa6,0x90,0x96,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 77
0x80,0x00,0x00,0x00,0x00,0x02,0x7a,0xe4,0x2b,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 78
0x80,0x00,0x00,0x00,0x00,0x03,0xa5,0x52,0x56,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 79
0x80,0x00,0x00,0x00,0x00,0x00,0xda,0xaa,0x9d,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 80
0x80,0x00,0x00,0x00,0x00,0x00,0x6d,0x5d,0x7f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 81
0x80,0x00,0x00,0x00,0x00,0x00,0x32,0xb2,0xba,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 82
0x80,0x00,0x00,0x00,0x00,0x00,0x1d,0x6d,0x77,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 83
0x80,0x00,0x00,0x00,0x00,0x00,0x06,0x9b,0xfd,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 84
0x80,0x00,0x00,0x00,0x00,0x00,0x03,0x5f,0xee,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 85
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0xe7,0x7d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 86
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x52,0xfb,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 87
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x1d,0xdf,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 88
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x6d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 89
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdb,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 90
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3e,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 91
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 92
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0a,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 93
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0d,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 94
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 95
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xb0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 96
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 97
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 98
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 99
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 100
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xf9,0xc3,0x5c,0x74,0x00,0x01, // 101
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x09,0xa4,0xe9,0x70,0x00,0x01, // 102
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xe8,0x17,0xb8,0xe2,0x00,0x01, // 103
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xe8,0x7c,0x30,0x6c,0x00,0x01, // 104
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xea,0x67,0x0b,0xe2,0x00,0x01, // 105
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x08,0x71,0x66,0x32,0x00,0x01, // 106
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xfb,0x83,0x1a,0xac,0x00,0x01, // 107
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x63,0x46,0x3c,0x00,0x01, // 108
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xde,0x94,0x8f,0xe4,0x00,0x01, // 109
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc7,0xc7,0x3e,0xf2,0x00,0x01, // 110
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xd8,0x86,0x5e,0x30,0x00,0x01, // 111
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x02,0xbe,0xa1,0x88,0x00,0x01, // 112
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0xfd,0x29,0x48,0x00,0x01, // 113
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x40,0xa1,0x73,0x50,0x00,0x01, // 114
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x8a,0x2e,0x72,0x6c,0x00,0x01, // 115
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x04,0x9c,0xa2,0x28,0x00,0x01, // 116
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x5d,0xbf,0x43,0x98,0x00,0x01, // 117
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x93,0x07,0x3d,0xc2,0x00,0x01, // 118
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x58,0x5b,0x92,0x1e,0x00,0x01, // 119
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x71,0xac,0xf6,0x66,0x00,0x01, // 120
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xbc,0xb5,0x91,0xfa,0x00,0x01, // 121
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x12,0x08,0x19,0xd8,0x00,0x01, // 122
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xbb,0x3c,0x5d,0x5c,0x00,0x01, // 123
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x63,0xf7,0x94,0x00,0x01, // 124
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2e,0x0a,0x71,0x98,0x00,0x01, // 125
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x66,0x14,0x00,0x00,0x01, // 126
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xfa,0xaa,0xaa,0xfe,0x00,0x01, // 127
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x09,0x4e,0x9c,0x82,0x00,0x01, // 128
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xe8,0x59,0x0c,0xba,0x00,0x01, // 129
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xea,0x94,0x40,0xba,0x00,0x01, // 130
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xea,0x52,0xc2,0xba,0x00,0x01, // 131
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x09,0x6f,0xba,0x82,0x00,0x01, // 132
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xf8,0x8a,0x2c,0xfe,0x00,0x01, // 133
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 134
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 135
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 136
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 137
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 138
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 139
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 140
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 141
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 142
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 143
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 144
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 145
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 146
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 147
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 148
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 149
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 150
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, // 151
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 152


};

const uint8_t redBuffer [] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 1
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 2
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 3
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 4
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 5
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 6
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 7
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 8
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 9
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 10
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 11
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 12
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 13
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 14
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 15
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 16
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 17
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 18
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 19
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 20
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 21
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 22
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 23
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 24
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 25
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 26
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 27
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 28
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 29
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 30
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 31
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 32
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xf8,0x00,0x03,0xf8,0x00,0x00, // 33
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xe0,0x00,0x00,0xf8,0x00,0x00, // 34
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x00,0x00,0x78,0x00,0x00, // 35
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x00,0x00,0x78,0x00,0x00, // 36
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x38,0x00,0x00, // 37
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x38,0x00,0x00, // 38
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x87,0xff,0xfc,0x38,0x00,0x00, // 39
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x87,0xff,0xfc,0x38,0x00,0x00, // 40
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x38,0x00,0x00, // 41
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x38,0x00,0x00, // 42
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x00,0x00,0x78,0x00,0x00, // 43
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x00,0x00,0x78,0x00,0x00, // 44
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xe0,0x00,0x00,0xf8,0x00,0x00, // 45
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xf8,0x00,0x03,0xf8,0x00,0x00, // 46
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 47
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 48
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 49
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xf8,0x00,0x03,0xf8,0x00,0x00, // 50
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xe0,0x00,0x00,0xf8,0x00,0x00, // 51
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x00,0x00,0x78,0x00,0x00, // 52
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x00,0x00,0x78,0x00,0x00, // 53
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x38,0x00,0x00, // 54
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x38,0x00,0x00, // 55
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x87,0xff,0xfc,0x38,0x00,0x00, // 56
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x87,0xff,0xfc,0x38,0x00,0x00, // 57
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x38,0x00,0x00, // 58
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x38,0x00,0x00, // 59
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x00,0x00,0x78,0x00,0x00, // 60
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x00,0x00,0x78,0x00,0x00, // 61
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xe0,0x00,0x00,0xf8,0x00,0x00, // 62
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xf8,0x00,0x03,0xf8,0x00,0x00, // 63
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 64
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 65
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 66
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xf0,0x38,0x03,0xf8,0x00,0x00, // 67
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xe0,0x10,0x00,0xf8,0x00,0x00, // 68
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x10,0x00,0x78,0x00,0x00, // 69
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x78,0x00,0x00, // 70
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x38,0x00,0x00, // 71
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x38,0x00,0x00, // 72
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x87,0xc3,0xfc,0x38,0x00,0x00, // 73
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x87,0xc3,0xfc,0x38,0x00,0x00, // 74
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0xc3,0x00,0x38,0x00,0x00, // 75
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0xff,0x00,0x38,0x00,0x00, // 76
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0xff,0x00,0x78,0x00,0x00, // 77
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0xff,0x00,0x78,0x00,0x00, // 78
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0xff,0x00,0xf8,0x00,0x00, // 79
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xe0,0xff,0x01,0xf8,0x00,0x00, // 80
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 81
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 82
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xff,0xff,0xf8,0x00,0x00, // 83
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 84
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 85
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 86
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 87
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 88
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 89
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 90
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 91
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 92
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 93
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 94
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 95
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 96
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 97
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 98
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 99
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 100
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 101
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 102
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 103
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 104
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 105
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 106
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 107
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 108
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 109
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 110
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 111
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 112
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 113
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 114
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 115
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 116
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 117
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 118
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 119
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 120
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 121
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 122
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 123
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 124
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 125
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 126
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 127
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 128
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 129
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 130
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 131
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 132
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 133
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 134
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 135
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 136
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 137
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 138
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 139
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 140
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 141
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 142
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 143
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 144
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 145
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 146
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 147
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 148
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 149
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 150
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 151
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 152

};
