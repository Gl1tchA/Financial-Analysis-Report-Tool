#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

//Function Prototypes
void main_menu();
void pick_year();

void title_function(char* text);
void exit_animation(char *phrase,int i);

void sales_performance_menu();

void clearInputBuffer();
int safe_atoi(const char* str);
void null_checker(FILE* file_pointer);
bool alloc_checker(void *ptr);
bool folderExists(const char *folderName);

typedef struct{
char name[50];
int revenue;
int amount_sold;
int price;
int count;

}product;


int main()
{
    main_menu();
    return 0;
}

//General Menu Functions
void main_menu(){
    int user_input;
    title_function("Financial Analytics Report Tool");
    printf("\n1. View sales performance\n");
    printf("2. Sales Comparison\n");
    printf("3. View Analytics Report\n");
    printf("4. Exit Program\n\n");
    printf("Enter number [1-4] : ");
    scanf("%d",&user_input);
    switch(user_input){
    case 1:
        system("cls");
        if(chdir("years") == 0){
            printf("Folder entered");
        }
        usleep(900000);
        system("cls");
        pick_year();
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        system("cls");
        exit_animation("Exiting Program.",4);
        break;
    default:
        clearInputBuffer();
        system("cls");
        main_menu();
        break;
    }
}

void pick_year(){
    char year[6];
    title_function("Year Menu");
    printf("\nNote: Enter 4 to go back to Main Menu\n\n");
    printf("Enter Year: ");
    scanf("%s", year);

    if (folderExists(year)) {
        if(chdir(year) == 0){
            system("cls");
            printf("Folder entered");
            usleep("200000");
            system("cls");
            sales_performance_menu();
        }
        else{
            printf("Failed to enter directory");
        }
    }
    else if(strcmp(year,"4") == 0){
        system("cls");
        chdir("..");
        main_menu();
    }
    else {
        system("cls");
        printf("Could not find '%s' folder.\n", year);
        usleep("200000");
        system("cls");
        pick_year();
    }
}

//Sales Performance Function
void sales_performance_menu(){
    char folderName[15];
    title_function("Sales Performance Menu");
    printf("\nNote: Enter 4 to go back to Year Menu\n\n");
    printf("Enter month: ");
    scanf("%s", folderName);

    if (folderExists(folderName)) {
        if(chdir(folderName) == 0){
            printf("Folder entered");
        }
        else{
            printf("Failed to enter directory");
        }
    }
    else if(strcmp(folderName,"4") == 0){
        system("cls");
        chdir("..");
        pick_year();
    }
    else {
        system("cls");
        printf("Could not find '%s' folder.\n", folderName);
        usleep("200000");
        system("cls");
        sales_performance_menu();
    }
}

//Calculation Functions


//Reusable Functions
void title_function(char* text) {
    int length = strlen(text);
    int i;
    for (i = 0; i < length + 9; i++) {
        printf("*");
    }
    printf("\n");
    printf("  * ");
    printf("%s", text);
    printf(" *\n");
    for (i = 0; i < length + 9; i++) {
        printf("*");
    }
    printf("\n");
}
void exit_animation(char *phrase,int i){
    char storage[20];
    if(i<=5){
        printf("%s",phrase);
        strcpy(storage,phrase);
        strcat(storage,".");
        usleep(500000);
        i++;
        system("cls");
        exit_animation(storage,i);
    }
}

//Error Checking Functions
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}
int safe_atoi(const char* str) {
    if (str == NULL) {
        fprintf(stderr, "Error: String pointer is NULL.\n");
        return 0;
    }

    return atoi(str);
}
void null_checker(FILE* file_pointer){
    if(file_pointer == NULL){
        exit_animation("Error in opening file, returning to main-menu.",0);
        main_menu();
    }
}
bool alloc_checker(void *ptr) {
    if (ptr == NULL) {
        printf("Memory allocation failed.\n");
        return false;
    }
    return true;
}
bool folderExists(const char *folderName) {
    DIR *directory = opendir(folderName);
    if (directory != NULL) {
        closedir(directory);
        return true;
    }
    return false;
}
