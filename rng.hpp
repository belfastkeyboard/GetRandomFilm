#ifndef     RANDOM_NUMBER_GENERATION
#define     RANDOM_NUMBER_GENERATION 1

#include    <random>
#include    <chrono>

/**
 * 
 * @brief Very basic implementation of 128-bit int.
 *        Intended purely for UUID purposes.
 *        Therefore only equality comparison operators are implemented.
 * 
 */
struct uint128_t
{
    uint64_t first;
    uint64_t second;
    bool operator==(const uint128_t& cmp);
    bool operator!=(const uint128_t& cmp);
};

namespace random
{
    /**
     * 
     * @brief   Get a random number between __min and __max.
     * 
     */
    template <typename T>
    T number(T __min, T __max);
    /**
     * 
     * @brief   Get a random number from a normal distribution.
     *          __mean specifies the peak of the bell-curve.
     *          Optional standard deviation, default = 10.0.
     * 
     */
    template <typename T>
    T weighted_number(T __mean, double __std_dev);
    /**
     * 
     * @brief   Get a random number from a normal distribution between __min and __max.
     *          __mean specifies the  peak of the bell-curve.
     *          Optional standard deviation, default = 10.0.
     * 
     * @warning This function can enter an infinite loop as a result of its parameters.
     * 
     */
    template <typename T>
    T weighted_number(T __min, T __max, double __mean, double __std_dev);
    /**
     * 
     * @brief   Roll a random number against a percentage.
     *          __x is clamped to 0, 100 if outside this range.
     * 
     */
    template <typename T>
    bool percentage(T __x);
    /**
     * 
     * @brief   Generates a random 64-bit UUID.
     * 
     */
    uint64_t UUID();
    /**
     * 
     * @brief   Generates a random 128-bit UUID.
     *          uint128_t struct has very basic implementation.
     *          Intended only for ID comparisons.
     * 
     */
    uint128_t UUID128();
    /**
     * 
     * @brief   Get a random index from a vector.
     * 
     */
    template <typename T>
    size_t index(std::vector<T> __vec);
    /**
     * 
     * @brief   Get a random index from a vector, using a normal distribution.
     *          Optional standard deviation, default = 10.0.
     * 
     * @warning This function can enter an infinite loop as a result of its parameters.
     * 
     */
    template <typename T>
    size_t weighted_index(std::vector<T> __v, size_t __mean, size_t __std_dev);
    /**
     * 
     * @brief   Get a random index from a vector, where the vector is an list of indices.
     * 
     */
    template <typename T>
    size_t weighted_index(std::vector<T> __vec);
    /**
     * 
     * @brief Get a Mersenne twister generator object. Used in algorithms like std::shuffle.
     * 
     */
    std::mt19937 gen();
}


#endif