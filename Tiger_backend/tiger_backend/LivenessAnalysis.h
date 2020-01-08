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
#include "BackEndHelper.h"

class  LivenessAnalysis{

  static int sNum;
  static int floatVarNum;
  int totalIntReg = 10;
  int totalFloatReg = 27;
  std::string intRegsName[10] = {"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$t8","$t9"};  
  std::string floatRegsName[27] = {"$f5","$f6","$f7","$f8","$f9","$f10","$f11","$f12","$f13",
  "$f14","$f15","$f16","$f17","$f18","$f19","$f20","$f21","$f22","$f23",
 "$f24","$f25","$f26","$f27","$f28","$f29","$f30","$f31"};
  bool floatRegsUsed[27];
  bool intRegsUsed[10];


public:


  LivenessAnalysis(FuncInfo &finfo, std::map<std::string, std::string> &gFloatVal, bool opt){
    FuncLivenessAnalyze(finfo, gFloatVal);
  }
  LivenessAnalysis(FuncInfo &finfo, std::map<std::string, std::string> &gFloatVal){
    LivenessAnalyze(finfo, gFloatVal);
  }

  void LivenessAnalyze(FuncInfo &finfo, std::map<std::string, std::string> &gFloatVal){
    FuncLivenessAnalyze(finfo, gFloatVal);
    FuncLiveRangeBuild(finfo);
    if(finfo.intWebMap.size() != 0){
      finfo.intVarGraph = FuncBuildGraph(finfo.intWebMap);
    }
    if(finfo.floatWebMap.size() != 0){
      finfo.floatVarGraph = FuncBuildGraph(finfo.floatWebMap);
    }
    if(finfo.intWebMap.size() != 0) FuncGraphColor(finfo, finfo.intVarGraph, finfo.intWebMap, true);
    if(finfo.floatWebMap.size() != 0) FuncGraphColor(finfo, finfo.floatVarGraph, finfo.floatWebMap, false);
  }

  // GraphColoring
  void FuncGraphColor(FuncInfo &finfo, std::map<Webs, std::set<Webs> > &map, std::map<std::string, std::vector<Webs>> &varWebMap, bool isIntMap);
  
  // BuildGraph
  bool CheckWebInterefence(Webs wi, Webs wj);
  std::map<Webs, std::set<Webs> > FuncBuildGraph(std::map<std::string, std::vector<Webs> > webMap);

  
  // LiveRange 
  void FuncLiveRangeBuild(FuncInfo &finfo);
  void AnalysisLiveRange(Webs &w, Livescope lscope, std::string var, IRNode *curNode, std::vector<IRNode *> &nodes);
  bool CheckUsesSet(std::set<int> &seti, std::set<int> &setj);
  void RemoveRedundantDefs(std::map<std::string, std::vector<Webs> > &webMap);
  bool CheckVarEnd(std::string var, IRNode *cur);
  void WebAddLiveScope(Webs &w, Livescope lscope);
  std::string genNewWeb(){
    std::string webName = "s" + std::to_string(sNum);
    sNum++;
    return webName;
  }
  std::string getVarType(FuncInfo &finfo, std::string var){
    auto it1 = finfo.varMap.find(var);
    if(it1 != finfo.varMap.end()){
      return it1->second.second.typeVal;
    }
    auto it2 = finfo.outscopeVarMap.find(var);
    if(it2 != finfo.outscopeVarMap.end()){
      return it2->second.second.typeVal;
    }
  }
  
  // LivenessAnalysis
  void FuncLivenessAnalyze(FuncInfo &finfo, std::map<std::string, std::string> &gFloatVal);
  void AnalysisVar(std::string var, FuncInfo &finfo, IRNode *cur, std::map<std::string, int> &outscopeVarDefs, std::map<std::string, std::string> &gFloatVal);
  const bool isIntegerList(std::string stmt) {
    return stmt.compare("int-list:") == 0;
  }
  const bool isFloatList(std::string stmt) {
    return stmt.compare("float-list:") == 0;
  }
  const bool isLabel(std::string stmt) {
    return stmt.back() == ':' && !isIntegerList(stmt) && !isFloatList(stmt) && !isFunctionDeclStmt(stmt); 
  }
  std::string getLabelName(std::string LabelStmt) {
    return LabelStmt.substr(0, LabelStmt.size() - 1);
  } 

};