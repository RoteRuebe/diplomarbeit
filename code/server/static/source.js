Chart.defaults.global.elements.point.pointStyle = "line"
const chart_vibration = new Chart("chart_vibration", {
    type: "line",
    data: {
      labels: Array.from(Array(100).keys()),
      datasets: [{
        borderColor: "blue",
        data: [],
        fill: false
      }]
    },
    options: {
        animation: {duration: 50}
    }
  });

const chart_acc = new Chart("chart_acc", {
    type: "line",
    data: {
        labels: Array.from(Array(100).keys()),
        datasets: [{
            borderColor: "red",
            data: [],
            fill: false
        },
        {
            borderColor: "green",
            data: [],
            fill: false
        },
        {
            borderColor: "blue",
            data: [],
            fill: false
        }
    ]},
    options: {
        animation: {duration: 50}
    }
});

const chart_gyro = new Chart("chart_gyro", {
    type: "line",
    data: {
        labels: Array.from(Array(100).keys()),
        datasets: [{
            borderColor: "red",
            data: [],
            fill: false
        },
        {
            borderColor: "green",
            data: [],
            fill: false
        },
        {
            borderColor: "blue",
            data: [],
            fill: false
        }
    ]},
    options: {
        animation: {duration: 50}
    }
});

function sendRequest(url, callback) {
    let req = new XMLHttpRequest();
    req.responseType = 'json';
    req.open("GET", url);
    req.send();

    // the response is {"message": "Hello, world!"}
    req.onreadystatechange = function() {
        if(req.readyState == 4) {
            callback(req)
        }
    };
}



function getData() {
    sendRequest("/data/log", function(req) {
        log_text = document.getElementById("log_text")
        log_text.innerHTML = req.response.join("\n");
        log_text.scrollTop = log_text.scrollHeight;
    });

    sendRequest("data/gyro", function(req) {
        chart_gyro["data"]["datasets"][0]["data"] = req.response["x"]
        chart_gyro["data"]["datasets"][1]["data"] = req.response["y"]
        chart_gyro["data"]["datasets"][2]["data"] = req.response["z"]
        chart_gyro.update()
    });

    sendRequest("data/acc", function(req) {
        chart_acc["data"]["datasets"][0]["data"] = req.response["x"]
        chart_acc["data"]["datasets"][1]["data"] = req.response["y"]
        chart_acc["data"]["datasets"][2]["data"] = req.response["z"]
        chart_acc.update()
    });

    sendRequest("data/status", function(req) {
        if (req.response["robotConnected"] == "True") {
            document.getElementById("robotConnected").innerHTML = "<div class='box' style='background-color: green'></div> &ensp; Robot Connected"
        }
        else {
            document.getElementById("robotConnected").innerHTML = "<div class='box'></div> &ensp; Robot Disconnected"
        }

        if (req.response["controllerConnected"] == "True") {
            document.getElementById("controllerConnected").innerHTML = "<div class='box' style='background-color: green'></div> &ensp; Controller Connected"
        }
        else {
            document.getElementById("controllerConnected").innerHTML = "<div class='box'></div> &ensp; Controller Disconnected"
        }

    });

    /*
    let reqVibration = new XMLHttpRequest();
    sendRequest(reqVibration, "/data/vibration");
    */
    
    
    
    //chart_vibration["data"]["datasets"][0]["data"] = JSON.parse(reqVibration.response);

    //chart_vibration.update()
}
setInterval(getData, 100);
