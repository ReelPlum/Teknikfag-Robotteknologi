#pip install asyncio
#pip install bleak
#pip install ipaddress


import asyncio
from bleak import BleakScanner
from bleak import BleakClient
import time
import ipaddress

def check(a, b):
    if (not b.service_uuids):
        return
    
    if ('ce2240b9-dce8-4fb5-8a91-c58eb1905869' in b.service_uuids):
        return True
    
def Callback(sender, data):
    print(f'{sender}, {data}')

async def run():
    devices = await BleakScanner.find_device_by_filter(check)
    while (not devices):
        devices = await BleakScanner.find_device_by_filter(check)

    print(devices.name)
        
    async with BleakClient(devices.address) as Client:
        print("connecting!")
        print("Umm?")
        print(Client.mtu_size)
            
        print("Writing gatt")
        await Client.write_gatt_char("d6efa987-2a50-4c40-bf6e-953baf9b916c", b"IOT_NET:esp32esp", response=False)
            
        while True:
            data = await Client.read_gatt_char("d6efa987-2a50-4c40-bf6e-953baf9b916c")
            print(data)
            
            decodedData = data.decode('utf-8')
            print(decodedData)
            
            #Check if IP & port is recieved
            valid = True
            try:
                ip_object = ipaddress.ip_address(decodedData)
            except:
                valid = False
            
            print(valid)
            if valid:
                print(f"Got a valid ip-address {decodedData}")
                break
            break
            #await asyncio.sleep(2)
        
    print("Hi?")

loop = asyncio.get_event_loop()
loop.run_until_complete(run())