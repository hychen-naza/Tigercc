  void GraphColor(std::map<Webs, std::set<Webs> > &map, std::map<std::string, std::vector<Webs>> &varWebMap, bool isIntMap, int lineNum) {
    // free dead var in previous scope or caller, and begin new graph analysis
    // now we still in previous curscope curreg
    for(auto it = curScope->curReg->varWebMap.begin(); it !=curScope->curReg->varWebMap.end(); ++it){
      Webs w = it->second;
      bool varLive = false;
      for(auto lscope : w.liveScopes){
        if(lineNum >= lscope.liveBegin && lineNum <= lscope.liveEnd){
          varLive = true;
          break;
        }
      }
      if(!varLive){
        curScope->curReg->varWebMap.erase(it);
        FreeWebsReg(w);
      }
    }
    // all available reg for this graph coloring
    std::vector<std::pair<int, bool> > regVec;
    if(isIntMap){
      for(int i=0;i<totalIntReg;++i){
        // means this reg is available
        if(!intRegsUsed[i]) regVec.push_back(std::make_pair(i, false));
      }
    }
    if(!isIntMap){
      for(int i=0;i<totalFloatReg;++i){
        if(!floatRegsUsed[i]) regVec.push_back(std::make_pair(i, false));
      }
    }
    int regNum = regVec.size();
    // 需要regIdx vector
    std::map<Webs, int> colorMap;
    for(auto it = map.begin(); it != map.end(); ++it){
      colorMap[it->first] = -1;
    }
    // copy graph
    std::map<Webs, std::set<Webs> > copyMap;
    for (auto it = map.begin(); it != map.end(); it++) {
      std::set<Webs> copySet;
      for (auto web : it->second) {
        copySet.insert(web);
      }
      copyMap[it->first] = copySet;
    }

    std::vector<Webs> stack;
    while (true) {
      // While graph G has a node N with degree less than R
      while (true) {
        Webs node;
        bool hasNodeSmaller = false;
        for(auto it = copyMap.begin(); it != copyMap.end(); ++it){
          if (it->second.size() < regNum) {
            node = it->first;
            hasNodeSmaller = true;
            break;
          }
        }
        if (hasNodeSmaller == false) break;
        // push N on a stack S
        stack.push_back(node);
        // Remove N and its associated edges from G 
        copyMap.erase(node);
        for(auto it = copyMap.begin(); it != copyMap.end(); ++it){
          if (it->second.find(node) != it->second.end()) {
            it->second.erase(node);
          }
        }
      }
      //If the entire graph has been removed then the graph is R-colorable 
      if (copyMap.size() == 0) {
        while (!(stack.size() == 0)) {
          Webs top = stack.back();
          stack.pop_back();
          // since every node has its 
          for (int i = 0; i < regNum; i++) {
            regVec[i].second = false;
          }
          for (Webs neighbor : map[top]) {
            if (colorMap[neighbor] >= 0) {
              regVec[colorMap[neighbor]].second = true;
            }
          }
          for (int i = 0; i < regNum; i++) {
            // if regs[i] is false, then it can be used
            if (!regVec[i].second) {
              colorMap[top] = regVec[i].first;
              break;
            }
          }
        }
        // done with coloring
        break;
      } 
      else {
        // 选取第一个spill。但是实际上应该选取spill cost最大的那个
        // spill时要加上sw和free reg
        Webs toBeSpilled = copyMap.begin()->first;
        // Simplify the graph G by choosing an object to spill and remove its node N from G
        copyMap.erase(toBeSpilled);
        for(auto it = copyMap.begin(); it != copyMap.end(); ++it){
          it->second.erase(toBeSpilled);
        }
      }
    }
    // connect web with register
    for(auto it = varWebMap.begin(); it != varWebMap.end(); ++it){
      for(auto iter = it->second.begin(); iter != it->second.end(); ++iter){
        int regIndex = (colorMap.find(*iter))->second;
        if(isIntMap) iter->regName = intRegsName[regIndex];
        else iter->regName = floatRegsName[regIndex];
      }
    }
  }



    void RegIRCodeGen(Scope *curscope, RegInfo *curReg){
    int lineNume = curReg->nodes.size();
    for(int i=0; i<lineNume; ++i){
      std::string ir = std::regex_replace((curReg->nodes[i])->content, std::regex(", "), ",");
      std::cout << "ir is " << ir << std::endl;
      if(ir.find("assign")==0){
        continue;
          //AssignInstAllocate(ir, curReg, i);
      }
      if(isBinaryInstruction(ir) || isBranchInstruction(ir)){
        ArithmeticInstAllocate(ir, curReg, i);
      }
    }
  }
  void ArithmeticInstAllocate(std::string ir, RegInfo *rinfo, int line){
    auto components = split(ir, ',');
    std::stringstream ss;
    for(auto s : components){
      std::string reg;

      if(FindVarReg(s, rinfo, line, reg)){
        ss << "," << reg;
      }
      else{
        ss << s;
      }
      /*auto it = curScope->curReg->varRegMap.find(s);
      // 那些var, temp, const已经被分配好了reg
      if(it != curScope->curReg->varRegMap.end()){
        ss << "," << it->second;
      }
      else{
        ss << s;
      }*/
    }
    std::cout << ss.str() << std::endl;
  }
  bool FindVarReg(std::string var, RegInfo *rinfo, int line, std::string &reg){
    if(rinfo->intWebMap.find(var) != rinfo->intWebMap.end()){
      std::cout << "we find var " << std::endl;
      auto webVec = rinfo->intWebMap[var];
      for(auto web : webVec){
        for(auto lscope : web.liveScopes){
          if(line >= lscope.liveBegin && line <= lscope.liveEnd){
            reg = web.regName;
            return true;
          }
        }
      }
    }
    else if(rinfo->floatWebMap.find(var) != rinfo->floatWebMap.end()){
      auto webVec = rinfo->floatWebMap[var];
      for(auto web : webVec){
        for(auto lscope : web.liveScopes){
          if(line >= lscope.liveBegin && line <= lscope.liveEnd){
            reg = web.regName;
            return true;
          }
        }
      }
    }
    return false;
  }




  bool AllocateVarReg(RegInfo *rinfo, std::string var, std::string &reg){
    // 目前还没有讨论spill的情况
    std::stringstream ss;
    if(IsIntNum(var)){
      reg = VarAllocateNTReg();
      ss << "\tli " << reg << "," << var;
      regCode.push_back(ss.str());
      return true;
    }
    if(IsFloatNum(var)){
      reg = VarAllocateFTReg();
      std::string floatVar = (gFloatVal.find(var))->second;
      ss << "\tl.s " << reg << "," << floatVar;
      regCode.push_back(ss.str());
      return true;
    }
    std::pair<int, Type> varPair;
    auto it = rinfo->varRegMap.find(var);
    // 这种已经分配了的是我预先插入了，那时值就已经在reg中了
    // 是跟函数的参数一样，自己手动加载到reg的
    if(it != rinfo->varRegMap.end()){
      reg = it->second;
      return true;
    }
    else{
      // 先得知道var类型, 这个var从curScope->curReg中的三个map中一定能找到
      if(GetVarInfo(var, varPair)){
        int stackPos = varPair.first;
        if(varPair.second.typeVal == "int"){
          reg = VarAllocateNTReg();
          ss << "\tlw " << reg << "," << stackPos << "($sp)";
          regCode.push_back(ss.str());
        }
        else if(varPair.second.typeVal == "float"){
          reg = VarAllocateFTReg();
          ss << "\tl.s " << reg << "," << stackPos << "($sp)"; 
          regCode.push_back(ss.str());
        }
        return true;
      }
      return false;
    } 
  }
  // 这些地方都没考虑spill
  std::string VarAllocateNTReg(std::string var){
    if(freeNTReg.size() > 0){
      std::string allocatedReg = freeNTReg[0];
      freeNTReg.erase(freeNTReg.begin());
      varRegMap.insert(std::make_pair(var, allocatedReg));
      return allocatedReg;
    }
    else{
      printf("Error no available register\n");
      return "Error";
    }
  }
  std::string VarAllocateFTReg(std::string var){
    if(freeFTReg.size() > 0){
      std::string allocatedReg = freeFTReg[0];
      freeFTReg.erase(freeFTReg.begin());
      varRegMap.insert(std::make_pair(var, allocatedReg));
      return allocatedReg;
    }
    else{
      printf("Error no available register\n");
      return "Error";
    }
  } 

  std::string NaiveAllocateNTReg(){
    if(freeNTReg.size() > 0){
      std::string allocatedReg = freeNTReg[0];
      freeNTReg.erase(freeNTReg.begin());
      return allocatedReg;
    }
    else{
      printf("Error no available register\n");
      return "Error";
    }
  }
  std::string NaiveAllocateFTReg(){
    if(freeFTReg.size() > 0){
      std::string allocatedReg = freeFTReg[0];
      freeFTReg.erase(freeFTReg.begin());
      return allocatedReg;
    }
    else{
      printf("Error no available register\n");
      return "Error";
    }
  }
  void NaiveFreeNTReg(std::string reg){
    freeNTReg.push_back(reg); 
  }
  void NaiveFreeFTReg(std::string reg){
    freeFTReg.push_back(reg);
  }

  void FreeVarReg(std::string var){
    auto it = curScope->curReg->varRegMap.find(var);
    if(it != curScope->curReg->varRegMap.end()){
      if(it->second.find("f") != string::npos){
        freeFTReg.push_back(it->second);
      }
      else{
        freeNTReg.push_back(it->second);
      }
      curScope->curReg->varRegMap.erase(it);
    }
  }