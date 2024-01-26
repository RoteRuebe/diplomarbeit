
<html>
  <head>
    <style>
      #log {
        float: left;
        width: 500px;
        height: 800px;
        white-space: pre-line;
      }

      #chart {
        float: right;
        width: 100%;
        max-width: 700px;
      }
    </style>
  </head>


  <body>
    <p>
      Log Nachrichten des Roboters:
    </p>
    <textarea id="log_text" rows= 30, cols = 50> </textarea>
    
    <canvas id="chart"> </canvas>

    <script src = "https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js"> </script>
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
      setInterval(getData, 1000);
    </script>
  </body>
</html>
