/*
* File: ert_main.c
*
* Code generated for Simulink model 'pv'.
*
* Model version                  : 1.17
* Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
* C/C++ source code generated on : Mon Mar  4 15:44:02 2019
*
* Target selection: ert.tlc
* Embedded hardware selection: Intel->x86-64 (Windows64)
* Code generation objectives: Unspecified
* Validation result: Not run
*/

#include <stddef.h>
#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */
#include "pv.h"                        /* Model's header file */
#include "rtwtypes.h"

#include <wiringSerial.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <ncurses.h>


//Emulacion de panel
#define BUFFER_SIZE 1024
double Vg,Vc,n,Ns,T1,Voc_T1,Isc_T1,Isc_T2,T2,Voc_T2,Ics_T2,TaK,K0,IL_T1,IL,I0,I0_T1,Xv,dVdI_Voc,k,q,Rs,A,Vt_Ta,Ia;
double Suns,TaC,Va,Temp,Volt,Irra;

const char *delimiter_characters = "\t";
const char *filename = "perfiles_meteo_consum1/myData7_5min.txt";//"perfiles_meteo_consum/myData7_5min.txt";
FILE *input_file; //= fopen(filename, "r");
char buffer[BUFFER_SIZE];
char *last_token;
char *year;
char *day;
char *times;
char *outhum;
char *windspd;
char *rain;
char *solarrad;
char *tempout;
char *consumptp;
char *consumptq;
int contador;
int contador2=0;

//------Entradas-------
double ipv=0.0;
double vload3=0.0;
double Prefd=0.0;
double Qrefd=0.0;
MCP3204 ad_MCP3204;
int fileDescriptor;
char error[55];

double iinv=0.0;   //corriente de salida del inversor :::::::::::::::::::::::::::::

//------Salidas--------
double vdc=0.0;
double soc=0.0;

#define MSGISIZE 9

//Comunicacion
int  bytes_read = 0;
int pinr=0;
int var=0;
char bufferAux;
char read_buffer[MSGISIZE];   /* Buffer to store the data received              */
int var1=0;
int var2=0;
int var3=0;
int var4=0;
char var1s[7];
char var2s[7];
char var3s[7];
char var4s[7];
int fd3;
int i3a=0;
char buffer3[8];

//===============================================================
//-------Variables para graficar
#include <math.h>
#define NUM_POINTS 100000
char * commandsForGnuplot[] = {"set title \"TITLEEEEE\"", "plot 'data.temp'"};
double xvals[NUM_POINTS];//= {1.0, 2.0, 3.0, 4.0, 5.0};
double yvals[NUM_POINTS];// = {5.0 ,3.0, 1.0, 3.0, 5.0};
FILE * temp;
FILE *	gnuplotPipe;
double in=0;
//===============================================================

//--------------Para RT-----------------
#include <time.h>
#include <sched.h>
//#include <wiringPi.h>

#define NSEC_PER_SEC    1000000000

//--------------------------------------


/*
* Associating rt_OneStep with a real-time clock or interrupt service routine
* is what makes the generated code "real-time".  The function rt_OneStep is
* always associated with the base rate of the model.  Subrates are managed
* by the base rate from inside the generated code.  Enabling/disabling
* interrupts and floating point context switches are target specific.  This
* example code indicates where these should take place relative to executing
* the generated code step function.  Overrun behavior should be tailored to
* your application needs.  This example simply sets an error status in the
* real-time model and returns from rt_OneStep.
*/
void rt_OneStep(void);

//----------------------------Para RT------------------------------
/* using clock_nanosleep of librt */
extern int clock_nanosleep(clockid_t __clock_id, int __flags,
  __const struct timespec *__req,
  struct timespec *__rem);

  /* the struct timespec consists of nanoseconds
  * and seconds. if the nanoseconds are getting
  * bigger than 1000000000 (= 1 second) the
  * variable containing seconds has to be
  * incremented and the nanoseconds decremented
  * by 1000000000.
  */
  static inline void tsnorm(struct timespec *ts)
  {
    while (ts->tv_nsec >= NSEC_PER_SEC) {
      ts->tv_nsec -= NSEC_PER_SEC;
      ts->tv_sec++;
    }
  }
  //------------------------------------------------------------------

  void rt_OneStep(void)
  {
    static boolean_T OverrunFlag = false;

    /* Disable interrupts here */

    /* Check for overrun */
    if (OverrunFlag) {
      rtmSetErrorStatus(pv_M, "Overrun");
      return;
    }

    OverrunFlag = true;

    if (contador==15*60*1000/4 || contador2==0){

      contador2=1;
      fgets(buffer, BUFFER_SIZE, input_file);	//Second line for the labels

      puts(buffer);
      //while(fgets(buffer, BUFFER_SIZE, input_file) != NULL){
      //fgets(buffer, BUFFER_SIZE, input_file);
      // Gets each token as a string and prints it
      year=strtok(buffer, delimiter_characters);
      //printf("year: %s\n",year);
      day = strtok(NULL, delimiter_characters);
      //printf("day: %s\n",day);
      times = strtok(NULL, delimiter_characters);
      //printf("time: %s\n",times);
      outhum = strtok(NULL, delimiter_characters);
      //printf("outhum: %s\n",outhum);
      windspd = strtok(NULL, delimiter_characters);
      //printf("windspd: %s\n",windspd);
      rain = strtok(NULL, delimiter_characters);
      //printf("rain: %s\n",rain);
      solarrad = strtok(NULL, delimiter_characters);
      printf("solarrad: %s\n",solarrad);
      tempout = strtok(NULL, delimiter_characters);
      printf("tempout: %s\n",tempout);
      consumptp = strtok(NULL, delimiter_characters);
      printf("consumption: %s\n",consumptp);
      consumptq = strtok(NULL, delimiter_characters);
      printf("consumption: %s\n",consumptq);
      /*while(last_token != NULL){
      * printf("%s\n",last_token);
      * last_token = strtok(NULL, delimiter_characters);
      * }*/
      //}

    }
    else{
      contador=0;
    }

    contador=contador+1;


    if(ferror(input_file)){
      perror("The following error ocurred");
    }
    //fclose(input_file);

    TaC= atof(tempout) ; //Lectura desde el txt
    Va=0.5;
    Suns=atof(solarrad)/1000.0 ; //Lectura desde el txt
    TaK = 273 + TaC;
    IL_T1 = Isc_T1 * Suns;
    IL = IL_T1 + K0*(TaK - T1);
    I0= I0_T1*pow((TaK/T1),(3/n))*exp(-q*Vg/(n*k)*((1/TaK)-(1/T1)));
    Vt_Ta = A * k * TaK / q;
    Vc = Va/Ns;
    printf("Aqui estoy\n");

    for (int j=1;j<=5;j++){
      Ia=Ia- (IL - Ia - I0*( exp((Vc+Ia*Rs)/Vt_Ta) -1))/(-1 - (I0*( exp((Vc+Ia*Rs)/Vt_Ta) -1))*Rs/Vt_Ta);
    }



    //ipv=1.5;//500;   //Proveniente de la fuente de generación PV
    ipv=Ia;
    iinv = 50.0;                //lectura de la corriente del inversor
    set_Idc_PV(ipv);            //asigna corriente proveniente del arreglo de paneles fotovoltaicos
    set_Iinv_PV(iinv);          //asigna corriente proveniente del inversor
    /* Save FPU context here (if necessary) */
    /* Re-enable timer or interrupt here */
    /* Set model inputs here */

    /* Step the model for base rate */
    pv_step();

    vdc=get_Vdc();
    soc = get_SOC();

    printf("Tension salida dc: %3.2f \n",vdc);
    printf("El estado de la bateria es: %3.2f \n",soc);


    i3a=i3*10;

    memset(buffer3,0,sizeof(buffer3));
    //sprintf(buffer,"p%07dq%07dv%07ds%07d\n",Pma,Qma,Vloada,soca);
    sprintf(buffer3,"v%07d\n",i3a);
    //while(pinr==0){
    serialPuts(fd3,buffer3);
    serialFlush(fd3);
    //pinr=digitalRead(2);
    //printf("El dato pin es: %d \n",pinr);
    //}
    serialFlush(fd3);
    tcflush(fd3, TCIOFLUSH);
    /* Get model outputs here */

    /* Indicate task complete */
    OverrunFlag = false;

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */
  }

  /*
  * The example "main" function illustrates what is required by your
  * application code to initialize, execute, and terminate the generated code.
  * Attaching rt_OneStep to a real-time clock is target specific.  This example
  * illustrates how you do this relative to initializing the model.
  */
  int_T main(int_T argc, const char *argv[])
  {
    /* Unused arguments */
    (void)(argc);
    (void)(argv);

    Va=0;
    q=1.6*pow(10,-19);
    k=1.38*pow(10,-23);
    n=1.12;
    Vg=1.12;
    Ns=36;
    T1=273+25;
    Voc_T1=21.06/Ns;
    Isc_T1=3.3;//3.8;
    Voc_T2=19/Ns;//17.05/Ns;
    Isc_T2=3.4;//3.92;
    K0=(Isc_T2 - Isc_T1)/(T2 - T1);
    I0_T1=Isc_T1/(exp(q*Voc_T1/(n*k*T1))-1);
    Xv = I0_T1*q/(n*k*T1) * exp(q*Voc_T1/(n*k*T1));
    dVdI_Voc = - 1.15/Ns / 2;
    Rs = - dVdI_Voc - 1/Xv;
    A=1;
    printf("Numero: %3.2f \n",I0_T1);



    //Verificando txt de datos
    //FILE *input_file = fopen(filename, "r");
    input_file = fopen(filename, "r");
    if (input_file == NULL){
      fprintf(stderr, "Unable to open file %s\n",filename);
    }
    fgets(buffer, BUFFER_SIZE, input_file);	//First line for the labels

    //Para RT
    struct timespec t;
    struct sched_param param;
    /* default interval = 50000ns = 50us
    * cycle duration = 100us
    */
    int interval=4*1000000;		//en ns   ->  20000=20us


    /* Unused arguments */
    (void)(argc);
    (void)(argv);

    //Grafica


    temp = fopen("data.temp", "w");

    gnuplotPipe = popen ("gnuplot -persistent", "w");

    fprintf(gnuplotPipe,"set grid \n");

    //Serial

    fd3=serialOpen ("/dev/ttyACM0", 115200);
    serialClose(fd3);
    fd3=serialOpen ("/dev/ttyACM0", 115200);

    sleep(1);

    //------------GPIO---------------------
    wiringPiSetup();
    pinMode(0, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(3, OUTPUT);
    //wiringPiISR(2, INT_EDGE_RISING, &lectura);
    pinMode(2, INPUT);
    digitalWrite(3,HIGH);
    digitalWrite(1,LOW);

    int estado=0;

    /* Initialize model */
    pv_initialize();

    /* get current time */
    clock_gettime(0,&t);
    /* start after one second */
    t.tv_sec++;

    /* Simulating the model step behavior (in non real-time) to
    *  simulate model behavior at stop time.
    */
    while ((rtmGetErrorStatus(pv_M) == (NULL)) && !rtmGetStopRequested(pv_M)) {
      /* wait untill next shot */
      clock_nanosleep(0, TIMER_ABSTIME, &t, NULL);
      /* do the stuff */
      if(estado==0){
        estado=1;

      }else{
        estado=0;
      }
      digitalWrite (21, estado) ;
      rt_OneStep();
      t.tv_nsec+=interval;
      tsnorm(&t);
    }

    /* Disable rt_OneStep() here */

    /* Terminate model */
    pv_terminate();
    return 0;
  }

  /*
  * File trailer for generated code.
  *
  * [EOF]
  */
