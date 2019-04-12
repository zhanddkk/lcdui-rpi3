/*
 * lcd.h
 *
 *  Created on: Aug 28, 2018
 *      Author: zhanlei
 */

#ifndef LCD_H_
#define LCD_H_

#include <string>

class Gpio;
class Lcd {
public:
	Lcd(uint32_t i2c, uint8_t addr, uint32_t reset_pin, uint32_t bkl_pin);
	virtual ~Lcd();
	bool init();
	bool sync();

private:
	typedef struct {
		uint8_t control_data;
		uint8_t cmd;
	}CommandWord;
	Gpio *m_reset_pin;
	Gpio *m_backlight_pin;
	uint8_t m_i2c_addr;
	int m_i2c_fd;
	std::string m_i2c_dev_path;
	uint8_t *m_frame_buffer;
	uint8_t *m_frame_mem_ptr;
	size_t m_frame_buffer_size;

	bool send_command(uint8_t cmd, const uint8_t *data, uint32_t len);

	bool write_ram(const uint8_t *data, uint32_t len);

	bool send_data(void *data, size_t len);
};

#endif /* LCD_H_ */
