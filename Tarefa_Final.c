#include <at89x52.h>
#include <string.h>
#include <utils.h>

// Anotações
// - sobrando 2 pinos de P2

// ---------------- Variáveis ------------------ //
char Power = '9'; // A potência máxima deve ser 9 por que "10" usaria mais que 8 bits.
				  
char NumberOfDigits = 0;						// Armazena a quantidade de digitos para facilitar a utilização do display 7 seg.
char Number1, Number2, Number3, Number4 = 0;	// Números correspondente aos displays 4, 3, 2 e 1, respectivamente.
char PauseFlag = 0;

void main() {
	IE = 0x85;						// Habilita interrupções globais, interrupção externa 0, 1
	P0 = 0x00;						// Inicializa P0 como saída.
	P2_6 = 0;
	IT1 = 1;						// Interrupção externa sensível a borda de descida.
	config_lcd();					// Configura display LCD.
	write_msg("Inicializando..."); 	// Mensagem Inicial.
	delay_ms(500);
	clear_lcd();
	keyboard_input();				// Função que recebe entrada de tempo.
	delay_ms(1);
}

// ------------- Funções do Display de 7 Segmentos ------------- // 
void set_display(char Number) {			// Ordena os números conforme a quantidade inserida
	if (NumberOfDigits == 0) {	
		Number1 = Number;
	}
	else if (NumberOfDigits == 1) {
		Number2 = Number1;
		Number1 = Number;
	}
	else if (NumberOfDigits == 2) {
		Number3 = Number2;
		Number2 = Number1;
		Number1 = Number;
	}
	else if (NumberOfDigits == 3) {
		Number4 = Number3;
		Number3 = Number2;
		Number2 = Number1;
		Number1 = Number;
	}
	else {						// Se insere mais de 4 dígitos a leitura recomeça.
		NumberOfDigits = 0;
		Number1 = Number2 = Number3 = Number4 = 0;
		set_display(Number1);
		return;
	}
	NumberOfDigits++;
}

void print_display (void) {
	display_4(Number4);			// Coloca cada dígito em seu devido display.
	display_3(Number3);
	display_2(Number2);
	display_1(Number1);
	delay_50us();				// Delay para que exibição no display.
}

// ------------- Funções do Teclado ------------- //
void keyboard_input(void) {			// Obtem entrada do teclado.
	write_msg("Insira o tempo");

	while(1) {	
		check_line_0();				// Verifica a linha 0 do keypad.
		check_line_1();				// Verifica a linha 1 do keypad.
		check_line_2();				// Verifica a linha 2 do keypad.
		check_line_3();				// Verifica a linha 3 do keypad.
		print_display();			// Imprime os dígitos conforme são inseridos
	}
}


void check_line_0(void) {			// Conjunto de funções que verificam as linhas
	Linha0 = 0;						// Ao zerar uma linha, esta fica ativa para receber entradas
	Linha1 = Linha2 = Linha3 = 1;	// Ao setar uma linha, esta fica inativa para entradas.

	if (Coluna0 == 0) {				// Tecla 1.
		set_display(1);					
		while(Coluna0 == 0) {
			print_display();
		}		// Debouncing do botão. Garante que não será acionado mais de uma vez antes de ser solto.
	}
	if (Coluna1 == 0) {				// Tecla 2.
		set_display(2);
		while(Coluna1 == 0) {
			print_display();
		}		
	}
	if (Coluna2 == 0) {				// Tecla 3.
		set_display(3);
		while(Coluna2 == 0) {
			print_display();
		}
	}
	delay_50us();					// Delay de 5 us.
}

void check_line_1(void) {
	Linha1 = 0;						// Ativa a linha 1
	Linha0 = Linha2 = Linha3 = 1;

	if (Coluna0 == 0) {				// Tecla 4.
		set_display(4);
		while(Coluna0 == 0) {
			print_display();
		}
	}
	if (Coluna1 == 0) {				// Tecla 5.
		set_display(5);
		while(Coluna1 == 0) {
			print_display();
		}
	}
	if (Coluna2 == 0) {				// Tecla 6.
		set_display(6);
		while(Coluna2 == 0) {
			print_display();
		}
	}
	delay_50us();					// Delay de 5 us.
}

void check_line_2(void) {
	Linha2 = 0;						// Ativa a linha 2
	Linha0 = Linha1 = Linha3 = 1;

	if (Coluna0 == 0) {				// Tecla 7.
		set_display(7);
		while(Coluna0 == 0) {
			print_display();
		}
	}
	if (Coluna1 == 0) {				// Tecla 8.
		set_display(8);
		while(Coluna1 == 0) {
			print_display();
		}
	}
	if (Coluna2 == 0) {				// Tecla 9.
		set_display(9);
		while(Coluna2 == 0) {
			print_display();
		}
	}
	delay_50us();					// Delay de 5 us.
}

void check_line_3(void) {
	Linha3 = 0;						// Ativa a linha 3
	Linha0 = Linha1 = Linha2 = 1;
	
	if (Coluna0 == 0) {				// Tecla * corresponde ao acionamento do forno. 
		timer_dec(); 				// Aqui pode ser o start_stop / add + 30 segndos (usando uma flag) ?
		while(Coluna0 == 0) {
			print_display();
		}
	}
	if (Coluna1 == 0) {				// Tecla 0.
		set_display(0);
		while(Coluna1 == 0) {
			print_display();
		}
	}
	if (Coluna2 == 0) {				// Tecla # corresponde ao ajuste de potência.
		set_power();
		while(Coluna2 == 0) {
			print_display();
		}
	}
	delay_50us();				// Delay de 5 us.
}


// ------------- Função que converte os números para o sinal da porta ------------- //
void number_to_port(char number) {
	if (number == 9){
		P2_0 = P2_3 = 1;
		P2_1 = P2_2 = 0;
	}
	else if (number == 8) {
		P2_0 = P2_1 = P2_2 = 0;
		P2_3 = 1;
	}
	else if (number == 7) {
		P2_0 = P2_1 = P2_2 = 1;
		P2_3 = 0;
	}
	else if (number == 6) {
		P2_0 = P2_3 = 0;
		P2_1 = P2_2 = 1;
	}
	else if (number == 5) {
		P2_0 = P2_2 = 1;
		P2_1 = P2_3 = 0;
	}
	else if (number == 4) {
		P2_0 = P2_1 = P2_3 = 0;
		P2_2 = 1;
	}
	else if (number == 3) {
		P2_0 = P2_1 = 1;
		P2_2 = P2_3 = 0;
	}
	else if (number == 2) {
		P2_0 = P2_2 = P2_3 = 0;
		P2_1 = 1;
	}
	else if (number == 1) {
		P2_0 = 1;
		P2_1 = P2_2 = P2_3 = 0;
	}
	else if (number == 0) {
		P2_0 = P2_1 = P2_2 = P2_3 = 0;
	}
	delay_50us();
	delay_ms(5);
}

// ------------- Funções que atualizam o display ------------- //
void display_1(char number) {
	P2_4 = P2_5 = 0;					// Seleciona o display de 7 segmentos acionado pelos pinos do MC.
	delay_50us();						// Delay para evitar conflito de dados durante uma troca de número
	number_to_port(number);
	delay_50us();
}

void display_2(char number) {
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
	P2_4 = P2_5 = 1;
	delay_50us();
	number_to_port(number);
	delay_50us();
}

// ------------- Interrupções / Parar/Continuar ------------- //
void ISR_open_door(void) interrupt 0 { 	// Utiliza interrupção externa 0 (maior prioridade)
	write_msg("PORTA ABERTA");
	while (P3_2 == 0){
		print_display();
	}
	clear_lcd();
	timer_dec();
	//IE0 = 0;
}

void ISR_stop_start (void) interrupt 2 {		// Utiliza interrupção externa 1
	write_msg("Pause");
	PauseFlag = !(PauseFlag);
	//IE1 = 0;
	while (PauseFlag) {							// Não despausa
		print_display();	
	}
	write_msg("Start");
	//clear_lcd();
	timer_dec();
}

void time_out(void) {
	int i;
	
	
	// Quando estoura o timer: beepa o buzzer 5 vezes e volta pro início. (keypad input)
	for (i = 0; i < 5; i++) {
		P2_6 = 1;
		write_msg("TIME OUT");
		delay_ms_print(20); // Não pisca todo display 7 seg.
		clear_lcd();
		P2_6 = 0;
		delay_ms(777);
	}
	keyboard_input();
}

// ------------- Controle de Potência ------------- //
void set_power(void) { // potencia começa em 10 e a cada clique do botão diminui em 1 unidade. potencia maxima = 10
	if (Power == '0') {
		Power = '9';
		delay_50us();
		write_msg("Potencia: ");
		LCD = '1';
		wr_char();
		LCD = '0';
		wr_char();
		return;
	}
	Power--;
	delay_ms(1);
	write_msg("Potencia: ");
	delay_50us();
	LCD = (Power + 1);
	wr_char();
	delay_50us();
}

// ------------- Temporizador decrescente ------------- //
void timer_dec(void) {			// Quando chama essa função não será feito mais nada além de
	while (!((Number1 == 0) && (Number2 == 0) && (Number3 == 0) && (Number4 == 0))) {					//  contar o tempo e acionar o motor, ent�o n�o ser� tratado entradas do keypad durante sua execu��o
		delay_ms_print(42);
		if (Number4 != 0 && Number3 == 0 && Number2 == 0 && Number1 == 0) { 	
			delay_ms_print(42);			// Se ainda tem dezenas de minutos				
			Number4--;					// Mas a unidade de minutos estão em zero
			Number3 = 9;				// E a dezena de segundos estão em zero
			Number2 = 5;				// E a Unidade de segundos também é zero
			Number1 = 9;
		}
		if (Number3 != 0 && Number2 == 0 && Number1 == 0) {
			delay_ms_print(42);
			Number3--;
			Number2 = 5;
			Number1 = 9;
		}
		if (Number2 != 0 && Number1 == 0) {
			delay_ms_print(42);
			Number1 = 9;
			Number2--;
		}
		Number1--;
		
	}
	write_msg("Finalizado");
	time_out();
	// Ao final setar a flag de interrup��o por timer T1. e tamb�m tratar essa interrup��o
}

void delay_ms_print(unsigned int ms) {	
	char i;
	for (i = 0; i < ms; i++) {
		print_display();
		delay_ms(1);
	}
}
