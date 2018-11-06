#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using eosio::name;
using eosio::print;

CONTRACT ping_contract : public eosio::contract 
{
  public:
    using eosio::contract::contract;

    ACTION ping(name receiver) {
        eosio::print("Pong from ", name{receiver});
    }
};

EOSIO_DISPATCH(ping_contract, (ping))