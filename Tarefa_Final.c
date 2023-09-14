// ======================================= Universidade Federal de Pelotas
// ======================================= Centro de Desenvolvimento Tecnológico
// ======================================= Bacharelado em Engenharia de Computação
// ======================================= Disciplina: 22000279 --- Microcontroladores
// ======================================= Turma: 2023/1 --- M1
// ======================================= Professor: Alan Rossetto
//
//										  Descrição: Trabalho Final: Microondas controlado por 8051.
// 								
//
// 										  Identificação:
// 										  Nome da(o) aluna(o) & Matrícula: Giordano M. Rossa 17100479
// 										  Nome da(o) aluna(o) & Matrícula: Alberto Neuenfeld Helbig 18101080
//			


#include <at89x52.h>
#include <string.h>
#include <utils.h>

// ---------------- Variáveis ------------------ //
char Power = '9'; // A potência máxima usada é '9' por que "10" usaria mais que 8 bits.
				  
char NumberOfDigits = 0;						// Armazena a quantidade de digitos para facilitar a utilização do display 7 seg.
char Number1, Number2, Number3, Number4 = 0;	// Números correspondente aos displays 4, 3, 2 e 1, respectivamente. (Índice no proteus)

void main() {
	IE = 0x85;						// Habilita interrupções globais, interrupção externa 0, 1
	P0 = 0x00;						// Inicializa P0 como saída.
	BUZZER = 0;						// Buzzer inicia desativado.
	MOTOR = 0;						// Motor inicia desligado.
	LED = 1;						// Aciona o pino do LED pois o componente será ativo em nível baixo.
	IT0 = 0;						// Interrupção externa 0 sensível a nível lógico 0.
	IT1 = 0;						// Interrupção externa 1 sensível a nível lógico 0
	config_lcd();					// Configura display LCD.
	write_msg("Inicializando..."); 	// Mensagem Inicial.
	delay_ms(550);					// Delay para ler a mensagem
	clear_lcd();					// Limpa LCD
	keyboard_input();				// Função que recebe entrada de tempo.
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

// ------------- Funções que atualizam o display ------------- //
void display_1(char number) {
	P2_4 = 0;					        // Seleciona o display de 7 segmentos acionado pelos pinos do MC.
	P2_5 = 0;
	delay_50us();						// Delay para evitar conflito de dados durante uma troca de número.
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
	P2_4 = 1;
	P2_5 = 1;
	delay_50us();
	number_to_port(number);
	delay_50us();
}
// ------------- Função que converte os números para o sinal da porta ------------- //
void number_to_port(char number) {
	if (number == 9) {			// Verifica o número de entrada.
		P2_0 = 1;				// Aciona os respectivos pinos da porta P2.
		P2_1 = 0;
		P2_2 = 0;
		P2_3 = 1;
		delay_50us();

	}
	else if (number == 8) {
		P2_0 = 0;
		P2_1 = 0;
		P2_2 = 0;
		P2_3 = 1;
		delay_50us();
	}
	else if (number == 7) {
		P2_0 = 1;
		P2_1 = 1;
		P2_2 = 1;
		P2_3 = 0;
		delay_50us();
	}
	else if (number == 6) {
		P2_0 = 0;
		P2_1 = 1;
		P2_2 = 1;
		P2_3 = 0;
		delay_50us();
	}
	else if (number == 5) {
		P2_0 = 1;
		P2_1 = 0;
		P2_2 = 1;
		P2_3 = 0;
		delay_50us();
	}
	else if (number == 4) {
		P2_0 = 0;
		P2_1 = 0;
		P2_2 = 1;
		P2_3 = 0;
		delay_50us();	
	}
	else if (number == 3) {
		P2_0 = 1;
		P2_1 = 1;
		P2_2 = 0;
		P2_3 = 0;
		delay_50us();
	}
	else if (number == 2) {
		P2_0 = 0;
		P2_1 = 1;
		P2_2 = 0;
		P2_3 = 0;
		delay_50us();	
	}
	else if (number == 1) {
		P2_0 = 1;
		P2_1 = 0;
		P2_2 = 0;
		P2_3 = 0;
		delay_50us();
	}
	else if (number == 0) {
		P2_0 = 0;
		P2_1 = 0;
		P2_2 = 0;
		P2_3 = 0;
		delay_50us();
	}
	delay_ms(5);                    // Com delays menores ocorre falhas na disposição do display.
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
	Linha1 = 1;						// Ao setar uma linha, esta fica inativa para entradas.
	Linha2 = 1;
	Linha3 = 1;

	if (Coluna0 == 0) {				// Tecla 1.
		set_display(1);					
		while(Coluna0 == 0) {       // Debouncing do botão. Garante que não será acionado mais de uma vez antes de ser solto.
			BUZZER = 1;
			print_display();
		}		
		BUZZER = 0;
	}
	if (Coluna1 == 0) {				// Tecla 2.
		set_display(2);
		while(Coluna1 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	if (Coluna2 == 0) {				// Tecla 3.
		set_display(3);
		while(Coluna2 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	
	if (Coluna3 == 0) {				// Brigadeiro
		brigadeiro();
		while(Coluna3 == 0) {
			print_display();
		}

	}
	
	delay_50us();					// Delay de 50 us.
}

void check_line_1(void) {
	Linha0 = 1;
	Linha1 = 0;						// Ativa a linha 1
	Linha2 = 1;
	Linha3 = 1;

	if (Coluna0 == 0) {				// Tecla 4.
		set_display(4);
		while(Coluna0 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	if (Coluna1 == 0) {				// Tecla 5.
		set_display(5);
		while(Coluna1 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	if (Coluna2 == 0) {				// Tecla 6.
		set_display(6);
		while(Coluna2 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	if (Coluna3 == 0) {				// Pipoca
		pipoca();
		while(Coluna3 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	delay_50us();					// Delay de 5 us.
}

void check_line_2(void) {
	Linha0 = 1;
	Linha1 = 1;
	Linha2 = 0;						// Ativa a linha 2
	Linha3 = 1;

	if (Coluna0 == 0) {				// Tecla 7.
		set_display(7);
		while(Coluna0 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	if (Coluna1 == 0) {				// Tecla 8.
		set_display(8);
		while(Coluna1 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	if (Coluna2 == 0) {				// Tecla 9.
		set_display(9);
		while(Coluna2 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	if (Coluna3 == 0) {				// Descongelar carne.
		desc_carne();
		while(Coluna3 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	delay_50us();					// Delay de 5 us.
}

void check_line_3(void) {
	Linha0 = 1;
	Linha1 = 1;
	Linha2 = 1;
	Linha3 = 0;						// Ativa a linha 3
	if (Coluna0 == 0) {				// Tecla * corresponde ao acionamento do forno. 
		BUZZER = 1;
		timer_dec(); 				
		while(Coluna0 == 0) {
			print_display();
		}
		
	}
	if (Coluna1 == 0) {				// Tecla 0.
		set_display(0);
		while(Coluna1 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	if (Coluna2 == 0) {				// Tecla # corresponde ao ajuste de potência.
		set_power();
		while(Coluna2 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	if (Coluna3 == 0) {				// Descongelar feijão.
		desc_feijao();
		while(Coluna3 == 0) {
			BUZZER = 1;
			print_display();
		}
		BUZZER = 0;
	}
	delay_50us();					// Delay de 50 us.
}



// ------------- Funções especiais ------------- //
void brigadeiro (void) { 	// Potência 5 e 7 minutos de duração
	BUZZER = 1;
	delay_ms(333);
	BUZZER = 0;
	Power = '4';
	Number1 = 1;
	Number2 = 0;
	Number3 = 7;
	Number4 = 0;
	NumberOfDigits = 3;
	write_msg("Brigadeiro");
	timer_dec();
}

void pipoca (void) {		// Potência 7 e 3 minutos de duração
	BUZZER = 1;
	delay_ms(333);
	BUZZER = 0;
	Power = '6';
	Number1 = 0;
	Number2 = 0;
	Number3 = 3;
	Number4 = 0;
	NumberOfDigits = 3;
	write_msg("Pipoca");
	timer_dec();
}

void desc_carne (void) {	// Potência 3 e 10 minutos de duração
	BUZZER = 1;
	delay_ms(333);
	BUZZER = 0;
	Power = '2';
	Number1 = 0;
	Number2 = 0;
	Number3 = 0;
	Number4 = 1;
	NumberOfDigits = 3;
	write_msg("Desc. Carne");
	timer_dec();
}
void desc_feijao (void) {	// Potência 3 e 5 minutos de duração
	BUZZER = 1;
	delay_ms(333);
	BUZZER = 0;
	Power = '2';
	Number1 = 0;
	Number2 = 0;
	Number3 = 5;
	Number4 = 0;
	NumberOfDigits = 3;
	write_msg("Desc. Feijao");
	timer_dec();
}



// ------------- Interrupções / Parar/Continuar ------------- //
void ISR_open_door(void) interrupt 0 { 	// Utiliza interrupção externa 0 (maior prioridade)
	MOTOR = 0;
	LED = 0;							// Acende a luz.
	write_msg("PORTA ABERTA");			// Mensagem no LCD.
	while (P3_2 == 0){					// Segura a execução.
		delay_ms_print(33);				// Enquanto a porta está aberta, continua impriminto o tempo mas piscando.
		delay_ms(777);                  // Delay para que o display pisque.
	}
	IE0 = 0;							// Limpa a flag de detecção de interrupção externa 0.
	P3_2 = 1;							// Para manter o nível alto no pino de interrupção.
	LED = 1;							// Apaga a luz.
	clear_lcd();						// Limpa LCD.
	timer_dec();						// Volta a contar o tempo.
	
}

void ISR_30_sec(void) interrupt 2 {		// Utiliza interrupção externa 1
	write_msg("+ 30 s");
	
	if((Number2 + 3) < 6){
		Number2 = Number2 + 3;
	}
	else {
		Number3++;
		Number2 = (Number2 + 3 - 6);
	}
	
	while (P3_3) {						// Segura a execução até soltar o botão
		print_display();	
	}
	IE1 = 0;
	timer_dec();
}

void time_out(void) {
	char i; 				// Quando estoura o timer: beepa o buzzer 5 vezes e volta pro início. (keypad input)
	for (i = 0; i < 5; i++) {
		BUZZER = 1;
		write_msg("TIME OUT");
		delay_ms_print(17); // Não pisca todo display 7 seg. Talvez implementar alguma função para dar clear ou add o caso em display_1 ('vazio') para que deixe o display em branco
		clear_lcd();
		BUZZER = 0;
		delay_ms(444);
	}
	
	NumberOfDigits = 0;		// Restaura o contador de dígitos.
	Number1 = 0;			// Restaura os números que contam o tempo.
	Number2 = 0;
	Number3 = 0;
	Number4 = 0;
	Power = '9';			// Restaura a potência original para o próximo uso.
	keyboard_input();		// Volta a esperar entrada do usuário.
}

// ------------- Controle de Potência ------------- //
void set_power(void) {               // Potncia começa em 10 (9 mas o microondas não opera em potência 0) e a cada clique do botão diminui em 1 unidade.
	if (Power == '0') {
		Power = '9';
		delay_50us();
		write_msg("Potencia: ");    // Para mostrar potência máxima é escrito um char de cada vez.
		LCD = '1';
		wr_char();
		LCD = '0';
		wr_char();
		return;
	}
	Power--;
	delay_ms(1);
	write_msg("Potencia: ");        // Para as demais potências basta utilizar a função write_msg combinada com uma escrita no LCD.
	delay_50us();
	LCD = (Power + 1);              // Power + 1 evita o número 0.
	wr_char();
	delay_50us();
}

// ------------- Temporizador decrescente ------------- //
void timer_dec(void) {
	MOTOR = 1;							// Liga o motor.
	BUZZER = 0;							// Garante que o buzzer estará desativado.
	LED = 0;							// Acende a luz interna quando está.
	while (!((Number1 == 0) && (Number2 == 0) && (Number3 == 0) && (Number4 == 0))) {
		if (Number2 != 0 && Number1 == 0) {									// Momento para decrementar Number2 (Dezenas de segundos)
			delay_ms_print(43);
			Number2--;
			Number1 = 9;
			
		}		
		if (Number4 != 0 && Number3 == 0 && Number2 == 0 && Number1 == 0) { // Momento para decrementar Number4 (Dezenas de minutos)
			delay_ms_print(43);						
			Number4--;					
			Number3 = 9;				
			Number2 = 5;			
			Number1 = 9;
		}
		if (Number3 != 0 && Number2 == 0 && Number1 == 0) {					// Momento para decrementar Number3 (Unidades de minutos)
			delay_ms_print(43);
			Number3--;
			Number2 = 5;
			Number1 = 9;
		}
		delay_ms_print(43);													 // Momento para decrementar Number1 (Unidades de segundos)
		Number1--;
	}
	MOTOR = 0;
	write_msg("Finalizado");
	time_out();
}

void delay_ms_print(unsigned int ms) {	// Realiza um delay enquanto printa o Display 7 SEG
	char i;
	for (i = 0; i < ms; i++) {
		print_display();
		delay_ms(1);
	}
}
