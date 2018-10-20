/*
  Autor: Miguel Angel Reyes Solis
         Juan Pablo Gallardo Ochoa

  Descripcion: Programa para calcular la humedad, temperatura y luminosidad 
                por medio de un fotoresistor y un sensor DHT11 de tal manera 
                que una vez hechas las mediciones, sean mostradas en un lcd.

*/

#include "DHT.h"            // importamos la libreria dth para el funcionamiento del sensor
#include <LiquidCrystal.h>  // importamos la libreria para el funcionamiento del lcd
#define RS 3                // Definimos los pines de entrada para los pines del lcd
#define E 5
#define D4 6
#define D5 9
#define D6 10
#define D7 11
#define DHTTYPE DHT11       // Definimos que el sensor de temperatura y humedad que usaremos sera el DHT 11

const long A = 1000;        //Resistencia en oscuridad en KΩ indispensable para la formula del calculo de la luminosidad
const int B = 15;           //Resistencia a la luz (10 Lux) en KΩ indispensable para la formula del calculo de la luminosidad
const int Rc = 10;          //Resistencia calibracion en KΩ indispensable para la formula del calculo de la luminosidad
const int LDRPin = A0;      //Pin de entrada del fotoresistor o LDR
const int DHTPin = 2;       //Pin de entrada para el sensor de temperatura y humedad
int V;                      //variable para guardar la entrada del LDR
int ilum;                   // DEfinicion de la variable que resultara despues del calculo

float temp;                 //Definimos la variable de la temperatura
float hum;                  //Definimos la variable de la temperatura

String tem_hum = "";        // Inicializamos los mensajes que se mostraran de temperatura y humedad
String luminosidad = "";    // Inicializamos los mensajes que se mostraran de luminosidad

DHT dht(DHTPin, DHTTYPE);   //Inicializamos el sensor de temperatura definiendo el tipo de sensor y el pin de entrada
LiquidCrystal lcd(RS,E,D4,D5,D6,D7);  //Inicializamos el LCD pasando los parametros de la tarjeta

 
void setup() {
   Serial.begin(9600);      
   dht.begin();             //Iniciamos con el funcionamiento del sensor de temperatura
   lcd.begin(16,2);         //Iniciamos con el funcionamiento del LCD definiendo que es de 16 * 2 lineas
}
 
void loop() {

  delay(1000);              //Cada segundo hara la lectura y la mostrara en la pantalla

   V = analogRead(LDRPin);   //Guardamos la lectura del fotoresistor(LDR) 
   ilum = ((long)V*A*10)/((long)B*Rc*(1024-V)); // Ejecutamos la formula del calculo de la luminosidad
   hum = dht.readHumidity();  //Leemos la humedad del sensor
   temp = dht.readTemperature(); //Leemos la temperatura del sensor
   tem_hum="";                   // Limpiamos el mensaje
   tem_hum.concat("H: ");        // Concatemanos los textos y variables en un solo mensaje en relacion a la temperatura y la humedad
   tem_hum.concat(hum);
   tem_hum.concat(" T:");
   tem_hum.concat(temp);

   luminosidad = "";                  // Limpiamos el mensaje
   luminosidad = "Lum: ";             // Concatemanos los textos y variables en un solo mensaje en relacion a la temperatura y la humedad
   luminosidad.concat(ilum);

   
 // Condicionamos de tal forma que si no se logra obtener una lectura entonces mande un mensaje de error
   if (isnan(hum) || isnan(temp)) {
      lcd.clear();
      lcd.write("Error en la lectura");
      return;
   }else{
    // Si se logro la lectura del sensor entonces limpiamos la pantalla, mostramos la temperatura y la humedad en la primera linea
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print(tem_hum);
    lcd.setCursor(1,1); // Cambiamos de linea para mostrar la luminosidad
    lcd.print(luminosidad);
   }

   
   
}




