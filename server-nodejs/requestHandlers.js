var sql = require("./sql");
var url = require("url");
var result = require("./result");

function kinds_list(request, response) {
    var SqlQuery = "select * from kinds";
    sql.connectionSQL(SqlQuery, result.list, response);
}

function images_list(request, response) {
    var kind = url.parse(request.url,true).query.kind;
    if ( null == kind || '' == kind ) return {};
    var SqlQuery = "select * from images where kind=\'" + kind + "\'";
    sql.connectionSQL(SqlQuery, result.list, response);
}

exports.kinds_list = kinds_list;
exports.images_list = images_list;
