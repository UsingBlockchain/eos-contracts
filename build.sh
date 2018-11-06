#!/bin/bash

echo ""
echo "\t\t--- Now building eVias EOS Smart Contracts ---"
echo ""

echo "\t1) Building ping functional test.."
eosio-cpp -o ping/ping.wasm ping/ping.cpp

echo "\t2) Building secrets functional test.."
eosio-cpp -o secrets/secrets.wasm secrets/secrets.cpp

echo "\t3) Building token functional test.."
eosio-cpp -o token/token.wasm token/token.cpp

echo "\t4) Building dices functional test.."
eosio-cpp -o dices/dices.wasm dices/dices.cpp

echo ""
echo "\t\t--- Done building eVias EOS Smart Contracts ---"
echo ""

exit 0
