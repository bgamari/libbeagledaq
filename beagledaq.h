/* tracker - Back-focal plane droplet tracker
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

using std::array;

class dac8568 : spi_device {
private:
        uint32_t build_command(char* buf, uint8_t prefix, uint8_t control,
                        uint8_t addr, uint16_t data, uint8_t features)
        {
                buf[0] = (0xf & features) << 0
                       | (0xf & data) << 4;
                buf[1] = (0x0ff0 & data) >> 4;
                buf[2] = (0xf000 & data) >> 12
                       | (0xf & addr) << 4
                buf[3] = (0xf & control) << 0
                       | (0xf & prefix) << 4;
        }

public:
        dac8568(const char* dev): spi_device(dev) { }

	class command : public spi_device::command {
		virtual void pack(uint8_t* buf) { };
	};

        class write_cmd : public command {
                enum write_mode {
                        WRITE_SEL = 0x0,
                        UPDATE_SEL = 0x1,
                        WRITE_SEL_UPDATE_ALL = 0x02,
                        WRITE_UPDATE_SEL = 0x3,
                };
                write_mode mode;
                uint8_t addr;
                uint16_t data;

                void pack(uint8_t* buf) {
                        build_command(buf, 0, mode, addr, data, 0);
                }
        public:
                write_cmd(write_mode mode, uint8_t addr, uint16_t data) :
                        mode(mode), addr(addr), data(data) { }
        };

	void submit(std::vector<command*>& cmds) {
		spi_device::submit(cmds);
	}
};

class ad7606 : spi_device {
public:
        ad7607(const char* dev): spi_device(dev) { }

        array<uint16_t, 8> read() {
                array<uint16_t, 8> samples;
                uint8_t* buf = new uint8_t[2*8];
                send_msg(buf, buf, 2*8);

                uint8_t* b = buf;
                for (int i=0; i<8*2; i++) {
                        samples[i] = 0;
                        samples[i] |= (*b & 0xf) << 0; b++;
                        samples[i] |= (*b & 0xf) << 8; b++;
                }

                delete [] buf;
                return samples;       
        }
};

class beagle_daq {
        int start_acq_fd;
        array<ad7606*,4> adcs;
        array<dac8568*,4> dacs;

        beagle_daq() {
                start_acq_fd = open("/sys/class/gpio/gpio145/value", O_WRONLY);
                if (start_acq_fd < 0)
                        throw new std::runtime_error("Error opening start acquire gpio pin");

                for (int i=0; i<4; i++) {
                        adcs[i] = new ad7606((boost::format("/dev/spidev3.%d") % i).to_string());
                        dacs[i] = new dac8568((boost::format("/dev/spidev3.%d") % i).to_string());
                }
        }

        void trigger_acquire() {
                write(start_acq_fd, "1", 1);
                write(start_acq_fd, "0", 1);
        }
};

