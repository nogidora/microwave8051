#include <at89x52.h>
#include <string.h>
 
 
#define LCD	P0
#define EN	P3_7
#define RS	P3_6

#define Linha0 P1_0
#define Linha1 P1_1
#define Linha2 P1_2
#define Linha3 P1_3
#define Coluna0 P1_4
#define Coluna1 P1_5
#define Coluna2 P1_6


 
void display_1(char number); // Display 1 = Unidade segundos
void display_2(char number); // Display 2 = Dezena segundos
void display_3(char number); // Display 3 = Unidade minutos
void display_4(char number); // Display 4 = Dezena minutos

void keyboard_input(void);
void check_line_0(void);
void check_line_1(void);
void check_line_2(void);	
void check_line_3(void);



void number_to_port (char number);
void set_display (char Number);
void print_display (void);
void print_display_sec (void);
void print_display_keypad (void);


void config_lcd(void);
void line_1(void);
//void line_2(void);
void write_msg(char msg[]);
void wr_cmd(void);
void wr_char(void);
void clear_lcd(void);


void ISR_open_door (void);
void start_stop (void);

void set_power(void);

void timer_dec (void);

void delay_5us(void);
void delay_50us(void);
void delay_ms(unsigned int ms);
void delay_ms_print(unsigned int ms);

// ------------- Funções do LCD ------------- //
void config_lcd(void) {
	LCD = 0x38;		// Define 8 bits, duas linhas e matriz de 5x7 pontos.
	wr_cmd();    	// Escreve comando.
	//LCD = 0x06;	// Deslocamento do cursor E->D.
	//wr_cmd();    	// Escreve comando.
	LCD = 0x0C;		// Display ligado, cursor desligado.
	wr_cmd();    	// Escreve comando.
	LCD = 0x01;		// Limpa o display do LCD.
	wr_cmd();    	// Escreve comando.
	line_1();
}

void line_1(void) {	
	LCD = 0x00;
	wr_cmd();
}

/* 
void line_2(void) {
	LCD = 0xC0;
	wr_cmd();
} */

void clear_lcd(void) {
	LCD = 0x01;
	wr_cmd();

}

void write_msg(char msg[]) {
	unsigned char i;
	unsigned char len;
	
	len = strlen(msg);
	
	clear_lcd();
	line_1();
	
	
	for(i = 0; i < len; i++)
	{
		LCD = msg[i];
		wr_char();
		delay_5us();
	}
	return;
}

void wr_cmd(void) {
	RS = 0;
	EN = 1;
	delay_5us();
	EN = 0;
	delay_ms(5);
}

void wr_char(void) {
	RS = 1;
	EN = 1;
	delay_5us();
	EN = 0;
	delay_ms(5);
}



// ------------- Funções de Delay ------------- //
void delay_5us(void) {
	unsigned char i;
	for(i = 0; i < 5; i++){}
}


void delay_50us(void) {
	unsigned char i;
	for(i = 0; i < 50; i++){}
}


void delay_ms(unsigned int ms) {		// Delay por polling via T/C 0.
	TMOD |= 0x01;						// A operação OU preserva alguma eventual configuração prévia do T/C 1.
	
	while(ms){
		TH0 = 0xFC;						// Valor de recarga para 1 ms @ f = 11.0592 MHz (i.e. 64614).
		TL0 = 0x66;
		TR0 = 1;
		while(!TF0);
		TF0 = 0;
		TR0 = 0;
		ms--;
	}
}
