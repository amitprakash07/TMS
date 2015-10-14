#include<reg52.h>
#include <string.h>
at 0x35 unsigned char CIPHER[]={0x1B,0x15,0x06,0x03}; // STORING VEHICLE NUMBER IN ENCRYPTED FORM
at 0x40 unsigned char KEY[]={0x15,0x38}; // STORING KEY FOR DECRYPTION
at 0x50 unsigned int SPEED; 	// SPEED OF THE VEHICLE
void INIT_TRANS_MODULE();
void TRANSMIT();
void CALCULATE_SPEED();

void main()
{
	int m,k;
	INIT_TRANS_MODULE(); 			// INITIALISING TRANSMITTER MODULE
	while(1)   							// SUPER LOOP
	{	 
		//RI=1;
		if(IE0==1)
		{
			//TR0=1;
			CALCULATE_SPEED();	
		}	
		if(RI==1)  							// if receive interrupt flag found
		 {
		    REN=0; 						// disable reception
		    RI=0;
		    for(k=0;k<5;k++)
		    {
					for(m=0;m<10;m++);    
		        TRANSMIT();			// transmitting function 
		    }
		    REN=1;     				
		 }
		 
	}
}




void INIT_TRANS_MODULE()
{
   //EA=1; 				 	// ENABLE ALL INTERRUPTS
   ES=1;  					// ENABLE SERIAL DATA TRANSMISSION INTERRUPT
   EX1=0; 					// DISABLE EXTERNAL INTERRUPT 1
   EX0=1; 					// ENABLE EXTERNAL INTERRUPT 0	
   TMOD=0x29;    			// TIMER0 STARTS WHEN (INT0) INTERRUPT OCCURS i.e. GATE=1 AND TIMER1 GATE=0 (0010 1001)
   IT0=1; 					// ENABLE EXTERNAL INTERRUPT 0 ONLY ON EDGE TRIGGERING
   TL0=TH0=0x00; 			// TIMER0 COUNTS TO ZERO
   TR1=1; 					// TIMER1 RUN FLAG=0
   SM0=0;
   SM1=1; 					// serial data transfer in mode 1
//   PCON=0x80; 				// baud rate modify bit
   TH1=0xe8; 				// baud rate timer value = 250 for 1000 baud rate   
//   TL1=0x00; 				// timer1 TL1=0
   REN=1;
   RI=0;
}   
    

void TRANSMIT()
{
	int cipher_len=strlen(CIPHER); 			 // length of CIPHER
	int key_len=strlen(KEY);    				 // length of KEY
	int i;
	//TR1=1; // starting timer 1
	for (i=0;i<cipher_len;i++)
	{
	   SBUF=CIPHER[i];        						// writing cipher bytes to sbuf
	   while(!TI);
	   TI=0;            								// wait for transmission interrupt flag
	}
	for (i=0;i<key_len;i++)
	{
	   SBUF=KEY[i];        							 // writing key bytes to sbuf
	   while(!TI);
	   TI=0;        									 // wait for transmission interrupt flag
	} 
	SBUF=SPEED;
	while(!TI);
	TI=0; 
	   
}

void CALCULATE_SPEED()
{
	IE0=0; 					// disabling interrupt
	if(TH0!=0 || TL0!=0)
	{
	SPEED=((0x02*0x03)/(int)TL0)*10; 	// transferring timer value to speed
	}
	else 
	{
	SPEED=0x00;
	}
	TH0=TL0=0x00; 				//resetting timer 0
	
}
