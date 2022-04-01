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

void setHttpHeader(char*);
// void response(struct sockaddr_in* );

void calculate(char*);

int addition(int, int);
int subtraction(int, int);
int multiply(int, int);
double divide(int, int);
int power(int, int);

struct Stack* stack_init();
char top(struct Stack*);
void push(struct Stack*, char);
char pop(struct Stack*);

int main(int argc, char const *argv[])
{
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

    char* hello="HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

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
        write(new_socket, hello, strlen(hello));

        printf("----------Hello message sent-------------\n");
        close(new_socket);
    }
    
    return 0;
}


void setHttpHeader(char httpRequestHeader[]) {
    char httpResponseHeader[2000]="HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";

    char buttonPressed=httpRequestHeader[4];
}

void calculate(char* input) {
    struct Stack* operatorStack=stack_init();
    char postfix[50]="";
    stack_init(operatorStack);
    
    for (int i=0; i<strlen(input); i++) {
        char op=input[i];

        switch (op)
        {
            case '+':
            {
                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    push(operatorStack, op);
                }
                break;
            }
            case '-':
            {
                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    push(operatorStack, op);
                }
                break;
            }
            case 'X':
            {
                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    char onTop=top(operatorStack);
                    if (onTop=='X' || onTop=='/' || onTop=='+' || onTop=='-') {
                        pop(operatorStack);
                        strncat(postfix, &onTop, 1);
                        push(operatorStack, op);
                    }
                    else {
                        push(operatorStack, op);
                    }
                }
                break;
            }
            case '/':
            {
                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    char onTop=top(operatorStack);
                    if (onTop=='X' || onTop=='/' || onTop=='+' || onTop=='-') {
                        pop(operatorStack);
                        strncat(postfix, &onTop, 1);
                        push(operatorStack, op);
                    }
                    else {
                        push(operatorStack, op);
                    }
                }
                break;
            }
                
            
            case '(':
            {
                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    push(operatorStack, op);
                }
                break;
            }
            case ')':
            {
                if (operatorStack->size==0)
                    push(operatorStack, op);
                else {
                    char onTop=top(operatorStack);
                    while (onTop!='(')
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
                strncat(postfix, &op, 1);
                break;
            }
                
        }
    }

    char onTop=top(operatorStack);
    pop(operatorStack);
    strncat(postfix, &onTop, 1);

    printf("%s \n", postfix);

    struct Stack* operandStack=stack_init();

    for (int i=0; i<strlen(postfix); i++) {
        char op=input[i];

        switch (op)
        {
            case '+':
            {
                int x,y;

                char xChar=top(operandStack);
                pop(operandStack);
                char yChar=top(operandStack);
                pop(operandStack);

                sscanf(xChar, "%d", &x);
                sscanf(yChar, "%d", &y);

                int sum=addition(x, y);
                
                break;
            }
                
            case '-':
                break;
            
            case 'X':
                break;
            
            case '/':
                break;

            default:
                break;
        }
    }
}

int addition(int x, int y) {
    return x+y;
}

int subtraction(int x, int y) {
    return x-y;
}

int multiply(int x, int y) {
    return x*y;
}

double divide(int x, int y) {
    return (double)(x/y);
}

// int power(int x, int y) {
//     return pow(x,y);
// }


struct Stack* stack_init() {
    struct Stack* s=(struct Stack*)malloc(sizeof(struct Stack));
    
    s->size=0;

    return s;
}

char top(struct Stack* s) {
    if (s->size==0) {
        fprintf(stderr, "Error: stack empty\n");
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

char pop(struct Stack* s) {
    if (s->size==0)
        fprintf(stderr, "Error: stack empty\n");
    else
        return s->stack[s->size--];
        // s->size--;
}