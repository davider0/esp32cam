#include <cstdlib>
#include <iostream>
#include <string.h>
using namespace std;
void flash(int tiempoIn, int tiempoOut){
    digitalWrite(4, HIGH);   // Led ON
    delay(tiempoIn);              // pausa 1 seg
    digitalWrite(4, LOW);    // Led OFF
    delay(tiempoOut);  
}


string convertToMorse(string str){
  char ascii[28] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ", lwcAscii[28] = " abcdefghijklmnopqrstuvwxyz";
  string morse[27] = {"  ", ".- ", "-... ", "-.-. ", "-.. ", ". ", "..-. ", "--. ", ".... ", ".. ", ".--- ", "-.- ", ".-.. ", "-- ", "-. ", "--- ", ".--.", "--.- ", ".-. ", "... ", "- ", "..- ", "...- ", ".-- ", "-..- ", "-.-- ", "--.. "};
  string outpt;
  int xx=0;
  int size = str.length();
  cout<<"Length:"<<size<<endl;
  while(xx<str.length())
    {
       int x=0;
        bool working = false;
        while(!working)
        {
           if(ascii[x] != str[xx]&&lwcAscii[x] != str[xx])
           {      
             x++;
           } else {
              working = !working;
          }
        }             
        cout<<morse[x];
        outpt = outpt + morse[x];
        xx++;
    }

  while(xx<outpt.length()+1){
      if(outpt[xx] == '.')
      {
          flash(250,250);
      } else {
        if(outpt[xx] == '-'){
            flash(1000,500);
        } else {
        if(outpt[xx] == ' '){
           flash(0,500);
        }   
       }    
    }
     xx++; 
  }
}
void setup() {
  pinMode(4, OUTPUT);
}


void loop() {
  convertToMorse("hola");
}



