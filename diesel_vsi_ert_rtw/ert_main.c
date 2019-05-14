/*
* File: ert_main.c
*
* Code generated for Simulink model 'diesel_vsi'.
*
* Model version                  : 1.25
* Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
* C/C++ source code generated on : Tue Mar  5 09:14:33 2019
*
* Target selection: ert.tlc
* Embedded hardware selection: Intel->x86-64 (Windows64)
* Code generation objectives: Unspecified
* Validation result: Not run
*/

#include <stddef.h>
#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */
#include "diesel_vsi.h"                /* Model's header file */
#include "rtwtypes.h"

#include <wiringSerial.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <stdint.h>
//#include <ncurses.h>


//Variables creadas por el programador
//------Entradas-------
double Pref_d=0.0;
double Qref_d=0.0;
double Vload=0.0;
double par=0.0;
double vref=0.0;
int fileDescriptor;
char error[55];

double iinv=0.0;      //corriente proveniente del inversor

//------Salidas--------
double Idie=0.0;
double caudal=0.0;
double vdc=0.0;
double min=5000;
double max=0.0;

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
char var1s[7];
char var2s[7];
char var3s[7];
int fd;
int fd2;
int Vdca=0;
int caudala=0;
char buffer[8];
char buffer2[8];
double tiempo=0.0;

//--------------Para RT-----------------
#include <time.h>
#include <sched.h>
//#include <wiringPi.h>

#define NSEC_PER_SEC    1000000000

//--------------------------------------


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
      rtmSetErrorStatus(diesel_vsi_M, "Overrun");
      return;
    }

    OverrunFlag = true;

    /* Save FPU context here (if necessary) */
    /* Re-enable timer or interrupt here */
    /* Set model inputs here */
    par=0.1;//1.4;
    vref=1000;            //Tension de ref del bus DC que va al inversor
    Vload = 170;          //Tension proveniente de la carga
    iinv = 20;            //Corriente salida del inversor

    //set_Pref_d(Pref_d);
    //set_Qref_d(Qref_d);
    set_Vload(Vload);
    set_par(par);
    set_Vref(vref);
    set_Iinv(iinv);

    /* Step the model for base rate */
    diesel_vsi_step();

    /* Get model outputs here */

    caudal=get_flujo();       //Caudal de combustible
    vdc=get_vdc();            //Salida DC que va al inversor


    printf("El flujo es : %3.2f \n",caudal);
    printf("El nivel del bus dc es: %3.2f \n",vdc);


    Vdca=vdc*10;
    caudala=caudal*10;

    memset(buffer,0,sizeof(buffer));
    //sprintf(buffer,"p%07dq%07dv%07ds%07d\n",Pma,Qma,Vloada,soca);
    sprintf(buffer,"v%07d\n",Vdca);
    //while(pinr==0){
    serialPuts(fd,buffer);
    serialFlush(fd);
    //pinr=digitalRead(2);
    //printf("El dato pin es: %d \n",pinr);
    //}
    serialFlush(fd);
    tcflush(fd, TCIOFLUSH);

    //Datos del caudal Bayona
    memset(buffer2,0,sizeof(buffer2));
    //sprintf(buffer,"p%07dq%07dv%07ds%07d\n",Pma,Qma,Vloada,soca);
    sprintf(buffer2,"v%07d\n",caudala);
    //while(pinr==0){
    serialPuts(fd2,buffer2);
    serialFlush(fd2);
    //pinr=digitalRead(2);
    //printf("El dato pin es: %d \n",pinr);
    //}
    serialFlush(fd2);
    tcflush(fd2, TCIOFLUSH);

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

    //Para RT
    struct timespec t;
    struct sched_param param;
    /* default interval = 50000ns = 50us
     * cycle duration = 100us
     */
    int interval=4*1000000;		//en ns   ->  20000=20us

    if(argc>=2 && atoi(argv[1])>0)
    {
        printf("using realtime, priority: %d\n",atoi(argv[1]));
        param.sched_priority = atoi(argv[1]);
        /* enable realtime fifo scheduling */
        if(sched_setscheduler(0, SCHED_FIFO, &param)==-1){
            perror("sched_setscheduler failed");
            exit(-1);
        }
    }
    if(argc>=3){
        interval=atoi(argv[2]);
        printf("using realtime, priority: %d\n",interval);
    }
    /* Unused arguments */
    (void)(argc);
    (void)(argv);


    //Serial

    fd=serialOpen ("/dev/ttyACM0", 115200);  //0 Para la corriente del generador diesel
    serialClose(fd);
    fd=serialOpen ("/dev/ttyACM0", 115200);

    fd2=serialOpen ("/dev/ttyACM1", 9600);
    serialClose(fd2);
    fd2=serialOpen ("/dev/ttyACM1", 9600);

    sleep(1);

    //Inicializar comunicacion UDP---------------------------------------------------------
    //setUDP();

    //------------GPIO---------------------
    wiringPiSetup();
    pinMode(0, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(3, OUTPUT);
    //wiringPiISR(2, INT_EDGE_RISING, &lectura);
    pinMode(2, INPUT);
    digitalWrite(3,HIGH);
    digitalWrite(1,LOW);

    /* Initialize model */
    diesel_vsi_initialize();


    int estado=0;

    /* get current time */
    clock_gettime(0,&t);
    /* start after one second */
    t.tv_sec++;

    /* Simulating the model step behavior (in non real-time) to
    *  simulate model behavior at stop time.
    */
    while ((rtmGetErrorStatus(diesel_vsi_M) == (NULL)) && !rtmGetStopRequested
    (diesel_vsi_M)) {
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
    diesel_vsi_terminate();
    return 0;
  }

  /*
  * File trailer for generated code.
  *
  * [EOF]
  */
