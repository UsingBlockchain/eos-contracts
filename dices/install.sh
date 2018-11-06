#!/bin/bash

echo ""
echo "\t\t--- Now configuring tests/dices EOS Functional Test ---"
echo ""

# @objects owner & active
owner=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`
active=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`

owner_pub=`cleos wallet import -n eviasContracts --private-key ${owner} | grep "imported private key for: " | awk '{print $5}'`
active_pub=`cleos wallet import -n eviasContracts --private-key ${active} | grep "imported private key for: " | awk '{print $5}'`

# create account @token
cleos create account eosio dices.ctr ${owner_pub} ${active_pub}

# deploy contract
cleos set contract dices.ctr /work/dices -p dices.ctr

# -- TEST

# @tester and @user
tester=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`
user=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`

tester_pub=`cleos wallet import -n eviasContracts --private-key ${tester} | grep "imported private key for: " | awk '{print $5}'`
user_pub=`cleos wallet import -n eviasContracts --private-key ${user} | grep "imported private key for: " | awk '{print $5}'`

# create account @tester @user
cleos create account eosio dices.tester ${tester_pub} ${tester_pub}
cleos create account eosio dices.user ${user_pub} ${user_pub}

# subscribe users
cleos push action dices.ctr subscribe '{"owner":"dices.ctr", "account":"dices.user"}' -p dices.ctr
cleos push action dices.ctr subscribe '{"owner":"dices.ctr", "account":"dices.tester"}' -p dices.ctr

# play ...
cleos push action dices.ctr play '{"account":"dices.user", "seed":"123"}' -p dices.user@active
cleos push action dices.ctr play '{"account":"dices.tester", "seed": "456"}' -p dices.tester@active

# display high score
cleos push action dices.ctr highscore '{}' -p dices.ctr
