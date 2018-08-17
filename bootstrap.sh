#!/bin/bash

WALLET_URL='--wallet-url http://172.18.0.3:5555'
SERVER_URL='-u http://172.18.0.2:7777'

# initialize wallet and store PW
cleos ${WALLET_URL} wallet create --file /tmp/work/default.pw

# import eos private key
cleos ${WALLET_URL} wallet import --private-key 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3

# create system accounts
eosio_bpay=`cleos ${WALLET_URL} create key --to-console | grep "Private key: " | awk '{print $3}'`
eosio_msig=`cleos ${WALLET_URL} create key --to-console | grep "Private key: " | awk '{print $3}'`
eosio_names=`cleos ${WALLET_URL} create key --to-console | grep "Private key: " | awk '{print $3}'`
eosio_ram=`cleos ${WALLET_URL} create key --to-console | grep "Private key: " | awk '{print $3}'`
eosio_ramfee=`cleos ${WALLET_URL} create key --to-console | grep "Private key: " | awk '{print $3}'`
eosio_saving=`cleos ${WALLET_URL} create key --to-console | grep "Private key: " | awk '{print $3}'`
eosio_stake=`cleos ${WALLET_URL} create key --to-console | grep "Private key: " | awk '{print $3}'`
eosio_token=`cleos ${WALLET_URL} create key --to-console | grep "Private key: " | awk '{print $3}'`
eosio_vpay=`cleos ${WALLET_URL} create key --to-console | grep "Private key: " | awk '{print $3}'`

# now importing keys
eosio_bpay_pub=`cleos ${WALLET_URL} wallet import --private-key ${eosio_bpay} | grep "imported private key for: " | awk '{print $5}'`
eosio_msig_pub=`cleos ${WALLET_URL} wallet import --private-key ${eosio_msig} | grep "imported private key for: " | awk '{print $5}'`
eosio_names_pub=`cleos ${WALLET_URL} wallet import --private-key ${eosio_names} | grep "imported private key for: " | awk '{print $5}'`
eosio_ram_pub=`cleos ${WALLET_URL} wallet import --private-key ${eosio_ram} | grep "imported private key for: " | awk '{print $5}'`
eosio_ramfee_pub=`cleos ${WALLET_URL} wallet import --private-key ${eosio_ramfee} | grep "imported private key for: " | awk '{print $5}'`
eosio_saving_pub=`cleos ${WALLET_URL} wallet import --private-key ${eosio_saving} | grep "imported private key for: " | awk '{print $5}'`
eosio_stake_pub=`cleos ${WALLET_URL} wallet import --private-key ${eosio_stake} | grep "imported private key for: " | awk '{print $5}'`
eosio_token_pub=`cleos ${WALLET_URL} wallet import --private-key ${eosio_token} | grep "imported private key for: " | awk '{print $5}'`
eosio_vpay_pub=`cleos ${WALLET_URL} wallet import --private-key ${eosio_vpay} | grep "imported private key for: " | awk '{print $5}'`

# now creating accounts
cleos ${WALLET_URL} ${SERVER_URL} create account eosio eosio.bpay ${eosio_bpay_pub}
cleos ${WALLET_URL} ${SERVER_URL} create account eosio eosio.msig ${eosio_msig_pub}
cleos ${WALLET_URL} ${SERVER_URL} create account eosio eosio.names ${eosio_names_pub}
cleos ${WALLET_URL} ${SERVER_URL} create account eosio eosio.ram ${eosio_ram_pub}
cleos ${WALLET_URL} ${SERVER_URL} create account eosio eosio.ramfee ${eosio_ramfee_pub}
cleos ${WALLET_URL} ${SERVER_URL} create account eosio eosio.saving ${eosio_saving_pub}
cleos ${WALLET_URL} ${SERVER_URL} create account eosio eosio.stake ${eosio_stake_pub}
cleos ${WALLET_URL} ${SERVER_URL} create account eosio eosio.token ${eosio_token_pub}
cleos ${WALLET_URL} ${SERVER_URL} create account eosio eosio.vpay ${eosio_vpay_pub}

# installing contracts
cleos ${WALLET_URL} ${SERVER_URL} set contract eosio.token /contracts/eosio.token
cleos ${WALLET_URL} ${SERVER_URL} set contract eosio.msig /contracts/eosio.msig

# sending funds to eosio (put 1B coins in circulation)
cleos ${WALLET_URL} ${SERVER_URL} push action eosio.token create '[ "eosio", "10000000000.0000 SYS" ]' -p eosio.token
cleos ${WALLET_URL} ${SERVER_URL} push action eosio.token issue '[ "eosio", "1000000000.0000 SYS", "memo" ]' -p eosio

# now set eosio.system contract
cleos ${WALLET_URL} ${SERVER_URL} set contract eosio /contracts/eosio.system

# done configuring network
printf "\n\n${bldred}\t _______  _______  _______ _________ _______\n"
printf '\t(  ____ \(  ___  )(  ____ \\\\__   __/(  ___  )\n'
printf "\t| (    \/| (   ) || (    \/   ) (   | (   ) |\n"
printf "\t| (__    | |   | || (_____    | |   | |   | |\n"
printf "\t|  __)   | |   | |(_____  )   | |   | |   | |\n"
printf "\t| (      | |   | |      ) |   | |   | |   | |\n"
printf "\t| (____/\| (___) |/\____) |___) (___| (___) |\n"
printf "\t(_______/(_______)\_______)\_______/(_______)\n${txtrst}"
printf "\n\nYour EOS blockchain is now configured...\n\n"
