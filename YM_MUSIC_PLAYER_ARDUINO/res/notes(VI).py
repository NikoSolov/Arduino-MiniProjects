f=open("LOGYM.txt", "w")
freqs=[]
i=-12*4+3
while (440)*(2**(i/12))<4000:
    freqs.append((440)*(2**(i/12)))
    i+=1

freq_master=1777777#2000000 
saved=0
j=1
signs=["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#","A","A#","B"]
for i in range(len(freqs)):
    min=100000000
    for code in range(4096):
#        print("-",freq)
#        print("*", code, freq_master/(16*(code+1)), freq-freq_master/(16*(code+1)))
        freq=freqs[i]-freq_master/(16*(code+1))
        if min>abs(freq):
            saved=code
            min=abs(freq)
    if i%12==0: f.write("\n "+str(j)+" octave -------------\n"); j+=1
    f.write(signs[i%12]+str(j-1)+"\t"+hex(saved)+"\t"+str(freqs[i])+"\t"+str(min)+"\t"+str(freq_master/(16*(saved+1)))+"\n")
f.close()
