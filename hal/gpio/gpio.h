/*
 * gpio.h
 *
 *  Created on: Aug 28, 2018
 *      Author: zhanlei
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include <string>
class Gpio {
public:
	typedef enum {
		E_OUT,
		E_IN
	}Direction;

	Gpio(uint32_t number);
	virtual ~Gpio();

	bool do_export();
	bool do_unexport();
	bool set(bool sync = false);
	bool reset(bool sync = false);
	bool get(uint32_t &val);
	bool set_direction(Direction direction);

private:
	uint32_t m_number;
	int m_value_fd;
	std::string m_gpio_path;

};

#endif /* GPIO_H_ */
