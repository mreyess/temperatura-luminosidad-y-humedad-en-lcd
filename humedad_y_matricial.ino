/*
  Autor: Miguel Angel Reyes Solis
         Juan Pablo Gallardo Ochoa

  Descripcion: Programa para calcular la humedad, temperatura y luminosidad 
                por medio de un fotoresistor y un sensor DHT11 de tal manera 
                que una vez hechas las mediciones, sean mostradas en un lcd.

*/

#include "DHT.h"            // importamos la libreria dth para el funcionamiento del sensor
#include <LiquidCrystal.h>  // importamos la libreria para el funcionamiento del lcd
#include <Keypad.h>         // importamos la libreria para el teclado matricial
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
const byte nofilas = 4;     //definimos el numero de filas del teclado matricial
const byte nocolumnas = 4;  //definimos el numero de columnas del teclado matricial
int V;                      //variable para guardar la entrada del LDR
int ilum;                   // DEfinicion de la variable que resultara despues del calculo

float temp;                 //Definimos la variable de la temperatura
float hum;                  //Definimos la variable de la temperatura

String tem_hum = "";        // Inicializamos los mensajes que se mostraran de temperatura y humedad
String luminosidad = "";    // Inicializamos los mensajes que se mostraran de luminosidad

const byte pinfilas[nofilas] = { 12, 8, 7, 4 }; //definimos que pines seran los pertenecientes a las filas
const byte pincolumnas[nocolumnas] = { A4, A3, A2, A1 }; // definimos que pines seran los pertenecientes a las columnas

char teclas[nofilas][nocolumnas] = { //definimos la matriz con el valor que tomara cada tecla al ser presionada
   { '1','2','3', 'A' },
   { '4','5','6', 'B' },
   { '7','8','9', 'C' },
   { '*','0','#', 'D' }
};

DHT dht(DHTPin, DHTTYPE);   //Inicializamos el sensor de temperatura definiendo el tipo de sensor y el pin de entrada
LiquidCrystal lcd(RS,E,D4,D5,D6,D7);  //Inicializamos el LCD pasando los parametros de la tarjeta
Keypad keypad = Keypad(makeKeymap(teclas), pinfilas, pincolumnas, nofilas, nocolumnas); //Inicializamos el teclado matricial pasando los parametros al constructor

String mensajes [] = {"Mensaje 1", "Mensaje 2", "Mensaje 3", "Mensaje 4"}; //Definimos un arreglo de mensajes a mostrar
int cont = 0;         // Inicializamos el contador que nos dara la posicion a mostrar
int tamano = 0;       // Inicializamos la variable que guardara el tamaño del arreglo
 
void setup() {
   Serial.begin(9600);      
   dht.begin();             //Iniciamos con el funcionamiento del sensor de temperatura
   lcd.begin(16,2);         //Iniciamos con el funcionamiento del LCD definiendo que es de 16 * 2 lineas
}
 
void loop() {

  char key = keypad.getKey(); //Pedimos la tecla presionada y asignamos el valor
  tamano = sizeof(mensajes)/sizeof(mensajes[0]); //Obtenemos el tamaño del arreglo


 
   if (key) {
      if(key == 'B'){
                       
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
                
        }else if(key == '4'){
          if(tamano > 0){                           //Verificamos que el arreglo tenga al menos un mensaje
            if(cont <= 0){                          //Checamos que haya algun mensaje anterior
                cont = -1;
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.write("No hay anteriores");
                Serial.println(tamano);
              }else{
                cont -= 1;                          // Mostramos un mensaje anterior
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.print(mensajes[cont]);
              }
            }else{
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.write("No hay mensajes");
              }
          }else if(key == '6'){                        
            if(tamano > 0){                        //Verificamos que el arreglo tenga al menos un mensaje
              if(cont >= tamano-1){                //Checamos que haya algun mensaje posterior
                  cont = tamano;
                  lcd.clear();
                  lcd.setCursor(1,0);
                  lcd.write("No hay posteriores");
                }else{
                  cont += 1;                      // Mostramos un mensaje anterior
                  lcd.clear();
                  lcd.setCursor(1,0);
                  lcd.print(mensajes[cont]);
                }
            }else{
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.write("No hay mensajes");
              }
          }
   }



   
   
}




