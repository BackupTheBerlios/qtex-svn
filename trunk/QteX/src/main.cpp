#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
  int pid, pipes[2];
  
  // Zunächst eine Pipe erzeugen
  if (pipe(pipes) != 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  
  // Dann einen neuen Prozess erzeugen
  pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  
  if (pid == 0) { // Im Child-Prozess
    // Child-Prozess soll senden, daher Pipe-Empfangsseite schließen
    close(pipes[0]);
    
    // Die Standardausgabe (1) mit der Sendeseite der Pipe überschreiben
    if (dup2(pipes[1], 1) == -1) {
      perror("dup2");
      exit(EXIT_FAILURE);
    }
    
    // Das gewünschte Kommando ausführen
    char *argument[3] = { "/bin/date", "--version", NULL };
    char *command = argument[0];
    if (execv(command, argument) == -1) {
      perror("execv");
      exit(EXIT_FAILURE);
    }
  } else { // Im Vater-Prozess
    // Vater-Prozess soll empfangen, daher Pipe-Sendeseite schließen
    close(pipes[1]);
    
    int readsize = 1024;
    char *buffer = new char[readsize + 1];
    
    while (true) { 
        // Die Pipe solange auslesen, wie Daten ankommen
        int ret = read(pipes[0], buffer, readsize); 
        if (ret < 0) { 
            if (errno == EINTR) 
              continue;
            perror("read"); 
            exit(EXIT_FAILURE); 
        }
        
        if (ret == 0) 
          break;
          
        buffer[ret] = 0; 
        printf(buffer); 
      }
  }
  
  return 0;
}
