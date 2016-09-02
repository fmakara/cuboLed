//Funcoes:
//void CUBO_iniciar()
//void CUBO_clearScreen()
//void CUBO_point(char x, char y, char z, bool p)
//void CUBO_setPoint(char x, char y, char z)
//void CUBO_clearPoint(char x, char y, char z)
//void CUBO_togglePoint(char x, char y, char z)
//char CUBO_getPoint(char x, char y, char z)




//Renderizador
//Usa interrupcoes de timer1
#include <avr/pgmspace.h>
#define SERIAL_CLK    13
#define SERIAL_STROBE 12
#define SERIAL_DATA   11
#define SIGNAL_ON  LOW
#define SIGNAL_OFF HIGH

#define ATUALIZACOES_POR_SEGUNDO 60
#define NOP() __asm__("nop\n\t")
//LAYER_POS tem a posicao das layers em funcao dos pinos
//Ex. 3,2,1..., pino0 esta em layer 3, pino1 esta em layer2...
const unsigned char LAYER_POS[8] PROGMEM = {
  7, 6, 5, 4, 3, 2, 1, 0//    3, 2, 1, 0, 4, 6, 5, 7
};
//LED_POS tem a posicao do LED ligado ao pino
//Ex. 0x00, 0x01, 0x30 tem os pinos (0,1,2) ligados em 
//[][0]=1<<0, [][0]=1<<1, [][3]=1<<0...
const unsigned char LED_POS[64] PROGMEM = {
  0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,
  0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71,
  0x02, 0x12, 0x22, 0x32, 0x52, 0x42, 0x62, 0x72,//4 e 5 trocados
  0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73,
  0x04, 0x14, 0x24, 0x34, 0x44, 0x54, 0x64, 0x74,
  0x05, 0x15, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75,
  0x06, 0x16, 0x26, 0x36, 0x46, 0x56, 0x66, 0x76,
  0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77
};
//BUFFER_CUBO tem os leds dispostos da maneira: [z][y] = 1<<x
unsigned char BUFFER_CUBO[8][8], LAYER_ATUAL;
void CUBO_clearScreen(){
  unsigned char *b=BUFFER_CUBO[0];
  for(int i=0;i<64;i++,b++)*b=0;
}
void CUBO_iniciar(){
  TCCR1A = 0;
  TCCR1B = (1<<WGM12)|(1<<CS11)|(1<<CS10);//CTC, /64
  OCR1A = 31250/ATUALIZACOES_POR_SEGUNDO;//=16M/(8*64*ATUA)
  TIMSK1 = (1 << OCIE1A);
  sei();
  LAYER_ATUAL = 0;
  CUBO_clearScreen();
  pinMode(SERIAL_CLK,OUTPUT);
  pinMode(SERIAL_STROBE,OUTPUT);
  pinMode(SERIAL_DATA,OUTPUT);

  pinMode(BOTAO1,INPUT_PULLUP);
  pinMode(BOTAO2,INPUT_PULLUP);
  pinMode(BOTAO3,INPUT_PULLUP);
  pinMode(BOTAO4,INPUT_PULLUP);
  pinMode(BOTAO5,INPUT_PULLUP);
  pinMode(BOTAO6,INPUT_PULLUP);
}
ISR(TIMER1_COMPA_vect){
  unsigned char i, j;
  //Primeiro sao shiftados as layers, 1 para acionar
  digitalWrite(SERIAL_STROBE, SIGNAL_OFF);
  digitalWrite(SERIAL_CLK, SIGNAL_OFF);
  for(i=0;i<8;i++){
    if(pgm_read_byte_near(LAYER_POS+7-i)==LAYER_ATUAL)digitalWrite(SERIAL_DATA,SIGNAL_OFF);
    else digitalWrite(SERIAL_DATA,SIGNAL_ON);
    NOP();
    digitalWrite(SERIAL_CLK, SIGNAL_ON);
    NOP();
    digitalWrite(SERIAL_CLK, SIGNAL_OFF);
  }
  //Entao sao shiftadas as camadas, ao contrario
  for(i=0;i<64;i++){
    j = pgm_read_byte_near(LED_POS+63-i);
    if(BUFFER_CUBO[LAYER_ATUAL][(j>>4)&0x0F] & (1<<(j&0x0F)) )digitalWrite(SERIAL_DATA,SIGNAL_ON);
    else digitalWrite(SERIAL_DATA,SIGNAL_OFF);
    NOP();
    digitalWrite(SERIAL_CLK, SIGNAL_ON);
    NOP();
    digitalWrite(SERIAL_CLK, SIGNAL_OFF);
  }
  digitalWrite(SERIAL_STROBE, SIGNAL_ON);
  NOP();
  digitalWrite(SERIAL_STROBE, SIGNAL_OFF);
  LAYER_ATUAL++;
  if(LAYER_ATUAL==8)LAYER_ATUAL=0;
}
void CUBO_point(char x, char y, char z, bool p){
  if(x>7||x<0||y>7||y<0||z>7||z<0)return;
  if(p)BUFFER_CUBO[z][y]|=  1<<x;
  else BUFFER_CUBO[z][y]&=~(1<<x);
}
void CUBO_setPoint(char x, char y, char z){
  if(x>7||x<0||y>7||y<0||z>7||z<0)return;
  BUFFER_CUBO[z][y]|=1<<x;
}
void CUBO_clearPoint(char x, char y, char z){
  if(x>7||x<0||y>7||y<0||z>7||z<0)return;
  BUFFER_CUBO[z][y]&=~(1<<x);
}
void CUBO_togglePoint(char x, char y, char z){
  if(x>7||x<0||y>7||y<0||z>7||z<0)return;
  BUFFER_CUBO[z][y]^=1<<x;
}
char CUBO_getPoint(char x, char y, char z){
  if(x>7||x<0||y>7||y<0||z>7||z<0)return -1;
  if(BUFFER_CUBO[z][y]&(1<<x))return 1;
  return 0;
}
