dnl                                               -*- Autoconf -*-
dnl Processe com o autoconf para produzir o script de configuração  

dnl Versão requerida pelo autoconf
AC_PREREQ([2.69])
AC_INIT(Game, 1.0, renatocordeiroferreira@gmail.com)
: ${CFLAGS="-Iinclude"}

dnl Nosso projeto é escrito em C
AC_LANG(C)

dnl Diretório para os arquivos-fonte
AC_CONFIG_SRCDIR([src])

dnl Checando por programas

AC_PROG_CC
AC_PROG_SED

dnl Checa por bibliotecas
AC_CHECK_LIB([m], sqrt)

dnl Procura pela ferramenta 'make'
AC_PROG_MAKE_SET

dnl Checa pela presença dos headers C padrão
AC_HEADER_STDC

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([sqrt])

AC_CONFIG_FILES([makefile])
AC_OUTPUT
