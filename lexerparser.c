#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum{
KEYWORD,      
IDENTIFIER,   
NUMBER,       
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

//GLOBAL defined file pointer and currtoken
FILE* fp;
Token currentToken;

//LEXER PART
Token getNextToken()
{
  Token token;
  char ch;
  while((ch=fgetc(fp))!=EOF)
  {
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
      return token;
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
      return token;
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
      return token;
    }
    
  }
   //if none of the above cases return
   token.type=EOF_TOKEN;
   strcpy(token.value,"EOF");
   return token;
}

void match(TokenType expected)
{
  if(currentToken.type=expected)
  {
    currentToken=getNextToken();
  }
  else
  {
    printf("Syntax error: Expected %d, got %d\n",expected,currentToken.type);
    exit(1);
  }
}


void parseTerm()
{
  if(currentToken.type==NUMBER)
  {
    match(NUMBER);
  }
  else if(currentToken.type==IDENTIFIER)
  {
    match(IDENTIFIER);
  }
  else
  {
    printf("Syntax Error in term %s\n",currentToken.value);
    exit(1);
  }
}

void parseExpression()
{
  parseTerm();
  if(currentToken.type==PLUS)
  {
    match(PLUS);
    parseTerm();
  }
}

int main()
{
    fp=fopen("input.txt","r");
    if(!fp)
    {  
      printf("Error opening file\n");
      return 1;
    }
    
    currentToken=getNextToken();
    
    parseExpression();
    
    if(currentToken.type=!EOF_TOKEN)
    {
      printf("UNEXPECTED TOKEN: got %s\n",currentToken.value);
      return 1;
    }
    
    printf("Parsing done\n");
    fclose(fp);
    return 0;
}
