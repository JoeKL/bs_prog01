#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
    static char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("Fehler beim Abrufen des Arbeitsverzeichnisses");
        return NULL;
    }
    return cwd;
}

char *getSystemHostname()
{
    static char hostname[256];
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

int main()
{

    while (1)
    {
        char *prompt = buildPrompt();

        if (prompt != NULL)
        {
            printf("%s", prompt);
            free(prompt); // Speicher freigeben, nachdem wir fertig sind
        }

        char input[256];

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Fehler bei der Eingabe.\n");
        }
        else
        {
            int numArgs;
            char command[128];
            char argument[128];

            numArgs = sscanf(input, "%s -%s", command, argument);

            if (numArgs == 1)
            {
                printf("Befehl: %s\n", command);
            }
            else if (numArgs == 2)
            {
                printf("Befehl: %s\n", command);
                printf("Argument: -%s\n", argument);
            }
            else
            {
                printf("Fehler bei der Eingabe: Befehl -Argument");
            }
        }
    }

    return 0;
}