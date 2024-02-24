const chart_vibration = new Chart("chart_vibration", {
    type: "line",
    data: {
      labels: [1,2,3,4,5,6,7,8,9,10],
      datasets: [{
        borderColor: "blue",
        data: [],
        fill: true
      }]
    },
    options: {
        animation: {duration: 75}
    }
  });

const chart_acc = new Chart("chart_acc", {
    type: "line",
    data: {
        labels: [1,2,3,4,5,6,7,8,9,10],
        datasets: [{
            borderColor: "red",
            data: [],
            fill: true
        },
        {
            borderColor: "blue",
            data: [],
            fill: true
        },
        {
            borderColor: "green",
            data: [],
            fill: true
        }
    ]},
    options: {
        animation: {duration: 75}
    }
});

const chart_gyro = new Chart("chart_gyro", {
    type: "line",
    data: {
        labels: [1,2,3,4,5,6,7,8,9,10],
        datasets: [{
            borderColor: "red",
            data: [],
            fill: true
        },
        {
            borderColor: "blue",
            data: [],
            fill: true
        },
        {
            borderColor: "green",
            data: [],
            fill: true
        }
    ]},
    options: {
        animation: {duration: 75}
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

function callBackLog(req) {
    log_text = document.getElementById("log_text")
    log_text.innerHTML = req.response.join("\n");
    log_text.scrollTop = log_text.scrollHeight;
}

function callBackAcc(req) {
    chart_acc["data"]["datasets"][1]["data"] = req.response["x"]
    chart_acc["data"]["datasets"][2]["data"] = req.response["y"]
    chart_acc["data"]["datasets"][0]["data"] = req.response["z"]
    chart_acc.update()
}

function callBackGyro(req) {
    chart_gyro["data"]["datasets"][0]["data"] = req.response["x"]
    chart_gyro["data"]["datasets"][1]["data"] = req.response["y"]
    chart_gyro["data"]["datasets"][2]["data"] = req.response["z"]
    chart_gyro.update()
}

function getData() {
    sendRequest("/data/log", callBackLog);

    /*
    let reqVibration = new XMLHttpRequest();
    sendRequest(reqVibration, "/data/vibration");
    */
    sendRequest("data/gyro", callBackGyro);
    sendRequest("data/acc", callBackAcc);
    
    //chart_vibration["data"]["datasets"][0]["data"] = JSON.parse(reqVibration.response);

    //chart_vibration.update()
}
setInterval(getData, 500);
