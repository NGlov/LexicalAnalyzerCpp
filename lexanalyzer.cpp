#include <iostream>
#include "lex.h"
#include<map>
#include<string>
#include <cctype>
#include <fstream>
#include <cstdlib>
#include <vector>
#ifndef lexanalyzer
#define lexanalyzer
using namespace std;
static map<string, Token> keywords{
    {"program", PROGRAM},
    {"end", END},
    {"else", ELSE},
    {"if", IF},
    {"integer", INTEGER},
    {"real", REAL},
    {"character", CHARACTER},
    {"print", PRINT},
    {"len", LEN},
};
static map<string, Token> symbols{
    {"+", PLUS},
    {"-", MINUS},
    {"*", MULT},
    {"/", DIV},
    {"**", POW},
    {"=", ASSOP},
    {"==", EQ},
    {"<", LTHAN},
    {">", GTHAN},
    {"//", CAT},
    {",", COMMA},
    {"(", LPAREN},
    {")", RPAREN},
    {"::", DCOLON},
    {".", DOT},
    {"*", DEF}
};
static map<string, Token> keyWords
    {
        {"PROGRAM", PROGRAM},
        {"END", END},
        {"ELSE", ELSE},
        {"IF", IF},
        {"INTEGER", INTEGER},
        {"REAL", REAL},
        {"CHARACTER", CHARACTER},
        {"PRINT", PRINT},
        {"LEN", LEN}
    };
static map<Token, string> tokenStrings = {
    {ICONST, "ICONST"},
    {RCONST, "RCONST"},
    {BCONST, "BCONST"},
    {SCONST, "SCONST"},
    {IDENT, "IDENT"},
    {ERR, "ERR"},
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {MULT, "MULT"},
    {DIV, "DIV"},
    {POW, "POW"},
    {ASSOP, "ASSOP"},
    {EQ, "EQ"},
    {LTHAN, "LTHAN"},
    {GTHAN, "GTHAN"},
    {CAT, "CAT"},
    {COMMA, "COMMA"},
    {LPAREN, "LPAREN"},
    {RPAREN, "RPAREN"},
    {DCOLON, "DCOLON"},
    {DOT, "DOT"},
    {DEF, "DEF"},
    {PROGRAM, "PROGRAM"},
    {END, "END"},
    {ELSE, "ELSE"},
    {IF, "IF"},
    {INTEGER, "INTEGER"},
    {REAL, "REAL"},
    {CHARACTER, "CHARACTER"},
    {PRINT, "PRINT"},
    {LEN, "LEN"},
    {THEN, "THEN"}
};

static map<Token, string> delim =
{

    {COMMA, "COMMA"},
    {LPAREN, "LPAREN"},
    {RPAREN, "RPAREN"},
    {DCOLON, "DCOLON"},
    {DOT, "DOT"},
    {DEF, "DEF"}

};

static map<Token, string> op =
{
     {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {MULT, "MULT"},
    {DIV, "DIV"},
    {POW, "POW"},
    {ASSOP, "ASSOP"},
    {EQ, "EQ"},
    {LTHAN, "LTHAN"},
    {GTHAN, "GTHAN"},
    {CAT, "CAT"},
};



static map<string, Token> allTokens
    {
        {"PLUS", PLUS},
        {"MINUS", MINUS},
        {"MULT", MULT},
        {"DIV", DIV},
        {"POW", POW},
        {"ASSOP", ASSOP},
        {"EQ", EQ},
        {"LTHAN", LTHAN},
        {"GTHAN", GTHAN},
        {"CAT", CAT},
        {"COMMA", COMMA},
        {"LPAREN", LPAREN},
        {"RPAREN", RPAREN},
        {"DCOLON", DCOLON},
        {"DOT", DOT},
        {"DEF", DEF}
    };



inline LexItem id_or_kw(const string& lexeme, int linenum)
{

    auto keyword = keyWords.find(lexeme);
 
    string tokenString="";
    Token token = IDENT;

    if(keyword == keyWords.end()) 
    {
        return LexItem(token, lexeme, linenum); 
    }
    else
    {
        Token token = keyword->second; 
        return LexItem(token, lexeme, linenum);
    }



}

inline ostream& operator<<(ostream& out, const LexItem& tok)
{
    if(tok.GetToken() == ICONST || tok.GetToken() == RCONST || tok.GetToken() == BCONST)
    {
        out << tokenStrings[tok.GetToken()] << ": " << '(' << tok.GetLexeme() << ')';
       
    }
    else if(tok.GetToken() == IDENT)
    {
        out << tokenStrings[tok.GetToken()] << ": " << "\'" << tok.GetLexeme() << '\'';
       
    }
    else if(tok.GetToken() == SCONST)
    {
        out << tokenStrings[tok.GetToken()] << ": \"" << tok.GetLexeme() << '\"';
    }
    else if(tok.GetToken() == ERR)
    {
        out << tokenStrings[tok.GetToken()] << ": {" << tok.GetLexeme() << "} " << "Error at line no. " << tok.GetLinenum();
    }
    else if (tok.GetToken() == PROGRAM || tok.GetToken()== END || tok.GetToken()== ELSE || tok.GetToken()== IF || tok.GetToken()== INTEGER || tok.GetToken()== REAL || tok.GetToken()== CHARACTER || tok.GetToken()== PRINT || tok.GetToken()== LEN || tok.GetToken()== THEN)
    {
        out <<  tokenStrings[tok.GetToken()] << ": " << "Keyword";
    }

    else 
    {

        Token tokenValue = tok.GetToken();
        auto it = delim.find(tokenValue);

        if (it != delim.end())
        {
            out << delim[tok.GetToken()] << ": Delimiter Character " << "\"" << tok.GetLexeme() << "\"";
        }
       
        else
        {
            out << op[tok.GetToken()] << ": Operator Symbol " << "\"" << tok.GetLexeme() << "\"";
        }
    }

    return out;
}  
                                 
inline LexItem getNextToken(istream& in, int& linenum){
    enum TokState { START, INID, ININT, INSTRING, INREAL, INCOMMENT} lexstate = START;
    string lexeme;
    char ch;
    ifstream fileName;
    while(in.get(ch))
    {
        lexeme += ch;
        switch( lexstate ) 
        {
            case START:
                if(isspace(ch)){
                    continue;
                    }
                if(ch == '+'){
                    return LexItem(PLUS, lexeme, linenum);
                }
                if(ch == '-'){
                    return LexItem(MINUS, lexeme, linenum);
                }
                if(ch == '*'){
                    if(in.peek() == '*')
                    {
                        in.get(ch);
                        lexeme += ch;
                        return LexItem(POW, lexeme, linenum);
                    }
                    else if(in.peek() == ','){
                    
                        return LexItem(DEF, lexeme, linenum);
                    }
                    
                    return LexItem(MULT, lexeme, linenum);
                }
                if(ch == '/'){
                    if(in.peek() == '/'){
                        in.get(ch);
                        lexeme += ch;
                        return LexItem(CAT, lexeme, linenum);
                    }
                    
                    return LexItem(DIV, lexeme, linenum);
                }
                if(ch == '='){
                    if(in.peek() == '='){
                        in.get(ch);
                        lexeme += ch;
                        return LexItem(EQ, lexeme, linenum);

                    }
                    return LexItem(ASSOP, lexeme, linenum);
                }
                if(ch == '<'){
                    return LexItem(LTHAN, lexeme, linenum);
                }
                if(ch == '>'){
                    return LexItem(GTHAN, lexeme, linenum);
                }
                if(ch == ','){
                    return LexItem(COMMA, lexeme, linenum);
                }
                if(ch == '('){
                    return LexItem(LPAREN, lexeme, linenum);
                }
                if(ch == ')'){
                    return LexItem(RPAREN, lexeme, linenum);
                }
                if(ch == ':'){
                    if(in.peek() == ':'){
                        in.get(ch);
                        lexeme += ch;
                        return LexItem(DCOLON, lexeme, linenum);
                    }
                    
                    return LexItem(ERR, lexeme, linenum);
                }
                if(ch == '.'){
                    return LexItem(DOT, lexeme, linenum);
                }
  
                if(isalpha(ch)){
                        lexstate = INID;
                        break;
                    }
                    if(ch == '\"' || ch == '\''){
                        lexstate = INSTRING;
                        break;
                    }
                    if(isdigit(ch)){
                        lexstate = ININT;
                        break;
                    }
                    if(ch == '!'){
                        in.get(ch);
                        lexstate = INCOMMENT;
                        break;
                    }
                    
                    break;
                    case ININT:
                    if(isdigit(ch)){
                        continue;
                                    }
                                    else if(ch == '.'){
                                        lexstate = INREAL;
                                        break;
                                    }
                                    else{
                                        lexstate = START;
                                        return LexItem(ICONST, lexeme, linenum);
                                    }

                                    break;
                                    //Other states will follow

                                    case INSTRING:
                                    lexeme += ch;
                                    if(ch == '\"' || ch == '\''){
                                        if(ch == lexeme.front()){
                                            return LexItem(SCONST, lexeme, linenum);
                                        }
                                        else{
                                            return LexItem(ERR, lexeme, linenum);
                                        }
                                    }
                                    break; 
                                    case INREAL:
                                    if(isdigit(ch)){
                                        lexeme += ch;
                                    }
                                    else if(ch == '.'){
                                        lexstate = START;
                                        lexeme += ch;
                                        return LexItem(ERR, lexeme, linenum);
                                    }
                                    else{
                                        lexstate = START;
                                        return LexItem(RCONST, lexeme, linenum);
                                    }
                                    break;
                                    case INID:
                                    if(ch == '_' || isdigit(ch) || isalpha(ch) || ch == '$'){
                                        lexeme += ch;
                                        return LexItem(IDENT, lexeme, linenum);
                                    }
                                    else{
                                        in.putback(ch);
                                        lexstate = START;
                                        lexeme += ch;
                                    }
                                    case INCOMMENT:
                                        if(ch == '\n'){
                                        linenum++;
                                    }
                                    break;
                                    }
                                    return LexItem(ERR, "ERROR", linenum);
                                    }
                                    return LexItem(DONE, "", linenum);
    #endif
                                    }
                                    