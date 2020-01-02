//I certify that no unauthorized assistance has been received or given in the completion of this work. Signature: Anna Serenius;

#include <stdio.h>
#include <string.h>

//function to print out string stored in array
void print(char outputString[]) {
int a = 0;
outputString[80] = '\n';
while(a<81){
putchar(outputString[a]);
a++;
}
}


int main(int argc, char ** argv){

int i=0;
char c;
char outputString[81];
char newline = '\n';
char asterisk = '*';
char space = ' ';
char carrot = '^';

c = getchar();


//loop to read in input, make required character substitutions and store string in an array
while(i<80){
if(c == '\n'){
outputString[i] = ' ';
i++;
}else if(c == '*'  && i>0){
        if(outputString[i-1] == '*'){
        outputString[i-1] = '^';
}else{
   outputString[i] = c;
   i++;
}
}else{
outputString[i] = c;
i++;
} 
c = getchar(); 
}
if(i==80 && c!=EOF){
print(outputString);
i=0;
}
if(c == EOF){
return 0;
}

return 0;

}


