# DigitalClock_AVR32
Digital Clock Based on Atmel Atmega32 MC a Mini Project for Summing up Interfacing 1 Course by Edges for Training -MT Diploma. 

The clock has three seven-segment displays to show the hours, minutes, and seconds. It also has a buzzer that sounds every minute.

# Requirements
1. Use ATmega32 Microcontroller with frequency 1Mhz.
2. Configure Timer1 in ATmega32 with CTC mode to count the Stop Watch time.
3. Use six Common Anode 7-segments.
4. Connect 7447 decoder 4-pins to the first 4-pins in PORTC
Use first 6-pins in PORTA as the enable/disable pins for the six 7-segments
3. Stop Watch counting should start once the power is connected to the MCU.
3. Configure  **External Interrupt INT0**  with falling edge. Connect a push button with the internal pull-up resistor. If a falling edge detected the Stop Watch time should be reset.
3. Configure **External Interrupt INT1**  with raising edge. Connect a push button with the external pull-down resistor. If a raising edge detected the Stop Watch time should be paused.
3. Configure **External Interrupt INT2**  with falling edge. Connect a push button with the internal pull-up resistor. If a falling edge detected the Stop Watch time should be resumed
# Project Covers
- Shifting and bit masking
- Pre Processor Directives
- Interrupts
- Timer 
- GPIO
