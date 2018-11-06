#!/bin/bash

echo ""
echo "\t\t--- Now configuring tests/token EOS Functional Test ---"
echo ""

# @token owner & active
owner=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`
active=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`

owner_pub=`cleos wallet import -n eviasContracts --private-key ${owner} | grep "imported private key for: " | awk '{print $5}'`
active_pub=`cleos wallet import -n eviasContracts --private-key ${active} | grep "imported private key for: " | awk '{print $5}'`

# create account @token
cleos create account eosio eosio.token ${owner_pub} ${active_pub}

# deploy contract
cleos set contract eosio.token /contracts/eosio.token -p eosio.token

# -- TEST

# @tester and @user
tester=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`
user=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`

tester_pub=`cleos wallet import -n eviasContracts --private-key ${tester} | grep "imported private key for: " | awk '{print $5}'`
user_pub=`cleos wallet import -n eviasContracts --private-key ${user} | grep "imported private key for: " | awk '{print $5}'`

# create account @tester @user
cleos create account eosio token.tester ${tester_pub} ${tester_pub}
cleos create account eosio token.user ${user_pub} ${user_pub}

# create custom token EVS
cleos push action eosio.token create '{"issuer":"eosio", "maximum_supply":"290888.0000 EVS"}' -p eosio.token
cleos push action eosio.token issue '{"to":"token.user","quantity":"100.0000 EVS","memo":"issuing EVS"}' -p eosio
cleos push action eosio.token transfer '{"from":"token.user","to":"token.tester","quantity":"10.0000 EVS","memo":"transferring EVS"}' -p token.user@active
