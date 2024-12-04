#include <stdio.h>
#include <string.h>
#include <windows.h>

#define MAX_KEY_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define PASSWORD_DELIMITER ':'
#define MAX_PASSWORDS 200
#define PASSWORD_FILE "passwords.txt"

int isAddNewCommand(const char *str);
int isListCommand(const char *str);
int isHelpCommand(const char *str);
int isFindCommand(const char *str);
struct Password parsePassword(const char *input);
void printWelcomeMessage();
void printCommands();
void loadPasswords(struct Password *pwds, int *pos);
void savePasswords(const struct Password *pwds, int pos);
void copyToClipboard(const char *str);
void getExecutableDirectory(char *buffer, size_t bufferSize);

struct Password
{
    char key[MAX_KEY_LENGTH];
    char pwd[MAX_PASSWORD_LENGTH];
};


int main(int argc, char const *argv[])
{
    if (argc < 2) 
    {
        printWelcomeMessage();
        return 0;
    }

    struct Password pwds[MAX_PASSWORDS];
    int pos = 0;

    loadPasswords(pwds, &pos);

    char activeCommand[10];
    strcpy(activeCommand, argv[1]);

    if (isAddNewCommand(activeCommand))
    {
        if (argc < 3) {return 0;}
        struct Password pwd = parsePassword(argv[2]);
        if (pos < MAX_PASSWORDS)
        {
            // check for duplicate key
            for (int i = 0; i < pos; i++)
            {
                if (strcmp(pwd.key, pwds[i].key) == 0)
                {
                    printf("Duplicate Key");
                    return EXIT_FAILURE;
                }
            }
            
            pwds[pos++] = pwd;
            savePasswords(pwds, pos);
            printf("Password saved.");
        }
        else
        {
            printf("Password storage is full.\n");
        }
    }

    if (isListCommand(activeCommand))
    {
        for (int i = 0; i < pos; i++)
        {
            printf("%s\n",pwds[i].key);
        }
    }

    if (isHelpCommand(activeCommand))
    {
        printCommands();
    }

    if (isFindCommand(activeCommand))
    {
        if (argc < 3) {return 0;}

        int found = 0;        
        for (int i = 0; i < pos; i++)
        {
            if (strcmp(argv[2], pwds[i].key) == 0)
            {
                printf("Found saved password for %s\n", pwds[i].key);
                copyToClipboard(pwds[i].pwd);
                printf("Password copied to clipboard.");
                found = 1;
                break;
            }
        }
        if (found == 0)
        {
            printf("No password found for %s\n", argv[2]);
        }
    }

    return 0;
}

struct Password parsePassword(const char *input)
{
    struct Password pwd = {"", ""};

    int delimiterFound = 0, keyIndex = 0, pwdIndex = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == ':')
        {
            delimiterFound = 1;
            continue;
        }

        if (!delimiterFound)
        {
            if (keyIndex < sizeof(pwd.key) - 1) 
            {
                pwd.key[keyIndex++] = input[i];
            }
        }
        else
        {
            if (pwdIndex < sizeof(pwd.pwd) - 1) 
            {
                pwd.pwd[pwdIndex++] = input[i];
            }
        }
    }    

    pwd.key[keyIndex] = '\0';
    pwd.pwd[pwdIndex] = '\0';
    
    return pwd;
}

int isAddNewCommand(const char *str)
{
    return strcmp(str, "new") == 0;
}

int isListCommand(const char *str)
{
    return strcmp(str, "list") == 0;
}

int isHelpCommand(const char *str)
{
    return strcmp(str, "help") == 0;
}

int isFindCommand(const char *str)
{
    return strcmp(str, "find") == 0;
}

void printWelcomeMessage()
{
    printf("Welcome to Password Manager.\n");
    printf("Type help to see list of available commands\n");
}

void printCommands()
{
    printf("\nAvailable Commands:\n");
    printf("-------------------\n");
    printf("1. Add a New Password:\n");
    printf("   Command: pwdm new <key>:<password>\n");
    printf("   Example: pwdm new email:mysecurepassword123\n\n");

    printf("2. List All Saved Passwords:\n");
    printf("   Command: pwdm list\n");

    printf("3. Find a Password:\n");
    printf("   Command: pwdm find <key>\n");
    printf("   Example: pwdm find email\n");
    printf("   Description: Searches for the password associated with the specified key and saves it in clipboard.\n\n");

    printf("4. Help:\n");
    printf("   Command: pwdm help\n");
    printf("   Description: Displays this list of available commands.\n\n");

    printf("5. Exit:\n");
    printf("   Command: Press Ctrl + D (or equivalent EOF signal) to exit the program.\n\n");

    printf("Note:\n");
    printf(" - Keys and passwords are case-sensitive.\n");
    printf(" - Passwords are limited to %d characters.\n", MAX_PASSWORD_LENGTH);
    printf(" - A maximum of %d passwords can be saved at once.\n\n", MAX_PASSWORDS);
}

void loadPasswords(struct Password *pwds, int *pos)
{
    char exeDirectory[MAX_PATH];
    getExecutableDirectory(exeDirectory, sizeof(exeDirectory));
    char filePath[MAX_PATH];
    snprintf(filePath, sizeof(filePath), "%s\\passwords.txt", exeDirectory);

    FILE *file = fopen(filePath, "a+");

    if (file == NULL)
    {
        return;
    }

    char line[MAX_PASSWORD_LENGTH + MAX_KEY_LENGTH];
    while(fgets(line, sizeof(line), file) != NULL)
    {
        if(*pos >= MAX_PASSWORDS)
        {
            break;
        }

        line[strcspn(line, "\n")] = '\0';
        struct Password pwd = parsePassword(line);
        pwds[(*pos)++] = pwd;
    }
    fclose(file);
}

void savePasswords(const struct Password *pwds, int pos)
{
    char exeDirectory[MAX_PATH];
    getExecutableDirectory(exeDirectory, sizeof(exeDirectory));
    char filePath[MAX_PATH];
    snprintf(filePath, sizeof(filePath), "%s\\passwords.txt", exeDirectory);

    FILE *file = fopen(filePath, "w");
    if (file == NULL)
    {
        printf("Error opening file to save passwords.\n");
        return;
    }

    for (int i = 0; i < pos; i++)
    {
        fprintf(file, "%s:%s\n", pwds[i].key, pwds[i].pwd);
    }

    fclose(file);
}

void copyToClipboard(const char *str)
{
    // open the clipboard
    if(!OpenClipboard(NULL))
    {
        fprintf(stderr, "Failed to open clipboard\n");
        return;
    }

    EmptyClipboard();

    // Allocate global memory for the string
    size_t length = strlen(str) + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, length);
    
    if(!hMem)
    {
        CloseClipboard();
        fprintf(stderr, "Failed to allocate global memory.\n");
        return;
    }

    // Copy the string to the allocated memory
    memcpy(GlobalLock(hMem), str, length);
    GlobalUnlock(hMem);

    // Set the clipboard data
    SetClipboardData(CF_TEXT, hMem);

    CloseClipboard();
}

void getExecutableDirectory(char *buffer, size_t bufferSize)
{
    // Get the full path of the executable
    GetModuleFileName(NULL, buffer, bufferSize);

    // Find the last backslash ('\') to isolate the directory
    for (int i = strlen(buffer) - 1; i >= 0; i--)
    {
        if (buffer[i] == '\\')
        {
            buffer[i] = '\0'; // Terminate the string at the last backslash
            break;
        }
    }
}
