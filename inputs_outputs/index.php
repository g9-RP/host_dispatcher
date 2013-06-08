<!DOCTYPE html>
<html>
  <head>
    <title>Wireless Cookie Jar</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <!-- Bootstrap -->
    <link href="css/bootstrap.min.css" rel="stylesheet" media="screen">
  </head>
  <body>

    <script src="js/bootstrap.min.js"></script>
    <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js"       type="text/javascript"></script> 
<script src="http://code.highcharts.com/highcharts.js"></script>
<script src="http://code.highcharts.com/modules/exporting.js"></script>

    <script type="text/javascript">
	$(function () {
        $('#container').highcharts({
            chart: {
                type: 'spline'
            },
            title: {
                text: 'Cookie Jar Level Over Time'
            },
            xAxis: {
                type: 'datetime'
       
            },
            yAxis: {
                title: {
                    text: 'Cookie Level (%)'
                },
                min: 0,
                max: 100
            },
          
            series: [{
                name: 'Cookie Jar',
                // Define the data points. All series have a dummy year
                // of 1970/71 in order to be compared on the same x axis. Note
                // that in JavaScript, months start at 0 for January, 1 for February etc.
                data: [
                    <?php
                    	require_once('include/database.php');
                    	$data = getDataForHC();

						for($i=0; $i < sizeof($data); $i++){
							if ($data[$i]['cookie_level'] != ""){
								if ($i != sizeof($data)-1){
									echo "[",$data[$i]['time_posted'], ", ", $data[$i]['cookie_level'], "], ";
								}
								else{
									echo "[",$data[$i]['time_posted'], ", ", $data[$i]['cookie_level'], "]";
								}
							}
						}
                    ?>
                ]
            }]
        });
    });
    

		</script>

    <div class="navbar navbar-inverse navbar-fixed-top">
      <div class="navbar-inner">
        <div class="container">
          <button type="button" class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse">
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="brand" href="index.php">Wireless Cookie Jar</a>
          <div class="nav-collapse collapse">
            <ul class="nav">
              <li class="active"><a href="#">Home</a></li>
              <li><a href="about.php">About</a></li>
            </ul>
          </div><!--/.nav-collapse -->
        </div>
      </div>
    </div>

    <div class="container">

      <!-- Main hero unit for a primary marketing message or call to action -->
      <div class="hero-unit">
        <h1>Wireless Cookie Jar</h1>
        <p>An embedded system project for ELEC3607 by Khanh Nguyen and Kevin Nguyen.</p>
        <p><a href="about.php" class="btn btn-primary btn-large">Learn more </a></p>
      </div>


      <div class="row">
	  <div class="span4">
	    <div class="mycontent-left well">
	    	<center><h2>Current Level</h2></center>
          <p><div id = "cookiepic" data-toggle="tooltip" title="first tooltip">

			<?php
				require_once('include/database.php');

				ini_set('display_errors', 'On');
				error_reporting(E_ALL);

				$latest = getLatestLevel();
				if($latest['cookie_level'] <= 5){
					echo "<img src = \"img/cookie0.png\"></img>";
				}
				elseif($latest['cookie_level'] <= 10){
					echo "<img src = \"img/cookie1.png\"></img>";
				}

				elseif($latest['cookie_level'] <= 20){
					echo "<img src = \"img/cookie2.png\"></img>";
				}

				elseif($latest['cookie_level'] <= 30){
					echo "<img src = \"img/cookie3.png\"></img>";
				}

				elseif($latest['cookie_level'] <= 40){
					echo "<img src = \"img/cookie4.png\"></img>";
				}

				elseif($latest['cookie_level'] <= 50){
					echo "<img src = \"img/cookie5.png\"></img>";
				}

				elseif($latest['cookie_level'] <= 60){
					echo "<img src = \"img/cookie6.png\"></img>";
				}

				elseif($latest['cookie_level'] <= 70){
					echo "<img src = \"img/cookie7.png\"></img>";
				}

				elseif($latest['cookie_level'] <= 80){
					echo "<img src = \"img/cookie8.png\"></img>";
				}

				elseif($latest['cookie_level'] <= 90){
					echo "<img src = \"img/cookie9.png\"></img>";
				}

				elseif($latest['cookie_level'] <= 100){
					echo "<img src = \"img/cookie10.png\"></img>";
				}
				echo "</div></p>";

				echo "<p>
          				<div class=\"progress progress-striped active\">
			  			<div class=\"bar\" style=\"width:",$latest['cookie_level'],"%;\"></div>
						</div>
					</p>
	      	<center><p><span class=\"badge badge-info\">Cookie Jar is currently at ",$latest['cookie_level'],"%</span></p></center>";

			?>
          <center><p><a class="btn btn-primary" href="index.php">Refresh </a></p></center>
	    </div>

	  </div>
	  <div class="span8">
	    <div class="mycontent-right well">
          <p><div id="container" style="min-width: 400px; height: 400px; margin: 0 auto"></div></p>
	    </div>
	  </div>
	</div>

      <hr>

      <footer>
        <center><p>Khanh Nguyen and Kevin Nguyen | ELEC3607 | Semester 1 | 2013</p></center>
      </footer>

    </div> <!-- /container -->



  </body>
</html>

