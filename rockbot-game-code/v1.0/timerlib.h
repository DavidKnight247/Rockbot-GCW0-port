#ifndef TIMERLIB_H
#define TIMERLIB_H

/**
 * @brief
 *
 */
class timerLib
{
public:
/**
 * @brief
 *
 */
    timerLib();
    /**
     * @brief
     *
     * @param int
     */
    void delay(int) const;
    /**
     * @brief
     *
     * @return unsigned int
     */
    unsigned int getTimer() const;

    void start_ticker(); // start counting ticks
    unsigned int get_ticks();

private:
    unsigned int _ticks;
};

#endif // TIMERLIB_H
