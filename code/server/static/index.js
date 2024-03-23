Chart.defaults.global.elements.point.pointStyle = "line"

const chartAcc0 = new Chart("chartAcc0", {
    type: "line",
    data: {
        labels: Array.from(Array(100).keys()),
        datasets: [{
            label: "Acc. X",
            borderColor: "red",
            data: [],
            fill: false
        },
        {
            label: "Acc. Y",
            borderColor: "green",
            data: [],
            fill: false
        },
        {
            label: "Acc. Z",
            borderColor: "blue",
            data: [],
            fill: false
        }
    ]},
    options: {
        animation: {duration: 50}
    }
});

const chartAcc1 = new Chart("chartAcc1", {
    type: "line",
    data: {
        labels: Array.from(Array(100).keys()),
        datasets: [{
            label: "Acc. X",
            borderColor: "red",
            data: [],
            fill: false
        },
        {
            label: "Acc. Y",
            borderColor: "green",
            data: [],
            fill: false
        },
        {
            label: "Acc. Z",
            borderColor: "blue",
            data: [],
            fill: false
        }
    ]},
    options: {
        animation: {duration: 50}
    }
});

const chartAcc2 = new Chart("chartAcc2", {
    type: "line",
    data: {
        labels: Array.from(Array(100).keys()),
        datasets: [{
            label: "Acc. X",
            borderColor: "red",
            data: [],
            fill: false
        },
        {
            label: "Acc. Y",
            borderColor: "green",
            data: [],
            fill: false
        },
        {
            label: "Acc. Z",
            borderColor: "blue",
            data: [],
            fill: false
        }
    ]},
    options: {
        animation: {duration: 50}
    }
});

const chartGyro0 = new Chart("chartGyro0", {
    type: "line",
    data: {
        labels: Array.from(Array(100).keys()),
        datasets: [{
            label: "Gyro X",
            borderColor: "red",
            data: [],
            fill: false
        },
        {
            label: "Gyro Y",
            borderColor: "green",
            data: [],
            fill: false
        },
        {
            label: "Gyro Z",
            borderColor: "blue",
            data: [],
            fill: false
        }
    ]},
    options: {
        animation: {duration: 50}
    }
});

const chartGyro1 = new Chart("chartGyro1", {
    type: "line",
    data: {
        labels: Array.from(Array(100).keys()),
        datasets: [{
            label: "Gyro X",
            borderColor: "red",
            data: [],
            fill: false
        },
        {
            label: "Gyro Y",
            borderColor: "green",
            data: [],
            fill: false
        },
        {
            label: "Gyro Z",
            borderColor: "blue",
            data: [],
            fill: false
        }
    ]},
    options: {
        animation: {duration: 50}
    }
});

const chartGyro2 = new Chart("chartGyro2", {
    type: "line",
    data: {
        labels: Array.from(Array(100).keys()),
        datasets: [{
            label: "Gyro X",
            borderColor: "red",
            data: [],
            fill: false
        },
        {
            label: "Gyro Y",
            borderColor: "green",
            data: [],
            fill: false
        },
        {
            label: "Gyro Z",
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

    req.onreadystatechange = function() {
        if(req.readyState == 4) {
            callback(req)
        }
    };
}

function getData() {
    sendRequest("data/gyro/0", function(req) {
        chartGyro0["data"]["datasets"][0]["data"] = req.response["x"]
        chartGyro0["data"]["datasets"][1]["data"] = req.response["y"]
        chartGyro0["data"]["datasets"][2]["data"] = req.response["z"]
        chartGyro0.update()
    });

    sendRequest("data/acc/0", function(req) {
        chartAcc0["data"]["datasets"][0]["data"] = req.response["x"]
        chartAcc0["data"]["datasets"][1]["data"] = req.response["y"]
        chartAcc0["data"]["datasets"][2]["data"] = req.response["z"]
        chartAcc0.update()
    });
    
    sendRequest("data/status/0", function(req) {
        if (req.response["robotConnected"] == "True") {
            document.getElementById("robotConnected0").innerHTML = "<div class='box' style='background-color: green'></div> &ensp; Robot Connected"
        } else {
            document.getElementById("robotConnected0").innerHTML = "<div class='box'></div> &ensp; Robot Disconnected"
        }
        
        if (req.response["controllerConnected"] == "True") {
            document.getElementById("controllerConnected0").innerHTML = "<div class='box' style='background-color: green'></div> &ensp; Controller Connected"
        } else {
            document.getElementById("controllerConnected0").innerHTML = "<div class='box'></div> &ensp; Controller Disconnected"
        }
    });
    
    sendRequest("data/gyro/1", function(req) {
        chartGyro1["data"]["datasets"][1]["data"] = req.response["y"]
        chartGyro1["data"]["datasets"][0]["data"] = req.response["x"]
        chartGyro1["data"]["datasets"][2]["data"] = req.response["z"]
        chartGyro1.update()
    });

    sendRequest("data/acc/1", function(req) {
        chartAcc1["data"]["datasets"][0]["data"] = req.response["x"]
        chartAcc1["data"]["datasets"][1]["data"] = req.response["y"]
        chartAcc1["data"]["datasets"][2]["data"] = req.response["z"]
        chartAcc1.update()
    });    

    sendRequest("data/status/1", function(req) {
        if (req.response["robotConnected"] == "True") {
            document.getElementById("robotConnected1").innerHTML = "<div class='box' style='background-color: green'></div> &ensp; Robot Connected"
        } else {
            document.getElementById("robotConnected1").innerHTML = "<div class='box'></div> &ensp; Robot Disconnected"
        }
        
        if (req.response["controllerConnected"] == "True") {
            document.getElementById("controllerConnected1").innerHTML = "<div class='box' style='background-color: green'></div> &ensp; Controller Connected"
        } else {
            document.getElementById("controllerConnected1").innerHTML = "<div class='box'></div> &ensp; Controller Disconnected"
        }
    });
    
    sendRequest("data/gyro/2", function(req) {
        chartGyro2["data"]["datasets"][1]["data"] = req.response["y"]
        chartGyro2["data"]["datasets"][0]["data"] = req.response["x"]
        chartGyro2["data"]["datasets"][2]["data"] = req.response["z"]
        chartGyro2.update()
    });

    sendRequest("data/acc/2", function(req) {
        chartAcc2["data"]["datasets"][0]["data"] = req.response["x"]
        chartAcc2["data"]["datasets"][1]["data"] = req.response["y"]
        chartAcc2["data"]["datasets"][2]["data"] = req.response["z"]
        chartAcc2.update()
    });    

    sendRequest("data/status/2", function(req) {
        if (req.response["robotConnected"] == "True") {
            document.getElementById("robotConnected2").innerHTML = "<div class='box' style='background-color: green'></div> &ensp; Robot Connected"
        } else {
            document.getElementById("robotConnected2").innerHTML = "<div class='box'></div> &ensp; Robot Disconnected"
        }
        
        if (req.response["controllerConnected"] == "True") {
            document.getElementById("controllerConnected2").innerHTML = "<div class='box' style='background-color: green'></div> &ensp; Controller Connected"
        } else {
            document.getElementById("controllerConnected2").innerHTML = "<div class='box'></div> &ensp; Controller Disconnected"
        }
    });
    
    //chart_vibration["data"]statusList
statusList
statusList["datasets"][0]["data"] = JSON.parse(reqVibration.response);

    //chart_vibration.update()
}
setInterval(getData, 100);
