#include <stdio.h>

int main(){

    int x=5;
    int y=0;
    char op=0;

   // while(1){
        printf("%s","Hello World!\n");
        printf("%d",x);

        scanf("%d",&x);
        printf("\n%d",x);

        scanf("%d",&y);
        printf("\n%d",y);

        printf("%s","Indlæs operationen +-*/");
        scanf("%c",&op);

        if(op=='+'){
            printf("Sum er: %d",(x+y));
        }

        printf(" x er: %d y er: %d",x,y);
   //}
}