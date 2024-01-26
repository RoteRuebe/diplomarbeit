
<html>
  <head>
    <style>
      #log {
        border: 1px outset black;
        position: relativ;
        left: 30px;
        width: 500px;
        height: 800px;
      }
    </style>
  </head>


  <body>
    <div id="log">
      <p id="log_text">
      </p>
    <script>
      function getData() {
        var req = new XMLHttpRequest();
        req.open("GET", "./log.txt", false);
        req.send(null);
        document.getElementById("log_text").innerHTML = req.responseText;
      }
      setInterval(getData, 1000);
    </script>
  </body>
</html>
