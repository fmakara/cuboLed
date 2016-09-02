#define SNAKE_BTN_PIN_UP 2
#define SNAKE_BTN_PIN_DN 3
#define SNAKE_BTN_PIN_LF 4
#define SNAKE_BTN_PIN_RI 5
#define SNAKE_BTN_PIN_FR 6
#define SNAKE_BTN_PIN_BA 7

#define SNAKE_MAXSIZE 256
#define SNAKE_DELAY_INICIAL 350
#define SNAKE_DELAY_DECREMENTO 4
#define SNAKE_DELAY_MINIMO 100


#define SNAKE_BTN_NO 0
#define SNAKE_BTN_UP (1<<0)
#define SNAKE_BTN_DN (1<<1)
#define SNAKE_BTN_LF (1<<2)
#define SNAKE_BTN_RI (1<<3)
#define SNAKE_BTN_FR (1<<4)
#define SNAKE_BTN_BA (1<<5)
/*char lerJoystickSnake(){
  char ret = 0;
  if(!digitalRead(SNAKE_BTN_PIN_UP))ret|=SNAKE_BTN_UP;
  if(!digitalRead(SNAKE_BTN_PIN_DN))ret|=SNAKE_BTN_DN;
  if(!digitalRead(SNAKE_BTN_PIN_LF))ret|=SNAKE_BTN_LF;
  if(!digitalRead(SNAKE_BTN_PIN_RI))ret|=SNAKE_BTN_RI;
  if(!digitalRead(SNAKE_BTN_PIN_FR))ret|=SNAKE_BTN_FR;
  if(!digitalRead(SNAKE_BTN_PIN_BA))ret|=SNAKE_BTN_BA;
  return ret;
}*/
char lerJoystickSnake(){
  char retorno = SNAKE_BTN_NO;
  if(Serial.available()){
    char c = Serial.read();
    switch(c){
      case 'i':return SNAKE_BTN_UP;
      case 'k':return SNAKE_BTN_DN;
      case 'a':return SNAKE_BTN_LF;
      case 'd':return SNAKE_BTN_RI;
      case 'w':return SNAKE_BTN_FR;
      case 's':return SNAKE_BTN_BA;
    }
  }
  if(!digitalRead(BOTAO1))retorno |= SNAKE_BTN_UP;
  if(!digitalRead(BOTAO2))retorno |= SNAKE_BTN_DN;
  if(!digitalRead(BOTAO3))retorno |= SNAKE_BTN_LF;
  if(!digitalRead(BOTAO4))retorno |= SNAKE_BTN_RI;
  if(!digitalRead(BOTAO5))retorno |= SNAKE_BTN_FR;
  if(!digitalRead(BOTAO6))retorno |= SNAKE_BTN_BA;
  
  return retorno;
}
inline void snakeDesenhar(char* cobra, int tamanho){
  for(int i=0;i<tamanho;i++){
    CUBO_setPoint(cobra[i*3+0],cobra[i*3+1],cobra[i*3+2]);
  }
}
inline void snakeAndar(char* cobra, int tamanho, char direcao){
  //Posicao 0 eh a cabeca
  char buff[3];
  buff[0] = cobra[0];
  buff[1] = cobra[1];
  buff[2] = cobra[2];
  switch(direcao){
    case SNAKE_BTN_UP:buff[2]++;if(buff[2]>7)buff[2]=0;break;
    case SNAKE_BTN_DN:buff[2]--;if(buff[2]<0)buff[2]=7;break;
    case SNAKE_BTN_RI:buff[0]++;if(buff[0]>7)buff[0]=0;break;
    case SNAKE_BTN_LF:buff[0]--;if(buff[0]<0)buff[0]=7;break;
    case SNAKE_BTN_FR:buff[1]++;if(buff[1]>7)buff[1]=0;break;
    case SNAKE_BTN_BA:buff[1]--;if(buff[1]<0)buff[1]=7;break;
  }
  for(int i=tamanho;i>0;i--){
    cobra[i*3+0] = cobra[i*3-3];
    cobra[i*3+1] = cobra[i*3-2];
    cobra[i*3+2] = cobra[i*3-1];
  }
  cobra[0] = buff[0];
  cobra[1] = buff[1];
  cobra[2] = buff[2];
}
bool snakeTestar(char* cobra, int tamanho, bool modo){
  for(int i=1;i<tamanho;i++){
    if(cobra[0]==cobra[i*3+0] && cobra[1]==cobra[i*3+1] && cobra[2]==cobra[i*3+2])return false;
    if(modo){
      if( (cobra[i*3-3]==7&&cobra[i*3+0]==0) ||
          (cobra[i*3-3]==0&&cobra[i*3+0]==7) ||
          (cobra[i*3-2]==7&&cobra[i*3+1]==0) ||
          (cobra[i*3-2]==0&&cobra[i*3+1]==7) ||
          (cobra[i*3-1]==7&&cobra[i*3+2]==0) ||
          (cobra[i*3-1]==0&&cobra[i*3+2]==7) )return false;
    }
  }
  return true;
}
inline void gerarFruta(char* fruta, char* cobra, int tamanho){
  bool ok = false;
  while(!ok){
    fruta[0] = random(0,8); fruta[1] = random(0,8); fruta[2] = random(0,8);
    ok = true;
    for(int i=0;i<tamanho && ok;i++)
      if(fruta[0]==cobra[i*3+0] && fruta[1]==cobra[i*3+1] && fruta[2]==cobra[i*3+2]) ok=false;
  }
}
void jogarSnake(bool modo){
  char cobra[SNAKE_MAXSIZE][3], fruta[3], direcao = SNAKE_BTN_LF, teclas=SNAKE_BTN_NO, apertadas=SNAKE_BTN_NO;
  int tamanho = 2;
  char string[8] = "       ";
  bool finalizado = false;
  cobra[0][0]=4;  cobra[0][1]=4;  cobra[0][2]=4;
  cobra[1][0]=4;  cobra[1][1]=4;  cobra[1][2]=5;
  /*pinMode(SNAKE_BTN_PIN_UP,INPUT_PULLUP);
  pinMode(SNAKE_BTN_PIN_DN,INPUT_PULLUP);
  pinMode(SNAKE_BTN_PIN_LF,INPUT_PULLUP);
  pinMode(SNAKE_BTN_PIN_RI,INPUT_PULLUP);
  pinMode(SNAKE_BTN_PIN_FR,INPUT_PULLUP);
  pinMode(SNAKE_BTN_PIN_BA,INPUT_PULLUP);*/
  
  gerarFruta(fruta,cobra[0],tamanho);
  CUBO_clearScreen();
  GRAFICO_letra('3',0,0,7,true,1,0,0,0,1,0);
  snakeDesenhar(cobra[0],tamanho);
  CUBO_setPoint(fruta[0],fruta[1],fruta[2]);
  delay(500);
  
  CUBO_clearScreen();
  GRAFICO_letra('2',0,0,7,true,1,0,0,0,1,0);
  snakeDesenhar(cobra[0],tamanho);
  CUBO_setPoint(fruta[0],fruta[1],fruta[2]);
  delay(500);
  
  CUBO_clearScreen();
  GRAFICO_letra('1',0,0,7,true,1,0,0,0,1,0);
  snakeDesenhar(cobra[0],tamanho);
  CUBO_setPoint(fruta[0],fruta[1],fruta[2]);
  delay(500);
  
  while(!finalizado){
    //Checar direcao do teclado e verificar se nao esta contra a atual
    teclas = lerJoystickSnake();
    if((teclas&SNAKE_BTN_UP)&&!(apertadas&SNAKE_BTN_UP)&&direcao!=SNAKE_BTN_DN)direcao = SNAKE_BTN_UP;
    if((teclas&SNAKE_BTN_DN)&&!(apertadas&SNAKE_BTN_DN)&&direcao!=SNAKE_BTN_UP)direcao = SNAKE_BTN_DN;
    if((teclas&SNAKE_BTN_LF)&&!(apertadas&SNAKE_BTN_LF)&&direcao!=SNAKE_BTN_RI)direcao = SNAKE_BTN_LF;
    if((teclas&SNAKE_BTN_RI)&&!(apertadas&SNAKE_BTN_RI)&&direcao!=SNAKE_BTN_LF)direcao = SNAKE_BTN_RI;
    if((teclas&SNAKE_BTN_FR)&&!(apertadas&SNAKE_BTN_FR)&&direcao!=SNAKE_BTN_BA)direcao = SNAKE_BTN_FR;
    if((teclas&SNAKE_BTN_BA)&&!(apertadas&SNAKE_BTN_BA)&&direcao!=SNAKE_BTN_FR)direcao = SNAKE_BTN_BA;
    
    snakeAndar(*cobra,tamanho,direcao);
    if(fruta[0]==cobra[0][0] && fruta[1]==cobra[0][1] && fruta[2]==cobra[0][2]){
      tamanho++;
      gerarFruta(fruta,*cobra,tamanho);
      Serial.print("Fruta: ");
      Serial.print(fruta[0]);
      Serial.print(":");
      Serial.print(fruta[1]);
      Serial.print(":");
      Serial.println(fruta[2]);
    }
    if(!snakeTestar(*cobra,tamanho,modo))finalizado = true;
    if(tamanho==SNAKE_MAXSIZE)finalizado = true;
    CUBO_clearScreen();
    snakeDesenhar(cobra[0],tamanho);
    CUBO_setPoint(fruta[0],fruta[1],fruta[2]);
    delay(max(SNAKE_DELAY_INICIAL-tamanho*SNAKE_DELAY_DECREMENTO,SNAKE_DELAY_MINIMO));
  }
  if(tamanho==SNAKE_MAXSIZE){//Ganhou
    delay(1000);
    CUBO_clearScreen();
    snakeDesenhar(cobra[0],0);
    delay(800);
    CUBO_clearScreen();
    snakeDesenhar(cobra[0],tamanho);
    delay(800);
    CUBO_clearScreen();
    snakeDesenhar(cobra[0],0);
    delay(800);
    CUBO_clearScreen();
    snakeDesenhar(cobra[0],tamanho);
    delay(800);
  }else{//perdeu
    delay(1000);
    for(int i=tamanho;i>0;i--){
      CUBO_clearScreen();
      snakeDesenhar(cobra[0],i);
      CUBO_setPoint(fruta[0],fruta[1],fruta[2]);
      delay(100);
    }
  }
  int u = tamanho%10;
  int d = (tamanho/10)%10;
  int c = (tamanho/100)%10;
  if(c>0){
    string[0] = '0'+c;
    string[4] = '0'+c;
  }
  if(d>0||c>0){
    string[1] = '0'+d;
    string[5] = '0'+d;
  }
  string[2] = '0'+u;
  string[6] = '0'+u;
  int k=0;
  while(1){
    CUBO_clearScreen();
    GRAFICO_printStr(string+3,-k, 0,0 ,true, 1, 0, 0,0,0,1);
    GRAFICO_printStr(string+2,0,7+k,0 ,true, 0,-1, 0,0,0,1);
    GRAFICO_printStr(string+1,7+k,7,0 ,true,-1, 0, 0,0,0,1);
    GRAFICO_printStr(string+0, 7,-k,0 ,true, 0, 1, 0,0,0,1);
    if(Serial.available()){
      Serial.read();
      return;
    }
    if(!digitalRead(BOTAO1)){
      while(!digitalRead(BOTAO1));
      return;
    }
    k++;
    if(k==32)k=0;
    delay(100);
  }
  
}

