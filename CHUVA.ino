void chuva(){
  while(1){
    for(int i=0;i<7;i++){
      for(int j=0;j<8;j++){
        for(int k=0;k<8;k++){
          CUBO_point(k,j,i, CUBO_getPoint(k,j,i+1));
        }
      }
    }
    for(int j=0;j<8;j++){
      for(int k=0;k<8;k++){
        CUBO_clearPoint(k,j,7);
      }
    }
    for(int i=0;i<4;i++){
      CUBO_setPoint(random(0,8), random(0,8), 7);
    }
    //ps2x.read_gamepad(false, false);
    //if(ps2x.ButtonPressed(PSB_GREEN)){
    //  Serial.println("Saiu chuva");
    //  return;
    //}
    if(Serial.available()){
      Serial.read();
      return;
    }
    if(!digitalRead(BOTAO1)){
      while(!digitalRead(BOTAO1));
      return;
    }
    delay(100);
  }
}

