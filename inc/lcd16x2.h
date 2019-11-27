/ * *
  **************************************************** ****************************
  * @file lcd16x2.c
  * @ autor Yohanes Erwin Setiawan
  * @date 6 de fevereiro de 2016
	* @note Reescrever o formulário Peter Fleury AVR LCD library
  **************************************************** ****************************
  * /

/ * * Inclui ---------------------------------------------- ------------------ * /
# include  " lcd16x2.h "

/ * * Protótipos de funções privadas -------------------------------------------- - * /
 nulo  estático lcd16x2_toggle_e ( nulo );
estático  void  lcd16x2_write ( uint8_t dados, uint8_t rs);
estático  uint8_t  lcd16x2_read ( uint8_t rs);
estático  uint8_t  lcd16x2_wait_busy ( nulo );
estático  vazio  lcd16x2_new_line ( uint8_t pos);

estático  uint8_t display_cursor_on_off_control;
GPIO_InitTypeDef GPIO_InitStruct;

/ * * Funções públicas --------------------------------------------- ----------- * /
/ * *
  **************************************************** ****************************
  * @brief Inicialize o LCD 16x2 no modo de E / S de 4 bits.
  * @param Exibir, sublinhar o cursor e piscar o cursor. Vejo
  * Os atributos de exibição e cursor do LCD são definidos no arquivo lcd16x2.h.
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_init ( uint8_t disp_attr)
{
	// Atraso na inicialização
	DelayInit ();

	// Relógio GPIO para linhas de controle e dados
	RCC_APB2PeriphClockCmd (LCD16X2_RCC_GPIO_CONTROL, ENABLE);
	RCC_APB2PeriphClockCmd (LCD16X2_RCC_GPIO_DATA, ENABLE);

	// Configurar E / S para linhas de controle como saída
	GPIO_InitStruct. GPIO_Pin = LCD16X2_PIN_RS | LCD16X2_PIN_RW |
		LCD16X2_PIN_EN;
	GPIO_InitStruct. GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct. GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init (LCD16X2_GPIO_CONTROL, & GPIO_InitStruct);

	// Configurar E / S para linhas de dados como saída
	GPIO_InitStruct. GPIO_Pin = LCD16X2_PIN_D4 | LCD16X2_PIN_D5
		LCD16X2_PIN_D6 LCD16X2_PIN_D7;
	GPIO_InitStruct. GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct. GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init (LCD16X2_GPIO_DATA, & GPIO_InitStruct);

	// Atraso na inicialização
	Atrasos (LCD16X2_DELAY_POWER_ON);

	// Inicializa o modo de 8 bits primeiro
	LCD16X2_GPIO_D5-> BSRR = LCD16X2_PIN_D5;	// Conjunto de funções
	LCD16X2_GPIO_D4-> BSRR = LCD16X2_PIN_D4;	// modo de 8 bits
	lcd16x2_toggle_e ();
	// Atraso, o sinalizador de ocupado não pode ser verificado aqui
	Atrasos (LCD16X2_DELAY_INIT);

	// Repita o último comando
	lcd16x2_toggle_e ();
	// Atraso, o sinalizador de ocupado não pode ser verificado aqui
	Atrasos (LCD16X2_DELAY_INIT_REP);

	// Repita o último comando pela terceira vez
	lcd16x2_toggle_e ();
	// Atraso, o sinalizador de ocupado não pode ser verificado aqui
	Atrasos (LCD16X2_DELAY_INIT_REP);

	// Inicializa o modo de 4 bits
	LCD16X2_GPIO_D5-> BSRR = LCD16X2_PIN_D5;	// Conjunto de funções
	LCD16X2_GPIO_D4-> BRR = LCD16X2_PIN_D4;	// modo de 4 bits
	lcd16x2_toggle_e ();
	Atrasos (LCD16X2_DELAY_INIT_4BIT);

	/ * A partir de agora o LCD aceita apenas E / S de 4 bits * /

	// interface de 4 bits, 2 linhas, fonte no formato de ponto 5x7
	lcd16x2_write_command (LCD16X2_FUNCTION_SET | LCD16X2_4BIT_INTERFACE |
		LCD16X2_2LINE_MODE | LCD16X2_5X7DOT_FORMAT);
	// Exibição desativada
	lcd16x2_write_command (LCD16X2_DISPLAY_CURSOR_ON_OFF | LCD16X2_DISPLAY_OFF);
	// Limpar tela
	lcd16x2_clrscr ();
	// modo de entrada
	lcd16x2_entry_inc ();
	// Exibe o cursor ativado
	display_cursor_on_off_control = disp_attr;
	lcd16x2_write_command (LCD16X2_DISPLAY_CURSOR_ON_OFF |
		display_cursor_on_off_control);
}

/ * *
  **************************************************** ****************************
  * @brief Escreva um comando no LCD.
  * @param O conjunto de instruções do LCD.
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_write_command ( uint8_t cmd)
{
	lcd16x2_wait_busy ();
	lcd16x2_write (cmd, 0 );
}

/ * *
  **************************************************** ****************************
  * @brief Escreva um byte de dados no LCD.
  * @param Dados que deseja gravar no LCD.
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_write_data ( dados uint8_t )
{
	lcd16x2_wait_busy ();
	lcd16x2_write (dados, 1 );
}

/ * *
  **************************************************** ****************************
  * @brief Limpe a tela LCD e retorne o cursor para a posição inicial.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_clrscr ()
{
	lcd16x2_write_command (LCD16X2_CLEAR_DISPLAY);
}

/ * *
  **************************************************** ****************************
  * @brief Retorne o cursor para a posição inicial.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_home ()
{
	lcd16x2_write_command (LCD16X2_CURSOR_HOME);
}

/ * *
  **************************************************** ****************************
  * @brief Coloque o cursor do LCD na posição específica.
  * coluna LCD @param (x)
  * @param linha do LCD (y)
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_gotoxy ( uint8_t x, uint8_t y)
{
# if LCD16X2_LINES == 1
	lcd16X2_write_command (LCD16X2_SET_DDRAM_ADDRESS |
		(LCD16X2_START_LINE_1 + x));
# elif LCD16X2_LINES == 2
	se (y == 0 )
		lcd16x2_write_command (LCD16X2_SET_DDRAM_ADDRESS |
			(LCD16X2_START_LINE_1 + x));
	outro
		lcd16x2_write_command (LCD16X2_SET_DDRAM_ADDRESS |
			(LCD16X2_START_LINE_2 + x));
# endif
}

/ * *
  **************************************************** ****************************
  * @brief Obter o cursor do LCD / endereço DDRAM.
  * @param None
  * @valor do cursor LCD / endereço DDRAM.
  **************************************************** ****************************
  * /
uint8_t  lcd16x2_getxy ()
{
	retornar  lcd16x2_wait_busy ();
}

/ * *
  **************************************************** ****************************
  * @brief Define o modo de entrada do LCD: incrementa o cursor.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_entry_inc ()
{
	lcd16x2_write_command (LCD16X2_CHARACTER_ENTRY_MODE | LCD16X2_INCREMENT |
		LCD16X2_DISPLAY_SHIFT_OFF);
}

/ * *
  **************************************************** ****************************
  * @brief Define o modo de entrada do LCD: diminui o cursor.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_entry_dec ()
{
	lcd16x2_write_command (LCD16X2_CHARACTER_ENTRY_MODE | LCD16X2_DECREMENT |
		LCD16X2_DISPLAY_SHIFT_OFF);
}

/ * *
  **************************************************** ****************************
  * @brief Defina o modo de entrada do LCD: incrementa o cursor e desloca o caractere para a esquerda.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_entry_inc_shift ()
{
	lcd16x2_write_command (LCD16X2_CHARACTER_ENTRY_MODE | LCD16X2_INCREMENT |
		LCD16X2_DISPLAY_SHIFT_ON);
}

/ * *
  **************************************************** ****************************
  * @brief Defina o modo de entrada do LCD: diminua o cursor e mude o caractere para a direita.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_entry_dec_shift ()
{
	lcd16x2_write_command (LCD16X2_CHARACTER_ENTRY_MODE | LCD16X2_DECREMENT |
		LCD16X2_DISPLAY_SHIFT_ON);
}

/ * *
  **************************************************** ****************************
  * @brief Ligue a tela (pode ver os caracteres na tela).
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_display_on ()
{
	display_cursor_on_off_control | = LCD16X2_DISPLAY_ON;
	lcd16x2_write_command (LCD16X2_DISPLAY_CURSOR_ON_OFF |
		display_cursor_on_off_control);
}

/ * *
  **************************************************** ****************************
  * @brief Desligue a tela (em branco / não pode ver os caracteres na tela).
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_display_off ()
{
	display_cursor_on_off_control & = ~ LCD16X2_DISPLAY_ON;
	lcd16x2_write_command (LCD16X2_DISPLAY_CURSOR_ON_OFF |
		display_cursor_on_off_control);
}

/ * *
  **************************************************** ****************************
  * @brief Ligue o cursor sublinhado.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_cursor_on ()
{
	display_cursor_on_off_control | = LCD16X2_CURSOR_UNDERLINE_ON;
	lcd16x2_write_command (LCD16X2_DISPLAY_CURSOR_ON_OFF |
		display_cursor_on_off_control);
}

/ * *
  **************************************************** ****************************
  * @brief Desative o cursor sublinhado.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_cursor_off ()
{
	display_cursor_on_off_control & = ~ LCD16X2_CURSOR_UNDERLINE_ON;
	lcd16x2_write_command (LCD16X2_DISPLAY_CURSOR_ON_OFF |
		display_cursor_on_off_control);
}

/ * *
  **************************************************** ****************************
  * @brief Ligue o cursor piscando.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_blink_on ()
{
	display_cursor_on_off_control | = LCD16X2_CURSOR_BLINK_ON;
	lcd16x2_write_command (LCD16X2_DISPLAY_CURSOR_ON_OFF |
		display_cursor_on_off_control);
}

/ * *
  **************************************************** ****************************
  * @brief Desligue o cursor piscando.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_blink_off ()
{
	display_cursor_on_off_control & = ~ LCD16X2_CURSOR_BLINK_ON;
	lcd16x2_write_command (LCD16X2_DISPLAY_CURSOR_ON_OFF |
		display_cursor_on_off_control);
}

/ * *
  **************************************************** ****************************
  * @brief Desloque o visor LCD para a esquerda.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_display_shift_left ()
{
	lcd16x2_write_command (LCD16X2_DISPLAY_CURSOR_SHIFT |
		LCD16X2_DISPLAY_SHIFT | LCD16X2_LEFT_SHIFT);
}

/ * *
  **************************************************** ****************************
  * @brief Desloque o visor LCD para a direita.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_display_shift_right ()
{
	lcd16x2_write_command (LCD16X2_DISPLAY_CURSOR_SHIFT |
		LCD16X2_DISPLAY_SHIFT | LCD16X2_RIGHT_SHIFT);
}

/ * *
  **************************************************** ****************************
  * @brief Desloque o cursor do LCD para a esquerda (endereço DDRAM aumentado).
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_cursor_shift_left ()
{
	lcd16x2_write_command (LCD16X2_DISPLAY_CURSOR_SHIFT |
		LCD16X2_DISPLAY_CURSOR_SHIFT | LCD16X2_LEFT_SHIFT);
}

/ * *
  **************************************************** ****************************
  * @brief Desloque o cursor do LCD para a direita (endereço DDRAM diminuído).
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_cursor_shift_right ()
{
	lcd16x2_write_command (LCD16X2_DISPLAY_CURSOR_SHIFT |
		LCD16X2_DISPLAY_CURSOR_SHIFT | LCD16X2_RIGHT_SHIFT);
}

/ * *
  **************************************************** ****************************
  * @brief Coloque um caracter no visor LCD.
  * @param Caractere que deseja exibir.
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_putc ( const  char c)
{
	uint8_t pos = lcd16x2_getxy ();

	if (c == ' \ n ' )
	{
		lcd16x2_new_line (pos);
	}
	outro
	{
# if LCD16X2_LINES == 1
		if (pos == (LCD16X2_START_LINE_1 + LCD16X2_DISP_LENGTH))
			lcd16x2_write (LCD16X2_SET_DDRAM_ADDRESS |
				LCD16X2_START_LINE_1, 0 );
# elif LCD16X2_LINES == 2
		if (pos == (LCD16X2_START_LINE_1 + LCD16X2_DISP_LENGTH))
			lcd16x2_write (LCD16X2_SET_DDRAM_ADDRESS |
				LCD16X2_START_LINE_2, 0 );
		 caso contrário, se (pos == (LCD16X2_START_LINE_2 + LCD16X2_DISP_LENGTH))
			lcd16x2_write (LCD16X2_SET_DDRAM_ADDRESS |
				LCD16X2_START_LINE_1, 0 );
# endif

		lcd16x2_write_data (c);
	}
}

/ * *
  **************************************************** ****************************
  * @brief Coloque a corda no visor LCD.
  * @param String que deseja ser exibida.
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_puts ( caracteres const  *)
{
	while (* s) {
		lcd16x2_putc (* s ++);
	}
}

/ * *
  **************************************************** ****************************
  * @brief Crie um caractere personalizado no local do CGRAM.
  * localização do CGRAM do @param (0-7).
  * @param Padrão de caracteres personalizados (8 bytes).
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_create_custom_char ( uint8_t local, uint8_t * data_bytes)
{
	int i;

	// Temos apenas 8 locais de 0 a 7 para caracteres personalizados
	localização & = 0x07 ;

	// Definir endereço CGRAM
	lcd16x2_write_command (LCD16X2_SET_CGRAM_ADDRESS | (localização << 3 ));

	// Grava um padrão de caracteres personalizado de 8 bytes
	para (i = 0 ; i < 8 ; i ++)
	{
		lcd16x2_write_data (dados_bytes [i]);
	}
}

/ * *
  **************************************************** ****************************
  * @brief Coloque um caractere personalizado em um local específico da tela LCD.
  * coluna LCD @param
  * @param linha do LCD
  * @param Localização de caracteres personalizados no CGRAM (0-7).
  * @retval Nenhum
  **************************************************** ****************************
  * /
void  lcd16x2_put_custom_char ( uint8_t x, uint8_t y, uint8_t local)
{
	lcd16x2_gotoxi (x, y);
	lcd16x2_write_data (local);
}

/ * * Funções privadas --------------------------------------------- ---------- * /
/ * *
  **************************************************** ****************************
  * @brief Dê o pulso de ativação ao pino EN do LCD.
  * @param None
  * @retval Nenhum
  **************************************************** ****************************
  * /
estático  vazio  lcd16x2_toggle_e ()
{
	// EN pin = HIGH
	LCD16X2_GPIO_EN-> BSRR = LCD16X2_PIN_EN;
	// Comprimento do pulso em nós
	Atrasos (LCD16X2_DELAY_ENABLE_PULSE);
	// EN pin = LOW
	LCD16X2_GPIO_EN-> BRR = LCD16X2_PIN_EN;
}

/ * *
  **************************************************** ****************************
  * @brief Escreva instruções ou dados no LCD.
  * @param Instrução / dados que deseja enviar para o LCD.
  * @param Instrução ou seleção de registro de dados. Se escrever instruções, então
  * RS = 0. Caso contrário, RS = 1.
  * @retval Nenhum
  **************************************************** ****************************
  * /
estática  vazio  lcd16x2_write ( uint8_t dados, uint8_t rs)
{
	// Modo de gravação (RW = 0)
	LCD16X2_GPIO_RS-> BRR = LCD16X2_PIN_RW;

	se (rs)
		// Grava dados (RS = 1)
		LCD16X2_GPIO_RS-> BSRR = LCD16X2_PIN_RS;
	outro
		// Grava instrução (RS = 0)
		LCD16X2_GPIO_RS-> BRR = LCD16X2_PIN_RS;

	// Configure todos os pinos de dados como saída
	GPIO_InitStruct. GPIO_Pin = LCD16X2_PIN_D4 | LCD16X2_PIN_D5
		LCD16X2_PIN_D6 LCD16X2_PIN_D7;
	GPIO_InitStruct. GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct. GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init (LCD16X2_GPIO_DATA, & GPIO_InitStruct);

	// Saída de mordidela alta primeiro
	LCD16X2_GPIO_D7-> BRR = LCD16X2_PIN_D7;
	LCD16X2_GPIO_D6-> BRR = LCD16X2_PIN_D6;
	LCD16X2_GPIO_D5-> BRR = LCD16X2_PIN_D5;
	LCD16X2_GPIO_D4-> BRR = LCD16X2_PIN_D4;
	if (dados e 0x80 ) LCD16X2_GPIO_D7-> BSRR = LCD16X2_PIN_D7;
	if (dados e 0x40 ) LCD16X2_GPIO_D7-> BSRR = LCD16X2_PIN_D6;
	if (dados e 0x20 ) LCD16X2_GPIO_D7-> BSRR = LCD16X2_PIN_D5;
	if (dados & 0x10 ) LCD16X2_GPIO_D7-> BSRR = LCD16X2_PIN_D4;
	lcd16x2_toggle_e ();

	// Mordida baixa de saída
	LCD16X2_GPIO_D7-> BRR = LCD16X2_PIN_D7;
	LCD16X2_GPIO_D6-> BRR = LCD16X2_PIN_D6;
	LCD16X2_GPIO_D5-> BRR = LCD16X2_PIN_D5;
	LCD16X2_GPIO_D4-> BRR = LCD16X2_PIN_D4;
	if (dados e 0x08 ) LCD16X2_GPIO_D7-> BSRR = LCD16X2_PIN_D7;
	if (dados & 0x04 ) LCD16X2_GPIO_D7-> BSRR = LCD16X2_PIN_D6;
	if (dados e 0x02 ) LCD16X2_GPIO_D7-> BSRR = LCD16X2_PIN_D5;
	if (dados e 0x01 ) LCD16X2_GPIO_D7-> BSRR = LCD16X2_PIN_D4;
	lcd16x2_toggle_e ();

	// Todos os pinos de dados estão altos (inativos)
	LCD16X2_GPIO_D7-> BSRR = LCD16X2_PIN_D7;
	LCD16X2_GPIO_D7-> BSRR = LCD16X2_PIN_D6;
	LCD16X2_GPIO_D7-> BSRR = LCD16X2_PIN_D5;
	LCD16X2_GPIO_D7-> BSRR = LCD16X2_PIN_D4;
}

/ * *
  **************************************************** ****************************
  * @brief Leia o endereço DDRAM + sinalizador de ocupado ou dados do LCD.
  * Endereço DDRAM @param + sinalizador de ocupado ou seleção de registro de dados.
  * Se ler endereço DDRAM + sinalizador de ocupado, então RS = 0. Caso contrário, RS = 1.
  * endereço DDRAM @retval + sinal de ocupado ou valor dos dados.
  **************************************************** ****************************
  * /
estático  uint8_t  lcd16x2_read ( uint8_t rs)
{
	uint8_t data = 0 ;

	// Modo de leitura (RW = 1)
	LCD16X2_GPIO_RS-> BSRR = LCD16X2_PIN_RW;

	se (rs)
		// Ler dados (RS = 1)
		LCD16X2_GPIO_RS-> BSRR = LCD16X2_PIN_RS;
	outro
		// Ler sinalizador de ocupado e endereço DDRAM (RS = 0)
		LCD16X2_GPIO_RS-> BRR = LCD16X2_PIN_RS;

	// Configure todos os pinos de dados como entrada
	GPIO_InitStruct. GPIO_Pin = LCD16X2_PIN_D4 | LCD16X2_PIN_D5
		LCD16X2_PIN_D6 LCD16X2_PIN_D7;
	GPIO_InitStruct. GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct. GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init (LCD16X2_GPIO_DATA, & GPIO_InitStruct);

	// EN pin = HIGH
	LCD16X2_GPIO_EN-> BSRR = LCD16X2_PIN_EN;
	// Comprimento do pulso em nós
	Atrasos (LCD16X2_DELAY_ENABLE_PULSE);
	/ * Leia primeiro petiscos altos * /
	se (LCD16X2_GPIO_D4-> IDR e LCD16X2_PIN_D4) dados | = 0x10 ;
	se (LCD16X2_GPIO_D5-> IDR e LCD16X2_PIN_D5) dados | = 0x20 ;
	se (LCD16X2_GPIO_D6-> IDR e LCD16X2_PIN_D6) dados | = 0x40 ;
	se (LCD16X2_GPIO_D7-> IDR e LCD16X2_PIN_D7) dados | = 0x80 ;
	// EN pin = LOW
	LCD16X2_GPIO_EN-> BRR = LCD16X2_PIN_EN;

	// PT pin LOW delay
	Atrasos (LCD16X2_DELAY_ENABLE_PULSE);

	// EN pin = HIGH
	LCD16X2_GPIO_EN-> BSRR = LCD16X2_PIN_EN;
	// Comprimento do pulso em nós
	Atrasos (LCD16X2_DELAY_ENABLE_PULSE);
	/ * Ler pouca mordidela * /
	se (LCD16X2_GPIO_D4-> IDR e LCD16X2_PIN_D4) dados | = 0x01 ;
	if (LCD16X2_GPIO_D5-> IDR & LCD16X2_PIN_D5) data | = 0x02 ;
	if (LCD16X2_GPIO_D6-> IDR & LCD16X2_PIN_D6) data | = 0x04 ;
	se (LCD16X2_GPIO_D7-> IDR e LCD16X2_PIN_D7) dados | = 0x08 ;
	// EN pin = LOW
	LCD16X2_GPIO_EN-> BRR = LCD16X2_PIN_EN;

	retornar dados;
}

/ * *
  **************************************************** ****************************
  * @brief Aguarde o LCD até terminar o trabalho.
  * @param None
  * endereço DDRAM @retval + valor do sinalizador ocupado.
  **************************************************** ****************************
  * /
static  uint8_t  lcd16x2_wait_busy ()
{
	// Aguarde até que o sinalizador de ocupado seja limpo
	while ( lcd16x2_read ( 0 ) & (LCD16X2_BUSY_FLAG));

	// O atraso necessário para o contador de endereços é atualizado depois que o sinalizador de ocupado é limpo
	Atrasos (LCD16X2_DELAY_BUSY_FLAG);

	// Contador de leitura e endereço de retorno
	retornar  lcd16x2_read ( 0 );
}

/ * *
  **************************************************** ****************************
  * @brief Atribua um novo caractere de linha
  * @param Posição atual do cursor / endereço DDRAM.
  * @retval Nenhum
  **************************************************** ****************************
  * /
estático  vazio  lcd16x2_new_line ( uint8_t pos)
{
	uint8_t address_counter;

# if LCD16X2_LINES == 1
	address_counter = 0 ;
# elif LCD16X2_LINES == 2
	if (pos <LCD16X2_START_LINE_2)
		address_counter = LCD16X2_START_LINE_2;
	outro
		address_counter = LCD16X2_START_LINE_1;
# endif

	lcd16x2_write_command (LCD16X2_SET_DDRAM_ADDRESS | address_counter);
}

/ * ******************************** FIM DO ARQUIVO ************* ****************** * /
/ * ************************************************** **************************** * /
