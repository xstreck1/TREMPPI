__author__ = 'adams_000'

#for i in range(0,64):
#    Bmal1 = (i >> 0) % 2
#    Cry1 = (i >> 1) % 2
#    Dbp = (i >> 2) % 2
#    Per2 = (i >> 3) % 2
#    Rev_erba = (i >> 4) % 2
#    Rory = (i >> 5) % 2
#    print('\t{\n\t\t"desc": [\n\t\t\t{\n\t\t\t\t"id": 0,\n\t\t\t\t"values": {\n\t\t\t\t\t"Name": "cycle_'+str(i)+'",\n\t\t\t\t\t"Type": "Cycle",\n\t\t\t\t\t"Experiment": "",\n\t\t\t\t\t"Verify": true\n\t\t\t\t}\n\t\t\t}\n\t\t],\n\t\t"data": [\n\t\t\t{\n\t\t\t\t"id": 0,\n\t\t\t\t"values": {\n\t\t\t\t\t"Measurement": "Bmal1='+str(Bmal1)+'&Cry1='+str(Cry1)+'&Dbp='+str(Dbp)+'&Per2='+str(Per2)+'&Rev_erba='+str(Rev_erba)+'&Rory='+str(Rory)+'"\n\t\t\t\t}\n\t\t\t},\n\t\t\t{\n\t\t\t\t"id": 1,\n\t\t\t\t"values": {\n\t\t\t\t\t"Measurement": "Bmal1='+str(1-Bmal1)+'&Cry1='+str(1-Cry1)+'&Dbp='+str(1-Dbp)+'&Per2='+str(1-Per2)+'&Rev_erba='+str(1-Rev_erba)+'&Rory='+str(1-Rory)+'"\n\t\t\t\t}\n\t\t\t}\n\t\t]\n\t},\n')

my_str = ""
for i in range(0,64):
    my_str += 'R_cycle_' + str(i) +","
print(my_str)