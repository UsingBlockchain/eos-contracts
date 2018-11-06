#include <eosiolib/eosio.hpp>

using eosio::name;

/**
 * @brief Random Number Generator singleton class
 * @detail Random number generation using a linear congruential generator.
 * @see https://github.com/bada-studio/knights_contract/
 **/
class random_gen
{
    static random_gen instance;

    const uint32_t a = 1103515245;
    const uint32_t c = 12345;
    uint64_t seed = 0;

  public:

    /**
     * @brief Singleton class: Get the singleton instance
     * @param   eosio::name     player
     * @return  random_gen
     **/
    static random_gen& get_instance(name player)
    {
        if (instance.seed == 0) {
            instance.seed = current_time() + player.value;
        }
        return instance;
    }

    /**
     * @brief Get a random number up to `to`.
     * @param   uint32_t    to
     * @return  uint32_t
     **/
    uint32_t range(uint32_t to)
    {
        capi_checksum256 result;
        sha256((char *)&seed, sizeof(seed), &result);
        seed = result.hash[1];
        seed <<= 32;
        seed |= result.hash[0];
        return (uint32_t)(seed % to);
    }
};