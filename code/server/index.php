<!DOCTYPE html>

<html>
  <head>
    <title> SumoBots </title>
    <script src = "https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js"> </script>
    <link rel="icon" type="image/png" href="/favicon.png"> </link>
    <style>
      html, body {
        font-family: helvetica;
      }

      p {
        position:relative;
      }

      textarea {
        position: relative;
        width: 100%;
        height: 50%;
      }

      li {
        list-style-type: none;
      }
      .column {
        position: absolute;
        width: 30%;
        height: 100%;
        top: 0px;
        padding: 3%
      }

      .box {
        position: relative;
        display: inline-block;
        border-radius: 30%;

        width: 13px;
        height: 13px;
        top: 0px;
        left: 0px;
        background-color: red;
      }

      #log {
        width: 33%;
        height: 800px;
        white-space: pre-line;
      }

      #chart {
        width: 100%;
        height: 50%;
      }

    </style>
  </head>


  <body>
    <div id="column1" class="column">
      <p style="text-align: center">
        Log Nachrichten des Roboters.
      </p>
      <textarea id="log_text" readonly=true> </textarea>

      <form action="/index.php" method="post">
        <input type="text" id="message" placeholder="Command to robot"> </input>
        <input type="submit" id="submit" value="Send"> </input>
      </form>
    </div>

    <div id="column2" style="left: 33%" class="column">
      <canvas id="chart_vibration"> </canvas>
      <canvad id="chart_gyro"> </canvas>
    </div>

    <div id="column3" style="left: 66%" class="column">
      <ul>
        <li> <div class="box" style="background-color: green"></div> &ensp; Robot connected </li>
        <li> <div class="box"></div> &ensp; Controller disconnected </li>
        <li> <div class="box"></div> &ensp; Camera disconnected </li>
      </ul>
    </div>

    <script>
      let xValues = Array.from( Array(100).keys )
      const chart = new Chart("chart_vibration", {
        type: "line",
        data: {
          labels: xValues,
          datasets: [{
            borderColor: "blue",
            data: d_vibration,
            fill: false
          }]
        },
        options: {}
      });

      const chart = new Chart("chart_gyro", {
        type: "line",
        data: {
          labels: xValues,
          datasets: [
          {
            borderColor: "red",
            data: [],
            fill: false
          },
          {
            borderColor: "blue",
            data: [],
            fill: false
          },
          {
            borderColor: "green",
            data: [],
            fill: false
          }
        ]},
        options: {}
      });


      function getData() {
        var req = new XMLHttpRequest();
        req.open("GET", "./logs/log.txt", false);
        req.send(null);

        log_text = document.getElementById("log_text")
        log_text.innerHTML = req.responseText;
        log_text.scrollTop = log_text.scrollHeight

        req.open("GET", "./logs/acc_x.txt", false);
        req.send(null);
        var arr = req.responseText.split("\n")
        chart["data"]["datasets"][0]["data"] = arr.slice(-101);
        req.open("GET", "./logs/acc_y.txt", false);
        req.send(null);
        var arr = req.responseText.split("\n")
        chart["data"]["datasets"][1]["data"] = arr.slice(-101);
        req.open("GET", "./logs/acc_z.txt", false);
        req.send(null);
        var arr = req.responseText.split("\n")
        chart["data"]["datasets"][2]["data"] = arr.slice(-101);

        chart.update()
      }
      setInterval(getData, 1000);
    </script>

  </body>
</html>
