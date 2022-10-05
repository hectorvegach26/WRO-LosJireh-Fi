#include <math.h>
#include <PWMServo.h>
#include <Servo.h>
#include <NewPing.h>

PWMServo mainmotor;  //Ubicacion de los Pines para el motor DC
#define IN1 7
#define IN2 8
#define ENA 5

#define Velocidad 140
#define Vel_Alta 220

Servo smotordir;    //servomotor para direccionales
#define servo_pin 9

#define Dir_Frente 95   //Configuración y ajuste del cero grado del motor direccional
int dir_derecha=Dir_Frente+32;
int dir_izquierda=Dir_Frente-30;

Servo smotorsen;   //Servomotor para sensor delantero
#define Dir_Frente_sen 90
int trig3_pin = 11;        //sensor 3  Derecho
int echo3_pin = 12;        //sensor 3  Derecho
int trig2_pin = 4;        //sensor 2  Izquierdo
int echo2_pin = 6;        //sensor 2  Izquierdo
int trig1_pin = 3;        //sensor 1 Dir_Frente
int echo1_pin = 2;        //sensor 1 Dir_Frente 

NewPing Sensor3(trig3_pin,echo3_pin);
NewPing Sensor2(trig2_pin,echo2_pin);
NewPing Sensor1(trig1_pin,echo1_pin);

//*** Definicion variables ****
int dist3 = 0;     //distancia captada por sensor 3
int dist2 = 0;     //distancia captada por sensor 2
int dist1 = 0;     //distancia captada por sensor 1
int dist23 =0;     //distancia entre los sensores2 y 3
int dist30 =0;    
int dist60 =0;
int dif1   =0;     //Para saber la dif entre el punto inicial y el actual
int dif2   =0;     //Para saber la dif entre el punto inicial y el actual
int dif3   =0;     //Para saber la dif entre el punto inicial y el actual
int grado_inicial = 0; //Recibe los grados que trae el servo direccion
int grado_final   = 0; //grados que debe llegar el servo dir
int pos           = 0;
int giro = 0;
int tiempo_giro     = 780;
int tiempo_espera   = 1000;  //680 && 1000
int dist_max_lat    =  140;  //distancia que activa el cruce
int dist_max_fren   =   40;  //distancia que evita chocar pared
int dist_segura_lat =   20;  //distancia seguro de los lados para desplzarse
int angulo_inicio   =    0;  // angulo inicial antes de cambiar
int vueltas  = 0;  //
int posi_Xi_inicial = 0;    //posicion de partida x2 sensor 2
int posi_Xd_inicial = 0;    //posicion de partida x3 sensor 3
int posi_y_inicial  = 0;    //posicion de partida y
// *******
// *** DEFINICION DE FUNCIONES ****
void mov_adelante()
{
  Serial.print("Adelante");
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, Velocidad);
}
void mov_atras()
{
  Serial.print("Atras");
  digitalWrite(IN1,LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, Velocidad);
}
void mov_parar()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, Velocidad);
}
void mov_der()  //Gira a la dercha y vuelve a la psocion recta
{ 
  grado_inicial=Dir_Frente;   //smotordir.read();
  grado_final=145;
 for(pos=grado_inicial; pos<grado_final; pos+=1)
 {
  smotordir.write(pos);
  delay(15);
 }  
 delay(250); //tiempo_giro
 smotordir.write(Dir_Frente); 
 delay(tiempo_espera);  //tiempo que debe mantenerse recto 
}


void mov_izq2()  //Gira a la dercha y vuelve a la psocion recta
{ 
  grado_inicial=Dir_Frente;   //smotordir.read();
  grado_final=45;
 for(pos=grado_inicial; pos>grado_final; pos-=1)
 {
  smotordir.write(pos);
  delay(15);
 }  
 delay(250); //tiempo_giro
 smotordir.write(Dir_Frente); 
 delay(tiempo_espera);  //tiempo que debe mantenerse recto 
}

void mov_izq()  //Gira a la Izquierda y vuelve a la psocion recta
{ 
  grado_inicial = smotordir.read();
  grado_final   =  55;
  smotordir.write(grado_final);
  delay(1800);
  smotordir.write(Dir_Frente); 
  delay(tiempo_espera);  //tiempo que debe mantenerse recto
}
 
void medir_laterales()
{
  dist3  = Sensor3.ping_cm();
  delay(5);
  dist2  = Sensor2.ping_cm();
  delay(5);
  dist1  = Sensor1.ping_cm();
  delay(5);
  dist23 = dist2 + dist3;
   if (dist1==0){
    dist1 = 250;
  }
  if (dist2==0){
    dist2 = 50;
  }
  if (dist3==0){
    dist3 = 50;
  }
  Serial.print("  dist1:  ");
  Serial.print(dist1);
  Serial.print("  dist2: ");
  Serial.print(dist2);
  Serial.print("  dist3: ");
  Serial.print(dist3);
Serial.print("dist23 ");  
  Serial.println(dist23);
}
void desplaza_der(){  //para mover grados a la derecha
      grado_final=smotordir.read();
    grado_final=grado_final + 1;
    if (grado_final>120)
       {grado_final=117;}//para eveitar que baje mucho
    smotordir.write(grado_final);
    delay(65);
}

void diagonal(){
if (dist23>120 && dist1>120){
  mov_parar();
}
  
}


void desplaza_izq(){ //para mover grados a la izq
     grado_final=smotordir.read();
     grado_final=grado_final - 1;
     if (grado_final<45)
         {grado_final=43; }//para eveitar que baje mucho
     smotordir.write(grado_final);
     delay(65);
}
void ajustar_centro()
{
  if (dist2 < dist_segura_lat) //HAY QUE EMPUJARLO A LA DERECHA
  {
    mov_parar();
    smotordir.write(120);
    mov_adelante();
//    desplaza_der();
   }
 if (dist3 < dist_segura_lat)
    {  //HAY QUE EMPUJARLO A LA  IZQ
    mov_parar();
    smotordir.write(65);
    mov_adelante();
      //desplaza_izq();   
    }
}
void evitar_obstaculo()
{
  smotorsen.write(15);
  dist30  = Sensor1.ping_cm();
  Serial.print(dist30);
  Serial.print("  dist30: ");
  delay(1000);  

  smotorsen.write(90);
  dist1  = Sensor1.ping_cm();
  delay(1000);

  smotorsen.write(180);
  dist60  = Sensor1.ping_cm();
    Serial.print(dist60);
  Serial.println("  dist60: ");
  delay(1000);  
  
}
void ajustar_origen()  //funcion para volver al lugar de partida
{
   medir_laterales();
   dif2 = posi_Xi_inicial - dist2;  //sensor2
   dif3 = posi_Xd_inicial - dist3;  //sensor3 
   dif1 = posi_y_inicial  - dist1;  //sensor1
   if (dif2>0)
      {
         desplaza_izq();
      }
      if (dif2<0){
         desplaza_der();
     }
     if (dif1> 5 and dif1<-5) //falta para llegar
      {
        mov_parar();  //llego a la meta
      }
} 

void corregir()
{
  smotordir.write(75);
  delay(200);
  smotordir.write(dir_izquierda);
  delay(200);
  smotordir.write(95);
 
 
  
//if(dist3<dist2)
//{
//  mov_parar();
//  delay(1000);
//  mov_atras();
//  smotordir.write(dir_derecha);
//  delay(3000);
//  mov_adelante();
//  smotordir.write(95);
//  delay(1000);
//}else
//{
//  smotordir.write(dir_izquierda);
//  delay(3000);
//  mov_adelante();
//  smotordir.write(95);
//  delay(1000);
}
  
  //echar para adelante y parar
  
  

void setup() 
{
   Serial.begin(9600);
   smotordir.attach(servo_pin);    //calibracion servomotor de direccion
   
   smotorsen.attach(10,980,1700);  //calibracion servomotor del sensor delantero
   smotorsen.write(90);
   posi_Xi_inicial = Sensor2.ping_cm();  //posicion de partida x
   posi_Xd_inicial = Sensor3.ping_cm();  //posicion de partida x
   posi_y_inicial  = Sensor1.ping_cm();  //posicion de partida y
  smotordir.write(95);
}
 

void loop() 
{  
  mov_adelante();
   medir_laterales();
    diagonal();
   if(dist1< 85 && dist23>180   )
   {
    if(dist2>dist3)
    {
      mov_izq2();
    }
    if(dist2<dist3){
    mov_der();
    }
   }else{
    if(dist23<110){
    ajustar_centro();
    }
   }
//   if(dist23>110 && dist1<70)
//   {
//    corregir();
//   }
   
}
