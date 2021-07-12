import os

arr = []

lineCount = sum(1 for line in open("all_month.csv", "r"))

def readFile(start, end):
        global header
        counter = 0

        file_name = open("all_month.csv", "r")
        for lines in file_name:
                if counter==0:
                        header = lines
                elif counter >= start:
                        arr.append(lines.strip().split(','))

                counter+=1
                if counter == end:
                        break

        file_name.close()



def sortFile(process):
        no_rows = len(arr)
        cnt = 0
        for i in range(1, no_rows):
                for j in range(0, no_rows-i):

                        try:
                                if float(arr[j+1][1]) < float(arr[j][1]):
                                        tmp=arr[j]
                                        arr[j]=arr[j+1]
                                        arr[j+1]=tmp
                        except (IndexError):
                                print "IndexError Error found for", str(j+1), str(len(arr)), " processID=",str(process)
                cnt += 1
                if cnt%1000==0 or cnt==no_rows-1:
                        print str(cnt), "lines sorted by", str(process)



def fileDump(name):
        outFile = open(name,'w');
        outFile.write(header)
        for lines in arr:
                outFile.write(",".join([x for x in lines])+"\n")
        outFile.close()


def mergeFiles():
        global arr,header
        file_list = [open("first_child.txt",'r'), open("second_child.txt",'r'), open("Third_child.txt",'r')]
        arr = []
        for files in file_list:
                counter = 0
                for lines in files:
                        if counter > 0:
                                arr.append(lines.strip().split(','))
                        else:
                                header = lines
                        counter+=1
                files.close()


if __name__=="__main__":
        p1=os.fork()
        p2=os.fork()
        id = os.getpid()
        n=lineCount//3
        if p1==0 and p2>0:
                start, end = 1, n
                print "first_child Process ",str(id)," starts\n"
                print "first_child Process_id(id=",str(id),") picks ",str(n)," records for sorting\n"
                readFile(start, end)
                sortFile(id)
                fileDump("first_child.txt")
                print "first_child Process_id(id=",str(id),") ends sorting rows...\n"
                print "first_child Process ",str(id)," ends\n"
                os.wait()
        elif p1>0 and p2==0:
                start, end = n, n*2
                print "second_child Process ",str(id)," starts\n"
                print "second_child Process_id(id=",str(id),") picks ",str(n)," records for sorting\n"
                readFile(start, end)
                sortFile(id)
                fileDump("second_child.txt")
                print "second_child Process_id(id=",str(id),") ends sorting rows...\n"

        elif p1==0 and p2==0:
                start, end = (n*2), lineCount
                print "Third_child Process ",str(id)," starts\n"
                print "Third_child Process_id(id=",str(id),") picks ",str(n)," records for sorting\n"
                readFile(start, end)
                sortFile(id)
                fileDump("Third_child.txt")
                print "Third_child Process_id(id=",str(id),") ends sorting rows...\n"
        elif p1>0 and p2>0:
                print "Parent process",str(id)," starts\n"
                processes = [p1,p2]
                while processes:
                        prc=processes
                        for p in prc:
                                print "PARENT: Waiting for %s" % p
                                done = os.waitpid(p, 0)
                                print "PARENT: Child done:", done
                                if done[0]:
                                        processes.remove(p)

                print "HUrrAh!! All children are finished and first_child.txt, second_child.txt, Third_child.txt are created\n\n"
                print "It's time to sort all three children files files"
                mergeFiles()
                sortFile(id)
                fileDump("sorted_final.txt")
                print "Merging and sorting done...\nExecv() is next"
                os.execv("./exec", ("NULL",))
                print("Something's not right...")

        else:
                print "Something's not right"
