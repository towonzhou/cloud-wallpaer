var mysql = require('mysql');

function connectionSQL(SqlQuery, list, response) {
    var connection = mysql.createConnection({
        host : 'localhost',
        user : 'root',
        password : 'root',
    });

    connection.connect();

    connection.query('use cloud_wallpaper', function(err, rows, fields) {
        if (err) throw err;
    });

    connection.query(SqlQuery, function(err, rows, fields) {
        list(rows,response);
    });

    connection.end();
}

exports.connectionSQL = connectionSQL;
