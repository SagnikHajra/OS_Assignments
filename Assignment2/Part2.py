from threading import *
import time

# creating thread instance where count = 3
obj = Semaphore(3)

# arr = []
lineCount = sum(1 for line in open("all_month.csv", "r"))

def readTheMainFile(arr, start, end):
    global header
    counter = 0

    file_name = open("all_month.csv", "r")
    for lines in file_name:
        if counter == 0:
            header = lines
        elif counter>= start:
            arr.append(lines.strip().split(','))
        counter += 1
        if counter > end:
            break
    file_name.close()

# creating instance
def sortFile(start, end, name):
    # calling acquire method
    obj.acquire()
    start_time = time.time()
    arr = []
    if end >0:
        readTheMainFile(arr, start, end)
        no_rows = end - start
    else:
        readTheTmpFile(arr)
        no_rows = lineCount

    cnt = 0
    # count = 0
    for i in range(1, len(arr)):
        for j in range(0, len(arr) - i):
            # count += 1
            try:
                if float(arr[j + 1][1]) < float(arr[j][1]):
                    tmp = arr[j]
                    arr[j] = arr[j + 1]
                    arr[j + 1] = tmp

            except IndexError:
                print "IndexError Error found for", str(j + 1), str(len(arr)), " ", name
        cnt += 1
        if cnt % 1000 == 0 or cnt == no_rows - 1:
            print str(cnt), "lines sorted by", name
    if end>0:
        file_dump(arr, name+".txt")
    else:
        file_dump(arr, "final_sorted_file.txt")
    print "Thread finished, time elapsed by ", name, " = ", str(time.time() - start_time), " sec"
    # calling release method
    obj.release()




def readTheTmpFile(arr):
    global header
    counter = 0

    file_list = [open("Thread_1.txt", 'r'), open("Thread_2.txt", 'r'), open("Thread_3.txt", 'r')]
    for files in file_list:
        counter = 0
        for lines in files:
            if counter > 0:
                arr.append(lines.strip().split(','))
            else:
                header = lines
            counter += 1
        files.close()


def file_dump(arr, name):
    outFile = open(name, 'w')
    outFile.write(header)
    for lines in arr:
        outFile.write(",".join([x for x in lines]) + "\n")
    print "Dump ",name, " taken"
    outFile.close()





if __name__ == "__main__":
    start_time = time.time()
    # read_the_file()
    # size = len(arr)
    n = lineCount//3
    arr_slice = [[0, n], [n, n+n], [n+n, lineCount]]
    # print(arr_slice)
    # creating multiple thread
    t1 = Thread(target=sortFile, args=(arr_slice[0][0], arr_slice[0][1], 'Thread_1',))
    t2 = Thread(target=sortFile, args=(arr_slice[1][0], arr_slice[1][1], 'Thread_2',))
    t3 = Thread(target=sortFile, args=(arr_slice[2][0], arr_slice[2][1], 'Thread_3',))

    t4 = Thread(target=sortFile, args=(0, 0, 'Thread_4',))
    # t5 = Thread(target=sortFile, args=(arr_slice[4][0], arr_slice[4][1], 'Thread_5',))

    # t5 = Thread(target=sortFile, args=(0, 0, 'Thread_5',))

    # calling the threads
    t1.start()
    t2.start()
    t3.start()
    #x = time.time()
    while activeCount() - 1 > 0:
        # if int(time.time() - x)>10:
        #    print "Running 1,2,3... Threads active=",str(activeCount() - 1)
            x = time.time()
        time.sleep(0.2)
        continue
    print "Time elapsed by Thread-1,2 and 3 combined = ", str(time.time() - start_time), " sec"
    start_time2 = time.time()
    t4.start()
    # #t5.start()
    #x = time.time()
    while activeCount() - 1 > 0:
        #if int(time.time() - x) > 10:
        #    print "Running 4... Threads active=", str(activeCount() - 1)
            x = time.time()
        time.sleep(0.2)
        continue
    print("Time elapsed by Thread-4 = ", str(time.time() - start_time2), " sec")
    print "Total Time = ", str(time.time() - start_time), " sec"