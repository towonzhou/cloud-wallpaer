var server = require("./server");
var router = require("./router");
var requestHandlers = require("./requestHandlers");

var handle = {}
handle["/"] = requestHandlers.kinds_list;
handle["/kinds_list"] = requestHandlers.kinds_list;
handle["/images_list"] = requestHandlers.images_list;

server.start(router.route, handle);
