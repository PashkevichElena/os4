#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>

#define FILE_NAME "root.bin"

enum operation { MD, MF, RD, RF, COPY, CUT, PASTE, WRITE, READ, CD, CD_BAK, DIR, FIL, ERROR, EXIT };

typedef struct File{
    char name[10];
    char data[10];
    int numberOfFiles;
    struct File *next;
    struct File *previous;
}File;

typedef struct Directory{
    char name[10];
    int numberOfDirectories;
    struct Directory* directories[100];
    int numberOfFiles;
    File* files[100];
}Directory;


void writeName(FILE*, char[10]);
int getNumberOfDirectories(Directory);
void writeNumberOfDirectories(FILE*, int);
void writeDirectories(FILE*, Directory);
int getNumberOfFiles(Directory);
void writeNumberOfFiles(FILE*, int);
void writeFiles(FILE*, Directory);

char* readName(FILE*);
char* readData(FILE*);
int readNumberOfDirectories(FILE*);
//void readDirectories(FILE*, Directory*);
int readNumberOfFiles(FILE*);
void readFiles(FILE*, Directory*);

char* directoryName;
char* fileName;
char* data;

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

bool isFileNameForWrite(char *input){
    char *secondWorld;
    strtok_r(input, " ", &secondWorld);
    if (secondWorld != NULL) {
        data = secondWorld;
    }
    fileName = input;

    char  *fileExt;
    strtok_r(input, ".", &fileExt);
    if (fileExt != NULL) {
        return true;
    } else return false;

    return false;
}

bool isFileName(char *input){
    char *secondWorld;
    strtok_r(input, " ", &secondWorld);
    if (secondWorld != NULL) {
        return false;
    }
    fileName = input;

    char  *fileExt;
    strtok_r(input, ".", &fileExt);
    if (fileExt != NULL) {
        return false;
    } else return true;

    return false;
}

bool isOnlyCommand(char *input){
    char *secondWorld;
    strtok_r(input, " ", &secondWorld);
    if (input != NULL) {
        return false;
    } else return true;
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

int getNumberOfFiles(Directory root){
    int numberOfFiles=0;
    for(int i=0; i<100; i++){
        if(root.files[i] != NULL){
            numberOfFiles += 1;
        } else break;
    }
    return numberOfFiles;
}

void writeNumberOfFiles(FILE *fileBin,  int numberOfFiles){
    int ifError = (int) fwrite(&numberOfFiles, sizeof(int), 1, fileBin);
    printf (" number of Files %i\n", numberOfFiles);
    if(ifError == -1){
        printf ("Error with writing number of Files %i\n", numberOfFiles);
    }
}

int readNumberOfFiles(FILE *fileBin){
    int numberOfFiles;
    int ifError = (int) fread(&numberOfFiles, sizeof(int), 1, fileBin);
    printf (" number of Files %i\n", numberOfFiles);
    if(ifError == -1){
        printf ("Error with reading number of Files %i\n", numberOfFiles);
    }
    return numberOfFiles;
}

void writeToFile(Directory root){
    FILE *fileBin = (FILE*)malloc(sizeof(FILE*));
    fileBin = fopen(FILE_NAME, "w+b");
    if (fileBin == 0) {
        printf("Error opening fileBin");
    }

    writeName(fileBin, root.name);
    root.numberOfDirectories = getNumberOfDirectories(root);
    writeNumberOfDirectories(fileBin, root.numberOfDirectories);
    writeDirectories(fileBin, root);
    root.numberOfFiles = getNumberOfFiles(root);
    writeNumberOfFiles(fileBin, root.numberOfFiles);
    writeFiles(fileBin, root);

    fclose(fileBin);
}

char* readName(FILE *fileBin){
    char* name;
    if(!(name = (char *)calloc(1, sizeof(char)*10))){
        printf("Возникли проблемы с выделением памяти"); }
    int ifError =  (int) fread(name, sizeof(char)*10, 1, fileBin);
    printf (" reading name %s\n", name);
    if(ifError == -1){
        printf ("Error with reading name %s\n", name);
    }
    return name;
}

Directory* readFromFile(){
    Directory *root;
    if(!(root = (Directory *)calloc(1, sizeof(Directory)))){
        printf("Возникли проблемы с выделением памяти"); }
    FILE *fileBin = (FILE*)malloc(sizeof(FILE*));
    fileBin = fopen(FILE_NAME, "r");
    if (fileBin == 0) {
        printf("Error opening fileBin");
    }
    strcpy( root->name, readName(fileBin));
    root->numberOfDirectories = readNumberOfDirectories(fileBin);
    readDirectories(fileBin, root);
    root->numberOfFiles = readNumberOfFiles(fileBin);
    readFiles(fileBin, root);

    fclose(fileBin);
    return root;
}

void writeName(FILE *fileBin, char name[10]){
   int ifError = (int) fwrite(name, sizeof(char)*10, 1, fileBin);
    printf (" name %s\n", name);
   if(ifError == -1){
       printf ("Error with writing name %s\n", name);
   }
}

int getNumberOfDirectories(Directory root){
    int numberOfDirectory=0;
    for(int i=0; i<100; i++){
        if(root.directories[i] != NULL){
            numberOfDirectory += 1;
        } else break;
    }
    return numberOfDirectory;
}

void writeNumberOfDirectories(FILE *fileBin, int numberOfDirectories){
    int ifError = (int) fwrite(&numberOfDirectories, sizeof(int), 1, fileBin);
    printf (" number of Directory %i\n", numberOfDirectories);
    if(ifError == -1){
        printf ("Error with writing number of Directory %i\n", numberOfDirectories);
    }
}

int readNumberOfDirectories(FILE *fileBin){
    int numberOfDirectories;
    int ifError = (int) fread(&numberOfDirectories, sizeof(int), 1, fileBin);
    printf ("read number of Directory %i\n", numberOfDirectories);
    if(ifError == -1){
        printf ("Error with writing number of Directory %i\n", numberOfDirectories);
    }
    return numberOfDirectories;
}

void readDirectories(FILE *fileBin, Directory* root){
    printf("number of directories in reading %i\n", root->numberOfDirectories);
    for(int i = 0; i < root->numberOfDirectories; i++){
        if(!(root->directories[i] = (Directory *)calloc(1, sizeof(Directory)))){
            printf("Возникли проблемы с выделением памяти"); }
        strcpy( root->directories[i]->name,  readName(fileBin));
        root->directories[i]->numberOfDirectories = readNumberOfDirectories(fileBin);
        readDirectories(fileBin, root->directories[i]);
        root->directories[i]->numberOfFiles = readNumberOfFiles(fileBin);
        readFiles(fileBin, root->directories[i]);
    }
}

void readFiles(FILE *fileBin, Directory* root){
    for(int i = 0; i < root->numberOfFiles; i++){
        if(!(root->files[i] = (File *)calloc(1, sizeof(File)))){
            printf("Возникли проблемы с выделением памяти"); }
        strcpy(root->files[i]->name, readName(fileBin));
        strcpy(root->files[i]->data, readData(fileBin));
        root->files[i]->numberOfFiles = readNumberOfFiles(fileBin);
        readFileTail(fileBin, root->files[i]);
    }
}

void writeDirectories(FILE *fileBin, Directory root){
    for(int i = 0; i < root.numberOfDirectories; i++){
        writeName(fileBin,  root.directories[i]->name);
        root.directories[i]->numberOfDirectories = getNumberOfDirectories(*root.directories[i]);
        writeNumberOfDirectories(fileBin, root.directories[i]->numberOfDirectories);
        writeDirectories(fileBin, *root.directories[i]);
        root.directories[i]->numberOfFiles = getNumberOfFiles(*root.directories[i]);//ATTENTION
        writeNumberOfFiles(fileBin, root.directories[i]->numberOfFiles);
        writeFiles(fileBin, *root.directories[i]);
    }
}

void readFileTail(FILE *fileBin, File* head) {
    int number = head->numberOfFiles;
    for (int i = 0; i < number; i++) {
        File *file;
        if (!(file = (File *) calloc (1, sizeof (File)))) {
            printf ("Возникли проблемы с выделением памяти");
        }
        strcpy (file->data, readData(fileBin));
        //  readData(fileBin, file->data);
        head->next = file;
        head = file;
      //  file = file->next;
    }
}

void writeData(FILE *fileBin, char data[10]){
    int ifError = (int) fwrite(data, sizeof(char)*10, 1, fileBin);
    printf (" data  %s\n", data);
    if(ifError == -1){
        printf ("Error with writing data  %s\n", data);
    }
}

char* readData(FILE *fileBin){
    char* data;
    if(!(data = (char *)calloc(1, sizeof(char)*10))){
        printf("Возникли проблемы с выделением памяти"); }
    int ifError =  (int) fread(data, sizeof(char)*10, 1, fileBin);
    printf (" reading data %s\n", data);
    if(ifError == -1){
        printf ("Error with reading name %s\n", data);
    }
    return  data;
}
void writeFiles(FILE *fileBin, Directory root){
    //  printf ("Error with writing data  %s\n", data);
    for(int i = 0; i < root.numberOfFiles; i++){
        writeName(fileBin, root.files[i]->name);
        writeData(fileBin, root.files[i]->data);
        root.files[i]->numberOfFiles = getNumberOfFilesTail(root.files[i]);
        writeNumberOfFiles(fileBin, root.files[i]->numberOfFiles);
        writeFileTail(fileBin, root.files[i]);
    }
}

void writeFileTail(FILE *fileBin, File* file){
    int number = file -> numberOfFiles;
   // printf ("number of File tail in writinf %i", file->numberOfFiles);
    if(file -> numberOfFiles != 0){
        file = file->next;
        for(int i=0; i < number; i++){
          //  writeName(fileBin, file->name);
       //     printf ("4");
            writeData(fileBin, file->data);
       //     printf ("5");
            file = file->next;
        //    printf ("6");
        }
    }
}

int getNumberOfFilesTail(File* file){
    int numberOfFileTail = 0;
    //ATTENTION
    //----------ВОТ ЭТОТ ПАГАНЕЦ ДОБАВЛЯЕТ ЕДИНИЦУ
    while (file -> next){
        numberOfFileTail += 1;
        file = file -> next;
    }
    return  numberOfFileTail;
}




void  makeDirectory();
void  makeFile();
void  removeDirectory();
void  removeFile();
void  copyFile();
void  cutFile();
void  pasteFile();
void  writeFile();
void  readFile();
void  openDirectory();
void  openDirectoryBack();
void  showDirectoryList();
void  showFileList();

const char *root = "root";
const char *to = ">";
const char *separator = "/";
char path[1024];

Directory* cutFromDirectory;
char nameOfCatedFile[1024];
Directory* activDirectory;
File* copiedFile;

typedef struct Path{
    Directory* dir;
    struct Path* next;
    struct Path* prev;
} Path;

Path *end;

int main() {

    char input[1024];
    if (!(end = (Path *) calloc (1, sizeof (Path)))) {
        printf ("Возникли проблемы с выделением памяти\n");
    }
    Directory *root;
    root = readFromFile();
   // strcpy(root->name, "root");
    activDirectory = root;
    end->dir = root;

    snprintf(path, sizeof path, "%s", root->name);
    bool running = true;
    printf("Доступные операции:\n");
    printf("  создание каталога:        md имя_каталога\n");
    printf("  создание файла:           mf имя_файла\n");
    printf("  удаление каталога:        rd имя_каталога\n");
    printf("  удаление файла:           rf имя_файла\n");
    printf("  копирование файла:        copy имя_файла\n");
    printf("  вставить файл:            paste\n");
    printf("  вырезать файл:            cut имя_файла\n");
    printf("  запись в файл:            write имя_файла данные\n");
    printf("  чтение из файла:          read имя_файла\n");
    printf("  перейти  каталог:         cd путь_каталога\n");
    printf("  перейти вверх:            cd..\n");
    printf("  список каталогов:         dir \n");
    printf("  список файлов:            file \n");
    printf("  выход:                    exit\n");
    while(running){
        printf("%s", path);
        printf("%s", to);
        gets(&input);
        int cases = changeStringToInt(input);
        switch(cases) {
            case MD:
                makeDirectory();
                break;
            case MF:
                makeFile();
                break;
            case RD:
                removeDirectory();
                break;
            case RF:
                removeFile();
                break;
            case COPY:
                copyFile();
                break;
            case CUT:
                cutFile();
                break;
            case PASTE:
                pasteFile();
                break;
            case WRITE:
                writeFile();
                break;
            case READ:
                readFile();
                break;
            case CD:
                openDirectory();
                break;
            case CD_BAK:
                openDirectoryBack();
                break;
            case DIR:
                showDirectoryList();
                break;
            case FIL:
                showFileList();
                break;
            case ERROR:
                printf("Please check your operation\n");
                break;
            case EXIT:
                writeToFile(*root);
                running = false;
                break;
        }
    }
    float x;
    scanf ("%f",&x);
    return 0;
}

void copyFile(){
    bool isFileExist = false;
  for(int i = 0; i < 100; i++){
      if (activDirectory->files[i] != NULL) {
          if (strcmp (activDirectory->files[i]->name, fileName) == 0) {
              if (!(copiedFile = (File *) calloc (1, sizeof (File)))) {
                  printf ("Возникли проблемы с выделением памяти\n");
              }
              isFileExist = true;
              strcpy(copiedFile->name, activDirectory->files[i]->name);
              copiedFile->next = activDirectory->files[i]->next;
              copiedFile->previous = activDirectory->files[i]->previous;
              strcpy(copiedFile->data, activDirectory->files[i]->data);
              break;
          }
      }
  }
    if(!isFileExist){
        printf ("File doesn't exist\n");
    }
}

void pasteFile(){
    if(copiedFile != NULL){
        for(int i = 0; i < 100; i++){
            if (activDirectory->files[i] == NULL) {
                if (!(activDirectory->files[i] = (File *) calloc (1, sizeof (File)))) {
                    printf ("Возникли проблемы с выделением памяти\n");
                }
                strcpy(activDirectory->files[i]->name, copiedFile->name);
                activDirectory->files[i]->next = copiedFile->next;
                activDirectory->files[i]->previous = copiedFile->previous;
                strcpy(activDirectory->files[i]->data,copiedFile->data);
                free(copiedFile);
                copiedFile=NULL;
                break;
            }
        }
    } else {
        printf ("No file to paste\n");
    }
    if(cutFromDirectory != NULL){
        for (int i = 0; i < 100; i++) {
            if (cutFromDirectory->files[i] != NULL) {
                if (strcmp (cutFromDirectory->files[i]->name, nameOfCatedFile) == 0) {
                    free (cutFromDirectory->files[i]->name);
                    free (&cutFromDirectory->files[i]->data);
                    free (&cutFromDirectory->files[i]->next);
                    free (&cutFromDirectory->files[i]->previous);
                    cutFromDirectory->files[i] = NULL;
                    cutFromDirectory = NULL;
                    break;
                }
            }
        }
    }
}

void cutFile(){
    bool isFileExist = false;
    for(int i = 0; i < 100; i++){
        if (activDirectory->files[i] != NULL) {
            if (strcmp (activDirectory->files[i]->name, fileName) == 0) {
                if (!(copiedFile = (File *) calloc (1, sizeof (File)))) {
                    printf ("Возникли проблемы с выделением памяти\n");
                }
                isFileExist = true;
                strcpy(copiedFile->name, activDirectory->files[i]->name);
                copiedFile->next = activDirectory->files[i]->next;
                copiedFile->previous = activDirectory->files[i]->previous;
                strcpy(copiedFile->data, activDirectory->files[i]->data);
                strcpy(nameOfCatedFile,  activDirectory->files[i]->name);
                cutFromDirectory = activDirectory;
                break;
            }
        }
    }
    if(!isFileExist){
        printf ("File doesn't exist");
    }
}

void writeFile(){
    bool isFileExist = false;
    int globalI=0;
    for (int i = 0; i < 100; i++) {
        if (activDirectory->files[i] != NULL) {
            if (strcmp (activDirectory->files[i]->name, fileName) == 0) {
                isFileExist = true;
                globalI = i;
                break;
            }
        }
    }
    if(!isFileExist){
        for (int i = 0; i < 100; i++) {
            if (activDirectory->files[i] == NULL) {
                if (!(activDirectory->files[i] = (File *) calloc (1, sizeof (File)))) {
                    printf ("Возникли проблемы с выделением памяти\n");
                }
                strcpy (activDirectory->files[i]->name, fileName);
                globalI = i;
                break;
            }
        }
    }
    activDirectory->files[ globalI]->next = activDirectory->files[globalI]->previous = NULL;
    File* activ;
    activ = activDirectory->files[globalI];
    for (int j = 0; j<10; j++){
        activ->data[j] = data[j];
    }
    for(int k = 10 ; k < 1024; k++){

        if(data[k+1]==NULL){
            break;
        }
        if(k%10 == 0){
            File* next;
            if (!(next = (File *) calloc (1, sizeof (File)))) {
                printf ("Возникли проблемы с выделением памяти\n");
            }
            activ -> next = next;
            next -> previous = activ;
            activ = next;
        }
        activ -> data[k%10] = data[k];

    }

}

void readFile(){
    bool isFileDoesntExist = true;
    for (int i = 0; i < 100; i++) {
        if (activDirectory->files[i] != NULL) {
            if (strcmp (activDirectory->files[i]->name, fileName) == 0) {
                isFileDoesntExist = false;
                File *activ;
                activ = activDirectory->files[i];
                while(activ != NULL){
                    printf ("%s", activ->data);
                    activ = activ->next;
                }
            }
        }
    }
    if(isFileDoesntExist) {
        printf ("File doesn't exist");
    }
    printf ("\n");
}

void showFileList(){
    bool check = true;
    for (int i = 0; i < 100; i++) {
        if (activDirectory->files[i] != NULL) {
            printf ("   %s\n", activDirectory->files[i]->name);
            check = false;
        }
    }
    if (check) {
           printf("File list is empty\n");
    }
}

void showDirectoryList(){
    bool check = true;
    for (int i = 0; i < 100; i++) {
        if (activDirectory->directories[i] != NULL) {
            printf ("   %s\n", activDirectory->directories[i]->name);
            check = false;
        }
    }
    if (check) {
        printf("Directory list is empty\n");
    }
}

void openDirectoryBack(){
    if(end -> prev != NULL) {
        char copyPath[1024];
        strcpy(copyPath, path);
        snprintf (path, sizeof path, "%s", "");

        char *token, *last;
        token = strtok_r(copyPath, "/", &last);
        snprintf (path, sizeof path, "%s%s", path,token);

        while(last != NULL) {
            token = strtok_r (NULL, "/", &last);
            if (last != NULL) {
                snprintf (path, sizeof path, "%s%s%s", path, separator, token);
            }
        }

        activDirectory = (end->prev)->dir;
        end = end->prev;
        free (end->next);
        end->next = NULL;
    } else {
        printf("Current diretory has not parent directory\n");
    }

}
void makeDirectory(){
    for (int i = 0; i < 100; i++){
        if(activDirectory->directories[i] == NULL){
            if(!(activDirectory->directories[i] = (Directory *)calloc(1, sizeof(Directory)))){
                printf("Возникли проблемы с выделением памяти\n");
            }
            strcpy(activDirectory->directories[i]->name,directoryName);
            break;
        } else {
            if (strcmp (activDirectory->directories[i]->name, directoryName) == 0) {
                printf("Директория с этим именем уже существует\n");
                break;
            }
        }
    }
}

void makeFile() {
    for (int i = 0; i < 100; i++) {
        if (activDirectory->files[i] != NULL) {
            if (strcmp (activDirectory->files[i]->name, fileName) == 0) {
                printf ("Файл с этим именем уже существует\n");
                break;
            }
        } else {
            if (!(activDirectory->files[i] = (File *) calloc (1, sizeof (File)))) {
                printf ("Возникли проблемы с выделением памяти\n");
            }
            strcpy (activDirectory->files[i]->name, fileName);
            break;
        }
    }
}

void removeDirectory() {
    bool check = false;
    for (int i = 0; i < 100; i++) {
        if (activDirectory->directories[i] != NULL) {
            if (strcmp (activDirectory->directories[i]->name, directoryName) == 0) {
                free (activDirectory->directories[i]);
               // free (activDirectory->directories[i]->directories);
                //free (activDirectory->directories[i]->files);
                activDirectory->directories[i] = NULL;
                check = false;

                break;
            } else {
                check = true;
            }
        }
    }
    if(check){ printf ("Директория с этим именем не существует\n");}
}
void removeFile() {
    bool check = false;
    for (int i = 0; i < 100; i++) {
        if (activDirectory->files[i] != NULL) {
            if (strcmp (activDirectory->files[i]->name, fileName) == 0) {
                free (activDirectory->files[i]);
                activDirectory->files[i] = NULL;
                check = false;
                break;
            } else {
                check = true;
            }
        }
    }
    if(check){ printf ("Файл с этим именем не существует\n");}
}

void openDirectory(){
    bool  check = true;
    for (int i = 0 ; i < 100; i++){
        if(activDirectory->directories[i] != NULL){
            if(strcmp(activDirectory->directories[i]->name, directoryName) == 0){
                snprintf (path, sizeof path, "%s%s%s", path, separator, directoryName);
                check = false;
                activDirectory = activDirectory->directories[i];
                Path *current;
                if (!(current = (Path *) calloc (1, sizeof (Path)))) {
                    printf ("Возникли проблемы с выделением памяти\n");
                }
                current -> dir = activDirectory;
                current -> prev = end;
                end -> next = current;
                end = current;
                break;
            }
        }
    }
    if(check){
        printf ("Directory doesn't exist\n");
    }
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
                if(isFileName(last)){
                    return COPY;
                } else return ERROR;
            }
            case PASTE: {
                if(isOnlyCommand(last)){
                    return PASTE;
                } else return ERROR;
            }
            case CUT: {
                if(isFileName(last)){
                    return CUT;
                } else return ERROR;
            }
            case WRITE: {
                if(isFileNameForWrite(last)){
                    return WRITE;
                } else return ERROR;
            }
            case READ: {
                if(isFileName(last)){
                    return READ;
                } else return ERROR;
            }
            case CD: {
                if(isDirectoryPath(last)){
                    return CD;
                } else return ERROR;
            }
            case CD_BAK: {
                if(isOnlyCommand(last)){
                    return CD_BAK;
                } else return ERROR;
            }
            case FIL: {
                if(isOnlyCommand(last)){
                    return FIL;
                } else return ERROR;
            }
            case DIR: {
                if(isOnlyCommand(last)){
                    return DIR;
                } else return ERROR;
            }
            case EXIT: {
                if(isOnlyCommand(last)){
                    return EXIT;
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
    char* cut = "cut";
    char* paste = "paste";
    char* write = "write";
    char* read = "read";
    char* file = "file";
    char* cd = "cd";
    char* cd_bak = "cd..";
    char* dir = "dir";
    char* exit = "exit";

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
    } else if(strcmp(operation, cut) == 0) {
        return CUT;
    }else if(strcmp(operation, paste) == 0) {
        return PASTE;
    } else if(strcmp(operation, write) == 0){
        return WRITE;
    } else if(strcmp(operation, read) == 0) {
        return READ;
    } else if(strcmp(operation, cd) == 0) {
        return CD;
    }else if(strcmp(operation, cd_bak) == 0) {
        return CD_BAK;
    }else if(strcmp(operation, file) == 0) {
        return FIL;
    } else if (strcmp(operation, dir) == 0){
        return DIR;
    } else if (strcmp(operation, exit) == 0){
        return EXIT;
    } else  return ERROR;
}
