#include "LivenessAnalysis.h"

// LivenessAnalysis
// LivenessAnalysis
// LivenessAnalysis
void LivenessAnalysis::AnalysisVar(std::string var, FuncInfo &finfo, IRNode *cur, std::map<std::string, int> &outscopeVarDefs, std::map<std::string, std::string> &gFloatVal){
	if(IsFloatNum(var)){
		std::string str = "float_var_" + std::to_string(floatVarNum);
		gFloatVal.insert(std::make_pair(var, str));
		floatVarNum++;
	}

	if (IsFloatNum(var) || IsIntNum(var)) {
	  cur->consts.insert(var);
	  return;
	}
	// if this var is outscopeVar, we should def it frist for live range build
	if(finfo.outscopeVarMap.find(var) != finfo.outscopeVarMap.end()){
	  auto it = outscopeVarDefs.find(var);
	  // means not the first place this outscope var used
	  if(it->second > cur->id){
	    it->second = cur->id;
	    cur->defs.insert(var);
	  }
	  else cur->uses.insert(var);
	}
	else cur->uses.insert(var);
}
void LivenessAnalysis::FuncLivenessAnalyze(FuncInfo &finfo, std::map<std::string, std::string> &gFloatVal){
	std::map<std::string, int> Labels;
	// create a new IRNode for each line;
	for (int i = 0; i < finfo.stmts.size(); i++) {
	  std::string stmt = finfo.stmts[i];
	  IRNode *cur = new IRNode(i, stmt);
	  finfo.nodes.push_back(cur);
	  // save location for each label
	  if (isLabel(stmt)) {
	    Labels[getLabelName(stmt)] = i;
	  }
	}
	std::map<std::string, int> outscopeVarDefs;
	for(auto it = finfo.outscopeVarMap.begin(); it != finfo.outscopeVarMap.end(); ++it){
	  outscopeVarDefs.insert(std::make_pair(it->first, 999999));
	}
	for (unsigned i = 0; i < finfo.nodes.size(); i++) {
	  IRNode* cur = finfo.nodes[i];
	  std::string str = cur->content;
	  str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	  std::vector<std::string> tokens = split(str, ',');
	  if (isLabel(tokens[0])) {
	    cur->to.push_back(i + 1);
	  } 
	  else if (tokens[0] == "assign") {
	    cur->to.push_back(i + 1);
	    cur->defs.insert(tokens[1]);
	  	if (tokens.size() == 4) {
	  		AnalysisVar(tokens[3], finfo, cur, outscopeVarDefs, gFloatVal);
	  	} else {
	  		AnalysisVar(tokens[2], finfo, cur, outscopeVarDefs, gFloatVal);
	  	}
	    if(tokens.size() == 4){
	      cur->consts.insert(tokens[3]);
	    }
	  } 
	  else if (tokens[0]=="array_store" || tokens[0]=="array_load") {
	    cur->to.push_back(i + 1);
	    cur->defs.insert(tokens[1]);
	    AnalysisVar(tokens[2], finfo, cur, outscopeVarDefs, gFloatVal);
	    AnalysisVar(tokens[3], finfo, cur, outscopeVarDefs, gFloatVal);
	  }
	  else if (tokens[0]=="add" || tokens[0]=="sub" || tokens[0]=="mult" || tokens[0]=="div" || tokens[0]=="and" || tokens[0]=="or") {
	    cur->to.push_back(i + 1);
	    cur->defs.insert(tokens[3]);
	    AnalysisVar(tokens[1], finfo, cur, outscopeVarDefs, gFloatVal);
	    AnalysisVar(tokens[2], finfo, cur, outscopeVarDefs, gFloatVal);
	  } 
	  else if (tokens[0]=="breq" || tokens[0]=="brneq" || tokens[0]=="brlt" || tokens[0]=="brgt" || tokens[0]=="brgeq" || tokens[0]=="brleq") {
	    cur->to.push_back(i + 1);
	    cur->to.push_back(Labels[tokens[3]]);
	    AnalysisVar(tokens[1], finfo, cur, outscopeVarDefs, gFloatVal);
	    AnalysisVar(tokens[2], finfo, cur, outscopeVarDefs, gFloatVal);
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
	      AnalysisVar(tokens[i], finfo, cur, outscopeVarDefs, gFloatVal);
	    }
	    if(funcName == "printi" || funcName == "prints" || funcName == "flush" || funcName == "getchar"){
	      continue;
	    }        
	  }
	  else if(tokens[0] == "return"){
	    AnalysisVar(tokens[1], finfo, cur, outscopeVarDefs, gFloatVal);
	  }
	  // function decl
	  else if(isFunctionDeclStmt(str)){
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
	  
	}
	// Go over all finfo.nodes. Find in, out or each IRNode. Terminates until in and out don't change for all IRfinfo.nodes.
	bool flag = true;
	while (flag) {
	  flag = false;
	  for (int i = finfo.nodes.size() - 1; i >= 0; i--) {
	    IRNode *cur = finfo.nodes[i];
	    int inSize = cur->in.size();
	    int outSize = cur->out.size();
	    for (auto str : cur->out) {
	      if(std::find(cur->defs.begin(), cur->defs.end(), str) == cur->defs.end()) {
	          cur->in.insert(str);
	      }
	    }
	    cur->in.insert(cur->uses.begin(), cur->uses.end());
	    for (auto idx : cur->to) {
	      cur->out.insert(finfo.nodes[idx]->in.begin(), finfo.nodes[idx]->in.end());
	    }
	    if (inSize != cur->in.size() || outSize != cur->out.size()) {
	      flag = true;
	    }
	  }
	}
}

// LiveRange
// LiveRange
// LiveRange

void LivenessAnalysis::FuncLiveRangeBuild(FuncInfo &finfo){
    for (int i = 0; i < finfo.nodes.size(); i++) {
      for(auto var : finfo.nodes[i]->defs){
        std::string s = genNewWeb();
        std::string type = getVarType(finfo, var);
        Webs web(s);
        Livescope lscope(i, i);
        AnalysisLiveRange(web, lscope, var, finfo.nodes[i], finfo.nodes);
        if(type == "float") finfo.floatWebMap[var].push_back(web);
        if(type == "int") finfo.intWebMap[var].push_back(web);
      }
      for(auto t : finfo.nodes[i]->consts){
        std::string s = genNewWeb();
        Webs web(s);     
        Livescope lscope(i, i);
        web.liveScopes.push_back(lscope);
        web.realUses.insert(i);
        if(IsFloatNum(t)) finfo.floatWebMap[t].push_back(web);
        else if(IsIntNum(t)) finfo.intWebMap[t].push_back(web);
      }
    }
    /*for(auto it = finfo.intWebMap.begin(); it != finfo.intWebMap.end(); ++it){
		std::vector<Webs> webVec = it->second;
		std::cout << "var is " << it->first << ":";
		for(auto w : webVec){
			std::cout << "< web " << w.s << ", reg " << w.regName << ">";
			for(auto iterator = w.liveScopes.begin(); iterator != w.liveScopes.end(); ++iterator){
	          std::cout << "<" << (*iterator).liveBegin << "," << (*iterator).liveEnd << ">,";
	        } 
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}*/
    // remove defs that reach the same use
    if(finfo.floatWebMap.size() > 0) RemoveRedundantDefs(finfo.floatWebMap);
    if(finfo.intWebMap.size() > 0) RemoveRedundantDefs(finfo.intWebMap);
    /*std::cout << "\n\n!Error!!after remove redundant defs \n\n" << std::endl;
    for(auto it = finfo.intWebMap.begin(); it != finfo.intWebMap.end(); ++it){
		std::vector<Webs> webVec = it->second;
		std::cout << "var is " << it->first << ":";
		for(auto w : webVec){
			std::cout << "< web " << w.s << ", reg " << w.regName << ">";
			for(auto iterator = w.liveScopes.begin(); iterator != w.liveScopes.end(); ++iterator){
	          std::cout << "<" << (*iterator).liveBegin << "," << (*iterator).liveEnd << ">,";
	        } 
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}*/
}
void LivenessAnalysis::AnalysisLiveRange(Webs &w, Livescope lscope, std::string var, IRNode *curNode, std::vector<IRNode *> &nodes){   
    // realUses is used to remove redundant webs
    if(curNode->uses.find(var) != curNode->uses.end()){
      w.realUses.insert(curNode->id);
    }
    // last line 
    if(curNode->content.find("return")==0 && curNode->to.size() == 0){
      lscope.liveEnd = curNode->id;
      w.liveScopes.push_back(lscope); 
    }
    // still have next line
    for(auto idx : curNode->to){
      if(CheckVarEnd(var, nodes[idx])){
        lscope.liveEnd = curNode->id;
        WebAddLiveScope(w, lscope);
      }
      else if(idx < lscope.liveEnd){
        if(idx < lscope.liveBegin) lscope.liveBegin = idx;
        AnalysisLiveRange(w, lscope, var, nodes[idx], nodes);
      }
      else if(idx == lscope.liveEnd){
      	WebAddLiveScope(w, lscope);
      }
      else if(idx == (lscope.liveEnd + 1)){
        lscope.liveEnd = idx;
        AnalysisLiveRange(w, lscope, var, nodes[idx], nodes);
      }
      else if(idx > (lscope.liveEnd + 1)){
        lscope.liveEnd = curNode->id;
        WebAddLiveScope(w, lscope);
        Livescope newlscope(idx, idx);  
        AnalysisLiveRange(w, newlscope, var, nodes[idx], nodes);
      }       
    }
}
bool LivenessAnalysis::CheckVarEnd(std::string var, IRNode *cur){
    // var 在 def，不在use，结束且end值不前进
    if(cur->defs.find(var) != cur->defs.end() && cur->in.find(var) == cur->in.end()){
    	return true;
    } 
    // var 在 def，在use，如果不结束呢，应该是会只有一个livescope才对
    // 这样forward就没用了
    else if(cur->defs.find(var) != cur->defs.end() && cur->in.find(var) != cur->in.end()){
    	return false;
    } 
    // var 不在 def，在use，没结束但end值不前进
    else if(cur->defs.find(var) == cur->defs.end() && cur->in.find(var) != cur->in.end()){
    	return false;
    }  
    // var 不在 def，不在use，结束
    else if(cur->defs.find(var) == cur->defs.end() && cur->in.find(var) == cur->in.end()){
    	return true;
    }  
}

void LivenessAnalysis::WebAddLiveScope(Webs &w, Livescope lscope){
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
bool LivenessAnalysis::CheckUsesSet(std::set<int> &seti, std::set<int> &setj){
	//
	if(seti.size()==0 || setj.size()==0) return true; 
	if(seti.size() == setj.size()){
		for(auto elem : seti){
		  if(setj.find(elem)==setj.end()) return false;
		}
		return true;
	}
	if(seti.size() != setj.size()) {
		if(seti.size() > setj.size()){
			// 即setj的所有元素都在seti中，则可以共用一个web
			for(auto elem : setj){
			  if(seti.find(elem)==seti.end()) return false;
			}
			return true;
		}
		if(setj.size() > seti.size()){
			for(auto elem : seti){
			  if(setj.find(elem)==setj.end()) return false;
			}
			return true;
		}
		return false;
	}
}
void LivenessAnalysis::RemoveRedundantDefs(std::map<std::string, std::vector<Webs> > &webMap){
	for(auto it = webMap.begin(); it != webMap.end(); ++it){             
	  while(1){
	  	RECUR:
	    bool hasWebCombine = false;
	    for(int i=0; i < it->second.size(); ++i){
	      Webs wi = it->second[i];
	      for(int j=i+1; j < it->second.size(); ++j){
	        Webs wj = it->second[j];
	        // two uses set is same or 被包含, we can combine these two webs
	        if(CheckUsesSet(wi.realUses, wj.realUses)){
	          hasWebCombine = true;
	          std::string s = genNewWeb();
	          Webs web(s);
	          web.realUses.insert(wi.realUses.begin(), wi.realUses.end());
	          web.realUses.insert(wj.realUses.begin(), wj.realUses.end());
	          /*if(it->first == "i2"){
	          	std::cout << "< web " << wi.s << ", reg " << wi.regName << ">";
				for(auto iterator = wi.liveScopes.begin(); iterator != wi.liveScopes.end(); ++iterator){
		          std::cout << "<" << (*iterator).liveBegin << "," << (*iterator).liveEnd << ">,";
		        } 
		        std::cout << std::endl;
		        std::cout << "(";
		        for(auto l : wi.realUses){
		        	std::cout << l << ",";
		        }
		        std::cout << ")" << std::endl;
		        std::cout << "< web " << wj.s << ", reg " << wj.regName << ">";
				for(auto iterator = wj.liveScopes.begin(); iterator != wj.liveScopes.end(); ++iterator){
		          std::cout << "<" << (*iterator).liveBegin << "," << (*iterator).liveEnd << ">,";
		        } 
		        std::cout << std::endl;
		        std::cout << "(";
		        for(auto l : wj.realUses){
		        	std::cout << l << ",";
		        }
		        std::cout << ")" << std::endl;
	          }*/
	          for(auto it = wi.liveScopes.begin(); it != wi.liveScopes.end(); ++it){	          	
	            WebAddLiveScope(web, (*it));
	          }
	          for(auto it = wj.liveScopes.begin(); it != wj.liveScopes.end(); ++it){	            
	            WebAddLiveScope(web, (*it));
	          }
	          it->second.erase(find(it->second.begin(), it->second.end(), wi));
	          it->second.erase(find(it->second.begin(), it->second.end(), wj));
	          it->second.push_back(web);
	          /*if(it->first == "i2"){
	          	std::cout << "after combine !!!\n" << std::endl;
	          	std::cout << "< web " << web.s << ", reg " << web.regName << ">";
				for(auto iterator = wi.liveScopes.begin(); iterator != wi.liveScopes.end(); ++iterator){
		          std::cout << "<" << (*iterator).liveBegin << "," << (*iterator).liveEnd << ">,";
		        } 
		        std::cout << std::endl;
		        std::cout << "(";
		        for(auto l : web.realUses){
		        	std::cout << l << ",";
		        }
		        std::cout << ")" << std::endl;
		      }*/
		      goto RECUR;
	        }
	      }
	    }
	    if(!hasWebCombine) break;
	  }
	} 
}


// BuildGraph
// BuildGraph
// BuildGraph
bool LivenessAnalysis::CheckWebInterefence(Webs wi, Webs wj){
    for(auto it = wi.liveScopes.begin(); it != wi.liveScopes.end(); ++it){
      Livescope wiLive = (*it);
      for(auto iter = wj.liveScopes.begin(); iter != wj.liveScopes.end(); ++iter){
        Livescope wjLive = (*iter);
        if((wjLive.liveBegin >= wiLive.liveBegin && wjLive.liveBegin <= wiLive.liveEnd) 
          || (wjLive.liveEnd >= wiLive.liveBegin && wjLive.liveEnd <= wiLive.liveEnd)
          || (wiLive.liveEnd >= wjLive.liveBegin && wiLive.liveEnd <= wjLive.liveEnd)
          || (wiLive.liveBegin >= wjLive.liveBegin && wiLive.liveBegin <= wjLive.liveEnd)){
          return true;
        }
      }
    }
    return false;
}

std::map<Webs, std::set<Webs> > LivenessAnalysis::FuncBuildGraph(std::map<std::string, std::vector<Webs> > webMap) {
    std::map<Webs, std::set<Webs> > graph;  
    std::vector<Webs> allWebs;
    for(auto it = webMap.begin(); it != webMap.end(); ++it){
      allWebs.insert(std::end(allWebs), std::begin(it->second), std::end(it->second));
    }
    /*for(auto it = allWebs.begin(); it != allWebs.end(); ++it){
      std::cout << "web " << it->s << ":";
      for(auto livescope : it->liveScopes){
        std::cout << "<" << livescope.liveBegin << "," << livescope.liveEnd << ">";
      }
      std::cout << std::endl;
    }*/
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
        if (CheckWebInterefence(wi, wj)) { 
          graph[wi].insert(wj);
          graph[wj].insert(wi);
        }
        else{
        }
      }
    }
        /*for(auto it = graph.begin(); it != graph.end(); ++it){
      std::cout << "web " << it->first.s << ", neighbors : ";
      for(auto iter : it->second){
        std::cout << iter.s << ",";
      }
      std::cout << std::endl;
    }*/
    return graph;
}



// GraphColoring
// GraphColoring
// GraphColoring
void LivenessAnalysis::FuncGraphColor(FuncInfo &finfo, std::map<Webs, std::set<Webs> > &map, std::map<std::string, std::vector<Webs>> &varWebMap, bool isIntMap) {

    std::vector<std::pair<int, bool> > regVec;
    if(isIntMap){
      for(int i=0;i<totalIntReg;++i){
        regVec.push_back(std::make_pair(i, false));
      }
    }
    if(!isIntMap){
      for(int i=0;i<totalFloatReg;++i){
        regVec.push_back(std::make_pair(i, false));
      }
    }
    int regNum = regVec.size();

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
        Webs toBeSpilled = copyMap.begin()->first;
        std::string var;
        for(auto it = varWebMap.begin(); it != varWebMap.end(); ++it){
        	auto webs = it->second;
        	for(Webs w : webs){
        		if(w == toBeSpilled){
        			var = it->first;
        			goto SW;
        		}
        	}
        }
        SW:       
        // Simplify the graph G by choosing an object to spill and remove its node N from G
        copyMap.erase(toBeSpilled);
        for(auto it = copyMap.begin(); it != copyMap.end(); ++it){
          it->second.erase(toBeSpilled);
        }
        // also 
      }
    }
    // connect web with register
    for(auto it = varWebMap.begin(); it != varWebMap.end(); ++it){
      for(auto iter = it->second.begin(); iter != it->second.end(); ++iter){
        auto itera = (colorMap.find(*iter));
        if(itera->second != -1){
        	int regIndex = itera->second;
        	//std::cout << "regIndex is " << regIndex << ", reg name is " << intRegsName[regIndex] << std::endl;
        	if(isIntMap) iter->regName = intRegsName[regIndex];
        	else iter->regName = floatRegsName[regIndex];
        }
        else{
        	std::cout << "spilled web " << iter->s << std::endl;
        	iter->regName = "spill";
        }
      }
    }
    /*for(auto it = varWebMap.begin(); it != varWebMap.end(); ++it){
		std::vector<Webs> webVec = it->second;
		std::cout << "var is " << it->first << ":";
		for(auto w : webVec){
			std::cout << "< web " << w.s << ", reg " << w.regName << ">";
			for(auto livescope : w.liveScopes){
				std::cout << "(" << livescope.liveBegin << "," << livescope.liveEnd << ")";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}*/
}