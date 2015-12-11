#!/bin/bash

if [ $# != 1 ];then
    echo "ERROR: too few argument."
    exit 1;
fi

if [ ! -f $1 ];then
    echo "ERROR: No this file."
    exit 1;
fi

bisonVer=`bison -V|head -1|sed 's/^bison (GNU Bison) \(...\).*$/\1/'`

if [ "$bisonVer" == "2.5" ];then
    echo "OK"
    exit 0
fi

cat $1|sed 's/int yyparse (void);//'|sed '/| yyparse.  |/,/\/\* The lookahead symbol.  \*\//s/^/%%%/'|sed 's/%%%\/\* The lookahead symbol.  \*\//\/\* WE WANT TO FIX YYPARSE.  \*\//'|sed '/^%%%/d'|sed 's/\/\* WE WANT TO FIX YYPARSE.  \*\//| yyparse.  |\n`----------\*\/\n\n#ifdef YYPARSE_PARAM\n#if (defined __STDC__ || defined __C99__FUNC__ \\\n     || defined __cpluscplus || defined _MSC_VER)\nint\nyyparse (void \*YYPARSE_PARAM)\n#else\nint\nyyparse (YYPARSE_PARAM)\n    void *YYPARSE_PARAM;\n#endif\n#else \/\* ! YYPARSE_PARAM \*\/\n#if (defined __STDC__ || defined __C99__FUNC__ \\\n     || defined __cplusplus || defined _MSC_VER)\nint\nyyparse (void)\n#else\nint\nyyparse ()\n\n#endif\n#endif\n{\n\/\* The lookahead symbol.  \*\//' | sed 's/yychar = yylex (\&yylval);/yychar = YYLEX;/'| sed 's/\(\/\* Enable debugging if requested.  \*\/\)/#ifdef YYLEX_PARAM\n# define YYLEX yylex (\&yylval, YYLEX_PARAM)\n#else\n# define YYLEX yylex (\&yylval)\n#endif\n\1/'> tempFixBisonFile
mv tempFixBisonFile $1