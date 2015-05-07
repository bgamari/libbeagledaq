# coding: utf-8

from spidev import SpiDev

class ad7606():
    try:
        start_acq_fd = open("/sys/class/gpio/gpio145/value", 'w', 0)
    except IOError as e:
        print("Error opening start acquire gpio pin: %s" % e)
        raise

    def __init__(self, dev):
        self.dev = SpiDev()
        try:
            self.dev.open(3,dev)
            self.dev.mode = 2
        except IOError as e:
            print("Error opening /dev/spidev3.%d: %s" % (dev, e))
            raise

    def trigger_acquire(self):
        self.start_acq_fd.write(b'1')
        self.start_acq_fd.write(b'0')

    def read(self):
        self.trigger_acquire()
        buf = self.dev.readbytes(16)
        samples = [0,0,0,0,0,0,0,0]
        for i in xrange(8):
            samples[i] = buf[2*i] << 8 | buf[2*i+1] << 0
        return samples

class dac8568():
    def __init__(self, dev):
        self.dev = SpiDev()
        try:
            self.dev.open(4,dev)
        except IOError as e:
            print("Error opening /dev/spidev4.%d: %s" % (dev, e))
            raise

    def build_command(self, control, addr, data):
        prefix = 0
        features = 0
        cmd = [0,0,0,0]
        cmd[3] = (0xf    & features) << 0 | (0xf & data) << 4
        cmd[2] = (0x0ff0 & data) >> 4
        cmd[1] = (0xf000 & data) >> 12    | (0xf & addr) << 4
        cmd[0] = (0xf    & control) << 0  | (0xf & prefix) << 4
        return cmd
       
    # indexed by label number, mapping to DAC channel number
    channel_map = (0,2,4,6,7,5,3,1)
    write_mode = { 'WRITE': 0x0,
                   'UPDATE': 0x1,
                   'WRITE_UPDATE_ALL': 0x2,
                   'WRITE_UPDATE': 0x3 }

    def write(self, addr, data, mode = write_mode['WRITE_UPDATE']):
        addr = self.channel_map[addr]
        cmd  = self.build_command(mode, addr, data)
        self.dev.writebytes(cmd)

class beagle_daq():
    def __init__(self):
        self.adc0 = ad7606(0)
        self.dac0 = dac8568(0)

    def read(self, channel):
        return self.adc0.read()[channel]

    def write(self, channel, data):
        self.dac0.write(channel, data)
