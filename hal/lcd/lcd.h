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
	enum Color {
		E_BLUE = 0,
		E_WHITE = 1,
	};

	virtual ~Lcd();
	static Lcd &instance();
	bool init();
	bool sync();
	void clear();

	void draw_point(int x, int y, Color color);
	void draw_v_line(int x, int y0, int y1, Color color);
	void draw_h_line(int x0, int x1, int y, Color color);
	void draw_line(int x0, int y0, int x1, int y1, Color color);
	void draw_bitmap_1(int x0, int y0, int w, int h, const char *data, bool inverse = false);
	void fill_rect(int x0, int y0, int x1, int y1, Color color);

	void flush_ram(int column, int row, size_t total_column, size_t total_row);
	void set_ram(int column, int row, size_t total_column, const uint8_t *src, size_t size);

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

	Lcd(uint32_t i2c, uint8_t addr, uint32_t reset_pin, uint32_t bkl_pin);
	Lcd() = delete;
	bool send_command(uint8_t cmd, const uint8_t *data, uint32_t len);
	bool write_ram(const uint8_t *data, uint32_t len);
	bool send_data(void *data, size_t len);
	void write_buf_byte(int index, Color color, uint8_t d);
};

#endif /* LCD_H_ */
