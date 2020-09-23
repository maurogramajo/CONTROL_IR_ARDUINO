#include <Arduino.h>
#include <IRremote.h>
#include <Adafruit_NeoPixel.h>

#if defined(ESP32)
int IR_RECEIVE_PIN = 15;
#else
int IR_RECEIVE_PIN = 11;
#endif
IRrecv irrecv(IR_RECEIVE_PIN);

decode_results results;

#define IR_UNO  0xFFA25D
#define IR_DOS  0xFF629D
#define IR_TRES  0xFFE21D
#define IR_CUATRO  0xFF22DD
#define IR_CINCO  0xFF02FD
#define IR_SEIS  0xFFC23D
#define IR_SIETE  0xFFE01F
#define IR_OCHO  0xFFA857
#define IR_NUEVE  0xFF906F
#define IR_CERO  0xFF9867
#define IR_LEFT  0xFF10EF
#define IR_RIGHT  0xFF5AA5
#define IR_UP  0xFF18E7
#define IR_DOWN  0xFF4AB5
#define IR_OK  0xFF38C7
#define IR_NUMERAL  0xFF6897
#define IR_ASTERISCO  0xFFB04F
#define IR_APRETADO 0xFFFFFFFF

Adafruit_NeoPixel strip = Adafruit_NeoPixel(100, 6, NEO_GRB + NEO_KHZ800);

bool algo_cambio;
int red, green, blue;
decode_results boton_funcion;
int posicion;
int columna;

// On the Zero and others we switch explicitly to SerialUSB
#if defined(ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);
#if defined(__AVR_ATmega32U4__)
    while (!Serial)
        ; //delay for Leonardo, but this loops forever for Maple Serial
#endif
#if defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL)
    delay(2000); // To be able to connect Serial monitor after reset and before first printout
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__));

    // In case the interrupt driver crashes on setup, give a clue
    // to the user what's going on.
    Serial.println("Enabling IRin");
    irrecv.enableIRIn(); // Start the receiver

    Serial.print(F("Ready to receive IR signals at pin "));
    Serial.println(IR_RECEIVE_PIN);

    strip.begin();
    strip.clear();
    strip.show();

    red = green = blue = 0;
    posicion = 0;
    columna = 0;
    algo_cambio = false;
    boton_funcion.value = 0;
}

void loop() {
    if (irrecv.decode(&results)) {
      switch (results.value)
      {
      case IR_UNO:
        Serial.println("1");
        red = 50;
        green = 0;
        blue = 0;
        algo_cambio = true;
        boton_funcion.value = 0;
        break;
      case IR_DOS:
        Serial.println("2");
        red = 0;
        green = 50;
        blue = 0;
        algo_cambio = true;
        boton_funcion.value = 0;
        break;
      case IR_TRES:
        Serial.println("3");
        red = 0;
        green = 0;
        blue = 50;
        algo_cambio = true;
        boton_funcion.value = 0;
        break;
      case IR_CUATRO:
        Serial.println("4");
        red = 50;
        green = 50;
        blue = 0;
        algo_cambio = true;
        boton_funcion.value = 0;
        break;
      case IR_CINCO:
        Serial.println("5");
        red = 0;
        green = 50;
        blue = 50;
        algo_cambio = true;
        boton_funcion.value = 0;
        break;
      case IR_SEIS:
        Serial.println("6");
        red = 50;
        green = 0;
        blue = 50;
        algo_cambio = true;
        boton_funcion.value = 0;
        break;
      case IR_SIETE:
        Serial.println("7");
        red = 75;
        green = 50;
        blue = 0;
        algo_cambio = true;
        boton_funcion.value = 0;
        break;
      case IR_OCHO:
        Serial.println("8");
        red = 50;
        green = 75;
        blue = 0;
        algo_cambio = true;
        boton_funcion.value = 0;
        break;
      case IR_NUEVE:
        Serial.println("9");
        red = 0;
        green = 50;
        blue = 75;
        algo_cambio = true;
        boton_funcion.value = 0;
        break;
      case IR_CERO:
        Serial.println("0");
        red = 0;
        green = 75;
        blue = 50;
        algo_cambio = true;
        boton_funcion.value = 0;
        break;
      case IR_LEFT:
        Serial.println("←");
        columna-=10;
        if(columna < 0)
          columna = 0;
        
        algo_cambio = true;
        boton_funcion.value = results.value;
        break;
      case IR_RIGHT:
        Serial.println("→");
        columna+=10;
        if(columna > 90)
          columna = 90;
        
        algo_cambio = true;
        boton_funcion.value = results.value;
        break;
      case IR_UP:
        Serial.println("↑");

        posicion--;
        if(posicion < 0)
          posicion = 0;

        algo_cambio = true;

        boton_funcion.value = results.value;
        break;
      case IR_DOWN:
        Serial.println("↓");
        
        posicion++;
        if(posicion > 9)
          posicion = 9;

        algo_cambio = true;

        boton_funcion.value = results.value;
        break;
      case IR_ASTERISCO:
        Serial.println("*");
        boton_funcion.value = 0;
        break;
      case IR_NUMERAL:
        Serial.println("#");
        boton_funcion.value = 0;
        break;
      case IR_OK:
        Serial.println("OK");
        boton_funcion.value = 0;
        break;
      case IR_APRETADO:
        Serial.println("SE MANTIENE APRETADO EL BOTON");
        if(boton_funcion.value != 0)
        {
          if(boton_funcion.value == IR_UP){
            posicion--;
            if(posicion < 0)
              posicion = 0;

            algo_cambio = true;
          }
          if(boton_funcion.value == IR_DOWN){
            posicion++;
            if(posicion > 9)
              posicion = 9;

            algo_cambio = true;
          }
          if(boton_funcion.value == IR_LEFT){
            columna-=10;
            if(columna < 0)
              columna = 0;
            algo_cambio = true;
          }
          if(boton_funcion.value == IR_RIGHT){
            columna+=10;
            if(columna > 90)
              columna = 90;
            algo_cambio = true;
          }
        }
        break;
      default:
        Serial.println(results.value, HEX);
        boton_funcion.value = 0;
        break;
      }
      irrecv.resume(); // Receive the next value
      if (algo_cambio == true)
      {
        strip.clear();
        strip.setPixelColor(posicion+columna, red, green, blue);
        strip.show();
        algo_cambio = false;
      }
      
    }
    delay(100);
}