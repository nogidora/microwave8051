#include <at89x52.h>
#include <string.h>

#define LCD	P1
#define EN	P3_7
#define RS	P3_6
#define	DISPLAY P2

// ta sobrando 2 pinos de P2
void display_1(void); // Display 1 = Dezena minutos
void display_2(void); // Display 2 = Unidade minutos
void display_3(void); // Display 3 = Dezena segundos
void display_4(void); // Display 4 = Unidade segundos
void config_lcd(void);
void line_1(void);
void line_2(void);
void write_msg(char msg[]);
void wr_cmd(void);
void wr_char(void);
void delay_5us(void);



void set_power(void);
void delay_ms(unsigned int ms);

char power = '9'; // a potencia maxima tem que ser 9 por que "10" usa mais que 8 bits..
				  // podemos imprimir 10 quando a potencia for 9 etc


void main () {
	config_lcd();
	line_1();
	write_msg("Inicializando...");
	delay_ms(1);		
	while(1) {
		DISPLAY = 999;
	}
		
}




void display_1(void) {
	P2_0 = 1;
	P2_1 = 0;
	P2_2 = 1;
	P2_3 = 0;

}

void display_2(void) {
	P2_0 = 0;
	P2_1 = 0;
	P2_2 = 1;
	P2_3 = 0;

}

void display_3(void) {
	P2_0 = 1;
	P2_1 = 0;
	P2_2 = 0;
	P2_3 = 1;

}

void display_4(void) {
	P2_0 = 1;
	P2_1 = 0;
	P2_2 = 1;
	P2_3 = 0;

}


void set_power (void) { // potencia começa em 10 e a cada clique do botão diminui em 1 unidade. potencia maxima = 10
	if (power == '0'){
		power = '9';
	}
	else{
		power--;
	}
	delay_ms(10);
	write_msg("Potencia: ");
	delay_ms(1);
	LCD = power;
	wr_char();
	delay_5us();
}


// ----- Funções do LCD -----//

void config_lcd(void) {
	LCD = 0x38;
	wr_cmd();
	LCD = 0x06;
	wr_cmd();
	LCD = 0x0E;
	wr_cmd();
	LCD = 0x01;
	wr_cmd();
}

void line_1(void) {	
	LCD = 0x00;
	wr_cmd();
}

void line_2(void) {
	LCD = 0xC0;
	wr_cmd();
}

void clear_lcd(void) {
	LCD = 0x01;
	wr_cmd();

}

void write_msg(char msg[]) {
	unsigned char i;
	unsigned char len;
	
	len = strlen(msg);
	
	line_1();
	clear_lcd();
	
	for(i = 0; i < len; i++)
	{
		LCD = msg[i];
		wr_char();
		delay_5us();
	}
	return;
}

void wr_cmd(void)
{
	RS = 0;
	EN = 1;
	delay_5us();
	EN = 0;
	delay_ms(5);
}

void wr_char(void)
{
	RS = 1;
	EN = 1;
	delay_5us();
	EN = 0;
	delay_ms(5);
}

void delay_5us(void)
{
	unsigned char i;
	for(i = 0; i < 5; i++){}
}


void delay_ms(unsigned int ms) {			// Delay por polling via T/C 0.
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