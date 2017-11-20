all:input readm readl readc readg readr count random random2 phase analy.out regress.out data_processor.out

input:input.c
	gcc -o input input.c 
readl:readl.c
	gcc -o readl readl.c
readc:readc.c
	gcc -o readc readc.c
readg:readg.c
	gcc -o readg readg.c
readm:readm.c
	gcc -o readm readm.c
readr:readr.c
	gcc -o readr readr.c
count:count.c
	gcc -o count count.c
random:random.c
	gcc -o random random.c
random2:random2.c
	gcc -o random2 random2.c
phase:phase.c
	gcc -o phase phase.c
analy.out:test2.cpp
	g++ test2.cpp -o analy.out
regress.out:regr.cpp
	g++ regr.cpp -o regress.out
data_processor.out:data_processor.cpp
	g++ data_processor.cpp -o data_processor.out
clean:     
	rm -rf input readl readm readc readg readr count random phase analy.out regress.out data_processor.out
