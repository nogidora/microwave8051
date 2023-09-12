#include <at89x52.h>
#include <string.h>
#include <utils.h>

// ta sobrando 2 pinos de P2

// ---------------- Vari�veis ------------------ //
char Power = '9'; // a potencia maxima tem que ser 9 por que "10" usa mais que 8 bits..
				  // podemos imprimir 10 quando a potencia for 9 etc
				  
				  
char NumberOfDigits = '0';		// Armazena a quantidade de digitos para facilitar a utiliza��o do display 7 seg.
char Number1, Number2, Number3, Number4 = '\0'; // N�meros correspondente aos displays 4, 3, 2 e 1, respectivamente.

void main () {
	P0 = 0x00;					// Inicializa P0 como sa�da.
	config_lcd();
	write_msg("Inicializando...");
	delay_ms(1);
	keyboard_input();
	delay_ms(1);
	
}


void set_display (char Number) {
	if (NumberOfDigits == '0') {
		Number1 = Number;
		NumberOfDigits++;
	}
	else if (NumberOfDigits == '1') {
		Number2 = Number1;
		Number1 = Number;
		NumberOfDigits++;
	}
	else if (NumberOfDigits == '2') {
		Number3 = Number2;
		Number2 = Number1;
		Number1 = Number;
		NumberOfDigits++;
	}
	else if (NumberOfDigits == '3') {
		Number4 = Number3;
		Number3 = Number2;
		Number2 = Number1;
		Number1 = Number;
		NumberOfDigits++;
	}
	else {
		NumberOfDigits = '\0';
		Number1 = '\0';
		Number2 = '\0';
		Number3 = '\0';
		Number4 = '\0';
		set_display(Number);
	}
}

void print_display (void) {

	display_4(Number4);
	display_3(Number3);
	display_2(Number2);
	display_1(Number1);
	delay_5us();
	return;
}




// ------------- Fun��es do Teclado ------------- //

void keyboard_input(void) {
	while(1) {	
		check_line_0();			// Verifica a linha 0 do keypad.
		check_line_1();			// Verifica a linha 1 do keypad.
		check_line_2();			// Verifica a linha 2 do keypad.
		check_line_3();			// Verifica a linha 3 do keypad.
		if (NumberOfDigits >'0') {
			print_display();
		}
	}
}


void check_line_0(void) {
	Linha0 = 0;					// Ao zerar uma linha, esta fica ativa para receber entradas
	Linha1 = 1;					// Ao setar uma linha, esta fica inativa para entradas.
	Linha2 = 1;
	Linha3 = 1;
	if (Coluna0 == 0) {			// Tecla 1.
	
		set_display(1);					while(Coluna0 == 0);	// Debouncing do bot�o. Garante que n�o ser� acionado mais de uma vez antes de ser solto.
	}
	
	if (Coluna1 == 0) {			// Tecla 2.
	
		set_display(2);
		while(Coluna1 == 0);		
	}
	
	if (Coluna2 == 0) {			// Tecla 3.
	
		set_display(3);
		while(Coluna2 == 0);
	}
	delay_50us();				// Delay de 50 us.
}

void check_line_1(void) {
	Linha0 = 1;
	Linha1 = 0;					// Ativa a linha 1
	Linha2 = 1;
	Linha3 = 1;
	if (Coluna0 == 0) {			// Tecla 4.
	
		set_display(4);
		while(Coluna0 == 0);
	}
	
	if (Coluna1 == 0) {			// Tecla 5.
	
		set_display(5);
		while(Coluna1 == 0);
	}
	
	if (Coluna2 == 0) {			// Tecla 6.
	
		set_display(6);
		while(Coluna2 == 0);
	}
	delay_50us();				// Delay de 50 us.
}

void check_line_2(void) {
	Linha0 = 1;
	Linha1 = 1;
	Linha2 = 0;					// Ativa a linha 2
	Linha3 = 1;
	if (Coluna0 == 0) {			// Tecla 7.
	
		set_display(7);
		while(Coluna0 == 0);
	}
	
	if (Coluna1 == 0) {				// Tecla 8.
	
		set_display(8);
		while(Coluna1 == 0);
	}
	
	if (Coluna2 == 0) {				// Tecla 9.
	
		set_display(9);
		while(Coluna2 == 0);
	}
	delay_50us();				// Delay de 50 us.
}
void check_line_3(void) {

	Linha0 = 1;
	Linha1 = 1;
	Linha2 = 1;
	Linha3 = 0;						// Ativa a linha 3
	if (Coluna0 == 0) {				// Tecla *. 
	
		timer_dec();  // Aqui pode ser o start_stop / add + 30 segndos (usando uma flag)
		while(Coluna0 == 0);
	}
	
	if (Coluna1 == 0) {				// Tecla 0.
	
		set_display(0);
		while(Coluna1 == 0);
	}
	
	if (Coluna2 == 0) {				// Tecla #.
	
		set_power();
		while(Coluna2 == 0);
	}
	delay_50us();				// Delay de 50 us.
}


// ------------- Fun��o que converte os n�meros para o sinal da porta ------------- //
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

// ------------- Fun��es que atualizam o display ------------- //
void display_1(char number) {
	P2_4 = 0;
	P2_5 = 0;
	delay_50us();
	number_to_port(number);
	delay_50us();
	return;
}

void display_2(char number) {
	P2_4 = 1;
	P2_5 = 0;
	delay_50us();
	number_to_port(number);
	delay_50us();
	return;
}

void display_3(char number) {
	P2_4 = 0;
	P2_5 = 1;
	delay_50us();
	number_to_port(number);
	delay_50us();
	return;
}

void display_4(char number) {
	P2_4 = 1;
	P2_5 = 1;
	delay_50us();
	number_to_port(number);
	delay_50us();
	return;
}



// ------------- Interrup��es / Parar/Continuar ------------- //
void open_door (void) interrupt 0 { // Utiliza interrup��o externa 0 (maior prioridade)
	while (P3_2 == 0);
	// TR1 = !(TR1) // Para ou reinicia a contagem
}

void stop_start (void) interrupt 3 { // Utiliza interrup��o externa 1
	P3_3 = !(P3_3);
	// TR1 = !(TR1) // Para ou reinicia a contagem
	return;

}

//void time_out (void) interrupt X`{
	// Quando estoura o timer: beepa o buzzer 5 vezes e volta pro in�cio. (keypad input)
//}


// ------------- Controle de Pot�ncia ------------- //
void set_power (void) { // potencia come�a em 10 e a cada clique do bot�o diminui em 1 unidade. potencia maxima = 10
	if (Power == '0'){
		Power = '9';
		delay_50us();
		write_msg("Potencia: ");
		LCD = '1';
		wr_char();
		LCD = '0';
		wr_char();
		return;
	}
	else {
		Power--;
	}
	delay_ms(1);
	write_msg("Potencia: ");
	delay_50us();
	LCD = (Power + 1);
	wr_char();
	delay_5us();
	return;
}

// ------------- Temporizador decrescente ------------- //
void timer_dec (void) {			// Quando chama essa fun��o n�o ser� feito mais nada al�m de
	while (!((Number1 == '\0') && (Number2 == '\0') && (Number3 == '\0') && (Number4 == '\0'))) {					//  contar o tempo e acionar o motor, ent�o n�o ser� tratado entradas do keypad durante sua execu��o
		delay_ms_print(42);
		Number1--;
		
		if (Number4 != '\0') {				// Se ainda tem dezenas de minutos
			if (Number3 == '\0'){			// Mas a unidade de minutos est� em zero
				if (Number2 == '\0'){		// E a dezena de segundos est� em zero
					if (Number1 == '\0'){	// E a Unidade de segundos tamb�m � zero
						delay_ms_print(42);
						Number4--;
						Number3 = 9;
						Number2 = 5;
						Number1 = 9;
					}
				}
			}
		}
		
		if (Number3 != '\0') {
			if (Number2 == '\0') {
				if (Number1 == '\0') {
					delay_ms_print(42);
					Number3--;
					Number2 = 5;
					Number1 = 9;
				}
			}
		}
		
		if (Number2 != '\0') {
			if (Number1 == '\0'){
				delay_ms_print(42);
				Number1 = 9;
				Number2--;
			}
		}
	
		if (Number1 != '\0'){
		}
	}
	write_msg("Finalizado");

	// Ao final setar a flag de interrup��o por timer T1. e tamb�m tratar essa interrup��o
}

void delay_ms_print(unsigned int ms) {	
	char i = 0;
	for (i = 0; i < ms; i++) {
		print_display();
		delay_ms(1);
	}
	return;
}
