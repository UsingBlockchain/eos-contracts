#include "dices.hpp"
#include <ctime>

using eosio::name;
using eosio::print;

/**
 * Public API
 * 
 * -------------
 **/

void dice_contract::subscribe(const name& owner, const name& account) 
{
    require_auth(owner);
    eosio_assert(owner != account, "account cannot be the contract owner");

    // instantiate players index
    players_table players(_self, _self.value);

    // make sure accounts are subscribed only once
    auto pl = players.find(account.value);
    eosio_assert(pl == players.end(), "player already exists");

    // store created proposal
    players.emplace(owner, [&](auto& e) {
        e.who = name{account}.value;
        e.payer = name{owner}.value;
    });

    eosio::print("Welcome, ", name{account});
}

void dice_contract::play(const name& account, std::string seed)
{
    require_auth(account);
    eosio_assert(seed.size() > 0, "seed should not be empty");

    // instantiate players index
    players_table players(_self, _self.value);

    // check registration status
    auto player = players.find(account.value);
    eosio_assert(player != players.end(), "account should be a subscribed player");

    // prepare representation
    const char* acct_format = account.to_string().c_str();
    const char* seed_format = seed.c_str();

    // create dice hash content
    capi_checksum256 seed_hash;
    capi_checksum256 acct_hash;
    sha256(seed_format, seed.size(), &seed_hash);
    sha256(acct_format, account.to_string().size(), &acct_hash);

    // generate random number
    random_gen  gen = random_gen::get_instance(account);
    std::string rand_num = std::to_string(gen.range(1103515245));

    // create dice score content that will be hashed
    std::string dice;
    dice += sha256_to_hex(seed_hash);
    dice += sha256_to_hex(acct_hash);
    dice += rand_num;

    // compute dice game result
    capi_checksum256 dice_hash;
    sha256(dice.c_str(), dice.size(), &dice_hash);

    // Roll Dice logger message
    std::string d_message;
    d_message += "Rolled dice for " + account.to_string();
    d_message += " with Score: " + sha256_to_hex(dice_hash);
    eosio::print(d_message);

    if (compare_scores((*player).score, dice_hash) >= 0) {
        return ;
    }

    // modify DB
    players.modify(player, name((*player).payer), [&](auto& p) {
        p.score = dice_hash;
        p.seed  = seed;
    });

    // Last Score logger message
    std::string s_message;
    s_message += "Last Score for " + account.to_string();
    s_message += " set to: " + sha256_to_hex((*player).score);
    eosio::print(s_message);
}

void dice_contract::highscore()
{
    // instantiate players index
    players_table players(_self, _self.value);

    name player;
    capi_checksum256 high_score = {};
    auto itr = players.begin();
    for (; itr != players.end(); itr++) {

        if (compare_scores(high_score, (*itr).score) >= 0) {
            continue;
        }

        high_score = (*itr).score;
        player = name((*itr).who);
    }

    // High Score Reading logger message
    std::string message;
    message += "Current Best Player: " + player.to_string();
    message += " with High Score: " + sha256_to_hex(high_score);
    eosio::print(message);
}

EOSIO_DISPATCH(dice_contract, (subscribe)(play)(highscore))
