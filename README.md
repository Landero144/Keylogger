# Keylogger
Crear Keylogger para sistema linux
Puntuaciones:
-está en la cuenta de github (+5pts)
-escrito en c (+5pts)
-README en formato correcto con información completa de instrucciones de uso, supuestos del programa y casos de uso. (+20pts)
-escribe en un fichero lo que el usuario va a escribiendo (+25pts)
-toma en cuenta teclas especiales (+25pts)
-loggea tal como lo escribe el usuario (+20pts)

- Instrucciones de uso:
En linux en mi caso kali linux
Ejecuta el programa como superusuario:

sudo ./keylogger

Se abrira una terminal de comandos y se cerrara 
 Ahora el programa comenzará a capturar las pulsaciones de teclas y las registrará en el archivo keylogger.txt en la carpeta /home/kali/Keylogger/.
Carpeta que previamente cree, pero en el codigo puedes editar en donde quieres que se cree el archivo 

- Supuestos del programa
El programa tiene los siguientes supuestos:
* El programa asume que el teclado es un teclado QWERTY en inglés.
* El archivo de log se guarda en donde tu indiques en la edicion del codigo, por default tiene una default /home/kali/.
* Se han añadido mapeos básicos para teclas Shift y algunas teclas especiales (Enter, Backspace, Space, Esc).

- Casos de uso
- Monitoreo de las pulsaciones de teclas para pruebas de seguridad.
- Uso personal para registro de entrada
- Poder ingresar a ciertos logins de diversas paginas
- Registrar conversaciones
- Comprobar si alguien uso tu equipo
Se sabe que el uso de este tipo de softwares puede ser malintencionado asi que hay que tener cuidado ya que en algunos lugares
es hasta Ilegal.
