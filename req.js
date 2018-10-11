var http = require("http");

var options = {
  hostname: "platform-davidlopessmallcon-gokodoek.srv.ravcloud.com",
  port: 8080,
  method: "GET",
  path: "/ok"
};

http
  .request(options, function(resp) {
    var data = "";

    // A chunk of data has been recieved.
    resp.on("data", function(chunk) {
      data += chunk;
    });

    // The whole response has been received. Print out the result.
    resp.on("end", function() {
      console.log(data);
    });
  })
  .on("error", function(err) {
    console.log("Error: " + err);
  });
