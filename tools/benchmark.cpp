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

#include <time.h>
#include <unistd.h>
#include "beagledaq.h"

int main(int argc, const char** argv)
{
        unsigned int adc = 2, dac = 2;
        timespec ts1, ts2;
        float rate;
        beagle_daq bd;

        // ADC test
        clock_gettime(CLOCK_MONOTONIC, &ts1);
        for (int i=0; i<1000000; i++) {
                bd.trigger_acquire();
                bd.adcs[adc]->read();
        }
        clock_gettime(CLOCK_MONOTONIC, &ts2);
        rate = (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec)*1e-9;
        printf("ADC reads: %f / sec\n", rate);

        // DAC test
        clock_gettime(CLOCK_MONOTONIC, &ts1);
        for (int i=0; i<1000000; i++) {
                bd.dacs[dac]->write(dac8568::write_cmd::write_mode::WRITE_UPDATE, 1, 0x7fff);
        }
        clock_gettime(CLOCK_MONOTONIC, &ts2);
        rate = (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec)*1e-9;
        printf("DAC writes: %f / sec\n", rate);
}

