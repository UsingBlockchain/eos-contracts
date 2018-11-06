#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/types.h>
#include "utils.hpp"
#include "random.hpp"

// c++ stdlib
#include <string>
#include <ctime>

using eosio::name;

class [[eosio::contract]] dice_contract 
    : public eosio::contract 
{
  public:
    using eosio::contract::contract;

    dice_contract(name self, name code, eosio::datastream<const char*> ds)
        : eosio::contract(self, self, ds)
    {}

    ACTION subscribe(const name& owner, const name& account);
    ACTION play(const name& account, std::string seed);
    ACTION highscore();

    struct [[eosio::table]] player {
        uint64_t who;
        uint64_t payer;
        std::string  seed;
        capi_checksum256  score = {};

        uint64_t primary_key() const { return who; }
        uint64_t get_secondary_1() const { return payer; }
        EOSLIB_SERIALIZE(player, (who)(payer)(seed)(score))
    };

    typedef eosio::multi_index<name("players"), player>      players_table;

protected:

    inline int compare_scores(capi_checksum256 s1, capi_checksum256 s2);
};

int dice_contract::compare_scores(capi_checksum256 s1, capi_checksum256 s2)
{
    // shortcuts
    const char* full_1 = (sha256_to_hex(s1)).c_str();
    const char* full_2 = (sha256_to_hex(s2)).c_str();
    const int   size_1 = strlen(full_1);
    const int   size_2 = strlen(full_2);

    // prepare
    int i = 0;
    int lim = std::min(size_1, size_2);

    // evaluate
    while (i < lim)
    {
        if (full_1[i] != full_2[i]) {
            return full_1[i] - full_2[i];
        }

        i++;
    };

    return size_1 - size_2;
}