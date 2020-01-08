JAVA=java
OUTPUT=output
GENERATED=generated
GRAMMAR=tiger.g4

# here is where you plug in the runtime for your OS
#RUNTIME=/usr/local/include
CC=g++
CCARGS=-c -I $(GENERATED) -std=c++11	
LDARGS=-g
#LIBS=/usr/local/lib/libantlr4-runtime.a

all: tiger

tiger: dirs tiger.cpp
	$(CC) $(CCARGS) tiger.cpp  -o $(OUTPUT)/tiger.o  
	$(CC) $(CCARGS) LivenessAnalysis.cpp -o $(OUTPUT)/LivenessAnalysis.o 
	$(CC) $(CCARGS) RegisterAllocation.cpp -o $(OUTPUT)/RegisterAllocation.o 
	$(CC) $(CCARGS) OptRegisterAllocation.cpp -o $(OUTPUT)/OptRegisterAllocation.o
	$(CC) $(CCARGS) BackEndHelper.cpp -o $(OUTPUT)/BackEndHelper.o  
	$(CC) $(LDARGS) -o tiger $(OUTPUT)/tiger.o $(OUTPUT)/BackEndHelper.o $(OUTPUT)/RegisterAllocation.o $(OUTPUT)/LivenessAnalysis.o $(OUTPUT)/OptRegisterAllocation.o $(LIBS) 


dirs:
	mkdir -p $(OUTPUT) 
	mkdir -p $(GENERATED) 

clean:
	rm -rf $(OUTPUT)
	rm -rf $(GENERATED)
