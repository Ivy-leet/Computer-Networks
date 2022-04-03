#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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



void setHttpHeader(char*);
void response(struct sockaddr_in* );
void getCalculator(char*, int, char*);
char* getHeader(int number);
void response(struct sockaddr_in* );
// void response(struct sockaddr_in* );

void calculate(char*);

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
    /*
    char input[50];
    printf("Please enter expression: ");
    scanf("%s", input);

    calculate(input);
    */
    
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen=sizeof(address);

    //char* hello="HTTP/1.1 200 OK\nContent-Type: text/html;charser=UTF-8\nContent-Length: 500\n\n<?xml>\n<!DOCTYPE hmtl>\n<html>\n<head>\n<h1>Test</h1></head>\n<body>hee</body>\n</html>";

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
    char tes[3000];
    char ans[1000];

    while (1) {
        printf("Waiting for new connection...\n\n");

        // Waiting for upcoming client
        if ((new_socket=accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }


        char buffer[1000]={0};
        valread=read(new_socket, buffer, 1000);
        printf("%s\n", buffer);
        if(buffer[5]!='f'){
            ans[i]=buffer[5];
            i++;
        }

        if(buffer[5]=='C'){
            memset(ans, 0, strlen(ans));
            i=0;
        }

        getCalculator(tes, new_socket, ans);
        write(new_socket, tes, strlen(tes));

        printf("----------Hello message sent-------------\n");
        close(new_socket);
    }
    
    
    return 0;
}


void setHttpHeader(char httpRequestHeader[]) {
    char httpResponseHeader[2000]="HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";

    char buttonPressed=httpRequestHeader[4];
}

/*char* toArr(int number){
    int n = log10(number)+1;
    int i;
    char* numArr= callol(n, sizeof(char));
    for(i = n-1; i >= 0; --i, number /= 10){
        numArr[i] = (number % 10)+'0';
    }
    return numArr;
}*/


void getCalculator(char* header, int s, char* ans){
    char* head = "HTTP/1.1 200 OK\nContent-Type: text/html;charser=UTF-8\nContent-Length: 3000\n\n<?xml>\n<!DOCTYPE hmtl>\n<html>\n<head><style>body{\n\tbackground-color: #a9bd7e;\n}\ntable{\n\tmargin: auto;\nbackground-color: #9dd2ea;\n\twidth: 295px;\n\theight: 325px;\n\ttext-align: center;\n\tborder-radius: 4px;\n}\n\tth{\n\tleft : 5px;\n\ttop: 5px;\nt color: #495069;\n\twidth: 60px;\n\theight: 50px;\n\tmargin: 5px;\n\tfont-size: 20px;}\ntable, th, tr{\n\tborder: 3px solid #a9bd7e;\nborder-collapse: collapse;\n\tcolor: white;\n}\na:link, a:visited{\n\tcolor: white;\n\ttext-decoration: none;\n\ttext-align: center;\n\tpadding: 20px 20px;\n}</style></head>\n<body><table style='width:100%'><tr><th colspan='4'><h1>";
    char* body = "</h1></th></tr><tr><th><a href='1'>1</a></th><th><a href='2'>2</a></th><th><a href='3'>3</a></th><th><a href='+'>+</a></th></tr><tr><th><a href='4'>4</a></th><th><a href='5'>5</a></th><th><a href='6'>6</a></th><th><a href='-'>-</a></th></tr><tr><th><a href='7'>7</a></th><th><a href='8'>8</a></th><th><a href='9'>9</a></th><th><a href='*'>*</a></th></tr><tr><th><a href='C'>C</a></th><th><a href='0'>0</a></th><th><a href='='>=</a></th><th><a href='~'>/</a></th></tr></table></body>\n</html>";
    memset(header, 0, strlen(header));
    strcat(header, head);
    strcat(header, ans);
    strcat(header, body);
    //printf(header);
}

/*
char* 9etHeader(int len){
/  hello =
    char strl[11];
    sprintf(strl, "%ld", len);
    strncat(ret, strl, strlen(strl))X
    printf(ret);
    retu1n ret;
}
*/

void calculate(char* input) {
    // Stack to store operators
    struct Stack* operatorStack=stack_init();

    // Stack to store operands
    struct OperandStack* operandStack=operandStack_init();

    char postfix[50]="";
    char dol='$'; // To represent end of number
    int count=0;
    
    for (int i=0; i < strlen(input); i++) {
    for (int i=0; i<strlen(input); i++) {
    /**
     * Converting input to postfix
    */
    for (int i=0; i<strlen(input); i++) {
>>>>>>> refs/remotes/origin/main
        char op=input[i];

        switch (op)
        {
            case '+':
            {
                if (count>0) {
                    strncat(postfix, &dol, 1);
                    count=0;
                }

                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    char onTop=top(operatorStack);
                    while ((onTop=='X' || onTop=='/' || onTop=='+' || onTop=='-') && operatorStack->size>0) {
                        pop(operatorStack);
                        strncat(postfix, &onTop, 1);
                        
                        onTop=top(operatorStack);
                    }

                    push(operatorStack, op);
                }
                break;
            }
            case '-':
            {
                if (count>0) {
                    strncat(postfix, &dol, 1);
                    count=0;
                }

                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    char onTop=top(operatorStack);
                    while ((onTop=='X' || onTop=='/' || onTop=='+' || onTop=='-') && operatorStack->size>0) {
                        pop(operatorStack);
                        strncat(postfix, &onTop, 1);
                        
                        onTop=top(operatorStack);
                    }

                    push(operatorStack, op);
                }
                break;
            }
            case 'X':
            {
                if (count>0) {
                    strncat(postfix, &dol, 1);
                    count=0;
                }

                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    char onTop=top(operatorStack);

                    while ((onTop==')' | onTop=='(' || onTop=='X' || onTop=='/') && operatorStack->size>0)
                    {
                        pop(operatorStack);
                        strncat(postfix, &onTop, 1);
                        
                        onTop=top(operatorStack);
                    }
                    
                    push(operatorStack, op);
                }
                break;
            }
            case '/':
            {
                if (count>0) {
                    strncat(postfix, &dol, 1);
                    count=0;
                }
                    
                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    char onTop=top(operatorStack);

                    while ((onTop==')' | onTop=='(' || onTop=='X' || onTop=='/') && operatorStack->size>0)
                    {
                        pop(operatorStack);
                        strncat(postfix, &onTop, 1);
                        onTop=top(operatorStack);
                    }
                    
                    push(operatorStack, op);
                }
                break;
            }
                
            
            case '(':
            {
                if (count>0) {
                    push(operatorStack, '$');
                    count=0;
                }

                push(operatorStack, op);

                break;
            }
            case ')':
            {
                if (count>0) {
                    push(operatorStack, '$');
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
                break;
            }
            
            default:
            {
                count++;
                strncat(postfix, &op, 1);
                break;
            }
                
        }
    }

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

                double num=subtraction(x,y);

                pushO(operandStack, num);

                break;
            }
                
            
            case 'X':
            {
                double x,y;

                x=topO(operandStack);
                popO(operandStack);
                y=topO(operandStack);
                popO(operandStack);

                double num=multiply(x,y);

                pushO(operandStack, num);

                break;
            }
                
            
            case '/':
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
            

            default:
            {
                strncat(numberAsString, &op, 1);
                break;
            }
                
        }
    }
    
    double num=topO(operandStack); // result of expression

    // printf("%f \n", num);
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
