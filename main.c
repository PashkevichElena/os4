#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>

enum operation {
    MD = 0,
    MF = 1,
    RD = 2,
    RF = 3,
    COPY = 4,
    MOVE = 5,
    WRITE = 6,
    READ = 7,
    CD = 8,
    ERROR = 9
};
char* directoryName;
char* fileName;
bool isDirectoryName(char* input){
    char *secondWorld;
    strtok_r(input, " ", &secondWorld);
    if (secondWorld != NULL) {
        return false;
    }

    char  *fileExt;
    strtok_r(input, ".", &fileExt);
    if (fileExt != NULL) {
        return false;
    }
    directoryName = input;
    return true;
}
bool isFileName(char *input){
    char *secondWorld;
    strtok_r(input, " ", &secondWorld);
    if (secondWorld != NULL) {
        return false;
    }

    char  *fileExt;
    strtok_r(input, ".", &fileExt);
    if (fileExt != NULL) {
        return true;
    } else return false;
  //  fileName = input;
    return true;
}

bool isFilePath(char *input){
    char *secondWorld;
    strtok_r(input, " ", &secondWorld);
    if (secondWorld != NULL) {
        return false;
    }

    char  *fileExt;
    strtok_r(input, ".", &fileExt);
    if (fileExt != NULL) {
        return false;
    }
    return true;
}
bool isFilePathes(char *input){
    char *secondWorld;
    strtok_r(input, " ", &secondWorld);
    if (secondWorld != NULL) {
        return false;
    }

    char  *fileExt;
    strtok_r(input, ".", &fileExt);
    if (fileExt != NULL) {
        return true;
    } else return false;
    return true;
}

bool isDirectoryPath(char *input){
    char *secondWorld;
    strtok_r(input, " ", &secondWorld);
    if (secondWorld != NULL) {
        return false;
    }

    char  *fileExt;
    strtok_r(input, ".", &fileExt);
    if (fileExt != NULL) {
        return false;
    }
    directoryName = input;
    return true;
}
typedef struct name_addres{
    char name[100];
    struct file *addres;
    struct name_addres *next;
    struct name_addres *previous;
}name_addres;
typedef struct file{
    char name[4];
    struct name_addres *first;
    char data[1024];
    struct file *next;
    struct file *previous;
}file;

const char *root = "root";
const char *to = ">";
const char *separator = "/";
char path[1024];
FILE* openFile(const char *keysFilename) {
    FILE *files = (FILE*)malloc(sizeof(FILE));
    files = fopen(keysFilename, "w+b");
    if (!files) {
        return NULL;
    }
    return files;
}
void createOnePerson(FILE *fileBin) {
    file pFile;

    strcpy(pFile.name,"root");

    fwrite(&pFile, sizeof(pFile), 1, fileBin);

}
void createRoot(FILE *file) {
    int counter = 1;
    createOnePerson(file);

    rewind(file);
    fwrite(&counter, sizeof(counter), 1, file);
}
file* readFiles(FILE *keys, int *size) {
    file *out = NULL;
    rewind(keys);
    fread(size, sizeof(*size), 1, keys);
    out = (file*) malloc(*size * sizeof(file));
    fread(out, sizeof(file), *size, keys);
    return out;
}
void printFileNames(file pk) {
    printf("%s\n", pk.name);
}
int main() {
    int size;
    int i;
    file *f = NULL;
    FILE *fileBin = openFile ("D:/fileManager.bin");
    if (fileBin == 0) {
        printf ("Ошибка открытия файла");
    }
    createRoot (fileBin);
    f = readFiles (fileBin, &size);

    for (i = 0; i < size; i++) {
        printFileNames (f[i]);
    }

    fclose (fileBin);
    return 0;
}
    /*char input[1024];
    file *taleFile, *headFile;
    taleFile = headFile = NULL;
    snprintf(path, sizeof path, "%s%s", root, to);
    printf("Доступные операции:\n");
    printf("  создание каталога:        md имя_каталога\n");
    printf("  создание файла:           mf имя_файла\n");
    printf("  удаление каталога:        rd имя_каталога\n");
    printf("  удаление файла:           rf имя_файла\n");
    printf("  копирование файла:        copy путь_файла1 путь_файла2\n");
    printf("  перемещение файла:        move путь_файла1 путь_файла2\n");
    printf("  запись в файл:            write путь_файла1\n");
    printf("  чтение из файла:          read путь_файла1 путь_файла2\n");
    printf("  перейти  каталог:         cd путь_каталога\n");
    printf("  выход:              exit\n");
    while(true){
        printf("%s", path);
        gets(&input);
        int cases = changeStringToInt(input);
        switch(cases) {
            case MD:
                printf("Please check your operationMD\n");
                makeDirectory(&taleFile, &headFile);
                break;
            case MF:
                printf("Please check your operationMF\n");
                break;
            case RD:
                printf("Please check your operationRD\n");
                break;
            case RF:
                printf("Please check your operationRF\n");
                break;
            case COPY:
                printf("Please check your operationCPY\n");
                break;
            case MOVE:
                printf("Please check your operationMOVE\n");
                break;
            case WRITE:
                printf("Please check your operationWRITE\n");
                break;
            case READ:
                printf("Please check your operationREAD\n");
                break;
            case CD:
                printf("Please check your operationCD\n");
                openDirectory(headFile);
                break;
            case ERROR:
                printf("Please check your operation\n");
                break;
        }
    }
    return 0;*/
//}

void makeDirectory(file **tailFile, file **headFile){
    file *t;
    if(!(t = (file *)calloc(1, sizeof(file)))){ printf("Возникли проблемы с выделением памяти"); }

    strcpy(t->name, directoryName);

    if(!*tailFile || !*headFile){//список не создан
        *tailFile = t;
        *headFile = t;
    } else {//список есть добавили в конец
        t->next = *tailFile; //указатель на хвост
        (*tailFile)->previous = t; //хвост уазывает на новый элемент
        *tailFile = t;//указатель хвоста на новый элементвв
    }
}

bool isDirectoryExist(file *p){
    char *last, *directory;
    bool check = false;
    directory = strtok_r(directoryName, "/", &last);
    if (directory != NULL) {
        char *fileName = p -> name;
        while(p){
            if(strcmp(fileName, directoryName) == 0){
                check = true;
                break;
            } else p = p -> next;
        }
        //допилить проверкку для длинного пути когда научишься писать папку в папку
        // strtok_r(NULL, "/", &last);
    }
    return check;
}

void openDirectory(file *headFile){
    if(isDirectoryExist(headFile)) {
        snprintf (path, sizeof path, "%s%s%s%s", root, separator, directoryName, to);
    } else { printf("Проверьте путь к каталогу"); }
    //проверка имени и соответствия что такие директории есть
    //чтобы глобальный указатель на текущую директорию указывал на ссотетсувующщую ячейку по имени

}


int changeStringToInt(char* input){
    char *firstWord, *last;
    int typeOfFirstWord = 0;
    firstWord = strtok_r(input, " ", &last);
    if(firstWord != NULL){
        typeOfFirstWord = getTypeOfFirstWord (firstWord);
        switch(typeOfFirstWord){
            case MD: {
                if(isDirectoryName(last)){
                    return MD;
                } else return ERROR;
            }
            case MF: {
                if(isFileName(last)){
                    return MF;
                } else return ERROR;
            }
            case RD: {
                if(isDirectoryName (last)){
                    return RD;
                } else return ERROR;
            }
            case RF: {
                if(isFileName (last)){
                    return RF;
                } else return ERROR;
            }
            case COPY: {
                if(isFilePathes(last)){
                    return COPY;
                } else return ERROR;
            }
            case MOVE: {
                if(isFilePathes(last)){
                    return MOVE;
                } else return ERROR;
            }
            case WRITE: {
                if(isFilePath(last)){
                    return WRITE;
                } else return ERROR;
            }
            case READ: {
                if(isFilePath(last)){
                    return READ;
                } else return ERROR;
            }
            case CD: {
                if(isDirectoryPath(last)){
                    return CD;
                } else return ERROR;
            }
            case ERROR: {
                return ERROR;
            }
        }
    } else {
        return ERROR;
    }
}

int getTypeOfFirstWord(char* operation){
    char* md = "md";
    char* mf = "mf";
    char* rd = "rd";
    char* rf = "rf";
    char* copy = "copy";
    char* move = "move";
    char* write = "write";
    char* read = "read";
    char* cd = "cd";

    if(strcmp(operation, md) == 0){
       return MD;
    } else if(strcmp(operation, mf) == 0){
        return MF;
    } else if(strcmp(operation, rd) == 0) {
        return RD;
    } else if(strcmp(operation, rf) == 0){
        return RF;
    } else if(strcmp(operation, copy) == 0){
        return COPY;
    } else if(strcmp(operation, move) == 0) {
        return MOVE;
    } else if(strcmp(operation, write) == 0){
        return WRITE;
    } else if(strcmp(operation, read) == 0) {
        return READ;
    } else if(strcmp(operation, cd) == 0){
        return CD;
    } else  return ERROR;
}
//ЧТо ДЕЛАТЬ С СВ