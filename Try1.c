/*

 17. The application provides a variety of movies (title, description, duration, etc.). 
 The user can search and select movies they have watched. Additionally, besides the "Watched List," 
 the user can add movies to a "To Watch" list. The application allows searching for movies, 
 adding them to the desired list, and deleting them from a list.

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#define MAX_MOVIES 100
#define MOVIE "movies.txt"

// For the clear screen part
#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif
void clearScreen() {
    system(CLEAR);
};

// The colors
#define COLOR_RESET   "\x1b[0m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_RED     "\x1b[31m"


// Movie struct definition
typedef struct Movie {
    char title[100];
    char description[255];
    char seen[4];
    int duration;  // duration in minutes
    
} Movie;

void header() {
    printf("--------------\n");
    printf("1 - Add\n");
    printf("2 - Display\n");
    printf("3 - Delete\n");
    printf("4 - Search\n");
    printf("5 - See Movie Status\n");
    printf("0 - Exit\n");
    printf("--------------\n");
}

// Add a movie to the file - 
void add() {
    FILE *file = fopen(MOVIE, "a");  // Open file in append mode
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Movie movie;

    printf("Enter movie title: ");
    getchar();  // Clear the newline left by previous input
    fgets(movie.title, sizeof(movie.title), stdin);
    movie.title[strcspn(movie.title, "\n")] = '\0';  // Remove the newline character

    printf("Enter movie description: ");
    fgets(movie.description, sizeof(movie.description), stdin);
    movie.description[strcspn(movie.description, "\n")] = '\0';  // Remove the newline character

    int ok=1;
    printf("Enter Yes/No if you've watched the movie or not: ");
    do
    {
        
        fgets(movie.seen, sizeof(movie.seen), stdin);
        movie.seen[strcspn(movie.seen, "\n")] = '\0';
        
        
        if(strcmp(movie.seen,"Yes") == 0 || strcmp(movie.seen,"No")== 0)
        {
            ok=0;
            break;
        }
            
        printf("We need a Yes or a No\n");
                
        
    }while(ok==1);
    printf("Enter movie duration (in minutes): ");
    
    scanf("%d", &movie.duration);

    // Write movie to file
    fprintf(file, "%s\n", movie.title);
    fprintf(file, "%s\n", movie.description);
    fprintf(file, "%s\n", movie.seen);
    fprintf(file, "%d\n", movie.duration);
    
    fclose(file);

    printf("Movie added to the file.\n");
}

// Display movies from the file - works
int display() {
    FILE *file = fopen(MOVIE, "r");  // Open file in read mode
    if (file == NULL) 
    {
        perror("Error opening file!\n");
        return 1;
    }

    Movie movie;
    printf("\n--- Movies ---\n");

    for(;;) 
    {  // infinite loop until there is nothing left to read
        if (fgets(movie.title, sizeof(movie.title), file) == NULL) break;  // Citeste titlul
        if (fgets(movie.description, sizeof(movie.description), file) == NULL) break;  // Citeste descrierea
        if (fgets(movie.seen, sizeof(movie.seen), file) == NULL) break;
        if (fscanf(file, "%d\n", &movie.duration) != 1) break;  // Citeste durata și consumă '\n'
        
        
        // showing the data
        printf("Title: %s%s%s\n", COLOR_GREEN, movie.title, COLOR_RESET);
        printf("Description: %s\n", movie.description);

        if(strcmp(movie.seen,"Yes") == 0) 
        {
            printf("Status: %sseen%s\n", COLOR_BLUE, COLOR_RESET);
        } 
        else 
        {
            printf("Status: %snot seen%s\n", COLOR_RED, COLOR_RESET);
        }

        printf("Duration: %s%d minutes%s\n", COLOR_YELLOW, movie.duration, COLOR_RESET);
        printf("----------------------\n");
    }
    fclose(file);
}

//Search if you added a certain movie or not and shows it - works
void searching()
{
    FILE *file = fopen(MOVIE, "r");
    if(file == NULL)
    {
        perror("Error opening the file!");
        return;
    }
    char x[100];
    printf("Enter movie title to search: ");
    getchar();
    fgets(x, sizeof(x), stdin);
    x[strcspn(x, "\n")] = '\0';  // Elimina '\n' din input

    Movie movie;
    int found=0;
    while(1)
    {  // infinite loop until there is nothing left to read
        if (fgets(movie.title, sizeof(movie.title), file) == NULL) break;  // Citeste titlul
        movie.title[strcspn(movie.title, "\n")] = '\0';
        if (fgets(movie.description, sizeof(movie.description), file) == NULL) break;  // Citeste descrierea
        movie.description[strcspn(movie.description, "\n")] = '\0'; 
        if (fgets(movie.seen, sizeof(movie.seen), file) == NULL) break;
        movie.seen[strcspn(movie.seen, "\n")] = '\0';
        if (fscanf(file, "%d\n", &movie.duration) != 1) break;  // Citeste durata si consumă '\n'
        
        
        if(strncmp(movie.title,x,strlen(x)) == 0)
        {
            printf("\nWe have found the film in the list!\n");
            printf("Title: %s%s%s\n", COLOR_GREEN, movie.title, COLOR_RESET);
            printf("Description: %s\n", movie.description);
            printf("Duration: %s%d minutes%s\n", COLOR_YELLOW, movie.duration, COLOR_RESET);
            printf("Seen: %s%s%s\n", 
                strcmp(movie.seen, "Yes") == 0 ? COLOR_BLUE : COLOR_RED,
                movie.seen,
                COLOR_RESET);

            found = 1;
            break;
        }
       
    }
    if (found!=1) 
    {
        printf("\nNot found in the list. Add it now!\n");
    }
    fclose(file);
}


// Delete a movie from the file - works
void delete() {
    FILE *file = fopen(MOVIE, "r");  // Deschidem fisierul pentru citire
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    char title[100];
    getchar();
    printf("Enter movie title to delete: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;  // Eliminăm newline-ul la finalul titlului

    FILE *tempFile = fopen("temp.txt", "w");  // Deschidem fisierul temporar pentru scriere
    if (!tempFile) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    Movie movie;
    int found = 0;

    // Citim din fisierul original și procesăm fiecare film
    while (fgets(movie.title, sizeof(movie.title), file)) {
        movie.title[strcspn(movie.title, "\n")] = 0;  // Eliminăm '\n' de la sfârșitul titlului
        fgets(movie.description, sizeof(movie.description), file);
        movie.description[strcspn(movie.description, "\n")] = 0;  // Eliminăm '\n' de la sfârșitul descrierii
        fgets(movie.seen, sizeof(movie.seen), file);
        movie.seen[strcspn(movie.seen, "\n")] = '\0';  // Eliminăm '\n' de la sfârșitul statusului
        if (fscanf(file, "%d\n", &movie.duration) != 1) break;  // Citește durata și consumă '\n'
        
        // Căutăm filmul să-l ștergem
        if (strncmp(movie.title, title, strlen(title)) != 0) {
            // Dacă titlul nu se potrivește cu cel introdus, îl copiem în fișierul temporar
            fprintf(tempFile, "%s\n", movie.title);
            fprintf(tempFile, "%s\n", movie.description);
            fprintf(tempFile, "%s\n", movie.seen);
            fprintf(tempFile, "%d\n", movie.duration);
        } else {
            found = 1;  // Dacă găsim filmul, nu-l copiem și marcăm că l-am găsit
        }
    }

    fclose(file);
    fclose(tempFile);

    // Dacă am găsit filmul, înlocuim fișierul original cu cel temporar
    if (found) {
        remove(MOVIE);  // Ștergem fișierul original
        rename("temp.txt", MOVIE);  // Redenumim fișierul temporar pentru a deveni fișierul original
        printf("Movie deleted from the file.\n");
    } else {
        remove("temp.txt");  // Dacă nu am găsit filmul, ștergem fișierul temporar
        printf("Movie not found.\n");
    }
}

void display_seen_status()
{
    int ok=1;
    do{
        int choice;
        printf("--------------\n");
        printf("1 - Seen Movies\n");
        printf("2 - Not Seen Movies\n");
        printf("0 - Exit\n");
        printf("--------------\n");
        scanf("%d", &choice);
    
        if(choice == 0 && choice!=1 && choice!=2)
        {
            ok=0;
            break;
        }
        while(choice>2)
        {
            printf("Enter a valid number!\n");
            scanf("%d", &choice);
        }
    
    
        FILE *file = fopen(MOVIE, "r");
        if (file == NULL) 
        {
            perror("Error opening file!\n");
            return;
        }

        Movie movie;
        
        printf("\n--- Movies ---\n");
        while (fgets(movie.title, sizeof(movie.title), file)) 
        {
            movie.title[strcspn(movie.title, "\n")] = '\0';
            fgets(movie.description, sizeof(movie.description), file);
            movie.description[strcspn(movie.description, "\n")] = '\0';
            fgets(movie.seen, sizeof(movie.seen), file);
            movie.seen[strcspn(movie.seen, "\n")] = '\0';
            fscanf(file, "%d\n", &movie.duration);

            if ((choice == 1 && strcmp(movie.seen, "Yes") == 0) || (choice == 2 && strcmp(movie.seen, "No") == 0)) 
            {
                printf("Title: %s%s%s\n", COLOR_GREEN, movie.title, COLOR_RESET);
                printf("Description: %s\n", movie.description);
                printf("Duration: %s%d minutes%s\n", COLOR_YELLOW, movie.duration, COLOR_RESET);
                printf("----------------------\n");
            }

        }
        // sta 5 secunde
        sleep(5);
        fclose(file);
        clearScreen();
    }while(ok==1);
    clearScreen();
    return;
}



//The menu
void menu(int op) 
{
    switch (op) {
        case 1:
            clearScreen();
            add();
            break;
        case 2:
            clearScreen();
            display();
            break;
        case 3:
            clearScreen();
            delete();
            break;
        case 4:
            clearScreen();
            searching();
            break;
        case 5:
            clearScreen();
            display_seen_status();
            break;
        default:
            printf("EXIT!\n");
    }
}

int main() {
    int option;

    //Create the file if it doesn't exist
    FILE *file = fopen(MOVIE, "a");
    if (file) {
        fclose(file);
    } else {
        printf("Error opening file!\n");
        return 1;
    }

    do {
        header();
        printf("Enter option: ");
        scanf("%d", &option);
        menu(option);
    } while (option > 0 && option <= 5);

    return 0;
}