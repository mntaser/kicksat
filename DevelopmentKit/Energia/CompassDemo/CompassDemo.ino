#include <SpriteRadio.h>
#include <SpriteMag.h>

unsigned char prn0[64] = {
  0b10101010, 0b11110111, 0b11100111, 0b10100100, 0b11111001, 0b01111101, 0b00000010, 0b11000100,
  0b11001110, 0b11110101, 0b10110111, 0b01010110, 0b10010111, 0b01000100, 0b00010000, 0b11010111,
  0b01100001, 0b00011111, 0b11110001, 0b10000010, 0b10001110, 0b11010000, 0b10111000, 0b01110000,
  0b01110100, 0b11010101, 0b00110001, 0b11101101, 0b10011111, 0b10111011, 0b10011100, 0b11000011,
  0b00010111, 0b10011010, 0b00110010, 0b00000001, 0b00101000, 0b01111001, 0b00110111, 0b00011100,
  0b10001001, 0b00001001, 0b11010111, 0b11110101, 0b00100011, 0b01101010, 0b00001100, 0b11001010,
  0b01010100, 0b01010011, 0b10001010, 0b11100101, 0b01100000, 0b00110111, 0b10001000, 0b10110101,
  0b10010010, 0b01011001, 0b10110001, 0b10100111, 0b10000001, 0b11110110, 0b01011011, 0b11100000
};
unsigned char prn1[64] = {
  0b10101010, 0b10000001, 0b01001010, 0b11110010, 0b11101110, 0b00000111, 0b00111010, 0b01001111,
  0b01011101, 0b01000100, 0b10000110, 0b01110000, 0b10111101, 0b10110011, 0b01000011, 0b10111100,
  0b00111111, 0b11100000, 0b11110111, 0b11000101, 0b11001100, 0b10000010, 0b01010011, 0b10110100,
  0b01111001, 0b11110011, 0b01100010, 0b10100100, 0b01110001, 0b10110101, 0b01110001, 0b00110001,
  0b00010000, 0b00001000, 0b01000110, 0b00010011, 0b10010101, 0b01100001, 0b10111101, 0b00110111,
  0b00100010, 0b10000101, 0b01101001, 0b11111011, 0b00100100, 0b10110111, 0b11100100, 0b11010100,
  0b11001100, 0b00000110, 0b00110010, 0b10001101, 0b00101111, 0b11101000, 0b10110001, 0b11010110,
  0b01011001, 0b11100011, 0b11101110, 0b10000011, 0b01011011, 0b01110110, 0b00001011, 0b01011110
};

//Initialize the radio class, supplying the Gold Codes that correspond to 0 and 1
SpriteRadio radio = SpriteRadio(prn0, prn1);

SpriteMag mag = SpriteMag();

float declination = 5*(PI/180);

void setup() {
  pinMode(5, OUTPUT);
  radio.txInit();
  mag.init();
  Serial.begin(9600);
}

void loop() {
  //Blink LED while we're doing stuff
  digitalWrite(5, HIGH);
  
  MagneticField b = mag.read();
  
  //Calculate heading in radians from magnetic field components
  float heading = atan2(b.y, b.x);
  
  //correct for declination
  heading += declination;
  
  //Scale between 0 and 2*PI
  if(heading < 0) heading += 2*PI;
  if(heading > 2*PI) heading -= 2*PI;
  
  //Convert to degrees
  heading = heading*(180/PI);
  
  int heading_int = int(heading);
  int heading_decimal = int((heading-heading_int)*10);
  
  char string[7];
  
  sprintf(string,"%3i %1i",heading_int,heading_decimal);
  string[3] = '.'; //Weirdness with handling of '.' character in sprintf
  string[5] = '\n'; //Add newline to end of string
  
  /*
  Serial.print(heading_int);
  Serial.print(".");
  Serial.println(heading_decimal);
  */
  
  Serial.print(string);

  radio.transmit(string,6);
  
  digitalWrite(5,LOW);
  
  delay(500);
}
