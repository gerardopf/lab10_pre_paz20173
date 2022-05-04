/*
 * Lab10
 * Gerardo Paz Fuentes
 * 
 * 
 *
 * Fecha de creación: 03/05/22
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>         // registros del PIC
#include <stdio.h>
#include <stdlib.h>

/* -------------CONSTANTES ------------ */
#define _XTAL_FREQ 1000000
#define LEN_MSG 9       // largo de mensaje

/* ------------- VARIABLES ------------ */
char mensaje[LEN_MSG] = {'D','a','t','o',':',' ',' ', 0x0D, 0x0A};
uint8_t indice = 0; // posición del mensaje
uint8_t valor_old = 0;  // valor anterior
int estado = 0;

/* ------------- PROTOTIPOS DE FUNCIÓN ------------ */
void setup(void);

/* -------------CÓDIGO PRINCIPAL  ------------ */

void __interrupt() isr(void) {
    if(RCIF){   // si se reciben datos
        mensaje[6] = RCREG; // guardar en arreglo
        PORTD = mensaje[6]; // mostrar
    }
    return;
}

// Terminal - PÏC
/*void main(void) {
    setup();
    while (1) {
        indice = 0; // reiniciar índice
        if(valor_old!= mensaje[6]){ // que el valor sea diferente
            while(indice < LEN_MSG){
                if(PIR1bits.TXIF){      // esperar a que se libere TXREG para enviar por el serial
                    TXREG = mensaje[indice];
                    indice++;
                }
            }
            valor_old = mensaje[6]; // guardar para comparar después
        }
    }
}*/

// PIC - Terminal
void main(void) {
    setup();
    while (1) {
        __delay_us(100000);
        TXREG = 'a';
    }
}

void setup(void) {
    ANSEL = 0b00000001; // AN0
    ANSELH = 0;

    TRISA = 0b00000001; // AN0 in
    TRISD = 0;  // D out
    
    PORTA = 0;
    PORTD = 0;  // puertos limpios

    // INTERRUPCIONES
    INTCONbits.GIE = 1; // globales
    INTCONbits.PEIE = 1;    // periféricos
    PIE1bits.RCIE = 1;  // recepción

    // Oscilador 
    OSCCONbits.IRCF = 0b100; // 1MHz
    OSCCONbits.SCS = 1; // Oscilador interno
    
    // COMUNICACIÓN SERIAL
    TXSTAbits.SYNC = 0; // asíncroma (full duplex)
    TXSTAbits.BRGH = 1; // Baud rate de alta velocidad
    BAUDCTLbits.BRG16 = 1;  // 16 bits para el Baud rate
    
    SPBRG = 25;
    SPBRGH = 0; // Baud rate 9600, error 0.16%
    
    RCSTAbits.SPEN = 1; // comunicación habilitada
    TXSTAbits.TX9 = 0;  // solo 8 bits
    TXSTAbits.TXEN = 1; // transmisor habilitado
    RCSTAbits.CREN = 1; // receptor habilitado

    return;
}