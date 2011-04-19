/* BeagleDAQ - Data Acquisition System for the BeagleBoard
 *
 * Copyright © 2010 Ben Gamari
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/ .
 *
 * Author: Ben Gamari <bgamari@physics.umass.edu>
 */


#pragma once

#include <vector>
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>

#include <boost/format.hpp>

#include "spi_device.h"

using std::string;
using std::array;

class dac8568 : spi_device {
private:
        static uint32_t build_command(uint8_t* buf, uint8_t prefix, uint8_t control,
                        uint8_t addr, uint16_t data, uint8_t features)
        {
                buf[3] = (0xf & features) << 0
                       | (0xf & data) << 4;
                buf[2] = (0x0ff0 & data) >> 4;
                buf[1] = (0xf000 & data) >> 12
                       | (0xf & addr) << 4;
                buf[0] = (0xf & control) << 0
                       | (0xf & prefix) << 4;
        }

public:
        dac8568(string dev): spi_device(dev) { }

	class command : public spi_device::command {
		virtual void pack(uint8_t* buf) { };
	};

        class write_cmd : public command {
        public:
                enum write_mode {
                        WRITE = 0x0,
                        UPDATE = 0x1,
                        WRITE_UPDATE_ALL = 0x02,
                        WRITE_UPDATE = 0x3,
                };
        private:
                write_mode mode;
                uint8_t addr;
                uint16_t data;

                unsigned int length() { return 4; }
                void pack(uint8_t* buf) {
                        build_command(buf, 0, mode, addr, data, 0);
                }
                void unpack(const uint8_t* buf) { }
        public:
                write_cmd(write_mode mode, uint8_t addr, uint16_t data) :
                        mode(mode), addr(addr), data(data) { }
        };

	void submit(std::vector<command*>& cmds) {
		spi_device::submit(cmds);
	}

        void write(write_cmd::write_mode mode, uint8_t addr, uint16_t data)
        {
                write_cmd cmd(mode, addr, data);
                std::vector<command*> cmds = {&cmd};
                submit(cmds);
        }
};

class ad7606 : spi_device {
public:
        ad7606(string dev) : spi_device(dev) { set_mode(SPI_CPOL|0); }

        array<uint16_t, 8> read() {
                array<uint16_t, 8> samples;
                uint8_t* buf = new uint8_t[2*8];
                send_msg(buf, buf, 2*8);

                uint8_t* b = buf;
                for (int i=0; i<8; i++) {
                        samples[i] = 0;
                        samples[i] |= *b << 8; b++;
                        samples[i] |= *b << 0; b++;
                }

                delete [] buf;
                return samples;       
        }
};

class beagle_daq {
        int start_acq_fd;
public:
        array<ad7606*,4> adcs;
        array<dac8568*,4> dacs;

        beagle_daq() {
                start_acq_fd = open("/sys/class/gpio/gpio145/value", O_WRONLY);
                if (start_acq_fd < 0)
                        throw new std::runtime_error("Error opening start acquire gpio pin");

                for (int i=0; i<4; i++) {
                        adcs[i] = new ad7606((boost::format("/dev/spidev3.%d") % i).str());
                        dacs[i] = new dac8568((boost::format("/dev/spidev4.%d") % i).str());
                }
        }

        void trigger_acquire() {
                write(start_acq_fd, "1", 1);
                write(start_acq_fd, "0", 1);
        }
};

