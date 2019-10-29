	//compile using "gcc ert_main.c carga_sf.c rt_nonfinite.c rtGetInf.c rtGetNaN.c libmcp3204.c -lm -lwiringPi -lrt -Wall"
	
	/*
	 * File: ert_main.c
	 *
	 * Code generated for Simulink model 'carga_sf'.
	 *
	 * Model version                  : 1.66
	 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
	 * C/C++ source code generated on : Wed Aug 15 11:49:54 2018
	 *
	 * Target selection: ert.tlc
	 * Embedded hardware selection: Intel->x86-64 (Windows64)
	 * Code generation objectives: Unspecified
	 * Validation result: Not run
	 */
	
	#include <stddef.h>
	#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */
	#include "carga_sf.h"                  /* Model's header file */
	#include "rtwtypes.h"
	#include <wiringSerial.h>
	#include <stdlib.h> 
	#include <errno.h>
	#include <string.h>
	#include <unistd.h> 
	#include <termios.h>
	#include <wiringPi.h>
	#include <ncurses.h>
	#include "libmcp3204.h"
	
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
	 
	//Variables creadas por el programador
	
	#define MSGISIZE 26
	
	 //------Entradas-------
	 double i1=0.0;
	 double i2=0.0;
	 double i3=0.0;
	 MCP3204 ad_MCP3204;
	 int fileDescriptor;
	 char error[55];
	 
	 //------Salidas--------
	 double Pm=0.0;
	 double Qm=0.0;
	 double Vload=0.0;
	 double min=5000;
	 double max=0.0;
	 
	 double tiempo=0.0;
	 
	 
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
	 int Vloada=0;
	 int Pma=0;
	 int Qma=0;
	 char buffer[8];
	 
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
	 
	 
	 int i;
	 int k;
	 
	 // 39 datos
	 // Dt = 427us (idealmente) para 60Hz
	 int senw[] = {2048,	2377,	2697,	3000,	3278,
	              3525,	3733,	3898,	4015,	4081,
	              4094,	4055,	3963,	3822,	3634,
	              3406,	3143,	2851,	2538,	2213,
	              1883,	1558,	1245,	953,	690,
	              462,	274,	133,	41,	2,	
	              15, 81,	198,	363,	571,	
	              818, 1096,	1399,	1719};
	
	              
	              
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
	    rtmSetErrorStatus(carga_sf_M, "Overrun");
	    return;
	  }
	
	  OverrunFlag = true;
	  /* Save FPU context here (if necessary) */
	  /* Re-enable timer or interrupt here */
	  /* Set model inputs here */
	  
	  //------- entradas-------------
	  digitalWrite(0,LOW);
	  //tcflush(fd, TCIOFLUSH);
	  
	  var=0;
	  //(read_buffer,0,sizeof(read_buffer));
	  //printf("Va leer 2\n");
	  //memset(&bufferAux,0,sizeof(bufferAux));
	  //printf("Va leer\n");
	  //sprintf(read_buffer,"b%07dd%07de\n",Pma,Qma);
	  //while(serialDataAvail(fd)==0){} 
	  
	  
	  
	  //memset(read_buffer,0,sizeof(read_buffer));
	  //printf("Captó dato serial\n");
	  
	  
	  //Nuevooooooooo
	  /*while(read_buffer[0]!='f' || read_buffer[MSGISIZE-1]!='e'){
		bytes_read=read(fd,&read_buffer,MSGISIZE); 
		//printf("Ard msg: %s \n",read_buffer);
		serialFlush(fd);
		tcflush(fd, TCIOFLUSH);
		//fflush(stdin);
		//fflush(stdout);
		
	  }
	  digitalWrite(1,HIGH);
	  
	  
	  //Nuevoooooo
	  digitalWrite(0,HIGH);
	  serialFlush(fd);
	  tcflush(fd, TCIOFLUSH);
	  //fflush(stdin);
	  //fflush(stdout);
	  digitalWrite(0,LOW);*/
	  
	  var=1;
	  //printf("Trama de arduino: %s\n",read_buffer);
	  
	  //Lectura ADC 
	  
	  if (MCP3204_convert(fileDescriptor,singleEnded,CH0,&ad_MCP3204,error))
	{
		printf("Error during conversion1.\n");
		printf("%s\n",error);
		exit(1);
	}
	
	
	var1=MCP3204_getValue(ad_MCP3204);
	printf("Valor adc1: %d \n", var1);
	
	
	//var1=MCP3204_analogValue(ad_MCP3204);
	
	if (MCP3204_convert(fileDescriptor,singleEnded,CH1,&ad_MCP3204,error))
	{
		printf("Error during conversion.\n");
		printf("%s\n",error);
		exit(1);
	}
	
	
	var2=MCP3204_getValue(ad_MCP3204);
	//var2=MCP3204_analogValue(ad_MCP3204);
	//var3=var2;
	
	if (MCP3204_convert(fileDescriptor,singleEnded,CH2,&ad_MCP3204,error))
	{
		printf("Error during conversion.\n");
		printf("%s\n",error);
		exit(1);
	}
	
	
	var3=MCP3204_getValue(ad_MCP3204);  
	  /*if (var==1){
		  int q=0;
		  //Acomodar en arreglo de caracteres los datos recibidos
		  for(q=0; q<7;q++){
			var1s[q]=read_buffer[q+2]; //Primer dato
			var2s[q]=read_buffer[q+10]; //Segundo dato
			var3s[q]=read_buffer[q+18]; //Tercer dato
	      }
	  //var1=atof(var1s)/10.0; //Primer dato en numero
	  //var2=atof(var2s)/10.0; //Segundo dato en numero
	  //var3=atof(var3s)/10.0; //Segundo dato en numero
	  var1=atoi(var1s); //Primer dato en numero
	  var2=atoi(var2s); //Segundo dato en numero
	  var3=atoi(var3s); //Segundo dato en numero*/
	  
	  
	  
	  //var3=atof(var3s)/10.0; //Segundo dato en numero    
	  //printf ("El numero1  es :%d \n",var1);
	  //printf ("El numero2  es :%d \n",var2); 
	  //printf ("El numero3  es :%d \n",var3); 
	  
	  /*
	  double k1=(440+450)/4095.0;
	  double vx1=-450;
	  double k2=(1220+1155)/4095.0;
	  double vx2=-1155;
	  double k3=(1650+1655)/4095.0;
	  double vx3=-1655;
	  */
	  
	  
	  //double k1=(1500+1500)/4095.0;
	  //double vx1=-1500;
	  //double k2=(1500+1500)/4095.0;  
	  //double vx2=-1500;
	  
	  //double k=(2*170)/2248.0;
      //double vx=-170-(502*2*170)/2248.0;
      
	  double k1=(2*50)/2207.0;
	  double vx1=-50-(556*2*50)/2207.0;
	  double k2=(2*50)/2203.0;
	  double vx2=-50-(543*2*50)/2203.0;
	  double k3=(2*90)/2207.0;
	  double vx3=-90-(556*2*90)/2207.0;
	  
	  
	 /* double k2=(2*300)/2203.0;
	  double vx2=-300-(543*2*300)/2203.0;
	  double k3=(2*50)/2207.0;
	  double vx3=-50-(556*2*50)/2207.0;
	  */
	  
	  //printf ("i1: %d i2: %d i3: %d\n",var1,var2,var3);
	  i1=(var1*k1)+vx1;
	  //i1=i1/10.0;
	  i2=(var2*k2)+vx2;
	  //i2=i2/10.0;
	  //i1=40*sin(2*3.14*60*tiempo);
	  //i2=40*sin(2*3.14*60*tiempo);
	  i3=var3*k3+vx3;//90*sin(2*3.14*60*tiempo);//(var3*k3)+vx3;
	  
	  tiempo=tiempo+0.0001;
	  if (tiempo==0.0168) tiempo=0.0;
	  //i3=i3/10.0;
	  //Variables prubea generadores
	  //i1=(16.67*2)*var1/4095.0-16.67;
	  
	  //i1=(16.67*2)*var1/255.0-16.67;
	  //i2=i1;
	  //i3=i1;
	  //printf("Valor adc 1: %d \n", var1);
	  //printf("Valor adc 2: %d \n", var2);
	  
	
	
	//if (min>var1) min=var1;
	//if (max<var1) max=var1;
	
	printf("Valor minimo: %3.2f \n", min);
	printf("Valor maximo: %3.2f \n", max);
	
	
    
	  
	  set_i1(i1);
	  set_i2(i2);
	  set_i3(i3);
	  
	  //set_i1((senw[i]-2048)*0.0391);
	  //set_i2((senw[i]-2048)*0.0391);
	  //set_i3((senw[i]-2048)*0.0391);
	  
	  printf ("i1: %f i2: %f i3: %f\n",i1,i2,i3);
	  /* Step the model for base rate */
	  carga_sf_step();
	
	  /* Get model outputs here */
	  //----- salidas-------
	  Pm=get_Pm();
	  Qm=get_Qm();
	  Vload=get_Vload();
	  
	  printf("La potencia P medida es: %3.2f \n",Pm);
	  printf("La potencia Q medida es: %3.2f \n",Qm);
	  printf("Voltaje : %3.2f \n",Vload);
	  
	  if (min>Vload) min=Vload;
	  if (max<Vload) max=Vload;
	  
	  //----------Serial----------------------
	  //-----------Escritura-envio---------------------
	  //Pma=Pm*10;
	  //Qma=Qm*10;
	  Vloada=Vload*10;
	  Pma=Pm*10;
	  Qma=Qm*10;
	  //Vloada=-456;
	  //if(Vloada>2300){
		  //Vloada=2300;
		  //}else if(Vloada<-2300){
		  //Vloada=-2300;
		  //}
	  
	  
	  memset(buffer,0,sizeof(buffer));
	   //sprintf(buffer,"p%07dq%07dv%07ds%07d\n",Pma,Qma,Vloada,soca);
	   //sprintf(buffer,"v%07d\n",Vloada);
	   sprintf(buffer,"v%07d%07d%07de\n",Vloada,Pma,Qma);
	   //while(pinr==0){
		   serialPuts(fd,buffer);
		   serialPuts(fd2,buffer);
		   //serialFlush(fd);
		   //pinr=digitalRead(2);
		   //printf("so\n");
		   //printf("El dato es: %s \n",buffer);
	   //}    
	   serialFlush(fd);
	   tcflush(fd, TCIOFLUSH);
	   serialFlush(fd2);
	   tcflush(fd2, TCIOFLUSH);
	   pinr=0;
	   digitalWrite(1,LOW);
	   var=0;
	   //printf("Ready for next loop\n");
	   //-----------Grafica---------------------
	   //in+=0.0001;
	   
	   //fprintf(temp, "%d %d \n",var1,var2);
	   fprintf(temp, "%3.2f %3.2f %3.2f %3.2f %3.2f %3.2f \n",i1,i2,i3,Vload,Pm,Qm);
	  
	  
	  //}
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
	  
	  //Grafica
	  
	  
	  temp = fopen("data.temp", "w");
	    
	  gnuplotPipe = popen ("gnuplot -persistent", "w");
	    
	  fprintf(gnuplotPipe,"set grid \n");
	  
	  //Serial
	  
	  
	  fd=serialOpen ("/dev/ttyACM0", 115200);
	  serialClose(fd);
	  fd=serialOpen ("/dev/ttyACM0", 115200);
	  
	  fd2=serialOpen ("/dev/ttyACM1", 115200);
	  serialClose(fd2);
	  fd2=serialOpen ("/dev/ttyACM1", 115200);
	  
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
	  
	  if (MCP3204_init(&fileDescriptor,"/dev/spidev1.2",&ad_MCP3204,mode_SPI_00,4.08,error))
		{
			printf("Cannot initialize the MCP3204 ADC.\n");
			printf("%s\n",error);
			exit(1);
		}
	  
	  /* Initialize model */
	  carga_sf_initialize();
	
	  /* Simulating the model step behavior (in non real-time) to
	   *  simulate model behavior at stop time.
	   */
	
	  k=0;
	  i=0;
	  int estado=0;
	  
	  /* get current time */
	  clock_gettime(0,&t);
	  /* start after one second */
	  t.tv_sec++;
	  
	  while ((rtmGetErrorStatus(carga_sf_M) == (NULL)) && !rtmGetStopRequested
	         (carga_sf_M)) {
	    
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
	    /*
	    delay(1);
	    i++; 
	    if(i==39){
			i=0;
		}*/
		//k++;
		if(k==60001){
			break;
		}
	  }
	
	  /* Disable rt_OneStep() here */
	
	  /* Terminate model */
	  carga_sf_terminate();
	  return 0;
	}
	
	/*
	 * File trailer for generated code.
	 *
	 * [EOF]
	 */
	
