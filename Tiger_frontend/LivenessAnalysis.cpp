#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <set>
#include <map>
#include <iterator>
#include <iomanip>
#include "scope.cpp"
#include "BackEndHelper.h"

class  LivenessAnalysis{

  Scope *globalScope;
  int sNum;

public:
  LivenessAnalysis(Scope *scope){
    sNum = 0;   
    globalScope = scope;
    Scope *curscope = scope;
    curscope->curReg = &((curscope->regTable.find("main"))->second);
    LivenessAnalyze(curscope->mainCode, curscope, curscope->curReg);
  }

  void CheckIsOutScopeVar(std::string var, RegInfo *curReg, IRNode *cur, std::map<std::string, int> &outscopeVarDefs){
    if(curReg->outscopeVar.find(var) != curReg->outscopeVar.end()){
      auto it = outscopeVarDefs.find(var);
      if(it->second > cur->id){
        it->second = cur->id;
        cur->defs.insert(var);
      }
      else cur->uses.insert(var);
    }
    else cur->uses.insert(var);
  }

  void LivenessAnalyze(std::vector<std::string> stmts, Scope *curScope, RegInfo *curReg){
    std::map<std::string, int> Labels;
    // create a new IRNode for each line;
    for (int i = 0; i < stmts.size(); i++) {
      auto stmt = stmts[i];
      IRNode *cur = new IRNode(i, stmt);
      curReg->nodes.push_back(cur);
      // save location for each label
      if (isLabel(stmt)) {
        Labels[getLabelName(stmt)] = i;
      }
    }
    // Go over all IRcurReg->nodes. Find to, defs, ues, consts for each IRNode.
    std::map<std::string, int> outscopeVarDefs;
    for(auto it = curReg->outscopeVar.begin(); it != curReg->outscopeVar.end(); ++it){
      outscopeVarDefs.insert(std::make_pair(it->first, 999999));
    }
    for (unsigned i = 0; i < curReg->nodes.size(); i++) {
      IRNode* cur = curReg->nodes[i];
      std::string str = cur->content;
      str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
      std::vector<std::string> tokens = split(str, ',');
      if (isLabel(tokens[0])) {
        cur->to.push_back(i + 1);
      } 
      else if (tokens[0] == "assign") {
        cur->to.push_back(i + 1);
        cur->defs.insert(tokens[1]);
        if(tokens.size() == 3){
          if (IsFloatNum(tokens[2]) || IsIntNum(tokens[2])) {
            cur->consts.insert(tokens[2]);
          }
          else{
            // if this var is outscopeVar, we should def it frist for live range build
            CheckIsOutScopeVar(tokens[2], curReg, cur, outscopeVarDefs);
          }
        }
        else if(tokens.size() == 4){
          cur->consts.insert(tokens[3]);
        }
      } 
      else if (tokens[0]=="store" || tokens[0]=="load") {
        cur->to.push_back(i + 1);
        cur->defs.insert(tokens[1]);
        if (IsFloatNum(tokens[2]) || IsIntNum(tokens[2])) {
          cur->consts.insert(tokens[2]);
        }
        else cur->uses.insert(tokens[2]);
        if (IsFloatNum(tokens[3]) || IsIntNum(tokens[3])) {
          cur->consts.insert(tokens[3]);
        }
        else cur->uses.insert(tokens[3]);
      }
      else if (tokens[0]=="add" || tokens[0]=="sub" || tokens[0]=="mult" || tokens[0]=="div" || tokens[0]=="and" || tokens[0]=="or") {
        cur->to.push_back(i + 1);
        cur->defs.insert(tokens[3]);
        if (IsFloatNum(tokens[1]) || IsIntNum(tokens[1])) {
          cur->consts.insert(tokens[1]);
        }
        else CheckIsOutScopeVar(tokens[1], curReg, cur, outscopeVarDefs);
        if (IsFloatNum(tokens[2]) || IsIntNum(tokens[2])) {
          cur->consts.insert(tokens[2]);
        }
        else CheckIsOutScopeVar(tokens[2], curReg, cur, outscopeVarDefs);
      } 
      else if (tokens[0]=="breq" || tokens[0]=="brneq" || tokens[0]=="brlt" || tokens[0]=="brgt" || tokens[0]=="brgeq" || tokens[0]=="brleq") {
        cur->to.push_back(i + 1);
        cur->to.push_back(Labels[tokens[3]]);

        if (IsFloatNum(tokens[1]) || IsIntNum(tokens[1])) {
          cur->consts.insert(tokens[1]);
        }
        else cur->uses.insert(tokens[1]);
        if (IsFloatNum(tokens[2]) || IsIntNum(tokens[2])) {
          cur->consts.insert(tokens[2]);
        }
        else cur->uses.insert(tokens[2]);
      } 
      else if (tokens[0] == "goto") {
        cur->to.push_back(Labels[tokens[1]]);
      } 
      else if(tokens[0] == "call" || tokens[0] == "callr"){
        cur->to.push_back(i + 1);
        // we only do liveness analysis, so no need to add MIPS code here
        std::string funcName;
        int begin;
        if(tokens[0] == "call"){
          begin = 2;
          funcName = tokens[1];
        }
        else{
          cur->defs.insert(tokens[1]);
          begin = 3;
          funcName = tokens[2];
        }
        for(int i = begin; i < tokens.size(); ++i){
          if (IsFloatNum(tokens[i]) || IsIntNum(tokens[i])) {
            cur->consts.insert(tokens[i]);
          }
          else cur->uses.insert(tokens[i]);
        }
        if(funcName == "printi" || funcName == "prints" || funcName == "flush" || funcName == "getchar"){
          continue;
        }
        std::vector<std::string> funcInsts;
        std::string instBegin = "#start_function " + funcName;
        auto itBegin = find(curScope->funcCode.begin(),curScope->funcCode.end(), instBegin);    
        itBegin += 1;
        std::string instEnd = "#end_function " + funcName;
        auto itEnd = find(curScope->funcCode.begin(),curScope->funcCode.end(), instEnd);   
        funcInsts.assign(itBegin, itEnd);
        RegInfo *rinfo = &((curScope->regTable.find(funcName))->second);
        LivenessAnalyze(funcInsts, curScope, rinfo);
      }
      else if(tokens[0] == "return"){
        cur->uses.insert(tokens[1]);
      }
      // function decl
      else if(str.find("int")==0 || str.find("void")==0 || str.find("float")==0){
        cur->to.push_back(i + 1);
        std::string ir = std::regex_replace(str, std::regex("int|void|float| "), "");
        int pos1 = ir.find('(') + 1;
        int pos2 = ir.find(')');
        std::string param = ir.substr(pos1, pos2-pos1);
        auto paramVec = split(param, ',');
        for(auto s:paramVec){
            cur->defs.insert(s);
        }
      }
      else if(str.find("#enterscope")==0){
        cur->to.push_back(i + 1);
        Scope *s = globalScope;
        auto components = split(str,'_');
        for(int i=components.size()-1; i>=1; i--){
          int pos = std::stoi(components[i]);
          s = s->scopeKids[pos];
        }
        s->curReg = &((s->regTable.find("main"))->second);
        LivenessAnalyze(s->mainCode, s, s->curReg);
      }
      else if(str.find("#exitscope")==0){
        continue;
      }
    }
    // 最后一个指令的to是不能超过curReg->nodes.size()
    IRNode *lastNode = curReg->nodes[curReg->nodes.size()-1];
    for(auto it = lastNode->to.begin(); it != lastNode->to.end(); ++it){
      if((*it) > curReg->nodes.size()-1){
        lastNode->to.erase(it);
        break;
      }
    }
    
    // Go over all IRcurReg->nodes. Find in, out or each IRNode. Terminates until in and out don't change for all IRcurReg->nodes.
    
    printf("live analysis in progess \n");
    bool flag = true;
    while (flag) {
      flag = false;
      for (int i = curReg->nodes.size() - 1; i >= 0; i--) {
        IRNode *cur = curReg->nodes[i];
        int inSize = cur->in.size();
        int outSize = cur->out.size();
        for (auto str : cur->out) {
          if(std::find(cur->defs.begin(), cur->defs.end(), str) == cur->defs.end()) {
              cur->in.insert(str);
          }
        }
        cur->in.insert(cur->uses.begin(), cur->uses.end());
        for (auto idx : cur->to) {
          cur->out.insert(curReg->nodes[idx]->in.begin(), curReg->nodes[idx]->in.end());
        }
        if (inSize != cur->in.size() || outSize != cur->out.size()) {
          flag = true;
        }
      }
    }    
    LiveRangeBuild(curReg);
  }
  void LiveRangeBuild(RegInfo *curReg){
    for (int i = 0; i < curReg->nodes.size(); i++) {
      for(auto t : curReg->nodes[i]->defs){
        std::string s = genNewWeb();
        std::string type = getVarType(curReg, t);
        Webs web(s);
        Livescope lscope(i, i);
        LiveRangeAnalysis(web, lscope, t, curReg->nodes[i], curReg->nodes);
        if(type == "float") curReg->floatWebMap[t].push_back(web);
        if(type == "int") curReg->intWebMap[t].push_back(web);
      }
      for(auto t : curReg->nodes[i]->consts){
        std::string s = genNewWeb();
        std::string type;
        if(IsFloatNum(t)) type = "float";
        else if(IsIntNum(t)) type = "int";
        Webs web(s);     
        Livescope lscope(i, i);
        web.liveScopes.push_back(lscope);
        if(type == "float") curReg->floatWebMap[t].push_back(web);
        if(type == "int") curReg->intWebMap[t].push_back(web);
      }
    }

    // remove defs that reach the same use
    if(curReg->floatWebMap.size() > 0) RemoveRedundantDefs(curReg->floatWebMap);
    if(curReg->intWebMap.size() > 0) RemoveRedundantDefs(curReg->intWebMap);
    //
    printf("BuildGraph\n");
    if(curReg->intWebMap.size() != 0){
      curReg->intVarGraph = BuildGraph(curReg->intWebMap);
    }
    if(curReg->floatWebMap.size() != 0){
      curReg->floatVarGraph = BuildGraph(curReg->floatWebMap);
    }
    printf("end BuildGraph\n");
    std::cout << std::endl;
    for(auto it = curReg->intWebMap.begin(); it != curReg->intWebMap.end(); ++it){
      std::cout << "int var " << it->first << ", live scope : ";
      for(auto iter = it->second.begin(); iter != it->second.end(); ++iter){
        std::cout << "\t" << (*iter).s << " : ";
        for(auto iterator = (*iter).liveScopes.begin(); iterator != (*iter).liveScopes.end(); ++iterator){
          std::cout << "<" << (*iterator).liveBegin << "," << (*iterator).liveEnd << ">,";
        }     
      }
      std::cout << std::endl;
    }   
    /*for(auto s = curReg->intWebMap.begin(); s != curReg->intWebMap.end(); ++s){
      std::cout << "var " << s->first << " has web" << std::endl;
    }*/
  }
  bool CheckUsesSet(std::set<int> &seti, std::set<int> &setj){
    if(seti.size()==0 || setj.size()==0) return false;
    if(seti.size() != setj.size()) return false;
    for(auto elem : seti){
      if(setj.find(elem)==setj.end()) return false;
    }
    return true;
  }
  void RemoveRedundantDefs(std::map<std::string, std::vector<Webs> > &webMap){
     
    for(auto it = webMap.begin(); it != webMap.end(); ++it){             
      while(1){
        auto webVec = it->second;
        bool hasWebCombine = false;
        for(int i=0; i < webVec.size(); ++i){
          Webs wi = webVec[i];
          for(int j=i+1; j < webVec.size(); ++j){
            Webs wj = webVec[j];
            // two uses set is same, we can combine these two webs
            if(CheckUsesSet(wi.realUses, wj.realUses)){
              hasWebCombine = true;
              std::string s = genNewWeb();
              Webs web(s);
              for(auto it = wi.liveScopes.begin(); it != wi.liveScopes.end(); ++it){
                WebAddLiveScope(web, (*it));
              }
              for(auto it = wj.liveScopes.begin(); it != wj.liveScopes.end(); ++it){
                WebAddLiveScope(web, (*it));
              }
              it->second.erase(find(it->second.begin(), it->second.end(), wi));
              it->second.erase(find(it->second.begin(), it->second.end(), wj));
              it->second.push_back(web);
            }
          }
        }
        if(!hasWebCombine) break;
      }
    } 
  }



  bool checkVarEnd(std::string var, IRNode *cur){
    // var 不在 in 中了
    if(cur->in.find(var) == cur->in.end()) return true;
    else return false;
  }

  void WebAddLiveScope(Webs &w, Livescope lscope){
    for(auto it = w.liveScopes.begin(); it != w.liveScopes.end(); ++it){
      if((*it).liveBegin <= lscope.liveBegin && (*it).liveEnd >= lscope.liveEnd){
        // 不插入, 因为范围大的那个是把不同block间，但序号相连的LiveIn都算上了
        return;
      }
      else if((*it).liveBegin >= lscope.liveBegin && (*it).liveEnd <= lscope.liveEnd){
        // 把范围小的那个删了再插入，有all defs that reach same use must be in the same web的意思
        // 各个def的liveBegin不同，但它们reach same use，所以liveEnd是一样的
        w.liveScopes.erase(it);
        w.liveScopes.push_back(lscope);
        return;
      }
      else if((*it).liveBegin <= lscope.liveBegin && (*it).liveEnd >= lscope.liveBegin && 
        (*it).liveEnd <= lscope.liveEnd){
        it->liveEnd = lscope.liveBegin - 1;
      }
    }
    w.liveScopes.push_back(lscope);
  }

  void LiveRangeAnalysis(Webs &w, Livescope lscope, std::string var, IRNode *curNode, std::vector<IRNode *> &nodes){   
    if(curNode->uses.find(var) != curNode->uses.end()){
      w.realUses.insert(curNode->id);
    }
    // last line 
    if(curNode->id == nodes.size()-1 && curNode->to.size() == 0){
      w.liveScopes.push_back(lscope); 
    }
    // still have next line
    for(auto idx : curNode->to){
      if(checkVarEnd(var, nodes[idx])){
        lscope.liveEnd = curNode->id;
        WebAddLiveScope(w, lscope);
      }
      else if(idx < lscope.liveEnd){
        if(idx < lscope.liveBegin) lscope.liveBegin = idx;
        LiveRangeAnalysis(w, lscope, var, nodes[idx], nodes);
      }
      else if(idx == (lscope.liveEnd + 1)){
        lscope.liveEnd = idx;
        LiveRangeAnalysis(w, lscope, var, nodes[idx], nodes);
      }
      else if(idx > (lscope.liveEnd + 1)){
        lscope.liveEnd = curNode->id;
        WebAddLiveScope(w, lscope);
        Livescope newlscope(idx, idx);  
        LiveRangeAnalysis(w, newlscope, var, nodes[idx], nodes);
      }       
    }
  }

  bool checkWebInterefence(Webs wi, Webs wj){
    for(auto it = wi.liveScopes.begin(); it != wi.liveScopes.end(); ++it){
      Livescope wiLive = (*it);
      for(auto iter = wj.liveScopes.begin(); iter != wj.liveScopes.end(); ++iter){
        Livescope wjLive = (*iter);
        if((wjLive.liveBegin >= wiLive.liveBegin && wjLive.liveBegin <= wiLive.liveEnd) 
          || (wjLive.liveEnd >= wiLive.liveBegin && wjLive.liveEnd <= wiLive.liveEnd)
          || (wiLive.liveEnd >= wjLive.liveBegin && wiLive.liveEnd <= wjLive.liveEnd)
          || (wiLive.liveEnd >= wjLive.liveBegin && wiLive.liveEnd <= wjLive.liveEnd)){
          return true;
        }
      }
    }
    return false;
  }

  std::map<Webs, std::set<Webs> > BuildGraph(std::map<std::string, std::vector<Webs> > webMap) {
    // key/value is string in Webs
    std::map<Webs, std::set<Webs> > graph; 
    // std::map<std::string, std::set<std::string> > graph; 
    std::vector<Webs> allWebs;
    for(auto it = webMap.begin(); it != webMap.end(); ++it){
      allWebs.insert(std::end(allWebs), std::begin(it->second), std::end(it->second));
    }
    for(auto it = allWebs.begin(); it != allWebs.end(); ++it){
      std::cout << "web " << it->s << ":";
      for(auto livescope : it->liveScopes){
        std::cout << "<" << livescope.liveBegin << "," << livescope.liveEnd << ">";
      }
      std::cout << std::endl;
    }
    for(auto it = allWebs.begin(); it != allWebs.end(); ++it){
      if(graph.find((*it)) == graph.end()){
        std::set<Webs> empty;
        graph.insert(std::make_pair((*it), empty));
      }
    }
    for(int i = 0; i < allWebs.size(); i++){
      Webs wi = allWebs[i];      
      for(int j = i + 1; j < allWebs.size(); j++){
        Webs wj = allWebs[j];
        //std::cout << "check web " << wi.s << " and web " << wj.s << std::endl;
        if (checkWebInterefence(wi, wj)) {
          //std::cout << "web " << wi.s << " has interfernece with web " << wj.s << std::endl;
          graph[wi].insert(wj);
          graph[wj].insert(wi);
        }
        else{
          //std::cout << "web " << wi.s << " has no  web " << wj.s << std::endl;
        }
      }
    }

    for(auto it = graph.begin(); it != graph.end(); ++it){
      std::cout << "web " << it->first.s << ", neighbors : ";
      for(auto iter : it->second){
        std::cout << iter.s << ",";
      }
      std::cout << std::endl;
    }
    return graph;
  }


  bool isLabel(std::string stmt) {
    return stmt.back() == ':';
  }
  std::string getLabelName(std::string LabelStmt) {
    return LabelStmt.substr(0, LabelStmt.size() - 1);
  } 
  std::string getVarType(RegInfo *rinfo, std::string var){
    auto it1 = rinfo->defVar.find(var);
    if(it1 != rinfo->defVar.end()){
      return it1->second.second.typeVal;
    }
    auto it2 = rinfo->tempVar.find(var);
    if(it2 != rinfo->tempVar.end()){
      return it2->second.second.typeVal;
    }
    auto it3 = rinfo->outscopeVar.find(var);
    if(it3 != rinfo->outscopeVar.end()){
      return it3->second.second.typeVal;
    }
  }
  std::string genNewWeb(){
    std::string webName = "s" + std::to_string(sNum);
    sNum++;
    return webName;
  }
};



