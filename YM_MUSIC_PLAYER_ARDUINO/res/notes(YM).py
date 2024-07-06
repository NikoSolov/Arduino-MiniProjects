f=open("LOGYM.txt", "w")
freqs=[]
i=-12*3-9
while (440)*(2**(i/12))<8000:
    freqs.append((440)*(2**(i/12)))
    i+=1

freq_master=1777777 
saved=0
j=1
f.write(str(freq_master))
f.write("\n")
for i in range(len(freqs)):
    min=100000000
    for code in range(4096):
#        print("-",freq)
#        print("*", code, freq_master/(16*(code+1)), freq-freq_master/(16*(code+1)))
        freq=freqs[i]-freq_master/(16*(code+1))
        if min>abs(freq):
            saved=code
            min=abs(freq)
    if i%12==0: f.write("\n "+str(j)+" -------------\n"); j+=1
    f.write(str(i)+"\t"+hex(saved)+"\t"+str(freqs[i])+"\t"+str(min)+"\t"+str(freq_master/(16*(saved+1)))+"\n")
f.close()
