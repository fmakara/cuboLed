//#include <PS2X_lib.h>  
//PS2X ps2x; 
//int error = 0;


 
#define BOTAO1 2
#define BOTAO2 3
#define BOTAO3 4
#define BOTAO4 5
#define BOTAO5 6
#define BOTAO6 7
void setup() {
  randomSeed(analogRead(0));
  CUBO_iniciar();
  Serial.begin(115200);
  //error = ps2x.config_gamepad(4,5,6,7, false, false);
   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
}

void loop() {
  /*if(error==12){
    Serial.println("ERRO: nao encontrado");
    for(int k=0;k<33;k++){
      CUBO_clearScreen();
      GRAFICO_printStr("ERROE",-k, 0,0 ,true, 1, 0, 0,0,0,1);
      GRAFICO_printStr("OERRO",0,7+k,0 ,true, 0,-1, 0,0,0,1);
      GRAFICO_printStr("ROERR",7+k,7,0 ,true,-1, 0, 0,0,0,1);
      GRAFICO_printStr("RROER",  7,-k,0 ,true, 0, 1, 0,0,0,1);
      delay(100);
      if(k==32)k=0;
    }
  }else{*/
    
    
    //equalizador(1);
    Serial.println("Chuva");
    chuva();
    Serial.println("Snake");
    jogarSnake(false);
    while(Serial.available())Serial.read();
    Serial.println("PET");
    for(int k=0;k<33;k++){
      CUBO_clearScreen();
      GRAFICO_printStr("PET P",-k, 0,0 ,true, 1, 0, 0,0,0,1);
      GRAFICO_printStr(" PET ",0,7+k,0 ,true, 0,-1, 0,0,0,1);
      GRAFICO_printStr("T PET",7+k,7,0 ,true,-1, 0, 0,0,0,1);
      GRAFICO_printStr("ET PE",  7,-k,0 ,true, 0, 1, 0,0,0,1);
      delay(100);
      if(k==32)k=0;
      if(Serial.available()){
        Serial.read();
        k = 33;
      }
      if(!digitalRead(BOTAO1)){
        while(!digitalRead(BOTAO1));
        return;
      }
    }
  //}
}

