/**
 * Part of the evias/eos-contracts package.
 *
 * NOTICE OF LICENSE
 *
 * Licensed under the MIT License.
 *
 * This source file is subject to the MIT License that is
 * bundled with this package in the LICENSE file.
 *
 * @package    evias/eos-contracts
 * @version    1.0.0
 * @author     Grégory Saive <greg@evias.be>
 * @license    MIT License
 * @copyright  (c) 2018, Grégory Saive
 */

// Generic eosio library, i.e. print, type, math, etc
#include <eosiolib/eosio.hpp>

class survey_me : public eosio::contract 
{
  public:

    survey_me(account_name self) : contract(self) {}
    
    /**
     * @brief Information related to a survey
     * @abi table surveys i64
     */
    struct survey {
        static const uint16_t count_tasks = 10;
        static const uint16_t current_task = 1;

        survey() {
            for (int i = 1; i <= num_tasks; i++) {
                answers.insert(make_pair(i, {}));
            }
        }

        account_name           contributor;
        account_name           host;
        std::map<uint8_t, std::vector<uint8_t> >   answers;

        auto primary_key() const { return contributor; }
        EOSLIB_SERIALIZE(survey, (contributor)(host))
    };

    /**
     * @brief The table definition, used to store existing surveys and their current state
     */
    typedef eosio::multi_index<surveys_account, survey> surveys;

    /// @abi action
    /// Create a new survey
    void create(const account_name& contributor, const account_name& host);

    /// @abi action
    /// Restart a game
    /// @param by the account who wants to restart the survey
    void restart(const account_name& contributor, const account_name& host, const account_name& by);

    /// @abi action
    /// Close an existing game, and remove it from storage
    void close(const account_name& contributor, const account_name& host);

    /// @abi action
    /// Answer a survey task
    /// @param by the account who wants to make the move
    void answer(const account_name& contributor, const account_name& host, const account_name& by, const uint8_t task, const std::vector<uint8_t> choices);

};
/// @}