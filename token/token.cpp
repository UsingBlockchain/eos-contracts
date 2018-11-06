#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using eosio::name;
using eosio::print;

CONTRACT token_contract : public eosio::contract 
{
  public:
    using eosio::contract::contract;

    ACTION hello(name receiver) {
        eosio::print("Hello ", name{receiver});
    }
};

EOSIO_DISPATCH(token_contract, (hello))