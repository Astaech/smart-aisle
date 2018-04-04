import smbus
import time
import datetime
from pymongo import MongoClient
from pprint import pprint

client = MongoClient('mongodb://admin:admin@mongo:27017/')
db=client.smartaisle
products=db.products
bus = smbus.SMBus(1)
address = 0x12
empty = [0, 0, 0, 0, 0]

def mongoEntry(entry):
  if products.find_one({'tag':entry}):
    products.find_one_and_update(
      {'tag':entry},
      {'$set':{'lastMod':datetime.datetime.utcnow()}}
    )
  else:
    products.insert({
      'tag':entry,
      'createdOn':datetime.datetime.utcnow()
    })

while 1:
  bus.write_byte(address, 1)
  time.sleep(1)
  response = bus.read_i2c_block_data(address, 0x01, 5)
  print(response)
  if response != empty:
    mongoEntry(response)