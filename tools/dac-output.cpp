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
        beagle_daq bd;

        int t = 0;
        while (true) {
                bd.dacs[0]->write(dac8568::write_cmd::write_mode::WRITE_UPDATE_ALL, 0, 10000*sin(t));
                t += 1;
                usleep(10*1000);
        }
}

