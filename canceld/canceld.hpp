
// Generic eosio library, i.e. print, type, math, etc
#include <eosiolib/eosio.hpp>
#include <eosiolib/currency.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>

class canceld : public eosio::contract 
{
  public:
    const uint32_t FIVE_MINUTES = 5 * 60;
    const uint32_t ONE_DAY = 24 * 60 * 60;
    const uint32_t DAYS_FOR_VOTING = 30;

    canceld(account_name self) : contract(self) {}

    enum event_types {
        MINI_VILLAGE = 1000,
        VILLAGE     = 10000,
        MINI_REGION = 50000,
        REGION      = 100000,
        SUP_REGION  = 1000000
    };

    /**
     * @brief Information related to a event
     * @abi table events i64
     */
    struct event {

        event() {}

        // Proposal accounts
        account_name      proposal = 0;
        account_name      owner = 0;
        account_name      buyer = 0; // Boycott buyer account

        std::string       title;
        bool              is_bought = false;
        std::vector<account_name>    votes;

        // Proposals meta data
        uint8_t     cnt_needed_votes = 0;
        eosio::time_point_sec start;
        eosio::time_point_sec end;

        auto primary_key() const { return proposal; }
        EOSLIB_SERIALIZE(event, (proposal)(owner)(buyer)(title)(is_ended_votes)(votes)(cnt_needed_votes)(start)(end))
    };

    //@abi table account i64
    struct account {
        account( account_name o = account_name() ):owner(o){}

        account_name owner;
        asset        eos_balance;
        uint32_t     open_votes = 0;

        bool is_empty() const { return !( eos_balance.amount | open_votes ); }

        uint64_t primary_key() const { return owner; }

        EOSLIB_SERIALIZE( account, (owner)(eos_balance)(open_votes) )
    };

    /**
     * @brief The table definitions, used to store existing events 
     *        and their current state
     */
    typedef eosio::multi_index<N(event), event> event_index;
    typedef eosio::multi_index< N(account), account> account_index;

    event_index     events;
    account_index   accounts;

    /// @abi action
    /// Create a new event
    void create(const account_name& proposal, const account_name& owner, std::string title, uint8_t cnt_votes);

    /// @abi action
    /// vote for a event
    /// @param by the account who wants to make the vote
    void vote(const account_name& proposal, const account_name& owner, const account_name& by);

    /// @abi action
    /// buy an event: Cancel the boycott
    /// @param by the account who wants to make the vote
    void buy(const account_name& proposal, const account_name& owner, const account_name& by);

    /// @abi action
    void deposit(const account_name from, const asset& quantity);

    /// @abi action
    void withdraw(const account_name to, const asset& quantity);

};
/// @}