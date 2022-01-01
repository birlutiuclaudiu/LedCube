#include<avr/io.h>
#include<avr/interrupt.h>
#include "pitches.h"
#include "melodies.h"

//valoare ce indica optiunea de rulat
volatile char option = '0';

//setarea bititlor din port-uri 
const unsigned char setBit[] = {0b00000001, 0b00000010, 0b00000100, 0b00001000,
0b00010000, 0b00100000, 0b01000000, 0b10000000};

//pentru datele citite de la bt
char dataRead[100]="";


//varibile pentru porturi
unsigned char layer =0x00;
unsigned char colA= 0x00;
unsigned char colC= 0x00;

//definire contururi 
int contur[] = {3,7,11,15, 14,13,12,8,4,0,1,2}; //extern
int intContur[] = {5,6,9,10};                  //contur intern

void setup() {
    //setarea porturilor de iesire
    DDRA = 0xFF;     //primele 8 coloane
    DDRC = 0xFF;     //restul de 8 coloane
    DDRB = 0xFF;     //pentru nivel

    Serial1.begin(9600);
    //setarea timer pentru intreruperi cu scopul de a verifica daca s-a primit ceva pe linia seriala de la bt
    cli();  // facem disable la intreruperile globale
    TCCR1A = 0; // setam TCCR1A si B la 0
    TCCR1B = 0;
    //setam registrul cu valoarea caruia vom compara TCNT
    OCR1A = 15624;     //se face citirea datelor la o secunda
     //activam modul CTC:
    TCCR1B |= (1 << WGM12);
    //divizam ceasul placii cu 1024:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    //facem enable la intreruperea la comparare prin setarea bitului
    //corespunzator din masca
    TIMSK1 |= (1 << OCIE1A);
    //validam sistemul global d eintreruperi
    sei();
}

void initialise(){
  //se vor initializa toate porturile la valoare 0 pe iesiri
  PORTA = 0x00;
  PORTB = 0x00;
  PORTC = 0x00;
}

void loop() {
  delayMicroseconds(1000);
  switch (option) {
    case '0':
      function1();
      break;
    case '1':
      function2();
      break;
    case '2':
      function3();
      break;
    case '3':
      function4();
      break;
    case '4':
      function5();
      break;
    case '5':
      function6();
      break;
    case '6':
      function7();
      break;
    default:
      break;
  }
}

ISR(TIMER1_COMPA_vect)
{ 
  
  int availableBytes = Serial1.available();
  for(int i=0; i<availableBytes; i++){
      dataRead[i] = Serial1.read();
  }
 
   option = dataRead[0];
}



void function1(){
  while(option =='0'){
    initialise();
    songLayer();
  }
}
void function2(){
  while(option =='1'){
    initialise();
    songGodFather();
  }
}

void function3(){
  
  while(option == '2'){
   initialise();
   songLion();
  }
}

void function4(){
  initialise();
  PORTA = 0xFF; 
  PORTC = 0xFF; 
  while(option == '3'){
   PORTB = 0xFF;
   delay(200); 
   PORTB = 0x00;
   delay(200); 
  }
}

void function5(){
  initialise();
  PORTA = 0xFF; 
  PORTC = 0x00;
  PORTB = 0xFF; 
  while(option == '4'){
   PORTB = 0xFF;
   PORTC =0x00;
   PORTA = 0x0F;
   delay(200); 
   PORTA = 0xFF;
   delay(200); 
   PORTC = 0x0F;
   delay(200); 
   PORTC = 0xFF;
   delay(200); 
   PORTB = 0xFE;
   delay(200); 
   PORTB = 0xFC;
   delay(200);
   PORTB = 0xF8;
   delay(200);
  }
}

void function6(){
  initialise();  
  while(option == '5'){
   PORTB = 0xFF;
   PORTA = 0xFF; 
   PORTC = 0xFF;
  }
}

void function7(){
  initialise();  
  while(option == '6'){
   PORTB = (unsigned char) (random(256));
   PORTA = (unsigned char) (random(256));
   PORTC = (unsigned char) (random(256));
   delay(250);
   
  }
}

void songLayer(){
  int dir =0;
  layer = 0x00;
  PORTA = 0xFF;
  PORTC = 0xFF;
  int row=0;
  int notes = sizeof(merryChristmas) / sizeof(merryChristmas[0]) / 2;
  for (int thisNote = 0; thisNote < 2*notes; thisNote+=2) {
    if(option!='0')
      return;
    //calculam durata de afisare a notei
    int noteDuration = 1000/abs(merryChristmas[thisNote+1]);
    if (thisNote < notes/2){
      if (thisNote % 1 ==0){
        if (layer == 0x00){
          layer = 0x01;
        }
        if(layer == 0x08){
          dir = -1;      //jos
        }else if(layer == 0x01){
          dir = 1;       //sus
        }
        if (dir ==1){
          layer = (layer << 1);    //in sus
        }else {
          layer = layer >> 1;   //in jos
        }
        PORTB = layer;  
      }
    }else if(thisNote == notes/2 or thisNote == notes/2+1){
          PORTB = 0x0F;
          PORTA = 0x00;
          PORTC = 0x00;
          dir = -1;
    }else{
        switch(row){
            case 0 : PORTA = 0x0F; PORTC = 0x00; dir = -dir; row+=dir; 
              break;
            case 1 : PORTA = 0xF0; PORTC = 0x00; row+=dir;
              break;
            case 2 : PORTA = 0x00; PORTC = 0x0F; row+=dir;    
              break;
            case 3 : PORTA = 0x00; PORTC = 0xF0; dir = -dir; row+=dir;    
              break;
            default:
              break;
         }
    }
    //apelam functia de tone pentru difuzorul atasat la pinul 8 si durata specificata
    tone(8, merryChristmas[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void songGodFather(){
  layer = 0x01;
  colA = 0x60;
  colC = 0x06;
  PORTB = layer; 
  PORTA = colA;
  PORTC = colC;
  
  int index = 0;
  int pos =0;    //pozitie de setat in porturi
  
  int notes = sizeof(godFather) / sizeof(godFather[0]) / 2;
  for (int thisNote = 0; thisNote < 2*notes; thisNote+=2) {
    if(option!='1')
      return;
    //calculam durata de afisare a notei
    int noteDuration = 1000/abs(godFather[thisNote+1]);
    if (index < 48){
      pos = index%12; 
      if (contur[pos] <8){
        PORTC = colC;
        PORTA = colA | setBit[contur[pos]];
      }
      else {
        PORTA = colA;
        PORTC = colC | setBit[contur[pos]-8];
      }
      if (pos==0 and index!=0){
        layer = layer * 2 + 1;
      }
        PORTB = layer;
        index++;
    }else if (index == 48){
      layer = 0xFF;
      colA = colA & 0; //resetare
      colC = colC & 0; //resetare
      index++;
    }else {
      if(index %2 ==1){
         colA = colA & 0; //resetare
         colC = colC & 0; //resetare
         layer = 0xFF;
         for (pos = 0; pos <12; pos++){
            if (contur[pos] <8) 
               colA = colA | setBit[contur[pos]];
            else 
               colC = colC | setBit[contur[pos]-8];
         }
      }else {
         colA = colA & 0; //resetare
         colC = colC & 0; //resetare
         for (pos = 0; pos <4; pos++){
            if (contur[pos] <8) 
              colA = colA | setBit[intContur[pos]];
            else 
              colC = colC | setBit[intContur[pos]-8];
         }
         layer = 0x06;
      }
      
      PORTA = colA; 
      PORTC = colC;
      PORTB = layer;
      index++;
    }
    
    //apelam functia de tone pentru difuzorul atasat la pinul 8 si durata specificata
    tone(8, godFather[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void songLion(){
  int dir = -1;
  layer = 0x01;
  colA = 0x00;
  colC = 0x00;
  int contor = 0;
  int contor2 = 0;
  int leds[] = {0,1,2,3,7,6,5,4,8,9,10,11,15,14,13,12};

  
  int notes = sizeof(gameOfThrones) / sizeof(gameOfThrones[0]) / 2;
  for (int thisNote = 0; thisNote < 2*notes; thisNote+=2) {
    if(option!='2')
      return;
    //calculam durata de afisare a notei
    int noteDuration = 1000/abs(gameOfThrones[thisNote+1]);
    ////////////////////////////////////////////////////////////////
    if(thisNote < notes){
      contor = contor % 16; 
      if (dir == 1) {
        int pos = 16-contor; 
        if( leds[pos] < 8)
          colA = colA | setBit[leds[pos]];
        else
          colC = colC | setBit[leds[pos]-8];
      }else{
        if( leds[contor] < 8)
          colA = colA | setBit[leds[contor]];
        else
          colC = colC | setBit[leds[contor]-8];
      }
      PORTA = colA; 
      PORTC = colC;
      PORTB = layer;
      contor += 1; 
      if(contor == 16){
        dir = -dir;
        if (layer == 0x08){
          dir = -dir;
          layer = 0x01;
        }
        else 
          layer = layer << 1;
        colC = 0x00; colA = 0x00;
      }
    }else {
      
      PORTB = 0xFF;
      if (contor2 % 4 == 0){
        PORTA = 0x0F;
        PORTC = 0x0F;
      }else if(contor2 % 6 == 1){
        PORTA = 0xF0;
        PORTC = 0x0F;
      }else if(contor2 % 10 == 2){
        PORTA = 0x0F;
        PORTC = 0xF0;
        
      }else{
        PORTA = 0xF0;
        PORTC = 0xF0;
      }
      contor2 +=1;
    }
   
    //////////////////////////////////////////////////////////////////
    //apelam functia de tone pentru difuzorul atasat la pinul 8 si durata specificata
    tone(8, gameOfThrones[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(8);
  }
}
