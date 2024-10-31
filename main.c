#include <reg51.h>

sfr ldata = 0xA0;

sbit rw = P3^2;
sbit rs = P3^3;
sbit en = P3^4;
sbit startc = P3^0;
sbit eoc = P3^1;
sbit oe = P3^5;

sbit pinClock = P3^6;

void comandaLCD(unsigned char date);
void delay(unsigned int itime);
void display(char *p);
void initializareLCD();
void conversie();
void scrieLCD(unsigned char date);
void LCD_DATA_ADC(unsigned char nr);

signed int TAB[8];

void main() {
    unsigned char valoare_citita;
    P2 = 0x00;  // Port de intrare de la ADC
    P1 = 0xFF; // Port de iesire spre LCD
    initializareLCD();

    // Conversie valori din ADC
    oe = 1;
    delay(2); // Delay 2 ms
    display("Temperatura: ");

    while(1) {
        conversie(); // Conversie valori analog-digital
        delay(5);
        comandaLCD(0xC0);
        comandaLCD(0x06);
        valoare_citita = P1;
        LCD_DATA_ADC(valoare_citita);
    }
}

void initializareLCD() {
    comandaLCD(0x38); // 5x7 matrix
    comandaLCD(0x0C); // Display on, cursor off
    comandaLCD(0x80); // Prima linie, prima pozitie
    comandaLCD(0x0E);
}

void conversie() {
    startc = 1; // Pornim conversia
    delay(3); // Delay 1 ms
    startc = 0; // Impuls pentru start
    while(!eoc) {
        oe = 1;
    }
}

void comandaLCD(unsigned char date) {
    ldata = date;
    rs = 0;
    rw = 0;
    en = 1;
    delay(3);
    en = 0;
}

void scrieLCD(unsigned char date) {
    ldata = date;
    rs = 1;
    rw = 0;
    en = 1;
    delay(3);
    en = 0;
}

void delay(unsigned int itime) {
    unsigned int i, j;
    for(i = 0; i < itime; i++)
        for(j = 0; j < 1275; j++);
}

void display(char *p) {
    while(*p != '\0') {
        scrieLCD(*p++);
    }
}

void LCD_DATA_ADC(unsigned char nr) {
    int n, k = 0;
    int nr_2 = 2 * nr / 12.5 - 0.5;
    while(nr_2 > 0) {
        TAB[k] = nr_2 % 10;
        nr_2 = nr_2 / 10;
        k++;
    }
    k--;
    comandaLCD(0x06);
    for(n = k; n >= 0; n--) {
        scrieLCD(TAB[n] + 0x30); // Conversie in cod ASCII
    }
    scrieLCD(0xDF); // Caract. pentru grad în ASCII
    display("C");
}
