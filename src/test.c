#include "console.h"
#include "user_funcs.h"

#include "hardware/irq.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define BUF_SIZE 255

#define NUMBER_OF_STRING 10
#define MAX_STRING_SIZE 25

void setup_uart();
int parse_string(char *str, char tokens[][MAX_STRING_SIZE], char *delim);

static int chars_rxed = 0;

usr_funcs user_functions[MAX_USER_FUNCTIONS];

int main() {
	int len;
	int exit_flag = 0;
	int token_cnt = 0;
	char tokens[NUMBER_OF_STRING][MAX_STRING_SIZE];
	char buffer[BUF_SIZE], prev_buffer[BUF_SIZE];
	char str[80];

	stdio_init_all();

	// initialize the user functions
	init_user_functions();

	while (true) {
		// console prompt
		console_putc('\n');
		console_putc('$');

		// get command from the console
		len = console_gets(buffer, 128);

		// dealing with up arrow - previous buffer
		if (len == -1) { 
			sprintf(str, "$%s", prev_buffer);
			console_puts(str);
			strcpy(buffer, prev_buffer);
			
		} //end if

		strcpy(prev_buffer, buffer);
	   
		// split the command into tokens
		token_cnt = parse_string(buffer, tokens, " ");
		
		//Look for user defined functions and their variables
		if (token_cnt > 0) {
			//Check each function vs. what was entered
			for (int i = 0; i < MAX_USER_FUNCTIONS; i++) {
				
				//If no entry, break from loop
				if (strcmp(user_functions[i].command_name, "") == 0) { break; }

				//Display help for each specific command
				if (strcmp(user_functions[i].command_name, tokens[0]) == 0) {
					if ((strcmp(tokens[1], "-h") == 0) || (strcmp(tokens[1], "--help") == 0)) {
						sprintf(str, "\n%s\n", user_functions[i].command_help);
						console_puts(str);
						break;
					} 
				} 

				//List the user-defined functions
				if (strcmp("list", tokens[0]) == 0) {
					sprintf(str, "\n%s\n", user_functions[i].command_name);
					console_puts(str);
				}

				//Quit the console loop
				if (strcmp("quit", tokens[0]) == 0) {
					console_puts("\nGood bye.\n");
					exit_flag = 1;
					break;
				}

				//Execute the command function
				if (strcmp(user_functions[i].command_name, tokens[0]) == 0) {
					user_functions[i].user_function(tokens);
					break;
				}
			}
			
			console_puts("\n");
			console_puts("OK");
		}

		buffer[0] = '\0';
		for (int i = 0; i < token_cnt; i++)
			tokens[i][0] = '\0';

		if (exit_flag == 1)
			break;
	} 
} 

int parse_string(char *str, char tokens[][MAX_STRING_SIZE], char *delim) {
	char *buffer = str;
	int x = 0;

	char *token = strtok(buffer, delim);
	while (token != NULL) {
		strcpy(tokens[x], token); // getting each token
		token = strtok(NULL, " ");
		x++;
	}
	return x; // return number of tokens
}

//Setup the UART port
void setup_uart() {
	// Set up our UART with a basic baud rate.
	uart_init(UART_ID, 115200);

	// Set the TX and RX pins by using the function select on the GPIO
	// Set datasheet for more information on function select
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
	gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

	// Actually, we want a different speed
	// The call will return the actual baud rate selected, which will be as close
	// as possible to that requested
	int actual = uart_set_baudrate(UART_ID, BAUD_RATE);

	// Set UART flow control CTS/RTS, we don't want these, so turn them off
	uart_set_hw_flow(UART_ID, false, false);

	// Set our data format
	uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

	// Turn off FIFO's - we want to do this character by character
	uart_set_fifo_enabled(UART_ID, false);
	uart_set_translate_crlf(UART_ID, true);

}
