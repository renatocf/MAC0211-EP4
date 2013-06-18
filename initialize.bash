#!/bin/bash

DIR_NAME=MAC0122-EP2

SERVER_NICK=origin
SERVER_ADDRESS=git@github.com:renatocf/MAC0211-EP2.git

mkdir ${DIR_NAME}
cd ${DIR_NAME}
git init
git remote add ${SERVER_NICK} ${SERVER_ADDRESS}
git pull
cd ..
