#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>

char *FILE_NAME;// "root.bin"

enum operation { MD, MF, RD, RF, COPY, CUT, PASTE, WRITE, READ, CD, CD_BAK, DIR,  ERROR, SAVE, EXIT };

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
char* toBin(char*);
int getTypeOfFirstWord (char*);
int changeStringToInt(char*);
int getNumberOfDirectories(Directory);
void writeNumberOfDirectories(FILE*, int);
void writeDirectories(FILE*, Directory);
int getNumberOfFiles(Directory);
void writeNumberOfFiles(FILE*, int);
void writeFiles(FILE*, Directory);

void  makeDirectory();
void  makeFile();
void  removeDirectory();
void  removeFile();
void  copyFile();
void  saveDamp();
void  cutFile();
void  pasteFile();
void  writeFile();
void  readFile();
void  openDirectory();
void  openDirectoryBack();
void  showDirectoryList();
void  showFileList();

char* readName(FILE*);
char* readData(FILE*);
int readNumberOfDirectories(FILE*);
int readNumberOfFiles(FILE*);
void readFiles(FILE*, Directory*);
bool isFileName(char*);
bool isBin(char* input);
bool isTxt(char* input);

char directoryName;
char fileName;
char* data;

bool isDirectoryName(char* input){
    if(input==NULL){
       printf("Command without dir name\n");
        return false;
   }
    char *secondWorld;
    char *dirName;
    dirName = strtok_r(input, " ", &secondWorld);
    if (secondWorld != NULL) {
        return false;
    }

    char  *fileExt;
    strtok_r(dirName, ".", &fileExt);
    if (fileExt != NULL) {
        printf("Your file hasn't extension\n");
        return false;
    }
    strcpy(&directoryName, dirName);
    return true;
}

bool isFileNameForWrite(char *input){
    if(input==NULL){
        printf("Command without file name\n");
        return false;
    }
    char *secondWorld;
    strtok_r(input, " ", &secondWorld);
    if (secondWorld != NULL) {
        data = secondWorld;
        strcpy(&fileName, input);
        return true;
    }

    char  *fileExt;
    strtok_r(input, ".", &fileExt);
    if (fileExt != NULL) {
        printf("Your file hasn't extension\n");
        return false;
    } else return true;

    return false;
}

bool isFileName(char *input){
    if(input==NULL){
        printf("Command without file name\n");
        return false;
    }
    char *secondWorld;
    char copi;
    strtok_r(input, " ", &secondWorld);
    if (secondWorld != NULL) {
        return false;
    }
    strcpy(&fileName, input);
    strcpy(&copi, input);

    char  *fileExt;
    strtok_r(&copi, ".", &fileExt);
    if (fileExt != NULL) {
        if(!isTxt(fileExt)){
            if(isBin(fileExt)){
                return true;
            } else {
                printf("Please, check your file extension. It must be .txt or .bin\n");
                return  false;}
        } else return true;

    } else {
        printf("Your file without extension\n");
        return false;
    }

    return false;
}

bool isTxt(char* input){
    //  printf("%s", input);'

        if(strcmp(input, "txt")==0){
            return true;}
        else return false;


}

bool isBin(char* input){
        if(strcmp(input, "bin")==0){
            return true;}
        else return false;
}

bool isOnlyCommand(char *input){
    char *secondWorld;
    strtok_r(input, " ", &secondWorld);
    if (input != NULL) {
        return false;
    } else return true;
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
    strcpy( &directoryName, input);
   // directoryName = input;
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
    if(ifError == -1){
        printf ("Error with writing number of Files %i\n", numberOfFiles);
    }
}

int readNumberOfFiles(FILE *fileBin){
    int numberOfFiles;
    int ifError = (int) fread(&numberOfFiles, sizeof(int), 1, fileBin);
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
        printf("Error memory"); }
    int ifError =  (int) fread(name, sizeof(char)*10, 1, fileBin);
    if(ifError == -1){
        printf ("Error with reading name %s\n", name);
    } else {
        printf ("Name %s\n", name);
    }
    return name;
}

Directory* readFromFile(){
    printf("-------------Start reading damp--------------------\n");
    Directory *root;
    if(!(root = (Directory *)calloc(1, sizeof(Directory)))){
        printf("Error memory"); }

    FILE *fileBin;// = (FILE*)malloc(sizeof(FILE*));
    fileBin = fopen(FILE_NAME, "r");
    if (fileBin == NULL) {
        printf("Error opening fileBin");
    }
    strcpy( root->name, readName(fileBin));
    if(strcmp(root-> name, "")==0){
    strcpy( root->name, "root");
    return root;
    }
    root->numberOfDirectories = readNumberOfDirectories(fileBin);
    readDirectories(fileBin, root);
    root->numberOfFiles = readNumberOfFiles(fileBin);
    readFiles(fileBin, root);

    fclose(fileBin);
    return root;
}

void writeName(FILE *fileBin, char name[10]){
   int ifError = (int) fwrite(name, sizeof(char)*10, 1, fileBin);

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
    if(ifError == -1){
        printf ("Error with writing number of Directory %i\n", numberOfDirectories);
    }
}

int readNumberOfDirectories(FILE *fileBin){
    int numberOfDirectories;
    int ifError = (int) fread(&numberOfDirectories, sizeof(int), 1, fileBin);
    if(ifError == -1){
        printf ("Error with writing number of Directory %i\n", numberOfDirectories);
    }
    return numberOfDirectories;
}

void readDirectories(FILE *fileBin, Directory* root){
    for(int i = 0; i < root->numberOfDirectories; i++){
        if(!(root->directories[i] = (Directory *)calloc(1, sizeof(Directory)))){
            printf("Error memory"); }
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
            printf("Error memory"); }
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
            printf ("Error memory");
        }
        strcpy (file->data, readData(fileBin));
        head->next = file;
        head = file;
    }
}

void writeData(FILE *fileBin, char data[10]){
    int ifError = (int) fwrite(data, sizeof(char)*10, 1, fileBin);
    if(ifError == -1){
        printf ("Error with writing data  %s\n", data);
    }
}

char* readData(FILE *fileBin){
    char* data;
    if(!(data = (char *)calloc(1, sizeof(char)*10))){
        printf("Error memory"); }
    int ifError =  (int) fread(data, sizeof(char)*10, 1, fileBin);
    if(ifError == -1){
        printf ("Error with reading name %s\n", data);
    }
    return  data;
}

void writeFiles(FILE *fileBin, Directory root){
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
    if(file -> numberOfFiles != 0){
        file = file->next;
        for(int i=0; i < number; i++){
            writeData(fileBin, file->data);
            file = file->next;
        }
    }
}

int getNumberOfFilesTail(File* file){
    int numberOfFileTail = 0;
    while (file -> next){
        numberOfFileTail += 1;
        file = file -> next;
    }
    return  numberOfFileTail;
}


//const char *root = "root";
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
Directory *root;

int main(int argc, char* argv[]) {

    char input;
    if (!(end = (Path *) calloc (1, sizeof (Path)))) {
        printf ("Error memory\n");
    }


    if(!(root = (Directory *)calloc(1, sizeof(Directory)))){
        printf("Error memory"); }
    if(argc == 1){
        if(! (*(root->name) = (char)calloc(1, sizeof(char)))){
            printf("Error memory");}
        if(! (FILE_NAME = (char *)calloc(1, sizeof(char)))){
            printf("Error memory");}
        strcpy (FILE_NAME, "root.bin");
        strcpy (root->name, "root");
    } else if(argc == 2){
        if(! (FILE_NAME = (char *)calloc(1, sizeof(char)))){
            printf("Error memory");}
        strcpy(FILE_NAME, argv[1]);
        root = readFromFile();
    } else  printf("Check number of arguments");

 /*   if(argc == 2){
        if(! (FILE_NAME = (char *)calloc(1, sizeof(char)))){
            printf("Error memory");}
            strcpy(FILE_NAME, argv[1]);
            root = readFromFile();
    } else {
        if(! (*root->name = (char *)calloc(1, sizeof(char)))){
            printf("Error memory");}
            strcpy (root->name, "root");

    }
*/
    activDirectory = root;
    end->dir = root;

    snprintf(path, sizeof path, "%s", root->name);
    bool running = true;
    printf("Operation:\n");
    printf("  create directory:      md имя_каталога\n");
    printf("  create file:           mf имя_файла\n");
    printf("  remove directory:      rd имя_каталога\n");
    printf("  remove file:           rf имя_файла\n");
    printf("  copy file:             copy имя_файла\n");
    printf("  paste file:            paste\n");
    printf("  cut file:              cut имя_файла\n");
    printf("  write to file:         write имя_файла данные\n");
    printf("  read from file:        read имя_файла\n");
    printf("  goto directory:        cd путь_каталога\n");
    printf("  save damp:             save имя_файла\n");
    printf("  goto prev directory:   cd..\n");
    printf("  list of directory:     dir \n");
    printf("  exit:                  exit\n");
    while(running){
        printf("%s", path);
        printf("%s", to);

        gets(&input);
        int cases = changeStringToInt(&input);
        switch(cases) {
            case MD:
                makeDirectory();
                running = true;
                break;
            case MF:
                makeFile();
                running = true;
                break;
            case RD:
                removeDirectory();
                running = true;
                break;
            case RF:
                removeFile();
                running = true;
                break;
            case COPY:
                copyFile();
                running = true;
                break;
            case CUT:
                cutFile();
                running = true;
                break;
            case PASTE:
                pasteFile();
                running = true;
                break;
            case WRITE:
                writeFile();
                running = true;
                break;
            case READ:
                readFile();
                running = true;
                break;
            case CD:
                openDirectory();
                running = true;
                break;
            case CD_BAK:
                openDirectoryBack();
                running = true;
                break;
            case DIR:
                showDirectoryList();
                running = true;
                break;

            case SAVE:
                saveDamp();
                running = true;
                break;
            case ERROR:
                printf("Please check your operation\n");
                running = true;
                break;
            case EXIT:
                running = false;
                break;

            default: break;
        }
    }
   // float x;
    //scanf ("%f",&x);
    return 0;
}



void copyFile(){
    bool isFileExist = false;
  for(int i = 0; i < 100; i++){
      if (activDirectory->files[i] != NULL) {
          if (strcmp (activDirectory->files[i]->name, &fileName) == 0) {
              if (!(copiedFile = (File *) calloc (1, sizeof (File)))) {
                  printf ("Error with memory\n");
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
    if(strcmp(copiedFile->name, "") !=0){
        for(int i = 0; i < 100; i++){
            if (activDirectory->files[i] == NULL) {
                if (!(activDirectory->files[i] = (File *) calloc (1, sizeof (File)))) {
                    printf ("Error with memory\n");
                }

                strcpy(activDirectory->files[i]->name, copiedFile->name);
                activDirectory->files[i]->next = copiedFile->next;
                activDirectory->files[i]->previous = copiedFile->previous;
                strcpy(activDirectory->files[i]->data,copiedFile->data);
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
                   free (cutFromDirectory->files[i]);
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
            if (strcmp (activDirectory->files[i]->name, &fileName) == 0) {
                if (!(copiedFile = (File *) calloc (1, sizeof (File)))) {
                    printf ("Error with memory\n");
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
        printf ("File doesn't exist\n");
    }
}

void writeFile(){
    bool isFileExist = false;
    int globalI=0;
    for (int i = 0; i < 100; i++) {
        if (activDirectory->files[i] != NULL) {
            if (strcmp (activDirectory->files[i]->name, &fileName) == 0) {
                isFileExist = true;
                File *activ = activDirectory->files[i];
                while (activ->next != NULL) {
                    activ = activ->next;
                }
                char copi;
                strcpy(&copi, activDirectory->files[i]->name);

                char  *fileExt;
                strtok_r(&copi, ".", &fileExt);
                if(isBin(fileExt)){
                    strcpy (data, toBin(data));
                }

                        for (int k = 0; k < 1024; k++) {

                            if (k % 10 == 0) {
                                File *next;
                                if (!(next = (File *) calloc (1, sizeof (File)))) {
                                    printf ("Error with memory\n");
                                }
                                activ->next = next;
                                next->previous = activ;
                                activ = next;
                            }
                            activ->data[k % 10] = data[k];
                            if (data[k + 1] == NULL) {
                                for(int index = k; k<0; k--){
                                    data[index] = NULL;
                                }
                                break;
                            }
                        }
                break;
            }
        }
    }

    if(!isFileExist){
        for (int i = 0; i < 100; i++) {
            if (activDirectory->files[i] == NULL) {
                if (!(activDirectory->files[i] = (File *) calloc (1, sizeof (File)))) {
                    printf ("Error with memory\n");
                }
                strcpy (activDirectory->files[i]->name, &fileName);
                activDirectory->files[globalI]->next = activDirectory->files[globalI]->previous = NULL;
              //  File *next;

                File* activ;
                activ = activDirectory->files[globalI];
                char copi;
                strcpy(&copi, activDirectory->files[i]->name);

                char  *fileExt;
                strtok_r(&copi, ".", &fileExt);
                if(isBin(fileExt)){
                    strcpy (data, toBin(data));
                }

                for (int j = 0; j<10; j++){
                    if(data[j] == NULL){
                        for(int index = j; j<0; j--){
                            data[index] = NULL;
                        }
                        break;
                    }

                    activ->data[j] = data[j];
                }

                for(int k = 10 ; k < 1024; k++){
                    if(k%10 == 0){
                        File* next;
                        if (!(next = (File *) calloc (1, sizeof (File)))) {
                            printf ("Error with memory\n");
                        }
                        activ -> next = next;
                        next -> previous = activ;
                        activ = next;
                    }
                    activ -> data[k%10] = data[k];

                    if(data[k+1]==NULL){
                        for(int index = k; k<0; k--){
                            data[index] = NULL;
                        }
                        break;
                    }
                }
                break;
            }
        }
    }


}

char* toBin(char* data) {
    char* result;
    int* bin;
    bin = (int*)malloc(sizeof(int));
   // printf("bin  %i\n",(int) sizeof (bin));
  //  printf("data  %i\n",(int) sizeof (data));
    result = (char*)malloc(sizeof(char));
    //printf("result  %i\n",(int) sizeof (result));
    for (int i = 0; i<(((int) sizeof (data))/2); i++) {
        bin[i] = data[i];
    }
    int v;
    for (int j = 0; j<((int) sizeof (bin))/2; j++) {
        v = 128;
    for (int i = 1; i <= 8; i++) {
        if(bin[j] >= v){
            strcpy(&result[8*j -1 + i],"1");
            bin[j] -= v;
        } else {
            strcpy(&result[8*j -1 + i],"0");
        }
        v /= 2;
    }
}
    return result;
}

void readFile(){
    bool isFileDoesntExist = true;
    for (int i = 0; i < 100; i++) {
        if (activDirectory->files[i] != NULL) {
            if (strcmp (activDirectory->files[i]->name, &fileName) == 0) {
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
            printf ("         %s\n", activDirectory->files[i]->name);
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
            printf ("   <DIR> %s\n", activDirectory->directories[i]->name);
            check = false;
        }
    }

    if (check) {
        printf("Directory list is empty\n");
    }

    showFileList ();
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
      //  free (&end->next);
        end->next = NULL;
    } else {
        printf("Current directory has not parent directory\n");
    }

}
void makeDirectory() {
 //   printf("%s", directoryName);
    for (int i = 0; i < 100; i++) {
        if (activDirectory->directories[i] != NULL) {
            if (strcmp (activDirectory->directories[i]->name, &directoryName) == 0) {
                printf ("Directory with this name is already exist\n");
                return;
            }
        }
    }
    for (int i = 0; i < 100; i++) {
        if (activDirectory->directories[i] == NULL) {
            if (!(activDirectory->directories[i] = (Directory *) calloc (1, sizeof (Directory)))) {
                printf ("Error with memory\n");
            }
            if (!(*(activDirectory->directories[i]->name) = (char) calloc (1, sizeof (char)))) {
                printf ("Error with memory\n");
            }

            strcpy (activDirectory->directories[i]->name, &directoryName);
            return;
            break;

        }
    }
}


 /*   for (int i = 0; i < 100; i++){
        if(activDirectory->directories[i] == NULL){
            if(!(activDirectory->directories[i] = (Directory *)calloc(1, sizeof(Directory)))){
                printf("Error with memory\n");
            }
            strcpy(activDirectory->directories[i]->name,directoryName);
            break;
        } else {
            if (strcmp (activDirectory->directories[i]->name, directoryName) == 0) {
                printf("Directory with this name is already exist\n");
                break;
            }
        }
    }
}
*/
void makeFile() {
    for (int i = 0; i < 100; i++) {
        if (activDirectory->files[i] != NULL) {
            if (strcmp (activDirectory->files[i]->name, &fileName) == 0) {
                printf ("File with this name is already exist\n");
                return;

            }
        } else {
            if (!(activDirectory->files[i] = (File *) calloc (1, sizeof (File)))) {
                printf ("Error with memory\n");
            }
            strcpy (activDirectory->files[i]->name, &fileName);
            return;
        }
    }
}

void removeDirectory() {
    bool check = false;
    for (int i = 0; i < 100; i++) {
        if (activDirectory->directories[i] != NULL) {
            if (strcmp (activDirectory->directories[i]->name, &directoryName) == 0) {
                free (activDirectory->directories[i]);
                activDirectory->directories[i] = NULL;
                check = false;

                break;
            } else {
                check = true;
            }
        }
    }
    if(check){ printf ("Directory with this name is already exist\n");}
}

void removeFile() {
    bool check = false;
    for (int i = 0; i < 100; i++) {
        if (activDirectory->files[i] != NULL) {
            if (strcmp (activDirectory->files[i]->name, &fileName) == 0) {
                free (activDirectory->files[i]);
                activDirectory->files[i] = NULL;
                check = false;
                break;
            } else {
                check = true;
            }
        }
    }
    if(check){ printf ("File with this name is already exist\n");}
}

void openDirectory(){
    bool  check = true;
    for (int i = 0 ; i < 100; i++){
        if(activDirectory->directories[i] != NULL){
            if(strcmp(activDirectory->directories[i]->name, &directoryName) == 0){
                snprintf (path, sizeof path, "%s%s%s", path, separator, &directoryName);
                check = false;
                activDirectory = activDirectory->directories[i];
                Path *current;
                if (!(current = (Path *) calloc (1, sizeof (Path)))) {
                    printf ("Error with memory\n");
                    break;
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
    int typeOfFirstWord = ERROR;
    firstWord = strtok_r(input, " ", &last);
    if(firstWord != NULL){
      //  printf("changeStringToInt");
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

            case DIR: {
                if(isOnlyCommand(last)){
                    return DIR;
                } else return ERROR;
            }

            case SAVE: {
                if(isFileName(last)){
                    return SAVE;
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
    char* cd = "cd";
    char* cd_bak = "cd..";
    char* dir = "dir";
    char* exit = "exit";
    char* save = "save";

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
    } else if (strcmp(operation, save) == 0){
        return SAVE;
    } else if (strcmp(operation, dir) == 0){
        return DIR;
    } else if (strcmp(operation, exit) == 0){
        return EXIT;
    } else return ERROR;
}

void saveDamp(){
    char  *fileExt;
    strtok_r(&fileName, ".", &fileExt);
    if(isBin(fileExt)){
        char third[512];
        snprintf(third, sizeof third, "%s%s", &fileName, ".bin");
        strcpy(FILE_NAME, third);
        writeToFile(*root);
    } else printf("Please check file extension!\n");

}