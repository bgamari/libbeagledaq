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

#include <unistd.h>
#include <cmath>
#include "beagledaq.h"

int main(int argc, const char** argv)
{
        if (argc < 3) {
                fprintf(stderr, "Usage: %s [dac] [channel] [amp] [freq]\n", argv[0]);
                exit(1);
        }
        unsigned int dac = atoi(argv[1]);
        unsigned int chan = atoi(argv[2]);
        if (dac > 4 || chan > 8) {
                fprintf(stderr, "Invalid DAC or channel number\n");
                exit(1);
        }
        unsigned int amp = 0x7fff;
        if (argc > 3)
                amp = atoi(argv[3]);

        float freq = 1; // Hz
        if (argc > 4)
                freq = atof(argv[4]);

        beagle_daq bd;
        int t = 0;
        while (true) {
                bd.dacs[dac]->write(dac8568::write_cmd::write_mode::WRITE_UPDATE, chan, amp*(1 + sin(2*M_PI * t * 1e-3 * freq)));
                t += 1;
                usleep(1*1000);
        }
}

