#!/usr/bin/env bash

ERROR=0 \
    && pushd Pal/regression &>/dev/null \
    &&  make regression || ERROR=1 \
    && popd &>/dev/null \
    && pushd LibOS/shim/test/fs &>/dev/null \
    &&  echo ">>>>>> shim fs test <<<<<<" \
    &&  make test || { ERROR=1; echo ">>>>> FAIL <<<<<"; } \
    && popd &>/dev/null \
    && pushd LibOS/shim/test/regression &>/dev/null \
    &&  echo ">>>>>> shim regression <<<<<<" \
    &&  make regression || { ERROR=1; echo ">>>>> FAIL <<<<<"; } \
    && popd &>/dev/null \
    && pushd Examples/bash &>/dev/null \
    &&  echo ">>>>>> bash <<<<<<" \
    &&  make &>/dev/null && make regression || { ERROR=1; echo ">>>>> FAIL <<<<<"; } \
    && popd &>/dev/null \
    && pushd Examples/curl &>/dev/null \
    &&  echo ">>>>>> curl <<<<<<" \
    &&  make &>/dev/null && make check || { ERROR=1; echo ">>>>> FAIL <<<<<"; } \
    && popd &>/dev/null \
    && pushd Examples/nodejs &>/dev/null \
    &&  echo ">>>>>> nodejs <<<<<<" \
    &&  sudo apt-get -y install nodejs &>/dev/null \
    &&  make &>/dev/null && make check || { ERROR=1; echo ">>>>> FAIL <<<<<"; } \
    && popd &>/dev/null \
    && pushd Examples/nodejs-express-server &>/dev/null \
    &&  echo ">>>>>> nodejs-express-server  <<<<<<" \
    &&  sudo apt-get -y install npm node-gyp nodejs-dev libssl1.0-dev &>/dev/null \
    &&  npm install express \
    &&  make &>/dev/null && make check || { ERROR=1; echo ">>>>> FAIL <<<<<"; } \
    && popd &>/dev/null \
    && pushd Examples/python-simple &>/dev/null \
    &&  echo ">>>>>> python-simple <<<<<<" \
    &&  sudo apt-get -y install python3.6 &>/dev/null \
    &&  make &>/dev/null && make check || { ERROR=1; echo ">>>>> FAIL <<<<<"; } \
    && popd &>/dev/null \
    && pushd Examples/gcc &>/dev/null \
    &&  echo ">>>>>> gcc <<<<<<" \
    &&  make &>/dev/null && make check || { ERROR=1; echo ">>>>> FAIL <<<<<"; } \
    && popd &>/dev/null \
    && pushd Examples/python-scipy-insecure &>/dev/null \
    &&  echo ">>>>>> python-scipy-insecure <<<<<<" \
    &&  sudo apt-get -y install python3-scipy python3-numpy &>/dev/null \
    &&  make &>/dev/null && make check || { ERROR=1; echo ">>>>> FAIL <<<<<"; } \
    && popd &>/dev/null \
    && exit $ERROR

