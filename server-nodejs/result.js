function list(rows, response) {
    response.writeHead(200, {"Content-Type": "text/plain"});
    response.write(JSON.stringify({rows:rows}));
    response.end();
}

exports.list = list;
