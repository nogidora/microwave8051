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


// ta sobrando 2 pinos de P2
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


void config_lcd(void);
void line_1(void);
void line_2(void);
void write_msg(char msg[]);
void wr_cmd(void);
void wr_char(void);


void open_door (void);
void start_stop (void);

void set_power(void);



void delay_5us(void);
void delay_50us(void);
void delay_ms(unsigned int ms);



// ---------------- Variáveis ------------------ //
char power = '9'; // a potencia maxima tem que ser 9 por que "10" usa mais que 8 bits..
				  // podemos imprimir 10 quando a potencia for 9 etc

void main () {
	P0 = 0x00;					// Inicializa P0 como saída.
	config_lcd();
	write_msg("Inicializando...");
	delay_ms(1);
	

	
	keyboard_input();
	delay_ms(1);
	
}

void keyboard_input(void) {
	while(1) {	
		check_line_0();			// Verifica a linha 0 do keypad.
		check_line_1();			// Verifica a linha 1 do keypad.
		check_line_2();			// Verifica a linha 2 do keypad.
		check_line_3();			// Verifica a linha 3 do keypad.
	}
}

void check_line_0(void) {
	Linha0 = 0;					// Ao zerar uma linha, esta fica ativa para receber entradas
	Linha1 = 1;					// Ao setar uma linha, esta fica inativa para entradas.
	Linha2 = 1;
	Linha3 = 1;
	if (Coluna0 == 0) {			// Tecla 1.
	
		display_1(1);			// Escreve na serial.
		while(Coluna0 == 0);	// Debouncing do botão. Garante que não será acionado mais de uma vez antes de ser solto.
	}
	
	if (Coluna1 == 0) {			// Tecla 2.
	
		display_1(2);
		while(Coluna1 == 0);		
	}
	
	if (Coluna2 == 0) {			// Tecla 3.
	
		display_1(3);
		while(Coluna2 == 0);
	}
	delay_ms(1);				// Delay de 1 ms.
}

void check_line_1(void) {
	Linha0 = 1;
	Linha1 = 0;					// Ativa a linha 1
	Linha2 = 1;
	Linha3 = 1;
	if (Coluna0 == 0) {			// Tecla 4.
	
		display_1(4);
		while(Coluna0 == 0);
	}
	
	if (Coluna1 == 0) {			// Tecla 5.
	
		display_1(5);
		while(Coluna1 == 0);
	}
	
	if (Coluna2 == 0) {			// Tecla 6.
	
		display_1(6);
		while(Coluna2 == 0);
	}
	delay_ms(1);				// Delay de 1 ms.
}

void check_line_2(void) {
	Linha0 = 1;
	Linha1 = 1;
	Linha2 = 0;					// Ativa a linha 2
	Linha3 = 1;
	if (Coluna0 == 0) {			// Tecla 7.
	
		display_1(7);
		while(Coluna0 == 0);
	}
	
	if (Coluna1 == 0) {				// Tecla 8.
	
		display_1(8);
		while(Coluna1 == 0);
	}
	
	if (Coluna2 == 0) {				// Tecla 9.
	
		display_1(9);
		while(Coluna2 == 0);
	}
	delay_ms(1);					// Delay de 1 ms.
}
void check_line_3(void) {

	Linha0 = 1;
	Linha1 = 1;
	Linha2 = 1;
	Linha3 = 0;						// Ativa a linha 3
	if (Coluna0 == 0) {				// Tecla *.
	
		display_1('*');
		while(Coluna0 == 0);
	}
	
	if (Coluna1 == 0) {				// Tecla 0.
	
		display_1(0);
		while(Coluna1 == 0);
	}
	
	if (Coluna2 == 0) {				// Tecla #.
	
		set_power();
		while(Coluna2 == 0);
	}
	delay_ms(1);					// Delay de 1 ms.
}



void number_to_port (char number){
	if (number == 9){
		P2_0 = 1;
		P2_1 = 0;
		P2_2 = 0;
		P2_3 = 1;
		delay_50us();

	}
	else if (number == 8){
		P2_0 = 0;
		P2_1 = 0;
		P2_2 = 0;
		P2_3 = 1;
		delay_50us();
	}
	else if (number == 7){
		P2_0 = 1;
		P2_1 = 1;
		P2_2 = 1;
		P2_3 = 0;
		delay_50us();
	}
	else if (number == 6){
		P2_0 = 0;
		P2_1 = 1;
		P2_2 = 1;
		P2_3 = 0;
		delay_50us();
	}
	else if (number == 5){
		P2_0 = 1;
		P2_1 = 0;
		P2_2 = 1;
		P2_3 = 0;
		delay_50us();
	}
	else if (number == 4){
		P2_0 = 0;
		P2_1 = 0;
		P2_2 = 1;
		P2_3 = 0;
		delay_50us();	
	}
	else if (number == 3){
		P2_0 = 1;
		P2_1 = 1;
		P2_2 = 0;
		P2_3 = 0;
		delay_50us();
	}
	else if (number == 2){
		P2_0 = 0;
		P2_1 = 1;
		P2_2 = 0;
		P2_3 = 0;
		delay_50us();	
	}
	else if (number == 1){
		P2_0 = 1;
		P2_1 = 0;
		P2_2 = 0;
		P2_3 = 0;
		delay_50us();
	}
	else if (number == 0){
		P2_0 = 0;
		P2_1 = 0;
		P2_2 = 0;
		P2_3 = 0;
		delay_50us();
	}
	
	delay_ms(5);
}


void display_1(unsigned char number) {
	P2_4 = 0;
	P2_5 = 0;
	delay_50us();
	number_to_port(number);
	delay_50us();
	//return;
}

void display_2(unsigned char number) {
	P2_4 = 1;
	P2_5 = 0;
	delay_50us();
	number_to_port(number);
	delay_50us();
}

void display_3(char number) {
	P2_4 = 0;
	P2_5 = 1;
	delay_50us();
	number_to_port(number);
	delay_50us();

}

void display_4(char number) {
	P2_4 = 1;
	P2_5 = 1;
	delay_50us();
	number_to_port(number);
	delay_50us();
}



// ------------- Interrupções / Parar/Continuar ------------- //
void open_door (void) interrupt 0 { // Utiliza interrupção externa 0 (maior prioridade)
	while (P3_2 == 0);
	// TR1 = !(TR1) // Para ou reinicia a contagem
}

void stop_start (void) interrupt 3 { // Utiliza interrupção externa 1
	P3_3 = !(P3_3);
	// TR1 = !(TR1) // Para ou reinicia a contagem
	return;

}


// ------------- Controle de Potência ------------- //
void set_power (void) { // potencia começa em 10 e a cada clique do botão diminui em 1 unidade. potencia maxima = 10
	if (power == '0'){
		power = '9';
		delay_50us();
		write_msg("Potencia: ");
		LCD = '1';
		wr_char();
		LCD = '0';
		wr_char();
		return;
	}
	else {
		power--;
	}
	delay_ms(1);
	write_msg("Potencia: ");
	delay_50us();
	LCD = (power + 1);
	wr_char();
	delay_5us();
	return;
}


// ------------- Funções do LCD ------------- //
void config_lcd(void) {
	LCD = 0x38;		// Define 8 bits, duas linhas e matriz de 5x7 pontos.
	wr_cmd();    	// Escreve comando.
	//LCD = 0x06;		// Deslocamento do cursor E->D.
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

void wr_cmd(void)
{
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