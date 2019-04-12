/*
 * gpio.cpp
 *
 *  Created on: Aug 28, 2018
 *      Author: zhanlei
 */
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "gpio.h"

Gpio::Gpio(uint32_t number)
{
	char buf[10];
	int fd;
	ssize_t n = snprintf(buf, sizeof(buf), "%u", number);
	m_gpio_path = "/sys/class/gpio/gpio";
	m_gpio_path += buf;
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd < 0) {
		printf("open export file for gpio %u failed\n", number);
		exit(-1);
	}

	n = write(fd, buf, n);
	if (n < 0) {
		if (access(m_gpio_path.c_str(), F_OK) == -1) {
			printf("export gpio %u failed\n", number);
			exit(-1);
		}
	} else {
		fsync(fd);
		int check_time = 10;
		while (check_time > 0) {
			usleep(100000);
			check_time--;
			if (access(m_gpio_path.c_str(), F_OK) == 0) {
				break;
			}
		}
	}
	close(fd);
	std::string value_path = m_gpio_path + "/value";
	m_value_fd = open(value_path.c_str(), O_RDWR);
	if (m_value_fd < 0) {
		printf("can\'t open the value file of gpio %u %d %s\n", number, m_value_fd, value_path.c_str());
		exit(-1);
	}
	m_number = number;
}

Gpio::~Gpio()
{
	if (m_value_fd >= 0) {
		close(m_value_fd);
	}
}

bool Gpio::set(bool sync)
{
	lseek(m_value_fd, 0, SEEK_SET);
	ssize_t n = write(m_value_fd, "1", 1);
	if (sync) {
		fsync(m_value_fd);
	}
	return n == 1;
}

bool Gpio::reset(bool sync)
{
	lseek(m_value_fd, 0, SEEK_SET);
	ssize_t n = write(m_value_fd, "0", 1);
	fsync(m_value_fd);
	if (sync) {
		fsync(m_value_fd);
	}
	return n == 1;
}

bool Gpio::get(uint32_t &val)
{
	char _val;
	bool ret_val(false);
	lseek(m_value_fd, 0, SEEK_SET);
	ssize_t n = read(m_value_fd, &_val, 1);
	if (n == 1) {
		ret_val = true;
		val = _val == '0'? 0 : 1;
	}
	return ret_val;
}

bool Gpio::set_direction(Direction direction)
{
	bool ret_val(false);
	std::string _path = m_gpio_path + "/direction";
	int fd = open(_path.c_str(), O_RDWR);
	if (fd >= 0) {
		std::string _tmp_val;
		switch (direction) {
		case E_IN:
			_tmp_val = "in";
			break;
		case E_OUT:
			_tmp_val = "out";
			break;
		default:
			break;
		}
		if (_tmp_val.empty()) {
			printf("invalid argument for direction\n");
		} else {
			ssize_t n = write(fd, _tmp_val.c_str(), _tmp_val.length());
			ret_val = n >= 0? (size_t)n == _tmp_val.length() : false;
		}
		close(fd);
	} else {
		printf("can\'t open the direction file of gpio %u\n", m_number);
	}
	return ret_val;
}

