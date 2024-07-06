f=open("tracker.txt", "r")
g=open("result.txt", "w")

g.write("Vortex Tracker II v2.0 Pattern\n")
flag=False
falg=False
f.readline()
for i in f:
    m=0
    g.write("        ")
    for j in i:
        if m<4:
            if j=="|": n=0; flag=False; falg=False; m+=1
            if n<4:
                if j=="=" and falg==False: g.write("R"); falg=True; n+=1
                elif j=="=" and falg==True: g.write("-"); n+=1            
                elif j=="0" or j=="1" or j=="2" or j=="3" or j=="4" or j=="5" or j=="6" or j=="7" or j=="8" or j=="9": g.write(str(int(j)-1)); n+=1
                elif j==".": g.write("-"); n+=1
                else:
                    if (j=="|" and m==1) or (j=="|" and m==4): n+=1
                    else: g.write(j); n+=1
            elif flag==False: g.write(" .... ...."); flag=True

    g.write("\n")
    
f.close()
g.close()
        
