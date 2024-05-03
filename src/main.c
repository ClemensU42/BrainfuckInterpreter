#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 1024*32

int main(int argc, char** argv){

    if(argc != 2){
        printf("Not given the correct arguments!");
        return -1;
    }    

    FILE* infile;
    char* buffer;
    long numbytes;


    infile = fopen(argv[1], "r");

    if(infile == NULL){
        printf("file %s cannot be opened!", argv[1]);
        return -1;
    }

    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);
    fseek(infile, 0L, SEEK_SET);

    buffer = (char*)calloc(numbytes, sizeof(char));   
    if(buffer == NULL){
        printf("Couldn't allocate memory!");
        return -1;
    } 

    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);


    char data[DATA_SIZE];//(char*)calloc(1024 * 32, sizeof(char)); //allocate 32kb of data space

    unsigned long instructionPointer = 0;
    unsigned long dataPointer = 0;
    clock_t start, end;
    
    start = clock();
    while(instructionPointer <= numbytes){
        switch (buffer[instructionPointer]) {
            case '>':
                dataPointer++;
                if(dataPointer >= DATA_SIZE){
                    printf("data out of bounds! too high!\nPosition: %lu\n", dataPointer);
                    goto end;
                }
                break;
            case '<':
                dataPointer--;
                if(dataPointer < 0){
                    printf("data out of bounds! too low!\nPosition: %lu\n", dataPointer);
                    goto end;
                }
                break;
            case '+':
                data[dataPointer]++;
                break;
            case '-':
                data[dataPointer]--;
                break;
            case '[':
                if(data[dataPointer] == 0){
                    unsigned long startPos = instructionPointer;
                    int depth = 1;
                    while(1){
                        instructionPointer++;
                        if(instructionPointer >= numbytes){
                            printf("instruction Pointer out of bounds! too high!\n Position: %lu\n", startPos);
                            goto end;
                        }
                        char currentInstr = buffer[instructionPointer];
                        if(currentInstr == ']'){
                            if(--depth == 0)
                                break;
                        }
                        else if(currentInstr == '['){
                            depth++;
                        }

                    }  
                }
                break;
            case ']':
                if(data[dataPointer] != 0){
                    unsigned long startPos = instructionPointer;
                    int depth = 1;
                    while(1){
                        instructionPointer--;
                        if(instructionPointer < 0){
                            printf("instruction Pointer out of bounds! too low!\n Position: %lu\n", startPos);
                            goto end;
                        }
                        char currentInstr = buffer[instructionPointer];
                        if(currentInstr == '['){
                            if(--depth == 0)
                                break;
                        }
                        else if(currentInstr == ']'){
                            depth++;
                        }

                    }  
                }
                break;
            case '.':
                printf("%c", data[dataPointer]);
                break;
            case ',':
                break;
            default:
                break;
        }
        instructionPointer++;
    }
    end = clock();
    double cpuTimeUsed = ((double) (end - start) / CLOCKS_PER_SEC);

    printf("\n\nProgram took %f seconds to complete\n", cpuTimeUsed);

    //free(data);
end:
    free(buffer);
    return 0;
}