#!/bin/bash

echo ""
echo "\t\t--- Now configuring tests/secrets EOS Functional Test ---"
echo ""

# @secrets.ctr  owner & active
owner=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`
active=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`

owner_pub=`cleos wallet import -n eviasContracts --private-key ${owner} | grep "imported private key for: " | awk '{print $5}'`
active_pub=`cleos wallet import -n eviasContracts --private-key ${active} | grep "imported private key for: " | awk '{print $5}'`

# create account @secrets.ctr
cleos create account eosio secrets.ctr ${owner_pub} ${active_pub}

# deploy contract
cleos set contract secrets.ctr /work/secrets -p secrets.ctr

# PLAY TEST GAME

# @alice and @bob
alice=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`
bob=`cleos create key --to-console | grep "Private key: " | awk '{print $3}'`

alice_pub=`cleos wallet import -n eviasContracts --private-key ${alice} | grep "imported private key for: " | awk '{print $5}'`
bob_pub=`cleos wallet import -n eviasContracts --private-key ${bob} | grep "imported private key for: " | awk '{print $5}'`

# create accounts
cleos create account eosio alice ${alice_pub} ${alice_pub}
cleos create account eosio bob ${bob_pub} ${bob_pub}

# publish envelopes (hashed secret)
cleos push action secrets.ctr show '[ "alice", "HashOfAliceSecret" ]' -p alice@active
cleos push action secrets.ctr show '[ "bob", "HashOfBobSecret" ]' -p bob@active

# publish secrets
cleos push action secrets.ctr reveal '[ "alice", "HashOfAliceSecret", "AliceSecret" ]' -p alice@active
cleos push action secrets.ctr reveal '[ "bob", "HashOfBobSecret", "BobSecret" ]' -p bob@active
