// shell.c
#include "shell.h"

char *read_line()
{
    char buffer[INPUT_BUFFER_MAX]; // Temporärer Puffer für die Eingabe
    char *line;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        return NULL; // Fehlerfall oder EOF
    }

    // Entfernen des Zeilenumbruchs am Ende der Eingabe
    buffer[strcspn(buffer, "\n")] = 0;

    // Speicher für die zurückzugebende Zeile reservieren
    line = malloc(strlen(buffer) + 1);
    if (line == NULL)
    {
        return NULL; // Fehler bei der Speicherallokation
    }

    // Kopieren der Eingabe in den reservierten Speicher
    strcpy(line, buffer);
    return line;
}

char **split_line(char *line)
{

    int bufsize = MAX_ARGS;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "split_line: Speicherallokationsfehler\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " ");
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "split_line: Speicherallokationsfehler\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " ");
    }
    tokens[position] = NULL;
    return tokens;
}

int execute_args(char **args)
{
    if (args[0] == NULL)
    {
        // wenn keine argumente dann beende
        return -1;
    }

    const char *arg;

    if(args[1] != NULL) {
        arg = args[1];
    }
    if(args[2] != NULL) {
        printf("Warnung beim Argument lesen: Nur ein Argument unterstützt.");
    }


    // execlp ausführen
    execlp(args[0], args[0], arg, (char *)NULL);

    // execlp kehrt nur zurück, wenn ein Fehler auftritt
    perror("execlp");
    return -1;

}

char *getUsername()
{
    char *username = getenv("USER");

    // Versuche, die Umgebungsvariable USER zu bekommen
    if (username == NULL)
    {

        // Falls USER nicht gesetzt ist, versuche es mit LOGNAME
        username = getenv("LOGNAME");
    }
    if (username != NULL)
    {
        return username;
    }
    return NULL;
}

char *getCurrentWorkingDirectory()
{
    static char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("Fehler beim Abrufen des Arbeitsverzeichnisses");
        return NULL;
    }
    return cwd;
}

char *getSystemHostname()
{
    static char hostname[HOST_NAME_MAX];
    if (gethostname(hostname, sizeof(hostname)) == -1)
    {
        printf("Fehler beim Abrufen des Hostnames");
        return NULL;
    }
    return hostname;
}

char *buildPrompt()
{
    char *username = getUsername();
    if (username == NULL)
    {
        printf("Fehler beim Abrufen des Benutzernamens\n");
        return NULL;
    }

    char *hostname = getSystemHostname();
    if (hostname == NULL)
    {
        printf("Fehler beim Abrufen des Hostnamens\n");
        return NULL;
    }

    char *cwd = getCurrentWorkingDirectory();
    if (cwd == NULL)
    {
        printf("Fehler beim Abrufen des Arbeitsverzeichnisses\n");
        return NULL;
    }

    // Berechnen der benötigten Länge für den String
    int length = strlen(username) + strlen(hostname) + strlen(cwd) + 4; // Für "@", " ", "$" und das Nullzeichen
    char *prompt = malloc(length * sizeof(char));
    if (prompt == NULL)
    {
        printf("Speicherzuweisung für den Prompt fehlgeschlagen\n");
        return NULL;
    }

    // Zusammenbauen des Prompts
    sprintf(prompt, "%s@%s: %s$ ", username, hostname, cwd);
    return prompt;
}

void shell()
{
    // int status = 0;
    char *prompt;
    char *line;
    char **args;

    while (1) // dauerschleife, staus wird nicht gehandled
    {

        prompt = buildPrompt();
        printf("%s", prompt);

        line = read_line();
        args = split_line(line);
        // status = execute_args(args);
        execute_args(args);

        free(prompt);
        free(line);
        free(args);
    }
}