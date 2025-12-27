#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef enum{
KEYWORD,      //0
IDENTIFIER,   //1
NUMBER,       //2
ASSIGN,
PLUS,
SEMICOLON,
EOF_TOKEN
}TokenType;

typedef struct{
TokenType type;
char value[32];
}Token;

int isKeyword(char* str)
{
  return strcmp(str,"int")==0||strcmp(str,"print")==0;
}

void printToken(Token t)
{
  const char* names[]={"KEYWORD","IDENTIFIER","NUMBER","ASSIGN","PLUS","SEMICOLON","EOF"};
  printf("(%s,%s)\n",names[t.type],t.value);
}

int main()
{
  FILE* fp= fopen("input.txt","r");
  if(!fp)
  {
    printf("Cannot open file.\n");
    return 1;
  }
  char ch;
  while((ch=fgetc(fp))!=EOF)
  {
     Token token;
    //skip whitespace 
    if(isspace(ch))
    continue;
    
    //Identifier vs Keyword
    if(isalpha(ch))
    {
      int i=0;
      token.value[i++]=ch;
      while(isalnum(ch=fgetc(fp)))
      {
        token.value[i++]=ch;
      }
      token.value[i]='\0';
      ungetc(ch,fp);
      
      token.type=isKeyword(token.value)?KEYWORD:IDENTIFIER;
      printToken(token);
    }
    
    //if digit
    else if(isdigit(ch))
    {
      int i=0;
      token.value[i++]=ch;
      while(isdigit(ch=fgetc(fp)))
      {
        token.value[i++]=ch;
      }
      token.value[i]='\0';
      ungetc(ch,fp);
      
      token.type=NUMBER;
      printToken(token);
    }
    
    //if OPERATORS AND SYMBOL
    
    else
    {
      token.value[0]=ch;
      token.value[1]='\0';
      
      switch(ch)
      {
        case '=':
        token.type=ASSIGN;
        break;
        
        case '+':
        token.type=PLUS;
        break;
        
        case ';':
        token.type=SEMICOLON;
        break;
        
        default:
        printf("UNKNOWN CHARACTER\n");
        continue;
      }
      printToken(token);
    }
  }
  fclose(fp);
  return 0;
}
        


