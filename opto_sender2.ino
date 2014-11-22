/*
 * Generic Sender code : Send a value (counter) over RF 433.92 mhz
 * Fréquence : 433.92 mhz
 * Protocole : homepi 
 * Licence : CC -by -sa
 * Auteur : Yves Grange
 * Version : 0.1
 * Lase update : 10/10/2014
 * 
 * Based on: Valentin CARRUESCO aka idleman and Manuel Esteban aka Yaug (http://manuel-esteban.com) work  
 */
 


#define senderPin 12

long codeKit = 1111;  // Your unique ID for your Arduino node
int BytesType[] = {0,0,1,0}; // Protocol Code: here it's 10d (1010b) can be what you want
int Bytes[30]; 
int BytesData[30]; 
boolean Timing = false; // Print timings on serial when eq to true
unsigned long CounterValue = 0;
volatile int state = LOW;
int pin = 13;

// This function will return the value you want to send over RF, insert here code to calculate the value you want to send over the air
boolean getCounter(unsigned long *CounterValue){
  *CounterValue += 1; 
  //*CounterValue = 67129; 
  return true;
}

void itob(unsigned long integer, int length)
{  
 for (int i=0; i<length; i++){
   if ((integer / power2(length-1-i))==1){
     integer-=power2(length-1-i);
     Bytes[i]=1;
   }
   else Bytes[i]=0;
 }
}

void itobCounter(unsigned long integer, int length)
{  
 for (int i=0; i<length; i++){
   if ((integer / power2(length-1-i))==1){
     integer-=power2(length-1-i);
     BytesData[i]=1;
   }
   else BytesData[i]=0;
 }
}

unsigned long power2(int power){    //gives 2 to the (power)
 unsigned long integer=1;          
 for (int i=0; i<power; i++){      
   integer*=2;
 }
 return integer;
}

/**
 * Crée notre signal sous forme binaire
**/
void buildSignal()
{
  Serial.println(codeKit);
  // Converti les codes respectifs pour le signal en binaire
  itob(codeKit, 14);
  for(int j=0;j < 14; j++){
   Serial.print(Bytes[j]);
  }
  Serial.println();
}

// Convert 0 in 01 and 1 in 10 (Manchester conversion)
void sendPair(bool b) {
 if(b)
 {
   sendBit(true);
   sendBit(false);
 }
 else
 {
   sendBit(false);
   sendBit(true);
 }
}

//Envois d'une pulsation (passage de l'etat haut a l'etat bas)
//1 = 310µs haut puis 1340µs bas
//0 = 310µs haut puis 310µs bas
void sendBit(bool b) {
 if (b) {
   digitalWrite(senderPin, HIGH);
   delayMicroseconds(506);   //305 orinally, but tweaked.
   if (Timing) Serial.print("506 ");
   digitalWrite(senderPin, LOW);
   delayMicroseconds(2024);  //1225 orinally, but tweaked.
   if (Timing) Serial.print("2024 ");
 }
 else {
   digitalWrite(senderPin, HIGH); 
   delayMicroseconds(506);   //305 orinally, but tweaked.
   if (Timing) Serial.print("506 ");
   digitalWrite(senderPin, LOW);
   delayMicroseconds(4301);   //305 orinally, but tweaked.
   if (Timing) Serial.print("4301 ");
 }
}

/** 
 * Transmit data
 * @param boolean  positive : if the value you send is a positive or negative one
 * @param long Counter : the value you want to send
 **/
void transmit(boolean positive, unsigned long Counter)
{
 int i;
 itobCounter(Counter, 30);

 // Send the unique ID of your Arduino node
 for(i=0; i<14;i++)
 {
   sendPair(Bytes[i]);
 }

 // Send protocol type
 for(int j = 0; j<4; j++)
 {
   sendPair(BytesType[j]);
 }
 
 // Send the flag to mark the value as positive or negative
 sendPair(positive);

 // Send value (ie your counter)
 for(int j = 0; j<30; j++)
 {
   sendPair(BytesData[j]);
 }
 
 // Send the flag "End of the transmission"
 digitalWrite(senderPin, HIGH);
 if (Timing) Serial.print("506 ");
 delayMicroseconds(506);     
 digitalWrite(senderPin, LOW);
 if (Timing) Serial.print("8602 ");
 delayMicroseconds(8602);
 
}
 
 void setup()
{
  pinMode(senderPin, OUTPUT);
  pinMode(pin, OUTPUT);
  Serial.begin(115200);
  buildSignal();
  attachInterrupt(0, blink, RISING);
}

void loop()
{ 
    delay (15000);
    digitalWrite(pin, true);
    transmit(true, CounterValue);
    digitalWrite(pin, false);
    transmit(true, CounterValue);
    transmit(true, CounterValue);
    transmit(true, CounterValue);
    transmit(true, CounterValue);
}

void blink()
{
  CounterValue += 1;
  //Serial.print("Compteur : ");
  //Serial.println(CounterValue);
}
