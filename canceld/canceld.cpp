
#include "canceld.hpp"

using namespace eosio;

/**
 * @brief Apply create action
 * 
 * This will initiate a transfer of 10000 coins from `owner` to 
 * `proposal`.
 */
void canceld::create(
    const account_name& proposal, 
    const account_name& owner, 
    std::string title, 
    uint8_t cnt_votes
) 
{
    require_auth(owner);
    eosio_assert(proposal != owner, "proposal shouldn't be the same as owner");

    // Check if event already exists
    auto itr = events.find(proposal);
    eosio_assert(itr == events.end(), "event already exists");

    // validate account (pays for creation)
    auto act = accounts.find(owner);
    eosio_assert(itr != accounts.end(), "unknown account");

    // Creating new proposals costs 10000 coins
    uint8_t price = 10000;

    // update in-contract balance
    accounts.modify(act, 0, [&](auto& acnt) {
        eosio_assert(acnt.eos_balance >= price, "insufficient balance");
        acnt.eos_balance -= 10000;
    });

    std::stringstream stream;
    stream << "creating " << proposal << " for " << amt << " " << CORE_SYMBOL;

    // execute EOS transfer
    action(
        permission_level{ owner, N(active) },
        N(eosio.token), N(transfer),
        std::make_tuple(owner, proposal, price, stream.str())
    ).send();

    // store created proposal
    events.emplace(owner, [&](auto& e) {
        e.proposal = proposal;
        e.owner = owner;
        e.title = title;
        e.cnt_needed_votes = cnt_votes;
        e.start_timestamp = eosio::time_point_sec(now());
        e.end_timestamp = eosio::time_point_sec(now() + (DAYS_FOR_VOTING * ONE_DAY));
    });

    // empty accounts should be removed
    if (act->is_empty()) {
        accounts.erase(act);
    }
}

/**
 * @brief Apply vote action
 * 
 * This will initiate a transfer of 100 coins from `by` to 
 * `proposal`.
 */
void canceld::vote(const account_name& proposal, const account_name& owner, const account_name& by) 
{
    require_auth(by);

    // Check if event exists
    auto itr = events.find(proposal);
    eosio_assert(itr != events.end(), "event doesn't exists");
    eosio_assert(itr->is_bought == false, "the event was counter bought, congratulations!");
    eosio_assert(itr->owner == by, "you cannot vote for your own proposal!");

    // Check if vote exists for proposal
    auto votes = itr->votes;
    auto itr_v = votes.find(by);
    eosio_assert(itr_v == votes.end(), "you have already voted for this proposal!");

    // Creating new votes costs 100 coins
    uint8_t price = 100;

    // update in-contract balance
    accounts.modify(act, 0, [&](auto& acnt) {
        eosio_assert(acnt.eos_balance >= price, "insufficient balance");
        acnt.eos_balance -= 10000;
    });

    std::stringstream stream;
    stream << "voting in " << proposal << " for " << price << " " << CORE_SYMBOL;

    // execute EOS transfer
    action(
        permission_level{ owner, N(active) },
        N(eosio.token), N(transfer),
        std::make_tuple(by, proposal, price, stream.str())
    ).send();

    // store vote
    events.modify(itr, by, [&](auto& e) {
        e.votes.push_back(by);
    });
}

/**
 * @brief Apply buy action
 * 
 * This will initiate a transfer of `100 x needed_votes` coins from `by` to 
 * `proposal` and will effectively *close* the proposal.
 */
void canceld::buy(const account_name& proposal, const account_name& owner, const account_name& by)
{
    require_auth(by);

    // Check if event exists
    auto itr = events.find(proposal);
    eosio_assert(itr != events.end(), "event doesn't exists");
    eosio_assert(itr->is_bought == false, "the event was already counter bought!");

    // amount is hundred times the count of needed votes
    uint8_t price = 100 * (itr->cnt_needed_votes + 100); // includes create price

    // update in-contract balance
    accounts.modify(act, 0, [&](auto& acnt) {
        eosio_assert(acnt.eos_balance >= price, "insufficient balance");
        acnt.eos_balance -= price;
    });

    std::stringstream stream;
    stream << "buying " << proposal << " for " << price << " " << CORE_SYMBOL;

    // execute EOS transfer
    action(
        permission_level{ owner, N(active) },
        N(eosio.token), N(transfer),
        std::make_tuple(by, itr->buyer, price, stream.str())
    ).send();

    // update the proposal record
    // Proposal pays for the modification as it holds the creation funds.
    existing_owner_events.modify(itr, itr->proposal, [&](auto& e) {
        e.is_bought = true;
    });
}

/**
 * @brief Apply buy action
 * 
 * This will initiate a transfer of `100 x needed_votes` coins from `by` to 
 * `proposal` and will effectively *close* the proposal.
 */
void canceld::deposit(const account_name from, const asset& quantity) 
{
    eosio_assert(quantity.is_valid(), "invalid quantity");
    eosio_assert(quantity.amount > 0, "must deposit positive quantity");

    auto itr = accounts.find(from);
    if (itr == accounts.end()) {
        // Just in Time creation of the account
        itr = accounts.emplace(_self, [&](auto& acnt) {
            acnt.owner = from;
        });
    }

    std::stringstream stream;
    stream << "depositing " << quantity.amount << " " << CORE_SYMBOL << " with " << from;

    action(
        permission_level{ from, N(active) },
        N(eosio.token), N(transfer),
        std::make_tuple(from, _self, quantity, stream.str())
    ).send();

    accounts.modify( itr, 0, [&]( auto& acnt ) {
        acnt.eos_balance += quantity.amount;
    });
}

void canceld::withdraw(const account_name to, const asset& quantity) 
{
    require_auth( to );

    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must withdraw positive quantity" );

    auto itr = accounts.find( to );
    eosio_assert(itr != accounts.end(), "unknown account");

    accounts.modify( itr, 0, [&]( auto& acnt ) {
        eosio_assert( acnt.eos_balance >= quantity, "insufficient balance" );
        acnt.eos_balance -= quantity;
    });

    std::stringstream stream;
    stream << "withdrawing " << quantity.amount << " " << CORE_SYMBOL << " to " << to;

    action(
        permission_level{ _self, N(active) },
        N(eosio.token), N(transfer),
        std::make_tuple(_self, to, quantity, stream.str())
    ).send();

    if( itr->is_empty() ) {
        accounts.erase(itr);
    }
}

EOSIO_ABI(canceld, (create)(vote)(buy)(deposit)(withdraw))