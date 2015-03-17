//Ispirato da https://arduino-tweet.appspot.com/


#if defined(ARDUINO) && ARDUINO > 18   // Arduino 0019 or later
#include <SPI.h>
#endif
#include <Ethernet.h>
#include <Twitter.h>
#include <SD.h>

//Definisco la lunghezza massima del tweet
#define len 140

//Definisco il numero massimo di tweet
#define a 10

//definisco il mac e l'indirizzo ip dell' ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 0, 177 };


//Definisco il token di accesso della app che avremo autorizzato con il nostro profilo.

Twitter twitter("<<TOKEN HERE>>");

File myFile;

char msg[len];

struct messaggi{
  String mess;
} messaggio[a];

int ns=0;
int nt=0;

void setup(){
  delay(1000);
  Ethernet.begin(mac, ip);
  Serial.begin(9600);

  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
   
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  }
  
void loop(){
  if(ns<=nt){
    if(ns==0){
      creaMatrice();
    }
      creaMessaggio(ns);
      delay(1000);
      
      Serial.println("connecting ...");
      if (twitter.post(msg)) {
      int status = twitter.wait();
      if (status == 200) {
        Serial.println("OK.");
      } else {
        Serial.print("failed : code ");
        Serial.println(status);
      }
    } else {
      Serial.println("connection failed.");
    }
    Serial.print("Pubblicato numero: ");
    Serial.println(ns);
    ns++;
    Serial.print("Prossimo stato: ");
    Serial.println(ns);  
    //aspetto 5 minuti
    delay(150000);
   }
   delay(150000);
   Serial.println("I tweet da inserire sono finiti,modificare il file di testo"); 
}

void creaMatrice(){
  char ch;
  int c=0;
  myFile = SD.open("stati.txt");
  if (myFile) {
    Serial.println("Apro stati.txt:");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()){
       ch = myFile.read();
       if(ch!='/'){
         messaggio[c].mess += ch; // questo accumula i char dentro la stringa
       }
       else{
         c++;
       }
    }
  myFile.close();
  } 
  else {
  	// if the file didn't open, print an error:
    Serial.println("error opening stati.txt");
  }
  Serial.println("Matrice tweet creata.");
  //numero tweet
  nt=c;
}

void creaMessaggio(int n){
  messaggio[n].mess.toCharArray(msg,len);
  Serial.print("<--");
  Serial.print(msg);
  Serial.println("-->");
}
