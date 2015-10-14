#include<reg52.h>
#include<string.h>
at 0x35 unsigned char vehicle_no[4];
at 0x40 unsigned speed[2];

void INIT_RECEIVE_MODULE();
void TRANSMIT();

void main()
{
	int i=0;
	int j=20;
	INIT_RECEIVE_MODULE();   // initialise receiver module
	TRANSMIT();
	while(1)
	{
		
		if(j==0)
		{
		TRANSMIT();    // transmit signal
		j=20;
		}
		j--;
		if(RI==1)
		{
		  if(i<4)
		  {
		  	vehicle_no[i]=SBUF;   // receiving vehicle number
		  }
		  else
		  {
		  	i=0;
		  	speed[i]=SBUF;   // receiving speed
		  	if(i==2)i=0;
		  }	
		  i++;
		 RI=0;
		}
		
	}
	
} 


void TRANSMIT()
{
	SBUF=0x55;	 // writing key bytes to sbuf
	while(!TI);
	TI=0;  
	REN=1;
//	RI=1;      // wait for transmission interrupt flag
}


void INIT_RECEIVE_MODULE()
{
	
	EX1=0; 					// DISABLE EXTERNAL INTERRUPT 1
   EX0=1; 					// ENABLE EXTERNAL INTERRUPT 0	
   TMOD=0x20;    			// TMOD (0010 0000)
   IT0=1; 					// ENABLE EXTERNAL INTERRUPT 0 ONLY ON EDGE TRIGGERING
   //TL0=TH0=0x00; 			// TIMER0 COUNTS TO ZERO
   TR0=0; 					// TIMER0 RUN FLAG=0
   TR1=1; 					// TIMER1 RUN FLAG=0
   SM0=0;
   SM1=1; 					// serial data transfer in mode 1	
   TH1=0xe4; 				// baud rate timer value = 250 for 1000 baud rate   
   REN=0;
//   RI=1;
}
