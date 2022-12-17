local uci    = require "luci.model.uci"

module("luci.controller.COM.COM_tab", package.seeall)
-- UI control for serial config tab


function index()
entry({"admin","services", "new_tab"}, firstchild(), "Serial", 6
--top level tab should be changed to something more reasonable
entry({"admin","services","new_tab", "tab_from_cbi"}, cbi("COM/C

--entry({"admin", "services", "new_tab", "tab_from_cbi", "run"},
--add button to save settings and execute through controller
--manipulation through deamon service and rpc is a better way to

end

