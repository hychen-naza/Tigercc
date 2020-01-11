
JAVA=java
OUTPUT=output
GENERATED=generated
GRAMMAR=tiger.g4

# here is where you plug in the runtime for your OS
RUNTIME=/usr/local/include
CC=g++
CCARGS=-c -I $(RUNTIME)/antlr4-runtime/ -I $(GENERATED) -std=c++11	
LDARGS=-g
LIBS=/usr/local/lib/libantlr4-runtime.a

all: tiger

tiger: dirs antlr4 tiger.cpp
	$(CC) $(CCARGS) tiger.cpp  -o $(OUTPUT)/tiger.o 
	$(CC) $(CCARGS) scope.cpp -o $(OUTPUT)/scope.o
	$(CC) $(CCARGS) FrontEndHelper.cpp -o $(OUTPUT)/FrontEndHelper.o
	$(CC) $(CCARGS) SymbolTable.cpp -o $(OUTPUT)/SymbolTable.o 
	$(CC) $(CCARGS) SemanticCheck.cpp  -o $(OUTPUT)/SemanticCheck.o
	$(CC) $(CCARGS) IRGeneration.cpp -o $(OUTPUT)/IRGeneration.o  
	$(CC) $(CCARGS) LivenessAnalysis.cpp -o $(OUTPUT)/LivenessAnalysis.o 
	$(CC) $(CCARGS) RegisterAllocation.cpp -o $(OUTPUT)/RegisterAllocation.o 
	$(CC) $(CCARGS) OptRegisterAllocation.cpp -o $(OUTPUT)/OptRegisterAllocation.o
	$(CC) $(CCARGS) CodeGeneration.cpp -o $(OUTPUT)/CodeGeneration.o 
	$(CC) $(CCARGS) BackEndHelper.cpp -o $(OUTPUT)/BackEndHelper.o 
	$(CC) $(CCARGS) $(GENERATED)/tigerBaseListener.cpp -o $(OUTPUT)/tigerBaseListener.o 
	$(CC) $(CCARGS) $(GENERATED)/tigerLexer.cpp -o $(OUTPUT)/tigerLexer.o 
	$(CC) $(CCARGS) $(GENERATED)/tigerListener.cpp -o $(OUTPUT)/tigerListener.o 
	$(CC) $(CCARGS) $(GENERATED)/tigerParser.cpp -o $(OUTPUT)/tigerParser.o 
	$(CC) $(LDARGS) -o tiger $(OUTPUT)/tiger.o $(OUTPUT)/scope.o $(OUTPUT)/FrontEndHelper.o $(OUTPUT)/SymbolTable.o $(OUTPUT)/SemanticCheck.o $(OUTPUT)/IRGeneration.o $(OUTPUT)/LivenessAnalysis.o $(OUTPUT)/RegisterAllocation.o $(OUTPUT)/OptRegisterAllocation.o $(OUTPUT)/CodeGeneration.o $(OUTPUT)/BackEndHelper.o $(OUTPUT)/tigerBaseListener.o $(OUTPUT)/tigerLexer.o $(OUTPUT)/tigerListener.o $(OUTPUT)/tigerParser.o $(LIBS) 	
	

antlr4: $(GRAMMAR)
	$(JAVA) -jar antlr-4.7-complete.jar -Dlanguage=Cpp -o $(GENERATED) $(GRAMMAR)

dirs:
	mkdir -p $(OUTPUT) 
	mkdir -p $(GENERATED) 

clean:
	rm -rf $(OUTPUT)
	rm -rf $(GENERATED)
