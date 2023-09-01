#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_


#include "main.h"
class SerialPort
{
public:
    /** SerialPort
     *  @param[in] port Path to the serialport.
     *  @param[in] baudRate Serial baud rate configuration.
     *  @param[in] stopBits Number of stop bits (1 or 2).
     */
    SerialPort(const char *port, unsigned baudRate = 57600, unsigned stopBits = 1, bool canonical_mode = false);
    ~SerialPort();

    /** Write a buffer.
     * @param[in] buf Output buffer.
     * @param[in] nBytes Number of bytes to be written.
     * @return Number of bytes read, negative on error.
     */
    int write(const char *buf, size_t nBytes);

      /** Write a buffer.
     * @param[in] buf Output buffer.
     * @param[in] nBytes Number of bytes to be written.
     * @return Number of bytes read, negative on error.
     */
    int writebyte(const uint8_t *buf, size_t nBytes);


    /** Wrire a std::string.
     * @param[in] string String to be written.
     * @return Number of bytes written, negative on error.
     */
    int write(const std::string& string);

    /** Read one byte.
     * @return 1 byte data
     */
    uint8_t readbyte(void);
	
    int32_t Available(void);
protected:
    int mFileDesc; /**< File descriptor */

private:
    struct termios mOriginalTTYAttrs; /**< Original termios options */
};

#endif /* _SERIALPORT_H */
