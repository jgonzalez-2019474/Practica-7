/*
Fundación Kinal
Centro Educativo Técnico Labral Kinal
Electrónica
Grado: Quinto
Sección: A
Curso: Taller de Electrónica Digital y Reparación de Computadoras I
Nombre: Javier Andre Gonzalez Barrera
Carné: 2019474
*/
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define Pin_LED 3
#define Radar 4
#define Red 8
#define Blue 9
#define Green 10
#define Buzzer 14
#define Pin_puerta 6
#define Pin_Servo 15
#define Pin_rele1 11
#define Pin_rele2 12
#define Pin_Foco1 7
#define Pin_Foco2 5
#define columnas 16
#define filas 2
#define Direccion_lcd 0x27
Servo servo;
Adafruit_NeoPixel neopixel(7, Pin_LED, NEO_GRB + NEO_KHZ800);
OneWire ourWire(2);
DallasTemperature sensors(&ourWire);
LiquidCrystal_I2C LCD_Javier(Direccion_lcd, columnas, filas);


int Puerta = 0;
int Foco_1 = 0;
int Foco_2 = 0;

void Configsalidas()
{
    pinMode(Pin_LED, OUTPUT);
    pinMode(Blue, OUTPUT);
    pinMode(Green, OUTPUT);
    pinMode(Red, OUTPUT);
    pinMode(Buzzer, OUTPUT);
    pinMode(Pin_rele1, OUTPUT);
    pinMode(Pin_rele2, OUTPUT);
}
void Configentradas()
{
    pinMode(Pin_Foco1, INPUT);
    pinMode(Pin_Foco2, INPUT);
    pinMode(Radar, INPUT);
    pinMode(Pin_puerta, INPUT);
}

void temperatura();
int  obtenerTemperaturaCelsius();
void mostrarTemperaturaLCD(int temperaturaCelsius, int temperaturaFahrenheit, int Animacion);
void movimiento();
void controlPuerta();
void controlFoco1();
void controlFoco2();

byte Hielo[] = {
    B00000,
    B00000,
    B11111,
    B11111,
    B11011,
    B11111,
    B11111,
    B00000
};
byte Agua[] = {
    B00000,
    B00010,
    B00110,
    B01110,
    B11111,
    B11111,
    B01110,
    B00000
};

byte Fuego[] = {
    B10101,
    B01010,
    B10110,
    B01110,
    B11111,
    B11111,
    B11111,
    B01110
};

byte Humano[] = {
    B00000,
    B01110,
    B01010,
    B01110,
    B11111,
    B11111,
    B01010,
    B01010
};

byte f_apagado[] = {
    B00000,
    B10001,
    B11011,
    B01010,
    B00100,
    B01110,
    B11011,
    B10001
};
byte f_encendido[] = {
    B00000,
    B00000,
    B11111,
    B11111,
    B11011,
    B11111,
    B11111,
    B00000
};

void setup()
{
    Configsalidas();
    Configentradas();
    digitalWrite(Pin_rele1, HIGH);
    digitalWrite(Pin_rele1, HIGH);
    servo.attach(Pin_Servo);
    sensors.begin();  
    LCD_Javier.init(); 
    LCD_Javier.backlight(); 
    LCD_Javier.createChar(0,Hielo);
    LCD_Javier.createChar(1,Agua);
    LCD_Javier.createChar(2,Fuego);
    LCD_Javier.createChar(3,Humano);
    LCD_Javier.createChar(4,f_apagado);
    LCD_Javier.createChar(5,f_encendido);
    LCD_Javier.setCursor(0,1);
    LCD_Javier.print("P:cerrado");
    LCD_Javier.setCursor(9,1);
    LCD_Javier.print("F:");
    LCD_Javier.write(4);
    LCD_Javier.setCursor(13,1);
    LCD_Javier.print("F:");
    LCD_Javier.write(4);
}

void loop()
{
    int temperaturaCelsius = obtenerTemperaturaCelsius();
    controlPuerta();
    controlFoco1();
    controlFoco2();
    temperatura(temperaturaCelsius);
    movimiento();
    controlFoco1();
    controlFoco2();
    controlPuerta();
}
   
void temperatura(int temperaturaCelsius)
{
    switch (temperaturaCelsius)
    {
        case 15 ... 21:
            digitalWrite(Red, HIGH);
            digitalWrite(Blue, HIGH);
            digitalWrite(Green, LOW);
            break;
        case 22 ... 25:
            digitalWrite(Red, LOW);
            digitalWrite(Blue, LOW);
            digitalWrite(Green, HIGH);
            break;
        case 26 ... 45:
            digitalWrite(Red, HIGH);
            digitalWrite(Blue, LOW);
            digitalWrite(Green, LOW);
            break;
        default:
            break;
    }
}
int obtenerTemperaturaCelsius()
{
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    return static_cast<int>(temp);
}

void mostrarTemperaturaLCD(int temperaturaCelsius, int temperaturaFahrenheit, int Animacion)
{
    LCD_Javier.setCursor(0, 0);
    LCD_Javier.print("Temp: ");
    LCD_Javier.write(Animacion);
    LCD_Javier.setCursor(6, 0);
    LCD_Javier.print(temperaturaCelsius);
    LCD_Javier.setCursor(12, 0);
    LCD_Javier.print(temperaturaFahrenheit);
    LCD_Javier.write(0xDF); 
    LCD_Javier.print("C");
}

void movimiento()
{
    bool mov = digitalRead(Radar);
    if (mov == 1)
    {
        LCD_Javier.setCursor(9, 0);
        LCD_Javier.print("Scan: ");
        LCD_Javier.write(3);
        for (int i = 0; i < 8; i++)
        {
            neopixel.setPixelColor(i, neopixel.Color(255, 0, 0));
            neopixel.show();
        }
        digitalWrite(Buzzer, HIGH);
        delay(250);
        for (int i = 0; i < 8; i++)
        {
            neopixel.setPixelColor(i, neopixel.Color(124, 252, 0));
            neopixel.show();
        }
        LCD_Javier.setCursor(9, 0);
        LCD_Javier.print("Scan: ");
        digitalWrite(Buzzer, LOW);
        delay(250);
    }
    else if (mov == 0)
    {
        for (int i = 0; i < 8; i++)
        {
            neopixel.setPixelColor(i, neopixel.Color(0, 0, 255));
            neopixel.show();
        }
        LCD_Javier.setCursor(9, 0);
        LCD_Javier.print("Scan: ");
    }
}
void controlPuerta()
{
    if (digitalRead(Pin_puerta) && Puerta == 0)
    {
        servo.write(0);
        LCD_Javier.setCursor(0, 1);
        LCD_Javier.print("P: Abierto ");
        delay(500);
        Puerta = 1;
    }
    else if (digitalRead(Pin_puerta) && Puerta == 1)
    {
        servo.write(90);
        LCD_Javier.setCursor(0, 1);
        LCD_Javier.print("P: Cerrado ");
        delay(500);
        Puerta = 0;
    }
}

void controlFoco1()
{
    if (digitalRead(Pin_Foco1) && Foco_1 == 0)
    {
        digitalWrite(Pin_rele1, LOW);
        LCD_Javier.setCursor(9, 1);
        LCD_Javier.print("F:");
        LCD_Javier.write(5);
        delay(500);
        Foco_1 = 1;
    }
    else if (digitalRead(Pin_Foco1) && Foco_1 == 1)
    {
        digitalWrite(Pin_rele1, HIGH);
        LCD_Javier.setCursor(9, 1);
        LCD_Javier.print("F:");
        LCD_Javier.write(4);
        delay(500);
        Foco_1 = 0;
    }
}

void controlFoco2()
{
    if (digitalRead(Pin_Foco2) && Foco_2 == 0)
    {
        digitalWrite(Pin_rele2, LOW);
        LCD_Javier.setCursor(13, 1);
        LCD_Javier.print("F:");
        LCD_Javier.write(5);
        delay(500);
        Foco_2 = 1;
    }
    else if (digitalRead(Pin_Foco2) && Foco_2 == 1)
    {
        digitalWrite(Pin_rele2, HIGH);
        LCD_Javier.setCursor(13, 1);
        LCD_Javier.print("F:");
        LCD_Javier.write(4);
        delay(500);
        Foco_2 = 0;
    }
}
