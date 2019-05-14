function RTW_rtwnameSIDMap() {
	this.rtwnameHashMap = new Array();
	this.sidHashMap = new Array();
	this.rtwnameHashMap["<Root>"] = {sid: "pv"};
	this.sidHashMap["pv"] = {rtwname: "<Root>"};
	this.rtwnameHashMap["<S1>"] = {sid: "pv:79"};
	this.sidHashMap["pv:79"] = {rtwname: "<S1>"};
	this.rtwnameHashMap["<Root>/Ipv"] = {sid: "pv:196"};
	this.sidHashMap["pv:196"] = {rtwname: "<Root>/Ipv"};
	this.rtwnameHashMap["<Root>/Iinv"] = {sid: "pv:206"};
	this.sidHashMap["pv:206"] = {rtwname: "<Root>/Iinv"};
	this.rtwnameHashMap["<Root>/1-D Lookup Table2"] = {sid: "pv:1"};
	this.sidHashMap["pv:1"] = {rtwname: "<Root>/1-D Lookup Table2"};
	this.rtwnameHashMap["<Root>/Gain4"] = {sid: "pv:69"};
	this.sidHashMap["pv:69"] = {rtwname: "<Root>/Gain4"};
	this.rtwnameHashMap["<Root>/Integrator"] = {sid: "pv:78"};
	this.sidHashMap["pv:78"] = {rtwname: "<Root>/Integrator"};
	this.rtwnameHashMap["<Root>/MATLAB Function SOC "] = {sid: "pv:79"};
	this.sidHashMap["pv:79"] = {rtwname: "<Root>/MATLAB Function SOC "};
	this.rtwnameHashMap["<Root>/Sum7"] = {sid: "pv:94"};
	this.sidHashMap["pv:94"] = {rtwname: "<Root>/Sum7"};
	this.rtwnameHashMap["<Root>/soc"] = {sid: "pv:195"};
	this.sidHashMap["pv:195"] = {rtwname: "<Root>/soc"};
	this.rtwnameHashMap["<Root>/Vdc"] = {sid: "pv:204"};
	this.sidHashMap["pv:204"] = {rtwname: "<Root>/Vdc"};
	this.rtwnameHashMap["<S1>:1"] = {sid: "pv:79:1"};
	this.sidHashMap["pv:79:1"] = {rtwname: "<S1>:1"};
	this.getSID = function(rtwname) { return this.rtwnameHashMap[rtwname];}
	this.getRtwname = function(sid) { return this.sidHashMap[sid];}
}
RTW_rtwnameSIDMap.instance = new RTW_rtwnameSIDMap();