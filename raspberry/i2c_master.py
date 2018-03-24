import smbus
import time

bus = smbus.SMBus(1)
address = 0x12

i = 0

while 1:
  bus.write_byte(address, 1)
  time.sleep(1)
  response = bus.read_i2c_block_data(address, 0x01, 5)
  print(response)