#!/bin/bash

echo ""
echo "\t\t--- Now configuring tests/ping EOS Functional Test ---"
echo ""

# @ping.ctr  owner & active
owner=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`
active=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`

owner_pub=`cleos wallet import -n eviasContracts --private-key ${owner} | grep "imported private key for: " | awk '{print $5}'`
active_pub=`cleos wallet import -n eviasContracts --private-key ${active} | grep "imported private key for: " | awk '{print $5}'`

# create ping @ping.ctr
cleos create account eosio ping.ctr ${owner_pub} ${active_pub}

# deploy contract
cleos set contract ping.ctr /work/ping -p ping.ctr

# use contract
cleos push action ping.ctr ping '{"receiver": "ping.ctr"}' -p ping.ctr
