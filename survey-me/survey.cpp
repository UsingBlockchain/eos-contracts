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

#include "survey.hpp"

using namespace eosio;

/**
 * @brief Apply create action
 */
void survey_me::create(const account_name& contributor, const account_name& host) 
{
   require_auth(host);
   eosio_assert(contributor != host, "contributor shouldn't be the same as host");

   // Check if survey already exists
   surveys existing_host_surveys(_self, host);
   auto itr = existing_host_surveys.find(contributor);
   eosio_assert(itr == existing_host_surveys.end(), "survey already exists");

   existing_host_surveys.emplace(host, [&]( auto& g ) {
      g.contributor = contributor;
      g.host = host;
   });
}

/**
 * @brief Apply restart action
 */
void survey_me::restart(const account_name& contributor, const account_name& host, const account_name& by) 
{
   require_auth(by);

   // Check if survey exists
   surveys existing_host_surveys(_self, host);
   auto itr = existing_host_surveys.find(contributor);
   eosio_assert(itr != existing_host_surveys.end(), "survey doesn't exists");

   // Check if this survey belongs to the action sender
   eosio_assert(by == itr->host || by == itr->contributor, "this is not your survey!");

   // Reset survey
   existing_host_surveys.modify(itr, itr->host, []( auto& g ) {
      g.reset_survey();
   });
}

/**
 * @brief Apply close action
 */
void survey_me::close(const account_name& contributor, const account_name& host) 
{
   require_auth(host);

   // Check if survey exists
   surveys existing_host_surveys(_self, host);
   auto itr = existing_host_surveys.find(contributor);
   eosio_assert(itr != existing_host_surveys.end(), "survey doesn't exists");

   // Remove survey
   existing_host_surveys.erase(itr);
}

/**
 * @brief Apply answer action
 */
void survey_me::answer(const account_name& contributor, const account_name& host, const account_name& by, const uint8_t& task, const std::vector<uint8_t> choices) 
{
    require_auth(by);

    // Check if survey exists
    surveys existing_host_surveys(_self, host);
    auto itr = existing_host_surveys.find(contributor);
    eosio_assert(itr != existing_host_surveys.end(), "survey doesn't exists");

    // Check if this survey hasn't ended yet
    eosio_assert(itr->current_task >= itr->count_tasks, "the survey has ended!");
    // Check if this survey belongs to the action sender
    eosio_assert(by == itr->host || by == itr->contributor, "this is not your survey!");

    // store answers
    existing_host_surveys.modify(itr, itr->host, [&]( auto& g ) {
        g.answers[task] = choices;
    });
}


EOSIO_ABI(survey_me, (create)(restart)(close)(answer))