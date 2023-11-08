#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/multicore.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SW_0_PIN 9
#define TX_PIN 4
#define RX_PIN 5
#define UART_ID uart1
#define BAUD_RATE 9600
#define TIMEOUT_MS 500
#define STRLEN 128
#define BUFFER_SIZE 128

char circular_buffer[BUFFER_SIZE];
volatile int buffer_head = 0;
volatile int buffer_tail = 0;

void uart_rx_handler() {
    while (uart_is_readable(UART_ID)) {
        char received_char = uart_getc(UART_ID);
        int next_head = (buffer_head + 1) % BUFFER_SIZE;

        if (next_head != buffer_tail) {
            circular_buffer[buffer_head] = received_char;
            buffer_head = next_head;
        } else {
            // Buffer overflow, handle or discard the data
        }
    }
}

void uart_receiver_thread() {
    while (true) {
        // Check if there is data in the circular buffer
        while (buffer_tail != buffer_head) {
            char data = circular_buffer[buffer_tail];
            buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;

            // Process the received data
            printf("Received: %c\n", data);
        }
        sleep_ms(10); // Adjust this value based on your application's requirements
    }
}

int main() {
    stdio_init_all();

    gpio_init(SW_0_PIN);
    gpio_set_dir(SW_0_PIN, GPIO_IN);
    gpio_pull_up(SW_0_PIN);

    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);

    // Set up the UART receive interrupt
    uart_set_irq_enables(UART_ID, true, false);
    irq_set_exclusive_handler(UART1_IRQ, uart_rx_handler);
    irq_set_enabled(UART1_IRQ, true);

    multicore_launch_core1(uart_receiver_thread);

    int state = 1;

    printf("LoRa module test\n");
    sleep_ms(1000);

    while (true) {
        if (state == 1) {
            printf("Press SW_0 to start communication with the LoRa module...\n");
            while (gpio_get(SW_0_PIN)) {
                sleep_ms(10);
            }
            state = 2;
        } else if (state == 2) {
            printf("Connecting to LoRa module...\n");
            send_command("AT\r\n");
            if (read_response(5) == true) {
                printf("Connected to LoRa module\n");
                state = 3;
            } else {
                printf("Module not responding\n");
                state = 1;
            }
        } else if (state == 3) {
            printf("Reading firmware ver...\n");
            send_command("AT+VER\r\n");
            if (read_response(5) == true) {
                printf("\n");
            } else {
                printf("Module not responding\n");
            }
            state = 1;
        }
    }
    return 0;
}
