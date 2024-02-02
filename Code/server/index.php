<?php
  if ( $_SERVER["REQUEST_METHOD"] == "POST") {
    $msg = htmlspecialchars( $_POST["message"] );
    if (!empty($msg)) {
      $file = fopen("msg_to_robot.txt", "w");
      fwrite( $file, $msg);
      fclose( $file );
    }
  }
?>

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
        <input type="text" id="message" name="message" placeholder="Command to robot"> </input>
        <input type="submit" id="submit" name="submit" value="Send"> </input>
      </form>
    </div>

    <div id="column2" style="left: 33%" class="column">
      <canvas id="chart"> </canvas>
    </div>

    <div id="column3" style="left: 66%" class="column">
      <ul>
        <li> <div class="box" style="background-color: green"></div> &ensp; Robot connected </li>
        <li> <div class="box"></div> &ensp; Controller disconnected </li>
        <li> <div class="box"></div> &ensp; Camera disconnected </li>
      </ul>
    </div>

    <script>
      let xValues = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
      let yValues = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
      const chart = new Chart("chart", {
        type: "line",
        data: {
          labels: xValues,
          datasets: [{
            borderColor: "blue",
            data: yValues,
            fill: false
          }]
        },
        options: {}
      });

      function getData() {
        var req = new XMLHttpRequest();
        req.open("GET", "./log.txt", false);
        req.send(null);

        log_text = document.getElementById("log_text")
        log_text.innerHTML = req.responseText;
        log_text.scrollTop = log_text.scrollHeight

        req.open("GET", "./data.txt", false);
        req.send(null);
        var arr = req.responseText.split("\n")
        chart["data"]["datasets"][0]["data"] = arr.slice(-11);
        chart.update()
      }
      //setInterval(getData, 1000);
    </script>

  </body>
</html>
