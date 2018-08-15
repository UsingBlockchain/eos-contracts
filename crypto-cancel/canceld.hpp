
// Generic eosio library, i.e. print, type, math, etc
#include <eosiolib/eosio.hpp>

class canceld : public eosio::contract 
{
  public:

    canceld(account_name self) : contract(self) {}

    /**
     * @brief Information related to a event
     * @abi table events i64
     */
    struct event {

        event() {}

        account_name      proposal = 0;
        account_name      owner = 0;
        bool              is_closed = false;
        std::string       title;
        std::vector<account_name>    votes;

        auto primary_key() const { return proposal; }
        EOSLIB_SERIALIZE(event, (proposal)(owner)(title))
    };

    /**
     * @brief The table definitions, used to store existing events 
     *        and their current state
     */
    typedef eosio::multi_index<N(event), event> events;

    /// @abi action
    /// Create a new event
    void create(const account_name& proposal, const account_name& owner);

    /// @abi action
    /// Close an existing event
    void close(const account_name& proposal, const account_name& owner);

    /// @abi action
    /// vote for a event
    /// @param by the account who wants to make the vote
    void vote(const account_name& proposal, const account_name& owner, const account_name& by);

};
/// @}