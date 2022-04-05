/*
* 20575085, Koma T(Tlholo)
* 20456078, Dzimati BM(Malcolm)
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5555
#define STACK_MAX 100

struct Stack
{
    char stack[STACK_MAX];
    int size;
};

struct OperandStack
{
    double stack[STACK_MAX];
    int size;
};

void getCalculator(char*, int, char*, bool);
char* getHeader(int number);
char* getCurrentDate();
char* getLastModifiedDate();
char* getContentLength();

char* calculate(char*);

double addition(double, double);
double subtraction(double, double);
double multiply(double, double);
double divide(double, double);
double power(double, double);

struct Stack* stack_init();
char top(struct Stack*);
void push(struct Stack*, char);
void pop(struct Stack*);

struct OperandStack* operandStack_init();
double topO(struct OperandStack*);
void pushO(struct OperandStack*, double);
void popO(struct OperandStack*);

#include <string.h>

int main(int argc, char const *argv[])
{
    // Uncomment code to test calculator in terminal
    // Note: comment out the socket server parts
    //
    /*
    char input[50];
    printf("Please enter expression: ");
    scanf("%s", input);

    char* answer=calculate(input);
    printf("%s\n", answer);
    */
    
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen=sizeof(address);

    //Creating socket
    if ((server_fd=socket(AF_INET, SOCK_STREAM, 0))==0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);

    memset(address.sin_zero, '\0', sizeof(address.sin_zero));

    // Connect to remote server
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    // Adding listener
    if (listen(server_fd, 10)<0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    char header[3000];
    char ans[1000];

    while (1) {
        bool equal = false;
        printf("Waiting for new connection...\n\n");

        // Waiting for upcoming client
        if ((new_socket=accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }


        char buffer[1000]={0};
        valread=read(new_socket, buffer, 1000);
        printf("%s\n", buffer);

        if(ans[0]=='E'){
            memset(ans, 0, strlen(ans));
            i=0;
       }

        if(buffer[5]=='C'){
            memset(ans, 0, strlen(ans));
            i=0;
        }
        else if(buffer[5]=='='){
            char last = ans[strlen(ans)-1];
            if(last=='*' || last=='-' || last=='~' || last=='+' || last=='('){
                equal = true;
	    }else{
                char* an = calculate(ans);
                memset(ans, 0, strlen(ans));

                for(i = 0; i < strlen(an); i++){
                    ans[i] = an[i];
                }
            }
        }
        else if(buffer[5]=='B'){
            ans[strlen(ans)-1] = '\0';
            i--;
        }
        else if(buffer[5]!='f'){
            ans[i]=buffer[5];
            i++;
        }

        getCalculator(header, new_socket, ans, equal);
        write(new_socket, header, strlen(header));
        close(new_socket);
    }
    
    return 0;
}

void getCalculator(char* header, int s, char* ans, bool equal){
    char* head = "HTTP/1.1 200 OK\nConnection: keep-alive\nContent-Type: text/html;charser=UTF-8\nCache-Control: max-age=604800\n";
    char* error = "HTTP/1.1 400 Bad Request\nConnection: keep-alive\nContent-Type: text/html;charser=UTF-8\nCache-Control: max-age=604800\n";
    int last = strlen(ans)-1;
    printf(&ans[last]);

    if(ans[0]=='i' || ans[1]=='i' || ans[0]=='~' || ans[0]=='*'){
        //strcpy(head, error);

	memset(ans, 0, strlen(ans));
        strcat(ans, "Error");
    }

    if(equal){
        memset(ans, 0, strlen(ans));
        strcat(ans, "Error");
    }

    char* date=getCurrentDate();
    char* lastModified=getLastModifiedDate();
    char* server="Server: Maverick\n";
    char* title="\n\n<?xml>\n<!DOCTYPE hmtl>\n<html>\n<head><style>body{\n\tbackground-color: #a9bd7e;\n}\ntable{\n\tmargin: auto;\nbackground-color: #9dd2ea;\n\twidth: 295px;\n\theight: 325px;\n\ttext-align: center;\n\tborder-radius: 4px;\n}\n\tth{\n\tleft : 5px;\n\ttop: 5px;\nt color: #495069;\n\twidth: 60px;\n\theight: 50px;\n\tmargin: 5px;\n\tfont-size: 20px;}\ntable, th, tr{\n\tborder: 3px solid #a9bd7e;\nborder-collapse: collapse;\n\tcolor: white;\n}\na:link, a:visited{\n\tcolor: white;\n\ttext-decoration: none;\n\ttext-align: center;\n\tpadding: 20px 20px;\n}</style></head>\n<body><table style='width:100%'><tr><th colspan='4'><h1>";
    char* body = "</h1></th></tr><tr><th><a href = 'B'>Back</a></th><th><a href = '('>(</a></th><th><a href = ')'>)</a></th><th><a href = '~'>/</a></th></tr><tr><th><a href='1'>1</a></th><th><a href='2'>2</a></th><th><a href='3'>3</a></th><th><a href='+'>+</a></th></tr><tr><th><a href='4'>4</a></th><th><a href='5'>5</a></th><th><a href='6'>6</a></th><th><a href='-'>-</a></th></tr><tr><th><a href='7'>7</a></th><th><a href='8'>8</a></th><th><a href='9'>9</a></th><th><a href='*'>*</a></th></tr><tr><th><a href ='C'>Clear</a></th><th><a href = '0'>0</a></th><th colspan='2'><a href='='>=</a></th></tr></table></body>\n</html>";

    char* contentLength=getContentLength(title, ans, body);

    memset(header, 0, strlen(header));

    if(ans[0]=='E'){
        strcat(header, error);
        //printf(header);
    }else{
        strcat(header, head);
    }

    strcat(header, server);
    strcat(header, date);
    strcat(header, contentLength);
    strcat(header, lastModified);
    strcat(header, title);
    strcat(header, ans);
    strcat(header, body);
    //printf(header);
}

char* calculate(char* input) {
    // Stack to store operators
    struct Stack* operatorStack=stack_init();

    // Stack to store operands
    struct OperandStack* operandStack=operandStack_init();

    char postfix[50]="";
    char dol='$'; // To represent end of number
    char neg='n', pos='p'; // To represent signed numbers
    int count=0;
    bool operatorFirst=false;
    char forNegation[2]="1$";
    char multiplyChar='*';
    
    /**
     * Converting input to postfix
    */
    for (int i=0; i<strlen(input); i++) {
        char op=input[i];

        switch (op)
        {
            case '+':
            {
                if (count>0) {
                    strncat(postfix, &dol, 1);
                    count=0;
                }
                if (i==0 || operatorFirst==true)
                {
                    strncat(postfix, &pos, 1);

                }
                    
                else if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    char onTop=top(operatorStack);
                    while ((onTop=='*' || onTop=='~' || onTop=='+' || onTop=='-') && operatorStack->size>0) {
                        pop(operatorStack);
                        strncat(postfix, &onTop, 1);
                        
                        onTop=top(operatorStack);
                    }

                    push(operatorStack, op);
                }
                operatorFirst=true;
                break;
            }
            case '-':
            {
                if (count>0) {
                    strncat(postfix, &dol, 1);
                    count=0;
                }

                if (i==0 || operatorFirst==true)
                {
                    strncat(postfix, &neg, 1);
                }
                    
                else if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    char onTop=top(operatorStack);
                    while ((onTop=='*' || onTop=='~' || onTop=='+' || onTop=='-') && operatorStack->size>0) {
                        pop(operatorStack);
                        strncat(postfix, &onTop, 1);
                        
                        onTop=top(operatorStack);
                    }

                    push(operatorStack, op);
                }
                operatorFirst=true;
                break;
            }
            case '*':
            {
                if (count>0) {
                    strncat(postfix, &dol, 1);
                    count=0;
                }

                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    char onTop=top(operatorStack);

                    while ((onTop==')' | onTop=='(' || onTop=='*' || onTop=='~') && operatorStack->size>0)
                    {
                        pop(operatorStack);
                        strncat(postfix, &onTop, 1);
                        
                        onTop=top(operatorStack);
                    }
                    
                    push(operatorStack, op);
                }
                operatorFirst=true;
                break;
            }
            case '~':
            {
                if (count>0) {
                    strncat(postfix, &dol, 1);
                    count=0;
                }
                    
                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    char onTop=top(operatorStack);

                    while ((onTop==')' | onTop=='(' || onTop=='*' || onTop=='~') && operatorStack->size>0)
                    {
                        pop(operatorStack);
                        strncat(postfix, &onTop, 1);
                        onTop=top(operatorStack);
                    }
                    
                    push(operatorStack, op);
                }
                operatorFirst=true;
                break;
            }
            case '(':
            {
                if (count>0) {
                    strncat(postfix, &dol, 1);
                    count=0;
                }

                if (postfix[strlen(postfix)-1]=='n')
                {
                    strcat(postfix, forNegation);
                    postfix[strlen(postfix)-1]='\0';
                    push(operatorStack, multiplyChar);
                    operatorFirst=false;
                    push(operatorStack, op);
                }
                else {
                    push(operatorStack, op);

                    operatorFirst=true;
                }
                
                break;
            }
            case ')':
            {
                if (count>0) {
                    strncat(postfix, &dol, 1);
                    count=0;
                }

                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    char onTop=top(operatorStack);
                    while (onTop!='(' && operatorStack->size>0)
                    {
                        pop(operatorStack);
                        strncat(postfix, &onTop, 1);
                        onTop=top(operatorStack);
                    }

                    pop(operatorStack);
                }

                operatorFirst=true;
                break;
            }
            default:
            {
                count++;
                strncat(postfix, &op, 1);
                operatorFirst=false;
                break;
            }
                
        }
    }

    if (input[strlen(input)-1]!=')' && input[strlen(input)-1]!='(' && input[strlen(input)-1]!='*' && input[strlen(input)-1]!='~' && input[strlen(input)-1]!='+' && input[strlen(input)-1]!='-')
        strncat(postfix, &dol, 1);

    char onTop=top(operatorStack);

    while (operatorStack->size!=0)
    {
        pop(operatorStack);
        strncat(postfix, &onTop, 1);
        onTop=top(operatorStack);
    }
    
    // printf("%s \n", postfix);

    char numberAsString[50]="";

    neg='-';
    pos='+';

    /**
     * Perform operations on expression entered
    */
    for (int i=0; i<strlen(postfix); i++) {
        char op=postfix[i];

        switch (op)
        {
            case '$':
            {
                double num=atof(numberAsString);

                pushO(operandStack ,num);

                memset(numberAsString, 0, sizeof numberAsString);

                break;
            }
            case '+':
            {
                double x,y;

                x=topO(operandStack);
                popO(operandStack);
                y=topO(operandStack);
                popO(operandStack);
                
                double num=addition(x, y);

                pushO(operandStack, num);
                
                break;
            }
            case '-':
            {
                double x,y;

                x=topO(operandStack);
                popO(operandStack);
                y=topO(operandStack);
                popO(operandStack);

                double num=subtraction(y,x);

                pushO(operandStack, num);

                break;
            }
            case '*':
            {
                double x,y;

                x=topO(operandStack);
                popO(operandStack);
                y=topO(operandStack);
                popO(operandStack);
                // printf("x: %f y: %f\n", x, y);
                double num=multiply(x,y);

                pushO(operandStack, num);

                break;
            }
            case '~':
            {
                double x,y;

                x=topO(operandStack);
                popO(operandStack);
                y=topO(operandStack);
                popO(operandStack);

                double num=divide(y,x);

                pushO(operandStack, num);

                break;
            }
            case 'n':
            {
                strncat(numberAsString, &neg, 1);
                break;
            }
            case 'p':
            {
                strncat(numberAsString, &pos, 1);
                break;
            }
            default:
            {
                strncat(numberAsString, &op, 1);
                break;
            }

        }
    }
    
    double num=topO(operandStack); // result of expression
    char static answer[100];

    snprintf(answer, 100, "%f", num);

    return answer;
}

char* getCurrentDate() {
    char static dateField[100]="";
    time_t rawtime=time(NULL);

    struct tm *ptm=localtime(&rawtime);
    strftime(dateField, 100, "Date: %a, %d %b %Y %H:%M:%S GMT\n", ptm);

    return dateField;
}

char* getLastModifiedDate() {
    char static lastModifiedField[100]="";
    struct stat filestat;

    stat("server.c", &filestat);

    struct tm *ftm=localtime(&filestat.st_mtime);

    strftime(lastModifiedField, 100, "Last-Modiefied: %a, %d %b %Y %H:%M:%S GMT\n", ftm);

    return lastModifiedField;
}

char* getContentLength(char* x, char* y, char* z) {
    char static buffer[100]="";
    int contentLength=strlen(x)+strlen(y)+strlen(z);
    snprintf(buffer, 100, "Content-Length: %d\n", contentLength);

    return buffer;

}

double addition(double x, double y) {
    return x+y;
}

double subtraction(double x, double y) {
    return x-y;
}

double multiply(double x, double y) {
    return x*y;
}

double divide(double x, double y) {
    return (double)(x/y);
}

struct Stack* stack_init() {
    struct Stack* s=(struct Stack*)malloc(sizeof(struct Stack));
    
    s->size=0;

    return s;
}

char top(struct Stack* s) {
    if (s->size==0) {
        return -1;
    }

    return s->stack[s->size-1];
}

void push(struct Stack* s, char op) {
    if (s->size<STACK_MAX)
        s->stack[s->size++]=op;
    else
        fprintf(stderr, "Error: stack full\n");
}

void pop(struct Stack* s) {
    if (s->size==0)
        fprintf(stderr, "Error: stack empty\n");
    else
        s->size--;
}

struct OperandStack* operandStack_init() {
    struct OperandStack* s=(struct OperandStack*)malloc(sizeof(struct OperandStack));
    
    s->size=0;

    return s;
}

double topO(struct OperandStack* s) {
    if (s->size==0) {
        fprintf(stderr, "Error: stack empty\n");
        return -1;
    }

    return s->stack[s->size-1];
}

void pushO(struct OperandStack* s, double op) {
    if (s->size<STACK_MAX)
        s->stack[s->size++]=op;
    else
        fprintf(stderr, "Error: stack full\n");
}

void popO(struct OperandStack* s) {
    if (s->size==0)
        fprintf(stderr, "Error: stack empty\n");
    else
        s->size--;
}
