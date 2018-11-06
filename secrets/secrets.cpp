#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

using eosio::name;
using eosio::print;

CONTRACT secrets_contract : public eosio::contract 
{
  public:
    using eosio::contract::contract;

    ACTION show(name owner, std::string hash) {
        require_auth(owner);
        eosio::print("Hash published for ", name{owner}, ": ", hash);
    }

    ACTION reveal(name owner, std::string hash, std::string secret) {
        require_auth(owner);
        eosio::print("Secret published for ", name{owner}, ": ", secret);
    }
};

EOSIO_DISPATCH(secrets_contract, (show)(reveal))