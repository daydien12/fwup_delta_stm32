#include "bts_serialport.h"

int connect(const char *port, unsigned baudRate, unsigned stopBits, bool canonical_mode, struct termios *originalTTYAttrs)
{
  int serial_port;
  struct termios tty;
  speed_t baud = baudRate;
  serial_port = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  if (serial_port < 0)
  {
    return -1;
  }

  memset(&tty, 0, sizeof tty);
  if (tcgetattr(serial_port, &tty) != 0)
  {
    return -1;
  }

  cfsetospeed(&tty, baud);
  cfsetispeed(&tty, baud);

  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
  tty.c_iflag &= ~IGNBRK;
  tty.c_iflag &= ~(INLCR | ICRNL);
  tty.c_lflag = 0;
  tty.c_oflag = 0;
  tty.c_cc[VMIN] = 0;
  tty.c_cc[VTIME] = 50;
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_cflag |= (CLOCAL | CREAD);
  tty.c_cflag &= ~(PARENB | PARODD);
  tty.c_cflag |= 0;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;

  if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
  {
    return -1;
  }
  return serial_port;
  return -1;
}
/**
 * @brief Constructs a new SerialPort object.
 *
 * This constructor creates a new SerialPort object with the specified serial port name, baud rate,
 * stop bits, and canonical mode. The `port` argument specifies the name of the serial port, such as
 * "COM1" on Windows or "/dev/ttyUSB0" on Linux. The `baudRate` argument specifies the desired baud
 * rate for the serial connection, such as 9600 or 115200. The `stopBits` argument specifies the number
 * of stop bits to use in the serial connection, typically 1 or 2. The `canonical_mode` argument specifies
 * whether to use canonical mode for input and output, which typically means that the serial driver will
 * handle certain special characters such as newline (\n) and backspace (\b) in a special way.
 *
 * @param port The name of the serial port to use.
 * @param baudRate The desired baud rate for the serial connection.
 * @param stopBits The number of stop bits to use in the serial connection.
 * @param canonical_mode Whether to use canonical mode for input and output.
 */
SerialPort::SerialPort(const char *port, unsigned baudRate, unsigned stopBits, bool canonical_mode)
{
  mFileDesc = connect(port, baudRate, stopBits, canonical_mode, &mOriginalTTYAttrs);

  if (mFileDesc == -1)
  {
    std::cout << "Connect Error: " << strerror(errno) << std::endl;
  }
}

SerialPort::~SerialPort()
{
  if (mFileDesc == -1)
  {
    return;
  }

  tcflush(mFileDesc, TCIOFLUSH);
  close(mFileDesc);
}

/**
 * @brief Writes data to the serial port.
 *
 * This function writes `nBytes` bytes of data from the buffer `buf` to the serial port. If the write
 * operation is successful, this function returns the number of bytes written. If an error occurs,
 * such as if the serial port is not open or if the write buffer is full, this function returns -1.
 *
 * @param buf A pointer to the buffer containing the data to write.
 * @param nBytes The number of bytes to write to the serial port.
 * @return The number of bytes written to the serial port, or -1 if an error occurs.
 */
int SerialPort::write(const char *buf, size_t nBytes)
{
  if (nBytes == 0)
  {
    return 0;
  }
  return ::write(mFileDesc, buf, nBytes);
}

/**
 * @brief Writes a string to the serial port.
 *
 * This function writes the contents of the `string` parameter to the serial port. If the write
 * operation is successful, this function returns the number of bytes written. If an error occurs,
 * such as if the serial port is not open or if the write buffer is full, this function returns -1.
 *
 * @param string The string to write to the serial port.
 * @return The number of bytes written to the serial port, or -1 if an error occurs.
 */
int SerialPort::write(const std::string &string)
{
  size_t len = string.length();
  if (len == 0)
  {
    return 0;
  }
  return ::write(mFileDesc, string.c_str(), len);
}

/**
 * @brief Writes an array of bytes to the serial port.
 *
 * This function writes `nBytes` bytes of data from the buffer `buf` to the serial port. If the write
 * operation is successful, this function returns the number of bytes written. If an error occurs,
 * such as if the serial port is not open or if the write buffer is full, this function returns -1.
 *
 * @param buf A pointer to the buffer containing the bytes to write.
 * @param nBytes The number of bytes to write to the serial port.
 * @return The number of bytes written to the serial port, or -1 if an error occurs.
 */
int SerialPort::writebyte(const uint8_t *buf, size_t nBytes)
{
  int ret = 0;
  if (nBytes == 0)
  {
    return 0;
  }
  return ::write(mFileDesc, buf, nBytes);
}

/**
 * @brief Reads a single byte from the serial port.
 *
 * This function reads a single byte of data from the serial port and returns it. If no data is
 * available in the input buffer, this function blocks until data becomes available. If an error
 * occurs, such as if the serial port is not open or if an I/O error occurs, this function returns
 * -1.
 *
 * @return The byte of data read from the serial port, or -1 if an error occurs.
 */
uint8_t SerialPort::readbyte(void)
{
  uint8_t c;
  ::read(mFileDesc, &c, 1);
  return c;
}

/**
 * @brief Returns the number of bytes available for reading from the serial port.
 *
 * This function returns the number of bytes of data that are currently available for reading from
 * the serial port. If an error occurs, such as if the serial port is not open or if an I/O error
 * occurs, this function returns -1.
 *
 * @return The number of bytes available for reading from the serial port, or -1 if an error occurs.
 */
int32_t SerialPort::Available(void)
{
  int32_t ret = 0;
  ioctl(mFileDesc, FIONREAD, &ret);
  return ret;
}
