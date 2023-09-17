/*
 * main.c
 * Created on: Sep 10, 2023
 * Author: Omar
 *
 *      Use ATmega32 Microcontroller with frequency 1Mhz.
 *      FTimer = FCPU/N = 1Mhz/1024(Prescaler)= 1Khz..Approx.
 *      Ttimer = 1ms
 *      As Using 16bit Timer 1 Allows us to use more Compare Match Values (65536)
 *      SET Match to 1000 (1 Sec)
 *
 */

//////// Pre Processor Directives
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define Sec1EN (1<<PA0)
#define Sec2EN (1<<PA1)
#define Min1EN (1<<PA2)
#define Min2EN (1<<PA3)
#define Hr1EN (1<<PA4)
#define Hr2EN (1<<PA5)
#define Buzz (1<<PC4)
//////// Global Variable Deceleration
unsigned char second = 0;
unsigned char tick = 0;
unsigned char min = 0;
unsigned char hour = 0;

//unsigned char time[3]={0,0,0};

void Timer1_Initilaztion(void) {
	/*  Description :
	 * 	TCCR1A -  :
	 * 		Using Channel A in TIM1 ,  Bit 3 – FOC1A: =1
	 * 	TCCR1B :
	 * 		WGM12 =1 -  Mode 4 CTC OCR1A
	 * 		CS10,CS12 =1  - clkI/O/1024 (From prescaler)
	 * 	OCR1A -Output Compare Register 1 A : =1000
	 * 	TIMSK  Timer/Counter Interrupt Mask Register:
	 * 		OCIE1A =1
	 */

	TCCR1A = (1 << FOC1A);
	TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12);
	OCR1A = 1000;
	TIMSK |= (1 << OCIE1A);
}
ISR(TIMER1_COMPA_vect) {
	/*
	 * Description :
	 * Every Second (according to above calculation)AVR Sends an ISR
	 * 60 Second = 1 Min and 0 Second
	 * 60 Min = 1 HR and 0 Min and 0 Second
	 * Count Every ISR to Use Timer
	 *
	 */

	second++;
	tick++;


	if (second == 60) {
		second = 0;
		min++;
	}


	if (min == 60) {
		second = 0;
		min = 0;
		hour++;
	}
	if (hour == 24) {
		second = 0;
		min = 0;
		hour = 0;
	}
}

void Int0_Initilaztion(void) {
	/*	Configure External Interrupt INT0 with falling edge
	 * 	MCU Control Register – MCUCR :
	 * 	ISC01 ISC00
	 * 	1 		0 The falling edge of INT0 generates an interrupt request.
	 *
	 * General Interrupt Control Register –GICR
	 * INT0 =1
	 */
	sei(); // SREG = (1<<7) - Global I Bit Enable
	MCUCR |= (1 << ISC01);
	GICR |= (1 << INT0);

}
ISR(INT0_vect) {
	second = 0;
	min = 0;
	hour = 0;
	tick = 0;
}

void Int1_Initilaztion(void) {
	/*	Configure External Interrupt INT0 with Rising edge
	 * 	MCU Control Register – MCUCR :
	 * 	ISC11 ISC10
	 * 	1 		1 The rising edge of INT1 generates an interrupt request.
	 * General Interrupt Control Register –GICR
	 *  INT1 =1
	 *
	 */
	MCUCR |= (1 << ISC11) | (1 << ISC10);
	GICR |= (1 << INT1);

}
ISR(INT1_vect) {
	TCCR1B &= ~(1 << CS10) & ~(1 << CS12); // Disable Clock Source

	//TIMSK &=~(1<<OCIE1A); // Disable Mask Register
}

void Int2_Initilaztion(void) {
	/*	Configure External Interrupt INT0 with falling edge
	 * 	MCU Control and Status Register –MCUCSR
	 * 	ISC2
	 *
	 * 	If ISC2 is written to zero, a falling edge on INT2 activates the interrupt.
	 *
	 * 	General Interrupt Control Register –GICR :
	 * 	INT2 =1
	 *
	 *
	 */

	MCUCSR &= ~(1 << ISC2);
	GICR |= (1 << INT2);

}
ISR(INT2_vect) {
	TCCR1B |= (1 << CS10) | (1 << CS12); // Enable Clock Source Again
	/*
	 * TIMSK |= (1 << OCIE1A);
	 * Enable Mask Register Also Works but When Reset you need to resume again
	 * and may make calculation errors
	 */

}

void PinsPortsInitialize(void) {
	DDRA = 0X7F; // (PA0 -> PA6) Output

	DDRB &= ~(1 << PB2); // Input Resume Pin
	PORTB |= (1 << PB2); // Enable Internal Pull Up

	DDRC = 0X1F; // (PC0 -> PC4) Output

	DDRD &= ~(1 << PD2) & ~(1 << PD3);
	PORTD |= (1 << PD2); // Enable Internal Pull Up
}

int main() {

	Timer1_Initilaztion();
	Int0_Initilaztion();
	Int1_Initilaztion();
	Int2_Initilaztion();
	PinsPortsInitialize();

	while (1) {
		//PORT A Masking and don't mess with other ports
		//Enable PA0 ,Turn off all Other Used PortA , 7Seg No.6 Enable

		PORTA = (PORTA & 0XC0) | Sec1EN;
		/* Now Begin to Show First Second By Port C
		 * Our Objective is to divide a Tenth number into to digits
		 * Example No.53
		 * Any Number with remainder 10 , Results no from (0-9), 53%10 =3
		 * Also Using UNSIGNED CHAR has a benefit as if fraction appeared will be neglected ,No Float
		 * 53 / 10 equal 5.3 == 5.0 , 03/10 =0
		 * By Applying the above method
		 * Show 3 Number Then Show 5 in a very small fraction of toggling time
		 * will appear as consecutive 53
		 */
		PORTC = (PORTC & 0XF0) | (second % 10);
		_delay_us(3);
		PORTA = (PORTA & 0XC0) | Sec2EN;
		PORTC = (PORTC & 0XF0) | (second / 10);
		_delay_us(3);
		PORTA = (PORTA & 0XC0) | Min1EN;
		PORTC = (PORTC & 0XF0) | (min % 10);
		_delay_us(3);
		PORTA = (PORTA & 0XC0) | Min2EN;
		PORTC = (PORTC & 0XF0) | (min / 10);
		_delay_us(3);
		PORTA = (PORTA & 0XC0) | Hr1EN;
		PORTC = (PORTC & 0XF0) | (hour % 10);
		_delay_us(3);
		PORTA = (PORTA & 0XC0) | Hr2EN;
		PORTC = (PORTC & 0XF0) | (hour / 10);
		_delay_us(3);

		// Additional Buzzer added outside ISR to ensure Accurate ISR execution time
		if (tick == 60) {
			tick = 0;
			PORTC |= Buzz;
			_delay_ms(15);
			PORTC &= ~Buzz;
		}


	}

}
