//codigo para el desarrollo de la aplicacion de un examen auditivo 
//por medio de una señal senoidal enviada a diferentes frecuencias 


#include <stdio.h>
#include "stm32f7xx.h"
#include "math.h"

float t=0;
float funcion=0,f=2;
int n=315;
float seno[315];// arreglo 315 valores de la función seno
int freq_table[8] = {0, 200, 400,600, 800, 1000, 1200, 1400};//arreglo para manejar diferentes frecuencias de la función 
int sel=0;
int period=1000000 / 262;



extern "C"
{

void EXTI15_10_IRQHandler (void){ //interrupcion para el pulsador (PC13)
EXTI->PR |=(1UL<<13);  //va el pin
sel++;                 //selector de frecuencia 
	if (sel > 8) sel = 0; //reinicia a la frecuancia inicial  
	for(int i=0; i<10000; i++){}//delay

}
}

int main(void)

{
	
	//***************************
	//CONFIGURACION "CLOCK"
	RCC->AHB1ENR |= (1UL << 0);    //PRENDER EL CLOCK DEL PTA
	RCC->AHB1ENR |= (1UL << 1);    //PRENDER EL CLOCK DEL PTB
	RCC->APB2ENR |= (1UL<<14); //habilitar la interrupcion

	//***************************
	//CONFIGURACION DAC
	RCC->APB1ENR |= 0x20000000;      // Habilitar el reloj del DAC o 2000
  DAC->CR |= 0x00010001;           // DAC control reg, ambos canales habilitados
  GPIOA->MODER |= 0x00000F00;      // PTA4 y PTA5 en modo analogico
GPIOC->MODER &=  ~(3UL << 2*13); //pulsador como entrada (PC13)
	//CREAR LA SEÑAL SENOSOIDAL

		SYSCFG -> EXTICR[3]=0x20;
	EXTI -> IMR |= (1UL<<13);//Va el pin a utilizar
	EXTI -> RTSR |= (1UL<<13);//rising edge pin a utilizar
	NVIC_EnableIRQ(EXTI15_10_IRQn);  //EMPAQUETAMIENTO

	  


				
while(true){        //bucle infinito
		

for (int i=0;i<n;i++)
			 {   
				 period = 1000000 / freq_table[sel];  //periodo para frecuancias determinadas
				 t = t + 1;	 //se incrementa el tiempo para dar valores a la función 
				 funcion=(sin(t*3.14f * 2/period)*2047)+2047;        //f(t)=A*sen (wt + phi) = A*sen(2*pi*f*t+fase)
				 seno[i]=funcion; //guarda los valores de la función en un arreglo
				}		

		for (int i=0;i<n;i++)//for para enviar los valores de la función al DAC 
			 {   
			     DAC->DHR12R1 = seno[i]; //conversion analogica de los valore de la función 
				 
			 }
		
		  		
	}//cierra while
	
}//cierra main