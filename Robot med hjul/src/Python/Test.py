import subprocess

#this for loop depends on ho wlong you are willing to wait. I am
# for i in range(10):   #look for up to 10 devices
#     command=['ping', '-n', '1', '192.168.0.1'+str(i)]   #icrement the device names
#     subprocess.call(command)   #ping the devices to update data before  "arp -a'

arpa = subprocess.check_output(("arp", "-a")).decode("ascii") #call 'arp -a' and get results

#I count lines that contain 192.1868, but not ['192.168.0.1 ','192.168.0.255'] 
#because those are the router and broadcast gateway. Note that the machine 
#you are running the code from will get counted because it will be in the 
#first line "Interface: 192.168.0.10 --- 0x4" in my case
n_devices=[x for x in arpa.split('\n') if '192.168' in x and  all(y not in x for y in ['192.168.0.1 ','192.168.0.255']) ]

for i in arpa.split('\n'):
     print(i)

print(len(n_devices))