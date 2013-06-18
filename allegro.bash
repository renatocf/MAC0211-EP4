#!/bin/bash
# 
# Instala o allegro no Ubuntu. Para tanto, 
# bash e apt-get são necessários.

DIR=$(pwd)

# Recebe pacote do allegro 5.0.9
if ! [ -e ${DIR}/download ]; then
    sudo apt-get install wget
    wget http://sourceforge.net/projects/alleg/files/allegro/5.0.9/allegro-5.0.9.tar.gz/download
    # Checa erro no recebimento
    if [ $? -ne 0 ]; then
        echo "Pacote não disponível no link. Verifique se
        http://sourceforge.net/projects/alleg/files/allegro/5.0.9/allegro-5.0.9.tar.gz/download está disponível" 
        exit -1
    fi
fi

if ! [ -e ${DIR}/allegro-5.0.9.tar.gz ]; then
    # Muda nome do dowload para .tar.gz
    mv ${DIR}/download allegro-5.0.9.tar.gz
fi

if ! [ -d ${DIR}/allegro-5.0.9 ]; then
    # Extrai pacote do allegro
    tar xvzf allegro-5.0.9.tar.gz
    if [ $? -ne 0 ]; then
        echo "Pacote não extraído: Verifique se o tar está instalado 
              ou se o pacote está disponível"
        exit
    fi
fi

cd allegro-5.0.9 # Entra na pasta do allegro
if ! [ -d ${DIR}/allegro-5.0.9/build ]; then
    mkdir build # Cria pasta 'build'
fi
cd build # Entra na pasta build

# Instalação de dependências
sudo apt-get install build-essential git cmake xorg-dev libgl1-mesa-dev libglu-dev

# Cria makefile para compilação/instalação
sudo apt-get install cmake
cmake .. 
if [ $? -ne 0 ]; then
    echo "Problemas na compilação: verifique se cmake está disponível
          ou se houve problemas de dependência" 
    exit
fi

# Compila o pacote
make
if [ $? -ne 0 ]; then
    echo "Problemas na compilação: verifique se make está instalado"
    exit
fi

# Instala as bibliotecas
sudo make install
if [ $? -ne 0 ]; then
    echo "Verifique se há possibilidade de instalação como root"
    exit
fi

# Liga bibliotecas para que apareçam disponíveis
sudo ldconfig
if [ $? -ne 0 ]; then
    echo "Verifique se há possibilidade de root ou se houve
          problemas com o ldconfig"
    exit
fi

# Remove pacotes desnecessários
cd ${DIR} && rm allegro-5.0.9.tar.gz && rm -r allegro-5.0.9
