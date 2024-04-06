/*
 *
 *      Random Number Generation methods
 *      
 *      I created this simple library of RNG methods after
 *      being unable to reliably generate random numbers 
 *      with the recommanded random_device method.
 * 
 *      These collected methods are the functions that I found
 *      myself using regularly or that I thought might be 
 *      useful in the future.
 * 
 *      The basic design is a singleton-pattern object that
 *      uses system time to seed its generator. It also 
 *      allows for the entry of manual seeds with an
 *      alternate class constructor.
 * 
 * 
 *      Creating the generator:
 * 
 *          > Use 'Random& random = Random::instance();'
 * 
 *          > You must create a new instance of the generator
 *          if the generator is not accessible in the current
 *          scope.
 * 
 *          > Use 'Random& random = Random::instance(seed);'
 *          to make use of a manual seed. Seeds are uint64_t;
 * 
 *          The singleton pattern allows for the same seed to
 *          be in use for the program's entire execution.
 * 
 * 
 *      Accessing the methods:
 * 
 *          > Use 'random.method(argument);'.
 * 
 *      Features:
 * 
 *          > random.number()           || 2 overloads
 * 
 *          Min-max:                    returns a value from within a min-max range.
 *          Vector:                     returns a random element from within a vector.
 * 
 * 
 *          > random.weighted_number()  || 6 overloads
 * 
 *          Min-max, mean, std dev:     returns a value within the specified range,
 *                                      clustered around the mean,
 *                                      distribution governed by the standard deviation.
 *          Min-max, mean:              returns a value within the specified range,
 *                                      clustered around the mean,
 *                                      standard deviation defaults to 10.
 *          Mean, std dev:              returns a value clustered around the mean,
 *                                      distribution governed by the standard deviation,
 *                                      no min-max limitations.
 *          Mean:                       returns a value clustered around the mean,
 *                                      standard deviation defaults to 10,
 *                                      no min-max limitations.
 *          std::vector:                returns a random index from the vector.
 *          std::vector, std::vector:   returns a random index from the first vector,
 *                                      using second vector for weights.
 * 
 * 
 *          > random.percentage()       || 0 overloads
 * 
 *          percentage:                 returns a true value for passed percentage,
 *                                      false value for failed percentage. Values
 *                                      below 1 always return false, value above 100
 *                                      always return true. 50 is a coin flip.
 *          
 * 
 *          > random.UUID()             || 0 overloads
 * 
 *          no args                     generates a UUID, useful for getting a near-
 *                                      unique ID. Chances of an ID clash are almost
 *                                      non-existant. See: https://en.wikipedia.org/wiki/Universally_unique_identifier
 *                                      Return value is a non-standard 64-bit int.
 *                                                                    
 * 
 *      ***          <---TO DO--->          ***
 * 
 *      Implement 128-bit UUID.
 * 
 *      Improve error handling.
 * 
 * 
 */

/** 
 *   @file random number generators
 */

#ifndef     RANDOM_NUMBER_GENERATOR
#define     RANDOM_NUMBER_GENERATOR 1

#include    <random>
#include    <chrono>
#include    <cstdint>

/**
 * 
 * @brief   Class for generating random numbers.
 *          See member functions Random() and instance() for more information how numbers are obtained.
 *          Class is a singleton instance.
 *          Create with this syntax: Random random = Random::instance();
 * 
 * @param   (empty)
 *          No argument defaults to system time seed for gen.
 *          Overloads with manual seed.
 * @param   seed
 *          Constructor takes a manual seed.
 *          Mostly for testing purposes.
 * 
 */
class Random
{
private:
    /**
     * 
     * @brief    Private constructor prevents creating more than one instance. Ensures singleton pattern, see getInstance() for explanation why singleton pattern is desired.
     *           Obtains a seed via system time in nanoseconds, then seeds the Mersenne Twister engine, which powers all random functions within the class.
     */
    Random()
    {
        gen.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    }
    Random(uint64_t seed)
    {
        gen.seed(seed);
    }
public:
    /**
     * 
     * @brief Creates an object with singleton pattern.
     * This pattern ensures that the same seed is used throughout the program's run-time.
     * Obtaining multiple seeds quickly, by seeding within a function, can produce the same value multiple times in a row.
     * This is because the seed is obtained via system time in nanoseconds, but functions can be called multiple times per nanosecond.
     * Therefore the same input will produce the same output, and any function called more than once per nanosecond will return the same value multiple times in a row.
     * 
     */
    static Random& instance()
    {
        static Random instance;
        return instance;    
    }
    static Random& instance(uint64_t seed)
    {
        static Random instance(seed);
        return instance;    
    }
    /**
     * @brief An object of Marsenne Twister type. 
     */
    std::mt19937 gen;
    /**
     * @name Uniform Distribution Functions
     */
    /**
     * 
     * @brief   Returns a random value from within specified range.
     * 
     * @tparam  T
     *          Specifies return type, and data type of arguments __min and __max.
     * 
     * @param   __min
     *          Lowest bound of range for return value.
     * @param   __max
     *          Highest bound of range for return value.
     * 
     * @return  Type determined by template parameter.
     * 
     * @warning __min and __max argument data types must be equivalent.
     * 
     */
    template <typename T> T number(T __min, T __max)
    {
        std::uniform_real_distribution<double> dist(__min, __max);
        return static_cast<T>(dist(gen));
    }
    template <typename T> T number(T __min, size_t __max)
    {
        std::uniform_real_distribution<double> dist(__min, __max);
        return static_cast<T>(dist(gen));
    }

    #ifdef _GLIBCXX_VECTOR
    /**
     * 
     * @brief   Returns a random value from a vector.
     * 
     * @tparam  T
     *          Specifies return type, and vector type.
     * 
     * @param   __vec
     *          Vector used as distribution.
     *          Obtained by copy.
     * 
     * @return  Type determined by template parameter.
     * 
     */
    template <typename T> T number(std::vector<T> __vec)
    {
        std::uniform_real_distribution<double> dist(0, __vec.size() - 1);
        return __vec[static_cast<int>(dist(gen))];
    }
    #endif
    /**
     * @name Bell Curve Distribution Functions
     */
    /**
     * 
     * @brief   Returns a value from a bell-curve distribution. Standard deviation optional.
     * 
     * @tparam  T
     *          Specifies return type, and data type of arguments __min and __max.
     * 
     * @param   __min
     *          Lowest bound of range for return value.
     * @param   __max
     *          Highest bound of range for return value.
     * @param   __mean
     *          Specified mean value.
     *          The peak of the distribution, return values will cluster around this value.
     * @param   __std_dev
     *          Optional (default: 10).
     *          Specified standard deviation, determines spread of possible return values, higher values result in greater deviations from the mean.
     * 
     * @return  Type determined by template parameter.
     * 
     * @warning __min and __max argument data types must be equivalent.
     * 
     */
    template <typename T> T weighted_number(T __min, T __max, double __mean, double __std_dev)
    {
        std::normal_distribution<double> dist(__mean, __std_dev);
        T v = static_cast<T>(dist(gen));
        while (v < __min || v > __max) {
            v = static_cast<T>(dist(gen));
        }
        return v;
    }
    /**
     * 
     * @brief   Returns a value from a bell-curve distribution. Standard deviation optional.
     * 
     * @tparam  T
     *          Specifies return type, and data type of arguments __min and __max.
     * 
     * @param   __min
     *          Lowest bound of range for return value.
     * @param   __max
     *          Highest bound of range for return value.
     * @param   __mean
     *          Specified mean value.
     *          The peak of the distribution, return values will cluster around this value.
     * @param   __std_dev
     *          Optional (default: 10).
     *          Specified standard deviation, determines spread of possible return values, higher values result in greater deviations from the mean.
     * 
     * @return  Type determined by template parameter.
     * 
     * @warning __min and __max argument data types must be equivalent.
     * 
     */
    template <typename T> T weighted_number(T __min, T __max, double __mean)
    {
        std::normal_distribution<double> dist(__mean, 10.0);
        T v = static_cast<T>(dist(gen));
        while (v < __min || v > __max) {
            v = static_cast<T>(dist(gen));
        }
        return v;
    }
    /**
     * 
     * @brief   Returns a value from a bell-curve distribution. Standard deviation optional.
     * 
     * @tparam  T
     *          Specifies return type, and data type of __mean parameter.
     * 
     * @param   __mean
     *          Specified mean value.
     *          The peak of the distribution, return values will cluster around this value.
     * 
     * @param   __std_dev
     *          Optional (default: 10).
     *          Specified standard deviation, determines spread of possible return values, higher values result in greater deviations from the mean.
     * 
     * @return  Type determined by template parameter.
     * 
     * @warning Since no range is specified, returned value may not be acceptable.
     *          Specify __min and __max values if range required.
     * 
     */
    template <typename T> T weighted_number(T __mean, double __std_dev)
    {
        std::normal_distribution<double> dist(__mean, __std_dev);
        return static_cast<T>(dist(gen));
    }
    /**
     * 
     * @brief   Returns a value from a bell-curve distribution. Standard deviation optional.
     * 
     * @tparam  T
     *          Specifies return type, and data type of __mean parameter.
     * 
     * @param   __mean
     *          Specified mean value.
     *          The peak of the distribution, return values will cluster around this value.
     * 
     * @return  Type determined by template parameter.
     * 
     * @warning Since no range is specified, returned value may not be acceptable.
     *          Specify __min and __max values if range required.
     *          No standard deviation specified, default value of 10.0 used.
     * 
     */
    template <typename T> T weighted_number(T __mean)
    {
        std::normal_distribution<double> dist(__mean, 10.0);
        return static_cast<T>(dist(gen));
    }
    #ifdef _GLIBCXX_VECTOR
    /**
     * 
     * @brief   Returns a size_t from a vector of weights.
     *          Useful for getting a weighted index for a vector.
     * 
     * @tparam  T
     *          Value of vector elements.
     * 
     * @param   __v
     *          Vector of weights.
     * 
     * @return  Size_t for vector index.
     * 
     * @warning Cannot return values higher than __v.size() - 1;
     * 
     */
    template <typename T> size_t weighted_number(std::vector<T> __v)
    {
        std::discrete_distribution<> __d(__v.begin(), __v.end());
        return __d(gen);
    }
    /**
     * 
     * @brief   Returns a size_t from a vector of weights.
     *          Useful for getting a weighted index for a vector.
     * 
     * @tparam  T
     *          Value of vector elements.
     * 
     * @param   __vec
     *          Vector of weights.
     * 
     * @return  An int representing an index on the vector.
     * 
     * @warning Will only return ints within range of vector index.
     * 
     */   
    template <typename T1, typename T2> size_t weighted_number(std::vector<T1> __v, std::vector<T2> __w)
    {
        std::discrete_distribution<> __d(__w.begin(), __w.end());
        return __d(gen) * (__v.size() / __w.size());

    }
    template <typename T> size_t index(std::vector<T> __v)
    {
        std::uniform_real_distribution<double> dist(0, __v.size() - 1);
        return static_cast<size_t>(dist(gen));
    }
    #endif
    /**
     * @name Percentage Functions
     */
    /**
     * 
     * @brief   Tries a percentage chance, argument determines chance to succeed.
     * 
     * @tparam  T
     *          Determines parameter data type.
     * 
     * @param   __x
     *          Percentage to try.
     *          Higher values succeed more often.
     * 
     * @return  Boolean value.
     *          True is a successful roll, false is a failed roll.
     * 
     * @warning Numbers below 0 or above 100 will be clamped to within this range, clamped values are not random.
     *          0  will always fail, 100 will always succeed.
     * 
     */
    template <typename T> bool percentage(T __x)
    {
            std::max(T(0), std::min(T(100), __x));
            T randomNumber = number(0, 100);
            return randomNumber < __x;
    }
    /**
     * @name Unique ID Generator Functions
     */
    /**
     * 
     * @brief   Generates a random 64-bit unsigned integer.
     *          For use in acquiring a unique ID.
     * 
     * @return  uint64_t value.
     *          uint64_t is the largest possible int storage in the C++ standard library.
     * 
     * @warning With 500 million generated IDs there is a 0.7% chance of an ID collision.
     *          With 4 billion generated IDs there is a 35% chance of an ID collision.
     * 
     */

    uint64_t UUID()
    {
        std::uniform_int_distribution<uint64_t> dist(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max());
        return dist(gen);
    }

    // if defined boost lib
    // boost func
    // else if defined gnupm lib
    // gnupm func
    // else standard uuid
};

#endif