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
#include "beagledaq.h"

int main(int argc, const char** argv)
{
        if (argc != 2) {
                fprintf(stderr, "Usage: %s [adc]\n", argv[0]);
                exit(1);
        }
        unsigned int adc = atoi(argv[1]);
        if (adc > 4) {
                fprintf(stderr, "Invalid DAC or channel number\n");
                exit(1);
        }

        beagle_daq bd;
        while (true) {
                bd.trigger_acquire();
                array<int16_t,8> data = bd.adcs[adc]->read();
                for (int i=0; i<8; i++)
                        printf("%6d\t", data[i]);
                printf("\n");
                usleep(10*1000);
        }
}

