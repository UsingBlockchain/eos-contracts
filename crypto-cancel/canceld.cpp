
#include "canceld.hpp"

using namespace eosio;

/**
 * @brief Apply create action
 */
void canceld::create(const account_name& proposal, const account_name& owner) 
{
   require_auth(owner);
   eosio_assert(proposal != owner, "proposal shouldn't be the same as owner");

   // Check if event already exists
   events existing_owner_events(_self, owner);
   auto itr = existing_owner_events.find(proposal);
   eosio_assert(itr == existing_owner_events.end(), "event already exists");

   existing_owner_events.emplace(owner, [&]( auto& g ) {
      g.proposal = proposal;
      g.owner = owner;
   });
}

/**
 * @brief Apply close action
 */
void canceld::close(const account_name& proposal, const account_name& owner) 
{
   require_auth(owner);

   // Check if event exists
   events existing_owner_events(_self, owner);
   auto itr = existing_owner_events.find(proposal);
   eosio_assert(itr != existing_owner_events.end(), "event doesn't exists");

   // Remove event
   existing_owner_events.erase(itr);
}

/**
 * @brief Apply answer action
 */
void canceld::vote(const account_name& proposal, const account_name& owner, const account_name& by) 
{
    require_auth(by);

    // Check if event exists
    events existing_owner_events(_self, owner);
    auto itr = existing_owner_events.find(proposal);
    eosio_assert(itr != existing_owner_events.end(), "event doesn't exists");
    eosio_assert(itr->is_closed == false, "the event has ended!");

    // Check if answer exists for proposal
    auto votes = itr->votes;
    auto itr_v = votes.find(by);
    eosio_assert(itr_v == votes.end(), "you have already voted for this proposal!");

    // store vote
    votes.push_back(by);
}


EOSIO_ABI(canceld, (create)(close)(answer))