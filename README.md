# RP2040-pico-command-line-interpreter

This program is a work in progress. It is based on the "rp2040-pico-command-line-interpreter" by Frank Applin. This project assumes that you are using the C/C++ SDK for the Pico.

The goal of this project is to create a working command line interpreter for the RP2040, where the programmer can add specific functions, recompile, and reupload to the pico. The Pico would then run the program on the second core or interact with the PIO, and return data to the console.

An example by Frank Applin, allows for the user to use a RP2040 UART to send commands such as `blink_led` and the Pico will toggle given pin, a set number of times, for a set duration. The code can remain the same but each value can be set, without recompiling, by input through the UART console. 
```
$blink_led 25 10 250
OK

$
```

The programmer defines a routine in a "C" source file (user_funcs.c) and accompanying header file (user_funcs.h).

Continuing the blink_led example:
```
void blink_led(char tokens[][MAX_STRING_SIZE]) {

  uint8_t LED_PIN = atoi(tokens[1]);
  if (LED_PIN == 0) {
    console_puts("\nNot a valid pin number.\n");
    return;
  } // end if

  uint16_t iters = atoi(tokens[2]);
  if (iters == 0) {
    console_puts("\nNot a valid number for iterations.\n");
    return;
  } // end if

  uint16_t millis = atoi(tokens[3]);
  if (millis == 0) {
    console_puts("\nNot a valid number for duration.\n");
    return;
  } // end if

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  for (int i = 0; i < iters; i++) {
    gpio_put(LED_PIN, 1);
    sleep_ms(millis);
    gpio_put(LED_PIN, 0);
    sleep_ms(millis);
  } // end for

} // end blink_led
```
The parameters are passed in as an array of string tokens. The tokens are gathered in the main program, and handled by the console. The command name is tokens[0]. In the blink_led example above, the arguments are the LED pin - tokens[1], the number of times to blink - tokens[2], and the duration of the blink (in milliseconds) - tokens[3].

The command blink_led also has to be added to an intialization routine. The three pieces of information are the command name, command help, and the function name.

Make these modifications in user_funcs.c
```
// Each user function you create, you must also
// add to the init_user_functions routine.
void init_user_functions() {
  // This is my first user-defined function. I give
  // the command a name - in this case "blink_led".
  // You can supply a short help description. And, 
  // then, I point the user_functions struct array
  // to the actual name of my user-defined function.
  strcpy(user_functions[0].command_name, "blink_led");
  strcpy(user_functions[0].command_help,
         "blink_led pin_number iterations duration (ms)");
  user_functions[0].user_function = blink_led;

  // Here is a second user function.
  strcpy(user_functions[1].command_name, "read_pir_sensor");
  strcpy(user_functions[1].command_help,
         "read_pir_sensor pin_number iterations");
  user_functions[1].user_function = read_pir_sensor;

  //Here is a third user function.
  strcpy(user_functions[2].command_name, "calc");
  strcpy(user_functions[2].command_help, "calc math_expression");
  user_functions[2].user_function = calc;
} // init_user_functions
```

Finally, the function needs to be added to the accompanying header file (user_funcs.h).

```
void blink_led(char tokens[][MAX_STRING_SIZE]);
void read_pir_sensor(char tokens[][MAX_STRING_SIZE]);
void calc(char tokens[][MAX_STRING_SIZE]);

```

Run the make command in the build directory and then copy the .uf2 file to the Pico. The command line interpreter should start to run with the prompt "$" waiting for you to type a command to run. There is a list command to list the user-defined functions. 
