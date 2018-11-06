#!/bin/bash

src_path=/work
if [ $# -eq 1 ]; then
    src_path=$1
fi

if ! which cleos > /dev/null; then
    echo "Please run this command on a machine with the 'cleos' tool installed."
    exit 1
fi

echo ""
echo "\t\t--- Now starting eVias EOS Smart Contracts ---"
echo ""

cleos wallet create --file /work/eviasContracts.dat --name eviasContracts
cleos wallet unlock -n eviasContracts --password `cat /work/eviasContracts.dat`

# @eosio account 
cleos wallet import -n eviasContracts --private-key 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3

# install functional tests
sh ${src_path}/ping/install.sh

echo ""
echo "Waiting 3 seconds..."
sleep 3

sh ${src_path}/secrets/install.sh

echo ""
echo "Waiting 3 seconds..."
sleep 3

sh ${src_path}/token/install.sh

echo ""
echo "Waiting 3 seconds..."
sleep 3

sh ${src_path}/dices/install.sh

echo ""
echo "\t\t--- Done with eVias EOS Smart Contracts ---"
echo ""

exit 0
