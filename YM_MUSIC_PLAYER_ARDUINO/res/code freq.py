freq_master=1777777#8000000
file=open("log6.txt", "w")
file.write(str(freq_master)+"\n")
file.write("code\tfreq(hz)\n")
for code in range(65536): #4096
#    file.write(hex(code)+"\t"+str(freq_master/(16*(code+1)))+"\n")
    file.write(hex(code)+"\t"+str(freq_master/((code+1)))+"\n")
print("done")
file.close()
